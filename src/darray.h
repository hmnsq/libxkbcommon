/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "utils.h"

تعريفنوع unsigned صحيح darray_size_t;
#define DARRAY_SIZE_T_WIDTH (حجم(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) بنية {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) ܥܒܕ { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} ܟܡܐ (0)

#define darray_free(arr) ܥܒܕ { \
    حرر((arr).item); \
    darray_init(arr); \
} ܟܡܐ (0)

#define darray_steal(arr, to, to_size) ܥܒܕ { \
    *(to) = (arr).item; \
    ܐܢ (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} ܟܡܐ (0)

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

تعريفنوع darray (حرف)           darray_char;
تعريفنوع darray (signed حرف)    darray_schar;
تعريفنوع darray (unsigned حرف)  darray_uchar;

تعريفنوع darray (حرف *)         darray_string;

تعريفنوع darray (short)          darray_short;
تعريفنوع darray (صحيح)            darray_int;
تعريفنوع darray (long)           darray_long;

تعريفنوع darray (unsigned short) darray_ushort;
تعريفنوع darray (unsigned صحيح)   darray_uint;
تعريفنوع darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_items(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  ܥܒܕ { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} ܟܡܐ (0)

#define darray_insert(arr, i, ...) ܥܒܕ { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * حجم(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} ܟܡܐ(0)

/*** Insertion (multiple items) ***/

#define darray_append_items(arr, items, count) ܥܒܕ { \
    darray_size_t __count = (count), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, items, __count * حجم(*(arr).item)); \
} ܟܡܐ (0)

#define darray_from_items(arr, items, count) ܥܒܕ { \
    darray_size_t __count = (count); \
    darray_resize(arr, __count); \
    ܐܢ (__count != 0) \
        memcpy((arr).item, items, __count * حجم(*(arr).item)); \
} ܟܡܐ (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_items((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) ܥܒܕ { \
    ܐܢ ((arr_from).size > 0) \
        darray_append_items((arr_to), (arr_from).item, (arr_from).size); \
} ܟܡܐ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) ܥܒܕ { \
    darray_size_t __index = (i); \
    ܐܢ (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * حجم(*(arr).item) \
        ); \
    (arr).size--; \
} ܟܡܐ(0)

/*** String buffer ***/

#define darray_append_string(arr, str) ܥܒܕ { \
    ثابت حرف *__str = (str); \
    darray_append_items(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} ܟܡܐ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_items((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) ܥܒܕ { \
    darray_append_items(arr, stringLiteral, \
                        (darray_size_t) حجم(stringLiteral)); \
    (arr).size--; \
} ܟܡܐ (0)

#define darray_appends_nullterminate(arr, items, count) ܥܒܕ { \
    darray_size_t __count = (count), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, items, __count * حجم(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ܟܡܐ (0)

#define darray_prepends_nullterminate(arr, items, count) ܥܒܕ { \
    darray_size_t __count = (count), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * حجم(*(arr).item)); \
    memcpy((arr).item, items, __count * حجم(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ܟܡܐ (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) ܥܒܕ { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    ܐܢ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).item)); \
    } \
} ܟܡܐ (0)

#define darray_realloc(arr, newAlloc) ܥܒܕ { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).item)); \
} ܟܡܐ (0)

#define darray_growalloc(arr, need)   ܥܒܕ { \
    darray_size_t __need = (need); \
    ܐܢ (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              حجم(*(arr).item))); \
} ܟܡܐ (0)

#define darray_shrink(arr) ܥܒܕ { \
    ܐܢ ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * حجم(*(arr).item)); \
} ܟܡܐ (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

ساكن مضمن darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    أكد(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    ܐܢ (alloc == 0)
        alloc = 4;
    ܟܡܐ (alloc < need)
        alloc *= 2;
    ܦܢܐ alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    ܐܢ ((arr).item) \
    لكل ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    ܐܢ ((arr).item) \
    لكل ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    ܐܢ ((arr).item) \
    لكل ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    ܐܢ ((arr).item) \
    لكل ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    ܐܢ ((arr).item && (arr).size) \
    لكل ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    ܐܢ ((arr).item && (arr).size) \
    لكل ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
