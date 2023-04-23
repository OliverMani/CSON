#include <stdio.h>
#include <json/json.h>

#define myerr(error) if(error) fprintf(stderr, "Error: Something's wrong\n")

int main() {
    JSON* json = json_object();
    JSON* array = json_array();
    int error;
    error = json_array_append_string(array, "Banani");
    myerr(error);
    error = json_array_append_string(array, "Epli");
    myerr(error);
    error = json_array_append_string(array, "Pera");
    myerr(error);
    error = json_array_append_int(array, 10);
    myerr(error);
    error = json_array_append_boolean(array, 1);
    myerr(error);
    error = json_array_append_boolean(array, 0);
    myerr(error);
    
    JSON* json2 = json_object();
    error = json_set_object_as_int(json2, "Test", 15);
    myerr(error);

    json_set_object(json, "Hello", array);
    json_set_object(json, "Hello2", json_object());
    //json_set_object(json, "AnotherObj", json2);
    json_array_append(array, json2);
    json_pretty_print(json);
    printf("\n\n\n");
    json_free(json);
    
    
    json_pretty_print(json);
    //json_pretty_print(json2);
}