#ifndef CSON_IO_H
#define CSON_IO_H
#include "cson.h"
#include <stdio.h>

CSON* cson_load(FILE* file);
//CSON* cson_load(char* filename);
CSON* cson_loads(char* str);

void cson_fprintf(FILE* file, CSON* cson);
void cson_printf(CSON* cson);
int cson_save(char* filename, CSON* cson);

#endif