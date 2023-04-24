#include <cson/cson.h>
#include <cson/structures.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum Type cson_get_type(CSON* cson) {
    if(!cson)
        return NONE;
    return cson->type;
}

char* cson_get_string(CSON* cson) {
    if(!cson || cson->type != STRING)
        return NULL;
    return cson->data.str->string;
}

int64_t cson_get_int(CSON* cson) {
    if(!cson || cson->type != INT)
        return 0;
    return cson->data.i->i;
}

char cson_get_boolean(CSON* cson) {
    if(!cson || cson->type != BOOLEAN)
        return 0;
    return cson->data.bool->bool;
}

CSON* cson_string(char* str) {
    CSON* result = (CSON*) malloc(sizeof(CSON));
    result->type = STRING;
    struct __cson_string* stringobj = (struct __cson_string*) malloc(sizeof(struct __cson_string));
    char* string = (char*) malloc(strlen(str) + 1);
    strcpy(string, str);
    stringobj->string = string;
    result->data.str = stringobj;
    return result;
}
CSON* cson_int(int64_t i) {
    CSON* result = (CSON*) malloc(sizeof(CSON));
    result->type = INT;
    struct __cson_int* iobj = (struct __cson_int*) malloc(sizeof(struct __cson_int));
    iobj->i = i;
    result->data.i = iobj;
    return result;
}
CSON* cson_boolean(char bool) {
    CSON* result = (CSON*) malloc(sizeof(CSON));
    result->type = BOOLEAN;
    struct __cson_boolean* bobj = (struct __cson_boolean*) malloc(sizeof(struct __cson_boolean));
    bobj->bool = bool;
    result->data.bool = bobj;
    return result;
}

int cson_set_string(CSON* cson, char* str) {
    if(!cson || cson->type != STRING)
        return 1;
    free(cson->data.str->string);
    cson->data.str->string = (char*) malloc(strlen(str) + 1);
    strcpy(cson->data.str->string, str);
    return 0;
}

int cson_set_int(CSON* cson, int64_t i) {
    if(!cson || cson->type != INT)
        return 1;
    cson->data.i->i = i;
    return 0;
}
int cson_set_boolean(CSON* cson, char bool) {
    if(!cson || cson->type != INT)
        return 1;
    cson->data.bool->bool = bool;
    return 0;
}

static inline struct __cson_object* __create_object(struct __cson_object* left, struct __cson_object* right, char* key, CSON* value) {
    struct __cson_object* object = (struct __cson_object*) malloc(sizeof(struct __cson_object));
    object->left_node = NULL;
    object->right_node = NULL;
    object->value = value;
    object->key = key;
    return object;
}

int cson_set_object(CSON* cson, char* key, CSON* value) {
    if(!cson || cson->type != OBJECT)
        return 1; // Return error
    struct __cson_object* current = cson->data.object;
    struct __cson_object* prev = current;
    char* new_key = (char*) malloc(sizeof(char) * strlen(key) + 1);
    strcpy(new_key, key);
    while(1) {
        if(!current && prev) {
            current = __create_object(NULL, NULL, new_key, value);
            if(strcmp(prev->key, current->key) < 0) {
                prev->right_node = current;
            } else {
                prev->left_node = current;
            }
            break;
        } else if(!current) {
            cson->data.object = __create_object(NULL, NULL, new_key, value);
            break;
        }
        int compare = strcmp(key, current->key);
        // If current key IS key (node already defined)
        if(!compare) {
            // We do not want to cause memory leak
            free(new_key);
            // If programmer uses cson_object_set with the same object as value, we don't want to erase it!
            if(current->value != value) {
                cson_free(current->value);
                current->value = value;
            }
            break;
        } else if (compare < 0) {
            prev = current;
            current = current->left_node;
        } else if (compare > 0) {
            prev = current;
            current = current->right_node;
        }
        
    }

    return 0;
}

int cson_set_object_as_string(CSON* cson, char* key, char* string) {
    if(!cson)
        return 1;
    CSON* strobj = cson_string(string);
    return cson_set_object(cson, key, strobj);
}
int cson_set_object_as_int(CSON* cson, char* key, int64_t i) {
    if(!cson)
        return 1;
    CSON* iobj = cson_int(i);
    return cson_set_object(cson, key, iobj);
}

int cson_set_object_as_boolean(CSON* cson, char* key, char bool) {
    if(!cson)
        return 1;
    CSON* bobj = cson_boolean(bool);
    return cson_set_object(cson, key, bobj);
}

char* cson_get_object_as_string(CSON* cson, char* key) {
    CSON* object = cson_get_object(cson, key);
    if(!object || object->type != STRING)
        return NULL;
    return object->data.str->string;
}

int64_t cson_get_object_as_int(CSON* cson, char* key) {
    CSON* object = cson_get_object(cson, key);
    if(!object || object->type != INT)
        return 0;
    return object->data.i->i;
}

char cson_get_object_as_boolean(CSON* cson, char* key) {
    CSON* object = cson_get_object(cson, key);
    if(!object || object->type != BOOLEAN)
        return 0;
    return object->data.bool->bool;
}

CSON* cson_get_object(CSON* cson, char* key) {
    if(!cson || !key)
        return NULL;
    struct __cson_object* current = cson->data.object;
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




int cson_array_append(CSON* cson, CSON* value) {
    if(!cson)
        return 1;
    if(!cson->data.array)
        return cson_array_insert(cson, value, 0);
    return cson_array_insert(cson, value, cson->data.array->index + 1);
}
int cson_array_append_string(CSON* cson, char* string) {
    return cson_array_append(cson, cson_string(string));
}
int cson_array_append_int(CSON* cson, int64_t i) {
    return cson_array_append(cson, cson_int(i));
}
int cson_array_append_boolean(CSON* cson, char bool) {
    return cson_array_append(cson, cson_boolean(bool));
}


static inline struct __cson_array* create_node(CSON* value, uint32_t index, struct __cson_array* next) {
    struct __cson_array* node = (struct __cson_array*) malloc(sizeof(struct __cson_array));
    node->index = index;
    node->value = value;
    node->next = next;
    return node;
}

/*
* Walks backwards through the array to add
* <--
*/
int cson_array_insert(CSON* cson, CSON* value, uint32_t index) {
    if(!cson || cson->type != ARRAY)
        return 1; // Error
    struct __cson_array* current = cson->data.array;
    if(!current) {
        if(index)
            return 1; // Error
        cson->data.array = create_node(value, 0, NULL);
    } else if(index == current->index + 1) {
        struct __cson_array* new = create_node(value, index, current);
        cson->data.array = new;
    } else {
        if(index > current->index)
            return 1;
        while(current->index != index) {
            current->index++;
            current = current->next;
        }

        current->index++;
        struct __cson_array* next = current->next;
        current->next = create_node(value, index, next);
    }
    return 0;
}

int cson_array_insert_string(CSON* cson, char* string, uint32_t index) {
    return cson_array_insert(cson, cson_string(string), index);
}
int cson_array_insert_int(CSON* cson, uint64_t i, uint32_t index) {
    return cson_array_insert(cson, cson_int(i), index);
}
int cson_array_insert_boolean(CSON* cson, char bool, uint32_t index) {
    return cson_array_insert(cson, cson_boolean(bool), index);
}

CSON* cson_array_get(CSON* cson, uint32_t index) {
    if(!cson || cson->type != ARRAY || cson->data.array == NULL || cson->data.array->index < index)
        return NULL;
    struct __cson_array* current = cson->data.array;
    while(current->index != index)
        current = current->next;
    return current->value;
}




/* Create an empty object */
CSON* cson_object() {
    CSON* new = (CSON*) malloc(sizeof(CSON));
    new->type = OBJECT;
    new->data.object = NULL;
    return new;
}

/* Create an empty array */
CSON* cson_array() {
    CSON* new = (CSON*) malloc(sizeof(CSON));
    new->type = ARRAY;
    new->data.array = NULL;
    return new;
}

int __objfree(struct __cson_object* obj) {
    if(!obj)
        return 0;
    struct __cson_object* left = obj->left_node;
    struct __cson_object* right = obj->right_node;
    __objfree(left);
    __objfree(right);
    if(obj->key) {
        free(obj->key);
        obj->key = NULL;
    }
    if(obj->value)
        cson_free(obj->value);
    if(obj) {
        free(obj);
        obj = NULL;
    }
    return 0;
}

int __arrfree(struct __cson_array* array) {
    struct __cson_array* current = array;
    while(current) {
        struct __cson_array* next = current->next;
        if(current->value)
            cson_free(current->value);
        free(current);
        current = next;
    }
    return 0;
}

int cson_free(CSON* cson) {
    if(!cson)
        return 1;
    switch(cson->type) {
        case STRING:
            if(cson->data.str->string) {
                free(cson->data.str->string);
                cson->data.str->string = NULL;
            }
            if(cson->data.str) {
                free(cson->data.str);
                cson->data.str = NULL;
            }
            free(cson);
            break;
        case INT:
            if(cson->data.i) {
                free(cson->data.i);
                cson->data.i = NULL;
            }
            free(cson);
            break;
        case BOOLEAN:
            if(cson->data.bool) {
                free(cson->data.bool);
                cson->data.bool = NULL;
            }
            free(cson);
            break;
        case OBJECT:
            __objfree(cson->data.object);
            cson->data.object = NULL;
            free(cson);
            break;
        case ARRAY:
            __arrfree(cson->data.array);
            cson->data.array = NULL;
            free(cson);
            break;
    }
    return 0;
}




void __pretty_print_rec(CSON* cson, int tabs);

void __pretty_print_object_rec(struct __cson_object* object, int tabs, char is_root) {
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

void __pretty_print_array_rec(struct __cson_array* array, int tabs, char is_last) {
    if(!array)
        return;
    __pretty_print_array_rec(array->next, tabs, 0);
    __pretty_print_rec(array->value, tabs);
    if(!is_last)
        printf(", ");
}

void __pretty_print_rec(CSON* cson, int tabs) {
    if(!cson || cson->type == NONE)
        return;
    for(int i = 0; i < tabs; i++) 
        printf("    ");

    switch(cson->type) {
        case OBJECT:
            printf("{");
            if(cson->data.object) {
                printf("\n");
                __pretty_print_object_rec(cson->data.object, tabs + 1, 1);
            }
            printf("}");
            break;
        case ARRAY:
            printf("[");
            __pretty_print_array_rec(cson->data.array, tabs, 1);
            printf("]");
            break;
        case STRING:
            printf("\"%s\"", cson->data.str->string);
            break;
        case INT:
            printf("%i", cson->data.i->i);
            break;
        case BOOLEAN:
            if(cson->data.bool->bool)
                printf("true");
            else
                printf("false");
            break;
        default:
            printf("null");
            break;
            
    }
}

void cson_pretty_print(CSON* cson) {
    if(!cson)
        printf("null\n");
    __pretty_print_rec(cson, 0);
}

int cson_array_set(CSON* cson, uint32_t index, CSON* value) {
    if(!cson)
        return 1;
    struct __cson_array* current = cson->data.array;
    if(!current)
        return 1;
    while(current && current->index != index)
        current = current->next;
    if(!current)
        return 1;
    if(current->value != value) {
        cson_free(current->value);
        current->value = value;
    }
    return 0;
}

int __object_length(struct __cson_object* obj) {
    if(!obj)
        return 0;
    return 1 + __object_length(obj->left_node) + __object_length(obj->right_node);
}

int cson_length(CSON* cson) {
    if(!cson)
        return -1;
    switch (cson->type)
    {
    case OBJECT:
        return __object_length(cson->data.object);
    case ARRAY:
        if(!cson->data.array)
            return 0;
        return cson->data.array->index + 1;
    case STRING:
        if(!cson->data.str)
            return 0;
        return strlen(cson->data.str->string);
    default:
        return -1;
    }
}

int cson_array_free(CSON* cson, uint32_t index) {
    if(!cson || index < 0 || !cson->data.array || index > cson->data.array->index)
        return 1;
    struct __cson_array* current = cson->data.array;
    struct __cson_array* prev = current;
    while(current->index != index) {
        current->index--;
        prev = current;
        current = current->next;
    }
    prev->next = current->next;
    cson_free(current->value);
    free(current);
    return 0;
}

int cson_free_object(CSON* cson, char* key) {
    if(!cson || !cson->data.object)
        return 1;
    struct __cson_object* current = cson->data.object;
    struct __cson_object* parent = current;
    int compare;

    while(current) {
        parent = current;
        compare = strcmp(key, current->key);
        if(compare < 0)
            current = current->left_node;
        else if(compare > 0)
            current = current->right_node;
        else break;
    }
    if(!current)
        return 1;
    if(!current->left_node && !current->right_node) {
        if(current == cson->data.object) {
            cson->data.object = NULL;
        } else {
            if(current == parent->left_node)
                parent->left_node = NULL;
            else
                parent->right_node = NULL;
        }
        cson_free(current->value);
        free(current);
    } else if(!current->left_node) {
        struct __cson_object* right = current->right_node;
        cson_free(current->value);
        current->value = right->value;
        current->left_node = right->left_node;
        current->right_node = right->right_node;
        
        free(current->key);
        current->key = right->key;
        free(right);
    } else if(!current->right_node) {
        struct __cson_object* left = current->left_node;
        cson_free(current->value);
        current->value = left->value;
        current->left_node = left->left_node;
        current->right_node = left->right_node;
        
        free(current->key);
        current->key = left->key;
        free(left);
    } else { // If object tree has subtrees on both sides
        struct __cson_object* temp = current->right_node;
        struct __cson_object* tempParent = current;
        free(temp->key);
        cson_free(temp->value);
        while(temp->left_node) {
            tempParent = temp;
            temp = temp->left_node;
            tempParent->key = temp->key;
            tempParent->value = temp->value;
        }
        free(tempParent->left_node);
        tempParent->left_node = NULL;

    }
    return 0;
}

struct __cson_object* __object_clone(struct __cson_object* object) {
    if(!object) return NULL;
    struct __cson_object* clone = (struct __cson_object*) malloc(sizeof(struct __cson_object));
    clone->key = (char*) malloc(strlen(object->key) + 1);
    strcpy(clone->key, object->key);
    clone->value = cson_clone(object->value);
    clone->left_node = __object_clone(object->left_node);
    clone->right_node = __object_clone(object->right_node);
    return clone;
}

struct __cson_array* __array_clone(struct __cson_array* array) {
    if(!array) return NULL;
    struct __cson_array* clone = (struct __cson_array*) malloc(sizeof(struct __cson_array));
    clone->index = array->index;
    clone->value = cson_clone(array->value);
    clone->next = __array_clone(array->next);
    return clone;
}

/*
    Clones an CSON object
*/
CSON* cson_clone(CSON* cson) {
    CSON* clone = (CSON*) malloc(sizeof(CSON));
    clone->type = cson->type;
    switch(cson->type) {
        case BOOLEAN:
            if(!cson->data.bool)
                clone->data.bool = NULL;
            else {
                clone->data.bool = (struct __cson_boolean*) malloc(sizeof(struct __cson_boolean));
                clone->data.bool->bool = cson->data.bool->bool;
            }
            break;
        case INT:
            if(!cson->data.i)
                clone->data.i = NULL;
            else {
                clone->data.i = (struct __cson_int*) malloc(sizeof(struct __cson_int));
                clone->data.i->i = cson->data.i->i;
            }
            break;
        case STRING:
            if(!cson->data.str)
                clone->data.str = NULL;
            else {
                char* string = cson->data.str->string;
                clone->data.str = (struct __cson_string*) malloc(sizeof(struct __cson_string));
                if(!string)
                    clone->data.str->string = NULL;
                else {
                    clone->data.str->string = (char*) malloc(strlen(string) + 1);
                    strcpy(clone->data.str->string, string);
                }
            }
            break;
        case OBJECT:
            if(!cson->data.object)
                clone->data.object = NULL;
            else {
                struct __cson_object* object = cson->data.object;
                clone->data.object = __object_clone(object);
            }
            break;
        case ARRAY:
            if(!cson->data.array)
                clone->data.array = NULL;
            else {
                struct __cson_array* array = cson->data.array;
                clone->data.array = __array_clone(array);
            }
            break;
    }
    return clone;
}