#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../headers/log.h"

int CURRENT_LEVEL = NOTHING;

int trace(int level, const char * format, ...)
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

