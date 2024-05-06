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

// Тест для команды clr (очистка регистра)
void test_clr_command(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[1] = 12345; // Произвольное значение в регистре R1

    // Проверяем начальное состояние регистра
    assert(reg[1] == 12345);

    // Выполняем команду clr для регистра R1
    Command cmd = parse_cmd(0005001); // clr R1 (очищает значение в указанном регистре)

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение в регистре R1 стало равным нулю
    assert(reg[1] == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения регистров и памяти
    cleanup();
}

// Тест для команды sob (уменьшение регистра и условный переход)
void test_sob_command(void)
{
    // Сохраняем исходные значения регистров и памяти
    save_original_values();

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Устанавливаем начальные значения
    reg[2] = 5; // Произвольное начальное значение в регистре R2
    pc = 100; // Произвольное начальное значение счетчика команд

    // Проверяем начальное состояние регистра и счетчика команд
    assert(reg[2] == 5);
    assert(pc == 100);

    // Выполняем команду sob с произвольным параметром NN
    Command cmd = parse_cmd(0077202); // sob R2, 2

    // Выполняем команду
    cmd.do_command();

    // Проверяем, что значение в регистре R2 уменьшилось на 1
    assert(reg[2] == 4);

    // Проверяем, что pc уменьшилось на 2*NN_ARG.val, так как значение в регистре R2 после уменьшения не равно 0
    assert(pc == 100 - 2 * 2);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения регистра и счетчика команд
    cleanup();
}

// Тест для команды ccc (очистка флагов)
void test_ccc_command(void)
{
    // Сохраняем текущие значения флагов
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду ccc
    Command cmd = parse_cmd(000257); // ccc
    cmd.do_command();

    // Проверяем, что все флаги были успешно очищены
    assert(flags.N == 0);
    assert(flags.Z == 0);
    assert(flags.V == 0);
    assert(flags.C == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения флагов
    flags.N = original_N;
    flags.Z = original_Z;
    flags.V = original_V;
    flags.C = original_C;
}

// Тест для команды clc (очистка флага C)
void test_clc_command(void)
{
    // Сохраняем текущее значение флага C
    unsigned char original_C = flags.C;

    // Устанавливаем новое начальное значение для теста
    flags.C = 1;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду clc
    Command cmd = parse_cmd(000241); // clc
    cmd.do_command();

    // Проверяем, что флаг C был успешно очищен
    assert(flags.C == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага C
    flags.C = original_C;
}

// Тест для команды cln (очистка флага N)
void test_cln_command(void)
{
    // Сохраняем текущее значение флага N
    unsigned char original_N = flags.N;

    // Устанавливаем новое начальное значение для теста
    flags.N = 1;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду cln
    Command cmd = parse_cmd(000250); // cln
    cmd.do_command();

    // Проверяем, что флаг N был успешно очищен
    assert(flags.N == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага N
    flags.N = original_N;
}

// Тест для команды clv (очистка флага V)
void test_clv_command(void)
{
    // Сохраняем текущее значение флага V
    unsigned char original_V = flags.V;

    // Устанавливаем новое начальное значение для теста
    flags.V = 1;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду clv
    Command cmd = parse_cmd(000242); // clv
    cmd.do_command();

    // Проверяем, что флаг V был успешно очищен
    assert(flags.V == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага V
    flags.V = original_V;
}

// Тест для команды clz (очистка флага Z)
void test_clz_command(void)
{
    // Сохраняем текущее значение флага Z
    unsigned char original_Z = flags.Z;

    // Устанавливаем новое начальное значение для теста
    flags.Z = 1;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду clz
    Command cmd = parse_cmd(000244); // clz
    cmd.do_command();

    // Проверяем, что флаг Z был успешно очищен
    assert(flags.Z == 0);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага Z
    flags.Z = original_Z;
}

// Тест для команды nop (ничего не делать)
void test_nop_command(void)
{
    // Сохраняем текущие значения флагов
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду nop
    Command cmd = parse_cmd(000240); // nop
    cmd.do_command();

    // Проверяем, что флаги остались неизменными
    assert(flags.N == original_N);
    assert(flags.Z == original_Z);
    assert(flags.V == original_V);
    assert(flags.C == original_C);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);
}

// Тест для команды scc (установка всех флагов)
void test_scc_command(void)
{
    // Сохраняем текущие значения флагов
    unsigned char original_N = flags.N;
    unsigned char original_Z = flags.Z;
    unsigned char original_V = flags.V;
    unsigned char original_C = flags.C;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду scc
    Command cmd = parse_cmd(000277); // scc
    cmd.do_command();

    // Проверяем, что все флаги были успешно установлены
    assert(flags.N == 1);
    assert(flags.Z == 1);
    assert(flags.V == 1);
    assert(flags.C == 1);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходные значения флагов
    flags.N = original_N;
    flags.Z = original_Z;
    flags.V = original_V;
    flags.C = original_C;
}

// Тест для команды sec (установка флага C)
void test_sec_command(void)
{
    // Сохраняем текущее значение флага C
    unsigned char original_C = flags.C;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду sec
    Command cmd = parse_cmd(000261); // sec
    cmd.do_command();

    // Проверяем, что флаг C был успешно установлен
    assert(flags.C == 1);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага C
    flags.C = original_C;
}

// Тест для команды sen (установка флага N)
void test_sen_command(void)
{
    // Сохраняем текущее значение флага N
    unsigned char original_N = flags.N;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду sen
    Command cmd = parse_cmd(000270); // sen
    cmd.do_command();

    // Проверяем, что флаг N был успешно установлен
    assert(flags.N == 1);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага N
    flags.N = original_N;
}

// Тест для команды sev (установка флага V)
void test_sev_command(void)
{
    // Сохраняем текущее значение флага V
    unsigned char original_V = flags.V;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду sev
    Command cmd = parse_cmd(000262); // sev
    cmd.do_command();

    // Проверяем, что флаг V был успешно установлен
    assert(flags.V == 1);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага V
    flags.V = original_V;
}

// Тест для команды sez (установка флага Z)
void test_sez_command(void)
{
    // Сохраняем текущее значение флага Z
    unsigned char original_Z = flags.Z;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    // Выполняем команду sez
    Command cmd = parse_cmd(000264); // sez
    cmd.do_command();

    // Проверяем, что флаг Z был успешно установлен
    assert(flags.Z == 1);

    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение флага Z
    flags.Z = original_Z;
}

// Тест для команды br (безусловный переход)
void test_br_command(void)
{
    // Сохраняем текущее значение PC
    word original_PC = pc;

    pc = 0100;

    // Выводим информацию о тесте
    trace(TRACE, __FUNCTION__);

    trace(TRACE, "\n");
    // Выполняем команду br
    Command cmd = parse_cmd(000402); // br
    cmd.do_command();

    // Проверяем, что PC изменился на 2 слова вперед
    assert(pc == 0104);

    pc = 01004;

    // Выполняем команду br
    cmd = parse_cmd(000775); // br
    cmd.do_command();

    // Проверяем, что PC изменился на 3 слова назад
    assert(pc == 01000);


trace(TRACE, "\n");
    // Выводим результат теста
    print_all_OK(__FUNCTION__);

    // Восстанавливаем исходное значение PC
    pc = original_PC;
}
