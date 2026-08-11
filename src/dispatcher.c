#include "dispatcher.h"

void handle_parsing_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,"-Err Parsing Error\r\n",21);
}

void handle_execution_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,"-Err Execution Error\r\n",23);
}

void dispatch(conn_t* connection){
    command cmd = parse(&connection->_parser_ctx, &connection->_conn_ctx.incoming);
    parser_result parser_result = connection->_parser_ctx.result;


    if(parser_result == PARSER_ERROR){
        handle_parsing_error(connection);
        return;
    }
        
    if(parser_result == PARSER_INCOMPLETE)
        return;

    exec_result exec_result = exec(&cmd, &connection->_conn_ctx.outcoming);
    
    if(exec_result == INVALID)
        handle_execution_error(connection);
}


