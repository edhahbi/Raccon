#pragma once
#include "common.h"

typedef enum conn_state
{
    CONN_CLOSED,// connection closed
    CONN_WAIT,  // no more data in the socket in the case of recv and socket buffer full in the case of send
    CONN_ERROR  // socket error
} conn_state;
 
typedef enum parser_state{
    PARSER_OK,
    PARSER_INCOMPLETE,
    PARSER_ERROR
}parser_state;

typedef enum cmd_state{
    CMD_OK,
    CMD_INCOMPLETE,
    CMD_ERROR,
    CMD_NOT_CMD
}cmd_state;
