#pragma once
#include "sdc.h"
#include "obj.h"
#include "state.h"

typedef enum token_type
{
    RESP_BOOLEAN,
    RESP_INT64,
    RESP_STRING,
    RESP_OBJECT,
    RESP_DOUBLE
} token_type;

typedef union token_value
{
    object object;
    i64 int64;
    string str;
    bool b;
    double d;
} token_value;

typedef struct token
{
    token_type type;
    token_value* value;
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
