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
    test_mov,
    test_mode1_toreg,
    test_mode1_mem_to_mem,
    test_mode1_fromreg_to_mem,
    test_mov_mode2_mem_to_mem_autoincrement
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

    trace(TRACE, "\033[1;35mAll tests have been completed successfully\033[0m\n");
}

// ����, ��� �� ��������� ���������� ������� mov, add, halt
void test_parse_mov(void)
{
    save_original_values();

    trace(TRACE, "%s ", __FUNCTION__);
    Command cmd = parse_cmd(001164);
    assert(strcmp(cmd.name, "mov"));
    
    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}
// ����, ��� �� ��������� ��������� ��������� ss � dd � mov R5, R3
void test_mode0(void)
{
    save_original_values();

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
    
    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}
// ����, ��� mov � ���� 0 �������� ����� � mov R5, R3
void test_mov(void)
{
    save_original_values();
    
    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // dd   
    reg[5] = 34;    // ss
    Command cmd = parse_cmd(0010503);
    cmd.do_command();
    assert(reg[3] = 34);
    assert(reg[5] = 34);
    
    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}

// ����, ��� �� ��������� ��������� ��������� ss � dd � mov (R5), R3
void test_mode1_toreg(void)
{
    save_original_values();
    
    trace(TRACE, __FUNCTION__);


    // setup
    reg[3] = 12;    // dd
    reg[5] = 0200;  // ss
    w_write(0200, 34, MEMSPACE);


    Command cmd = parse_cmd(0011503);


    assert(ss.val == 34);
    assert(ss.adr == 0200);
    assert(dd.val == 12);
    assert(dd.adr == 3);


    cmd.do_command();


    assert(reg[3] = 34);
    // ���������, ��� �������� �������� �� ����������
    assert(reg[5] = 0200);


    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}

void test_mode1_fromreg_to_mem(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();
    
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 12;    // dd
    reg[5] = 0200;  // ss

    Command cmd = parse_cmd(0010315); // mov R3, (R5)

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� ���� �������� � ������
    assert(w_read(0200, MEMSPACE) == 12);

    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}

void test_mode1_mem_to_mem(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 0100;    // dd
    reg[5] = 0200;  // ss
    w_write(0200, 34, MEMSPACE); // ���������� �������� � ������
    w_write(0100, 23, MEMSPACE); // ���������� �������� � ������

    assert(w_read(0200, MEMSPACE) == 34);
    assert(w_read(0100, MEMSPACE) == 23);
    

    Command cmd = parse_cmd(0011513); // mov (R5), (R3)

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� �������� � ������ ����� � ������
    assert(w_read(0100, MEMSPACE) == 34);
    assert(w_read(0200, MEMSPACE) == 34);

    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov (R5)+, (R3)+,
void test_mov_mode2_mem_to_mem_autoincrement(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 01000;    // ����� � �������� R3
    reg[5] = 02000;    // ����� � �������� R5
    w_write(02000, 10, MEMSPACE); // ���������� ����� 10 �� ������ 2000
    w_write(01000, 6, MEMSPACE); // ���������� ����� 10 �� ������ 2000

    assert(w_read(02000, MEMSPACE) == 10);
    assert(w_read(01000, MEMSPACE) == 6);

    Command cmd = parse_cmd(0012523); // mov (R5)+, (R3)+

    assert(w_read(02000, MEMSPACE) == 10);
    assert(w_read(01000, MEMSPACE) == 6);

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� ����������
    assert(w_read(01000, MEMSPACE) == 10);

    // ���������, ��� �������� �������� R3 ��������� �� 2
    assert(reg[3] == 01002);

    // ���������, ��� �������� �������� R5 ��������� �� 2
    assert(reg[5] == 02002);

    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(__FUNCTION__)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

    // ������� ����� ���������� �����
    cleanup();
}