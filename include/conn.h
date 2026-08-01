#pragma once

#include "common.h"
#include "conn_ctx.h"
#include "parser_ctx.h"

typedef struct conn
{
    bool _inuse; // for later optimisation:
    conn_ctx_t _conn_ctx;
    parser_ctx _parser_ctx;
} conn_t;

void conn_init(conn_t *conn);
void conn_reset(conn_t *conn);
