#pragma once

#include "mem.h"

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_NN 4
#define HAS_R 8
#define HAS_XX 16

extern char BYTE_COMMAND;


typedef struct {
    word mask;
    word opcode;
    const char * name;
    void (* do_command)(void);
    char params;
} Command;

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

typedef struct {
    char val;
} Arg_XX;

extern Arg SS_ARG;
extern Arg DD_ARG;
extern Arg_R R_ARG;
extern Arg_NN NN_ARG;
extern Arg_XX XX_ARG;


Arg get_mr(word w);
Arg_R get_r(word w);
Arg_NN get_nn(word w);
Arg_XX get_xx(word w);


void do_halt(void);
void do_add(void);

void do_mov(void);
void do_movb(void);

void do_sob(void);

void do_clr(void);

void do_br(void);

void do_cmp(void);
void do_cmpb(void);

void do_tst(void);
void do_tstb(void);

void do_ccc();
void do_clc();
void do_cln();
void do_clv();
void do_clz();
void do_nop();
void do_scc();
void do_sec();
void do_sen();
void do_sev();
void do_sez();

void do_bcc();
void do_bcs();
void do_beq();
void do_bge();
void do_bgt();
void do_bhi();
void do_bhis();
void do_ble();
void do_blt();
void do_blo();
void do_blos();
void do_bmi();
void do_bne();
void do_bpl();
void do_bvc();
void do_bvs();

void do_nothing(void);




