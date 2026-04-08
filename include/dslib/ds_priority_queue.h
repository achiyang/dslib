#ifndef DSLIB_DS_PRIORITY_QUEUE_H
#define DSLIB_DS_PRIORITY_QUEUE_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_priority_queue ds_priority_queue;

/* create / destroy */
ds_priority_queue *ds_priority_queue_create(const ds_type *type);
void ds_priority_queue_destroy(ds_priority_queue *pq);

/* properties */
size_t ds_priority_queue_size(const ds_priority_queue *pq);
int ds_priority_queue_empty(const ds_priority_queue *pq);
const ds_type *ds_priority_queue_type(const ds_priority_queue *pq);

/* element access */
ds_status ds_priority_queue_top(const ds_priority_queue *pq, void *out);

/* modifiers */
ds_status ds_priority_queue_push(ds_priority_queue *pq, const void *elem);
ds_status ds_priority_queue_pop(ds_priority_queue *pq);
ds_status ds_priority_queue_clear(ds_priority_queue *pq);
ds_status ds_priority_queue_reserve(ds_priority_queue *pq, size_t new_capacity);

#endif /* DSLIB_DS_PRIORITY_QUEUE_H */