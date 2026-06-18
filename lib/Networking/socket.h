#pragma once
#include "common.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "logger.h"
#define PORT 8080


typedef struct socket{
    struct sockaddr_in _address;
    u32 _sockfd;
    u32 _backlog;
}socket_t;

void socket_init(
    socket_t* sock,
    ip_addr_t ip_addr,
    in_port_t port,
    u32 backlog);

void socket_listen(socket_t* socket);
void get_addr_info(struct sockaddr_in* client_addr);
