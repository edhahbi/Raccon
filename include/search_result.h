#pragma once

typedef enum search_state{
    TABLE0,
    TABLE1,
    NOTFOUND,
} search_state;

typedef struct search_result{
    void* const value;
    search_state state;
}search_result;
