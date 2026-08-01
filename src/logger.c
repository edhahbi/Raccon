#include "logger.h"

void Log(
    const char* file,
    const char* func,
    int line,
    const char* loglevel,
    const char* logcolor,
    const char *fmt,    
    ...)
{
    va_list args;
    va_start(args,fmt);

    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);

    char message[BUFFER_SIZE];

    vsnprintf(message, sizeof(message), fmt, args);

    char time_header[TIME_HEADER_SIZE];

    sprintf(
        time_header,
        "%04d/%02d/%02d %02d:%02d:%02d",
        local_time->tm_year + 1900,
        local_time->tm_mon + 1,
        local_time->tm_mday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec);

    printf("[%s] [%s:%s] [%d] [%s%s%s] %s\n",time_header, file, func,line,logcolor,loglevel,WHITE_COLOR,message);
    va_end(args);
}

