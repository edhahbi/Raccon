#pragma once
#include "obj.h" 
typedef union dict_value
{
    object* object_pt;
    u64* uint64_pt;
    i64* int64_pt;
    double* d_pt;
    string* str_pt;
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
