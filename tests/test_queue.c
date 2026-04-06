#include <assert.h>

#include "dslib/ds_queue.h"
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
    ds_queue *queue = ds_queue_create(&DS_INT);

    assert(queue != NULL);
    assert(ds_queue_size(queue) == 0);
    assert(ds_queue_empty(queue) == 1);
    assert(ds_queue_type(queue) == &DS_INT);

    ds_queue_destroy(queue);
}

static void test_enqueue_and_front(void)
{
    ds_queue *queue = ds_queue_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(queue != NULL);

    assert(ds_queue_enqueue(queue, &a) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 10);

    assert(ds_queue_enqueue(queue, &b) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 10);

    assert(ds_queue_enqueue(queue, &c) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 10);

    assert(ds_queue_size(queue) == 3);

    ds_queue_destroy(queue);
}

static void test_dequeue_fifo_order(void)
{
    ds_queue *queue = ds_queue_create(&DS_INT);
    int a = 10, b = 20, c = 30;
    int out = 0;

    assert(queue != NULL);

    assert(ds_queue_dequeue(queue) == DS_ERR);

    assert(ds_queue_enqueue(queue, &a) == DS_OK);
    assert(ds_queue_enqueue(queue, &b) == DS_OK);
    assert(ds_queue_enqueue(queue, &c) == DS_OK);

    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 10);

    assert(ds_queue_dequeue(queue) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 20);

    assert(ds_queue_dequeue(queue) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 30);

    assert(ds_queue_dequeue(queue) == DS_OK);
    assert(ds_queue_dequeue(queue) == DS_ERR);

    ds_queue_destroy(queue);
}

static void test_clear_and_reuse(void)
{
    ds_queue *queue = ds_queue_create(&DS_INT);
    int a = 1, b = 2, c = 3;
    int out = 0;

    assert(queue != NULL);

    assert(ds_queue_enqueue(queue, &a) == DS_OK);
    assert(ds_queue_enqueue(queue, &b) == DS_OK);

    assert(ds_queue_clear(queue) == DS_OK);
    assert(ds_queue_size(queue) == 0);
    assert(ds_queue_empty(queue) == 1);

    assert(ds_queue_enqueue(queue, &c) == DS_OK);
    assert(ds_queue_front(queue, &out) == DS_OK);
    assert(out == 3);

    ds_queue_destroy(queue);
}

static void test_destroy_lifecycle(void)
{
    ds_queue *queue;
    tracked_int a = { .value = 1 };
    tracked_int b = { .value = 2 };
    tracked_int c = { .value = 3 };

    g_destroy_count = 0;

    queue = ds_queue_create(&TRACKED_INT_TYPE);
    assert(queue != NULL);

    assert(ds_queue_enqueue(queue, &a) == DS_OK);
    assert(ds_queue_enqueue(queue, &b) == DS_OK);
    assert(ds_queue_enqueue(queue, &c) == DS_OK);

    assert(g_destroy_count == 0);

    assert(ds_queue_dequeue(queue) == DS_OK);
    assert(g_destroy_count == 1);

    assert(ds_queue_clear(queue) == DS_OK);
    assert(g_destroy_count == 3);

    assert(ds_queue_enqueue(queue, &a) == DS_OK);
    assert(ds_queue_enqueue(queue, &b) == DS_OK);

    ds_queue_destroy(queue);
    assert(g_destroy_count == 5);
}

static void test_error_cases(void)
{
    ds_queue *queue = ds_queue_create(&DS_INT);
    int x = 10;
    int out = 0;

    assert(ds_queue_create(NULL) == NULL);

    assert(queue != NULL);

    assert(ds_queue_front(queue, &out) == DS_ERR);
    assert(ds_queue_front(queue, NULL) == DS_ERR);

    assert(ds_queue_enqueue(NULL, &x) == DS_ERR);
    assert(ds_queue_enqueue(queue, NULL) == DS_ERR);

    assert(ds_queue_dequeue(NULL) == DS_ERR);
    assert(ds_queue_clear(NULL) == DS_ERR);

    ds_queue_destroy(queue);
}

int main(void)
{
    test_create_initial_state();
    test_enqueue_and_front();
    test_dequeue_fifo_order();
    test_clear_and_reuse();
    test_destroy_lifecycle();
    test_error_cases();

    return 0;
}