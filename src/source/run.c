#include <string.h>

#include "../headers/run.h"
#include "../headers/log.h"
#include "../headers/command.h"
#include "../headers/mem.h"

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
        
            // у команды есть SS?
            if (command[i].params & HAS_SS)
            {
                SS_ARG = get_mr((w >> 6));
            }
            // у команды есть DD?
            if (command[i].params & HAS_DD)
            {
                DD_ARG = get_mr(w);
            }
            // у команды есть R?
            if(command[i].params & HAS_R)
            {
                R_ARG = get_r(w >> 6);
            }
            // у команды есть NN?
            if (command[i].params & HAS_NN)
            {
                NN_ARG = get_nn(w);
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