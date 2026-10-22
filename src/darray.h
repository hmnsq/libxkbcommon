/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "utils.h"

تعريفنوع بینښه صحيح darray_اندازه_t;
#تعریف_کړه DARRAY_اندازه_T_پلنوالی (حجم(darray_اندازه_t) * CHAR_BIT)
#تعریف_کړه DARRAY_اندازه_MAX UINT_MAX

#تعریف_کړه darray(type) بنية {         \
    /** Current size */               \
    darray_اندازه_t اندازه;               \
    /** Count of allocated items */   \
    darray_اندازه_t alloc;              \
    /** Array of items */             \
    type *توکی ATTR_COUNTED_BY(اندازه); \
}

#تعریف_کړه darray_new() { 0, 0, 0 }

#تعریف_کړه darray_init(arr) افعل { \
    (arr).توکی = 0; (arr).اندازه = 0; (arr).alloc = 0; \
} طالما (0)

#تعریف_کړه darray_free(arr) افعل { \
    حرر((arr).توکی); \
    darray_init(arr); \
} طالما (0)

#تعریف_کړه darray_steal(arr, منزل, منزل_اندازه) افعل { \
    *(منزل) = (arr).توکی; \
    إذا (منزل_اندازه) \
        *(darray_اندازه_t *) (منزل_اندازه) = (arr).اندازه; \
    darray_init(arr); \
} طالما (0)

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
تعريفنوع darray (نښهدار حرف)    darray_schar;
تعريفنوع darray (بینښه حرف)  darray_uchar;

تعريفنوع darray (حرف *)         darray_string;

تعريفنوع darray (لنډ)          darray_short;
تعريفنوع darray (صحيح)            darray_int;
تعريفنوع darray (اوږد)           darray_long;

تعريفنوع darray (بینښه لنډ) darray_ushort;
تعريفنوع darray (بینښه صحيح)   darray_uint;
تعريفنوع darray (بینښه اوږد)  darray_ulong;

/*** Access ***/

#تعریف_کړه darray_توکی(arr, i)     ((arr).توکی[i])
#تعریف_کړه darray_توکي(arr)       ((arr).توکی)
#تعریف_کړه darray_اندازه(arr)        ((arr).اندازه)
#تعریف_کړه darray_تشه(arr)       ((arr).اندازه == 0)

#تعریف_کړه darray_وروستی(arr, fallback) ( \
    !(arr).اندازه \
        ? (fallback) \
        : (arr).توکی[(arr).اندازه - 1] \
)

/*** Insertion (single item) ***/

#تعریف_کړه darray_append(arr, ...)  افعل { \
    darray_resize(arr, (arr).اندازه + 1); \
    (arr).توکی[(arr).اندازه - 1] = (__VA_ARGS__); \
} طالما (0)

#تعریف_کړه darray_ورننباسه(arr, i, ...) افعل { \
    darray_اندازه_t __index = (i); \
    darray_resize(arr, (arr).اندازه+1); \
    memmove( \
        (arr).توکی + __index + 1, \
        (arr).توکی + __index, \
        ((arr).اندازه - __index - 1) * حجم(*(arr).توکی) \
    ); \
    (arr).توکی[__index] = (__VA_ARGS__); \
} طالما(0)

/*** Insertion (multiple items) ***/

#تعریف_کړه darray_append_توکي(arr, توکي, شمېر) افعل { \
    darray_اندازه_t __count = (شمېر), __oldSize = (arr).اندازه; \
    darray_resize(arr, __oldSize + __count); \
    حافظه_نقل((arr).توکی + __oldSize, توکي, __count * حجم(*(arr).توکی)); \
} طالما (0)

#تعریف_کړه darray_from_توکي(arr, توکي, شمېر) افعل { \
    darray_اندازه_t __count = (شمېر); \
    darray_resize(arr, __count); \
    إذا (__count != 0) \
        حافظه_نقل((arr).توکی, توکي, __count * حجم(*(arr).توکی)); \
} طالما (0)

#تعریف_کړه darray_copy(arr_منزل, arr_from) \
    darray_from_توکي((arr_منزل), (arr_from).توکی, (arr_from).اندازه)

#تعریف_کړه darray_concat(arr_منزل, arr_from) افعل { \
    إذا ((arr_from).اندازه > 0) \
        darray_append_توکي((arr_منزل), (arr_from).توکی, (arr_from).اندازه); \
} طالما (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#تعریف_کړه darray_remove_وروستی(arr) (--(arr).اندازه)

/* Warning, slow: Requires copying all elements after removed item. */
#تعریف_کړه darray_remove(arr, i) افعل { \
    darray_اندازه_t __index = (i); \
    إذا (__index < (arr).اندازه-1) \
        memmove( \
            &(arr).توکی[__index], \
            &(arr).توکی[__index + 1], \
            ((arr).اندازه -1 - __index) * حجم(*(arr).توکی) \
        ); \
    (arr).اندازه--; \
} طالما(0)

/*** String buffer ***/

#تعریف_کړه darray_append_string(arr, str) افعل { \
    ثابت حرف *__str = (str); \
    darray_append_توکي(arr, __str, (darray_اندازه_t) مزي_اوږدوالی(__str) + 1); \
    (arr).اندازه--; \
} طالما (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#تعریف_کړه darray_append_string0(arr, string) \
    darray_append_توکي((arr), (string), (darray_اندازه_t)مزي_اوږدوالی(string) + 1)

#تعریف_کړه darray_append_lit(arr, stringLiteral) افعل { \
    darray_append_توکي(arr, stringLiteral, \
                        (darray_اندازه_t) حجم(stringLiteral)); \
    (arr).اندازه--; \
} طالما (0)

#تعریف_کړه darray_appends_nullterminate(arr, توکي, شمېر) افعل { \
    darray_اندازه_t __count = (شمېر), __oldSize = (arr).اندازه; \
    darray_resize(arr, __oldSize + __count + 1); \
    حافظه_نقل((arr).توکی + __oldSize, توکي, __count * حجم(*(arr).توکی)); \
    (arr).توکی[(arr).اندازه - 1] = 0; \
    (arr).اندازه--; \
} طالما (0)

#تعریف_کړه darray_prepends_nullterminate(arr, توکي, شمېر) افعل { \
    darray_اندازه_t __count = (شمېر), __oldSize = (arr).اندازه; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).توکی + __count, (arr).توکی, \
            __oldSize * حجم(*(arr).توکی)); \
    حافظه_نقل((arr).توکی, توکي, __count * حجم(*(arr).توکی)); \
    (arr).توکی[(arr).اندازه - 1] = 0; \
    (arr).اندازه--; \
} طالما (0)

/*** Size management ***/

#تعریف_کړه darray_resize(arr, new_اندازه) \
    darray_growalloc(arr, (arr).اندازه = (new_اندازه))

#تعریف_کړه darray_resize0(arr, new_اندازه) افعل { \
    darray_اندازه_t __oldSize = (arr).اندازه, __newSize = (new_اندازه); \
    (arr).اندازه = __newSize; \
    إذا (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).توکی[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).توکی)); \
    } \
} طالما (0)

#تعریف_کړه darray_realloc(arr, newAlloc) افعل { \
    (arr).توکی = realloc((arr).توکی, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).توکی)); \
} طالما (0)

#تعریف_کړه darray_growalloc(arr, need)   افعل { \
    darray_اندازه_t __need = (need); \
    إذا (__need > (arr).alloc) \
        darray_realloc(arr, darray_بل_alloc((arr).alloc, __need, \
                                              حجم(*(arr).توکی))); \
} طالما (0)

#تعریف_کړه darray_shrink(arr) افعل { \
    إذا ((arr).اندازه > 0) \
        (arr).توکی = realloc((arr).توکی, \
                             ((arr).alloc = (arr).اندازه) * حجم(*(arr).توکی)); \
} طالما (0)

#تعریف_کړه darray_max_alloc(توکی_اندازه) (UINT_MAX / (توکی_اندازه))

ساكن مضمن darray_اندازه_t
darray_بل_alloc(darray_اندازه_t alloc, darray_اندازه_t need, اندازه_t توکی_اندازه)
{
    أكد(need < darray_max_alloc(توکی_اندازه) / 2); /* Overflow. */
    إذا (alloc == 0)
        alloc = 4;
    طالما (alloc < need)
        alloc *= 2;
    أرجع alloc;
}

/*** Traversal ***/

#تعریف_کړه darray_foreach(i, arr) \
    إذا ((arr).توکی) \
    لكل ((i) = &(arr).توکی[0]; (i) < &(arr).توکی[(arr).اندازه]; (i)++)

#تعریف_کړه darray_foreach_from(i, arr, from) \
    إذا ((arr).توکی) \
    لكل ((i) = &(arr).توکی[from]; (i) < &(arr).توکی[(arr).اندازه]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#تعریف_کړه darray_enumerate(idx, val, arr) \
    إذا ((arr).توکی) \
    لكل ((idx) = 0, (val) = &(arr).توکی[0]; \
         (idx) < (arr).اندازه; \
         (idx)++, (val)++)

#تعریف_کړه darray_enumerate_from(idx, val, arr, from) \
    إذا ((arr).توکی) \
    لكل ((idx) = (from), (val) = &(arr).توکی[from]; \
         (idx) < (arr).اندازه; \
         (idx)++, (val)++)

#تعریف_کړه darray_foreach_سرچپه(i, arr) \
    إذا ((arr).توکی && (arr).اندازه) \
    لكل ((i) = &(arr).توکی[(arr).اندازه - 1]; \
         (i) >= &(arr).توکی[0]; \
         (i)--)

#تعریف_کړه darray_enumerate_سرچپه(idx, val, arr) \
    إذا ((arr).توکی && (arr).اندازه) \
    لكل ((idx) = (arr).اندازه - 1, (val) = &(arr).توکی[(arr).اندازه - 1]; \
         (val) >= &(arr).توکی[0]; \
         (idx)--, (val)--)
