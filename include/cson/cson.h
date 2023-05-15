#ifndef CSON_H
#define CSON_H
#include <stddef.h>
#include <stdint.h>

/**
 * This tells you what type the CSON/JSON object is.
 * Types:
 *  - STRING
 *  - INT
 *  - BOOLEAN
 *  - ARRAY
 *  - OBJECT
 *  - WHITESPACE (for library usage)
 *  - NONE (for library usage)
*/
enum Type;

/**
 * This is the CSON object, it might contain Object, Array, String, Boolean and Int, Float is not implemented yet but it'll come with time.
*/
typedef struct __cson CSON;

/**
 * Get the type of a CSON object
*/
enum Type cson_get_type(CSON* cson);

/**
 * Takes in CSON object and returns the string as C string from it.
 * Returns NULL if CSON is not string.
*/
char* cson_get_string(CSON* cson);

/**
 * Takes in CSON object and returns 64-bit int from it.
*/
int64_t cson_get_int(CSON* cson);

/**
 * Takes in CSON object and returns a boolean (0 as false or 1 as true).
*/
char cson_get_boolean(CSON* cson);

/**
 * Turns C string into a CSON string.
*/
CSON* cson_string(char* cson);

/**
 * Turns 64-bit int into CSON int.
*/
CSON* cson_int(int64_t i);
CSON* cson_boolean(char bool);

/**
 * Modifies CSON string.
*/
int cson_set_string(CSON* cson, char* str);

/**
 * Modifies CSON int.
*/
int cson_set_int(CSON* cson, int64_t i);

/**
 * Modifies CSON boolean
*/
int cson_set_boolean(CSON* cson, char bool);


/**
 * Finds a CSON string from CSON object with a key and modifies it.
*/
int cson_set_object_as_string(CSON* cson, char* key, char* string);

/**
 * Finds a CSON int from CSON object with a key and modifies it.
*/
int cson_set_object_as_int(CSON* cson, char* key, int64_t i);

/**
 * Finds a CSON boolean from CSON object with a key and modifies it.
*/
int cson_set_object_as_boolean(CSON* cson, char* key, char bool);

/**
 * Sets a object from key to a value.
*/
int cson_set_object(CSON* cson, char* key, CSON* value);

/**
 * Finds a string inside a CSON object and returns it from the key.
*/
char* cson_get_object_as_string(CSON* cson, char* key);

/**
 * Finds an 64-bit integer inside a CSON object and returns it from the key.
*/
int64_t cson_get_object_as_int(CSON* cson, char* key);

/**
 * Find a boolean inside a CSON object and returns it from the key.
*/
char cson_get_object_as_boolean(CSON* cson, char* key);

/**
 * Find a CSON object inside a CSON object from a key and returns it.
*/
CSON* cson_get_object(CSON* cson, char* key);

/**
 * Deletes an CSON object from memory by key
*/
int cson_free_object(CSON* cson, char* key);

/**
 * Takes in CSON array and puts any CSON value to the end.
*/
int cson_array_append(CSON* cson, CSON* value);

/**
 * Takes in CSON array and puts a string to the end. The string will turn into a CSON string.
*/
int cson_array_append_string(CSON* cson, char* string);

/**
 * Takes in CSON array and puts a 64-bit integer to the end. The integer will turn into a CSON integer.
*/
int cson_array_append_int(CSON* cson, int64_t i);

/**
 * Takes in CSON array and puts a boolean to the end. The boolean will turn into CSON boolean.
*/
int cson_array_append_boolean(CSON* cson, char bool);

/**
 * Takes in CSON array and inserts the value to a specific index of the array.
*/
int cson_array_insert(CSON* cson, CSON* value, uint32_t index);

/**
 * Takes in CSON array and inserts a string to a specific index of the array.
*/
int cson_array_insert_string(CSON* cson, char* string, uint32_t index);

/**
 * Takes in CSON array and inserts an 64-bit integer to a specific index of the array.
*/
int cson_array_insert_int(CSON* cson, uint64_t i, uint32_t index);

/**
 * Takes in CSON array and inserts a boolean to a specific index of the array.
*/
int cson_array_insert_boolean(CSON* cson, char bool, uint32_t index);

/**
 * Returns an CSON object from an index from CSON array
*/
CSON* cson_array_get(CSON* cson, uint32_t index);

/**
 * Updates an CSON object in a specific index in a CSON array
*/
int cson_array_set(CSON* cson, uint32_t index, CSON* value);

/**
 * Deletes an CSON object inside a CSON array. This will update all indexes after the chosen index.
*/
int cson_array_free(CSON* cson, uint32_t index);

/**
 * Creates an empty CSON object of type OBJECT.
*/
CSON* cson_object();

/**
 * Creates an empty CSON array of type ARRAY.
*/
CSON* cson_array();

/**
 * Gets length of CSON object;
 * Type OBJECT will return the count of nodes in the object.
 * Type ARRAY will return the length of the array.
 * Type STRING will return the length of the string.
*/
int cson_length(CSON* cson);

/**
 * Deletes an CSON object from memory. This function will also delete all children objects of this object recursivly.
*/
int cson_free(CSON* cson);

/**
 * Attempts to pretty print. It is not perfert tho.
*/
void cson_pretty_print(CSON* cson);

/**
 * Clones an CSON object. It is good to use if you're putting an object from object A to object B to prevent errors in object B if object A gets deleted.
*/
CSON* cson_clone(CSON* cson);

#endif

/*

*/