#pragma once

#include "mem.h"

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_R 4
#define HAS_NN 8

typedef struct {
    word mask;
    word opcode;
    const char * name;
    void (* do_command)(void);
    char params;
} Command;

#define N_COMMANDS (4 + 1) // 4 команды + unknown

extern Command command[];

typedef struct {
    word val;     
    address adr;   
    int reg_space;
} Arg;

typedef struct {
    address adr;    
} Arg_R;

typedef struct {
    word val;   
} Arg_NN;

extern Arg SS_ARG;
extern Arg DD_ARG;
extern Arg_R R_ARG;
extern Arg_NN NN_ARG;

Arg get_mr(word w);
Arg_R get_r(word w);
Arg_NN get_nn(word w);


void do_halt(void);
void do_add(void);
void do_mov(void);
void do_sob(void);
void do_nothing(void);




