#include "dispatcher.h"

void dispatch(conn_t* connection){
    parse(connection);
    if(connection->_parser_ctx.result == PARSER_OK){
        LOG_DEBUG("+OK\r\n");
    }
}
