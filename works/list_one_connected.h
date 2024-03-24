#include <stdio.h>
#include <stdlib.h>

#define FORMAT_SPEC "%d"


typedef int Data;
struct Node {
    Data val;               // данные, которые хранятся в одном элементе
    struct Node * next;     // указатель на следующий элемент списка
};

struct Node * list_create()
{
    return NULL;
}
void list_push (struct Node ** plist, Data x)
{
    struct Node * p = (struct Node *)malloc(sizeof(struct Node));
    p->val = x;
    p->next = *plist;
    *plist = p;
}
Data list_pop (struct Node ** plist)
{
    Data res = (*plist)->val;
    struct Node * p = *plist;
    *plist = p->next;
    free(p);
    return res;
}
Data list_get(struct Node * list)
{
    return list->val;
}
void list_print (struct Node * list)
{
    for (struct Node * plist = list; plist != NULL; plist = plist->next)
    {
        printf(FORMAT_SPEC " ", plist->val);
    }
    printf("\n");
}
int list_size(struct Node * list)
{
    int n = 0;

    for (struct Node * plist = list; plist != NULL; plist = plist->next)
    {
        n++;
    }

    return n;
}

int list_is_empty (struct Node * list)
{
    return list == NULL;
}
void list_clear(struct Node ** plist)
{
    while(!list_is_empty(*plist))
    {
        list_pop(plist);
    }
}