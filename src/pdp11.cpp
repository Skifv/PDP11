#include <stdio.h>
#include "./headers/mem.h"
#include "./headers/log.h"
#include "./headers/load_dump.h"
#include "./headers/run.h"
#include <stdlib.h>

const char * filename;

int set_input(int argc, char * argv[]);

int main(int argc, char * argv[])
{
    set_input(argc, argv);

    CURRENT_LEVEL = DEBUG;

    load_file(filename);

    run();

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
