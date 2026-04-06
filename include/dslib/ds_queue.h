#ifndef DSLIB_DS_QUEUE_H
#define DSLIB_DS_QUEUE_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_queue ds_queue;

/* create / destroy */
ds_queue *ds_queue_create(const ds_type *type);
void ds_queue_destroy(ds_queue *queue);

/* properties */
size_t ds_queue_size(const ds_queue *queue);
int ds_queue_empty(const ds_queue *queue);
const ds_type *ds_queue_type(const ds_queue *queue);

/* element access */
ds_status ds_queue_front(const ds_queue *queue, void *out);

/* modifiers */
ds_status ds_queue_enqueue(ds_queue *queue, const void *elem);
ds_status ds_queue_dequeue(ds_queue *queue);
ds_status ds_queue_clear(ds_queue *queue);

#endif