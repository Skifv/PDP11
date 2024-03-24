#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

enum LEVELS {
    DEBUG,
    TRACE,
    INFO,
    ERROR
};

static int CURRENT_LEVEL = DEBUG;

int log(int level, const char * format, ...);
int set_log_level(int level);

#include "../reals/log.cpp"