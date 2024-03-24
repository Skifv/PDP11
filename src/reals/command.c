#pragma once

#include "../headers/command.h"

typedef struct {
    word mask;
    word opcode;
    const char * name;
//  (void *) do_command(void);
} Command;

Command command[] = 
{
    {0177777, 0000000, "halt"},
    {0170000, 0060000, "add"},
    {0170000, 0010000, "mov"},
};

void do_halt(void)
{
    log(INFO, "THE END\n");
    exit(0);
}

void do_add(void)
{

}

void do_mov(void)
{

}

void do_nothing() 
{

}