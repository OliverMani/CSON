#include <cson/validator.h>
#include <cson/structures.h>
#include <stdlib.h>
#include <string.h>

struct substring {
    char* begin;
    char* end;
};

static void skip_whitespace(char** cursor) {
    while(**cursor == ' ' || **cursor == '\t' || **cursor == '\r' || **cursor == '\n')
        ++(*cursor);
}

static inline struct substring* makesubstring(char* begin, char* end) {
    struct substring* result = (struct substring*) malloc(sizeof(struct substring));
    result->begin = begin;
    result->end = end;
    return result;
}

static inline char has_extra_data(char* string, char isolated) {
    
    skip_whitespace(&string);
    // If we have extra data and string is supposed to be isolated but it is not
    return ((*string == ',' || *string == ']' || *string == '}' || *string == ':') && isolated) || (isolated && *string != 0);
}

struct substring* get_string_substring(char* string, char isolated) {
    skip_whitespace(&string);
    if(*string != '"')
        return NULL;
    char* begin = string;
    string++;
    char last_was_backslash = 0;
    for(; *string && *string != '"' || last_was_backslash; string++) {
        if(last_was_backslash) {
            last_was_backslash = 0;
        } else if (*string == '\\') {
            last_was_backslash = 1;
        }
    }
    if(!string)
        return NULL;
    if(has_extra_data(string + 1, isolated))
        return NULL;
    
    return makesubstring(begin, string + 1);
    
}

char is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

struct substring* get_int_substring(char* string, int isolated) {
    skip_whitespace(&string);
    if(!is_digit(*string) && *string != '-' && *string != '+')
        return NULL;
    char* begin = string;
    if(*string == '-' || *string == '+')
        string++;
    while(is_digit(*string))
        string++;
    if(has_extra_data(string, isolated))
        return NULL;
    return makesubstring(begin, string);
}
struct substring* get_boolean_substring(char* string, char isolated) {
    skip_whitespace(&string);
    if(*string != 't' && *string != 'f')
        return NULL;
    // Programming Horror
    // Go grab some pop corn and soda
    //
    if(strlen(string) < 4) // If string to too short
        return NULL;
    if(string[0] == 't' && string[1] == 'r' && string[2] == 'u' && string[3] == 'e') { // Else if string (boolean) is true
        if(has_extra_data(string + 4, isolated))
            return NULL;
        return makesubstring(string, string + 4);
    } else if(string[0] == 'f' && string[1] == 'a' && string[2] == 'l' && string[3] == 's' && string[4] == 'e') { // Else if string (boolean) is false
        if(has_extra_data(string + 5, isolated))
            return NULL;
        return makesubstring(string, string + 5);
    }
    return NULL;  // Otherwise we have an error
}

struct substring* get_object_substring(char* string, char isolated);


struct substring* get_array_substring(char* string, char isolated) {
    skip_whitespace(&string);
    if(*string != '[')
        return NULL;
    char* begin = string;
    char is_beginning = 1;
    struct substring* substring = NULL;
    string++;
    skip_whitespace(&string);
    while(*string && *string != ']') {
        
        skip_whitespace(&string);
        if(substring) {
            // We do not want memory leak
            free(substring);
            substring = NULL;
        }
        enum Type type = cson_get_unpacked_type(*string);
        switch(type) {
            case ARRAY:
                substring = get_array_substring(string, 0);
                break;
            case INT:
                substring = get_int_substring(string, 0);
                break;
            case STRING:
                substring = get_string_substring(string, 0);
                break;
            case BOOLEAN:
                substring = get_boolean_substring(string, 0);
                break;
            case OBJECT:
                substring = get_object_substring(string, 0);
                break;
            case NONE:
                if(is_beginning)
                    return NULL;
            case WHITESPACE:
                skip_whitespace(&string);
                if(*string == ']')
                    return NULL;
                string++;
                continue;
            default:
                return NULL;
        }
        if(substring) {
            string = substring->end + 1;
        }
        else break;
        is_beginning = 0;
        
    }
    if(has_extra_data(string, isolated) && !(*begin == '[' && *string == ']' && is_beginning))
        return NULL;
    return makesubstring(begin, string);

}

struct substring* get_object_substring(char* string, char isolated) {
    skip_whitespace(&string);
    if(*string != '{') 
        return NULL;
    char* begin = string;
#define CSON_TMP_KEY_MODE 0
#define CSON_TMP_VALUE_MODE 1
    char mode = CSON_TMP_KEY_MODE; // 0 = Key mode, 1 = Value mode
    char is_beginning = 1;
    //string++;
    skip_whitespace(&string);
    while(*string && *string != '}') {
        string++;
        skip_whitespace(&string);
        
        struct substring* key = get_string_substring(string, 0);
        if(!key)
            return NULL;
        string = key->end;
        skip_whitespace(&string);
        free(key);
        if(*string != ':')
            return NULL;
        string++;
        skip_whitespace(&string);
        enum Type type = cson_get_unpacked_type(*string);
        struct substring* substring = NULL;
        switch(type) {
            case INT:
                substring = get_int_substring(string, 0);
                break;
            case STRING:
                substring = get_string_substring(string, 0);
                break;
            case BOOLEAN:
                substring = get_boolean_substring(string, 0);
                break;
            case ARRAY:
                substring = get_array_substring(string, 0);
                break;
            case OBJECT:
                substring = get_array_substring(string, 0);
                break;
            case NONE:
                if(is_beginning)
                    return NULL;
            case WHITESPACE:
                skip_whitespace(&string);
                if(*string == '}')
                    return NULL;
                string++;
                continue;
            default:
                return NULL;
        }

        if(!substring)
            return NULL;
        
        string = substring->end;
        
        skip_whitespace(&string);


        free(substring);

        is_beginning = 0;

        if(*string != ',' && *string != '}')
            return NULL;
        
    }
    if(*string != '}')
        return NULL;
#undef CSON_TMP_KEY_MODE
#undef CSON_TMP_KEY_MODE
    if(has_extra_data(string+1, isolated))
        return NULL;
    return makesubstring(begin, string);
}


char cson_is_valid(char* string) {
    skip_whitespace(&string);
    struct substring* substring = NULL;
    if(*string == '"') {
        substring = get_string_substring(string, 1);
    } else if(is_digit(*string) || *string == '+' || *string == '-') {
        substring = get_int_substring(string, 1);
    } else if(*string == 't' || *string == 'f') {
        substring = get_boolean_substring(string, 1);
    } else if(*string == '[') {
        substring = get_array_substring(string, 1);
    } else if(*string == '{') {
        substring = get_object_substring(string, 1);
    }
    if(!substring)
        return 0;
    free(substring);
    return 1;
}

