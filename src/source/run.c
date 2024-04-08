#include <string.h>

#include "../headers/run.h"
#include "../headers/log.h"
#include "../headers/command.h"
#include "../headers/mem.h"

#define prn fprintf(stderr, "\n%d: func: %s\n", __LINE__, __FUNCTION__)

word read_cmd(void)
{
    word w = w_read(pc, MEMSPACE);

    trace(TRACE, "%06o %06o : ", pc, w);

    pc += 2;

    return w;
}

Command parse_cmd(word w)
{
    Command cmd;
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
            cmd = command[i];
            break;
        }
    }

    return cmd;
}

void run()
{
    pc = 01000; 

    Command cmd;   
    while(1) 
    {
        cmd = parse_cmd(read_cmd());
        cmd.do_command();
    }
}