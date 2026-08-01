#pragma once

#include "common.h"
#include <string.h>
#include <stdlib.h>

typedef struct string{
    char* ptr;
    size_t size;
}string;


string sdc_init(const char* ptr, size_t size);
void sdc_join(string* sdc,const char* ptr, size_t size);
void sdc_free(string* sdc);
