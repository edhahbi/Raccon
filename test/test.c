#include "socket.h"
#include "evl.h"

int main(){

    socket_t s;
    socket_init(&s,"127.0.0.1",PORT,MAX_EVENTS);
    socket_listen(&s);
    event_loop_t evl;
    init_event_loop(&evl,&s,EPOLLIN | EPOLLOUT);
    start_event_loop(&evl);
    return 0;
}
