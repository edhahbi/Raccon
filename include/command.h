#pragma once
#include "sdc.h"
#include "obj.h"
#include "state.h"

typedef enum token_type
{
    RESP_BOOLEAN = 1,
    RESP_INT64 = 2,
    RESP_STRING = 4,
    RESP_OBJECT = 8,
    RESP_DOUBLE = 16
} token_type;

typedef struct token
{
    token_type type;
    void* value;
} token;

typedef struct command
{
    size_t argc;
    token* argv;
} command;

typedef struct cmd_result {
    cmd_state state;
    void (*exec_fn)();
}cmd_result;

void command_init(command*);
void command_cleanup(command*);
void command_dispose(command*);
token create_token(token_type, void *, size_t);
token* create_arg_obj(const property* const ps, size_t);
void push_arg(command *, token);
