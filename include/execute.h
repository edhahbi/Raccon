#pragma once
#include "conn.h"

typedef enum exec_result{
    OK,
    INVALID
}exec_result;

void exec(command*, buffer*);
