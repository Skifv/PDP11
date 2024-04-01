#pragma once

#include "mem.h"

void mem_dump(address adr, int size);
void load_file(const char * filename);
void load_data(FILE * stream);
