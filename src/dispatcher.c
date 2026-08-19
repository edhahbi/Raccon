#include "dispatcher.h"

void handle_parsing_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,"-Err Parsing Error\r\n",21);
}

void handle_execution_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,"-Err Execution Error\r\n",23);
}

void dispatch(conn_t* connection){
    parser_result parser_result = parse(&connection->_conn_ctx.incoming);

    if(parser_result.state == PARSER_ERROR){
        command_cleanup(&parser_result.cmd);
        handle_parsing_error(connection);
        return;
    }
        
    if(parser_result.state == PARSER_INCOMPLETE)
        return;

    exec_result exec_result = exec(&parser_result.cmd, &connection->_conn_ctx.outcoming);
    
    if(exec_result == INVALID){
        command_cleanup(&parser_result.cmd);
        handle_execution_error(connection);
    }   
}


