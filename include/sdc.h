#pragma once

#include "common.h"
#include <string.h>
#include <stdlib.h>

typedef struct string{
    char* ptr;
    size_t size;
}string;

string from_int_to_str(i64);
string from_uint_to_str(u64);
string sdc_init(const char* ptr,const size_t size);
string* sdc_init1(const char* ptr, const size_t size);
void sdc_push(string* sdc, char c);
void sdc_free(string* sdc);
void sdc_merge(string* dest,const string* src);
