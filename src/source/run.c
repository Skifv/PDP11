#include "../headers/run.h"
#include "../headers/log.h"
#include "../headers/command.h"

#define prn fprintf(stderr, "\n%d: func: %s\n", __LINE__, __FUNCTION__)


void run()
{
    pc = 01000;

    word w;     

    while(1) 
    {
        w = w_read(pc);

        trace(TRACE, "%06o %06o : ", pc, w);

        pc += 2;

        for (int i = 0; i < N_COMMANDS; i++)
        {
            if ((w & command[i].mask) == command[i].opcode)
            {
                trace(TRACE, "%s\n", command[i].name);
                command[i].do_command();
                break;
            }
        }
    }
}