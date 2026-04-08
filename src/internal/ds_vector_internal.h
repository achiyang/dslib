#ifndef DSLIB_DS_VECTOR_INTERNAL_H
#define DSLIB_DS_VECTOR_INTERNAL_H

#include <stddef.h>
#include <string.h>

#include "dslib/ds_type.h"

struct ds_vector {
    const ds_type *type;
    size_t size;
    size_t capacity;
    unsigned char *data;
};

static inline void *ds_vector_elem_ptr(struct ds_vector *vec, size_t index) {
    return (void *)(vec->data + (index * vec->type->size));
}

static inline const void *ds_vector_elem_cptr(const struct ds_vector *vec, size_t index) {
    return (const void *)(vec->data + (index * vec->type->size));
}

static inline void ds_vector_destroy_at(struct ds_vector *vec, size_t index) {
    if (vec->type->destroy) {
        vec->type->destroy(ds_vector_elem_ptr(vec, index));
    }
}

static inline void ds_vector_drop_back_raw(struct ds_vector *vec) {
    vec->size--;
}

#endif /* DSLIB_DS_VECTOR_INTERNAL_H */