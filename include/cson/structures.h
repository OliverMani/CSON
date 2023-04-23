#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdint.h>

struct __cson_string;
struct __cson_boolean;
struct __cson_int;
struct __cson_array;
struct __cson_object;


enum Type {
    STRING,
    BOOLEAN,
    INT,
    ARRAY,
    OBJECT,
    NONE
};

union __cson_value {
    struct __cson_string* str;
    struct __cson_boolean* bool;
    struct __cson_int* i;
    struct __cson_array* array;
    struct __cson_object* object;
};

typedef struct __cson {
    enum Type type;
    union __cson_value data;
} CSON;

struct __cson_object {
    char* key;
    struct __cson* value;
    struct __cson_object* left_node;
    struct __cson_object* right_node;
};

struct __cson_array {
    uint32_t index;
    struct __cson* value;
    struct __cson_array* next;
};

struct __cson_int {
    int64_t i;
};

struct __cson_boolean {
    uint8_t bool;
};

struct __cson_string {
    char* string;
};



#endif