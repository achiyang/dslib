#include <assert.h>

#include "dslib/ds_status.h"
#include "dslib/ds_type.h"
#include "dslib/ds_types.h"
#include "dslib/ds_deque.h"

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
    ds_deque *dq = ds_deque_create(&DS_INT);

    assert(dq != NULL);
    assert(ds_deque_size(dq) == 0);
    assert(ds_deque_capacity(dq) == 0);
    assert(ds_deque_empty(dq) == 1);
    assert(ds_deque_type(dq) == &DS_INT);

    ds_deque_destroy(dq);
}

static void test_push_back_and_peek(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);
    assert(ds_deque_push_back(dq, &c) == DS_OK);

    assert(ds_deque_size(dq) == 3);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 10);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 30);

    ds_deque_destroy(dq);
}

static void test_push_front_and_peek(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_push_front(dq, &a) == DS_OK);
    assert(ds_deque_push_front(dq, &b) == DS_OK);
    assert(ds_deque_push_front(dq, &c) == DS_OK);

    assert(ds_deque_size(dq) == 3);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 30);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 10);

    ds_deque_destroy(dq);
}

static void test_mixed_push_order(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int v0 = 0, v1 = 1, v2 = 2, v3 = 3;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_push_back(dq, &v2) == DS_OK);
    assert(ds_deque_push_front(dq, &v1) == DS_OK);
    assert(ds_deque_push_back(dq, &v3) == DS_OK);
    assert(ds_deque_push_front(dq, &v0) == DS_OK);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 0);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 3);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 1);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 2);

    ds_deque_destroy(dq);
}

static void test_pop_front(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_pop_front(dq) == DS_ERR);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);
    assert(ds_deque_push_back(dq, &c) == DS_OK);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_size(dq) == 2);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 20);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_pop_front(dq) == DS_ERR);

    ds_deque_destroy(dq);
}

static void test_pop_back(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_pop_back(dq) == DS_ERR);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);
    assert(ds_deque_push_back(dq, &c) == DS_OK);

    assert(ds_deque_pop_back(dq) == DS_OK);
    assert(ds_deque_size(dq) == 2);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 20);

    assert(ds_deque_pop_back(dq) == DS_OK);
    assert(ds_deque_pop_back(dq) == DS_OK);
    assert(ds_deque_pop_back(dq) == DS_ERR);

    ds_deque_destroy(dq);
}

static void test_wraparound_order(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;
    int out = 0;

    assert(dq != NULL);

    assert(ds_deque_reserve(dq, 4) == DS_OK);

    assert(ds_deque_push_back(dq, &v1) == DS_OK);
    assert(ds_deque_push_back(dq, &v2) == DS_OK);
    assert(ds_deque_push_back(dq, &v3) == DS_OK);
    assert(ds_deque_push_back(dq, &v4) == DS_OK);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_pop_front(dq) == DS_OK);

    assert(ds_deque_push_back(dq, &v5) == DS_OK);
    assert(ds_deque_push_back(dq, &v6) == DS_OK);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 3);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 6);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 4);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 5);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 6);

    ds_deque_destroy(dq);
}

static void test_reserve_preserves_order(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6;
    int out = 0;
    size_t old_cap;

    assert(dq != NULL);

    assert(ds_deque_reserve(dq, 4) == DS_OK);
    old_cap = ds_deque_capacity(dq);
    assert(old_cap >= 4);

    assert(ds_deque_push_back(dq, &v1) == DS_OK);
    assert(ds_deque_push_back(dq, &v2) == DS_OK);
    assert(ds_deque_push_back(dq, &v3) == DS_OK);
    assert(ds_deque_push_back(dq, &v4) == DS_OK);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_pop_front(dq) == DS_OK);

    assert(ds_deque_push_back(dq, &v5) == DS_OK);
    assert(ds_deque_push_back(dq, &v6) == DS_OK);

    assert(ds_deque_reserve(dq, 16) == DS_OK);
    assert(ds_deque_capacity(dq) >= 16);
    assert(ds_deque_size(dq) == 4);

    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 3);

    assert(ds_deque_back(dq, &out) == DS_OK);
    assert(out == 6);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 4);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 5);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 6);

    ds_deque_destroy(dq);
}

static void test_clear_keeps_capacity_and_reusable(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int a = 1, b = 2, c = 3, d = 4;
    int out = 0;
    size_t cap_before;

    assert(dq != NULL);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);
    assert(ds_deque_push_front(dq, &c) == DS_OK);

    cap_before = ds_deque_capacity(dq);
    assert(cap_before >= 3);

    assert(ds_deque_clear(dq) == DS_OK);
    assert(ds_deque_size(dq) == 0);
    assert(ds_deque_empty(dq) == 1);
    assert(ds_deque_capacity(dq) == cap_before);

    assert(ds_deque_push_back(dq, &d) == DS_OK);
    assert(ds_deque_front(dq, &out) == DS_OK);
    assert(out == 4);

    ds_deque_destroy(dq);
}

static void test_destroy_lifecycle(void)
{
    ds_deque *dq;
    tracked_int a = { .value = 1 };
    tracked_int b = { .value = 2 };
    tracked_int c = { .value = 3 };

    g_destroy_count = 0;

    dq = ds_deque_create(&TRACKED_INT_TYPE);
    assert(dq != NULL);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);
    assert(ds_deque_push_front(dq, &c) == DS_OK);

    assert(g_destroy_count == 0);

    assert(ds_deque_pop_front(dq) == DS_OK);
    assert(g_destroy_count == 1);

    assert(ds_deque_pop_back(dq) == DS_OK);
    assert(g_destroy_count == 2);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_front(dq, &b) == DS_OK);

    assert(ds_deque_clear(dq) == DS_OK);
    assert(g_destroy_count == 5);

    assert(ds_deque_push_back(dq, &a) == DS_OK);
    assert(ds_deque_push_back(dq, &b) == DS_OK);

    ds_deque_destroy(dq);
    assert(g_destroy_count == 7);
}

static void test_error_cases(void)
{
    ds_deque *dq = ds_deque_create(&DS_INT);
    int x = 10;
    int out = 0;

    assert(ds_deque_create(NULL) == NULL);

    assert(dq != NULL);

    assert(ds_deque_front(dq, &out) == DS_ERR);
    assert(ds_deque_front(dq, NULL) == DS_ERR);

    assert(ds_deque_back(dq, &out) == DS_ERR);
    assert(ds_deque_back(dq, NULL) == DS_ERR);

    assert(ds_deque_push_front(NULL, &x) == DS_ERR);
    assert(ds_deque_push_front(dq, NULL) == DS_ERR);

    assert(ds_deque_push_back(NULL, &x) == DS_ERR);
    assert(ds_deque_push_back(dq, NULL) == DS_ERR);

    assert(ds_deque_pop_front(NULL) == DS_ERR);
    assert(ds_deque_pop_front(dq) == DS_ERR);

    assert(ds_deque_pop_back(NULL) == DS_ERR);
    assert(ds_deque_pop_back(dq) == DS_ERR);

    assert(ds_deque_clear(NULL) == DS_ERR);
    assert(ds_deque_reserve(NULL, 8) == DS_ERR);

    ds_deque_destroy(dq);
}

int main(void)
{
    test_create_initial_state();
    test_push_back_and_peek();
    test_push_front_and_peek();
    test_mixed_push_order();
    test_pop_front();
    test_pop_back();
    test_wraparound_order();
    test_reserve_preserves_order();
    test_clear_keeps_capacity_and_reusable();
    test_destroy_lifecycle();
    test_error_cases();

    return 0;
}