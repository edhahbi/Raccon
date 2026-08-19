#pragma once
#include "buffer.h"

typedef struct conn_ctx
{
    buffer incoming; // incoming data
    buffer outcoming; // outcoming data
} conn_ctx;

void conn_ctx_init(conn_ctx* conn_ctx);
void conn_ctx_reset(conn_ctx* conn_ctx);
