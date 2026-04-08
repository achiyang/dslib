#include "dslib/ds_deque.h"
#include "ds_deque_internal.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void ds_deque_elem_copy_in(ds_deque *dq, size_t logical_index, const void *src) {
    void *dest = ds_deque_elem_ptr(dq, logical_index);

    if (dq->type->copy) {
        dq->type->copy(dest, src);
    }
    else {
        memcpy(dest, src, dq->type->size);
    }
}

static void ds_deque_elem_destroy_at(ds_deque *dq, size_t logical_index) {
    if (dq->type->destroy) {
        void *elem = ds_deque_elem_ptr(dq, logical_index);
        dq->type->destroy(elem);
    }
}

static void ds_deque_destroy_all(ds_deque *dq) {
    size_t i;

    for (i = dq->size; i > 0; i--) {
        ds_deque_elem_destroy_at(dq, i - 1);
    }
}

static ds_status ds_deque_grow(ds_deque *dq) {
    size_t new_capacity;

    if (dq->capacity == 0) {
        new_capacity = 4;
    }
    else {
        if (dq->capacity > SIZE_MAX / 2) {
            return DS_ERR;
        }
        new_capacity = dq->capacity * 2;
    }

    return ds_deque_reserve(dq, new_capacity);
}

ds_deque *ds_deque_create(const ds_type *type) {
    ds_deque *dq;

    if (type == NULL || type->size == 0) {
        return NULL;
    }

    dq = (ds_deque *)malloc(sizeof(ds_deque));
    if (!dq) {
        return NULL;
    }

    dq->type = type;
    dq->size = 0;
    dq->capacity = 0;
    dq->front = 0;
    dq->data = NULL;

    return dq;
}

void ds_deque_destroy(ds_deque *dq) {
    if (!dq) {
        return;
    }

    ds_deque_destroy_all(dq);
    free(dq->data);
    free(dq);
}

size_t ds_deque_size(const ds_deque *dq) {
    return dq ? dq->size : 0;
}

size_t ds_deque_capacity(const ds_deque *dq) {
    return dq ? dq->capacity : 0;
}

int ds_deque_empty(const ds_deque *dq) {
    return !dq || dq->size == 0;
}

const ds_type *ds_deque_type(const ds_deque *dq) {
    return dq ? dq->type : NULL;
}

ds_status ds_deque_front(const ds_deque *dq, void *out) {
    if (!dq || !out || dq->size == 0) {
        return DS_ERR;
    }

    memcpy(out, ds_deque_elem_cptr(dq, 0), dq->type->size);
    return DS_OK;
}

ds_status ds_deque_back(const ds_deque *dq, void *out) {
    if (!dq || !out || dq->size == 0) {
        return DS_ERR;
    }

    memcpy(out, ds_deque_elem_cptr(dq, dq->size - 1), dq->type->size);
    return DS_OK;
}

ds_status ds_deque_push_front(ds_deque *dq, const void *elem) {
    if (!dq || !elem) {
        return DS_ERR;
    }

    if (dq->size == dq->capacity) {
        if (ds_deque_grow(dq) != DS_OK) {
            return DS_ERR;
        }
    }

    dq->front = (dq->front + dq->capacity - 1) % dq->capacity;
    ds_deque_elem_copy_in(dq, 0, elem);
    dq->size++;

    return DS_OK;
}

ds_status ds_deque_push_back(ds_deque *dq, const void *elem) {
    if (!dq || !elem) {
        return DS_ERR;
    }

    if (dq->size == dq->capacity) {
        if (ds_deque_grow(dq) != DS_OK) {
            return DS_ERR;
        }
    }

    ds_deque_elem_copy_in(dq, dq->size, elem);
    dq->size++;

    return DS_OK;
}

ds_status ds_deque_pop_front(ds_deque *dq) {
    if (!dq || dq->size == 0) {
        return DS_ERR;
    }

    ds_deque_elem_destroy_at(dq, 0);
    dq->front = (dq->front + 1) % dq->capacity;
    dq->size--;

    if (dq->size == 0) {
        dq->front = 0;
    }

    return DS_OK;
}

ds_status ds_deque_pop_back(ds_deque *dq) {
    if (!dq || dq->size == 0) {
        return DS_ERR;
    }

    ds_deque_elem_destroy_at(dq, dq->size - 1);
    dq->size--;

    if (dq->size == 0) {
        dq->front = 0;
    }

    return DS_OK;
}

ds_status ds_deque_reserve(ds_deque *dq, size_t new_capacity) {
    unsigned char *new_data;
    size_t elem_size, first_count, second_count;

    if (!dq) {
        return DS_ERR;
    }

    if (new_capacity <= dq->capacity) {
        return DS_OK;
    }

    if (new_capacity > SIZE_MAX / dq->type->size) {
        return DS_ERR;
    }

    new_data = (unsigned char *)malloc(new_capacity * dq->type->size);
    if (!new_data) {
        return DS_ERR;
    }

    if (dq->size > 0) {
        elem_size = dq->type->size;
        first_count = dq->capacity - dq->front;

        if (first_count > dq->size) {
            first_count = dq->size;
        }

        second_count = dq->size - first_count;

        memcpy(
            new_data,
            ds_deque_elem_cptr(dq, 0),
            first_count * elem_size
        );

        if (second_count > 0) {
            memcpy(
                new_data + (first_count * elem_size),
                ds_deque_elem_cptr(dq, first_count),
                second_count * elem_size
            );
        }
    }

    free(dq->data);
    dq->data = new_data;
    dq->capacity = new_capacity;
    dq->front = 0;

    return DS_OK;
}

ds_status ds_deque_clear(ds_deque *dq) {
    if (!dq) {
        return DS_ERR;
    }

    ds_deque_destroy_all(dq);
    dq->size = 0;
    dq->front = 0;

    return DS_OK;
}