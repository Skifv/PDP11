#include <stdlib.h>

#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"

Command command[] = 
{
    {0177777, 0000000, "halt", do_halt},
    {0170000, 0060000, "add", do_add},
    {0170000, 0010000, "mov", do_mov},
    {0000000, 0000000, "unknown", do_nothing}
};

void do_halt(void)
{
    trace(INFO, "THE END\n");
    exit(0);
}

void do_add(void)
{

}

void do_mov(void)
{

}

void do_nothing(void) 
{
    
}