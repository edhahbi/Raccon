#pragma once
#define INVALID_LENGTH 0
#include "common.h"
#include "sdc.h"

typedef enum token_type{
    INTEGER,
    STRING,
    BULK_LEN,
    MULTIBULK_LEN
}token_type;

typedef struct token{
    token_type type;
    union token_value
    {
        u32 integer;
        string string;
    }token_value;
}token;
