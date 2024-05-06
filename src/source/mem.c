#include <string.h>

#include "../headers/mem.h"
#include "../headers/log.h"

word reg[REGSIZE];
static byte mem[PDP11_MEMSIZE];

PWD flags = {0, 0, 0, 0};

void set_Z (int result)
{
    flags.Z = (result == 0) ? 1 : 0;
}

void set_N (int result)
{
    flags.N = (result < 0) ? 1 : 0;
}

void set_C (int result)
{
    flags.C = (result & 0x010000) ? 1 : 0;
}

void reg_dump(void)
{
    for (int i = 0; i < 8; i++)
    {
        trace(TRACE, "r%d:%o ", i, reg[i]);
    }
    trace(TRACE, "\n");

    return;
}

byte b_read(address adr, int reg_space)
{
    if (reg_space)
    {
        return reg[adr] & 0xFF;
    }

    return mem[adr] & 0xFF;
}

void b_write(address adr, byte value, int reg_space)
{
    value = value & 0xFF;
    if (reg_space)
    {
        int b = (value >> 7) & 1; 
        if (b == 0)
            reg[adr] = value & 0x00FF;
        else
            reg[adr] = value | 0xFF00;
        return;
    }

    mem[adr] = value;
}

word w_read(address adr, int reg_space)
{
    if (reg_space)
    {
        return reg[adr] & 0xFFFF;
    }

    byte b0 = mem[adr];
    byte b1 = mem[adr + 1];

    word res = 0;

    res = b1;
    res = res << 8;
    res = res | b0;

    return res & 0xFFFF;
}

void w_write (address adr, word val, int reg_space)
{
    val = val & 0xFFFF;
    
    if (reg_space)
    {
        reg[adr] = val;
        return;
    }

    mem[adr + 1] = (val & 0xFF00) >> 8;
    mem[adr] = val & 0x00FF;
}

word original_reg_values[REGSIZE];
byte original_mem_values[PDP11_MEMSIZE];

void save_original_values() 
{
    // —охран€ем исходные значени€ регистров
    memcpy(original_reg_values, reg, REGSIZE * sizeof(word));
    // —охран€ем исходное состо€ние пам€ти
    memcpy(original_mem_values, mem, PDP11_MEMSIZE * sizeof(byte));
}

void restore_original_values() 
{
    // ¬осстанавливаем исходные значени€ регистров
    memcpy(reg, original_reg_values, REGSIZE * sizeof(word));
    // ¬осстанавливаем исходное состо€ние пам€ти
    memcpy(mem, original_mem_values, PDP11_MEMSIZE * sizeof(byte));
}

void cleanup() 
{
    restore_original_values(); // ¬осстанавливаем исходные значени€ регистров и пам€ть
}