#pragma once
#include "logger.h"
#include <stdbool.h>
#include <pci/types.h>
#include <errno.h>

#define MIN(x,y) (x)>(y)?(y):(x)
#define MAX(x,y) (x)>(y)?(x):(y)

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
