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
    test_mode5_mov_indirect_auto_decrement,
    test_clr_command,
    test_sob_command,
    test_ccc_command,
    test_clc_command,
    test_cln_command,
    test_clv_command,
    test_clz_command,
    test_nop_command,
    test_scc_command,
    test_sec_command,
    test_sen_command,
    test_sev_command,
    test_sez_command,
    test_br_command
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

// ���� ��� ������� clr (������� ��������)
void test_clr_command(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[1] = 12345; // ������������ �������� � �������� R1

    // ��������� ��������� ��������� ��������
    assert(reg[1] == 12345);

    // ��������� ������� clr ��� �������� R1
    Command cmd = parse_cmd(0005001); // clr R1 (������� �������� � ��������� ��������)

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� � �������� R1 ����� ������ ����
    assert(reg[1] == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ��������� � ������
    cleanup();
}

// ���� ��� ������� sob (���������� �������� � �������� �������)
void test_sob_command(void)
{
    // ��������� �������� �������� ��������� � ������
    save_original_values();

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ������������� ��������� ��������
    reg[2] = 5; // ������������ ��������� �������� � �������� R2
    pc = 100; // ������������ ��������� �������� �������� ������

    // ��������� ��������� ��������� �������� � �������� ������
    assert(reg[2] == 5);
    assert(pc == 100);

    // ��������� ������� sob � ������������ ���������� NN
    Command cmd = parse_cmd(0077202); // sob R2, 2

    // ��������� �������
    cmd.do_command();

    // ���������, ��� �������� � �������� R2 ����������� �� 1
    assert(reg[2] == 4);

    // ���������, ��� pc ����������� �� 2*NN_ARG.val, ��� ��� �������� � �������� R2 ����� ���������� �� ����� 0
    assert(pc == 100 - 2 * 2);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� �������� � �������� ������
    cleanup();
}

// ���� ��� ������� ccc (������� ������)
void test_ccc_command(void)
{
    // ��������� ������� �������� ������
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� ccc
    Command cmd = parse_cmd(000257); // ccc
    cmd.do_command();

    // ���������, ��� ��� ����� ���� ������� �������
    assert(flags.N == 0);
    assert(flags.Z == 0);
    assert(flags.V == 0);
    assert(flags.C == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ������
    flags.N = original_N;
    flags.Z = original_Z;
    flags.V = original_V;
    flags.C = original_C;
}

// ���� ��� ������� clc (������� ����� C)
void test_clc_command(void)
{
    // ��������� ������� �������� ����� C
    unsigned char original_C = flags.C;

    // ������������� ����� ��������� �������� ��� �����
    flags.C = 1;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� clc
    Command cmd = parse_cmd(000241); // clc
    cmd.do_command();

    // ���������, ��� ���� C ��� ������� ������
    assert(flags.C == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� C
    flags.C = original_C;
}

// ���� ��� ������� cln (������� ����� N)
void test_cln_command(void)
{
    // ��������� ������� �������� ����� N
    unsigned char original_N = flags.N;

    // ������������� ����� ��������� �������� ��� �����
    flags.N = 1;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� cln
    Command cmd = parse_cmd(000250); // cln
    cmd.do_command();

    // ���������, ��� ���� N ��� ������� ������
    assert(flags.N == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� N
    flags.N = original_N;
}

// ���� ��� ������� clv (������� ����� V)
void test_clv_command(void)
{
    // ��������� ������� �������� ����� V
    unsigned char original_V = flags.V;

    // ������������� ����� ��������� �������� ��� �����
    flags.V = 1;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� clv
    Command cmd = parse_cmd(000242); // clv
    cmd.do_command();

    // ���������, ��� ���� V ��� ������� ������
    assert(flags.V == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� V
    flags.V = original_V;
}

// ���� ��� ������� clz (������� ����� Z)
void test_clz_command(void)
{
    // ��������� ������� �������� ����� Z
    unsigned char original_Z = flags.Z;

    // ������������� ����� ��������� �������� ��� �����
    flags.Z = 1;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� clz
    Command cmd = parse_cmd(000244); // clz
    cmd.do_command();

    // ���������, ��� ���� Z ��� ������� ������
    assert(flags.Z == 0);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� Z
    flags.Z = original_Z;
}

// ���� ��� ������� nop (������ �� ������)
void test_nop_command(void)
{
    // ��������� ������� �������� ������
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� nop
    Command cmd = parse_cmd(000240); // nop
    cmd.do_command();

    // ���������, ��� ����� �������� �����������
    assert(flags.N == original_N);
    assert(flags.Z == original_Z);
    assert(flags.V == original_V);
    assert(flags.C == original_C);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);
}

// ���� ��� ������� scc (��������� ���� ������)
void test_scc_command(void)
{
    // ��������� ������� �������� ������
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� scc
    Command cmd = parse_cmd(000277); // scc
    cmd.do_command();

    // ���������, ��� ��� ����� ���� ������� �����������
    assert(flags.N == 1);
    assert(flags.Z == 1);
    assert(flags.V == 1);
    assert(flags.C == 1);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ������
    flags.N = original_N;
    flags.Z = original_Z;
    flags.V = original_V;
    flags.C = original_C;
}

// ���� ��� ������� sec (��������� ����� C)
void test_sec_command(void)
{
    // ��������� ������� �������� ����� C
    unsigned char original_C = flags.C;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� sec
    Command cmd = parse_cmd(000261); // sec
    cmd.do_command();

    // ���������, ��� ���� C ��� ������� ����������
    assert(flags.C == 1);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� C
    flags.C = original_C;
}

// ���� ��� ������� sen (��������� ����� N)
void test_sen_command(void)
{
    // ��������� ������� �������� ����� N
    unsigned char original_N = flags.N;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� sen
    Command cmd = parse_cmd(000270); // sen
    cmd.do_command();

    // ���������, ��� ���� N ��� ������� ����������
    assert(flags.N == 1);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� N
    flags.N = original_N;
}

// ���� ��� ������� sev (��������� ����� V)
void test_sev_command(void)
{
    // ��������� ������� �������� ����� V
    unsigned char original_V = flags.V;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� sev
    Command cmd = parse_cmd(000262); // sev
    cmd.do_command();

    // ���������, ��� ���� V ��� ������� ����������
    assert(flags.V == 1);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� V
    flags.V = original_V;
}

// ���� ��� ������� sez (��������� ����� Z)
void test_sez_command(void)
{
    // ��������� ������� �������� ����� Z
    unsigned char original_Z = flags.Z;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    // ��������� ������� sez
    Command cmd = parse_cmd(000264); // sez
    cmd.do_command();

    // ���������, ��� ���� Z ��� ������� ����������
    assert(flags.Z == 1);

    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� ����� Z
    flags.Z = original_Z;
}

// ���� ��� ������� br (����������� �������)
void test_br_command(void)
{
    // ��������� ������� �������� PC
    word original_PC = pc;

    pc = 0100;

    // ������� ���������� � �����
    trace(TRACE, __FUNCTION__);

    trace(TRACE, "\n");
    // ��������� ������� br
    Command cmd = parse_cmd(000402); // br
    cmd.do_command();

    // ���������, ��� PC ��������� �� 2 ����� ������
    assert(pc == 0104);

    pc = 01004;

    // ��������� ������� br
    cmd = parse_cmd(000775); // br
    cmd.do_command();

    // ���������, ��� PC ��������� �� 3 ����� �����
    assert(pc == 01000);


trace(TRACE, "\n");
    // ������� ��������� �����
    print_all_OK(__FUNCTION__);

    // ��������������� �������� �������� PC
    pc = original_PC;
}
