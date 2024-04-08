#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../headers/tests.h"
#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"
#include "../headers/run.h"

// ћассив из тестов
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
        trace(ERROR, "ќшибка! “акого теста не существует.\n");
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

// тест, что мы правильно определ€ем команды mov, add, halt
void test_parse_mov()
{
    trace(TRACE, "%s ", __FUNCTION__);
    Command cmd = parse_cmd(001164);
    assert(strcmp(cmd.name, "mov"));
    trace(TRACE, "\033[1;35mOK\033[0m\n");
}
// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
void test_mode0()
{
    trace(TRACE, "%s ", __FUNCTION__);
    reg[3] = 12;    // dd
    reg[5] = 34;    // ss
    Command cmd = parse_cmd(0010503);

    // это надо чтобы флаги деда не жаловались
    int x = 1;
    x += cmd.opcode;

    assert(ss.val == 34);
    assert(ss.adr == 5);
    assert(dd.val == 12);
    assert(dd.adr == 3);
    trace(TRACE, "\033[1;35mOK\033[0m\n");
}
// тест, что mov и мода 0 работают верно в mov R5, R3
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