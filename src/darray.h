/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>

#ଅନ୍ତର୍ଭାଵ "utils.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା darray_size_t;
#define DARRAY_SIZE_T_WIDTH (ଆକାର(darray_size_t) * CHAR_BIT)
#define DARRAY_SIZE_MAX UINT_MAX

#define darray(type) ସଂରଚନା {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) କର { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} ଯେତେବେଳେ (0)

#define darray_free(arr) କର { \
    ମୁକ୍ତ((arr).item); \
    darray_init(arr); \
} ଯେତେବେଳେ (0)

#define darray_steal(arr, to, to_size) କର { \
    *(to) = (arr).item; \
    ଯଦି (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} ଯେତେବେଳେ (0)

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

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (ଅକ୍ଷର)           darray_char;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (signed ଅକ୍ଷର)    darray_schar;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (unsigned ଅକ୍ଷର)  darray_uchar;

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (ଅକ୍ଷର *)         darray_string;

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (short)          darray_short;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)            darray_int;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (long)           darray_long;

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (unsigned short) darray_ushort;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)   darray_uint;
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_ବସ୍ତୁଗୁଡ଼ିକ(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  କର { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} ଯେତେବେଳେ (0)

#define darray_ଭର୍ତ୍ତି(arr, i, ...) କର { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * ଆକାର(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} ଯେତେବେଳେ(0)

/*** Insertion (multiple items) ***/

#define darray_append_ବସ୍ତୁଗୁଡ଼ିକ(arr, ବସ୍ତୁଗୁଡ଼ିକ, ଗଣନା_2) କର { \
    darray_size_t __count = (ଗଣନା_2), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, ବସ୍ତୁଗୁଡ଼ିକ, __count * ଆକାର(*(arr).item)); \
} ଯେତେବେଳେ (0)

#define darray_from_ବସ୍ତୁଗୁଡ଼ିକ(arr, ବସ୍ତୁଗୁଡ଼ିକ, ଗଣନା_2) କର { \
    darray_size_t __count = (ଗଣନା_2); \
    darray_resize(arr, __count); \
    ଯଦି (__count != 0) \
        memcpy((arr).item, ବସ୍ତୁଗୁଡ଼ିକ, __count * ଆକାର(*(arr).item)); \
} ଯେତେବେଳେ (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_ବସ୍ତୁଗୁଡ଼ିକ((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) କର { \
    ଯଦି ((arr_from).size > 0) \
        darray_append_ବସ୍ତୁଗୁଡ଼ିକ((arr_to), (arr_from).item, (arr_from).size); \
} ଯେତେବେଳେ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) କର { \
    darray_size_t __index = (i); \
    ଯଦି (__index < (arr).size-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * ଆକାର(*(arr).item) \
        ); \
    (arr).size--; \
} ଯେତେବେଳେ(0)

/*** String buffer ***/

#define darray_append_string(arr, str) କର { \
    ଅଚଳ ଅକ୍ଷର *__str = (str); \
    darray_append_ବସ୍ତୁଗୁଡ଼ିକ(arr, __str, (darray_size_t) strlen(__str) + 1); \
    (arr).size--; \
} ଯେତେବେଳେ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_ବସ୍ତୁଗୁଡ଼ିକ((arr), (string), (darray_size_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) କର { \
    darray_append_ବସ୍ତୁଗୁଡ଼ିକ(arr, stringLiteral, \
                        (darray_size_t) ଆକାର(stringLiteral)); \
    (arr).size--; \
} ଯେତେବେଳେ (0)

#define darray_appends_nullterminate(arr, ବସ୍ତୁଗୁଡ଼ିକ, ଗଣନା_2) କର { \
    darray_size_t __count = (ଗଣନା_2), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, ବସ୍ତୁଗୁଡ଼ିକ, __count * ଆକାର(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ଯେତେବେଳେ (0)

#define darray_prepends_nullterminate(arr, ବସ୍ତୁଗୁଡ଼ିକ, ଗଣନା_2) କର { \
    darray_size_t __count = (ଗଣନା_2), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * ଆକାର(*(arr).item)); \
    memcpy((arr).item, ବସ୍ତୁଗୁଡ଼ିକ, __count * ଆକାର(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} ଯେତେବେଳେ (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) କର { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    ଯଦି (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * ଆକାର(*(arr).item)); \
    } \
} ଯେତେବେଳେ (0)

#define darray_realloc(arr, newAlloc) କର { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * ଆକାର(*(arr).item)); \
} ଯେତେବେଳେ (0)

#define darray_growalloc(arr, need)   କର { \
    darray_size_t __need = (need); \
    ଯଦି (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              ଆକାର(*(arr).item))); \
} ଯେତେବେଳେ (0)

#define darray_shrink(arr) କର { \
    ଯଦି ((arr).size > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).size) * ଆକାର(*(arr).item)); \
} ଯେତେବେଳେ (0)

#define darray_max_alloc(itemSize) (UINT_MAX / (itemSize))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, size_t itemSize)
{
    ପ୍ରତିଜ୍ଞା(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    ଯଦି (alloc == 0)
        alloc = 4;
    ଯେତେବେଳେ (alloc < need)
        alloc *= 2;
    ଫେରନ୍ତୁ alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    ଯଦି ((arr).item) \
    ପାଇଁ ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    ଯଦି ((arr).item) \
    ପାଇଁ ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    ଯଦି ((arr).item) \
    ପାଇଁ ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    ଯଦି ((arr).item) \
    ପାଇଁ ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    ଯଦି ((arr).item && (arr).size) \
    ପାଇଁ ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    ଯଦି ((arr).item && (arr).size) \
    ପାଇଁ ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
