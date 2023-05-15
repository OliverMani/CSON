#ifndef CSON_IO_H
#define CSON_IO_H
#include "cson.h"
#include <stdio.h>

/**
 * **!!NOT READY!!**
 * This will load json from a json file and return a CSON object.
*/
CSON* cson_load(FILE* file);

/**
 * **!!NOT READY!!**
 * This will load a CSON object from a json string.
*/
CSON* cson_loads(char* str);

/**
 * This function writes a json file from a CSON object. No pretty printing.
*/
void cson_fprintf(FILE* file, CSON* cson);

/**
 * This function prints a CSON object. No pretty printing.
*/
void cson_printf(CSON* cson);

/**
 * This functions writes a CSON/json object to a file.
*/
int cson_save(char* filename, CSON* cson);

#endif