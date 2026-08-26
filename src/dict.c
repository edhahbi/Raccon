#include "dict.h"



void dict_init()
{
    db.rehasing_flag = false;
    db.rehasing = 0;
    db.ht[0] = dict_ht_init();
    db.ht[1] = dict_ht_init();
}

dict_entry *try_lookup(
    dict_entry *iter,
    dict_key key)
{

    while (iter)
    {
        if (compare_keys(key, iter->key))
        {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

static inline bool probably_in_t0(size_t hash) { return !db.rehasing_flag || db.rehasing <= hash; }

// returns a result state containing dict_entry*
search_result dict_try_get_entry(dict_key key)
{
    size_t fnv_hash = fnv_1a(key);
    size_t hash_t0 = get_hash(db.ht[0]->size, fnv_hash);

    if (probably_in_t0(hash_t0))
    {
        dict_entry *out = try_lookup(*dict_get_block(0, hash_t0), key);
        return (search_result){.value = out, .state = out ? TABLE0 : NOTFOUND};
    }

    size_t hash_t1 = get_hash(db.ht[1]->size, fnv_hash);
    dict_entry *out = try_lookup(*dict_get_block(1, hash_t1), key);
    return (search_result){.value = out, .state = out ? TABLE1 : NOTFOUND};
}

// returns a result state containing dict_tv
search_result dict_try_get(dict_key key)
{
    search_result entry_result = dict_try_get_entry(key);

    search_result dict_tv_result = entry_result.state != NOTFOUND ? 
    (search_result){.state = entry_result.state, .value = ((dict_entry *)entry_result.value)->tv} :
    (search_result){.state = entry_result.state, .value = NULL};

    return dict_tv_result;
}

void dict_set(dict_key key, dict_tv tv)
{
    search_result dict_tv_result = dict_try_get(key);
    size_t hash_fnv1 = fnv_1a(key);

    /*
        object is not found:
            -> if we are in a rehasing state we add to table [1 = db.rehasing_flag = true]
            -> if we're not in a rehasing state we add to table [0 = db.rehasing_flag = false]

        object is found:
            -> just modify the entry
    */
    if (dict_tv_result.state == NOTFOUND)
    {
        size_t final_hash = get_hash(db.ht[db.rehasing_flag]->size, hash_fnv1);
        dict_entry *const new_entry = create_entry(key, tv);
        add_entry(db.ht[db.rehasing_flag], final_hash, new_entry);
        db.ht[db.rehasing_flag]->used++;
        dict_continue_rehash(BIG_REHASH);
    }
    else
    {
        dict_tv const tv_ptr = ((dict_tv const)dict_tv_result.value);
        tv_ptr->type = tv->type;
        tv_ptr->value = tv->value;
    }
}

bool try_del_entry(dictht *ht, dict_key key, size_t hash)
{
    dict_entry *before = ht->table[hash];

    if (before == NULL)
        return false;

    if (compare_keys(before->key, key))
    {
        ht->table[hash] = before->next;
        free(before);
        return true;
    }

    while (before->next)
    {
        if (compare_keys(before->next->key, key))
        {
            dict_entry *temp = before->next;
            before->next = temp->next;
            free(temp);
            return true;
        }
        before = before->next;
    }

    return false;
}

bool dict_try_del(dict_key key)
{
    size_t hash_fnv1 = fnv_1a(key);
    size_t hash_t0 = get_hash(db.ht[0]->size, hash_fnv1);

    if (probably_in_t0(hash_t0) && try_del_entry(db.ht[0], key, hash_t0))
    {
        db.ht[0]->used--;
        dict_continue_rehash(SMALL_REHASH);
        return true;
    }

    size_t hash_t1 = get_hash(db.ht[1]->size, hash_fnv1);
    if (try_del_entry(db.ht[1], key, hash_t1))
    {
        db.ht[1]->used--;
        dict_continue_rehash(SMALL_REHASH);
        return true;
    }

    return false;
}

void resize(dictht *ht, size_t new_size)
{
    ht->size = new_size;
    ht->table = realloc(db.ht[1]->table, sizeof(dict_entry *) * ht->size);
}

static void stop_rehashing()
{
    db.rehasing = 0;
    db.rehasing_flag = false;

    dictht *swap = db.ht[0];
    db.ht[0] = db.ht[1];
    db.ht[1] = swap;
}

void dict_insert_in_block(dict_entry **block, dict_entry *entry_t0)
{
    if ((*block) == NULL)
    {
        (*block) = entry_t0;
        return;
    }

    dict_entry *entry_t1 = *block;
    while (entry_t1->next)
        entry_t1 = entry_t1->next;

    entry_t1->next = entry_t0;
}

void dict_rehash(){
    dict_entry **block_t0 = dict_get_block(0, db.rehasing);
    dict_entry *entry_t0 = *block_t0;

    while (entry_t0)
    {
        size_t hash_t1 = get_hash(db.ht[1]->size, fnv_1a(entry_t0->key));
        dict_entry **block_t1 = dict_get_block(1, hash_t1);
        dict_insert_in_block(block_t1, entry_t0);
        entry_t0 = entry_t0->next;
    }

    *block_t0 = NULL;

    db.rehasing++;

    if (db.rehasing == db.ht[0]->size)
        stop_rehashing();
    
}

void dict_continue_rehash(rehash_flag flag)
{
    if (db.rehasing_flag){
        dict_rehash();
        return;
    }
        
    switch (flag)
    {
    case BIG_REHASH:
        size_t br = db.ht[0]->size >> BIG_REHASH_SHIFT;
        if (db.ht[0]->used >= br)
        {
            db.rehasing_flag = 1;
            resize(db.ht[1], db.ht[1]->size << DEFAULT_RESIZE_SHIFT);
        }
        break;

    case SMALL_REHASH:
        size_t sr = db.ht[0]->size >> SMALL_REHASH_SHIT;
        if (db.ht[1]->used <= sr)
        {
            db.rehasing = 1;
            resize(db.ht[1], db.ht[1]->size >> DEFAULT_RESIZE_SHIFT);
        }
        break;

    default:
        break;
    }
}
