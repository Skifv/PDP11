#include <stdlib.h>
#include <stdio.h>
#define N1 4  // pop:  n < N1*size? => realloc, new_size = size / N2 + N3
#define N2 2  // push: n >= size?   => realloc, new_size = size * N2 + N3
#define N3 1  // увеличиваем/уменьшаем на ... + N3

#define FORMAT_SPEC "%d"
typedef int Data;

typedef struct {
    int n;
    int size;
    Data * a;
} Stack;


void stack_print(Stack * s)
{
    int i = 0;

    for (i = 0; i < s->n; i++)
    {
        printf(FORMAT_SPEC " ", s->a[i]);
    }
    printf("\n");
}


Stack * stack_create(int size)
{
    Stack * res = (Stack *)malloc(sizeof(Stack));
    
    res->a = (Data *)calloc(size, sizeof(Data));
    
    res->size = size;
    
    res->n = 0;
    
    return res;
}

void stack_push(Stack * s, Data x)
{
    if (s->n >= s->size)
    {
        s->size *= N2;
        s->a = (Data *)realloc(s->a, s->size * sizeof(Data) + N3);
    }

    s->a[s->n] = x;
    s->n += 1;
}

Data stack_pop(Stack * s)
{
    s->n -= 1;

    if (s->n < (s->size / N1))
    {
        s->a = (Data *)realloc(s->a, (s->size / N2) * sizeof(Data) + N3);
    }

    return s->a[s->n];

}

Data stack_get(Stack * s)
{
    return s->a[s->n - 1];
}
int  stack_size(Stack * s)
{
    return s->n;
}
int  stack_is_empty(Stack * s)
{
    return s->n == 0;
}
void stack_clear(Stack * s)
{
    s->n = 0;
}
Stack * stack_destroy(Stack * s)
{
    if (s == NULL)
    {
        return NULL;
    }

    s->n = 0;
    s->size = 0;
    free(s->a);
    free(s);

    return NULL;
}