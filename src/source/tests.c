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
    test_mov,
    test_mode0,
    test_mode1_toreg,
    test_mode1_mem_to_mem,
    test_mode1_fromreg_to_mem,
    test_mode2_mov_mem_to_mem_autoincrement,
    test_mode3_mov_indirect_autoincrement,
    test_mode4_mov_auto_decrement,
    test_mode5_mov_indirect_auto_decrement
};

void print_all_OK (const char * funcname)
{
    trace(TRACE, "\r"); // ������� ������� � ������ ������
    trace(TRACE, "\x1b[1A"); // ���������� ������ �� ���� ������ �����
    trace(TRACE, "\x1b[%dC", (int)strlen(funcname)); // ���������� ������� � ������� ������ ���������
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

}

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
    
    print_all_OK(__FUNCTION__);

    cleanup();
}
// ����, ��� �� ��������� ��������� ��������� ss � dd � mov R5, R3
void test_mode0(void)
{
    save_original_values();

    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // dd
    reg[5] = 34;    // ss
    
    parse_cmd(0010503);

    assert(SS_ARG.val == 34);
    assert(SS_ARG.adr == 5);
    assert(DD_ARG.val == 12);
    assert(DD_ARG.adr == 3);
    
    print_all_OK(__FUNCTION__);

    cleanup();
}
// ����, ��� mov � ���� 0 �������� ����� � mov R5, R3
void test_mov(void)
{
    save_original_values();
    
    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // DD   
    reg[5] = 34;    // ss
    Command cmd = parse_cmd(0010503);
    cmd.do_command();
    assert(reg[3] == 34);
    assert(reg[5] == 34);
    
    print_all_OK(__FUNCTION__);

    cleanup();
}

// ����, ��� �� ��������� ��������� ��������� ss � DD � mov (R5), R3
void test_mode1_toreg(void)
{
    save_original_values();
    
    trace(TRACE, __FUNCTION__);


    // setup
    reg[3] = 12;    // DD
    reg[5] = 0200;  // ss
    w_write(0200, 34, MEMSPACE);


    Command cmd = parse_cmd(0011503);


    assert(SS_ARG.val == 34);
    assert(SS_ARG.adr == 0200);
    assert(DD_ARG.val == 12);
    assert(DD_ARG.adr == 3);


    cmd.do_command();


    assert(reg[3] == 34);
    // ���������, ��� �������� �������� �� ����������
    assert(reg[5] == 0200);


    print_all_OK(__FUNCTION__);

    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov R3, (R5)
void test_mode1_fromreg_to_mem(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();
    
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 12;    // DD
    reg[5] = 0200;  // ss

    Command cmd = parse_cmd(0010315); // mov R3, (R5)

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� ���� �������� � ������
    assert(w_read(0200, MEMSPACE) == 12);

    print_all_OK(__FUNCTION__);

    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov (R5), (R3)
void test_mode1_mem_to_mem(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 0100;    // DD
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

    print_all_OK(__FUNCTION__);

    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov (R5)+, (R3)+,
void test_mode2_mov_mem_to_mem_autoincrement(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 01000;    // ����� � �������� R3
    reg[5] = 02000;    // ����� � �������� R5
    w_write(02000, 010, MEMSPACE); // ���������� ����� 10 �� ������ 2000
    w_write(01000, 06, MEMSPACE); // ���������� ����� 6 �� ������ 1000

    assert(w_read(02000, MEMSPACE) == 010);
    assert(w_read(01000, MEMSPACE) == 06);

    Command cmd = parse_cmd(0012523); // mov (R5)+, (R3)+

    assert(w_read(02000, MEMSPACE) == 010);
    assert(w_read(01000, MEMSPACE) == 06);

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� ����������
    assert(w_read(01000, MEMSPACE) == 010);

    // ���������, ��� �������� �������� R3 ��������� �� 2
    assert(reg[3] == 01002);

    // ���������, ��� �������� �������� R5 ��������� �� 2
    assert(reg[5] == 02002);

    print_all_OK(__FUNCTION__);

    // ������� ����� ���������� �����
    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov @(R5)+, @(R3)+,
void test_mode3_mov_indirect_autoincrement(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 01000;    // ����� � �������� R3
    reg[5] = 02000;    // ����� � �������� R5
    w_write(02000, 0300, MEMSPACE); 
    w_write(0300, 010, MEMSPACE); // ���������� ����� 10 �� ������ 300

    w_write(01000, 0100, MEMSPACE);
    w_write(0100, 02, MEMSPACE); // ���������� ����� 2 �� ������ 100

    // ��������� ��������� ��������� ������
    assert(w_read(0300, MEMSPACE) == 010);
    assert(w_read(02000, MEMSPACE) == 0300);

    assert(w_read(01000, MEMSPACE) == 0100);
    assert(w_read(0100, MEMSPACE) == 02);

    // ��������� ������� mov @(R5)+, @(R3)+
    Command cmd = parse_cmd(0013533);

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� ����������
    assert(w_read(0100, MEMSPACE) == 010);

    // ���������, ��� �������� �������� R3 ��������� �� 2
    assert(reg[3] == 01002);

    // ���������, ��� �������� �������� R5 ��������� �� 2
    assert(reg[5] == 02002);

    // ���������, ��� �������� � ������ �� ������, ����������� � �������� R5, �� ����������
    assert(w_read(02000, MEMSPACE) == 0300);

    // ���������, ��� �������� � ������ �� ������ 300 �� ����������
    assert(w_read(0300, MEMSPACE) == 010);

    // ���������, ��� �������� ���� �������� � ������
    assert(w_read(0100, MEMSPACE) == 010);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ��������� � ������
    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov -(R3), R1
void test_mode4_mov_auto_decrement(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 01002;    // ����� � �������� R3
    w_write(01000, 05, MEMSPACE); // ���������� ����� 5 �� ������ 01000 � ������

    // ��������� ��������� ��������� ������ � ��������
    assert(w_read(01000, MEMSPACE) == 05);
    assert(reg[3] == 01002);

    // ��������� ������� mov -(R3), R1
    Command cmd = parse_cmd(0014301); // mov -(R3), R1

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� ���������� � ������� R1
    assert(reg[1] == 05);

    // ���������, ��� �������� �������� R3 ����������� �� 2
    assert(reg[3] == 01000);

    // ���������, ��� �������� � ������ �� ������ 01000 �� ����������
    assert(w_read(01000, MEMSPACE) == 05);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ��������� � ������
    cleanup();
}

// ���� ���� ��������� ������������ ���������� ������� mov @-(R3), R1
void test_mode5_mov_indirect_auto_decrement(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[3] = 01002;    // ����� � �������� R3
    w_write(01000, 0206, MEMSPACE); // ���������� ����� 0206 �� ������ 01000 � ������
    w_write(0206, 05, MEMSPACE); // ���������� ����� 5 �� ������ 0206 � ������

    // ��������� ��������� ��������� ������ � ��������
    assert(w_read(01000, MEMSPACE) == 0206);
    assert(w_read(0206, MEMSPACE) == 05);
    assert(reg[3] == 01002);

    // ��������� ������� mov @-(R3), R1
    Command cmd = parse_cmd(0015301); // mov @-(R3), R1

    assert(w_read(01000, MEMSPACE) == 0206);
    assert(w_read(0206, MEMSPACE) == 05);
    assert(reg[3] == 01000);

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� �� ������ ���� ���������� � ������� R1
    assert(reg[1] == 05);

    // ���������, ��� �������� �������� R3 ����������� �� 2
    assert(reg[3] == 01000);

    // ���������, ��� �������� � ������ �� ������ 0206 �� ����������
    assert(w_read(0206, MEMSPACE) == 05);

    // ���������, ��� �������� � ������ �� ������ 01000 �� ����������
    assert(w_read(01000, MEMSPACE) == 0206);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ��������� � ������
    cleanup();
}


