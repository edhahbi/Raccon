#include "parser.h"

static parser_ctx* ps;

static size_t parser_index;
static size_t end_index;
static char* buffer_ptr;

bool valid_begin(){
    char c = buffer_ptr[parser_index];
    return c=='$' || c=='+' || c=='*';
}

void skip_error(){
    while (parser_index < end_index && !valid_begin()){
        parser_index++;
    }
}

token create_token(token_type token_type, void* token_val, size_t token_size){
    token t = (token){.type = token_type};
    switch (token_type)
    {
    case INTEGER:
        memcpy(&t.token_value.integer, token_val, sizeof(u32));
        break;
    
    case STRING:
        t.token_value.string = sdc_init(token_val,token_size);
        break;

    default:
        LOG_ERROR("Invalid Token type");
        exit(EXIT_FAILURE);
        break;
    }
    return t;
}

void parse_crlf(){
    if(parser_index == end_index){
        ps->result = PARSER_INCOMPLETE;
    }else if(buffer_ptr[parser_index] != '\n'){
        ps->result = PARSER_ERROR;
    }else{
        parser_index++;
        ps->result = PARSER_OK;
    }
}

void parse_simple_string(){
    u32 last_index = parser_index;

    while (parser_index < end_index && buffer_ptr[parser_index] !='\r')
        parser_index++;
    
    if(parser_index == end_index){
        ps->result = PARSER_INCOMPLETE;
    }else{
        parser_index++;
        parse_crlf();
        if(ps->result == PARSER_OK){
            token tt = create_token(STRING,buffer_ptr + last_index ,parser_index - last_index);
            token_array_push(&ps->tokens_array,tt);
        }
    }
}

u32 parse_integer(){
    u32 integer = 0;
    while (parser_index < end_index && isdigit(buffer_ptr[parser_index])){
        integer = (integer * 10) + (buffer_ptr[parser_index] - '0');
        parser_index++;
    }

    if(parser_index == end_index){
        ps->result = PARSER_INCOMPLETE;
    }else if(buffer_ptr[parser_index] == '\r'){
        parser_index++;
        parse_crlf();
    }else{
        ps->result = PARSER_ERROR;
    }

    return integer;
}

void parse_bulk_string(){
    u32 len = parse_integer();
    if(ps->result == PARSER_OK){
        if(parser_index + len > end_index){
            ps->result = PARSER_INCOMPLETE;
        }else if(buffer_ptr[parser_index + len - 1] != '\r'){
            ps->result = PARSER_ERROR;
        }else if(ps->result == PARSER_OK){
            parser_index += len;
            parse_crlf();
            if(ps->result == PARSER_OK){
                token tt = create_token(STRING,buffer_ptr + parser_index - len,len);
                token_array_push(&ps->tokens_array, tt);
            }
        }
    }
}

void parse_multi_bulk_string(){
    u32 len = parse_integer();
    if(ps->result == PARSER_OK){
        size_t i = 0;
        while(i < len && ps->result == PARSER_OK){
            parse_bulk_string();
        }
    }
}

void __parse(){
    char c = buffer_ptr[parser_index];
    parser_index++;
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


void handle_state(conn_t* connection){
    if(ps->result == PARSER_ERROR){
        skip_error();
        buffer_sync(&connection->_conn_ctx.incoming,parser_index);
    }else if(ps->result == PARSER_OK){
        buffer_sync(&connection->_conn_ctx.incoming,parser_index);
    }
}

void parse(conn_t* connection){
    buffer* incoming = &connection->_conn_ctx.incoming;
    ps = &connection->_parser_ctx;
    
    parser_index = 0;
    end_index = incoming->offset;
    buffer_ptr = incoming->ptr;

    __parse();
    handle_state(connection);
}


