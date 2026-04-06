#ifndef DSLIB_DS_STACK_H
#define DSLIB_DS_STACK_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_stack ds_stack;

/* create / destroy */
ds_stack *ds_stack_create(const ds_type *type);
void ds_stack_destroy(ds_stack *stack);

/* properties */
size_t ds_stack_size(const ds_stack *stack);
int ds_stack_empty(const ds_stack *stack);
const ds_type *ds_stack_type(const ds_stack *stack);

/* element access */
ds_status ds_stack_top(const ds_stack *stack, void *out);

/* modifiers */
ds_status ds_stack_push(ds_stack *stack, const void *elem);
ds_status ds_stack_pop(ds_stack *stack);
ds_status ds_stack_clear(ds_stack *stack);

#endif