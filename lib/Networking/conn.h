#pragma once

#include "common.h"
#include "socket.h"

typedef struct conn_ctx{
    u8* _inc_ctx;
    u8* _out_ctx; 
}conn_ctx_t;


typedef struct conn{
    u32 _flags;
    conn_ctx_t _ctx;
}conn_t;

void init_conn(conn_t* conn, u32 flags);
