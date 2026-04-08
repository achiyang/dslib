#include "dslib/ds_list.h"
#include "ds_list_internal.h"

#include <stdlib.h>
#include <string.h>

static struct ds_list_node *ds_list_node_create(const ds_list *list, const void *elem) {
    struct ds_list_node *node;

    node = (struct ds_list_node *)malloc(
        sizeof(struct ds_list_node) + list->type->size
    );
    if (!node) {
        return NULL;
    }

    node->prev = NULL;
    node->next = NULL;

    if (list->type->copy) {
        list->type->copy(ds_list_node_data(node), elem);
    } else {
        memcpy(ds_list_node_data(node), elem, list->type->size);
    }

    return node;
}

static void ds_list_node_destroy(const ds_list *list, struct ds_list_node *node) {
    if (list->type->destroy) {
        list->type->destroy(ds_list_node_data(node));
    }

    free(node);
}

static void ds_list_destroy_all(ds_list *list) {
    struct ds_list_node *cur;
    struct ds_list_node *next;

    cur = list->head;
    while (cur) {
        next = cur->next;
        ds_list_node_destroy(list, cur);
        cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

ds_list *ds_list_create(const ds_type *type) {
    ds_list *list;

    if (!type || type->size == 0) {
        return NULL;
    }

    list = (ds_list *)malloc(sizeof(ds_list));
    if (!list) {
        return NULL;
    }

    list->type = type;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void ds_list_destroy(ds_list *list) {
    if (!list) {
        return;
    }

    ds_list_destroy_all(list);
    free(list);
}

size_t ds_list_size(const ds_list *list) {
    return list ? list->size : 0;
}

int ds_list_empty(const ds_list *list) {
    return !list || list->size == 0;
}

const ds_type *ds_list_type(const ds_list *list) {
    return list ? list->type : NULL;
}

ds_status ds_list_front(const ds_list *list, void *out) {
    if (!list || !out || !list->head) {
        return DS_ERR;
    }

    memcpy(out, ds_list_node_cdata(list->head), list->type->size);
    return DS_OK;
}

ds_status ds_list_back(const ds_list *list, void *out) {
    if (!list || !out || !list->tail) {
        return DS_ERR;
    }

    memcpy(out, ds_list_node_cdata(list->tail), list->type->size);
    return DS_OK;
}

ds_status ds_list_push_front(ds_list *list, const void *elem) {
    struct ds_list_node *node;

    if (!list || !elem) {
        return DS_ERR;
    }

    node = ds_list_node_create(list, elem);
    if (!node) {
        return DS_ERR;
    }

    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }

    list->head = node;
    list->size++;

    return DS_OK;
}

ds_status ds_list_push_back(ds_list *list, const void *elem) {
    struct ds_list_node *node;

    if (!list || !elem) {
        return DS_ERR;
    }

    node = ds_list_node_create(list, elem);
    if (!node) {
        return DS_ERR;
    }

    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    list->size++;

    return DS_OK;
}

ds_status ds_list_pop_front(ds_list *list) {
    struct ds_list_node *node;

    if (!list || !list->head) {
        return DS_ERR;
    }

    node = list->head;
    list->head = node->next;

    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    list->size--;
    ds_list_node_destroy(list, node);

    return DS_OK;
}

ds_status ds_list_pop_back(ds_list *list) {
    struct ds_list_node *node;

    if (!list || !list->tail) {
        return DS_ERR;
    }

    node = list->tail;
    list->tail = node->prev;

    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    list->size--;
    ds_list_node_destroy(list, node);

    return DS_OK;
}

ds_status ds_list_clear(ds_list *list) {
    if (!list) {
        return DS_ERR;
    }

    ds_list_destroy_all(list);
    return DS_OK;
}