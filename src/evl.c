#include "evl.h"

static event_t ev_config;

int socket_add(event_loop_t* evl,u32 sockfd){
    ev_config.data.fd = sockfd;
    ev_config.events = evl->_flags;
    return epoll_ctl(evl->_epollfd,EPOLL_CTL_ADD,sockfd,&ev_config);
}

int socket_remove(event_loop_t* evl, u32 sockfd){
    return epoll_ctl(evl->_epollfd,EPOLL_CTL_DEL,sockfd,NULL);   
}

void init_event_loop(event_loop_t *evl, socket_t *listen_sock, u32 event_flags)
{
    if ((evl->_epollfd = epoll_create1(0)) == -1)
    {
        err("init_event_loop : epoll_create1 failure");
        exit(EXIT_FAILURE);
    }
    evl->_flags = event_flags;
    evl->_listensock = listen_sock;

    if(socket_add(evl,evl->_listensock->_sockfd) == -1){
        err("init_event_loop: socket_add failure");
        exit(EXIT_FAILURE);
    }

}

void handle_accept(event_loop_t* evl, size_t connIdx)
{
    int sockfd = evl->_events[connIdx].data.fd;
    conn_t* conn = &evl->_conns[connIdx];

    int client = socket_accept(sockfd);
    if(client <= 0){
        err("handle_accept: socket_accept failure");
        return;
    }

    if(socket_add(evl,client) == -1){
        err("handle_accept: add_socket failure");
        return;
    }
    conn_init(conn,client);

    info("socket %d with accepted successfully",client);
}

void try_handle_request(event_loop_t* evl, size_t connIdx){
    buff* inc = &evl->_conns[connIdx]._ctx._inc_ctx;
    // try parse command 
}

void handle_read(event_loop_t* evl,size_t connIdx){
    conn_t* conn = &evl->_conns[connIdx];
    Conn_state_t state = buff_read(conn->_socketfd,&conn->_ctx._inc_ctx);

    if(state == CONN_ERROR || state == CONN_CLOSED){
        close(conn->_socketfd);
        if(socket_remove(evl,conn->_socketfd) == -1){
            err("handle_read: socket_remove failure");
        }
        conn_reset(conn);
    }else{
        try_handle_request(evl,connIdx);
    }
}

void start_event_loop(event_loop_t *evl)
{
    for (;;)
    {
        u32 nfds = epoll_wait(evl->_epollfd, evl->_events, MAX_EVENTS, NO_TIMEOUT);

        for (u32 i = 0; i < nfds; i++)
        {
            int sockfd = evl->_events[i].data.fd;
            u32 flags = evl->_events[i].events;
            if (sockfd == evl->_listensock->_sockfd)
            {
                handle_accept(evl, i);
            }else{
                if(flags & EPOLLIN){
                    handle_read(evl,i);
                }else if(flags & EPOLLOUT){
                    //handle_write(evl, i);
                }
            }
            
        }
    }
}

