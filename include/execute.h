#pragma once
#include "conn.h"
#include "execution_ctx.h"
#include "consts.h"
#include "dict.h"

static buffer *out;
static execution_ctx exec_ctx;

static inline string *get_command_arg_string(const size_t idx){ return &exec_ctx.cmd->argv[idx].value.string;}
static inline u64 *get_command_arg_integer(const size_t idx){ return exec_ctx.cmd->argv[idx].value.integer;}
static inline arg_type get_arg_type(const size_t idx){return exec_ctx.cmd->argv[idx].type;}

exec_result exec(command*, buffer*);
