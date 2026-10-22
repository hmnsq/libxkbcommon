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

প্রকারলেখ unsigned পূর্ণসংখ্যা darray_আকার_t;
#define DARRAY_আকার_T_WIDTH (আকার(darray_আকার_t) * CHAR_BIT)
#define DARRAY_আকার_MAX UINT_MAX

#define darray(type) কাঠামো {         \
    /** Current size */               \
    darray_আকার_t আকার_2;               \
    /** Count of allocated items */   \
    darray_আকার_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(আকার_2); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) করো { \
    (arr).item = 0; (arr).আকার_2 = 0; (arr).alloc = 0; \
} যখন (0)

#define darray_free(arr) করো { \
    মুক্ত((arr).item); \
    darray_init(arr); \
} যখন (0)

#define darray_steal(arr, to, to_আকার) করো { \
    *(to) = (arr).item; \
    যদি (to_আকার) \
        *(darray_আকার_t *) (to_আকার) = (arr).আকার_2; \
    darray_init(arr); \
} যখন (0)

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

প্রকারলেখ darray (অক্ষর)           darray_char;
প্রকারলেখ darray (signed অক্ষর)    darray_schar;
প্রকারলেখ darray (unsigned অক্ষর)  darray_uchar;

প্রকারলেখ darray (অক্ষর *)         darray_string;

প্রকারলেখ darray (short)          darray_short;
প্রকারলেখ darray (পূর্ণসংখ্যা)            darray_int;
প্রকারলেখ darray (long)           darray_long;

প্রকারলেখ darray (unsigned short) darray_ushort;
প্রকারলেখ darray (unsigned পূর্ণসংখ্যা)   darray_uint;
প্রকারলেখ darray (unsigned long)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_বস্তুসমূহ(arr)       ((arr).item)
#define darray_আকার(arr)        ((arr).আকার_2)
#define darray_empty(arr)       ((arr).আকার_2 == 0)

#define darray_last(arr, fallback) ( \
    !(arr).আকার_2 \
        ? (fallback) \
        : (arr).item[(arr).আকার_2 - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  করো { \
    darray_resize(arr, (arr).আকার_2 + 1); \
    (arr).item[(arr).আকার_2 - 1] = (__VA_ARGS__); \
} যখন (0)

#define darray_ঢোকাও(arr, i, ...) করো { \
    darray_আকার_t __index = (i); \
    darray_resize(arr, (arr).আকার_2+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).আকার_2 - __index - 1) * আকার(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} যখন(0)

/*** Insertion (multiple items) ***/

#define darray_append_বস্তুসমূহ(arr, বস্তুসমূহ, গুনতি) করো { \
    darray_আকার_t __count = (গুনতি), __oldSize = (arr).আকার_2; \
    darray_resize(arr, __oldSize + __count); \
    memcpy((arr).item + __oldSize, বস্তুসমূহ, __count * আকার(*(arr).item)); \
} যখন (0)

#define darray_from_বস্তুসমূহ(arr, বস্তুসমূহ, গুনতি) করো { \
    darray_আকার_t __count = (গুনতি); \
    darray_resize(arr, __count); \
    যদি (__count != 0) \
        memcpy((arr).item, বস্তুসমূহ, __count * আকার(*(arr).item)); \
} যখন (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_বস্তুসমূহ((arr_to), (arr_from).item, (arr_from).আকার_2)

#define darray_concat(arr_to, arr_from) করো { \
    যদি ((arr_from).আকার_2 > 0) \
        darray_append_বস্তুসমূহ((arr_to), (arr_from).item, (arr_from).আকার_2); \
} যখন (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).আকার_2)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) করো { \
    darray_আকার_t __index = (i); \
    যদি (__index < (arr).আকার_2-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).আকার_2 -1 - __index) * আকার(*(arr).item) \
        ); \
    (arr).আকার_2--; \
} যখন(0)

/*** String buffer ***/

#define darray_append_string(arr, str) করো { \
    ধ্রুবক অক্ষর *__str = (str); \
    darray_append_বস্তুসমূহ(arr, __str, (darray_আকার_t) strlen(__str) + 1); \
    (arr).আকার_2--; \
} যখন (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_বস্তুসমূহ((arr), (string), (darray_আকার_t)strlen(string) + 1)

#define darray_append_lit(arr, stringLiteral) করো { \
    darray_append_বস্তুসমূহ(arr, stringLiteral, \
                        (darray_আকার_t) আকার(stringLiteral)); \
    (arr).আকার_2--; \
} যখন (0)

#define darray_appends_nullterminate(arr, বস্তুসমূহ, গুনতি) করো { \
    darray_আকার_t __count = (গুনতি), __oldSize = (arr).আকার_2; \
    darray_resize(arr, __oldSize + __count + 1); \
    memcpy((arr).item + __oldSize, বস্তুসমূহ, __count * আকার(*(arr).item)); \
    (arr).item[(arr).আকার_2 - 1] = 0; \
    (arr).আকার_2--; \
} যখন (0)

#define darray_prepends_nullterminate(arr, বস্তুসমূহ, গুনতি) করো { \
    darray_আকার_t __count = (গুনতি), __oldSize = (arr).আকার_2; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * আকার(*(arr).item)); \
    memcpy((arr).item, বস্তুসমূহ, __count * আকার(*(arr).item)); \
    (arr).item[(arr).আকার_2 - 1] = 0; \
    (arr).আকার_2--; \
} যখন (0)

/*** Size management ***/

#define darray_resize(arr, new_আকার) \
    darray_growalloc(arr, (arr).আকার_2 = (new_আকার))

#define darray_resize0(arr, new_আকার) করো { \
    darray_আকার_t __oldSize = (arr).আকার_2, __newSize = (new_আকার); \
    (arr).আকার_2 = __newSize; \
    যদি (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * আকার(*(arr).item)); \
    } \
} যখন (0)

#define darray_realloc(arr, newAlloc) করো { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * আকার(*(arr).item)); \
} যখন (0)

#define darray_growalloc(arr, need)   করো { \
    darray_আকার_t __need = (need); \
    যদি (__need > (arr).alloc) \
        darray_realloc(arr, darray_পরবর্তী_alloc((arr).alloc, __need, \
                                              আকার(*(arr).item))); \
} যখন (0)

#define darray_shrink(arr) করো { \
    যদি ((arr).আকার_2 > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).আকার_2) * আকার(*(arr).item)); \
} যখন (0)

#define darray_max_alloc(item_আকার) (UINT_MAX / (item_আকার))

স্থির অন্তঃরেখ darray_আকার_t
darray_পরবর্তী_alloc(darray_আকার_t alloc, darray_আকার_t need, আকার_t item_আকার)
{
    প্রতিজ্ঞা(need < darray_max_alloc(item_আকার) / 2); /* Overflow. */
    যদি (alloc == 0)
        alloc = 4;
    যখন (alloc < need)
        alloc *= 2;
    ফেরত alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    যদি ((arr).item) \
    জন্য ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).আকার_2]; (i)++)

#define darray_foreach_from(i, arr, from) \
    যদি ((arr).item) \
    জন্য ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).আকার_2]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    যদি ((arr).item) \
    জন্য ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).আকার_2; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    যদি ((arr).item) \
    জন্য ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).আকার_2; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    যদি ((arr).item && (arr).আকার_2) \
    জন্য ((i) = &(arr).item[(arr).আকার_2 - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    যদি ((arr).item && (arr).আকার_2) \
    জন্য ((idx) = (arr).আকার_2 - 1, (val) = &(arr).item[(arr).আকার_2 - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
