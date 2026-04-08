#include "dslib/ds_stack.h"
#include "dslib/ds_vector.h"

#include <stdlib.h>

struct ds_stack {
    ds_vector *impl;
};

ds_stack *ds_stack_create(const ds_type *type) {
    ds_stack *stack;

    stack = (ds_stack *)malloc(sizeof(ds_stack));
    if (!stack) {
        return NULL;
    }

    stack->impl = ds_vector_create(type);
    if (!stack->impl) {
        free(stack);
        return NULL;
    }

    return stack;
}

void ds_stack_destroy(ds_stack *stack) {
    if (!stack) {
        return;
    }

    ds_vector_destroy(stack->impl);
    free(stack);
}

size_t ds_stack_size(const ds_stack *stack) {
    return stack ? ds_vector_size(stack->impl) : 0;
}

int ds_stack_empty(const ds_stack *stack) {
    return stack ? ds_vector_empty(stack->impl) : 1;
}

const ds_type *ds_stack_type(const ds_stack *stack) {
    return stack ? ds_vector_type(stack->impl) : NULL;
}

ds_status ds_stack_top(const ds_stack *stack, void *out) {
    if (!stack) {
        return DS_ERR;
    }

    return ds_vector_back(stack->impl, out);
}

ds_status ds_stack_push(ds_stack *stack, const void *elem) {
    if (!stack) {
        return DS_ERR;
    }

    return ds_vector_push_back(stack->impl, elem);
}

ds_status ds_stack_pop(ds_stack *stack) {
    if (!stack) {
        return DS_ERR;
    }

    return ds_vector_pop_back(stack->impl);
}

ds_status ds_stack_clear(ds_stack *stack) {
    if (!stack) {
        return DS_ERR;
    }

    return ds_vector_clear(stack->impl);
}