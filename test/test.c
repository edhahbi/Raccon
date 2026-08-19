#include "evl.h"

int main(){
    socket_t server_sock;
    socket_init(&server_sock,"127.0.0.1",8080,MAX_EVENTS);
    socket_listen(&server_sock);
    
    event_loop_t evl;
    init_event_loop(&evl,&server_sock,EPOLLIN | EPOLLOUT);
    start_event_loop(&evl);

    return 0;
}
