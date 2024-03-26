#include <stdio.h>
#include "./headers/mem.h"
#include "./headers/log.h"
#include "./headers/load_dump.h"
#include "./headers/run.h"
#include <stdlib.h>
#include <string.h>

int CURRENT_LEVEL = INFO;

const char * parse_args(int argc, char * argv[]);

int main(int argc, char * argv[])
{  
    
    const char * filename = parse_args(argc, argv);
    load_file(filename);

    run();

    return 0;
}
void usage(char * argv[]) 
{
    fprintf(stdout, "Usage: %s [-t|-T] --|<filename>\n"
    "-t - trace on\n"
    "-T - FULL trace on\n"
    "-- - read from stdin\n", argv[0]);
}

const char * parse_args(int argc, char * argv[])
{
    const char * filename = NULL;
    if (argc == 1)
    {
        usage(argv);
        exit(1);
    }
    
    for (int argi = 1; argi < argc - 1; argi++)
    {
        if (0 == strcmp(argv[argi], "-t"))
        {
            set_log_level(TRACE);
        }
        else if (0 == strcmp(argv[argi], "-T"))
        {
            set_log_level(FULLTRACE);
        }
    }

    filename = argv[argc - 1];
    if (0 == strcmp(filename, "--"))
        filename = NULL;

    return filename;
}
