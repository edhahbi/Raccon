#include "sdc.h"

string sdc_init(const char* ptr, const size_t size){
    string s; 
    char* mem = malloc(size);
    
    if(mem == NULL){
        LOG_ERROR("couldn't allocate memory for string");
        exit(EXIT_FAILURE);
    }

    s.ptr = mem;
    s.size = size;

    if(ptr != NULL)
        memcpy(s.ptr,ptr,size);
    
    return s;
}

void sdc_free(string* sdc){
    free(sdc->ptr);
    sdc->ptr = NULL;
    sdc->size = 0;
}

void sdc_push(string* sdc, char c){
    sdc->ptr = realloc(sdc->ptr, ++sdc->size);
    sdc->ptr[sdc->size - 1] = c;
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
        return sdc_init("0",1);
    
    string sdc = sdc_init(NULL,1);

    while (uinteger){
        int digit = uinteger % 10;
        sdc_push(&sdc,from_digit_to_char(digit));
        uinteger /= 10;
    }
    
    return sdc;
}

void sdc_merge(string* dest,const string* src){
    dest->ptr = realloc(dest->ptr, dest->size + src->size);
    memmove(dest->ptr + dest->size, src, src->size);
    dest->size += src->size;
}
