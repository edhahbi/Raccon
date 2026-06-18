#include "socket.h"

void socket_init(socket_t* sock, ip_addr_t ip_addr, in_port_t port, u32 backlog){
    sock->_sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(sock->_sockfd == -1){
        err("failure when creating socket");
        exit(EXIT_FAILURE);
    }

    sock->_address = (struct sockaddr_in){0};
    sock->_address.sin_family = AF_INET;
    sock->_address.sin_port = htons(port);
    if(inet_pton(AF_INET, ip_addr, &sock->_address.sin_addr) == -1){
        err("can't bind the %s ip adress",ip_addr);
        return;
    }
    if(bind(sock->_sockfd,(struct sockaddr*)&sock->_address,sizeof(sock->_address))){
        err("can't bind the socket %s ip address",ip_addr);
        exit(EXIT_FAILURE);
    }
    sock->_backlog = Min(backlog,10);
    info("socket initialization has been done\n");
}   

void socket_listen(socket_t* sock){
    if(listen(sock->_sockfd, sock->_backlog)){
        err("socket can't listen on port %d or ip address %s",
            sock->_address.sin_port,
            inet_ntoa(sock->_address.sin_addr));
        exit(EXIT_FAILURE);
    }  

    info("listening on port %d with ip address %s"
        ,sock->_address.sin_port,
        inet_ntoa(sock->_address.sin_addr));    
}

void get_addr_info(struct sockaddr_in* client_addr){


    u32 ip_addr = client_addr->sin_addr.s_addr;
    info(
        "a new client has been accepted from %u.%u.%u.%u",
        ip_addr >> 24,
        (ip_addr >> 16) & 0xFF,
        (ip_addr >> 8) & 0xFF,
        ip_addr & 0xFF
    );
}
