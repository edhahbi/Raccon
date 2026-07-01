#pragma once
#include "common.h"
#include "conn_state.h"
#include <sys/socket.h>
#include <stdlib.h>

#ifndef DEFAULT_BUFF_SIZE
#define DEFAULT_BUFF_SIZE 32
#endif

#ifndef DEFAULT_GROWTH_FACTOR
#define DEFAULT_GROWTH_FACTOR 2
#endif

typedef struct
{
    char *ptr;
    size_t tail;
    size_t capacity;
} buff;

buff buff_init();
Conn_state_t buff_read(u32 socketfd, buff *buff);
void buff_reset(buff *buff);
