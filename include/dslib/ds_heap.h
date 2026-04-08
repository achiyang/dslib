#ifndef DSLIB_DS_HEAP_H
#define DSLIB_DS_HEAP_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_heap ds_heap;

/* create / destroy */
ds_heap *ds_heap_create(const ds_type *type);
void ds_heap_destroy(ds_heap *heap);

/* properties */
size_t ds_heap_size(const ds_heap *heap);
int ds_heap_empty(const ds_heap *heap);
const ds_type *ds_heap_type(const ds_heap *heap);

/* element access */
ds_status ds_heap_top(const ds_heap *heap, void *out);

/* modifiers */
ds_status ds_heap_push(ds_heap *heap, const void *elem);
ds_status ds_heap_pop(ds_heap *heap);
ds_status ds_heap_clear(ds_heap *heap);
ds_status ds_heap_reserve(ds_heap *heap, size_t new_capacity);

#endif /* DSLIB_DS_HEAP_H */