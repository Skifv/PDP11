#pragma once

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define REGSIZE 8
#define pc reg[REGSIZE - 1]

#define PDP11_MEMSIZE (64 * 1024)

// Для функций w_read, w_write. Если работаем с регистрами, то REGSPACE, если с mem, то MEMSPACE
#define REGSPACE 1
#define MEMSPACE 0

typedef struct {
    unsigned char N, Z, V, C;
} PWD;

extern PWD flags;

void set_C(int result);
void set_N(int result);
void set_Z(int result);

void reg_dump(void);
void flags_dump(void);

extern word reg[REGSIZE];

byte b_read(address adr, int reg_space);
void b_write(address adr, byte value, int reg_space);
word w_read(address adr, int reg_space);
void w_write(address adr, word value, int reg_space);

extern word original_reg_values[REGSIZE];
extern byte original_mem_values[PDP11_MEMSIZE];

void save_original_values(void);
void restore_original_values(void);
void cleanup(void);


