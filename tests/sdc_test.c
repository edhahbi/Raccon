#include "sdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void expect_true(int condition, const char* message){
    if(!condition){
        fprintf(stderr, "FAIL: %s\n", message);
        exit(1);
    }
}

static void expect_string_eq(const string* actual, const char* expected, const char* message){
    f
    if(strcmp(actual, expected) != 0){
        fprintf(stderr, "FAIL: %s (expected '%s', got '%s')\n", message, expected, actual);
        exit(1);
    }
}

static void test_sdc_init_and_push(void){
    string s = sdc_init("abc", 3);
    expect_true(s.size == 3, "init should preserve size");
    expect_true(memcmp(s.ptr, "abc", 3) == 0, "init should copy source bytes");

    sdc_push(&s, '!');
    expect_true(s.size == 4, "push should increase size");
    expect_true(s.ptr[3] == '!', "push should append the new character");

    sdc_free(&s);
}

static void test_sdc_merge(void){
    string left = sdc_init("Hello", 5);
    string right = sdc_init(" World", 6);

    sdc_merge(&left, &right);
    expect_true(left.size == 11, "merge should increase size by src size");
    expect_true(memcmp(left.ptr, "Hello World", 11) == 0, "merge should append source bytes");

    sdc_free(&left);
    sdc_free(&right);
}

int main(void){
    test_sdc_init_and_push();
    test_sdc_merge();

    puts("all SDC tests passed");
    return 0;
}
