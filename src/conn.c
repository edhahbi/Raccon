#include "conn.h"

void conn_init(conn_t* conn){
    conn->_inuse = true;
    conn_ctx_init(&conn->_conn_ctx);
    parser_ctx_init(&conn->_parser_ctx); 
}

void conn_reset(conn_t* conn){
    conn->_inuse = false;
    conn_ctx_reset(&conn->_conn_ctx);
    parser_ctx_init(&conn->_parser_ctx);
}
