#include "conn_ctx.h"

void conn_ctx_init(conn_ctx_t* conn_ctx){
    conn_ctx->incoming = buffer_init(DEFAULT_BUFFER_SIZE);
    conn_ctx->outcoming = buffer_init(DEFAULT_BUFFER_SIZE);
}

void conn_ctx_reset(conn_ctx_t* conn_ctx){
    buffer_reset(&conn_ctx->incoming);
    buffer_reset(&conn_ctx->outcoming); 
}
