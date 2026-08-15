#pragma once
#include "common.h"
#include "conn.h"
#include "buffer.h"
#include <ctype.h>

#define MESSAGE_LENGTH_MAX 1024

command parse(parser_ctx*, buffer*);

static parser_state* ps;
static command cmd;
