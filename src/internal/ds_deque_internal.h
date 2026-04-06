#ifndef DSLIB_DS_DEQUE_INTERNAL_H
#define DSLIB_DS_DEQUE_INTERNAL_H

#include <stddef.h>

#include "dslib/ds_type.h"

struct ds_deque {
    const ds_type *type;
    size_t size;
    size_t capacity;
    size_t front;
    unsigned char *data;
};

static inline size_t ds_deque_slot_index(const struct ds_deque *dq, size_t logical_index) {
    return (dq->front + logical_index) % dq->capacity;
}

static inline void *ds_deque_elem_ptr(struct ds_deque *dq, size_t logical_index) {
    size_t slot = ds_deque_slot_index(dq, logical_index);
    return (void *)(dq->data + (slot * dq->type->size));
}

static inline const void *ds_deque_elem_cptr(const struct ds_deque *dq, size_t logical_index) {
    size_t slot = ds_deque_slot_index(dq, logical_index);
    return (const void *)(dq->data + (slot * dq->type->size));
}

#endif