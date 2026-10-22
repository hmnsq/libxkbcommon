/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "utils.h"

തരനിർവചനം unsigned പൂർണ്ണസംഖ്യ darray_size_t;
#define DARRAY_SIZE_T_WIDTH (വലുപ്പം(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) ഘടന {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) ചെയ്യുക { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} ആകുമ്പോൾ (0)

#define darray_free(arr) ചെയ്യുക { \
    സ്വതന്ത്രം((arr).item); \
    darray_init(arr); \
} ആകുമ്പോൾ (0)

#define darray_steal(arr, to, to_size) ചെയ്യുക { \
    *(to) = (arr).item; \
    എങ്കിൽ (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} ആകുമ്പോൾ (0)

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

തരനിർവചനം darray (അക്ഷരം)           darray_char;
തരനിർവചനം darray (signed അക്ഷരം)    darray_schar;
തരനിർവചനം darray (unsigned അക്ഷരം)  darray_uchar;

തരനിർവചനം darray (അക്ഷരം *)         darray_string;

തരനിർവചനം darray (short)          darray_short;
തരനിർവചനം darray (പൂർണ്ണസംഖ്യ)            darray_int;
തരനിർവചനം darray (long)           darray_long;

തരനിർവചനം darray (unsigned short) darray_ushort;
തരനിർവചനം darray (unsigned പൂർണ്ണസംഖ്യ)   darray_uint;
തരനിർവചനം darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_ഇനങ്ങൾ(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  ചെയ്യുക { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} ആകുമ്പോൾ (0)

#define darray_തിരുകുക(arr, i, ...) ചെയ്യുക { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * വലുപ്പം(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} ആകുമ്പോൾ(0)

/*** Insertion (multiple items) ***/

#define darray_append_ഇനങ്ങൾ(arr, ഇനങ്ങൾ, എണ്ണം) ചെയ്യുക { \
    darray_size_t __count = (എണ്ണം), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, ഇനങ്ങൾ, __count * വലുപ്പം(*(arr).item)); \
} ആകുമ്പോൾ (0)

#define darray_from_ഇനങ്ങൾ(arr, ഇനങ്ങൾ, എണ്ണം) ചെയ്യുക { \
    darray_size_t __count = (എണ്ണം); \
    darray_resize(arr, __count); \
    എങ്കിൽ (__count != 0) \
        memcpy((arr).item, ഇനങ്ങൾ, __count * വലുപ്പം(*(arr).item)); \
} ആകുമ്പോൾ (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_ഇനങ്ങൾ((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) ചെയ്യുക { \
    എങ്കിൽ ((arr_from).size > 0) \
        darray_append_ഇനങ്ങൾ((arr_to), (arr_from).item, (arr_from).size); \
} ആകുമ്പോൾ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) ചെയ്യുക { \
    darray_size_t __index = (i); \
    എങ്കിൽ (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * വലുപ്പം(*(arr).item) \
        ); \
    (arr).size--; \
} ആകുമ്പോൾ(0)

/*** String buffer ***/

#define darray_append_string(arr, str) ചെയ്യുക { \
    സ്ഥിരം അക്ഷരം *__str = (str); \
    darray_append_ഇനങ്ങൾ(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} ആകുമ്പോൾ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_ഇനങ്ങൾ((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) ചെയ്യുക { \
    darray_append_ഇനങ്ങൾ(arr, stringLiteral, \
                        (darray_size_t) വലുപ്പം(stringLiteral)); \
    (arr).size--; \
} ആകുമ്പോൾ (0)

#define darray_appends_nullterminate(arr, ഇനങ്ങൾ, എണ്ണം) ചെയ്യുക { \
    darray_size_t __count = (എണ്ണം), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, ഇനങ്ങൾ, __count * വലുപ്പം(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ആകുമ്പോൾ (0)

#define darray_prepends_nullterminate(arr, ഇനങ്ങൾ, എണ്ണം) ചെയ്യുക { \
    darray_size_t __count = (എണ്ണം), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * വലുപ്പം(*(arr).item)); \
    memcpy((arr).item, ഇനങ്ങൾ, __count * വലുപ്പം(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ആകുമ്പോൾ (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) ചെയ്യുക { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    എങ്കിൽ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * വലുപ്പം(*(arr).item)); \
    } \
} ആകുമ്പോൾ (0)

#define darray_realloc(arr, newAlloc) ചെയ്യുക { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * വലുപ്പം(*(arr).item)); \
} ആകുമ്പോൾ (0)

#define darray_growalloc(arr, need)   ചെയ്യുക { \
    darray_size_t __need = (need); \
    എങ്കിൽ (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              വലുപ്പം(*(arr).item))); \
} ആകുമ്പോൾ (0)

#define darray_shrink(arr) ചെയ്യുക { \
    എങ്കിൽ ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * വലുപ്പം(*(arr).item)); \
} ആകുമ്പോൾ (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

സ്ഥിതം വരിയിൽ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    പ്രതിജ്ഞാ(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    എങ്കിൽ (alloc == 0)
        alloc = 4;
    ആകുമ്പോൾ (alloc < need)
        alloc *= 2;
    തിരികെ alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    എങ്കിൽ ((arr).item) \
    വേണ്ടി ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    എങ്കിൽ ((arr).item) \
    വേണ്ടി ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    എങ്കിൽ ((arr).item) \
    വേണ്ടി ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    എങ്കിൽ ((arr).item) \
    വേണ്ടി ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    എങ്കിൽ ((arr).item && (arr).size) \
    വേണ്ടി ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    എങ്കിൽ ((arr).item && (arr).size) \
    വേണ്ടി ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
