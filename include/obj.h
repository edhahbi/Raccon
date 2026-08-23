#pragma once
#include "common.h"
#include "sdc.h"

// can't do forward declaration since the size is needed
// this is the same property struct defined in dictht so it's castable to property
typedef struct property property;

typedef struct object{
    property* properties;
    size_t size;
} object;

object* create_object(size_t, property* const);
