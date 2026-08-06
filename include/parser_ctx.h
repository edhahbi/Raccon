#pragma once

#include <ctype.h>
#include <string.h>
#include "sdc.h"
#include "command.h"
#include "parser_state.h"

typedef struct parser_ctx{
    command command;
    parser_result result;
    parser_state state;
}parser_ctx;

void parser_ctx_init(parser_ctx* parser_ctx);
