#pragma once

#include <sys/epoll.h>
#include "socket.h"
#include "conn.h"

#define MAX_EVENTS 10
#define NO_TIMEOUT -1

typedef struct epoll_event event_t;

typedef struct event_loop{
    int _listensockfd;
    int _epollfd;
    event_t _events[MAX_EVENTS];
    conn_t _conns[MAX_EVENTS];
}event_loop_t;

void init_event_loop(event_loop_t* event_loop ,socket_t* server_socket, u32 event_flags);
void start_event_loop(event_loop_t* event_loop);
void handle_accept(event_t* event, conn_t* conn);

//todo
//void handle_read(conn_t* connection);
//void handle_write(conn_t* connection);
//bool try_parse();
