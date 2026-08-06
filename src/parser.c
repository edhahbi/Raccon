#include "parser.h"

parser_state* ps;
command* cmd;

bool valid_begin(){
    char c = ps->buffer_ptr[ps->parser_index];
    return c=='$' || c=='+' || c=='*';
}

void skip_error(){
    while (ps->parser_index < ps->end_index && !valid_begin()){
        ps->parser_index++;
    }
}

// be aware function introduces side effects (parser_index advancement)
void consume_char(char c){
    if(ps->parser_index == ps->end_index){
        ps->result = PARSER_INCOMPLETE;
    }
    else if(ps->buffer_ptr[ps->parser_index] != c){
        ps->result = PARSER_ERROR;
    }else{
        ps->result = PARSER_OK;
        ps->parser_index++;
    }
}

// be aware function introduces side effects (parser_index advancement)
int consume_integer(char c){
    if(ps->parser_index == ps->end_index){
        ps->result = PARSER_INCOMPLETE;
    }
    else if(!isdigit(c)){
        ps->result = PARSER_ERROR;
    }else{
        ps->result = PARSER_OK;
    }
    int integer = c - '0';
    ps->parser_index++;
    return integer;
}

void parse_crlf(){
    
    consume_char('\r');

    if(ps->result != PARSER_OK)
        return;

    consume_char('\n');
}

void parse_simple_string(){
    consume_char('+');

    u32 last_index = ps->parser_index;

    while (ps->result == PARSER_OK && ps->buffer_ptr[ps->parser_index] !='\r'){
        consume_char(ps->buffer_ptr[ps->parser_index]);
    }
        
    
    if(ps->result != PARSER_OK)
        return;
    

    parse_crlf();

    if(ps->result != PARSER_OK)
        return;
    

    arg arg = create_arg(STRING,ps->buffer_ptr + last_index ,ps->parser_index - last_index);
    push_arg(cmd,arg);
}

u32 parse_len(){
    u32 integer = 0;
    while (ps->result == PARSER_OK){
        integer = (integer * 10) + consume_integer(ps->buffer_ptr[ps->parser_index]);
    }

    if(ps->result != PARSER_OK){
        return 0;
    }

    parse_crlf();

    if(ps->result != PARSER_OK){
        return 0;
    }

    return integer;
}

void parse_bulk_string(){

    consume_char('$');

    u32 len = parse_len();
    if(ps->result != PARSER_OK)
        return;

    size_t i = 0;
    size_t arg_offset = ps->parser_index;
    while (i<len && ps->result == PARSER_OK){
        consume_char(ps->buffer_ptr[ps->parser_index]);
        i++;
    }
    
    if(ps->result != PARSER_OK)
        return;

    parse_crlf();

    if(ps->result != PARSER_OK)
        return;
    
    arg arg = create_arg(STRING,ps->buffer_ptr + arg_offset, len);
    push_arg(cmd,arg);
}

void parse_multi_bulk_string(){
    consume_char('*');
    u32 len = parse_len();

    if(ps->result != PARSER_OK)
        return;
    
    if(len > MAX_RESP_COMMAND_ARGS){
        ps->result = PARSER_ERROR;
        return;         
    }

    size_t i = 0;
    while(i < len && ps->result == PARSER_OK){
        parse_bulk_string();
        i++;
    }
}

void parse_begin(){
    char c = ps->buffer_ptr[ps->parser_index];
    switch (c)
    {
    case '+' :
        parse_simple_string();
        break;

    case '$':
        parse_bulk_string();
        break;

    case '*':
        parse_multi_bulk_string();
        break;
    default:
        break;
    }
}

void handle_parser_result(buffer* incoming){
    if(ps->result == PARSER_INCOMPLETE)
        return;

    if(ps->result == PARSER_ERROR){
        skip_error();
    }

    buffer_sync(incoming,ps->parser_index);
}

void parse(parser_ctx* parser_ctx, buffer* incoming){
    ps = &parser_ctx->state;
    cmd = &parser_ctx->command;

    parser_state_init(
        ps,
        incoming->ptr,
        incoming->offset);

    parse_begin();

    handle_parser_result(incoming);

    parser_state_reset(ps);
}


