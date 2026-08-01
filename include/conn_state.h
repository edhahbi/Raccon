#pragma once

typedef enum conn_state
{
    CONN_CLOSED,// connection closed
    CONN_WAIT,  // no more data in the socket in the case of recv and socket buffer full in the case of send
    CONN_ERROR  // socket error
} conn_state_t;
