#include "conn.h"

void conn_init(conn_t* conn, u32 socketfd){
    conn->_socketfd = socketfd;
    conn->_ctx._inc_ctx = buff_init();
    conn->_ctx._out_ctx = buff_init();
}

void conn_reset(conn_t* conn){
    conn->_socketfd = 0;
    buff_reset(&conn->_ctx._inc_ctx);
    buff_reset(&conn->_ctx._inc_ctx); 
}
