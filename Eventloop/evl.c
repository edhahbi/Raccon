#include "evl.h"

void init_event_loop(event_loop_t* event_loop ,socket_t* listen_sock,u32 event_flags){
    
    if((event_loop->_epollfd = epoll_create1(0)) == -1){
        err("epoll_create1 failure");
        exit(EXIT_FAILURE);
    }

    event_t events_config;
    events_config.events = event_flags;
    events_config.data.fd = listen_sock->_sockfd;

    if(epoll_ctl(event_loop->_epollfd,EPOLL_CTL_ADD,listen_sock->_sockfd,&events_config) == -1){
        err("epoll_ctl failure");
        exit(EXIT_FAILURE);
    }


}

void start_event_loop(event_loop_t* event_loop){
    for(;;){
        u32 nfds = epoll_wait(event_loop->_epollfd,event_loop->_events,MAX_EVENTS,NO_TIMEOUT);
        
        for(u32 i = 0; i<nfds; i++){
            u32 sockfd = event_loop->_events[i].data.fd;
            
            if(sockfd == event_loop->_listensockfd){
                handle_accept(&event_loop->_events[i], &(event_loop->_conns[i]));
            }else{
                info("client concerns are empty for now");
            }
        }
    }
}

void handle_accept(event_t* event , conn_t* conn){
    init_conn(conn,event->events);
    
    struct sockaddr_in client_addr; 
    socklen_t client_addr_len;
    accept4(event->data.fd,(struct sockaddr*)&client_addr,&client_addr_len, SOCK_NONBLOCK);
    sleep(1);
    get_addr_info(&client_addr);
}
