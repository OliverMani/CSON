/*
* THIS IS A TESTING FILE, THIS SHOULD NOT BE A PART OF THE LIBRARY ITSELF
*/
#include <stdio.h>
#include <cson/cson.h>
#include <cson/io.h>
#include <cson/validator.h>

#define myerr(error) if(error) fprintf(stderr, "Error: Something's wrong\n")

/*int main() {
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

    cson_set_object_as_string(json, "Hell", "NOPE"),
    error = cson_set_object(json, "Hello", array);
    myerr(error);
    cson_set_object(json, "Hello2", cson_object());
    //json_set_object(json, "AnotherObj", json2);
    cson_array_append(array, json2);
    cson_pretty_print(json);
    error = cson_free_object(json, "Hello");
    myerr(error);
    printf("\nNew CSON: ");
    cson_pretty_print(json);

    printf("\n\n\nClone: ");
    CSON* clone = cson_clone(json);
    cson_pretty_print(clone);
    //json_pretty_print(json2);
}*/

int main(){
    //char* input = "[10, false, true     ,  false, false, \"Hello\"]";
    char* input = "{\"Hello\": \"World\", \"Test\": 10}";
    CSON* string = cson_loads(input);
    char valid = cson_is_valid(input);
    printf("Valid: %s\n", valid ? "True" : "false");
    cson_pretty_print(string);
}