#include "mem.h"
#include "../reals/log.cpp"
#include <stdlib.h>

void mem_dump(address adr, int size);
void load_file(const char * filename);
void load_data(FILE * stream);

#include "../reals/load_dump.cpp"