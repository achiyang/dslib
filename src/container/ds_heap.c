#include "dslib/ds_heap.h"
#include "dslib/ds_vector.h"
#include "ds_vector_internal.h"

#include <stdlib.h>
#include <string.h>

struct ds_heap {
    ds_vector *impl;
};

static inline size_t ds_heap_parent(size_t i) { return (i - 1) / 2; }
static inline size_t ds_heap_left(size_t i)   { return (2 * i) + 1; }
static inline size_t ds_heap_right(size_t i)  { return (2 * i) + 2; }

static int ds_heap_compare_at(const ds_heap *heap, size_t i, size_t j) {
    const ds_type *type = heap->impl->type;
    const void *a = ds_vector_elem_cptr(heap->impl, i);
    const void *b = ds_vector_elem_cptr(heap->impl, j);

    return type->compare(a, b);
}

static ds_status ds_heap_sift_up(ds_heap *heap, size_t index) {
    const ds_type *type = heap->impl->type;
    size_t elem_size = type->size;
    unsigned char *temp;

    temp = (unsigned char *)malloc(elem_size);
    if (!temp) {
        return DS_ERR;
    }

    memcpy(temp, ds_vector_elem_cptr(heap->impl, index), elem_size);

    while (index > 0) {
        size_t parent = ds_heap_parent(index);
        const void *parent_ptr = ds_vector_elem_cptr(heap->impl, parent);

        if (type->compare(temp, parent_ptr) <= 0) {
            break;
        }

        memcpy(ds_vector_elem_ptr(heap->impl, index), parent_ptr, elem_size);
        index = parent;
    }

    memcpy(ds_vector_elem_ptr(heap->impl, index), temp, elem_size);
    free(temp);

    return DS_OK;
}

static ds_status ds_heap_sift_down(ds_heap *heap, size_t index) {
    const ds_type *type = heap->impl->type;
    size_t elem_size = type->size;
    size_t size = heap->impl->size;
    unsigned char *temp;

    temp = (unsigned char *)malloc(elem_size);
    if (!temp) {
        return DS_ERR;
    }

    memcpy(temp, ds_vector_elem_cptr(heap->impl, index), elem_size);

    while (1) {
        size_t left = ds_heap_left(index);
        size_t right = ds_heap_right(index);
        size_t best = left;

        if (left >= size) {
            break;
        }

        if (right < size && ds_heap_compare_at(heap, right, left) > 0) {
            best = right;
        }

        if (type->compare(ds_vector_elem_cptr(heap->impl, best), temp) <= 0) {
            break;
        }

        memcpy(
            ds_vector_elem_ptr(heap->impl, index),
            ds_vector_elem_cptr(heap->impl, best),
            elem_size
        );
        index = best;
    }

    memcpy(ds_vector_elem_ptr(heap->impl, index), temp, elem_size);
    free(temp);

    return DS_OK;
}

ds_heap *ds_heap_create(const ds_type *type) {
    ds_heap *heap;

    if (!type || !type->compare || type->size == 0) {
        return NULL;
    }

    heap = (ds_heap *)malloc(sizeof(ds_heap));
    if (!heap) {
        return NULL;
    }

    heap->impl = ds_vector_create(type);
    if (!heap->impl) {
        free(heap);
        return NULL;
    }

    return heap;
}

void ds_heap_destroy(ds_heap *heap) {
    if (!heap) {
        return;
    }

    ds_vector_destroy(heap->impl);
    free(heap);
}

size_t ds_heap_size(const ds_heap *heap) {
    return heap ? ds_vector_size(heap->impl) : 0;
}

int ds_heap_empty(const ds_heap *heap) {
    return heap ? ds_vector_empty(heap->impl) : 1;
}

const ds_type *ds_heap_type(const ds_heap *heap) {
    return heap ? ds_vector_type(heap->impl) : NULL;
}

ds_status ds_heap_top(const ds_heap *heap, void *out) {
    if (!heap || !out || ds_vector_size(heap->impl) == 0) {
        return DS_ERR;
    }

    return ds_vector_get(heap->impl, 0, out);
}

ds_status ds_heap_push(ds_heap *heap, const void *elem) {
    size_t index;

    if (!heap || !elem) {
        return DS_ERR;
    }

    if (ds_vector_push_back(heap->impl, elem) != DS_OK) {
        return DS_ERR;
    }

    index = ds_vector_size(heap->impl) - 1;

    if (ds_heap_sift_up(heap, index) != DS_OK) {
        ds_vector_pop_back(heap->impl);
        return DS_ERR;
    }

    return DS_OK;
}

ds_status ds_heap_pop(ds_heap *heap) {
    size_t size;
    
    if (!heap) {
        return DS_ERR;
    }

    size = heap->impl->size;

    if (size == 0) {
        return DS_ERR;
    }

    if (size == 1) {
        return ds_vector_pop_back(heap->impl);
    }

    ds_vector_destroy_at(heap->impl, 0);

    memcpy(
        ds_vector_elem_ptr(heap->impl, 0),
        ds_vector_elem_cptr(heap->impl, size - 1),
        heap->impl->type->size
    );

    ds_vector_drop_back_raw(heap->impl);

    return ds_heap_sift_down(heap, 0);
}

ds_status ds_heap_clear(ds_heap *heap) {
    if (!heap) {
        return DS_ERR;
    }

    return ds_vector_clear(heap->impl);
}

ds_status ds_heap_reserve(ds_heap *heap, size_t new_capacity) {
    if (!heap) {
        return DS_ERR;
    }

    return ds_vector_reserve(heap->impl, new_capacity);
}