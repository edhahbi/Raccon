#include "socket.h"

void socket_init(socket_t* sock, const char* ip_addr, in_port_t port, u32 backlog){
    sock->_sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(sock->_sockfd == -1){
        LOG_ERROR("failed to create a non-blocking TCP socket");
        exit(EXIT_FAILURE);
    }

    sock->_address = (struct sockaddr_in){0};
    sock->_address.sin_family = AF_INET;
    sock->_address.sin_port = htons(port);
    if(inet_pton(AF_INET, ip_addr, &sock->_address.sin_addr) == -1){
        LOG_ERROR("failed to convert IP address %s into network format", ip_addr);
        exit(EXIT_FAILURE);
    }

    int opt = 1;

    setsockopt(
        sock->_sockfd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );
    
    if(bind(sock->_sockfd,(struct sockaddr*)&sock->_address,sizeof(sock->_address))){
        LOG_ERROR("failed to bind socket %d to %s:%d", sock->_sockfd, ip_addr, port);
        exit(EXIT_FAILURE);
    }
    sock->_backlog = MIN(backlog,10);
    LOG_INFO("initialized socket %d for %s:%d with backlog %u", sock->_sockfd, ip_addr, port, sock->_backlog);
}   

void socket_listen(const socket_t* sock){
    if(listen(sock->_sockfd, sock->_backlog)){

        LOG_ERROR("failed to listen on socket %d for %s:%d",
            sock->_sockfd,
            inet_ntoa(sock->_address.sin_addr),
            ntohs(sock->_address.sin_port));
        exit(EXIT_FAILURE);
    }  

    LOG_INFO("listening on %s:%d using socket %d",
        inet_ntoa(sock->_address.sin_addr),
        ntohs(sock->_address.sin_port),
        sock->_sockfd);    
}


ipv4_header get_addr_info(const addr_ipv4* client_addr){
    const in_addr_t ip_addr = client_addr->sin_addr.s_addr;
    return (ipv4_header){
        .b1 = (ip_addr >> 24),
        .b2 = (ip_addr >> 16) & 0xFF,
        .b3 = (ip_addr >> 8) & 0xFF,
        .b4 = (ip_addr & 0xFF)
    };
}

int socket_accept(int socketfd){
    addr_ipv4 client_addr;
    socklen_t client_addr_len;
    int result = accept4(socketfd, (struct sockaddr *)&client_addr, &client_addr_len, SOCK_NONBLOCK);
    if(result > 0){
        ipv4_header header = get_addr_info(&client_addr);
        LOG_INFO(
        "accepted client connection from %u.%u.%u.%u:%d",
            header.b1,
            header.b2,
            header.b3,
            header.b4,
            ntohs(client_addr.sin_port)
        );
    }else{
        close(result);
        LOG_ERROR("Couldn't accept Socket Connection");
    }
    return result;
}


