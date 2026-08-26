#pragma once
#include "conn.h"
#include "dict.h"

#define PONG_LEN 7
#define OK_LEN 5
#define NIL_LEN 5
#define PARSER_ERR_MSG_LEN 20
#define EXCUTOR_ERR_MSG_LEN 22
#define LEN_01 4
#define CMD_NUM 4

static const buffer *out;
static const command* cmd;

static inline token* get_command_arg(const size_t idx) {return cmd->argv + idx;}
static inline string *get_command_arg_string(const size_t idx){ return cmd->argv[idx].value;}
static inline i64 *get_command_arg_int64(const size_t idx){ return cmd->argv[idx].value;}
static inline bool *get_command_arg_boolean(const size_t idx){ return cmd->argv[idx].value;}
static inline token_type get_arg_type(const size_t idx){return cmd->argv[idx].type;}
static inline bool valid_command_begining() { return get_arg_type(0) == RESP_STRING; }

void exec_PING();
void exec_SET();
void exec_GET();
void exec_DEL();
cmd_result valid_ping_cmd();
cmd_result valid_set_cmd();
cmd_result valid_get_cmd();
cmd_result valid_del_cmd();

static cmd_result (*func_arr[])() = {valid_ping_cmd,valid_set_cmd, valid_get_cmd,valid_del_cmd};
cmd_state exec_cmd();
cmd_state exec(const command* cmd, const buffer* buffer);
