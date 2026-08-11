#pragma once
#include "sdc.h"
#define MAX_RESP_COMMAND_ARGS 4

typedef enum command_type{
    PING,
    SET,
    GET,
    DEL
}command_type;

typedef enum arg_type
{
    INTEGER,
    STRING
} arg_type;

typedef union arg_value
{
    size_t integer;
    string string;
} arg_value;

typedef struct arg
{
    arg_type type;
    arg_value value;
} arg;

typedef struct command
{
    int argc;
    arg argv[MAX_RESP_COMMAND_ARGS];
} command;

static inline void init_command(command* command){ command->argc = 0;}
arg create_arg(arg_type arg_type, void *arg_val, size_t arg_size);
void push_arg(command *command, arg arg);
