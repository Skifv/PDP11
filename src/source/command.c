#include <stdlib.h>

#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"

Arg ss;
Arg dd;

/* ��� ����� ������� �������� �� unknown */

Command command[] = 
{
    {0177777, 0000000, "halt", do_halt},
    {0170000, 0060000, "add", do_add},
    {0170000, 0010000, "mov", do_mov},
    {0000000, 0000000, "unknown", do_nothing}
};

Arg get_mr(word w)
{
    Arg res;
    
    unsigned int r = w & 7;          // ����� ��������
    unsigned int m = (w >> 3) & 7;   // ����� ����

    switch (m) {
    // ���� 0, R1
    case 0:
        res.adr = r;        // ����� - ����� ��������
        res.reg_space = REGSPACE;
        res.val = reg[r];   // �������� - ����� � ��������
        trace(TRACE, "R%d ", r);
        break;


    // ���� 1, (R1)
    case 1:
        res.adr = reg[r];           // � �������� �����
        res.reg_space = MEMSPACE;
        res.val = w_read(res.adr, res.reg_space);  // �� ������ - ��������
        trace(TRACE, "(R%d) ", r);
        break;


    // ���� 2, (R1)+ ��� #3
    case 2:
        res.adr = reg[r];           // � �������� �����
        res.reg_space = MEMSPACE;
        res.val = w_read(res.adr, res.reg_space);  // �� ������ - ��������
        reg[r] += 2;                // TODO: +1
        // ������ ������ ��������� ��� PC � ������ ���������
        if (r == 7)
            trace(TRACE, "#%o ", res.val);
        else
            trace(TRACE, "(R%d)+ ", r);
        break;


    // �� ��� �� �������� ������ ����
    default:
        trace(ERROR, "Mode %d not implemented yet!\n", m);
        exit(1);
}

    return res;
}

void do_halt(void)
{
    reg_dump();
    trace(INFO, "THE END\n");
    exit(0);
}

void do_mov()
{
    // �������� ��������� ss ����� �� ������ ��������� dd
    w_write(dd.adr, ss.val, dd.reg_space);
}

void do_add()
{
    // ����� �������� ���������� ss � dd ����� �� ������ ��������� dd
    w_write(dd.adr, ss.val + dd.val, dd.reg_space);
}

void do_nothing(void) 
{
    
}