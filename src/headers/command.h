#pragma once

#include "mem.h"

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2

typedef struct {
    word mask;
    word opcode;
    const char * name;
    void (* do_command)(void);
    char params;
} Command;

#define N_COMMANDS (3 + 1)

extern Command command[];

typedef struct {
    word val;     // �������� (���)
    address adr;    // ����� (����)
    int reg_space;
} Arg;

extern Arg ss;
extern Arg dd;

Arg get_mr(word w);

void do_halt(void);
void do_add(void);
void do_mov(void);
void do_nothing(void);




