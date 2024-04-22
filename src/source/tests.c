#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../headers/tests.h"
#include "../headers/command.h"
#include "../headers/mem.h"
#include "../headers/log.h"
#include "../headers/run.h"

// Массив из тестов
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
    trace(TRACE, "\r"); // Возврат каретки в начало строки
    trace(TRACE, "\x1b[1A"); // Перемещаем курсор на одну строку вверх
    trace(TRACE, "\x1b[%dC", (int)strlen(funcname)); // Перемещаем каретку в крайнее правое положение
    trace(TRACE, "\033[1;32m OK                       \033[0m\n");

}

void run_test(int i)
{
    if ((i >= NTESTS) || (i < 0))
    {
        trace(ERROR, "Ошибка! Такого теста не существует.\n");
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

// тест, что мы правильно определяем команды mov, add, halt
void test_parse_mov(void)
{
    save_original_values();

    trace(TRACE, "%s ", __FUNCTION__);
    Command cmd = parse_cmd(001164);
    assert(strcmp(cmd.name, "mov"));
    
    print_all_OK(__FUNCTION__);

    cleanup();
}
// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
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
// тест, что mov и мода 0 работают верно в mov R5, R3
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

// тест, что мы разобрали правильно аргументы ss и DD в mov (R5), R3
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
    // проверяем, что значение регистра не изменилось
    assert(reg[5] == 0200);


    print_all_OK(__FUNCTION__);

    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov R3, (R5)
void test_mode1_fromreg_to_mem(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();
    
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 12;    // DD
    reg[5] = 0200;  // ss

    Command cmd = parse_cmd(0010315); // mov R3, (R5)

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение было записано в память
    assert(w_read(0200, MEMSPACE) == 12);

    print_all_OK(__FUNCTION__);

    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov (R5), (R3)
void test_mode1_mem_to_mem(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 0100;    // DD
    reg[5] = 0200;  // ss
    w_write(0200, 34, MEMSPACE); // Записываем значение в память
    w_write(0100, 23, MEMSPACE); // Записываем значение в память

    assert(w_read(0200, MEMSPACE) == 34);
    assert(w_read(0100, MEMSPACE) == 23);
    

    Command cmd = parse_cmd(0011513); // mov (R5), (R3)

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение из памяти было записано в другое место в памяти
    assert(w_read(0100, MEMSPACE) == 34);
    assert(w_read(0200, MEMSPACE) == 34);

    print_all_OK(__FUNCTION__);

    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov (R5)+, (R3)+,
void test_mode2_mov_mem_to_mem_autoincrement(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 01000;    // адрес в регистре R3
    reg[5] = 02000;    // адрес в регистре R5
    w_write(02000, 010, MEMSPACE); // записываем число 10 по адресу 2000
    w_write(01000, 06, MEMSPACE); // записываем число 6 по адресу 1000

    assert(w_read(02000, MEMSPACE) == 010);
    assert(w_read(01000, MEMSPACE) == 06);

    Command cmd = parse_cmd(0012523); // mov (R5)+, (R3)+

    assert(w_read(02000, MEMSPACE) == 010);
    assert(w_read(01000, MEMSPACE) == 06);

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение из памяти было перемещено
    assert(w_read(01000, MEMSPACE) == 010);

    // Проверяем, что значение регистра R3 увеличено на 2
    assert(reg[3] == 01002);

    // Проверяем, что значение регистра R5 увеличено на 2
    assert(reg[5] == 02002);

    print_all_OK(__FUNCTION__);

    // Очищаем после завершения теста
    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov @(R5)+, @(R3)+,
void test_mode3_mov_indirect_autoincrement(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 01000;    // адрес в регистре R3
    reg[5] = 02000;    // адрес в регистре R5
    w_write(02000, 0300, MEMSPACE); 
    w_write(0300, 010, MEMSPACE); // записываем число 10 по адресу 300

    w_write(01000, 0100, MEMSPACE);
    w_write(0100, 02, MEMSPACE); // записываем число 2 по адресу 100

    // Проверяем начальное состояние памяти
    assert(w_read(0300, MEMSPACE) == 010);
    assert(w_read(02000, MEMSPACE) == 0300);

    assert(w_read(01000, MEMSPACE) == 0100);
    assert(w_read(0100, MEMSPACE) == 02);

    // Выполняем команду mov @(R5)+, @(R3)+
    Command cmd = parse_cmd(0013533);

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение из памяти было перемещено
    assert(w_read(0100, MEMSPACE) == 010);

    // Проверяем, что значение регистра R3 увеличено на 2
    assert(reg[3] == 01002);

    // Проверяем, что значение регистра R5 увеличено на 2
    assert(reg[5] == 02002);

    // Проверяем, что значение в памяти по адресу, хранящемуся в регистре R5, не изменилось
    assert(w_read(02000, MEMSPACE) == 0300);

    // Проверяем, что значение в памяти по адресу 300 не изменилось
    assert(w_read(0300, MEMSPACE) == 010);

    // Проверяем, что значение было записано в память
    assert(w_read(0100, MEMSPACE) == 010);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения регистров и памяти
    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov -(R3), R1
void test_mode4_mov_auto_decrement(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 01002;    // адрес в регистре R3
    w_write(01000, 05, MEMSPACE); // записываем число 5 по адресу 01000 в память

    // Проверяем начальное состояние памяти и регистра
    assert(w_read(01000, MEMSPACE) == 05);
    assert(reg[3] == 01002);

    // Выполняем команду mov -(R3), R1
    Command cmd = parse_cmd(0014301); // mov -(R3), R1

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение из памяти было перемещено в регистр R1
    assert(reg[1] == 05);

    // Проверяем, что значение регистра R3 уменьшилось на 2
    assert(reg[3] == 01000);

    // Проверяем, что значение в памяти по адресу 01000 не изменилось
    assert(w_read(01000, MEMSPACE) == 05);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения регистров и памяти
    cleanup();
}

// Этот тест проверяет корректность выполнения команды mov @-(R3), R1
void test_mode5_mov_indirect_auto_decrement(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[3] = 01002;    // адрес в регистре R3
    w_write(01000, 0206, MEMSPACE); // записываем адрес 0206 по адресу 01000 в память
    w_write(0206, 05, MEMSPACE); // записываем число 5 по адресу 0206 в память

    // Проверяем начальное состояние памяти и регистра
    assert(w_read(01000, MEMSPACE) == 0206);
    assert(w_read(0206, MEMSPACE) == 05);
    assert(reg[3] == 01002);

    // Выполняем команду mov @-(R3), R1
    Command cmd = parse_cmd(0015301); // mov @-(R3), R1

    assert(w_read(01000, MEMSPACE) == 0206);
    assert(w_read(0206, MEMSPACE) == 05);
    assert(reg[3] == 01000);

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение из памяти было перемещено в регистр R1
    assert(reg[1] == 05);

    // Проверяем, что значение регистра R3 уменьшилось на 2
    assert(reg[3] == 01000);

    // Проверяем, что значение в памяти по адресу 0206 не изменилось
    assert(w_read(0206, MEMSPACE) == 05);

    // Проверяем, что значение в памяти по адресу 01000 не изменилось
    assert(w_read(01000, MEMSPACE) == 0206);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения регистров и памяти
    cleanup();
}


