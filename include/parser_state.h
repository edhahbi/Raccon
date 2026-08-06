#pragma once
#include "common.h"

typedef enum parser_result{
    PARSER_OK,
    PARSER_INCOMPLETE,
    PARSER_ERROR
}parser_result;

typedef struct parser_state{
    size_t parser_index;
    size_t end_index;
    char* buffer_ptr;
    parser_result result;
}parser_state;

void parser_state_init(parser_state* parser_state ,char* b_ptr, u32 end_idx);
void parser_state_reset(parser_state* parser_state);
