#pragma once
#include "dictht.h"
#include "search_result.h"

#define BIG_REHASH_SHIFT 1 
#define SMALL_REHASH_SHIT 2 

typedef struct dict
{
    bool rehasing_flag;
    dictht *ht[2];
    size_t rehasing;
} dict;

static dict db;

void dict_init();
search_result dict_try_get(dict_key); //return a search_result where the value is dict_tv* 
void dict_set(dict_key, dict_tv);
static inline size_t get_hash(size_t size, size_t hash){return hash & (size - 1);}
static inline dict_entry** dict_get_block(size_t ht_idx, size_t hash){return &(db.ht[ht_idx]->table[hash]);}
bool try_del_entry(dictht* ht,dict_key key,size_t hash);
void dict_del(dict_key);
void rehash();

