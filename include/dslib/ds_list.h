#ifndef DSLIB_DS_LIST_H
#define DSLIB_DS_LIST_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_list ds_list;

/* create / destroy */
ds_list *ds_list_create(const ds_type *type);
void ds_list_destroy(ds_list *list);

/* properties */
size_t ds_list_size(const ds_list *list);
int ds_list_empty(const ds_list *list);
const ds_type *ds_list_type(const ds_list *list);

/* element access */
ds_status ds_list_front(const ds_list *list, void *out);
ds_status ds_list_back(const ds_list *list, void *out);

/* modifiers */
ds_status ds_list_push_front(ds_list *list, const void *elem);
ds_status ds_list_push_back(ds_list *list, const void *elem);
ds_status ds_list_pop_front(ds_list *list);
ds_status ds_list_pop_back(ds_list *list);
ds_status ds_list_clear(ds_list *list);

#endif /* DSLIB_DS_LIST_H */