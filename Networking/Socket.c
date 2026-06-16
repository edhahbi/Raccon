#include "Socket.h"

void socket_init(Socket* sock, ip_addr_t ip_addr, in_port_t port, int backlog){
    sock->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock->_sockfd == -1){
        err("failure when creating socket");
        return;   
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
        return;
    }
    sock->backlog = Min(backlog,10);
    info("socket initialization has been done\n");
}   

void socket_listen(Socket* sock){
    if(listen(sock->_sockfd, sock->backlog)){
        err("socket can't listen on port %d or ip address %s",
            sock->_address.sin_port,
            inet_ntoa(sock->_address.sin_addr));
    }  

    info("listening on port %d with ip address %s"
        ,sock->_address.sin_port,
        inet_ntoa(sock->_address.sin_addr));

    while (true)
    {
        int client_fd = accept(sock->_sockfd,NULL,NULL);
        if(client_fd < 0){
            continue;
        }
        info("client has connected to the server");
        break;
    }
    
}
