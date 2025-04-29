#include <stdlib.h>
#include "lab.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

struct queue
{
    void **data;
    int capacity;
    int size;
    bool shutdown;
    pthread_mutex_t lock;
};

queue_t queue_init(int capacity)
{
    if (capacity < 1) {
        return NULL;
    }
    // Allocate the queue
    struct queue *q = (queue_t)malloc(sizeof(struct queue));
    if (q == NULL)
    {
        return NULL;
    }
    q->capacity = capacity;
    q->size = 0;
    q->shutdown = false;

    // Allocate the queue's array
    q->data = malloc(sizeof(void *) * capacity);
    if (q->data == NULL)
    {
        free(q);
        return NULL;
    }

    // Add a lock if everything creates properly
    pthread_mutex_init(&q->lock, NULL);

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

    pthread_mutex_destroy(&q->lock);
    free(q);
    q = NULL;
}

void enqueue(queue_t q, void *data)
{
    pthread_mutex_lock(&q->lock);

    if (q == NULL || data == NULL || q->size == q->capacity || q->shutdown)
    {
        pthread_mutex_unlock(&q->lock);
        return;
    }

    // Enqueue at next open spot
    q->data[q->size] = data;
    q->size++;

    pthread_mutex_unlock(&q->lock);
}

void *dequeue(queue_t q)
{
    pthread_mutex_lock(&q->lock);

    if (q == NULL || q->size == 0)
    {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }

    void *removed = q->data[0];

    // Shift everything left
    for (int i = 1; i < q->size; ++i)
    {
        q->data[i - 1] = q->data[i];
    }
    q->size--;

    pthread_mutex_unlock(&q->lock);

    return removed;
}

void queue_shutdown(queue_t q)
{
    pthread_mutex_lock(&q->lock);

    if (q == NULL)
    {
        pthread_mutex_unlock(&q->lock);
        return;
    }
    q->shutdown = true;

    pthread_mutex_unlock(&q->lock);
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