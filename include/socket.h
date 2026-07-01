#pragma once
#define _GNU_SOURCE
#include "common.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080

typedef struct sockaddr_in addr_ipv4;
typedef struct socket
{
    addr_ipv4 _address;
    int _sockfd;
    u32 _backlog;
} socket_t;

void socket_init(
    socket_t *sock,
    ip_addr_t ip_addr,
    in_port_t port,
    u32 backlog);

void socket_listen(socket_t *socket);
int socket_accept(int socketfd);
void get_addr_info(addr_ipv4 *client_addr);
