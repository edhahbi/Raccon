#pragma once

#include <ctype.h>
#include <string.h>
#include "sdc.h"
#include "command.h"
#include "parser_state.h"

typedef struct parser_ctx{
    size_t parser_index;
    size_t end_index;
    char* buffer_ptr;
}parser_ctx;

typedef struct parser_result{
    command cmd;
    parser_state state;
}parser_result;

void parser_ctx_init(parser_ctx* parser_state ,char* b_ptr, size_t end_idx);
void parser_ctx_reset(parser_ctx*);
void parser_result_init(parser_result* parser_result);

