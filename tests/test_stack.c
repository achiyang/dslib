#include <assert.h>

#include "dslib/ds_stack.h"
#include "dslib/ds_status.h"
#include "dslib/ds_type.h"
#include "dslib/ds_types.h"

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
    ds_stack *stack = ds_stack_create(&DS_INT);

    assert(stack != NULL);
    assert(ds_stack_size(stack) == 0);
    assert(ds_stack_empty(stack) == 1);
    assert(ds_stack_type(stack) == &DS_INT);

    ds_stack_destroy(stack);
}

static void test_push_and_top(void)
{
    ds_stack *stack = ds_stack_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(stack != NULL);

    assert(ds_stack_push(stack, &a) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 10);

    assert(ds_stack_push(stack, &b) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 20);

    assert(ds_stack_push(stack, &c) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 30);

    assert(ds_stack_size(stack) == 3);

    ds_stack_destroy(stack);
}

static void test_pop_lifo_order(void)
{
    ds_stack *stack = ds_stack_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(stack != NULL);

    assert(ds_stack_pop(stack) == DS_ERR);

    assert(ds_stack_push(stack, &a) == DS_OK);
    assert(ds_stack_push(stack, &b) == DS_OK);
    assert(ds_stack_push(stack, &c) == DS_OK);

    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 30);

    assert(ds_stack_pop(stack) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 20);

    assert(ds_stack_pop(stack) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 10);

    assert(ds_stack_pop(stack) == DS_OK);
    assert(ds_stack_pop(stack) == DS_ERR);

    ds_stack_destroy(stack);
}

static void test_clear_and_reuse(void)
{
    ds_stack *stack = ds_stack_create(&DS_INT);
    int a = 1, b = 2, c = 3;
    int out = 0;

    assert(stack != NULL);

    assert(ds_stack_push(stack, &a) == DS_OK);
    assert(ds_stack_push(stack, &b) == DS_OK);

    assert(ds_stack_clear(stack) == DS_OK);
    assert(ds_stack_size(stack) == 0);
    assert(ds_stack_empty(stack) == 1);

    assert(ds_stack_push(stack, &c) == DS_OK);
    assert(ds_stack_top(stack, &out) == DS_OK);
    assert(out == 3);

    ds_stack_destroy(stack);
}

static void test_destroy_lifecycle(void)
{
    ds_stack *stack;
    tracked_int a = { .value = 1 };
    tracked_int b = { .value = 2 };
    tracked_int c = { .value = 3 };

    g_destroy_count = 0;

    stack = ds_stack_create(&TRACKED_INT_TYPE);
    assert(stack != NULL);

    assert(ds_stack_push(stack, &a) == DS_OK);
    assert(ds_stack_push(stack, &b) == DS_OK);
    assert(ds_stack_push(stack, &c) == DS_OK);

    assert(g_destroy_count == 0);

    assert(ds_stack_pop(stack) == DS_OK);
    assert(g_destroy_count == 1);

    assert(ds_stack_clear(stack) == DS_OK);
    assert(g_destroy_count == 3);

    assert(ds_stack_push(stack, &a) == DS_OK);
    assert(ds_stack_push(stack, &b) == DS_OK);

    ds_stack_destroy(stack);
    assert(g_destroy_count == 5);
}

static void test_error_cases(void)
{
    ds_stack *stack = ds_stack_create(&DS_INT);
    int x = 10;
    int out = 0;

    assert(ds_stack_create(NULL) == NULL);

    assert(stack != NULL);

    assert(ds_stack_top(stack, &out) == DS_ERR);
    assert(ds_stack_top(stack, NULL) == DS_ERR);

    assert(ds_stack_push(NULL, &x) == DS_ERR);
    assert(ds_stack_push(stack, NULL) == DS_ERR);

    assert(ds_stack_pop(NULL) == DS_ERR);
    assert(ds_stack_clear(NULL) == DS_ERR);

    ds_stack_destroy(stack);
}

int main(void)
{
    test_create_initial_state();
    test_push_and_top();
    test_pop_lifo_order();
    test_clear_and_reuse();
    test_destroy_lifecycle();
    test_error_cases();

    return 0;
}