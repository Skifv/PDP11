#include <string.h>

#include "../headers/mem.h"
#include "../headers/log.h"
#include "../headers/command.h"

word reg[REGSIZE];
static byte mem[PDP11_MEMSIZE];

PWD flags = {0, 0, 0, 0};

void set_Z (int result)
{
    flags.Z = (result == 0) ? 1 : 0;
}

void set_N (int result)
{
    if (BYTE_COMMAND)
    {
        flags.N = ((result >> 7) & 1);
        return;
    }

    flags.N = ((result >> 15) & 1);
    return;
}

void set_C (int result)
{
    if (BYTE_COMMAND)
    {
        flags.N = ((result >> 8) & 1);
        return;
    }
    
    flags.C = ((result >> 16) & 1);
    return;
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

void flags_dump(void)
{
    trace(TRACE, "N Z V C\n");
    trace(TRACE, "%d %d %d %d\n", flags.N, flags.Z, flags.V, flags.C);

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
    // ��������� �������� �������� ���������
    memcpy(original_reg_values, reg, REGSIZE * sizeof(word));
    // ��������� �������� ��������� ������
    memcpy(original_mem_values, mem, PDP11_MEMSIZE * sizeof(byte));
}

void restore_original_values() 
{
    // ��������������� �������� �������� ���������
    memcpy(reg, original_reg_values, REGSIZE * sizeof(word));
    // ��������������� �������� ��������� ������
    memcpy(mem, original_mem_values, PDP11_MEMSIZE * sizeof(byte));
}

void cleanup() 
{
    restore_original_values(); // ��������������� �������� �������� ��������� � ������
}