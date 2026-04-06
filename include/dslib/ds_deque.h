#ifndef DSLIB_DS_DEQUE_H
#define DSLIB_DS_DEQUE_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_deque ds_deque;

/* create / destroy */
ds_deque *ds_deque_create(const ds_type *type);
void ds_deque_destroy(ds_deque *dq);

/* properties */
size_t ds_deque_size(const ds_deque *dq);
size_t ds_deque_capacity(const ds_deque *dq);
int ds_deque_empty(const ds_deque *dq);
const ds_type *ds_deque_type(const ds_deque *dq);

/* element access */
ds_status ds_deque_front(const ds_deque *dq, void *out);
ds_status ds_deque_back(const ds_deque *dq, void *out);

/* modifiers */
ds_status ds_deque_push_front(ds_deque *dq, const void *elem);
ds_status ds_deque_push_back(ds_deque *dq, const void *elem);
ds_status ds_deque_pop_front(ds_deque *dq);
ds_status ds_deque_pop_back(ds_deque *dq);
ds_status ds_deque_reserve(ds_deque *dq, size_t new_capacity);
ds_status ds_deque_clear(ds_deque *dq);

#endif /* DSLIB_DS_DEQUE_H */