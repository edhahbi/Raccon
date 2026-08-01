#include "parser_ctx.h"

void parser_ctx_init(parser_ctx* parser_ctx){
    parser_ctx->result = PARSER_OK;
    token_array_init(&parser_ctx->tokens_array,DEFAULT_TOKEN_ARRAY_SIZE);
}

void parser_ctx_reset(parser_ctx* parser_ctx){
    parser_ctx->result = PARSER_OK;
    token_array_reset(&parser_ctx->tokens_array);
}
