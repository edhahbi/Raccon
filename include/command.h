#pragma once
#include "sdc.h"
#include "obj.h"

typedef enum command_type{
    PING,
    SET,
    GET,
    DEL
}command_type;

typedef enum arg_type
{
    RESP_BOOLEAN,
    RESP_INT64,
    RESP_STRING,
    RESP_OBJECT,
    RESP_DOUBLE
} arg_type;

typedef union arg_value
{
    object object;
    i64 int64;
    string str;
    bool b;
    double d;
} arg_value;

typedef struct arg
{
    arg_type type;
    arg_value* value;
} arg;

typedef struct command
{
    size_t argc;
    arg* argv;
} command;

void command_init(command*);
void command_cleanup(command*);
arg create_arg(arg_type, void *, size_t);
arg* create_arg_obj(const property* const ps, size_t);
void push_arg(command *, arg);
