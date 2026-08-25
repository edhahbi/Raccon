#include "buffer.h"

buffer buffer_init(size_t size)
{
    char *mem = malloc(size);
    if (mem == NULL)
    {
        LOG_ERROR("Invalid Memory Allocation");
        exit(EXIT_FAILURE);
    }
    return (buffer){
        .capacity = size,
        .offset = 0,
        .ptr = mem};
}

void buffer_resize(buffer *const buff, size_t new_capacity)
{
    buff->ptr = realloc(buff->ptr, new_capacity);
    if (buff->ptr == NULL)
    {
        LOG_ERROR("Invalid Memory Allocation");
        exit(EXIT_FAILURE);
    }
    buff->capacity = new_capacity;
}

conn_state buffer_read(int socketfd, buffer *const buff)
{
    for (;;)
    {
        if (buffer_remaining(buff) == 0)
        {
            buffer_resize(buff, buff->capacity * 2);
        }

        ssize_t nbytes = recv(
            socketfd,
            buff->ptr + buff->offset,
            buffer_remaining(buff),
            0);

        // read data so there is probably more
        if (nbytes > 0)
        {
            buff->offset += nbytes;
            continue;
        }

        // client socket closed
        if (nbytes == 0)
            return CONN_CLOSED;

        if (nbytes == -1)
        {
            // there is no more data
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return CONN_WAIT;

            // interrupted by sys call try again
            else if (errno == EINTR)
                continue;

            // socket error
            return CONN_ERROR;
        }
    }
}

void buffer_sync(buffer *const buff, size_t index)
{
    if (index == buff->offset)
    {
        buff->offset = 0;
    }
    else
    {
        memmove(buff->ptr, buff->ptr + index, buff->offset - index);
        buff->offset = buff->offset - index;
    }
}

// functions for outcoming buffer

conn_state buffer_write(int socketfd, buffer *const buff)
{
    size_t tail = 0;
    for (;;)
    {
        if (tail == buff->offset)
        {
            buffer_sync(buff, tail);
            return CONN_WAIT;
        }

        ssize_t nbytes = send(
            socketfd,
            buff->ptr + tail,
            buff->offset - tail,
            0);

        if (nbytes > 0)
        {
            tail += nbytes;
            continue;
        }

        if (nbytes == 0)
            return CONN_CLOSED;

        if (nbytes == -1 && errno == EAGAIN)
        {
            buffer_sync(buff, tail);
            return CONN_WAIT;
        }

        if (nbytes == -1 && errno == EINTR)
            continue;

        else
            return CONN_ERROR;
    }
}

void buffer_push(buffer *const buff, size_t size, const char *format, const void *response)
{

    if (size > buff->capacity)
    {
        buffer_resize(buff, size);
    }

    snprintf(buff->ptr, size, format, response);

    buff->offset += size;
}
void buffer_free(buffer *const buff)
{
    free(buff->ptr);
    buff->capacity = 0;
    buff->offset = 0;
}
