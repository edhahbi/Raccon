#include "dict.h"
#include <stdio.h>
#include <stdlib.h>

static void expect_true(bool condition, const char* message){
    if(!condition){
        LOG_ERROR("FAIL: %s\n",message);
        exit(1);
    }
}

static void test_insert_and_lookup(void){
    dict_init();

    char key_bytes[] = {'x', 'w'};
    dict_key key = {
        .len = sizeof(key_bytes),
        .value = key_bytes
    };

    dict_tv value = {
        .type = UINT,
        .value.uint64_pt = &(u64){50}
    };

    dict_set(key, value);

    search_result result = dict_try_get(key);
    expect_true(result.state != NOTFOUND, "inserted key should be found");
    expect_true(result.value != NULL, "lookup should return a value pointer");
    expect_true(((dict_tv*)result.value)->type == UINT, "stored value type should be INT");
    expect_true(*(((dict_tv*)result.value)->value.uint64_pt) == 50, "stored integer value should match");
}

static void test_update_existing_key(void){
    char key_bytes[] = {'a', 'b'};
    dict_key key = {
        .len = sizeof(key_bytes),
        .value = key_bytes
    };

    dict_set(key, (dict_tv){ .type = UINT, .value.uint64_pt = &(u64){10} });
    dict_set(key, (dict_tv){ .type = UINT, .value.uint64_pt = &(u64){99} });

    search_result result = dict_try_get(key);
    expect_true(result.state != NOTFOUND, "updated key should still be found");
    expect_true(*(((dict_tv*)result.value)->value.uint64_pt) == 99, "updated value should replace the previous one");
}

static void test_missing_key_is_not_found(void){
    char key_bytes[] = {'m', 'i', 's', 's'};
    dict_key key = {
        .len = sizeof(key_bytes),
        .value = key_bytes
    };

    search_result result = dict_try_get(key);
    expect_true(result.state == NOTFOUND, "missing key should return NOTFOUND");
    expect_true(result.value == NULL, "missing key should have a NULL value pointer");
}

static void test_delete_removes_key(void){
    char key_bytes[] = {'d', 'e', 'l'};
    dict_key key = {
        .len = sizeof(key_bytes),
        .value = key_bytes
    };

    dict_set(key, (dict_tv){ .type = UINT, .value.uint64_pt = &(u64){7} });
    dict_try_del(key);

    search_result result = dict_try_get(key);
    expect_true(result.state == NOTFOUND, "deleted key should no longer be found");
}

int main(void){
    test_insert_and_lookup();
    test_update_existing_key();
    test_missing_key_is_not_found();
    test_delete_removes_key();

    puts("all dict tests passed");
    return 0;
}
