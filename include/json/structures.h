#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdint.h>

struct __json_string;
struct __json_boolean;
struct __json_int;
struct __json_array;
struct __json_object;


enum Type {
    STRING,
    BOOLEAN,
    INT,
    ARRAY,
    OBJECT,
    NONE
};

union __json_value {
    struct __json_string* str;
    struct __json_boolean* bool;
    struct __json_int* i;
    struct __json_array* array;
    struct __json_object* object;
};

typedef struct __json {
    enum Type type;
    union __json_value data;
} JSON;

struct __json_object {
    char* key;
    struct __json* value;
    struct __json_object* left_node;
    struct __json_object* right_node;
};

struct __json_array {
    uint32_t index;
    struct __json* value;
    struct __json_array* next;
};

struct __json_int {
    int64_t i;
};

struct __json_boolean {
    uint8_t bool;
};

struct __json_string {
    char* string;
};



#endif