#include <stdio.h>
#include <cson/cson.h>

#define myerr(error) if(error) fprintf(stderr, "Error: Something's wrong\n")

int main() {
    CSON* json = cson_object();
    CSON* array = cson_array();
    int error;
    error = cson_array_append_string(array, "Banani");
    myerr(error);
    error = cson_array_append_string(array, "Epli");
    myerr(error);
    error = cson_array_append_string(array, "Pera");
    myerr(error);
    error = cson_array_append_int(array, 10);
    myerr(error);
    error = cson_array_append_boolean(array, 1);
    myerr(error);
    error = cson_array_append_boolean(array, 0);
    myerr(error);
    
    CSON* json2 = cson_object();
    error = cson_set_object_as_int(json2, "Test", 15);
    myerr(error);

    cson_set_object(json, "Hello", array);
    cson_set_object(json, "Hello2", cson_object());
    //json_set_object(json, "AnotherObj", json2);
    cson_array_append(array, json2);
    cson_pretty_print(json);
    printf("\n\n\nget Hello: ");
    CSON* get = cson_get_object(json, "Hello");
    cson_pretty_print(get);
    printf("\n\n\nget get 1: ");
    CSON* indextest = cson_array_get(get, 10); // Epli
    cson_pretty_print(indextest);
    printf("\n");

    //json_pretty_print(json2);
}