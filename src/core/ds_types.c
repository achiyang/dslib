#include "dslib/ds_types.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ==================== char ==================== */

static int ds_char_compare(const void *a, const void *b) {
    char lhs = *(const char *)a;
    char rhs = *(const char *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_char_hash(const void *obj) {
    unsigned char value = *(const unsigned char *)obj;
    return (size_t)value;
}

static void ds_char_print(const void *obj, FILE *out) {
    fprintf(out, "'%c'", *(const char *)obj);
}

const ds_type DS_CHAR = {
    .size = sizeof(char),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_char_compare,
    .hash = ds_char_hash,
    .print = ds_char_print
};

/* ==================== int ==================== */

static int ds_int_compare(const void *a, const void *b) {
    int lhs = *(const int *)a;
    int rhs = *(const int *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_int_hash(const void *obj) {
    return (size_t)(*(const int *)obj);
}

static void ds_int_print(const void *obj, FILE *out) {
    fprintf(out, "%d", *(const int *)obj);
}

const ds_type DS_INT = {
    .size = sizeof(int),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_int_compare,
    .hash = ds_int_hash,
    .print = ds_int_print
};

/* ==================== long ==================== */

static int ds_long_compare(const void *a, const void *b) {
    long lhs = *(const long *)a;
    long rhs = *(const long *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_long_hash(const void *obj) {
    return (size_t)(*(const long *)obj);
}

static void ds_long_print(const void *obj, FILE *out) {
    fprintf(out, "%ld", *(const long *)obj);
}

const ds_type DS_LONG = {
    .size = sizeof(long),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_long_compare,
    .hash = ds_long_hash,
    .print = ds_long_print
};

/* ==================== float ==================== */

static int ds_float_compare(const void *a, const void *b) {
    float lhs = *(const float *)a;
    float rhs = *(const float *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_float_hash(const void *obj) {
    return (size_t)(*(const float *)obj);
}

static void ds_float_print(const void *obj, FILE *out) {
    fprintf(out, "%f", *(const float *)obj);
}

const ds_type DS_FLOAT = {
    .size = sizeof(float),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_float_compare,
    .hash = ds_float_hash,
    .print = ds_float_print
};

/* ==================== double ==================== */

static int ds_double_compare(const void *a, const void *b) {
    double lhs = *(const double *)a;
    double rhs = *(const double *)b;

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_double_hash(const void *obj) {
    return (size_t)(*(const double *)obj);
}

static void ds_double_print(const void *obj, FILE *out) {
    fprintf(out, "%lf", *(const double *)obj);
}

const ds_type DS_DOUBLE = {
    .size = sizeof(double),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_double_compare,
    .hash = ds_double_hash,
    .print = ds_double_print
};

/* ==================== pointer ==================== */

static int ds_ptr_compare(const void *a, const void *b) {
    uintptr_t lhs = (uintptr_t)(*(void * const *)a);
    uintptr_t rhs = (uintptr_t)(*(void * const *)b);

    return (lhs > rhs) - (lhs < rhs);
}

static size_t ds_ptr_hash(const void *obj) {
    uintptr_t value = (uintptr_t)(*(void * const *)obj);
    return (size_t)value;
}

static void ds_ptr_print(const void *obj, FILE *out) {
    fprintf(out, "%p", *(void * const *)obj);
}

const ds_type DS_PTR = {
    .size = sizeof(void *),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_ptr_compare,
    .hash = ds_ptr_hash,
    .print = ds_ptr_print
};

/* ==================== cstring view ==================== */

static int ds_cstring_view_compare(const void *a, const void *b) {
    const char *lhs = *(const char * const *)a;
    const char *rhs = *(const char * const *)b;

    if (!lhs && !rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;
    return strcmp(lhs, rhs);
}

static size_t ds_cstring_view_hash(const void *obj) {
    const unsigned char *s = (const unsigned char *)(*(const char * const *)obj);
    size_t hash = 1469598103934665603ull;

    if (!s) {
        return 0;
    }

    while (*s) {
        hash ^= (size_t)(*s++);
        hash *= 1099511628211ull;
    }

    return hash;
}

static void ds_cstring_view_print(const void *obj, FILE *out) {
    const char *s = *(const char * const *)obj;
    fprintf(out, "\"%s\"", s ? s : "NULL");
}

const ds_type DS_CSTRING_VIEW = {
    .size = sizeof(char *),
    .copy = NULL,
    .destroy = NULL,
    .compare = ds_cstring_view_compare,
    .hash = ds_cstring_view_hash,
    .print = ds_cstring_view_print
};