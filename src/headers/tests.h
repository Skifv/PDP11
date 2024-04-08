#pragma once

// Определение типа функции (в данном случае, функции, не принимающей аргументов и не возвращающей значения)
typedef void (*test_ptr)();

#define NTESTS 3

extern test_ptr tests[NTESTS];

void test_parse_mov(void);
void test_mode0(void);
void test_mov(void);
void run_all_tests(void);
void run_test(int i);