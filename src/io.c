#include <cson/io.h>
#include <string.h>
#include <cson/structures.h>
#include <stdint.h>
#include <stdlib.h>

struct substring_data {
    uint32_t begin;
    uint32_t end;
    enum Type type;
};

// Test JSON:
// {"hello": 10}





char* prepare_substring_to_unpack(char* str, uint32_t start, uint32_t end) {
    char* result = (char*) malloc(end - start + 1);
    strncpy(result, str + start, end - start);
    result[end - start] = 0;
    return result;
}

char* unpack_string_to_string(char* str) {
    char in_string = 1, last_was_backslash = 0;
    while(*str != '"') str++; // Find double quote
    char* begin = ++str;
    do {
        if(!last_was_backslash && *str == '\\')
            last_was_backslash = 1;
        else if(last_was_backslash) 
            last_was_backslash = 0;
        else if(!*str || (!last_was_backslash && *str == '"'))
            in_string = 0;
        str++;
    } while(in_string);
    if(in_string) {
        return NULL;
    }
    char* result = (char*) malloc(str - begin);
    strncpy(result, begin, (str - begin)-1);
    result[str - begin] = 0;
    return result;
}

CSON* unpack_string(char* str) {
    CSON* result = (CSON*) malloc(sizeof(CSON));
    result->type = STRING;
    result->data.str = (struct __cson_string*) malloc(sizeof(struct __cson_string));
    result->data.str->string = unpack_string_to_string(str);
    return result;
}

//r/programminghorror
CSON* unpack_boolean(char* str) {
    while(1) {
        switch(*str) {
            case 't':
            case 'f':
            goto after;
            case ' ':
            case '\n':
            case '\t':
            case '\f':
            case '\r':
            case '\v':
            break;
            default:
            goto error;
        }
        str++;
    }
    error:
    return NULL;
    after:
    // Check if the boolean is really true or false
    if((str[0] == 't' && str[1] == 'r' && str[2] == 'u' && str[3] == 'e' && !str[4])
        || (str[0] == 'f' && str[1] == 'a' && str[2] == 'l' && str[3] == 's' && str[4] == 'e') && !str[5]) {
        CSON* cson = (CSON*) malloc(sizeof(CSON));
        cson->type = BOOLEAN;
        struct __cson_boolean* bool = (struct __cson_boolean*) malloc(sizeof(struct __cson_boolean));
        bool->bool = *str == 't';
        cson->data.bool = bool;
        return cson;
    } else goto error;
}

int64_t str_to_int(char* begin, char* end) {
    int64_t result = 0;
    char* current = begin;
    char negative = *begin == '-';
    if(negative) current++;
    while(current != end) {
        result *= 10;
        result += *current - '0';
        current++;
    }
    if(negative)
        result = ~result + 1;
    return result;
}

CSON* unpack_int(char* str) {
    // Search for number
    while(1) {
        switch(*str) {
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            goto after;
            case ' ':
            case '\n':
            case '\t':
            case '\f':
            case '\r':
            case '\v':
            break;
            default:
            goto error;
        }
        str++;
    }
    error:
    return NULL;
    after:
    char* begin = str;
    if(*str == '-') str++;
    for(; *str && *str >= '0' && *str <= '9'; str++);
    if(*str) return NULL;
    CSON* result = (CSON*) malloc(sizeof(CSON));
    result->type = INT;
    result->data.i = (struct __cson_int*) malloc(sizeof(struct __cson_int*));
    result->data.i->i = str_to_int(begin, str);
    return result;
}

CSON* unpack_array(char* str);

CSON* unpack_object(char* str);

CSON* cson_load(FILE* file) {
    if(!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    char* buffer = (char*) malloc(size); // We use malloc in case if the file is big
    fseek(file, 0, SEEK_SET);
    fread(buffer, size, 1, file);
    CSON* result = cson_loads(buffer);
    free(buffer);
    return result;
}
//CSON* cson_load(char* filename);

// Unpacks from string to CSON object
CSON* cson_loads(char* str) {
    enum Type type = WHITESPACE;
    // Jump over whitespace
    do {
        type = cson_get_unpacked_type(*str);
        if(type == WHITESPACE)
            str++;
        if(!*str)
            return NULL;
    } while (type == WHITESPACE);

    CSON* result = NULL;

    switch (type) {
        case STRING:
            result = unpack_string(str);
            break;
        case BOOLEAN:
            result = unpack_boolean(str);
            break;
        case INT:
            result = unpack_int(str);
            break;
        case ARRAY:
        case OBJECT:
            break;
    }

    return result;
}

void cson_fprintf_rec(FILE* file, CSON* cson);

void cson_fprintf_object_rec(FILE* file, struct __cson_object* object, char root) {
    if(!object)
        return;
    cson_fprintf_object_rec(file, object->left_node, 0);
    cson_fprintf_object_rec(file, object->right_node, 0);
    fprintf(file, "\"%s\":", object->key);
    cson_fprintf_rec(file, object->value);
    if(!root)
        fprintf(file, ",");
}

void cson_fprintf_array_rec(FILE* file, struct __cson_array* array, char last) {
    if(!array)
        return;
    cson_fprintf_array_rec(file, array->next, 0);
    cson_fprintf_rec(file, array->value);
    if(!last)
        fprintf(file, ",");
}

void cson_fprintf_rec(FILE* file, CSON* cson) {
    if(!cson)
        return;
    switch(cson->type) {
        case BOOLEAN:
            if(cson->data.bool->bool)
                fprintf(file, "true");
            else
                fprintf(file, "false");
            break;
        case INT:
            fprintf(file, "%i", cson->data.i->i);
            break;
        case STRING:
            fprintf(file, "\"%s\"", cson->data.str->string);
            break;
        case ARRAY:
            fprintf(file, "[");
            cson_fprintf_array_rec(file, cson->data.array, 1);
            fprintf(file, "]");
            break;
        case OBJECT:
            fprintf(file, "{");
            cson_fprintf_object_rec(file, cson->data.object, 1);
            fprintf(file, "}");
            break;
    }
}



void cson_fprintf(FILE* file, CSON* cson) {
    if(!cson || !file)
        return;
    cson_fprintf_rec(file, cson);
}

void cson_printf(CSON* cson) {
    cson_fprintf(stdout, cson);
}

int cson_save(char* filename, CSON* cson) {
    FILE* file = fopen(filename, "w");
    if(!file) 
        return 1;
    cson_fprintf(file, cson);
    fclose(file);
    return 0;
}