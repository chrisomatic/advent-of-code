#pragma once

typedef struct
{
    int items[100];
    int item_count;
} Queue;

bool enqueue(Queue* q, int x)
{
    if(q->item_count >= 100)
        return false;

    q->items[q->item_count++] = x;
    return true;
}

int dequeue(Queue* q)
{
    if(q->item_count == 0)
        return 0;

    int item = q->items[0];
    for(int i = 0; i < q->item_count; ++i)
    {
        q->items[i] = q->items[i+1];
    }
    q->item_count--;

    return item;
}

void queue_reset(Queue* q)
{
    q->item_count = 0;
}

bool queue_empty(Queue* q)
{
    return (q->item_count == 0);
}

bool queue_full(Queue* q)
{
    return (q->item_count >= 100);
}

int queue_peek(Queue* q, int index)
{
    if(q->item_count <= index)
        return 0;

    return q->items[index];
}

void queue_print(Queue* q)
{
    printf("Queue %p (size: %d)\n",q,q->item_count);
    for(int j = 0; j < q->item_count; ++j)
    {
        int c = queue_peek(q,j);
        printf(" %c",c);
    }
    printf("\n");
}
