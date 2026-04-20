# dslib object model

## 개요
`dslib`는 `ds_type`으로 설명되는 object를 generic container에 저장한다.

현재 `dslib`는 **raw-relocation-safe value-like object model**을 채택한다.

## copy
`copy(dest, src)`는 `src` object를 `dest` storage에 기록하여
새로운 유효 object를 만드는 operation이다.

- `copy == NULL`이면 기본 동작은 `memcpy(dest, src, size)`이다.
- overwrite가 필요한 경우, container는 먼저 기존 object를 `destroy`한 뒤 `copy`를 호출해야 한다.
- 현재 `copy`는 failure를 반환하지 않는다.

## destroy
`destroy(obj)`는 저장된 object를 정리하는 operation이다.

- `destroy == NULL`이면 기본 동작은 no-op이다.

## raw relocation
현재 `dslib` container 내부 구현은 object의 저장 위치를 바꾸기 위해
**raw byte relocation**을 사용할 수 있다.

예:
- `vector reserve`
- `deque repack`
- `heap sift/pop`
- 앞으로의 tree rotation / payload handling
- 앞으로의 hash table rehash

따라서 `dslib`에 저장되는 object는 현재 모델에서
**raw relocation-safe**해야 한다.

## 현재 제한
현재 `dslib`는 다음을 완전한 일반 object model로 지원하지 않는다.

- failure-reporting deep copy
- user-defined move / relocate hook
- deep-owning type에 대한 완전 일반 지원

## user-defined type 작성 시 권장 사항
1. 가능하면 trivial value type으로 시작한다.
2. `destroy`가 필요한 경우 ownership semantics를 명확히 한다.
3. 현재 `dslib`에 저장하는 type은 raw relocation-safe해야 한다.
4. deep copy가 필요한 type은 현재 제한이 있음을 전제로 설계한다.