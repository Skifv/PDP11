#include <stdlib.h>

#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"

Arg ss;
Arg dd;

/* Все новые команды помещать до unknown */

Command command[] = 
{
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
    {0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
    {0000000, 0000000, "unknown", do_nothing, NO_PARAMS}
};

Arg get_mr(word w)
{
    Arg res;
    
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
        res.reg_space = MEMSPACE;
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение
        trace(TRACE, "(R%d) ", r);
        break;


    // мода 2, (R1)+ или #3
    case 2:
        res.adr = reg[r];           // в регистре адрес
        res.reg_space = MEMSPACE;
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение
        reg[r] += 2;                // TODO: +1
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            trace(TRACE, "#%o ", res.val);
        else
            trace(TRACE, "(R%d)+ ", r);
        break;

    // мода 3, @(Rn)+ или @#nn
    case 3:
        res.reg_space = MEMSPACE;
        res.adr = w_read(reg[r], res.reg_space);           // в регистре адрес
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение
        reg[r] += 2;                
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            trace(TRACE, "@#%o ", res.val);
        else
            trace(TRACE, "@(R%d)+ ", r);
        break;

    // мода 4, -(Rn)
    case 4:
        reg[r] -= 2;                // TODO: -1
        res.adr = reg[r];           // в регистре адрес
        res.reg_space = MEMSPACE;
        res.val = w_read(res.adr, res.reg_space);  // по адресу - значение    
        trace(TRACE, "-(R%d) ", r);
        break;
    // мода 5, @-(Rn) 
    case 5:
        reg[r] -= 2;   
        res.reg_space = MEMSPACE;             
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

void do_halt(void)
{
    reg_dump();
    trace(INFO, "THE END\n");
    exit(0);
}

void do_mov()
{
    // значение аргумента ss пишем по адресу аргумента dd
    w_write(dd.adr, ss.val, dd.reg_space);
}

void do_add()
{
    // сумму значений аргументов ss и dd пишем по адресу аргумента dd
    w_write(dd.adr, (ss.val + dd.val) & 0177777, dd.reg_space);
}

void do_nothing(void) 
{
    
}