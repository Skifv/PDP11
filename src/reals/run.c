#pragma once

#include "../headers/run.h"

#define prn fprintf(stderr, "\n%d: func: %s\n", __LINE__, __FUNCTION__)


void run()
{
    pc = 01000;

    word w;     

    while(1) 
    {
        w = w_read(pc);

        log(TRACE, "%06o %06o : ", pc, w);

        pc += 2;

        if (w == 0)
        {
            log(TRACE, "halt\n");
            do_halt();            
        }

        switch (w & 170000)
        {
        case 060000:
            log(TRACE, "add\n");
            do_add();
            break;
        case 010000:
            log(TRACE, "mov\n");
            do_mov();
            break;
        default:
            log(TRACE, "unknown\n");
            do_nothing();
            break;
        }


    }
}