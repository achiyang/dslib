#ifndef DSLIB_DS_LIST_INTERNAL_H
#define DSLIB_DS_LIST_INTERNAL_H

#include <stddef.h>

#include "dslib/ds_type.h"

struct ds_list_node {
    struct ds_list_node *prev;
    struct ds_list_node *next;
    unsigned char data[];
};

static inline void *ds_list_node_data(struct ds_list_node *node) {
    return (void *)node->data;
}

static inline const void *ds_list_node_cdata(const struct ds_list_node *node) {
    return (const void *)node->data;
}

#endif /* DSLIB_DS_LIST_INTERNAL_H */