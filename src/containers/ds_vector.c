#include <dslib/ds_vector.h>
#include <ds_vector_internal.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static void ds_vector_elem_copy_in(ds_vector *vec, size_t index, const void *src) {
    void *dest = ds_vector_elem_ptr(vec, index);

    if (vec->type->copy) {
        vec->type->copy(dest, src);
    }
    else {
        memcpy(dest, src, vec->type->size);
    }
}

static void ds_vector_elem_destroy_at(ds_vector *vec, size_t index) {
    if (vec->type->destroy) {
        void *elem = ds_vector_elem_ptr(vec, index);
        vec->type->destroy(elem);
    }
}

static void ds_vector_destroy_all(ds_vector *vec) {
    size_t i;

    for (i = vec->size; i > 0; --i) {
        ds_vector_elem_destroy_at(vec, i - 1);
    }
}

static ds_status ds_vector_grow(ds_vector *vec) {
    size_t new_capacity;

    if (vec->capacity == 0) {
        new_capacity = 4;
    }
    else {
        if (vec->capacity > __SIZE_MAX__ / 2) {
            return DS_ERR;
        }
        new_capacity = vec->capacity * 2;
    }

    return ds_vector_reserve(vec, new_capacity);
}

ds_vector *ds_vector_create(const ds_type *type) {
    ds_vector *vec;

    if (type == NULL || type->size == 0) {
        return NULL;
    }

    vec = (ds_vector *)malloc(sizeof(ds_vector));
    if (!vec) {
        return NULL;
    }

    vec->type = type;
    vec->size = 0;
    vec->capacity = 0;
    vec->data = NULL;

    return vec;
}

void ds_vector_destroy(ds_vector *vec) {
    if (!vec) {
        return;
    }

    ds_vector_destroy_all(vec);
    free(vec->data);
    free(vec);
}

size_t ds_vector_size(const ds_vector *vec) {
    return vec ? vec->size : 0;
}

size_t ds_vector_capacity(const ds_vector *vec) {
    return vec ? vec->capacity : 0;
}

int ds_vector_empty(const ds_vector *vec) {
    return !vec || vec->size == 0;
}

const ds_type *ds_vector_type(const ds_vector *vec) {
    return vec ? vec->type : NULL;
}

ds_status ds_vector_get(const ds_vector *vec, size_t index, void *out) {
    if (!vec || !out || index >= vec->size) {
        return DS_ERR;
    }

    memcpy(out, ds_vector_elem_cptr(vec, index), vec->type->size);
    return DS_OK;
}

ds_status ds_vector_set(ds_vector *vec, size_t index, const void *elem) {
    if (!vec || !elem || index >= vec->size) {
        return DS_ERR;
    }

    ds_vector_elem_destroy_at(vec, index);
    ds_vector_elem_copy_in(vec, index, elem);

    return DS_OK;
}

ds_status ds_vector_back(const ds_vector *vec, void *out) {
    if (!vec || !out || vec->size == 0) {
        return DS_ERR;
    }

    memcpy(out, ds_vector_elem_cptr(vec, vec->size - 1), vec->type->size);
    return DS_OK;
}

ds_status ds_vector_push_back(ds_vector *vec, const void *elem) {
    if (!vec || !elem) {
        return DS_ERR;
    }

    if (vec->size == vec->capacity) {
        if (ds_vector_grow(vec) != DS_OK) {
            return DS_ERR;
        }
    }

    ds_vector_elem_copy_in(vec, vec->size, elem);
    vec->size++;

    return DS_OK;
}

ds_status ds_vector_pop_back(ds_vector *vec) {
    if (!vec || vec->size == 0) {
        return DS_ERR;
    }

    ds_vector_elem_destroy_at(vec, vec->size - 1);
    vec->size--;

    return DS_OK;
}

ds_status ds_vector_reserve(ds_vector *vec, size_t new_capacity) {
    unsigned char *new_data;

    if (!vec) {
        return DS_ERR;
    }

    if (new_capacity <= vec->capacity) {
        return DS_OK;
    }

    if (new_capacity > __SIZE_MAX__ / vec->type->size) {
        return DS_ERR;
    }

    new_data = (unsigned char *)realloc(vec->data, new_capacity * vec->type->size);
    if (!new_data) {
        return DS_ERR;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;

    return DS_OK;
}

ds_status ds_vector_clear(ds_vector *vec) {
    if (!vec) {
        return DS_ERR;
    }
    
    ds_vector_destroy_all(vec);
    vec->size = 0;

    return DS_OK;
}