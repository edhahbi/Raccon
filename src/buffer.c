#include "buffer.h"


buffer buffer_init(size_t size){
    char* mem = malloc(size);
    if(mem == NULL){
        LOG_ERROR("Invalid Memory Allocation");
        exit(EXIT_FAILURE);
    }
    return (buffer){
        .capacity = size,
        .offset = 0,
        .ptr = mem
    };
}

void buffer_resize(buffer* buff,size_t new_capacity){
    buff->ptr = realloc(buff->ptr, new_capacity);
    if(buff->ptr == NULL){
        LOG_ERROR("Invalid Memory Allocation");
        exit(EXIT_FAILURE);
    }
    buff->capacity = new_capacity;
}

conn_state_t buffer_write(int socketfd, buffer *buff){
    for(;;){
        if(buffer_remaining(buff) == 0){
            buffer_resize(buff, buff->capacity * 2);
        } 

        ssize_t nbytes = recv(
            socketfd,
            buff->ptr + buff->offset,
            buffer_remaining(buff),
            0
        );

        // read data so there is probably more
        if(nbytes > 0){
            buff->offset += nbytes;
            continue;   
        }

        // client socket closed
        if(nbytes == 0)
            return CONN_CLOSED;
        

        if(nbytes == -1){
            LOG_DEBUG("%d",errno);
            // there is no more data
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                return CONN_WAIT;
            
            
            // interrupted by sys call try again
            else if(errno == EINTR)
                continue;

            // socket error 
            return CONN_ERROR;
        }
    }
}

void buffer_sync(buffer* buff,size_t ps_index){
    if(ps_index == buff->offset){
        buff->offset = 0;
    }else{
        memmove(buff->ptr, buff->ptr + ps_index, buff->offset - ps_index);
    }
}
