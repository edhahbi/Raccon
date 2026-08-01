#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define WHITE_COLOR "\033[0m"
#define GREEN_COLOR "\033[0;32m"
#define CYAN_COLOR "\033[0;36m"
#define YELLOW_COLOR "\033[0;33m"
#define RED_COLOR "\033[0;31m"

#define TIME_HEADER_SIZE 128
#define BUFFER_SIZE 1024

void Log(
    const char* file,
    const char* func,
    int line,
    const char* loglevel,
    const char* logcolor,
    const char *fmt,    
    ...);

#define LOG_DEBUG(fmt,...) \
    Log(__FILE__,__func__,__LINE__,"DBG",CYAN_COLOR,fmt __VA_OPT__(,) __VA_ARGS__)

#define LOG_INFO(fmt,...) \
    Log(__FILE__,__func__,__LINE__,"INF",GREEN_COLOR,fmt __VA_OPT__(,) __VA_ARGS__)

#define LOG_WARN(fmt,...) \
    Log(__FILE__,__func__,__LINE__,"WRN",YELLOW_COLOR,fmt __VA_OPT__(,) __VA_ARGS__)

#define LOG_ERROR(fmt,...) \
    Log(__FILE__,__func__,__LINE__,"ERR",RED_COLOR,fmt __VA_OPT__(,) __VA_ARGS__)

