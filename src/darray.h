/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>

#અન્તર્ભાવ "utils.h"

પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક darray_size_t;
#define DARRAY_SIZE_T_WIDTH (કદ(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) સંરચના {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) કરો { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} જ્યારે (0)

#define darray_free(arr) કરો { \
    મુક્ત((arr).item); \
    darray_init(arr); \
} જ્યારે (0)

#define darray_steal(arr, to, to_size) કરો { \
    *(to) = (arr).item; \
    જો (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} જ્યારે (0)

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

પ્રકારવ્યાખ્યા darray (અક્ષર)           darray_char;
પ્રકારવ્યાખ્યા darray (signed અક્ષર)    darray_schar;
પ્રકારવ્યાખ્યા darray (unsigned અક્ષર)  darray_uchar;

પ્રકારવ્યાખ્યા darray (અક્ષર *)         darray_string;

પ્રકારવ્યાખ્યા darray (short)          darray_short;
પ્રકારવ્યાખ્યા darray (પૂર્ણાંક)            darray_int;
પ્રકારવ્યાખ્યા darray (long)           darray_long;

પ્રકારવ્યાખ્યા darray (unsigned short) darray_ushort;
પ્રકારવ્યાખ્યા darray (unsigned પૂર્ણાંક)   darray_uint;
પ્રકારવ્યાખ્યા darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_વસ્તુઓ(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  કરો { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} જ્યારે (0)

#define darray_ઉમેરો(arr, i, ...) કરો { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * કદ(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} જ્યારે(0)

/*** Insertion (multiple items) ***/

#define darray_append_વસ્તુઓ(arr, વસ્તુઓ, ગણતરી) કરો { \
    darray_size_t __count = (ગણતરી), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, વસ્તુઓ, __count * કદ(*(arr).item)); \
} જ્યારે (0)

#define darray_from_વસ્તુઓ(arr, વસ્તુઓ, ગણતરી) કરો { \
    darray_size_t __count = (ગણતરી); \
    darray_resize(arr, __count); \
    જો (__count != 0) \
        memcpy((arr).item, વસ્તુઓ, __count * કદ(*(arr).item)); \
} જ્યારે (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_વસ્તુઓ((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) કરો { \
    જો ((arr_from).size > 0) \
        darray_append_વસ્તુઓ((arr_to), (arr_from).item, (arr_from).size); \
} જ્યારે (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) કરો { \
    darray_size_t __index = (i); \
    જો (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * કદ(*(arr).item) \
        ); \
    (arr).size--; \
} જ્યારે(0)

/*** String buffer ***/

#define darray_append_string(arr, str) કરો { \
    અચલ અક્ષર *__str = (str); \
    darray_append_વસ્તુઓ(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} જ્યારે (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_વસ્તુઓ((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) કરો { \
    darray_append_વસ્તુઓ(arr, stringLiteral, \
                        (darray_size_t) કદ(stringLiteral)); \
    (arr).size--; \
} જ્યારે (0)

#define darray_appends_nullterminate(arr, વસ્તુઓ, ગણતરી) કરો { \
    darray_size_t __count = (ગણતરી), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, વસ્તુઓ, __count * કદ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} જ્યારે (0)

#define darray_prepends_nullterminate(arr, વસ્તુઓ, ગણતરી) કરો { \
    darray_size_t __count = (ગણતરી), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * કદ(*(arr).item)); \
    memcpy((arr).item, વસ્તુઓ, __count * કદ(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} જ્યારે (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) કરો { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    જો (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * કદ(*(arr).item)); \
    } \
} જ્યારે (0)

#define darray_realloc(arr, newAlloc) કરો { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * કદ(*(arr).item)); \
} જ્યારે (0)

#define darray_growalloc(arr, need)   કરો { \
    darray_size_t __need = (need); \
    જો (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              કદ(*(arr).item))); \
} જ્યારે (0)

#define darray_shrink(arr) કરો { \
    જો ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * કદ(*(arr).item)); \
} જ્યારે (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

સ્થિર અંતઃરેખ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    પ્રતિજ્ઞા(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    જો (alloc == 0)
        alloc = 4;
    જ્યારે (alloc < need)
        alloc *= 2;
    પાછા alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    જો ((arr).item) \
    માટે ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    જો ((arr).item) \
    માટે ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    જો ((arr).item) \
    માટે ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    જો ((arr).item) \
    માટે ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    જો ((arr).item && (arr).size) \
    માટે ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    જો ((arr).item && (arr).size) \
    માટે ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
