#pragma once

enum LEVELS {
    DEBUG,
    FULLTRACE,
    TRACE,
    INFO,
    ERROR,
    NOTHING
};

extern int CURRENT_LEVEL;

int trace(int level, const char * format, ...);
int set_log_level(int level);

