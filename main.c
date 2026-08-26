#include "evl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void print_usage(const char *prog)
{
    fprintf(stderr, "Usage: %s [address] [port]\n", prog);
    fprintf(stderr, "Defaults: 127.0.0.1 8080\n");
}

int main(int argc, char **argv)
{
    const char *addr = "127.0.0.1";
    long port = 8080;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }
        addr = argv[1];
    }

    if (argc > 2)
    {
        char *endptr = NULL;
        errno = 0;
        long p = strtol(argv[2], &endptr, 10);
        if (errno != 0 || *endptr != '\0' || p <= 0 || p > 65535)
        {
            fprintf(stderr, "Invalid port: %s\n", argv[2]);
            print_usage(argv[0]);
            return 1;
        }
        port = p;
    }

    socket_t server_sock;
    socket_init(&server_sock, addr, (in_port_t)port, MAX_EVENTS);
    socket_listen(&server_sock);

    event_loop_t evl;
    init_event_loop(&evl, &server_sock, EPOLLIN | EPOLLOUT);
    start_event_loop(&evl);

    return 0;
}
