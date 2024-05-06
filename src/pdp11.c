#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/mem.h"
#include "./headers/log.h"
#include "./headers/load_dump.h"
#include "./headers/run.h"
#include "./headers/tests.h"

/* Чтобы поменять флаг при запуске, меняйте переменную FL в makefile */

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
    trace(INFO, "Usage: %s [-t|-T] --|<filename>\n"
    "-d - debug on\n"
    "-T - FULL trace on\n"
    "-t - trace on\n"
    "-i - info on\n"
    "-e - error on\n"
    "--testall - run all tests only\n"
    "--test one <number_of_test> - run tests[number_of_test]"
    "default - no messages\n"
    "-- - read from stdin\n", argv[0]);
}

const char * parse_args(int argc, char * argv[])
{
    // getopt
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
        else if (0 == strcmp(argv[argi], "-d"))
        {
            set_log_level(DEBUG);
        }
        else if (0 == strcmp(argv[argi], "-i"))
        {
            set_log_level(INFO);
        }

        if (0 == strcmp(argv[argi], "--testall"))
        {
            run_all_tests();
            exit(0);
        }
        else if (0 == strcmp(argv[argi], "--testone"))
        {
            int test_number = atoi(argv[argi + 1]);
            run_test(test_number);
            exit(0);
        }
    }

    filename = argv[argc - 1];
    if (0 == strcmp(filename, "--"))
        filename = NULL;

    return filename;
}
