#include "dispatcher.h"



void dispatch(conn_t* connection){
    parse(&connection->_parser_ctx, &connection->_conn_ctx.incoming);

    if(connection->_parser_ctx.result == PARSER_ERROR){
        //TODO handle parsing errors here
        return;
    }

    if(connection->_parser_ctx.result == PARSER_INCOMPLETE)
        return;
    

    exec(&connection->_parser_ctx.command, &connection->_conn_ctx.outcoming);
    
}
