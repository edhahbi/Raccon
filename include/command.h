#pragma once
#include "sdc.h"

typedef enum command_type{
    PING,
    SET,
    GET,
    DEL
}command_type;

typedef enum arg_type
{
    RESP_INTEGER,
    RESP_STRING
} arg_type;

typedef union arg_value
{
    u64 integer;
    string string;
} arg_value;

typedef struct arg
{
    arg_type type;
    arg_value value;
} arg;

typedef struct command
{
    size_t argc;
    arg* argv;
} command;

void command_init(command*);
void command_cleanup(command*);
arg create_arg(arg_type, void *, size_t);
void push_arg(command *, arg);
