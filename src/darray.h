/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ darray_ਮਾਪ_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DARRAY_ਮਾਪ_T_ਚੌੜਾਈ (ਆਕਾਰ(darray_ਮਾਪ_t) * CHAR_BIT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DARRAY_ਮਾਪ_MAX UINT_MAX

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray(type) ਬਣਤਰ {         \
    /** Current size */               \
    darray_ਮਾਪ_t ਮਾਪ;               \
    /** Count of allocated items */   \
    darray_ਮਾਪ_t alloc;              \
    /** Array of items */             \
    type *ਵਸਤੂ ATTR_COUNTED_BY(ਮਾਪ); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_new() { 0, 0, 0 }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_init(arr) ਕਰੋ { \
    (arr).ਵਸਤੂ = 0; (arr).ਮਾਪ = 0; (arr).alloc = 0; \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_free(arr) ਕਰੋ { \
    ਮੁਕਤ((arr).ਵਸਤੂ); \
    darray_init(arr); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_steal(arr, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_ਮਾਪ) ਕਰੋ { \
    *(ਮੰਜ਼ਿਲ) = (arr).ਵਸਤੂ; \
    ਜੇ (ਮੰਜ਼ਿਲ_ਮਾਪ) \
        *(darray_ਮਾਪ_t *) (ਮੰਜ਼ਿਲ_ਮਾਪ) = (arr).ਮਾਪ; \
    darray_init(arr); \
} ਜਦੋਂ (0)

/*
 * Typedefs for darrays of common types.  These are useful
 * when you want to pass a pointer to an darray(T) around.
 *
 * The following will produce an incompatible pointer warning:
 *
 *     void foo(darray(int) *arr);
 *     darray(int) arr = darray_new();
 *     foo(&arr);
 *
 * The workaround:
 *
 *     void foo(darray_int *arr);
 *     darray_int arr = darray_new();
 *     foo(&arr);
 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਅੱਖਰ)           darray_char;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)    darray_schar;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)  darray_uchar;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਅੱਖਰ *)         darray_string;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਛੋਟਾ)          darray_short;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਪੂਰਨ)            darray_int;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਲੰਮਾ)           darray_long;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ) darray_ushort;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)   darray_uint;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)  darray_ulong;

/*** Access ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਵਸਤੂ(arr, i)     ((arr).ਵਸਤੂ[i])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਚੀਜ਼ਾਂ(arr)       ((arr).ਵਸਤੂ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਮਾਪ(arr)        ((arr).ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਖਾਲੀ_ਥਾਵਾਂ(arr)       ((arr).ਮਾਪ == 0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਆਖਰੀ(arr, fallback) ( \
    !(arr).ਮਾਪ \
        ? (fallback) \
        : (arr).ਵਸਤੂ[(arr).ਮਾਪ - 1] \
)

/*** Insertion (single item) ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_append(arr, ...)  ਕਰੋ { \
    darray_resize(arr, (arr).ਮਾਪ + 1); \
    (arr).ਵਸਤੂ[(arr).ਮਾਪ - 1] = (__VA_ARGS__); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_ਪਾਓ(arr, i, ...) ਕਰੋ { \
    darray_ਮਾਪ_t __index = (i); \
    darray_resize(arr, (arr).ਮਾਪ+1); \
    memmove( \
        (arr).ਵਸਤੂ + __index + 1, \
        (arr).ਵਸਤੂ + __index, \
        ((arr).ਮਾਪ - __index - 1) * ਆਕਾਰ(*(arr).ਵਸਤੂ) \
    ); \
    (arr).ਵਸਤੂ[__index] = (__VA_ARGS__); \
} ਜਦੋਂ(0)

/*** Insertion (multiple items) ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_append_ਚੀਜ਼ਾਂ(arr, ਚੀਜ਼ਾਂ, ਗਿਣਤੀ_2) ਕਰੋ { \
    darray_ਮਾਪ_t __count = (ਗਿਣਤੀ_2), __oldSize = (arr).ਮਾਪ; \
    darray_resize(arr, __oldSize + __count); \
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((arr).ਵਸਤੂ + __oldSize, ਚੀਜ਼ਾਂ, __count * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_from_ਚੀਜ਼ਾਂ(arr, ਚੀਜ਼ਾਂ, ਗਿਣਤੀ_2) ਕਰੋ { \
    darray_ਮਾਪ_t __count = (ਗਿਣਤੀ_2); \
    darray_resize(arr, __count); \
    ਜੇ (__count != 0) \
        ਸਮ੍ਰਿਤੀ_ਨਕਲ((arr).ਵਸਤੂ, ਚੀਜ਼ਾਂ, __count * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_copy(arr_ਮੰਜ਼ਿਲ, arr_from) \
    darray_from_ਚੀਜ਼ਾਂ((arr_ਮੰਜ਼ਿਲ), (arr_from).ਵਸਤੂ, (arr_from).ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_concat(arr_ਮੰਜ਼ਿਲ, arr_from) ਕਰੋ { \
    ਜੇ ((arr_from).ਮਾਪ > 0) \
        darray_append_ਚੀਜ਼ਾਂ((arr_ਮੰਜ਼ਿਲ), (arr_from).ਵਸਤੂ, (arr_from).ਮਾਪ); \
} ਜਦੋਂ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_remove_ਆਖਰੀ(arr) (--(arr).ਮਾਪ)

/* Warning, slow: Requires copying all elements after removed item. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_remove(arr, i) ਕਰੋ { \
    darray_ਮਾਪ_t __index = (i); \
    ਜੇ (__index < (arr).ਮਾਪ-1) \
        memmove( \
            &(arr).ਵਸਤੂ[__index], \
            &(arr).ਵਸਤੂ[__index + 1], \
            ((arr).ਮਾਪ -1 - __index) * ਆਕਾਰ(*(arr).ਵਸਤੂ) \
        ); \
    (arr).ਮਾਪ--; \
} ਜਦੋਂ(0)

/*** String buffer ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_append_string(arr, str) ਕਰੋ { \
    ਅਚਲ ਅੱਖਰ *__str = (str); \
    darray_append_ਚੀਜ਼ਾਂ(arr, __str, (darray_ਮਾਪ_t) ਲੜੀ_ਲੰਬਾਈ(__str) + 1); \
    (arr).ਮਾਪ--; \
} ਜਦੋਂ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_append_string0(arr, string) \
    darray_append_ਚੀਜ਼ਾਂ((arr), (string), (darray_ਮਾਪ_t)ਲੜੀ_ਲੰਬਾਈ(string) + 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_append_lit(arr, stringLiteral) ਕਰੋ { \
    darray_append_ਚੀਜ਼ਾਂ(arr, stringLiteral, \
                        (darray_ਮਾਪ_t) ਆਕਾਰ(stringLiteral)); \
    (arr).ਮਾਪ--; \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_appends_nullterminate(arr, ਚੀਜ਼ਾਂ, ਗਿਣਤੀ_2) ਕਰੋ { \
    darray_ਮਾਪ_t __count = (ਗਿਣਤੀ_2), __oldSize = (arr).ਮਾਪ; \
    darray_resize(arr, __oldSize + __count + 1); \
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((arr).ਵਸਤੂ + __oldSize, ਚੀਜ਼ਾਂ, __count * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
    (arr).ਵਸਤੂ[(arr).ਮਾਪ - 1] = 0; \
    (arr).ਮਾਪ--; \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_prepends_nullterminate(arr, ਚੀਜ਼ਾਂ, ਗਿਣਤੀ_2) ਕਰੋ { \
    darray_ਮਾਪ_t __count = (ਗਿਣਤੀ_2), __oldSize = (arr).ਮਾਪ; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).ਵਸਤੂ + __count, (arr).ਵਸਤੂ, \
            __oldSize * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((arr).ਵਸਤੂ, ਚੀਜ਼ਾਂ, __count * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
    (arr).ਵਸਤੂ[(arr).ਮਾਪ - 1] = 0; \
    (arr).ਮਾਪ--; \
} ਜਦੋਂ (0)

/*** Size management ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_resize(arr, new_ਮਾਪ) \
    darray_growalloc(arr, (arr).ਮਾਪ = (new_ਮਾਪ))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_resize0(arr, new_ਮਾਪ) ਕਰੋ { \
    darray_ਮਾਪ_t __oldSize = (arr).ਮਾਪ, __newSize = (new_ਮਾਪ); \
    (arr).ਮਾਪ = __newSize; \
    ਜੇ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).ਵਸਤੂ[__oldSize], 0, \
               (__newSize - __oldSize) * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
    } \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_realloc(arr, newAlloc) ਕਰੋ { \
    (arr).ਵਸਤੂ = realloc((arr).ਵਸਤੂ, \
                         ((arr).alloc = (newAlloc)) * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_growalloc(arr, need)   ਕਰੋ { \
    darray_ਮਾਪ_t __need = (need); \
    ਜੇ (__need > (arr).alloc) \
        darray_realloc(arr, darray_ਅਗਲਾ_alloc((arr).alloc, __need, \
                                              ਆਕਾਰ(*(arr).ਵਸਤੂ))); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_shrink(arr) ਕਰੋ { \
    ਜੇ ((arr).ਮਾਪ > 0) \
        (arr).ਵਸਤੂ = realloc((arr).ਵਸਤੂ, \
                             ((arr).alloc = (arr).ਮਾਪ) * ਆਕਾਰ(*(arr).ਵਸਤੂ)); \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_max_alloc(ਵਸਤੂ_ਮਾਪ) (UINT_MAX / (ਵਸਤੂ_ਮਾਪ))

ਸਥਿਰ ਸਤਰਅੰਦਰ darray_ਮਾਪ_t
darray_ਅਗਲਾ_alloc(darray_ਮਾਪ_t alloc, darray_ਮਾਪ_t need, ਮਾਪ_t ਵਸਤੂ_ਮਾਪ)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(need < darray_max_alloc(ਵਸਤੂ_ਮਾਪ) / 2); /* Overflow. */
    ਜੇ (alloc == 0)
        alloc = 4;
    ਜਦੋਂ (alloc < need)
        alloc *= 2;
    ਵਾਪਸ alloc;
}

/*** Traversal ***/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_foreach(i, arr) \
    ਜੇ ((arr).ਵਸਤੂ) \
    ਲਈ ((i) = &(arr).ਵਸਤੂ[0]; (i) < &(arr).ਵਸਤੂ[(arr).ਮਾਪ]; (i)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_foreach_from(i, arr, from) \
    ਜੇ ((arr).ਵਸਤੂ) \
    ਲਈ ((i) = &(arr).ਵਸਤੂ[from]; (i) < &(arr).ਵਸਤੂ[(arr).ਮਾਪ]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_enumerate(idx, val, arr) \
    ਜੇ ((arr).ਵਸਤੂ) \
    ਲਈ ((idx) = 0, (val) = &(arr).ਵਸਤੂ[0]; \
         (idx) < (arr).ਮਾਪ; \
         (idx)++, (val)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_enumerate_from(idx, val, arr, from) \
    ਜੇ ((arr).ਵਸਤੂ) \
    ਲਈ ((idx) = (from), (val) = &(arr).ਵਸਤੂ[from]; \
         (idx) < (arr).ਮਾਪ; \
         (idx)++, (val)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_foreach_ਉਲਟ(i, arr) \
    ਜੇ ((arr).ਵਸਤੂ && (arr).ਮਾਪ) \
    ਲਈ ((i) = &(arr).ਵਸਤੂ[(arr).ਮਾਪ - 1]; \
         (i) >= &(arr).ਵਸਤੂ[0]; \
         (i)--)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ darray_enumerate_ਉਲਟ(idx, val, arr) \
    ਜੇ ((arr).ਵਸਤੂ && (arr).ਮਾਪ) \
    ਲਈ ((idx) = (arr).ਮਾਪ - 1, (val) = &(arr).ਵਸਤੂ[(arr).ਮਾਪ - 1]; \
         (val) >= &(arr).ਵਸਤੂ[0]; \
         (idx)--, (val)--)
