#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

const char * filename;

enum LEVELS {
    DEBUG,
    TRACE,
    INFO,
    ERROR
};

int CURRENT_LEVEL = DEBUG;

int log(int level, const char * format, ...)
{
    int sum = 0;

    if (level >= CURRENT_LEVEL)
    {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
    
    return sum;
}

int set_log_level(int level)
{
    int prev_level = CURRENT_LEVEL;
    CURRENT_LEVEL = level;
    return prev_level;
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

