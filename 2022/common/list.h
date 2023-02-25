#pragma once

#define LIST_MAX 100

typedef struct
{
    int items[LIST_MAX];
    int item_count;
} List_t;

bool list_is_empty(List_t* list)
{
    return (list->item_count == 0);
}
bool list_is_full(List_t* list)
{
    return (list->item_count >= LIST_MAX);
}

bool list_add(List_t* list, int x)
{
    if(list_is_full(list))
        return false;

    list->items[list->item_count++] = x;

    return true;
}

bool list_is_unique(List_t* list, int x)
{
    for(int i = 0; i < list->item_count; ++i)
    {
        if(x == list->items[i])
        {
            return false;
        }
    }
    return true;
}

void list_clear(List_t* list)
{
    list->item_count = 0;
}
