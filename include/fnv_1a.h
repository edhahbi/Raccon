#pragma once

#include "common.h"

#define OFFSET 14695981039346656037ULL
#define PRIME 1099511628211ULL

typedef struct dict_key
{
    void *value;
    size_t len;
} dict_key;

size_t fnv_1a(dict_key key);
