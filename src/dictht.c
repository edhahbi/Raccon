#include "dictht.h"

dictht* dict_ht_init(){
    dictht* ht = malloc(sizeof(dictht));
    ht->table = calloc(DEFAULT_TABLE_SIZE,sizeof(dict_entry*));
    ht->used = 0;
    ht->size = DEFAULT_TABLE_SIZE;
    return ht;
}

bool compare_keys(dict_key k1, dict_key k2){
    if(k1.len != k2.len){
        return false;
    }

    u8* bytes_k1 = (u8*)k1.value;
    u8* bytes_k2 = (u8*)k2.value;

    for (size_t byte = 0; byte < k1.len; byte++){
       if(bytes_k1[byte] != bytes_k2[byte])
            return false;
    }
    
    return true;
}

dict_entry* create_entry(dict_key key, dict_tv tv){
    dict_entry* entry = malloc(sizeof(dict_entry));
    entry->key = key;
    entry->tv = tv;
    entry->next = NULL;
    return entry;
}

void add_entry(const dictht* ht,size_t hash, dict_entry* const new_entry){
    dict_entry* entry = ht->table[hash];

    if(entry == NULL){
        ht->table[hash] = new_entry;
        return;
    }

    while (entry->next)
        entry = entry->next;
    
    entry->next = new_entry;
}
