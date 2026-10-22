/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "utils.h"

ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ darray_size_t;
#define DARRAY_SIZE_T_WIDTH (ಪರಿಮಾಣ(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) ಸಂರಚನಾ {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) ಕುರು { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} ಯಾವತ್ (0)

#define darray_free(arr) ಕುರು { \
    ಮುಕ್ತ((arr).item); \
    darray_init(arr); \
} ಯಾವತ್ (0)

#define darray_steal(arr, to, to_size) ಕುರು { \
    *(to) = (arr).item; \
    ಯದಿ (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} ಯಾವತ್ (0)

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

ಪ್ರಕಾರನಿರ್ದೇಶ darray (ವರ್ಣ)           darray_char;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (signed ವರ್ಣ)    darray_schar;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (unsigned ವರ್ಣ)  darray_uchar;

ಪ್ರಕಾರನಿರ್ದೇಶ darray (ವರ್ಣ *)         darray_string;

ಪ್ರಕಾರನಿರ್ದೇಶ darray (short)          darray_short;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (ಪೂರ್ಣಾಙ್ಕ)            darray_int;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (long)           darray_long;

ಪ್ರಕಾರನಿರ್ದೇಶ darray (unsigned short) darray_ushort;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (unsigned ಪೂರ್ಣಾಙ್ಕ)   darray_uint;
ಪ್ರಕಾರನಿರ್ದೇಶ darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_ವಸ್ತುಗಳು(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  ಕುರು { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} ಯಾವತ್ (0)

#define darray_ಸೇರಿಸು(arr, i, ...) ಕುರು { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * ಪರಿಮಾಣ(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} ಯಾವತ್(0)

/*** Insertion (multiple items) ***/

#define darray_append_ವಸ್ತುಗಳು(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ) ಕುರು { \
    darray_size_t __count = (ಎಣಿಕೆ), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, ವಸ್ತುಗಳು, __count * ಪರಿಮಾಣ(*(arr).item)); \
} ಯಾವತ್ (0)

#define darray_from_ವಸ್ತುಗಳು(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ) ಕುರು { \
    darray_size_t __count = (ಎಣಿಕೆ); \
    darray_resize(arr, __count); \
    ಯದಿ (__count != 0) \
        memcpy((arr).item, ವಸ್ತುಗಳು, __count * ಪರಿಮಾಣ(*(arr).item)); \
} ಯಾವತ್ (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_ವಸ್ತುಗಳು((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) ಕುರು { \
    ಯದಿ ((arr_from).size > 0) \
        darray_append_ವಸ್ತುಗಳು((arr_to), (arr_from).item, (arr_from).size); \
} ಯಾವತ್ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) ಕುರು { \
    darray_size_t __index = (i); \
    ಯದಿ (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * ಪರಿಮಾಣ(*(arr).item) \
        ); \
    (arr).size--; \
} ಯಾವತ್(0)

/*** String buffer ***/

#define darray_append_string(arr, str) ಕುರು { \
    ಧ್ರುವ ವರ್ಣ *__str = (str); \
    darray_append_ವಸ್ತುಗಳು(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} ಯಾವತ್ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_ವಸ್ತುಗಳು((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) ಕುರು { \
    darray_append_ವಸ್ತುಗಳು(arr, stringLiteral, \
                        (darray_size_t) ಪರಿಮಾಣ(stringLiteral)); \
    (arr).size--; \
} ಯಾವತ್ (0)

#define darray_appends_nullterminate(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ) ಕುರು { \
    darray_size_t __count = (ಎಣಿಕೆ), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, ವಸ್ತುಗಳು, __count * ಪರಿಮಾಣ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ಯಾವತ್ (0)

#define darray_prepends_nullterminate(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ) ಕುರು { \
    darray_size_t __count = (ಎಣಿಕೆ), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * ಪರಿಮಾಣ(*(arr).item)); \
    memcpy((arr).item, ವಸ್ತುಗಳು, __count * ಪರಿಮಾಣ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ಯಾವತ್ (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) ಕುರು { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    ಯದಿ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * ಪರಿಮಾಣ(*(arr).item)); \
    } \
} ಯಾವತ್ (0)

#define darray_realloc(arr, newAlloc) ಕುರು { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * ಪರಿಮಾಣ(*(arr).item)); \
} ಯಾವತ್ (0)

#define darray_growalloc(arr, need)   ಕುರು { \
    darray_size_t __need = (need); \
    ಯದಿ (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              ಪರಿಮಾಣ(*(arr).item))); \
} ಯಾವತ್ (0)

#define darray_shrink(arr) ಕುರು { \
    ಯದಿ ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * ಪರಿಮಾಣ(*(arr).item)); \
} ಯಾವತ್ (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    ಪ್ರತಿಜ್ಞಾ(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    ಯದಿ (alloc == 0)
        alloc = 4;
    ಯಾವತ್ (alloc < need)
        alloc *= 2;
    ಪ್ರತ್ಯಾಗಮ alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    ಯದಿ ((arr).item) \
    ಪ್ರತಿ ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    ಯದಿ ((arr).item) \
    ಪ್ರತಿ ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    ಯದಿ ((arr).item) \
    ಪ್ರತಿ ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    ಯದಿ ((arr).item) \
    ಪ್ರತಿ ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    ಯದಿ ((arr).item && (arr).size) \
    ಪ್ರತಿ ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    ಯದಿ ((arr).item && (arr).size) \
    ಪ್ರತಿ ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
