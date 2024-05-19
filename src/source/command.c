#include <stdlib.h>

#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"

Arg SS_ARG;
Arg DD_ARG;
Arg_R R_ARG;
Arg_NN NN_ARG;
Arg_XX XX_ARG;

char BYTE_COMMAND = 0;

address ostat = 0177564;
address odata = 0177566;

/* Все новые команды помещать до unknown */

Command command[] = 
{
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},

    {0170000, 0060000, "add",  do_add,  HAS_DD | HAS_SS},

    {0170000, 0010000, "mov",  do_mov,  HAS_DD | HAS_SS},
    {0170000, 0110000, "movb", do_movb, HAS_DD | HAS_SS},

    {0177000, 0077000, "sob",  do_sob,  HAS_R | HAS_NN},

    {0177700, 0005000, "clr",  do_clr,  HAS_DD},

    {0177700, 0005700, "tst",  do_tst,  HAS_DD}, 
    {0177700, 0105700, "tstb", do_tstb, HAS_DD},

    {0170000, 0020000, "cmp",  do_cmp,  HAS_DD | HAS_SS}, 
    {0170000, 0120000, "cmpb", do_cmpb, HAS_DD | HAS_SS}, 

    {0177000, 0004000, "jsr",  do_jsr,  HAS_R | HAS_DD},
    {0177770, 0000200, "rts",  do_rts,  HAS_R},


    {0177700, 0000400, "br", do_br, HAS_XX},
    {0177700, 0000500, "br", do_br, HAS_XX},
    {0177700, 0000600, "br", do_br, HAS_XX},
    {0177700, 0000700, "br", do_br, HAS_XX},

    {0103000, 0103000, "bcc",  do_bcc,  HAS_XX},
    {0103400, 0103400, "bcs",  do_bcs,  HAS_XX},
    {0001400, 0001400, "beq",  do_beq,  HAS_XX},
    {0002000, 0002000, "bge",  do_bge,  HAS_XX},
    {0003000, 0003000, "bgt",  do_bgt,  HAS_XX},
    {0101000, 0101000, "bhi",  do_bhi,  HAS_XX},
    {0103000, 0103000, "bhis", do_bhis, HAS_XX},
    {0003400, 0003400, "ble",  do_ble,  HAS_XX},
    {0002400, 0002400, "blt",  do_blt,  HAS_XX},
    {0103400, 0103400, "blo",  do_blo,  HAS_XX},
    {0101400, 0101400, "blos", do_blos, HAS_XX},
    {0100400, 0100400, "bmi",  do_bmi,  HAS_XX},
    {0001000, 0001000, "bne",  do_bne,  HAS_XX},
    {0100000, 0100000, "bpl",  do_bpl,  HAS_XX},
    {0102000, 0102000, "bvc",  do_bvc,  HAS_XX},
    {0102400, 0102400, "bvs",  do_bvs,  HAS_XX},

    {0177777, 000257, "ccc", do_ccc, NO_PARAMS},
    {0177777, 000241, "clc", do_clc, NO_PARAMS},
    {0177777, 000250, "cln", do_cln, NO_PARAMS},
    {0177777, 000242, "clv", do_clv, NO_PARAMS},
    {0177777, 000244, "clz", do_clz, NO_PARAMS},
    {0177777, 000240, "nop", do_nop, NO_PARAMS},
    {0177777, 000277, "scc", do_scc, NO_PARAMS},
    {0177777, 000261, "sec", do_sec, NO_PARAMS},
    {0177777, 000270, "sen", do_sen, NO_PARAMS},
    {0177777, 000262, "sev", do_sev, NO_PARAMS},
    {0177777, 000264, "sez", do_sez, NO_PARAMS},

    {0000000, 0000000, "unknown", do_nothing, NO_PARAMS}
};

void ostat_set_ready(void)
{
    byte ostate = b_read(ostat, MEMSPACE);
    ostate = ostate | 0200;
    b_write(ostat, ostate, MEMSPACE);
}

Arg get_mr(word w)
{
    Arg res;
    res.reg_space = MEMSPACE;
   
        
    unsigned int r = w & 7;          // номер регистра
    unsigned int m = (w >> 3) & 7;   // номер моды

 
    switch (m) {
    // мода 0, R1
    case 0:
        res.adr = r;        // адрес - номер регистра
        res.reg_space = REGSPACE;
        res.val = reg[r];   // значение - число в регистре
        trace(TRACE, "R%d ", r);
        break;


    // мода 1, (R1)
    case 1:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение
        trace(TRACE, "(R%d) ", r);
        break;


    // мода 2, (R1)+ или #3
    case 2:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение

        if (!BYTE_COMMAND || r >= 6)
        {
            reg[r] += 2;
        }
        else
        {
            reg[r] += 1;               
        }
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            trace(TRACE, "#%06o ", res.val);
        else
            trace(TRACE, "(R%d)+ ", r);
        break;

    // мода 3, @(Rn)+ или @#nn
    case 3:
        res.adr = w_read(reg[r], res.reg_space);           // в регистре адрес
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение
        reg[r] += 2;                
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            trace(TRACE, "@#%06o ", res.adr);
        else
            trace(TRACE, "@(R%d)+ ", r);
        break;

    // мода 4, -(Rn)
    case 4:
        if (!BYTE_COMMAND || r >= 6)
        {
            reg[r] -= 2;                // TODO: -1
        }
        else
        {
            reg[r] -= 1;
        }
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение    
        trace(TRACE, "-(R%d) ", r);
        break;
    // мода 5, @-(Rn) 
    case 5:
        reg[r] -= 2;          
        res.adr = w_read(reg[r], res.reg_space);           
        res.val = w_read(res.adr, res.reg_space);  
        trace(TRACE, "@-(R%d) ", r);
        break;
    // мода 8, X(Rn)
    case 6:
        word x = w_read(pc, MEMSPACE);
        pc += 2;

        res.adr = (reg[r] + x) & 0xFFFF;
        res.val = w_read(res.adr, res.reg_space);

        if (r == 7)
        {
            trace(TRACE, "%06o ", (int)res.adr);
        }
        else
            trace(TRACE, "%06o(R%d) ", (int)x, r);
        break;
    case 7:
        x = w_read(pc, MEMSPACE);
        pc += 2;

        res.adr = w_read((int)reg[r] + x, res.reg_space);
        res.val = w_read(res.adr, res.reg_space);

        if (r == 7)
            trace(TRACE, "@%06o ", (int)res.adr);
        else
            trace(TRACE, "@%06o(R%d) ", (int)x, r);
        break;
        
    // мы еще не дописали другие моды
    default:
        trace(ERROR, "Mode %d not implemented yet!\n", m);
        exit(1);
}

    return res;
}

Arg_R get_r(word w)
{
    Arg_R res;
    res.adr = w & 07;
    
    trace(TRACE, "R%d ", res.adr);

    return res;
}

Arg_NN get_nn(word w)
{
    Arg_NN res;
    res.val = w & 077;

    trace(TRACE, "%06o ", pc - 2 * res.val);

    return res;
}

Arg_XX get_xx(word w)
{
    Arg_XX res;
    res.val = w & 0xFF;

    trace(TRACE, "%06o ", pc + 2 * res.val);

    return res;
}

void do_halt(void)
{
    reg_dump();
    trace(TRACE, "THE END\n");
    exit(0);
}

void do_mov(void)
{
    // значение аргумента ss пишем по адресу аргумента dd
    w_write(DD_ARG.adr, SS_ARG.val, DD_ARG.reg_space);

    word result = w_read(DD_ARG.adr, DD_ARG.reg_space);
    set_Z(result);
    set_N(result);
    flags.V = 0;
}

void do_movb(void)
{
    // значение аргумента ss пишем по адресу аргумента dd
    b_write(DD_ARG.adr, SS_ARG.val, DD_ARG.reg_space);

    word result = w_read(DD_ARG.adr, DD_ARG.reg_space);
    set_Z(result);
    set_N(result);
    flags.V = 0;

    if (DD_ARG.adr == odata)
    {
        trace(INFO, "%c", b_read(odata, MEMSPACE));
    }
}

void do_add(void)
{
    // сумму значений аргументов ss и dd пишем по адресу аргумента dd
    w_write(DD_ARG.adr, SS_ARG.val + DD_ARG.val, DD_ARG.reg_space);

    word result = w_read(DD_ARG.adr, DD_ARG.reg_space);
    set_C(result);
    set_N(result);
    set_Z(result);
}

void do_sob(void)
{
    if (--reg[R_ARG.adr] > 0)
    {
        pc -= 2 * NN_ARG.val;
    }
}

// очистка регистра 
void do_clr(void)
{
    w_write(DD_ARG.adr, 0, DD_ARG.reg_space);
}

void do_br(void)
{
    pc = pc + 2 * XX_ARG.val;
}

void do_tst(void)
{
    word result = w_read(DD_ARG.adr, DD_ARG.reg_space);
    
    set_N(result);
    set_Z(result);
    flags.V = 0;
    flags.C = 0;
}

void do_tstb(void)
{
    byte result = b_read(DD_ARG.adr, DD_ARG.reg_space);
    
    set_N(result);
    set_Z(result);
    flags.V = 0;
    flags.C = 0;
}

void do_cmp(void)
{
    word result = w_read(SS_ARG.adr, SS_ARG.reg_space) - w_read(DD_ARG.adr, DD_ARG.reg_space);

    set_N(result);
    set_Z(result);
    set_C(result);
}

void do_cmpb(void)
{
    byte result = b_read(SS_ARG.adr, SS_ARG.reg_space) - b_read(DD_ARG.adr, DD_ARG.reg_space);

    set_N(result);
    set_Z(result);
    set_C(result);
}

void do_jsr(void)
{
    stack_push(reg[R_ARG.adr]);
    reg[R_ARG.adr] = pc;
    pc = DD_ARG.adr;
}

void do_rts(void)
{
    pc = reg[R_ARG.adr];
    reg[R_ARG.adr] = stack_pop();
}

// команды выставления флагов

void do_ccc() 
{
    flags.N = 0;
    flags.Z = 0;
    flags.V = 0;
    flags.C = 0;
}

void do_clc() 
{
    flags.C = 0;
}

void do_cln() 
{
    flags.N = 0;
}

void do_clv() 
{
    flags.V = 0;
}

void do_clz() 
{
    flags.Z = 0;
}

void do_nop() 
{
    // Ничего не делаем
}

void do_scc() 
{
    flags.N = 1;
    flags.Z = 1;
    flags.V = 1;
    flags.C = 1;
}

void do_sec() 
{
    flags.C = 1;
}

void do_sen() 
{
    flags.N = 1;
}

void do_sev() 
{
    flags.V = 1;
}

void do_sez() 
{
    flags.Z = 1;
}

// Условные ветвления

void do_bcc() {
    if (flags.C == 0)
        do_br();
}

void do_bcs() {
    if (flags.C == 1)
        do_br();
}

void do_beq() {
    if (flags.Z == 1)
        do_br();
}

void do_bge() {
    if ((flags.N ^ flags.V) == 0)
        do_br();
}

void do_bgt() {
    if ((flags.Z | (flags.N ^ flags.V)) == 0)
        do_br();
}

void do_bhi() {
    if ((flags.C | flags.Z) == 0)
        do_br();
}

void do_bhis() {
    if (flags.C == 0)
        do_br();
}

void do_ble() {
    if ((flags.Z | (flags.N ^ flags.V)) == 1)
        do_br();
}

void do_blt() {
    if ((flags.N ^ flags.V) == 1)
        do_br();
}

void do_blo() {
    if (flags.C == 1)
        do_br();
}

void do_blos() {
    if ((flags.C | flags.Z) == 1)
        do_br();
}

void do_bmi() {
    if (flags.N == 1)
        do_br();
}

void do_bne() {
    if (flags.Z == 0)
        do_br();
}

void do_bpl() {
    if (flags.N == 0)
        do_br();
}

void do_bvc() {
    if (flags.V == 0)
        do_br();
}

void do_bvs() {
    if (flags.V == 1)
        do_br();
}



void do_nothing(void) 
{
    
}