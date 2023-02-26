#pragma once

#define QUEUE_MAX 100

typedef struct
{
    void* items[QUEUE_MAX];
    int item_count;
} Queue_t;

bool enqueue(Queue_t* q, void* x)
{
    if(q->item_count >= QUEUE_MAX)
        return false;

    q->items[q->item_count++] = x;
    return true;
}

void* dequeue(Queue_t* q)
{
    if(q->item_count == 0)
        return 0;

    void* item = q->items[0];
    for(int i = 0; i < q->item_count; ++i)
    {
        q->items[i] = q->items[i+1];
    }
    q->item_count--;

    return item;
}

void queue_reset(Queue_t* q)
{
    q->item_count = 0;
}

bool queue_empty(Queue_t* q)
{
    return (q->item_count == 0);
}

bool queue_full(Queue_t* q)
{
    return (q->item_count >= QUEUE_MAX);
}

void* queue_peek(Queue_t* q, int index)
{
    if(q->item_count <= index)
        return 0;

    return q->items[index];
}


typedef struct
{
    int items[QUEUE_MAX];
    int item_count;
} IntQueue_t;

bool enqueue_int(IntQueue_t* q, int x)
{
    if(q->item_count >= QUEUE_MAX)
        return false;

    q->items[q->item_count++] = x;
    return true;
}

int dequeue_int(IntQueue_t* q)
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

void queue_int_reset(IntQueue_t* q)
{
    q->item_count = 0;
}

bool queue_int_empty(IntQueue_t* q)
{
    return (q->item_count == 0);
}

bool queue_int_full(IntQueue_t* q)
{
    return (q->item_count >= QUEUE_MAX);
}

int queue_int_peek(IntQueue_t* q, int index)
{
    if(q->item_count <= index)
        return 0;

    return q->items[index];
}

void queue_int_print(IntQueue_t* q)
{
    printf("Queue %p (size: %d)\n",q,q->item_count);
    for(int j = 0; j < q->item_count; ++j)
    {
        int c = queue_int_peek(q,j);
        printf(" %c",c);
    }
    printf("\n");
}






typedef struct
{
    int64_t items[QUEUE_MAX];
    int item_count;
} Int64Queue_t;

bool enqueue_int64(Int64Queue_t* q, int64_t x)
{
    if(q->item_count >= QUEUE_MAX)
        return false;

    q->items[q->item_count++] = x;
    return true;
}

int64_t dequeue_int64(Int64Queue_t* q)
{
    if(q->item_count == 0)
        return 0;

    int64_t item = q->items[0];
    for(int i = 0; i < q->item_count; ++i)
    {
        q->items[i] = q->items[i+1];
    }
    q->item_count--;

    return item;
}

void queue_int64_reset(Int64Queue_t* q)
{
    q->item_count = 0;
}

bool queue_int64_empty(Int64Queue_t* q)
{
    return (q->item_count == 0);
}

bool queue_int64_full(Int64Queue_t* q)
{
    return (q->item_count >= QUEUE_MAX);
}

int64_t queue_int64_peek(Int64Queue_t* q, int index)
{
    if(q->item_count <= index)
        return 0;

    return q->items[index];
}

void queue_int64_print(Int64Queue_t* q)
{
    printf("Queue %p (size: %d)\n",q,q->item_count);
    for(int j = 0; j < q->item_count; ++j)
    {
        int64_t c = queue_int64_peek(q,j);
        printf(" %ld",c);
    }
    printf("\n");
}
