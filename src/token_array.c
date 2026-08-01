#include "token_array.h"

void token_array_init(token_array* array ,size_t capacity){
    token* ptr = malloc(capacity * sizeof(token));
    
    if(ptr == NULL){
        LOG_ERROR("couldn't allocate memory for array");
        exit(EXIT_FAILURE);
    }
    
    array->offset = 0;
    array->capacity = capacity;
    array->tokens = ptr;
}

void token_array_resize(token_array* array,size_t new_capacity){
    array->tokens = realloc(array->tokens, new_capacity);
    if(array->tokens == NULL){
        LOG_ERROR("Invalid Memory Allocation");
        exit(EXIT_FAILURE);
    }
    array->capacity = new_capacity;
}

void token_array_push(token_array* array,token token){
    if(array->offset == array->capacity){
        token_array_resize(array,array->capacity * DEFAULT_GROWTH_FACTOR);
    }

    array->tokens[array->offset] = token;
    array->offset++;
}

void reset(token_array* array){
    array->offset = 0;
    array->capacity = 0;
    if(array->capacity > DEFAULT_TOKEN_ARRAY_SIZE){
        array = realloc(array,sizeof(token) * DEFAULT_TOKEN_ARRAY_SIZE);
    }
}

void token_array_reset(token_array* array){
    array->tokens = realloc(array->tokens,DEFAULT_TOKEN_ARRAY_SIZE);
    array->offset = 0;
}
