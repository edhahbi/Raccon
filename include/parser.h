#pragma once
#include "common.h"
#include "conn.h"
#include "buffer.h"
#include "parser_ctx.h"
#include <ctype.h>

#define MESSAGE_LENGTH_MAX 1024

parser_result parse(buffer*);

