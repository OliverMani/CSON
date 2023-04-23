#include <json/json.h>
#include <json/structures.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum Type json_get_type(JSON* json) {
    if(!json)
        return NONE;
    return json->type;
}

char* json_get_string(JSON* json) {
    if(!json || json->type != STRING)
        return NULL;
    return json->data.str->string;
}

int64_t json_get_int(JSON* json) {
    if(!json || json->type != INT)
        return 0;
    return json->data.i->i;
}

char json_get_boolean(JSON* json) {
    if(!json || json->type != BOOLEAN)
        return 0;
    return json->data.bool->bool;
}

JSON* json_string(char* str) {
    JSON* result = (JSON*) malloc(sizeof(JSON));
    result->type = STRING;
    struct __json_string* stringobj = (struct __json_string*) malloc(sizeof(struct __json_string));
    char* string = (char*) malloc(strlen(str) + 1);
    strcpy(string, str);
    stringobj->string = string;
    result->data.str = stringobj;
    return result;
}
JSON* json_int(int64_t i) {
    JSON* result = (JSON*) malloc(sizeof(JSON));
    result->type = INT;
    struct __json_int* iobj = (struct __json_int*) malloc(sizeof(struct __json_int));
    iobj->i = i;
    result->data.i = iobj;
    return result;
}
JSON* json_boolean(char bool) {
    JSON* result = (JSON*) malloc(sizeof(JSON));
    result->type = BOOLEAN;
    struct __json_boolean* bobj = (struct __json_boolean*) malloc(sizeof(struct __json_boolean));
    bobj->bool = bool;
    result->data.bool = bobj;
    return result;
}

int json_set_string(JSON* json, char* str) {
    if(!json || json->type != STRING)
        return 1;
    free(json->data.str->string);
    json->data.str->string = (char*) malloc(strlen(str) + 1);
    strcpy(json->data.str->string, str);
    return 0;
}

int json_set_int(JSON* json, int64_t i) {
    if(!json || json->type != INT)
        return 1;
    json->data.i->i = i;
    return 0;
}
int json_set_boolean(JSON* json, char bool) {
    if(!json || json->type != INT)
        return 1;
    json->data.bool->bool = bool;
    return 0;
}

static inline struct __json_object* __create_object(struct __json_object* left, struct __json_object* right, char* key, JSON* value) {
    struct __json_object* object = (struct __json_object*) malloc(sizeof(struct __json_object));
    object->left_node = NULL;
    object->right_node = NULL;
    object->value = value;
    object->key = key;
    return object;
}

int json_set_object(JSON* json, char* key, JSON* value) {
    if(!json || json->type != OBJECT)
        return 1; // Return error
    struct __json_object* current = json->data.object;
    struct __json_object* prev = current;
    char* new_key = (char*) malloc(sizeof(char) * strlen(key) + 1);
    strcpy(new_key, key);
    while(1) {
        if(!current && prev) {
            current = __create_object(NULL, NULL, new_key, value);
            if(strcmp(prev->key, current->key) < 0) {
                prev->left_node = current;
            } else {
                prev->right_node = current;
            }
        } else if(!current) {
            json->data.object = __create_object(NULL, NULL, new_key, value);
            break;
        }
        int compare = strcmp(key, current->key);
        // If current key IS key (node already defined)
        if(!compare) {
            // We do not want to cause memory leak
            free(new_key);
            // If programmer uses json_object_set with the same object as value, we don't want to erase it!
            if(current->value != value) {
                json_free(current->value);
                current->value = value;
            }
            break;
        } else if (compare < 0) {
            current = current->left_node;
        } else if (compare > 0) {
            current = current->right_node;
        }
        prev = current;
    }

    return 0;
}

int json_set_object_as_string(JSON* json, char* key, char* string) {
    if(!json)
        return 1;
    JSON* strobj = json_string(string);
    return json_set_object(json, key, strobj);
}
int json_set_object_as_int(JSON* json, char* key, int64_t i) {
    if(!json)
        return 1;
    JSON* iobj = json_int(i);
    return json_set_object(json, key, iobj);
}

int json_set_object_as_boolean(JSON* json, char* key, char bool) {
    if(!json)
        return 1;
    JSON* bobj = json_boolean(bool);
    return json_set_object(json, key, bobj);
}

char* json_get_object_as_string(JSON* json, char* key) {
    JSON* object = json_get_object(json, key);
    if(!object || object->type != STRING)
        return NULL;
    return object->data.str->string;
}

int64_t json_get_object_as_int(JSON* json, char* key) {
    JSON* object = json_get_object(json, key);
    if(!object || object->type != INT)
        return 0;
    return object->data.i->i;
}

char json_get_object_as_boolean(JSON* json, char* key) {
    JSON* object = json_get_object(json, key);
    if(!object || object->type != BOOLEAN)
        return 0;
    return object->data.bool->bool;
}

JSON* json_get_object(JSON* json, char* key) {
    if(!json || !key)
        return NULL;
    struct __json_object* current = json->data.object;
    int compare;
    while(current && (compare = strcmp(current->key, key))) {
        if(compare < 0)
            current = current->left_node;
        else if(compare > 0)
            current = current->right_node;
        else break;
    }
    if(current)
        return current->value;
    return NULL;
}




int json_array_append(JSON* json, JSON* value) {
    if(!json)
        return 1;
    if(!json->data.array)
        return json_array_insert(json, value, 0);
    return json_array_insert(json, value, json->data.array->index + 1);
}
int json_array_append_string(JSON* json, char* string) {
    return json_array_append(json, json_string(string));
}
int json_array_append_int(JSON* json, int64_t i) {
    return json_array_append(json, json_int(i));
}
int json_array_append_boolean(JSON* json, char bool) {
    return json_array_append(json, json_boolean(bool));
}


static inline struct __json_array* create_node(JSON* value, uint32_t index, struct __json_array* next) {
    struct __json_array* node = (struct __json_array*) malloc(sizeof(struct __json_array));
    node->index = index;
    node->value = value;
    node->next = next;
    return node;
}

/*
* Walks backwards through the array to add
* <--
*/
int json_array_insert(JSON* json, JSON* value, uint32_t index) {
    if(!json || json->type != ARRAY)
        return 1; // Error
    struct __json_array* current = json->data.array;
    if(!current) {
        if(index)
            return 1; // Error
        json->data.array = create_node(value, 0, NULL);
    } else if(index == current->index + 1) {
        struct __json_array* new = create_node(value, index, current);
        json->data.array = new;
    } else {
        if(index > current->index)
            return 1;
        while(current->index != index) {
            current->index++;
            current = current->next;
        }

        current->index++;
        struct __json_array* next = current->next;
        current->next = create_node(value, index, next);
    }
    return 0;
}

int json_array_insert_string(JSON* json, char* string, uint32_t index) {
    return json_array_insert(json, json_string(string), index);
}
int json_array_insert_int(JSON* json, uint64_t i, uint32_t index) {
    return json_array_insert(json, json_int(i), index);
}
int json_array_insert_boolean(JSON* json, char bool, uint32_t index) {
    return json_array_insert(json, json_boolean(bool), index);
}

JSON* json_array_get(JSON* json, uint32_t index) {
    if(!json || json->type != ARRAY || json->data.array == NULL || json->data.array->index < index)
        return NULL;
    struct __json_array* current = json->data.array;
    while(current->index != index)
        current = current->next;
    return current->value;
}




/* Create an empty object */
JSON* json_object() {
    JSON* new = (JSON*) malloc(sizeof(JSON));
    new->type = OBJECT;
    new->data.object = NULL;
    return new;
}

/* Create an empty array */
JSON* json_array() {
    JSON* new = (JSON*) malloc(sizeof(JSON));
    new->type = ARRAY;
    new->data.array = NULL;
    return new;
}

int __objfree(struct __json_object* obj) {
    if(!obj)
        return 0;
    struct __json_object* left = obj->left_node;
    struct __json_object* right = obj->right_node;
    __objfree(left);
    __objfree(right);
    if(obj->key) {
        free(obj->key);
        obj->key = NULL;
    }
    if(obj->value)
        json_free(obj->value);
    if(obj) {
        free(obj);
        obj = NULL;
    }
    return 0;
}

int __arrfree(struct __json_array* array) {
    struct __json_array* current = array;
    while(current) {
        struct __json_array* next = current->next;
        if(current->value)
            json_free(current->value);
        free(current);
        current = next;
    }
    return 0;
}

int json_free(JSON* json) {
    if(!json)
        return 1;
    switch(json->type) {
        case STRING:
            if(json->data.str->string) {
                free(json->data.str->string);
                json->data.str->string = NULL;
            }
            if(json->data.str) {
                free(json->data.str);
                json->data.str = NULL;
            }
            free(json);
            break;
        case INT:
            if(json->data.i) {
                free(json->data.i);
                json->data.i = NULL;
            }
            free(json);
            break;
        case BOOLEAN:
            if(json->data.bool) {
                free(json->data.bool);
                json->data.bool = NULL;
            }
            free(json);
            break;
        case OBJECT:
            __objfree(json->data.object);
            json->data.object = NULL;
            free(json);
            break;
        case ARRAY:
            __arrfree(json->data.array);
            json->data.array = NULL;
            free(json);
            break;
    }
    return 0;
}




void __pretty_print_rec(JSON* json, int tabs);

void __pretty_print_object_rec(struct __json_object* object, int tabs, char is_root) {
    if(!object) return;
    __pretty_print_object_rec(object->left_node, tabs, 0);
    __pretty_print_object_rec(object->right_node, tabs, 0);
    for(int i = 0; i < tabs; i++) 
        printf("    ");
    printf("\"%s\": ", object->key);
    __pretty_print_rec(object->value, 0);
    if(!is_root)
        printf(",");
    printf("\n");
}

void __pretty_print_array_rec(struct __json_array* array, int tabs, char is_last) {
    if(!array)
        return;
    __pretty_print_array_rec(array->next, tabs, 0);
    __pretty_print_rec(array->value, tabs);
    if(!is_last)
        printf(", ");
}

void __pretty_print_rec(JSON* json, int tabs) {
    if(!json || json->type == NONE)
        return;
    for(int i = 0; i < tabs; i++) 
        printf("    ");

    switch(json->type) {
        case OBJECT:
            printf("{");
            if(json->data.object) {
                printf("\n");
                __pretty_print_object_rec(json->data.object, tabs + 1, 1);
            }
            printf("}");
            break;
        case ARRAY:
            printf("[");
            __pretty_print_array_rec(json->data.array, tabs, 1);
            printf("]");
            break;
        case STRING:
            printf("\"%s\"", json->data.str->string);
            break;
        case INT:
            printf("%i", json->data.i->i);
            break;
        case BOOLEAN:
            if(json->data.bool->bool)
                printf("true");
            else
                printf("false");
            break;
            
    }
}

void json_pretty_print(JSON* json) {
    __pretty_print_rec(json, 0);
}

// Later on
JSON* json_free_object(JSON* json, char* key);
int json_array_set(JSON* json, uint32_t index, JSON* value);
int json_array_free(JSON* json, uint32_t index);
int json_length(JSON* json);
JSON* json_clone(JSON* json);