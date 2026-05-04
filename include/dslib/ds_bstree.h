#ifndef DSLIB_DS_BSTREE_H
#define DSLIB_DS_BSTREE_H

#include <stddef.h>

#include "ds_status.h"
#include "ds_type.h"

typedef struct ds_bstree ds_bstree;

/* create / destroy */
ds_bstree *ds_bstree_create(const ds_type *type);
void ds_bstree_destroy(ds_bstree *tree);

/* properties */
size_t ds_bstree_size(const ds_bstree *tree);
int ds_bstree_empty(const ds_bstree *tree);
const ds_type *ds_bstree_type(const ds_bstree *tree);

/* lookup */
int ds_bstree_contains(const ds_bstree *tree, const void *key);
ds_status ds_bstree_min(const ds_bstree *tree, void *out);
ds_status ds_bstree_max(const ds_bstree *tree, void *out);

/* modifiers */
ds_status ds_bstree_insert(ds_bstree *tree, const void *elem);
ds_status ds_bstree_remove(ds_bstree *tree, const void *key);
ds_status ds_bstree_clear(ds_bstree *tree);

#endif /* DSLIB_DS_BSTREE_H */