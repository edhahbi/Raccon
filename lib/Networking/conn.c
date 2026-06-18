#include "conn.h"

void init_conn(conn_t* conn, u32 flags){
    conn->_flags = flags;
    conn->_ctx._inc_ctx = malloc(sizeof(1024));
    conn->_ctx._out_ctx = malloc(sizeof(1024));
}
