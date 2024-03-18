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


void load_data()
{
    address adr;
    int n;
    word tmp;
    while (scanf("%x%x", &adr, &n) == 2)
    {
        for (address i = adr; i < adr + n; i ++)
        {
            scanf("%x", &tmp);
            b_write(i, tmp);
        }
    }
}

void mem_dump(address adr, int size)
{
    word w;
    for(address i = adr; i < adr + size; i += 2)
    {
        w = w_read(i);
        printf("%06o: %06o %04x\n", i, w, w);
    }  
}
