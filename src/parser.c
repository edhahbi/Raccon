#include "parser.h"



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
        ps_res->state = PARSER_INCOMPLETE;
    }
    else if(ps->buffer_ptr[ps->parser_index] != c){
        ps_res->state = PARSER_ERROR;
    }else{
        ps_res->state = PARSER_OK;
        ps->parser_index++;
    }
}

// be aware function introduces side effects (parser_index advancement)
int consume_integer(char c){
    if(ps->parser_index == ps->end_index){
        ps_res->state = PARSER_INCOMPLETE;
    }
    else if(!isdigit(c)){
        ps_res->state = PARSER_ERROR;
    }else{
        ps_res->state = PARSER_OK;
    }
    int integer = c - '0';
    ps->parser_index++;
    return integer;
}

void parse_crlf(){
    
    consume_char('\r');

    if(ps_res->state != PARSER_OK)
        return;

    consume_char('\n');
}

void parse_simple_string(){
    consume_char('+');

    size_t last_index = ps->parser_index;

    while (ps_res->state == PARSER_OK && ps->buffer_ptr[ps->parser_index] !='\r'){
        consume_char(ps->buffer_ptr[ps->parser_index]);
    }
        
    
    if(ps_res->state != PARSER_OK)
        return;
    

    parse_crlf();

    if(ps_res->state != PARSER_OK)
        return;
    

    arg arg = create_arg(RESP_STRING,ps->buffer_ptr + last_index ,ps->parser_index - last_index);
    push_arg(&ps_res->cmd,arg);
}

u32 parse_len(){
    u32 integer = 0;
    while (ps_res->state == PARSER_OK && ps->buffer_ptr[ps->parser_index] !='\r'){
        integer = (integer * 10) + consume_integer(ps->buffer_ptr[ps->parser_index]);
    }

    if(ps_res->state != PARSER_OK){
        return 0;
    }

    parse_crlf();

    if(ps_res->state != PARSER_OK){
        return 0;
    }

    return integer;
}

void parse_bulk_string(){

    consume_char('$');

    u32 len = parse_len();

    if(ps_res->state != PARSER_OK)
        return;

    size_t i = 0;
    size_t arg_offset = ps->parser_index;
    while (i<len && ps_res->state == PARSER_OK){
        consume_char(ps->buffer_ptr[ps->parser_index]);
        i++;
    }

    parse_crlf();

    if(ps_res->state != PARSER_OK)
        return;
    
    arg arg = create_arg(RESP_STRING,ps->buffer_ptr + arg_offset, len);
    push_arg(&ps_res->cmd,arg);
}

void parse_multi_bulk_string(){
    consume_char('*');
    u32 len = parse_len();

    if(ps_res->state != PARSER_OK)
        return;

    for (size_t i = 0; i < len; i++){
        parse_bulk_string();
        if(ps_res->state != PARSER_OK)
            return;
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
        ps_res->state = PARSER_ERROR;
        break;
    }
}

void handle_parser_result(buffer* incoming){
    if(ps_res->state == PARSER_INCOMPLETE)
        return;

    if(ps_res->state == PARSER_ERROR){
        skip_error();
    }

    buffer_sync(incoming,ps->parser_index);
}

parser_result parse(buffer* incoming){
    parser_ctx ps_ctx; 
    parser_ctx_init(&ps_ctx,incoming->ptr,incoming->offset);

    parser_result result;
    parser_result_init(&result);

    ps = &ps_ctx;
    ps_res = &result;

    parse_begin();

    handle_parser_result(incoming);

    parser_ctx_reset(ps);

    return result;
}


