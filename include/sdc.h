#pragma once

#include "common.h"
#include <string.h>
#include <stdlib.h>

typedef struct string{
    char* ptr;
    size_t size;
}string;

string from_int_to_srt(u64 uinteger);

string sdc_init(const char* ptr,const size_t size);
void sdc_push(string* sdc, char c);
void sdc_free(string* sdc);
void sdc_merge(string* dest,string* const src);
