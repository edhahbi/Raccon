#include "Socket.h"

int main(){

    Socket s;
    socket_init(&s,"127.0.0.1",8080,10);
    socket_listen(&s);
    return 0;
}
