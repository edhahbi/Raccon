#pragma once

#include <sys/epoll.h>
#include "socket.h"
#include "conn.h"
#include "buff.h"

#define MAX_EVENTS 10
#define NO_TIMEOUT -1

typedef struct epoll_event event_t;

typedef struct event_loop_t
{
    i32 _epollfd;
    i32 _flags;
    socket_t *_listensock;
    event_t _events[MAX_EVENTS];
    conn_t _conns[MAX_EVENTS];
} event_loop_t;

void init_event_loop(event_loop_t *event_loop, socket_t *server_socket, u32 event_flags);
void start_event_loop(event_loop_t *event_loop);
void handle_accept(event_loop_t *evl, size_t connIdx);
// todo
// void handle_read(conn_t* connection);
// void handle_write(conn_t* connection);
// bool try_parse();
