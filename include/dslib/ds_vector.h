#ifndef DSLIB_DS_VECTOR_H
#define DSLIB_DS_VECTOR_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_vector ds_vector;

/* create / destroy */
ds_vector *ds_vector_create(const ds_type *type);
void ds_vector_destroy(ds_vector *vec);

/* properties */
size_t ds_vector_size(const ds_vector *vec);
size_t ds_vector_capacity(const ds_vector *vec);
int ds_vector_empty(const ds_vector *vec);
const ds_type *ds_vector_type(const ds_vector *vec);

/* element access */
ds_status ds_vector_get(const ds_vector *vec, size_t index, void *out);
ds_status ds_vector_set(ds_vector *vec, size_t index, const void *elem);
ds_status ds_vector_back(const ds_vector *vec, void *out);

/* modifiers */
ds_status ds_vector_push_back(ds_vector *vec, const void *elem);
ds_status ds_vector_pop_back(ds_vector *vec);
ds_status ds_vector_reserve(ds_vector *vec, size_t new_capacity);
ds_status ds_vector_clear(ds_vector *vec);

#endif /* DSLIB_DS_VECTOR_H */