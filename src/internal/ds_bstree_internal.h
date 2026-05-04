#ifndef DSLIB_DS_BSTREE_INTERNAL_H
#define DSLIB_DS_BSTREE_INTERNAL_H

#include <stddef.h>

#include "dslib/ds_type.h"

struct ds_bstree_node {
    struct ds_bstree_node *parent;
    struct ds_bstree_node *left;
    struct ds_bstree_node *right;
    unsigned char data[];
};

struct ds_bstree {
    const ds_type *type;
    size_t size;
    struct ds_bstree_node *root;
};

static inline void *ds_bstree_node_data(struct ds_bstree_node *node) {
    return (void *)node->data;
}

static inline const void *ds_bstree_node_cdata(const struct ds_bstree_node *node) {
    return (const void *)node->data;
}

#endif /* DSLIB_DS_BSTREE_INTERNAL_H */