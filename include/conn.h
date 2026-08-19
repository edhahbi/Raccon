#pragma once

#include "common.h"
#include "conn_ctx.h"
#include "parser_ctx.h"

typedef struct conn
{
    bool _inuse; // for later optimisation: reuse the same connection instead of creating another one
    conn_ctx _conn_ctx;
} conn_t;

void conn_init(conn_t *conn);
void conn_reset(conn_t *conn);
