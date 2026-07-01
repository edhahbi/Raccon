#pragma once

#include "common.h"
#include "buff.h"
#include "conn_state.h"

typedef struct conn_ctx
{
    buff _inc_ctx;
    buff _out_ctx;
} conn_ctx_t;

typedef struct conn
{
    u32 _socketfd;
    u32 _flags;
    conn_ctx_t _ctx;
} conn_t;

void conn_init(conn_t *conn, u32 socketfd);
void conn_reset(conn_t *conn);
