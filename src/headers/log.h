#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

enum LEVELS {
    DEBUG,
    FULLTRACE,
    TRACE,
    INFO,
    ERROR
};

extern int CURRENT_LEVEL;

int log(int level, const char * format, ...);
int set_log_level(int level);

#include "../reals/log.c"