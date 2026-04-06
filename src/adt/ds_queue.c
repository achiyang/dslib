#include "dslib/ds_queue.h"
#include "dslib/ds_deque.h"

#include <stdlib.h>

struct ds_queue {
    ds_deque *impl;
};

ds_queue *ds_queue_create(const ds_type *type)
{
    ds_queue *queue;

    queue = (ds_queue *)malloc(sizeof(ds_queue));
    if (!queue) {
        return NULL;
    }

    queue->impl = ds_deque_create(type);
    if (!queue->impl) {
        free(queue);
        return NULL;
    }

    return queue;
}

void ds_queue_destroy(ds_queue *queue)
{
    if (!queue) {
        return;
    }

    ds_deque_destroy(queue->impl);
    free(queue);
}

size_t ds_queue_size(const ds_queue *queue)
{
    return queue ? ds_deque_size(queue->impl) : 0;
}

int ds_queue_empty(const ds_queue *queue)
{
    return queue ? ds_deque_empty(queue->impl) : 1;
}

const ds_type *ds_queue_type(const ds_queue *queue)
{
    return queue ? ds_deque_type(queue->impl) : NULL;
}

ds_status ds_queue_front(const ds_queue *queue, void *out)
{
    if (!queue) {
        return DS_ERR;
    }

    return ds_deque_front(queue->impl, out);
}

ds_status ds_queue_enqueue(ds_queue *queue, const void *elem)
{
    if (!queue) {
        return DS_ERR;
    }

    return ds_deque_push_back(queue->impl, elem);
}

ds_status ds_queue_dequeue(ds_queue *queue)
{
    if (!queue) {
        return DS_ERR;
    }

    return ds_deque_pop_front(queue->impl);
}

ds_status ds_queue_clear(ds_queue *queue)
{
    if (!queue) {
        return DS_ERR;
    }

    return ds_deque_clear(queue->impl);
}