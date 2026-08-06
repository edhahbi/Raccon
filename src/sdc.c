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

    memcpy(s.ptr,ptr,size);
    
    return s;
}

void sdc_free(string* sdc){
    free(sdc->ptr);
    sdc->ptr = NULL;
    sdc->ptr = 0;
}
