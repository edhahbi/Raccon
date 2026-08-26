#include "dictht.h"


#define OFFSET 14695981039346656037ULL
#define PRIME 1099511628211ULL

static inline u8* get_key_data(const dict_tv key){return ((string*)key->value)->ptr;}
static inline size_t get_key_len(const dict_tv key){return ((string*)key->value)->size;}

size_t fnv_1a(const dict_key key){

    if (key->type != RESP_STRING){
        LOG_ERROR("unsupported key type");
        exit(1);
    }

    u8* bytes = get_key_data(key);
    size_t len = get_key_len(key); 
    u64 hash = OFFSET;
    for (size_t byte = 0; byte < len; byte++){
        hash ^= bytes[byte];
        hash *= PRIME;
    }
    return hash;
}

dictht* dict_ht_init(){
    dictht* ht = malloc(sizeof(dictht));
    ht->table = calloc(DEFAULT_TABLE_SIZE,sizeof(dict_entry*));
    ht->used = 0;
    ht->size = DEFAULT_TABLE_SIZE;
    return ht;
}

bool compare_keys(dict_key k1, dict_key k2){
    if(get_key_len(k1) != get_key_len(k2)){
        return false;
    }

    u8* bytes_k1 = get_key_data(k1);
    u8* bytes_k2 = get_key_data(k2);

    for (size_t byte = 0; byte < get_key_len(k1); byte++){
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
