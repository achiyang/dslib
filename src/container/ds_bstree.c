#include "dslib/ds_bstree.h"
#include "ds_bstree_internal.h"

#include <stdlib.h>
#include <string.h>

static struct ds_bstree_node *ds_bstree_node_create(const ds_bstree *tree, const void *elem) {
    struct ds_bstree_node *node;

    node = (struct ds_bstree_node *)malloc(
        sizeof(struct ds_bstree_node) + tree->type->size
    );
    if (!node) {
        return NULL;
    }

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    if (tree->type->copy) {
        tree->type->copy(ds_bstree_node_data(node), elem);
    }
    else {
        memcpy(ds_bstree_node_data(node), elem, tree->type->size);
    }

    return node;
}

static void ds_bstree_node_destroy(ds_bstree *tree, struct ds_bstree_node *node) {
    if (tree->type->destroy) {
        tree->type->destroy(ds_bstree_node_data(node));
    }

    free(node);
}

static void ds_bstree_destroy_subtree(ds_bstree *tree, struct ds_bstree_node *node) {
    if (!node) return;

    ds_bstree_destroy_subtree(tree, node->left);
    ds_bstree_destroy_subtree(tree, node->right);
    ds_bstree_node_destroy(tree, node);
}

static struct ds_bstree_node *ds_bstree_find_node(const ds_bstree *tree, const void *key) {
    struct ds_bstree_node *cur;
    int cmp;

    cur = tree->root;
    while (cur) {
        cmp = tree->type->compare(key, ds_bstree_node_cdata(cur));

        if (cmp == 0) {
            return cur;
        }
        
        cur = (cmp < 0) ? cur->left : cur->right;
    }

    return NULL;
}

static struct ds_bstree_node *ds_bstree_min_node(struct ds_bstree_node *node) {
    if (!node) {
        return NULL;
    }

    while (node->left) {
        node = node->left;
    }

    return node;
}

static struct ds_bstree_node *ds_bstree_max_node(struct ds_bstree_node *node) {
    if (!node) {
        return NULL;
    }

    while (node->right) {
        node = node->right;
    }

    return node;
}

static void ds_bstree_transplant(
    ds_bstree *tree,
    struct ds_bstree_node *u,
    struct ds_bstree_node *v
) {
    if (!u->parent) {
        tree->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }

    if (v) {
        v->parent = u->parent;
    }
}

ds_bstree *ds_bstree_create(const ds_type *type) {
    ds_bstree *tree;

    if (!type || type->size == 0 || !type->compare) {
        return NULL;
    }

    tree = (ds_bstree *)malloc(sizeof(ds_bstree));
    if (!tree) {
        return NULL;
    }

    tree->type = type;
    tree->size = 0;
    tree->root = NULL;

    return tree;
}

void ds_bstree_destroy(ds_bstree *tree) {
    if (!tree) {
        return;
    }

    ds_bstree_destroy_subtree(tree, tree->root);
    free(tree);
}

size_t ds_bstree_size(const ds_bstree *tree) {
    return tree ? tree->size : 0;
}

int ds_bstree_empty(const ds_bstree *tree) {
    return !tree || tree->size == 0;
}

const ds_type *ds_bstree_type(const ds_bstree *tree) {
    return tree ? tree->type : NULL;
}

int ds_bstree_contains(const ds_bstree *tree, const void *key) {
    if (!tree || !key) {
        return 0;
    }

    return ds_bstree_find_node(tree, key) != NULL;
}

ds_status ds_bstree_min(const ds_bstree *tree, void *out) {
    struct ds_bstree_node *node;

    if (!tree || !out || !tree->root) {
        return DS_ERR;
    }

    node = ds_bstree_min_node(tree->root);
    memcpy(out, ds_bstree_node_cdata(node), tree->type->size);

    return DS_OK;
}

ds_status ds_bstree_max(const ds_bstree *tree, void *out) {
    struct ds_bstree_node *node;

    if (!tree || !out || !tree->root) {
        return DS_ERR;
    }

    node = ds_bstree_max_node(tree->root);
    memcpy(out, ds_bstree_node_cdata(node), tree->type->size);

    return DS_OK;
}

ds_status ds_bstree_insert(ds_bstree *tree, const void *elem) {
    struct ds_bstree_node *parent;
    struct ds_bstree_node *cur;
    struct ds_bstree_node *node;
    int cmp;

    if (!tree || !elem) {
        return DS_ERR;
    }

    parent = NULL;
    cur = tree->root;

    while (cur) {
        cmp = tree->type->compare(elem, ds_bstree_node_cdata(cur));

        if (cmp == 0) {
            return DS_ERR;
        }

        parent = cur;
        cur = (cmp < 0) ? cur->left : cur->right;
    }

    node = ds_bstree_node_create(tree, elem);
    if (!node) {
        return DS_ERR;
    }

    node->parent = parent;

    if (!parent) {
        tree->root = node;
    }
    else if (tree->type->compare(elem, ds_bstree_node_cdata(parent)) < 0) {
        parent->left = node;
    }
    else {
        parent->right = node;
    }

    tree->size++;
    return DS_OK;
}

ds_status ds_bstree_remove(ds_bstree *tree, const void *key) {
    struct ds_bstree_node *target;
    struct ds_bstree_node *succ;

    if (!tree || !key) {
        return DS_ERR;
    }

    target = ds_bstree_find_node(tree, key);
    if (!target) {
        return DS_ERR;
    }

    if (!target->left) {
        ds_bstree_transplant(tree, target, target->right);
    }
    else if (!target->right) {
        ds_bstree_transplant(tree, target, target->left);
    }
    else {
        succ = ds_bstree_min_node(target->right);

        if (succ->parent != target) {
            ds_bstree_transplant(tree, succ, succ->right);
            succ->right = target->right;
            succ->right->parent = succ;
        }

        ds_bstree_transplant(tree, target, succ);
        succ->left = target->left;
        succ->left->parent = succ;
    }

    ds_bstree_node_destroy(tree, target);
    tree->size--;

    return DS_OK;
}

ds_status ds_bstree_clear(ds_bstree *tree) {
    if (!tree) {
        return DS_ERR;
    }

    ds_bstree_destroy_subtree(tree, tree->root);
    tree->root = NULL;
    tree->size = 0;

    return DS_OK;
}