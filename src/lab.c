#include <stdlib.h>
#include "lab.h"

struct queue
{
    void **data;
    int capacity;
    int size;
    bool shutdown;
};

queue_t queue_init(int capacity)
{
    // Allocate the queue
    struct queue *q = (queue_t)malloc(sizeof(struct queue));
    if (q == NULL)
    {
        return NULL;
    }
    q->capacity = capacity;
    q->size = 0;
    q->shutdown = true;

    // Allocate the queue's array
    q->data = malloc(sizeof(void *) * capacity);
    if (q->data == NULL)
    {
        free(q);
        return NULL;
    }

    return q;
}

void queue_destroy(queue_t q)
{
    if (q == NULL)
    {
        return;
    }

    if (q->data)
    {
        free(q->data);
        q->data = NULL;
    }

    free(q);
}

void enqueue(queue_t q, void *data)
{
    if (q == NULL || data == NULL)
    {
        return;
    }

    if (q->size == q->capacity || q->shutdown)
    {
        return;
    }

    // Enqueue at next open spot
    q->data[q->size] = data;
    q->size++;
}

void *dequeue(queue_t q)
{
    if (q == NULL)
    {
        return NULL;
    }

    if (q->size == 0)
    {
        return NULL;
    }

    void *removed = q->data[0];

    // Shift everything left
    for (int i = 1; i < q->size; ++i)
    {
        q->data[i - 1] = q->data[i];
    }
    q->size--;

    return removed;
}

void queue_shutdown(queue_t q)
{
    if (q == NULL)
    {
        return;
    }
    q->shutdown = true;
}

bool is_empty(queue_t q)
{
    if (q == NULL)
    {
        return NULL;
    }
    return q->size == 0;
}

bool is_shutdown(queue_t q)
{
    if (q == NULL)
    {
        return NULL;
    }
    return q->shutdown;
}