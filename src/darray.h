/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "utils.h"

প্রকারনির্দেশ unsigned পূর্ণাঙ্ক darray_size_t;
#define DARRAY_SIZE_T_WIDTH (পরিমাণ(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) সংরচনা {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) কুরু { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} যাবত্ (0)

#define darray_free(arr) কুরু { \
    মুক্ত((arr).item); \
    darray_init(arr); \
} যাবত্ (0)

#define darray_steal(arr, to, to_size) কুরু { \
    *(to) = (arr).item; \
    যদি (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} যাবত্ (0)

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

প্রকারনির্দেশ darray (বর্ণ)           darray_char;
প্রকারনির্দেশ darray (signed বর্ণ)    darray_schar;
প্রকারনির্দেশ darray (unsigned বর্ণ)  darray_uchar;

প্রকারনির্দেশ darray (বর্ণ *)         darray_string;

প্রকারনির্দেশ darray (short)          darray_short;
প্রকারনির্দেশ darray (পূর্ণাঙ্ক)            darray_int;
প্রকারনির্দেশ darray (long)           darray_long;

প্রকারনির্দেশ darray (unsigned short) darray_ushort;
প্রকারনির্দেশ darray (unsigned পূর্ণাঙ্ক)   darray_uint;
প্রকারনির্দেশ darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_বস্তুসমূহ(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  কুরু { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} যাবত্ (0)

#define darray_ঢোকাও(arr, i, ...) কুরু { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * পরিমাণ(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} যাবত্(0)

/*** Insertion (multiple items) ***/

#define darray_append_বস্তুসমূহ(arr, বস্তুসমূহ, গুনতি) কুরু { \
    darray_size_t __count = (গুনতি), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, বস্তুসমূহ, __count * পরিমাণ(*(arr).item)); \
} যাবত্ (0)

#define darray_from_বস্তুসমূহ(arr, বস্তুসমূহ, গুনতি) কুরু { \
    darray_size_t __count = (গুনতি); \
    darray_resize(arr, __count); \
    যদি (__count != 0) \
        memcpy((arr).item, বস্তুসমূহ, __count * পরিমাণ(*(arr).item)); \
} যাবত্ (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_বস্তুসমূহ((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) কুরু { \
    যদি ((arr_from).size > 0) \
        darray_append_বস্তুসমূহ((arr_to), (arr_from).item, (arr_from).size); \
} যাবত্ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) কুরু { \
    darray_size_t __index = (i); \
    যদি (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * পরিমাণ(*(arr).item) \
        ); \
    (arr).size--; \
} যাবত্(0)

/*** String buffer ***/

#define darray_append_string(arr, str) কুরু { \
    ধ্রুব বর্ণ *__str = (str); \
    darray_append_বস্তুসমূহ(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} যাবত্ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_বস্তুসমূহ((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) কুরু { \
    darray_append_বস্তুসমূহ(arr, stringLiteral, \
                        (darray_size_t) পরিমাণ(stringLiteral)); \
    (arr).size--; \
} যাবত্ (0)

#define darray_appends_nullterminate(arr, বস্তুসমূহ, গুনতি) কুরু { \
    darray_size_t __count = (গুনতি), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, বস্তুসমূহ, __count * পরিমাণ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} যাবত্ (0)

#define darray_prepends_nullterminate(arr, বস্তুসমূহ, গুনতি) কুরু { \
    darray_size_t __count = (গুনতি), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * পরিমাণ(*(arr).item)); \
    memcpy((arr).item, বস্তুসমূহ, __count * পরিমাণ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} যাবত্ (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) কুরু { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    যদি (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * পরিমাণ(*(arr).item)); \
    } \
} যাবত্ (0)

#define darray_realloc(arr, newAlloc) কুরু { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * পরিমাণ(*(arr).item)); \
} যাবত্ (0)

#define darray_growalloc(arr, need)   কুরু { \
    darray_size_t __need = (need); \
    যদি (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              পরিমাণ(*(arr).item))); \
} যাবত্ (0)

#define darray_shrink(arr) কুরু { \
    যদি ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * পরিমাণ(*(arr).item)); \
} যাবত্ (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

স্থির অন্তঃস্থ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    প্রতিজ্ঞা(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    যদি (alloc == 0)
        alloc = 4;
    যাবত্ (alloc < need)
        alloc *= 2;
    প্রত্যাগম alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    যদি ((arr).item) \
    প্রতি ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    যদি ((arr).item) \
    প্রতি ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    যদি ((arr).item) \
    প্রতি ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    যদি ((arr).item) \
    প্রতি ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    যদি ((arr).item && (arr).size) \
    প্রতি ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    যদি ((arr).item && (arr).size) \
    প্রতি ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
