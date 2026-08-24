#pragma once
#include "conn.h"
#include "dict.h"

#define PONG_LEN 7
#define OK_LEN 6
#define NIL_LEN 6
#define PARSER_ERR_MSG_LEN 21
#define EXCUTOR_ERR_MSG_LEN 23
#define LEN_01 5

static const char* parser_err_msg = "-Err Parsing Error\r\n";
static const char* executor_err_msg = "-Err Execution Error\r\n";
static const char* one_msg = "+1\r\n";
static const char* zero_msg = "+0\r\n";

static const buffer *out;
static const command* cmd;

static inline arg* get_command_arg(const size_t idx) {return cmd->argv + idx;}
static inline string *get_command_arg_string(const size_t idx){ return &cmd->argv[idx].value->str;}
static inline i64 *get_command_arg_int64(const size_t idx){ return &cmd->argv[idx].value->int64;}
static inline bool *get_command_arg_boolean(const size_t idx){ return &cmd->argv[idx].value->b;}

static inline arg_type get_arg_type(const size_t idx){return cmd->argv[idx].type;}

state exec(const command*, const buffer*);
