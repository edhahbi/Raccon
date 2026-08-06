#include "parser_state.h"

void parser_state_init(parser_state* parser_state ,char* b_ptr, u32 end_idx){
    parser_state->parser_index = 0;
    parser_state->end_index = end_idx;
    parser_state->buffer_ptr = b_ptr;
    parser_state->result = PARSER_OK;
}

void parser_state_reset(parser_state* parser_state){
    parser_state->parser_index = parser_state->end_index = 0;
    parser_state->buffer_ptr = NULL;
    parser_state = PARSER_OK;
}
