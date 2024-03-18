#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define PDP11_MEMSIZE (64 * 1024)

byte mem[PDP11_MEMSIZE];

byte b_read(address adr);
void b_write(address adr, byte value);
word w_read(address adr);
void w_write(address adr, word value);

byte b_read(address adr)
{
    return mem[adr];
}

void b_write(address adr, byte value)
{
    mem[adr] = value;
}

word w_read(address adr)
{
    byte b0 = mem[adr];
    byte b1 = mem[adr + 1];

    word res = 0;

    res = b1;
    res = res << 8;
    res = res | b0;

    return res & 0xFFFF;
}

void w_write (address adr, word val)
{
    mem[adr + 1] = (val & 0xFF00) >> 8;
    mem[adr] = val & 0x00FF;
}


void test_mem()
{
    address a;
    byte b0, b1, bres;
    word w, wres;


    // пишем байт, читаем байт
    fprintf(stderr, "Пишем и читаем байт по четному адресу\n");
    a = 0;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    // тут полезно написать отладочную печать a, b0, bres
    fprintf(stderr, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);
    // аналогично стоит проверить чтение и запись по нечетному адресу


    // пишем слово, читаем слово
    fprintf(stderr, "Пишем и читаем слово\n");
    a = 2;        // другой адрес
    w = 0x3432;
    
    w_write(a, w);
    wres = w_read(a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o w=%04x wres=%04x, mem[a] = %02x, mem[a + 1] = %02x\n", a, w, wres, mem[a], mem[a+1]);
    assert(w == wres);


    // пишем 2 байта, читаем 1 слово
    fprintf(stderr, "Пишем 2 байта, читаем слово\n");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);


    // еще тесты
}

int main()
{
    test_mem();
    return 0;
}

