#include "sdc.h"

string sdc_init(const char* ptr, const size_t size){
    string s = {0};
    if(size == 0){
        s.ptr = calloc(1, 1);
        if(s.ptr == NULL){
            LOG_ERROR("couldn't allocate memory for string");
            exit(EXIT_FAILURE);
        }
        s.size = 0;
        return s;
    }

    s.ptr = malloc(size);
    if(s.ptr == NULL){
        LOG_ERROR("couldn't allocate memory for string");
        exit(EXIT_FAILURE);
    }

    s.size = size;

    if(ptr != NULL)
        memcpy(s.ptr, ptr, size);

    return s;
}

void sdc_free(string* sdc){
    free(sdc->ptr);
    sdc->ptr = NULL;
    sdc->size = 0;
}

void sdc_push(string* sdc, char c){
    char* new_ptr = realloc(sdc->ptr, sdc->size + 1);
    if(new_ptr == NULL){
        LOG_ERROR("couldn't reallocate string buffer");
        exit(EXIT_FAILURE);
    }

    sdc->ptr = new_ptr;
    sdc->ptr[sdc->size] = c;
    sdc->size += 1;
}

char from_digit_to_char(int digit){

    switch (digit)
    {
    case 0:
        return '0';
    
    case 1:
        return '1';

    case 2:
        return '2';

    case 3:
        return '3';

    case 4:
        return '4';

    case 5:
        return '5';

    case 6:
        return '6';

    case 7:
        return '7';
    
    case 8:
        return '8';
    
    case 9:
        return '9';

    default:
        LOG_ERROR("invalid digit");
        exit(1);
    }
}

string from_int_to_srt(u64 uinteger){
    if(uinteger == 0)
        return sdc_init("0", 1);

    string sdc = sdc_init(NULL, 0);
    char buffer[32];
    size_t len = 0;

    while (uinteger != 0){
        buffer[len++] = from_digit_to_char((int)(uinteger % 10));
        uinteger /= 10;
    }

    for (size_t i = 0; i < len; ++i){
        sdc_push(&sdc, buffer[len - 1 - i]);
    }

    return sdc;
}

void sdc_merge(string* dest, const string* src){
    if(src == NULL || src->size == 0)
        return;

    char* new_ptr = realloc(dest->ptr, dest->size + src->size);
    if(new_ptr == NULL){
        LOG_ERROR("couldn't expand string buffer during merge");
        exit(EXIT_FAILURE);
    }

    dest->ptr = new_ptr;
    memcpy(dest->ptr + dest->size, src->ptr, src->size);
    dest->size += src->size;
}
