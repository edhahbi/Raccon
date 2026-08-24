#include "dispatch.h"

void handle_parsing_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,PARSER_ERR_MSG_LEN,"%s",parser_err_msg);
}

void handle_execution_error(conn_t* connection){
    buffer_push(&connection->_conn_ctx.outcoming,EXCUTOR_ERR_MSG_LEN, "%s", executor_err_msg);
}

void dispatch(conn_t* connection){
    parser_result parser_result = parse(&connection->_conn_ctx.incoming);

    if(parser_result.state == ERROR){
        command_cleanup(&parser_result.cmd);
        handle_parsing_error(connection);
        return;
    }
        
    // TODO: do ressource cleanup 
    if(parser_result.state == INCOMPLETE){
        command_cleanup(&parser_result.cmd);
        return;
    }


    state exec_result = exec(&parser_result.cmd, &connection->_conn_ctx.outcoming);
    
    if(exec_result == ERROR){
        command_cleanup(&parser_result.cmd);
        handle_execution_error(connection);
    }   
}


