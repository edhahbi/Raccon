#include "fnv_1a.h"

size_t fnv_1a(dict_key key){
    u8* bytes = (u8*)key.value;
    u64 hash = OFFSET;
    for (size_t byte = 0; byte < key.len; byte++){
        hash ^= bytes[byte];
        hash *= PRIME;
    }
    return hash;
}
