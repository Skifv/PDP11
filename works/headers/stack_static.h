#include <stdio.h>
#include <stdlib.h>

typedef int Data;

#define FORMAT_SPEC "%d"
#define N 10

typedef struct {
    unsigned int n;
    Data a[N];
} Stack;

void stack_init(Stack * s)
{
    s->n = 0;   
}    

void stack_push(Stack * s, Data x)
{
    s->a[s->n] = x;
    s->n += 1;
}

Data stack_pop(Stack * s)
{
    s->n -= 1;
    return s->a[s->n];
}

Data stack_get(Stack * s)
{
    return s->a[s->n - 1];   
}

void stack_clear(Stack * s)
{
    s->n = 0;   
}

void stack_print(Stack * s)
{
    int i;
    
    for (i = 0; i < s->n; i++)
    {
        printf(FORMAT_SPEC " ", s->a[i]);
    }
    printf("\n");
}

int  stack_size(Stack * s)
{
    return s->n;
}


int  stack_is_empty(Stack * s)
{
    return s->n == 0;
}
int  stack_is_full(Stack * s)
{
    return s->n >= (sizeof(s->a) / sizeof(s->a[0]));
}
