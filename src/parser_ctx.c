#include "parser_ctx.h"

void parser_ctx_init(parser_ctx* parser_state ,char* b_ptr, size_t end_idx){
    parser_state->parser_index = 0;
    parser_state->end_index = end_idx;
    parser_state->buffer_ptr = b_ptr;
}

void parser_ctx_reset(parser_ctx* parser_state){
    parser_state->parser_index = 0;
    parser_state->end_index = 0;
    parser_state->buffer_ptr = NULL;
}

void parser_result_init(parser_result* parser_result){
    command_init(&parser_result->cmd);
}
