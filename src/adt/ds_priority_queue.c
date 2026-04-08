#include "dslib/ds_priority_queue.h"
#include "dslib/ds_heap.h"

#include <stdlib.h>

struct ds_priority_queue {
    ds_heap *impl;
};

ds_priority_queue *ds_priority_queue_create(const ds_type *type) {
    ds_priority_queue *pq;

    pq = (ds_priority_queue *)malloc(sizeof(ds_priority_queue));
    if (!pq) {
        return NULL;
    }

    pq->impl = ds_heap_create(type);
    if (!pq->impl) {
        free(pq);
        return NULL;
    }

    return pq;
}

void ds_priority_queue_destroy(ds_priority_queue *pq) {
    if (!pq) {
        return;
    }

    ds_heap_destroy(pq->impl);
    free(pq);
}

size_t ds_priority_queue_size(const ds_priority_queue *pq) {
    return pq ? ds_heap_size(pq->impl) : 0;
}

int ds_priority_queue_empty(const ds_priority_queue *pq) {
    return pq ? ds_heap_empty(pq->impl) : 1;
}

const ds_type *ds_priority_queue_type(const ds_priority_queue *pq) {
    return pq ? ds_heap_type(pq->impl) : NULL;
}

ds_status ds_priority_queue_top(const ds_priority_queue *pq, void *out) {
    if (!pq) {
        return DS_ERR;
    }

    return ds_heap_top(pq->impl, out);
}

ds_status ds_priority_queue_push(ds_priority_queue *pq, const void *elem) {
    if (!pq) {
        return DS_ERR;
    }

    return ds_heap_push(pq->impl, elem);
}

ds_status ds_priority_queue_pop(ds_priority_queue *pq) {
    if (!pq) {
        return DS_ERR;
    }

    return ds_heap_pop(pq->impl);
}

ds_status ds_priority_queue_clear(ds_priority_queue *pq) {
    if (!pq) {
        return DS_ERR;
    }

    return ds_heap_clear(pq->impl);
}

ds_status ds_priority_queue_reserve(ds_priority_queue *pq, size_t new_capacity) {
    if (!pq) {
        return DS_ERR;
    }

    return ds_heap_reserve(pq->impl, new_capacity);
}