
#pragma once

// Определение типа функции (в данном случае, функции, не принимающей аргументов и не возвращающей значения)
typedef void (*test_ptr)();

#define NTESTS 24

void run_all_tests(void);
void run_test(int i);

extern test_ptr tests[NTESTS];

void test_parse_mov(void);
void test_mov(void);
void test_mode0(void);
void test_mode1_mem_to_mem(void);
void test_mode1_fromreg_to_mem(void);
void test_mode1_toreg(void);
void test_mode2_mov_mem_to_mem_autoincrement(void);
void test_mode3_mov_indirect_autoincrement(void);
void test_mode4_mov_auto_decrement(void);
void test_mode5_mov_indirect_auto_decrement(void);
void test_clr_command(void);
void test_sob_command(void);
void test_ccc_command(void);
void test_clc_command(void);
void test_cln_command(void);
void test_clv_command(void);
void test_clz_command(void);
void test_nop_command(void);
void test_scc_command(void);
void test_sec_command(void);
void test_sen_command(void);
void test_sev_command(void);
void test_sez_command(void);
void test_br_command(void);