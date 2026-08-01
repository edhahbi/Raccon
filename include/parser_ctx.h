#pragma once

#include <ctype.h>
#include <string.h>
#include "token_array.h"

typedef enum parser_result_t{
    PARSER_OK,
    PARSER_INCOMPLETE,
    PARSER_ERROR
}parser_result_t;

typedef struct parser_ctx{
    token_array tokens_array;
    parser_result_t result;
}parser_ctx;

void parser_ctx_init(parser_ctx* parser_ctx);
void parser_ctx_reset(parser_ctx* parser_ctx);
