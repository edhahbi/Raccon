#pragma once
#include "common.h"
#include "command.h"
#include "sdc.h"
#include "state.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 32
#endif

#ifndef DEFAULT_GROWTH_FACTOR
#define DEFAULT_GROWTH_FACTOR 2
#endif

typedef struct
{
    char *ptr;
    size_t offset;
    size_t capacity;
} buffer;

buffer buffer_init(size_t size);
void buffer_free(buffer *buff);
static inline size_t buffer_remaining(const buffer *buff) { return buff->capacity - buff->offset; }
static inline size_t buffer_consumed(const buffer *buff) { return buff->offset; }
static inline void buffer_reset(buffer *buff) { buff->offset = 0; }

conn_state buffer_read(int sockfd, buffer *const buff);
conn_state buffer_write(int sockfd, buffer *const buff);
void buffer_sync(buffer *const buff, size_t ps_index);
void buffer_push(buffer *const buff, const size_t size, const char *format, ...);
