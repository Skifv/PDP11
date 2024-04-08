#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../headers/tests.h"
#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"
#include "../headers/run.h"

// ������ �� ������
test_ptr tests[NTESTS] =
{
    test_parse_mov,
    test_mode0,
    test_mov
};

void run_test(int i)
{
    if ((i >= NTESTS) || (i < 0))
    {
        trace(ERROR, "������! ������ ����� �� ����������.\n");
        return;
    }
    else
    {
        tests[i]();
    }
}

void run_all_tests(void)
{
    for (int i = 0; i < NTESTS; i++)
    {
        run_test(i);
    }
}

// ����, ��� �� ��������� ���������� ������� mov, add, halt
void test_parse_mov()
{
    trace(TRACE, "%s ", __FUNCTION__);
    Command cmd = parse_cmd(001164);
    assert(strcmp(cmd.name, "mov"));
    trace(TRACE, "\033[1;35mOK\033[0m\n");
}
// ����, ��� �� ��������� ��������� ��������� ss � dd � mov R5, R3
void test_mode0()
{
    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // dd
    reg[5] = 34;    // ss
    Command cmd = parse_cmd(0010503);

    // ��� ���� ����� ����� ���� �� ����������
    int x = 1;
    x += cmd.opcode;

    assert(ss.val == 34);
    assert(ss.adr == 5);
    assert(dd.val == 12);
    assert(dd.adr == 3);
    trace(TRACE, "\033[1;35mOK\033[0m\n");
}
// ����, ��� mov � ���� 0 �������� ����� � mov R5, R3
void test_mov()
{
    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // dd   
    reg[5] = 34;    // ss
    Command cmd = parse_cmd(0010503);
    cmd.do_command();
    assert(reg[3] = 34);
    assert(reg[5] = 34);
    trace(TRACE, "\033[1;35mOK\033[0m\n");
}