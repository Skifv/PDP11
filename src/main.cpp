#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

const char * filename;

typedef unsigned char byte;
typedef unsigned int word;
typedef word address;

#define PDP11_MEMSIZE (64 * 1024)

word reg[8];    // reg[i] - это регистр Ri

byte mem[PDP11_MEMSIZE];

enum LEVELS {
    DEBUG,
    TRACE,
    INFO,
    ERROR
};

int CURRENT_LEVEL = DEBUG;

int set_input(int argc, char * argv[]);
byte b_read(address adr);
void b_write(address adr, byte value);
word w_read(address adr);
void w_write(address adr, word value);
void mem_dump(address adr, int size);
void load_file(const char * filename);
int log(int level, const char * format, ...);
int set_log_level(int level);


int main(int argc, char * argv[])
{
    set_input(argc, argv);

    load_file(filename);

    mem_dump(0x40, 20);
    putchar('\n');
    mem_dump(0x200, 0x26);

    return 0;
}

int set_input(int argc, char * argv[])
{
    if (argc == 1)
    {
        filename = "stdin";
    }
    else if (argc >= 2)
    {
        filename = argv[argc - 1];
    }
    
    if (filename == NULL)
    {
        perror(filename);
        exit(1);
    }

    return 0;
}

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
        log(INFO, "%06o: %06o %04x\n", i, w, w);
    }  
}

void load_file(const char * file)
{
    FILE * fin = fopen(file, "r");
    if (fin == NULL)
    {
        perror(file);
        exit(1);
    }
    load_data(fin);
    fclose(fin);
}

int log(int level, const char * format, ...)
{
    int sum = 0;

    if (level >= CURRENT_LEVEL)
    {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
    
    return sum;
}

int set_log_level(int level)
{
    int prev_level = CURRENT_LEVEL;
    CURRENT_LEVEL = level;
    return prev_level;
}

