#pragma once

#include "mem.h"

typedef struct {
    word mask;
    word opcode;
    const char * name;
    void (* do_command)(void);
} Command;

#define N_COMMANDS (3 + 1)

extern Command command[];

typedef struct {
    word val;     // значение (что)
    address adr;    // адрес (куда)
    int reg_space;
} Arg;

extern Arg ss;
extern Arg dd;

Arg get_mr(word w);

void do_halt(void);
void do_add(void);
void do_mov(void);
void do_nothing(void);




