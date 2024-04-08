#pragma once

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define REGSIZE 8
#define pc reg[REGSIZE - 1]

// Для функций w_read, w_write. Если работаем с регистрами, то REGSPACE, если с mem, то MEMSPACE
#define REGSPACE 1
#define MEMSPACE 0

void reg_dump(void);

extern word reg[REGSIZE];

byte b_read(address adr);
void b_write(address adr, byte value);
word w_read(address adr, int reg_space);
void w_write(address adr, word value, int reg_space);

