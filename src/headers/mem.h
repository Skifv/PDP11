#pragma once

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define REGSIZE 8
#define pc reg[REGSIZE - 1]

byte b_read(address adr);
void b_write(address adr, byte value);
word w_read(address adr);
void w_write(address adr, word value);

#include "../reals/mem.c"
