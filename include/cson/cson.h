#ifndef CSON_H
#define CSON_H
#include <stddef.h>
#include <stdint.h>

enum Type;
typedef struct __cson CSON;

enum Type cson_get_type(CSON* cson);

char* cson_get_string(CSON* cson);
int64_t cson_get_int(CSON* cson);
char cson_get_boolean(CSON* cson);

CSON* cson_string(char* cson);
CSON* cson_int(int64_t i);
CSON* cson_boolean(char bool);

int cson_set_string(CSON* cson, char* str);
int cson_set_int(CSON* cson, int64_t i);
int cson_set_boolean(CSON* cson, char bool);



int cson_set_object_as_string(CSON* cson, char* key, char* string);
int cson_set_object_as_int(CSON* cson, char* key, int64_t i);
int cson_set_object_as_boolean(CSON* cson, char* key, char bool);
int cson_set_object(CSON* cson, char* key, CSON* value);

char* cson_get_object_as_string(CSON* cson, char* key);
int64_t cson_get_object_as_int(CSON* cson, char* key);
char cson_get_object_as_boolean(CSON* cson, char* key);

CSON* cson_get_object(CSON* cson, char* key);
int cson_free_object(CSON* cson, char* key);



int cson_array_append(CSON* cson, CSON* value);
int cson_array_append_string(CSON* cson, char* string);
int cson_array_append_int(CSON* cson, int64_t i);
int cson_array_append_boolean(CSON* cson, char bool);

int cson_array_insert(CSON* cson, CSON* value, uint32_t index);
int cson_array_insert_string(CSON* cson, char* string, uint32_t index);
int cson_array_insert_int(CSON* cson, uint64_t i, uint32_t index);
int cson_array_insert_boolean(CSON* cson, char bool, uint32_t index);

CSON* cson_array_get(CSON* cson, uint32_t index);
int cson_array_set(CSON* cson, uint32_t index, CSON* value);
int cson_array_free(CSON* cson, uint32_t index);

CSON* cson_object();
CSON* cson_array();

int cson_length(CSON* cson);

int cson_free(CSON* cson);

void cson_pretty_print(CSON* cson);


int cson_length(CSON* cson);
// Left to do
CSON* cson_clone(CSON* cson);

#endif

/*

*/