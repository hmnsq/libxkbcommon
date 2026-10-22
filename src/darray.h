/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#شامل <stdlib.h>
#شامل <string.h>
#شامل <assert.h>
#شامل <limits.h>

#شامل "utils.h"

تعریفنوع unsigned صحیح darray_size_t;
#define DARRAY_SIZE_T_WIDTH (اندازه(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) ساختار {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) انجامبده { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} وقتی (0)

#define darray_free(arr) انجامبده { \
    آزاد((arr).item); \
    darray_init(arr); \
} وقتی (0)

#define darray_steal(arr, to, to_size) انجامبده { \
    *(to) = (arr).item; \
    اگر (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} وقتی (0)

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

تعریفنوع darray (نویسه)           darray_char;
تعریفنوع darray (signed نویسه)    darray_schar;
تعریفنوع darray (unsigned نویسه)  darray_uchar;

تعریفنوع darray (نویسه *)         darray_string;

تعریفنوع darray (short)          darray_short;
تعریفنوع darray (صحیح)            darray_int;
تعریفنوع darray (long)           darray_long;

تعریفنوع darray (unsigned short) darray_ushort;
تعریفنوع darray (unsigned صحیح)   darray_uint;
تعریفنوع darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_اقلام(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  انجامبده { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} وقتی (0)

#define darray_درج(arr, i, ...) انجامبده { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * اندازه(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} وقتی(0)

/*** Insertion (multiple items) ***/

#define darray_append_اقلام(arr, اقلام, شمار) انجامبده { \
    darray_size_t __count = (شمار), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, اقلام, __count * اندازه(*(arr).item)); \
} وقتی (0)

#define darray_from_اقلام(arr, اقلام, شمار) انجامبده { \
    darray_size_t __count = (شمار); \
    darray_resize(arr, __count); \
    اگر (__count != 0) \
        memcpy((arr).item, اقلام, __count * اندازه(*(arr).item)); \
} وقتی (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_اقلام((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) انجامبده { \
    اگر ((arr_from).size > 0) \
        darray_append_اقلام((arr_to), (arr_from).item, (arr_from).size); \
} وقتی (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) انجامبده { \
    darray_size_t __index = (i); \
    اگر (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * اندازه(*(arr).item) \
        ); \
    (arr).size--; \
} وقتی(0)

/*** String buffer ***/

#define darray_append_string(arr, str) انجامبده { \
    ثابت نویسه *__str = (str); \
    darray_append_اقلام(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} وقتی (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_اقلام((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) انجامبده { \
    darray_append_اقلام(arr, stringLiteral, \
                        (darray_size_t) اندازه(stringLiteral)); \
    (arr).size--; \
} وقتی (0)

#define darray_appends_nullterminate(arr, اقلام, شمار) انجامبده { \
    darray_size_t __count = (شمار), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, اقلام, __count * اندازه(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} وقتی (0)

#define darray_prepends_nullterminate(arr, اقلام, شمار) انجامبده { \
    darray_size_t __count = (شمار), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * اندازه(*(arr).item)); \
    memcpy((arr).item, اقلام, __count * اندازه(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} وقتی (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) انجامبده { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    اگر (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * اندازه(*(arr).item)); \
    } \
} وقتی (0)

#define darray_realloc(arr, newAlloc) انجامبده { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * اندازه(*(arr).item)); \
} وقتی (0)

#define darray_growalloc(arr, need)   انجامبده { \
    darray_size_t __need = (need); \
    اگر (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              اندازه(*(arr).item))); \
} وقتی (0)

#define darray_shrink(arr) انجامبده { \
    اگر ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * اندازه(*(arr).item)); \
} وقتی (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

ایستا درونخطی darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    أكد(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    اگر (alloc == 0)
        alloc = 4;
    وقتی (alloc < need)
        alloc *= 2;
    بازگشت alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    اگر ((arr).item) \
    برای ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    اگر ((arr).item) \
    برای ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    اگر ((arr).item) \
    برای ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    اگر ((arr).item) \
    برای ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    اگر ((arr).item && (arr).size) \
    برای ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    اگر ((arr).item && (arr).size) \
    برای ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
