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


void load_data(FILE * stream)
{
    address adr;
    int n;
    word tmp;
    while (fscanf(stream, "%x%x", &adr, &n) == 2)
    {
        for (address i = adr; i < adr + n; i ++)
        {
            fscanf(stream, "%x", &tmp);
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

void load_file(const char * filename)
{
    FILE * fin = fopen(filename, "r");
    if (fin == NULL)
    {
        perror(filename);
        exit(1);
    }
    load_data(fin);
    fclose(fin);
}


int main(int argc, char * argv[])
{
    const char * filename;
    if (argc == 1)
    {
        filename = "stdin";
    }
    else if (argc >= 2)
    {
        filename = argv[argc - 1];
    }


    load_file(filename);

    mem_dump(0x40, 20);
    printf("\n");
    mem_dump(0x200, 0x26);

    return 0;
}