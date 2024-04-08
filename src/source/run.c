#include <string.h>

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
        w = w_read(pc, MEMSPACE);

        trace(TRACE, "%06o %06o : ", pc, w);

        pc += 2;

        for (int i = 0; i < N_COMMANDS; i++)
        {
            if ((w & command[i].mask) == command[i].opcode)
            {
                trace(TRACE, "%s ", command[i].name);
                
                // Проверка на наличие аргументов SS и DD для их чтения
                // unknown - ничего не читаем
                if (0 != strcmp(command[i].name, "unknown"))
                {
                    if (((command[i].mask >> 6) & 077) == 00)
                    {
                        ss = get_mr((w >> 6) & 077);
                    }
                    if (((command[i].mask) & 077) == 00)
                    {
                        dd = get_mr(w & 077);
                    }
                }
                
                trace(TRACE, "\n");

                command[i].do_command(); 
                break;
                

                /*
                trace(TRACE, "%s ", command[i].name);
                command[i].do_command(); 
                break;
                */
            }
        }
    }
}