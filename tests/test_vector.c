#include <assert.h>

#include "dslib/ds_status.h"
#include "dslib/ds_type.h"
#include "dslib/ds_types.h"
#include "dslib/ds_vector.h"

typedef struct tracked_int {
    int value;
} tracked_int;

static int g_destroy_count = 0;

static void tracked_int_destroy(void *obj)
{
    (void)obj;
    g_destroy_count++;
}

static const ds_type TRACKED_INT_TYPE = {
    .size = sizeof(tracked_int),
    .copy = NULL,
    .destroy = tracked_int_destroy,
    .compare = NULL,
    .hash = NULL,
    .print = NULL,
};

static void test_create_initial_state(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);

    assert(vec != NULL);
    assert(ds_vector_size(vec) == 0);
    assert(ds_vector_capacity(vec) == 0);
    assert(ds_vector_empty(vec) == 1);
    assert(ds_vector_type(vec) == &DS_INT);

    ds_vector_destroy(vec);
}

static void test_push_back_and_get(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(vec != NULL);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);
    assert(ds_vector_push_back(vec, &c) == DS_OK);

    assert(ds_vector_size(vec) == 3);
    assert(ds_vector_empty(vec) == 0);

    assert(ds_vector_get(vec, 0, &out) == DS_OK);
    assert(out == 10);

    assert(ds_vector_get(vec, 1, &out) == DS_OK);
    assert(out == 20);

    assert(ds_vector_get(vec, 2, &out) == DS_OK);
    assert(out == 30);

    ds_vector_destroy(vec);
}

static void test_back(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int a = 1, b = 2, out = 0;

    assert(vec != NULL);

    assert(ds_vector_back(vec, &out) == DS_ERR);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);

    assert(ds_vector_back(vec, &out) == DS_OK);
    assert(out == 2);

    ds_vector_destroy(vec);
}

static void test_reserve(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int x = 42;
    size_t cap;

    assert(vec != NULL);
    assert(ds_vector_capacity(vec) == 0);

    assert(ds_vector_reserve(vec, 16) == DS_OK);
    cap = ds_vector_capacity(vec);

    assert(cap >= 16);
    assert(ds_vector_size(vec) == 0);

    assert(ds_vector_push_back(vec, &x) == DS_OK);
    assert(ds_vector_size(vec) == 1);
    assert(ds_vector_capacity(vec) == cap);

    ds_vector_destroy(vec);
}

static void test_pop_back(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(vec != NULL);

    assert(ds_vector_pop_back(vec) == DS_ERR);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);
    assert(ds_vector_push_back(vec, &c) == DS_OK);

    assert(ds_vector_size(vec) == 3);

    assert(ds_vector_pop_back(vec) == DS_OK);
    assert(ds_vector_size(vec) == 2);

    assert(ds_vector_back(vec, &out) == DS_OK);
    assert(out == 20);

    assert(ds_vector_pop_back(vec) == DS_OK);
    assert(ds_vector_pop_back(vec) == DS_OK);
    assert(ds_vector_pop_back(vec) == DS_ERR);

    ds_vector_destroy(vec);
}

static void test_clear_keeps_capacity(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int a = 1, b = 2, c = 3;
    size_t cap_before;

    assert(vec != NULL);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);
    assert(ds_vector_push_back(vec, &c) == DS_OK);

    cap_before = ds_vector_capacity(vec);
    assert(cap_before >= 3);

    assert(ds_vector_clear(vec) == DS_OK);
    assert(ds_vector_size(vec) == 0);
    assert(ds_vector_empty(vec) == 1);
    assert(ds_vector_capacity(vec) == cap_before);

    ds_vector_destroy(vec);
}

static void test_set_updates_value(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int a = 1, b = 2, c = 3;
    int repl = 99;
    int out = 0;

    assert(vec != NULL);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);
    assert(ds_vector_push_back(vec, &c) == DS_OK);

    assert(ds_vector_set(vec, 1, &repl) == DS_OK);

    assert(ds_vector_get(vec, 0, &out) == DS_OK);
    assert(out == 1);

    assert(ds_vector_get(vec, 1, &out) == DS_OK);
    assert(out == 99);

    assert(ds_vector_get(vec, 2, &out) == DS_OK);
    assert(out == 3);

    ds_vector_destroy(vec);
}

static void test_destroy_lifecycle(void)
{
    ds_vector *vec;
    tracked_int a = { .value = 1 };
    tracked_int b = { .value = 2 };
    tracked_int c = { .value = 3 };
    tracked_int repl = { .value = 99 };

    g_destroy_count = 0;

    vec = ds_vector_create(&TRACKED_INT_TYPE);
    assert(vec != NULL);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);
    assert(ds_vector_push_back(vec, &c) == DS_OK);

    assert(g_destroy_count == 0);

    assert(ds_vector_set(vec, 1, &repl) == DS_OK);
    assert(g_destroy_count == 1);

    assert(ds_vector_pop_back(vec) == DS_OK);
    assert(g_destroy_count == 2);

    assert(ds_vector_clear(vec) == DS_OK);
    assert(g_destroy_count == 4);

    assert(ds_vector_push_back(vec, &a) == DS_OK);
    assert(ds_vector_push_back(vec, &b) == DS_OK);

    ds_vector_destroy(vec);
    assert(g_destroy_count == 6);
}

static void test_error_cases(void)
{
    ds_vector *vec = ds_vector_create(&DS_INT);
    int x = 10;
    int out = 0;

    assert(ds_vector_create(NULL) == NULL);

    assert(vec != NULL);

    assert(ds_vector_get(vec, 0, &out) == DS_ERR);
    assert(ds_vector_get(vec, 0, NULL) == DS_ERR);

    assert(ds_vector_set(vec, 0, &x) == DS_ERR);
    assert(ds_vector_set(vec, 0, NULL) == DS_ERR);

    assert(ds_vector_back(vec, &out) == DS_ERR);
    assert(ds_vector_back(vec, NULL) == DS_ERR);

    assert(ds_vector_push_back(NULL, &x) == DS_ERR);
    assert(ds_vector_push_back(vec, NULL) == DS_ERR);

    assert(ds_vector_pop_back(NULL) == DS_ERR);
    assert(ds_vector_clear(NULL) == DS_ERR);
    assert(ds_vector_reserve(NULL, 8) == DS_ERR);

    ds_vector_destroy(vec);
}

int main(void)
{
    test_create_initial_state();
    test_push_back_and_get();
    test_back();
    test_reserve();
    test_pop_back();
    test_clear_keeps_capacity();
    test_set_updates_value();
    test_destroy_lifecycle();
    test_error_cases();

    return 0;
}