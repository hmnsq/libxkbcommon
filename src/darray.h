/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>

#அந்தர்பாவ "utils.h"

வகைவரை unsigned முழு darray_size_t;
#define DARRAY_SIZE_T_WIDTH (அளவு(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) கட்டமைப்பு {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) செய் { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} போது (0)

#define darray_free(arr) செய் { \
    விடுவி((arr).item); \
    darray_init(arr); \
} போது (0)

#define darray_steal(arr, to, to_size) செய் { \
    *(to) = (arr).item; \
    எனில் (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} போது (0)

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

வகைவரை darray (எழுத்து)           darray_char;
வகைவரை darray (signed எழுத்து)    darray_schar;
வகைவரை darray (unsigned எழுத்து)  darray_uchar;

வகைவரை darray (எழுத்து *)         darray_string;

வகைவரை darray (short)          darray_short;
வகைவரை darray (முழு)            darray_int;
வகைவரை darray (long)           darray_long;

வகைவரை darray (unsigned short) darray_ushort;
வகைவரை darray (unsigned முழு)   darray_uint;
வகைவரை darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_பொருட்கள்(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  செய் { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} போது (0)

#define darray_செருகு(arr, i, ...) செய் { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * அளவு(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} போது(0)

/*** Insertion (multiple items) ***/

#define darray_append_பொருட்கள்(arr, பொருட்கள், மொத்தம்) செய் { \
    darray_size_t __count = (மொத்தம்), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, பொருட்கள், __count * அளவு(*(arr).item)); \
} போது (0)

#define darray_from_பொருட்கள்(arr, பொருட்கள், மொத்தம்) செய் { \
    darray_size_t __count = (மொத்தம்); \
    darray_resize(arr, __count); \
    எனில் (__count != 0) \
        memcpy((arr).item, பொருட்கள், __count * அளவு(*(arr).item)); \
} போது (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_பொருட்கள்((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) செய் { \
    எனில் ((arr_from).size > 0) \
        darray_append_பொருட்கள்((arr_to), (arr_from).item, (arr_from).size); \
} போது (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) செய் { \
    darray_size_t __index = (i); \
    எனில் (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * அளவு(*(arr).item) \
        ); \
    (arr).size--; \
} போது(0)

/*** String buffer ***/

#define darray_append_string(arr, str) செய் { \
    மாறிலி எழுத்து *__str = (str); \
    darray_append_பொருட்கள்(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} போது (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_பொருட்கள்((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) செய் { \
    darray_append_பொருட்கள்(arr, stringLiteral, \
                        (darray_size_t) அளவு(stringLiteral)); \
    (arr).size--; \
} போது (0)

#define darray_appends_nullterminate(arr, பொருட்கள், மொத்தம்) செய் { \
    darray_size_t __count = (மொத்தம்), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, பொருட்கள், __count * அளவு(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} போது (0)

#define darray_prepends_nullterminate(arr, பொருட்கள், மொத்தம்) செய் { \
    darray_size_t __count = (மொத்தம்), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * அளவு(*(arr).item)); \
    memcpy((arr).item, பொருட்கள், __count * அளவு(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} போது (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) செய் { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    எனில் (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * அளவு(*(arr).item)); \
    } \
} போது (0)

#define darray_realloc(arr, newAlloc) செய் { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * அளவு(*(arr).item)); \
} போது (0)

#define darray_growalloc(arr, need)   செய் { \
    darray_size_t __need = (need); \
    எனில் (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              அளவு(*(arr).item))); \
} போது (0)

#define darray_shrink(arr) செய் { \
    எனில் ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * அளவு(*(arr).item)); \
} போது (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

நிலையான உள்வரி darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    ப்ரதிஜ்ஞா(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    எனில் (alloc == 0)
        alloc = 4;
    போது (alloc < need)
        alloc *= 2;
    திரும்பு alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    எனில் ((arr).item) \
    க்கு ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    எனில் ((arr).item) \
    க்கு ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    எனில் ((arr).item) \
    க்கு ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    எனில் ((arr).item) \
    க்கு ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    எனில் ((arr).item && (arr).size) \
    க்கு ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    எனில் ((arr).item && (arr).size) \
    க்கு ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
