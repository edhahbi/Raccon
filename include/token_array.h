#pragma once
#include <stdlib.h>
#include "common.h"
#include "token.h"
#define DEFAULT_TOKEN_ARRAY_SIZE 64
#define DEFAULT_GROWTH_FACTOR 2 

typedef struct token_array{
    token* tokens;
    size_t offset;
    size_t capacity;
}token_array;

void token_array_init(token_array* array, size_t capacity);
void token_array_push(token_array* array,token token);
void token_array_reset(token_array* array);
void token_array_resize(token_array* array,size_t new_capacity);
