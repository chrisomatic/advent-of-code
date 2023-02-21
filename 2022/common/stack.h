#pragma once

typedef struct
{
    int items[100];
    int item_count;
} Stack;

void stack_push(Stack* s, int x)
{
    for(int i = s->item_count -1; i >= 0; --i)
    {
        s->items[i+1] = s->items[i];
    }
    s->items[0] = x;
    s->item_count++;
}

void stack_push_many(Stack* s, int* x, int count)
{
    for(int i = s->item_count -1; i >= 0; --i)
    {
        s->items[i+count] = s->items[i];
    }

    for(int i = 0; i < count; ++i)
    {
        s->items[i] = x[i];
    }
    s->item_count += count;
}

void stack_reverse(Stack* s)
{
    Stack t = {0};
    int item_count = s->item_count;
    for(int i = 0; i < item_count; ++i)
    {
        t.items[i] = s->items[item_count - i-1];
    }

    memcpy(s->items,t.items,100*sizeof(int));
}

int stack_pop(Stack* s)
{
    if(s->item_count == 0)
        return 0;

    int item = s->items[0];
    for(int i = 0; i < s->item_count; ++i)
    {
        s->items[i] = s->items[i+1];
    }
    s->item_count--;

    return item;
}

void stack_pop_many(Stack* s, int* x, int count)
{
    if(s->item_count == 0)
        return;

    int item = s->items[0];

    for(int i = 0; i < count; ++i)
    {
        x[i] = s->items[i];
    }

    for(int i = 0; i < s->item_count; ++i)
    {
        s->items[i] = s->items[i+count];
    }

    s->item_count -= count;

    return;
}

int stack_peek(Stack* s, int index)
{
    if(s->item_count <= index)
        return 0;

    return s->items[index];
}

void stack_print(Stack* s)
{
    printf("Stack %p (size: %d)\n",s,s->item_count);
    for(int j = 0; j < s->item_count; ++j)
    {
        int c = stack_peek(s,j);
        printf(" %c",c);
    }
    printf("\n");
}
