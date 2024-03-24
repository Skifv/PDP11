#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int Data;

#define PRINTER 0
#define FORMAT_SPEC "%d"
#define HAND 5

typedef struct Node Node;
struct Node {
    struct Node * next;
    struct Node * prev;
    Data data;
};

void list_print(struct Node * list);
void list_print_back(struct Node * list);
int list_is_empty (struct Node * list);
void list_init (struct Node * list);
void list_insert (struct Node * list, struct Node * t);
void list_insert_before (struct Node * list, struct Node * t);
Node * list_remove (struct Node * t);
struct Node * list_push_front (struct Node * list, Data d);
struct Node * list_push_back (struct Node * list, Data d);
Data list_pop_front (struct Node * list);
Data list_pop_back (struct Node * list);
Data list_delete (struct Node * t);
void list_clear (struct Node * list);

void list_print(struct Node * list)
{
    for (Node * p = list->next; p != list; p = p->next)
    {
        printf(FORMAT_SPEC " ", p->data);
    }
    printf("\n");
}

void list_print_back(struct Node * list)
{
    for (Node * p = list->next; p != list; p = p->prev)
    {
        printf(FORMAT_SPEC " ", p->data);
    }
    printf("\n");
}

int list_is_empty (struct Node * list)
{
    return list->next == list;
}

void list_init (struct Node * list)
{
    list->next = list->prev = list;
}

void list_insert (struct Node * list, struct Node * t)
{
    t->next          = list->next;
    t->prev          = list;
    list->next->prev = t;
    list->next       = t;
}

void list_insert_before (struct Node * list, struct Node * t)
{
    list_insert(list->prev, t);
}

Node * list_remove (struct Node * t)
{
    t->prev->next = t->next;
    t->next->prev = t->prev;
    return t;
}

struct Node * list_push_front (struct Node * list, Data d)
{
    Node * res = (Node *)malloc(sizeof(Node));

    res->data = d;
    list_insert(list, res);

    return res;
}

struct Node * list_push_back (struct Node * list, Data d)
{
    return list_push_front(list->prev, d);    
}

Data list_get_front (struct Node * list)
{
    return list->prev->data;
}

Data list_get_back (struct Node * list)
{
    return list->next->data;
}

Data list_pop_front (struct Node * list)
{
    return list_delete(list->next);
}

Data list_pop_back (struct Node * list)
{
    return list_delete(list->prev);
}

Data list_delete (struct Node * t)
{
    Data res = t->data;

    list_remove(t);
    free(t);

    return res;
}

void list_clear (struct Node * list)
{
    while (!list_is_empty(list))
    {
        list_delete(list->next);
    }
}


#define prn fprintf(stderr, "Function: %s, %d\n", __FUNCTION__, __LINE__);

void get_cards(Node * deck)
{
    int c;
    for (int i = 0; i < 5; i++)
    {
        scanf("%d", &c);
        list_push_front(deck, c);
    }
}

void one_step(Node * first, Node * second)
{
    Node * first_card  = list_remove(first->prev);
    Node * second_card = list_remove(second->prev);

    if (((first_card->data == 0 && second_card->data == 9) || first_card->data > second_card->data) && 
         (first_card->data != 9 && second_card->data != 0))
    {
        list_insert(first, first_card);
        list_insert(first, second_card);
    }
    else
    {
        list_insert(second, first_card);
        list_insert(second, second_card);
    }
    
    return;
} 


int main(void)
{
    Node first;
    Node second;
    int f_win = 0;
    int s_win = 0;
    int steps = 0;

    list_init(&first);
    list_init(&second);

    get_cards(&first);
    get_cards(&second);
#if PRINTER
    list_print(&first);
    list_print(&second);
#endif

    while(!list_is_empty(&first) && !list_is_empty(&second))
    {
        one_step(&first, &second);
        steps += 1;

        if (steps >= 50)
        {
            printf("botva\n");
            goto END;
            return 0;
        }
#if PRINTER
        printf("--------\n");
        list_print(&first);
        list_print(&second);
#endif
    }

    f_win = !list_is_empty(&first);
    s_win = !list_is_empty(&second);

    if (f_win)
    {
        printf("first ");
    }
    else if (s_win)
    {
        printf("second ");
    }
    else 
    {
        printf("error");
        return 1;
    }

    printf("%d\n", steps);
    
END:

    list_clear(&first);
    list_clear(&second);
    return 0;
}