#pragma once

#include "../headers/load_dump.h"

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
    if (file == NULL) {
        load_data(stdin);
        return;
    }
    
    FILE * fin = fopen(file, "r");
    if (fin == NULL)
    {
        perror(file);
        exit(1);
    }
    load_data(fin);
    fclose(fin);
}