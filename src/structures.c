#include <cson/structures.h>

enum Type cson_get_unpacked_type(char ch) {
    switch(ch) {
        case 't':
        case 'f':
            return BOOLEAN;
        case '{':
            return OBJECT;
        case '[':
            return ARRAY;
        case '"':
            return STRING;
        case '-':
        case '+':
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
            return INT;
    }
    if(cson_is_whitespace(ch))
        return WHITESPACE;
    return NONE;
}

static inline char cson_is_whitespace(char ch) {
    switch(ch) {
        case ' ':
        case '\n':
        case '\t':
        case '\f':
        case '\r':
        case '\v':
        return 1;
    }
    return 0;
}