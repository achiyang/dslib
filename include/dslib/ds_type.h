#ifndef DSLIB_DS_TYPE_H
#define DSLIB_DS_TYPE_H

#include <stddef.h>
#include <stdio.h>

typedef void   (*ds_copy_fn)(void *dest, const void *src);
typedef void   (*ds_destroy_fn)(void *obj);
typedef int    (*ds_compare_fn)(const void *a, const void *b);
typedef size_t (*ds_hash_fn)(const void *obj);
typedef void   (*ds_print_fn)(const void *obj, FILE *out);

typedef struct ds_type ds_type;

struct ds_type {
    size_t size;
    ds_copy_fn copy;
    ds_destroy_fn destroy;
    ds_compare_fn compare;
    ds_hash_fn hash;
    ds_print_fn print;
};

#endif /* DSLIB_DS_TYPE_H */