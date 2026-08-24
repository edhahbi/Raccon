#include "parser.h"

/*
int64 :
simple string + 
bulk string $
double ,
*/

static inline char* get_current_ptr(){
    return ps->buffer_ptr + ps->parser_index;
}

static inline char get_current_char(){
    return ps->buffer_ptr[ps->parser_index];
}


bool valid_begin(){
    char c = get_current_char();
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
        ps_res->state = INCOMPLETE;
    }
    else if(get_current_char() != c){
        ps_res->state = ERROR;
    }else{
        ps_res->state = OK;
        ps->parser_index++;
    }
}

void parse_crlf(){
    
    consume_char('\r');

    if(ps_res->state != OK)
        return;

    consume_char('\n');
}

i64 parse_i64(){
    
    char* end_ptr;
    errno = 0;

    i64 int64 = strtoll(get_current_ptr(),&end_ptr,10);

    // overflow or no conversion took place
    if(errno == ERANGE || end_ptr == get_current_ptr()){
        ps_res->state = ERROR;
        return 0;
    }

    ps->parser_index = end_ptr - ps->buffer_ptr;    

    parse_crlf();

    if(ps_res->state != OK)
        return 0;
    
    return int64;
}

void parse_double(){

    consume_char(',');

    char* end_ptr;
    errno = 0;

    double result = strtod(get_current_ptr(),&end_ptr);

    // overflow or no conversion took place
    if(errno == ERANGE || end_ptr == get_current_ptr()){
        ps_res->state = ERROR;
        return;
    }

    ps->parser_index = end_ptr - ps->buffer_ptr;

    parse_crlf();

    if(ps_res->state != OK)
        return;

    arg arg = create_arg(RESP_DOUBLE,&result,0);
    push_arg(&ps_res->cmd,arg);
    return;

}

void parse_simple_string(){
    consume_char('+');
    
    size_t arg_begin = ps->parser_index;

    while (get_current_char()!='\r' && ps_res->state == OK)
        consume_char(get_current_char());

    size_t arg_end = ps->parser_index;

    parse_crlf();

    if(ps_res->state != OK)
        return;
    
    arg arg = create_arg(RESP_STRING,ps->buffer_ptr + arg_begin, arg_end - arg_begin);
    push_arg(&ps_res->cmd,arg);
}

void parse_bulk_string(){

    consume_char('$');

    size_t len = (size_t)parse_i64();

    if(ps_res->state != OK)
        return;

    size_t i = 0;
    size_t arg_begin = ps->parser_index;
    while (i<len && ps_res->state == OK){
        consume_char(get_current_char());
        i++;
    }
    
    if(ps_res->state != OK)
        return;
    
    parse_crlf();

    if(ps_res->state != OK)
        return;

    arg arg = create_arg(RESP_STRING,ps->buffer_ptr + arg_begin, len);   
    push_arg(&ps_res->cmd,arg);
}

void parse_argument(){
    if(ps->parser_index == ps->end_index){
        ps_res->state = INCOMPLETE;
        return;
    }

    char c = ps->buffer_ptr[ps->parser_index];
    switch (c){
        case ':':
            i64 int64 = parse_i64();

            if(ps_res->state != OK)
                return;

            arg a = create_arg(RESP_INT64, &int64,sizeof(i64));
            push_arg(&ps_res->cmd,a);
            break;

        case '+':
            parse_simple_string();
            break;
            
        case '$':
            parse_bulk_string();
            break;

        case ',':
            parse_double();
            break;

        default:
            LOG_ERROR("unhandled type");
            exit(1);
    }
}

void parse_multi_bulk_string(){

    consume_char('*');

    size_t len = (size_t)parse_i64();

    if(ps_res->state != OK)
        return;

    for (size_t i = 0; i < len; i++){
        parse_argument();
        if(ps_res->state != OK)
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
        ps_res->state = ERROR;
        break;
    }
}

void handle_parser_result(buffer* incoming){
    if(ps_res->state == INCOMPLETE)
        return;

    if(ps_res->state == ERROR){
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


