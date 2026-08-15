#pragma once
#include "command.h"
#include "sdc.h"
#include "fnv_1a.h"

#define DEFAULT_TABLE_SIZE 8 // always power of 2
#define DEFAULT_RESIZE_SHIFT 1

typedef struct property property;

typedef struct object
{
    property* properties;
    size_t size;
} object;


typedef union dict_value
{
    object object;
    u64 uint64;
    i64 int64;
    double d;
} dict_value;

typedef enum value_type
{
    OBJECT,
    INT,
    UINT,
    DOUBLE,
    STRING
} value_type;

typedef struct dict_tv
{
    dict_value value;
    value_type type;
} dict_tv;

typedef struct property{
    string field;
    dict_tv value;
}property;

typedef struct dict_entry
{
    dict_key key;
    dict_tv tv;
    struct dict_entry *next;
} dict_entry;

typedef struct dictht
{
    dict_entry **table;
    size_t size;
    size_t used;
} dictht;

dictht* dict_ht_init();
dict_entry* create_entry(dict_key, dict_tv);
bool compare_keys(dict_key, dict_key);
void add_entry(const dictht*,size_t, dict_entry* const entry);
