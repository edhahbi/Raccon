#pragma once
#include "obj.h" 
typedef union dict_value
{
    object object;
    u64 uint64;
    i64 int64;
    double d;
    string str;
} dict_value;

typedef enum dict_type
{
    OBJECT,
    UINT,
    STRING
} dict_type;

typedef struct dict_tv
{
    dict_type type;
    dict_value value;
} dict_tv;
