#include "../headers/mem.h"
#include "../headers/log.h"

#define PDP11_MEMSIZE (64 * 1024)

word reg[REGSIZE];
static byte mem[PDP11_MEMSIZE];

void reg_dump(void)
{
    for (int i = 0; i < 8; i++)
    {
        trace(TRACE, "r%d:%o ", i, reg[i]);
    }
    trace(TRACE, "\n");

    return;
}

byte b_read(address adr)
{
    return mem[adr];
}

void b_write(address adr, byte value)
{
    mem[adr] = value;
}

word w_read(address adr, int reg_space)
{
    if (reg_space)
    {
        return reg[adr];
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
    if (reg_space)
    {
        reg[adr] = val;
        return;
    }

    mem[adr + 1] = (val & 0xFF00) >> 8;
    mem[adr] = val & 0x00FF;
}