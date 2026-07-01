#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define WHITE_COLOR "\033[0m"
#define GREEN_COLOR "\033[0;32m"
#define CYAN_COLOR "\033[0;36m"
#define YELLOW_COLOR "\033[0;33m"
#define RED_COLOR "\033[0;31m"

#define TIME_HEADER_SIZE 512
#define BUFFER_SIZE 1024

typedef enum
{
    INFO,
    DEBUG,
    WARN,
    ERROR
} loglevel;

void debug(
    const char *fmt,
    ...);

void warn(
    const char *fmt,
    ...);

void err(
    const char *fmt,
    ...);

void info(
    const char *fmt,
    ...);
