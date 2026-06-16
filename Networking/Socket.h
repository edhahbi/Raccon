#pragma once
#include "common.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "logger.h"
#define PORT 8080


typedef struct socket_t{
    struct sockaddr_in _address;
    int _sockfd;
    int backlog;
}Socket;

void socket_init(
    Socket* sock,
    ip_addr_t ip_addr,
    in_port_t port,
    int backlog);

void socket_listen(Socket* socket);


