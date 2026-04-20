#ifndef DSLIB_DS_TYPE_H
#define DSLIB_DS_TYPE_H

#include <stddef.h>
#include <stdio.h>

/*
 * dslib object model (current)
 *
 * dslib container는 ds_type으로 설명되는 value-like object를 저장한다.
 *
 * - copy:
 *   src object를 dest storage에 기록하여 새로운 유효 object를 만든다.
 *   copy가 NULL이면 기본 동작은 memcpy(dest, src, size)이다.
 *
 * - destroy:
 *   저장된 object를 정리한다.
 *   destroy가 NULL이면 기본 동작은 no-op이다.
 *
 * - internal relocation:
 *   현재 dslib container 내부 동작은 raw byte relocation을 사용할 수 있다.
 *   따라서 dslib에 저장되는 object는 raw relocation-safe해야 한다.
 *
 * 현재 모델은 deep-owning type을 완전한 일반 object model로 지원하지 않으며,
 * copy는 failure를 반환하지 않는다.
 */

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