#include <stdio.h>
#include "mem.cpp"
#include "log.cpp"
#include "load_dump.cpp"
#include <stdlib.h>

const char * filename;

int set_input(int argc, char * argv[]);

int main(int argc, char * argv[])
{
    set_input(argc, argv);

    load_file(filename);

    mem_dump(0x40, 20);
    putchar('\n');
    mem_dump(0x200, 0x26);

    return 0;
}

int set_input(int argc, char * argv[])
{
    if (argc == 1)
    {
        filename = "stdin";
    }
    else if (argc >= 2)
    {
        filename = argv[argc - 1];
    }
    
    if (filename == NULL)
    {
        perror(filename);
        exit(1);
    }

    return 0;
}
