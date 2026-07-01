#include "buff.h"


buff buff_init(){
    size_t cap = DEFAULT_BUFF_SIZE;
    void* ptr = malloc(cap);

    if(ptr == NULL){
        err("buff_init : malloc failure");
        exit(EXIT_FAILURE);
    }

    return (buff){
        .capacity = cap,
        .ptr = ptr,
        .tail = 0
    };
}

size_t buff_remaining(const buff* buff){
    return buff->capacity - buff->tail;
}
void buff_resize(buff* buff){
    void* ptr = realloc(buff->ptr,buff->capacity*2);
    if(ptr == NULL){
        err("buff_resize : realloc failure");
        exit(EXIT_FAILURE);
    }

    buff->ptr = ptr;
    buff->capacity *= 2;
}

void buff_reset(buff* buff){
    void* ptr = realloc(buff->ptr,DEFAULT_BUFF_SIZE);
    if(ptr == NULL){
        err("buff_resize : realloc failure");
        exit(EXIT_FAILURE);
    }

    buff->ptr = ptr;
    buff->capacity = DEFAULT_BUFF_SIZE;
    buff->tail = 0;
}

Conn_state_t buff_read(u32 socketfd, buff* buff){
    for(;;){
        if(buff_remaining(buff) == 0){
            buff_resize(buff);
        }

        ssize_t nbytes = recv(
            socketfd,
            buff->ptr + buff->tail,
            buff_remaining(buff),
            0
        );

        info("errno : %d",errno);

        if(nbytes > 0){
            buff->tail += nbytes;
            continue;
        }

        else if(nbytes == 0)
            return CONN_CLOSED;

        if(errno == EAGAIN){
            return CONN_NODATA;
        }else if(errno == EINTR){
            continue;
        }
        err("connection error for socket %d",socketfd);
        return CONN_ERROR;
            
        
    }
}

