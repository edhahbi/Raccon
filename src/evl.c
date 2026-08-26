#include "evl.h"
#include "dispatch.h"

static event_t ev_config;

int socket_add(const event_loop_t *evl, u32 sockfd)
{
    ev_config.data.fd = sockfd;
    ev_config.events = evl->_flags;
    return epoll_ctl(evl->_epollfd, EPOLL_CTL_ADD, sockfd, &ev_config);
}

int socket_remove(const event_loop_t *evl, u32 sockfd)
{
    return epoll_ctl(evl->_epollfd, EPOLL_CTL_DEL, sockfd, NULL);
}

void init_event_loop(event_loop_t *evl, socket_t *listen_sock, u32 event_flags)
{
    if ((evl->_epollfd = epoll_create1(0)) == -1)
    {
        LOG_ERROR("failed to create epoll instance with epoll_create1");
        exit(EXIT_FAILURE);
    }
    evl->_flags = event_flags;
    evl->_listensock = listen_sock;
    if (socket_add(evl, evl->_listensock->_sockfd) == -1)
    {
        LOG_ERROR("failed to register listening socket %d with epoll", evl->_listensock->_sockfd);
        exit(EXIT_FAILURE);
    }
}

void handle_accept(event_loop_t *evl, int sockfd)
{
    int client = socket_accept(sockfd);
    if (client < 0)
    {
        LOG_ERROR("failed to accept a client connection from listening socket %d", sockfd);
        return;
    }

    conn_t *conn = &evl->_conns[client];
    conn_init(conn);

    if (socket_add(evl, client) == -1)
    {
        LOG_ERROR("failed to register accepted client socket %d with epoll", client);
        return;
    }

    LOG_INFO("accepted client socket %d and added it to the connection table", client);
}

void handle_request(event_loop_t *evl, int sockfd)
{
    conn_t *conn = &evl->_conns[sockfd];
    conn_state state = buffer_read(sockfd, &conn->_conn_ctx.incoming);

    if (state == CONN_ERROR || state == CONN_CLOSED)
    {
        if (socket_remove(evl, sockfd) == -1)
        {
            LOG_ERROR("failed to remove socket %d from epoll after read state %d", sockfd, state);
        }
        close(sockfd);
        conn_reset(conn);
    }
    else
    {
        dispatch(conn);
    }
}

void handle_response(event_loop_t *evl, int sockfd)
{
    conn_t *conn = &evl->_conns[sockfd];
    conn_state state = buffer_write(sockfd, &conn->_conn_ctx.outcoming);

    if (state == CONN_ERROR || state == CONN_CLOSED)
    {
        if (socket_remove(evl, sockfd) == -1)
        {
            LOG_ERROR("failed to remove socket %d from epoll after read state %d", sockfd, state);
        }
        close(sockfd);
        conn_reset(conn);
    }
}

void start_event_loop(event_loop_t *evl)
{
    dict_init();
    for (;;)
    {
        u32 nfds = epoll_wait(evl->_epollfd, evl->_events, MAX_EVENTS, NO_TIMEOUT);

        for (u32 i = 0; i < nfds; i++)
        {
            int sockfd = evl->_events[i].data.fd;
            u32 flags = evl->_events[i].events;
            if (sockfd == evl->_listensock->_sockfd)
                handle_accept(evl, sockfd);
            else if (flags & EPOLLIN)
                handle_request(evl, sockfd);
            else if (flags & EPOLLOUT)
                handle_response(evl, sockfd);
        }
        dict_continue_rehash(NO_START_REHASH);
    }
}
