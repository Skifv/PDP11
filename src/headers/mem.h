#pragma once

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define REGSIZE 8
#define pc reg[REGSIZE - 1]

#define PDP11_MEMSIZE (64 * 1024)

// ��� ������� w_read, w_write. ���� �������� � ����������, �� REGSPACE, ���� � mem, �� MEMSPACE
#define REGSPACE 1
#define MEMSPACE 0

void reg_dump(void);

extern word reg[REGSIZE];

byte b_read(address adr);
void b_write(address adr, byte value);
word w_read(address adr, int reg_space);
void w_write(address adr, word value, int reg_space);

extern word original_reg_values[REGSIZE];
extern byte original_mem_values[PDP11_MEMSIZE];
void save_original_values(void);
void restore_original_values(void);
void cleanup(void);


