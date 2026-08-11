#pragma once
#include "command.h"

typedef enum exec_result{
    OK,
    INVALID
}exec_result;


typedef struct{
    command_type cmd_type;
    command* cmd;
    exec_result result;
}execution_ctx;


void excution_ctx_init(execution_ctx* execution_ctx, const command* cmd);
