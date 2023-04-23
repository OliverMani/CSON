#ifndef JSON_H
#define JSON_H
#include <stddef.h>
#include <stdint.h>

enum Type;
typedef struct __json JSON;

enum Type json_get_type(JSON* json);

char* json_get_string(JSON* json);
int64_t json_get_int(JSON* json);
char json_get_boolean(JSON* json);

JSON* json_string(char* json);
JSON* json_int(int64_t i);
JSON* json_boolean(char bool);

int json_set_string(JSON* json, char* str);
int json_set_int(JSON* json, int64_t i);
int json_set_boolean(JSON* json, char bool);



int json_set_object_as_string(JSON* json, char* key, char* string);
int json_set_object_as_int(JSON* json, char* key, int64_t i);
int json_set_object_as_boolean(JSON* json, char* key, char bool);
int json_set_object(JSON* json, char* key, JSON* value);

char* json_get_object_as_string(JSON* json, char* key);
int64_t json_get_object_as_int(JSON* json, char* key);
char json_get_object_as_boolean(JSON* json, char* key);

JSON* json_get_object(JSON* json, char* key);
JSON* json_free_object(JSON* json, char* key);



int json_array_append(JSON* json, JSON* value);
int json_array_append_string(JSON* json, char* string);
int json_array_append_int(JSON* json, int64_t i);
int json_array_append_boolean(JSON* json, char bool);

int json_array_insert(JSON* json, JSON* value, uint32_t index);
int json_array_insert_string(JSON* json, char* string, uint32_t index);
int json_array_insert_int(JSON* json, uint64_t i, uint32_t index);
int json_array_insert_boolean(JSON* json, char bool, uint32_t index);

JSON* json_array_get(JSON* json, uint32_t index);
int json_array_set(JSON* json, uint32_t index, JSON* value);
int json_array_free(JSON* json, uint32_t index);

JSON* json_object();
JSON* json_array();

int json_length(JSON* json);

int json_free(JSON* json);

void json_pretty_print(JSON* json);

// Left to do
int json_length(JSON* json);

JSON* json_clone(JSON* json);

#endif

/*

*/