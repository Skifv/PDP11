#include <stdlib.h>

#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"

Arg SS_ARG;
Arg DD_ARG;
Arg_R R_ARG;
Arg_NN NN_ARG;

char BYTE_COMMAND = 0;

/* Все новые команды помещать до unknown */

Command command[] = 
{
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
    {0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
    {0170000, 0110000, "movb", do_movb, HAS_DD | HAS_SS},
    {0177000, 0077000, "sob", do_sob, HAS_R | HAS_NN},
    {0000000, 0000000, "unknown", do_nothing, NO_PARAMS}
};

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
            trace(TRACE, "#%o ", res.val);
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
            trace(TRACE, "@#%o ", res.adr);
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

void do_halt(void)
{
    reg_dump();
    trace(INFO, "THE END\n");
    exit(0);
}

void do_mov(void)
{
    // значение аргумента ss пишем по адресу аргумента dd
    w_write(DD_ARG.adr, SS_ARG.val, DD_ARG.reg_space);
}

void do_movb(void)
{
    // значение аргумента ss пишем по адресу аргумента dd
    b_write(DD_ARG.adr, SS_ARG.val, DD_ARG.reg_space);
}

void do_add(void)
{
    // сумму значений аргументов ss и dd пишем по адресу аргумента dd
    w_write(DD_ARG.adr, SS_ARG.val + DD_ARG.val, DD_ARG.reg_space);
}

void do_sob(void)
{
    if (--reg[R_ARG.adr] > 0)
    {
        pc -= 2 * NN_ARG.val;
    }
}


void do_nothing(void) 
{
    
}