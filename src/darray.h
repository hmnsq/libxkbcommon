/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "utils.h"

تعريفنوع بیعلامت صحيح darray_اندازه_t;
#تعریف_کن DARRAY_اندازه_T_عرض (حجم(darray_اندازه_t) * CHAR_BIT)
#تعریف_کن DARRAY_اندازه_MAX UINT_MAX

#تعریف_کن darray(type) بنية {         \
    /** Current size */               \
    darray_اندازه_t اندازه;               \
    /** Count of allocated items */   \
    darray_اندازه_t alloc;              \
    /** Array of items */             \
    type *قلم ATTR_COUNTED_BY(اندازه); \
}

#تعریف_کن darray_new() { 0, 0, 0 }

#تعریف_کن darray_init(arr) افعل { \
    (arr).قلم = 0; (arr).اندازه = 0; (arr).alloc = 0; \
} طالما (0)

#تعریف_کن darray_free(arr) افعل { \
    حرر((arr).قلم); \
    darray_init(arr); \
} طالما (0)

#تعریف_کن darray_steal(arr, مقصد, مقصد_اندازه) افعل { \
    *(مقصد) = (arr).قلم; \
    إذا (مقصد_اندازه) \
        *(darray_اندازه_t *) (مقصد_اندازه) = (arr).اندازه; \
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
تعريفنوع darray (علامتدار حرف)    darray_schar;
تعريفنوع darray (بیعلامت حرف)  darray_uchar;

تعريفنوع darray (حرف *)         darray_string;

تعريفنوع darray (کوتاه)          darray_short;
تعريفنوع darray (صحيح)            darray_int;
تعريفنوع darray (دراز)           darray_long;

تعريفنوع darray (بیعلامت کوتاه) darray_ushort;
تعريفنوع darray (بیعلامت صحيح)   darray_uint;
تعريفنوع darray (بیعلامت دراز)  darray_ulong;

/*** Access ***/

#تعریف_کن darray_قلم(arr, i)     ((arr).قلم[i])
#تعریف_کن darray_اقلام(arr)       ((arr).قلم)
#تعریف_کن darray_اندازه(arr)        ((arr).اندازه)
#تعریف_کن darray_خالی(arr)       ((arr).اندازه == 0)

#تعریف_کن darray_آخرین(arr, fallback) ( \
    !(arr).اندازه \
        ? (fallback) \
        : (arr).قلم[(arr).اندازه - 1] \
)

/*** Insertion (single item) ***/

#تعریف_کن darray_append(arr, ...)  افعل { \
    darray_resize(arr, (arr).اندازه + 1); \
    (arr).قلم[(arr).اندازه - 1] = (__VA_ARGS__); \
} طالما (0)

#تعریف_کن darray_درج(arr, i, ...) افعل { \
    darray_اندازه_t __index = (i); \
    darray_resize(arr, (arr).اندازه+1); \
    memmove( \
        (arr).قلم + __index + 1, \
        (arr).قلم + __index, \
        ((arr).اندازه - __index - 1) * حجم(*(arr).قلم) \
    ); \
    (arr).قلم[__index] = (__VA_ARGS__); \
} طالما(0)

/*** Insertion (multiple items) ***/

#تعریف_کن darray_append_اقلام(arr, اقلام, شمار) افعل { \
    darray_اندازه_t __count = (شمار), __oldSize = (arr).اندازه; \
    darray_resize(arr, __oldSize + __count); \
    کپی_حافظه((arr).قلم + __oldSize, اقلام, __count * حجم(*(arr).قلم)); \
} طالما (0)

#تعریف_کن darray_from_اقلام(arr, اقلام, شمار) افعل { \
    darray_اندازه_t __count = (شمار); \
    darray_resize(arr, __count); \
    إذا (__count != 0) \
        کپی_حافظه((arr).قلم, اقلام, __count * حجم(*(arr).قلم)); \
} طالما (0)

#تعریف_کن darray_copy(arr_مقصد, arr_from) \
    darray_from_اقلام((arr_مقصد), (arr_from).قلم, (arr_from).اندازه)

#تعریف_کن darray_concat(arr_مقصد, arr_from) افعل { \
    إذا ((arr_from).اندازه > 0) \
        darray_append_اقلام((arr_مقصد), (arr_from).قلم, (arr_from).اندازه); \
} طالما (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#تعریف_کن darray_remove_آخرین(arr) (--(arr).اندازه)

/* Warning, slow: Requires copying all elements after removed item. */
#تعریف_کن darray_remove(arr, i) افعل { \
    darray_اندازه_t __index = (i); \
    إذا (__index < (arr).اندازه-1) \
        memmove( \
            &(arr).قلم[__index], \
            &(arr).قلم[__index + 1], \
            ((arr).اندازه -1 - __index) * حجم(*(arr).قلم) \
        ); \
    (arr).اندازه--; \
} طالما(0)

/*** String buffer ***/

#تعریف_کن darray_append_string(arr, str) افعل { \
    ثابت حرف *__str = (str); \
    darray_append_اقلام(arr, __str, (darray_اندازه_t) طول_رشته(__str) + 1); \
    (arr).اندازه--; \
} طالما (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#تعریف_کن darray_append_string0(arr, string) \
    darray_append_اقلام((arr), (string), (darray_اندازه_t)طول_رشته(string) + 1)

#تعریف_کن darray_append_lit(arr, stringLiteral) افعل { \
    darray_append_اقلام(arr, stringLiteral, \
                        (darray_اندازه_t) حجم(stringLiteral)); \
    (arr).اندازه--; \
} طالما (0)

#تعریف_کن darray_appends_nullterminate(arr, اقلام, شمار) افعل { \
    darray_اندازه_t __count = (شمار), __oldSize = (arr).اندازه; \
    darray_resize(arr, __oldSize + __count + 1); \
    کپی_حافظه((arr).قلم + __oldSize, اقلام, __count * حجم(*(arr).قلم)); \
    (arr).قلم[(arr).اندازه - 1] = 0; \
    (arr).اندازه--; \
} طالما (0)

#تعریف_کن darray_prepends_nullterminate(arr, اقلام, شمار) افعل { \
    darray_اندازه_t __count = (شمار), __oldSize = (arr).اندازه; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).قلم + __count, (arr).قلم, \
            __oldSize * حجم(*(arr).قلم)); \
    کپی_حافظه((arr).قلم, اقلام, __count * حجم(*(arr).قلم)); \
    (arr).قلم[(arr).اندازه - 1] = 0; \
    (arr).اندازه--; \
} طالما (0)

/*** Size management ***/

#تعریف_کن darray_resize(arr, new_اندازه) \
    darray_growalloc(arr, (arr).اندازه = (new_اندازه))

#تعریف_کن darray_resize0(arr, new_اندازه) افعل { \
    darray_اندازه_t __oldSize = (arr).اندازه, __newSize = (new_اندازه); \
    (arr).اندازه = __newSize; \
    إذا (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).قلم[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).قلم)); \
    } \
} طالما (0)

#تعریف_کن darray_realloc(arr, newAlloc) افعل { \
    (arr).قلم = realloc((arr).قلم, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).قلم)); \
} طالما (0)

#تعریف_کن darray_growalloc(arr, need)   افعل { \
    darray_اندازه_t __need = (need); \
    إذا (__need > (arr).alloc) \
        darray_realloc(arr, darray_بعدی_alloc((arr).alloc, __need, \
                                              حجم(*(arr).قلم))); \
} طالما (0)

#تعریف_کن darray_shrink(arr) افعل { \
    إذا ((arr).اندازه > 0) \
        (arr).قلم = realloc((arr).قلم, \
                             ((arr).alloc = (arr).اندازه) * حجم(*(arr).قلم)); \
} طالما (0)

#تعریف_کن darray_max_alloc(قلم_اندازه) (UINT_MAX / (قلم_اندازه))

ساكن مضمن darray_اندازه_t
darray_بعدی_alloc(darray_اندازه_t alloc, darray_اندازه_t need, اندازه_t قلم_اندازه)
{
    أكد(need < darray_max_alloc(قلم_اندازه) / 2); /* Overflow. */
    إذا (alloc == 0)
        alloc = 4;
    طالما (alloc < need)
        alloc *= 2;
    أرجع alloc;
}

/*** Traversal ***/

#تعریف_کن darray_foreach(i, arr) \
    إذا ((arr).قلم) \
    لكل ((i) = &(arr).قلم[0]; (i) < &(arr).قلم[(arr).اندازه]; (i)++)

#تعریف_کن darray_foreach_from(i, arr, from) \
    إذا ((arr).قلم) \
    لكل ((i) = &(arr).قلم[from]; (i) < &(arr).قلم[(arr).اندازه]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#تعریف_کن darray_enumerate(idx, val, arr) \
    إذا ((arr).قلم) \
    لكل ((idx) = 0, (val) = &(arr).قلم[0]; \
         (idx) < (arr).اندازه; \
         (idx)++, (val)++)

#تعریف_کن darray_enumerate_from(idx, val, arr, from) \
    إذا ((arr).قلم) \
    لكل ((idx) = (from), (val) = &(arr).قلم[from]; \
         (idx) < (arr).اندازه; \
         (idx)++, (val)++)

#تعریف_کن darray_foreach_معکوس(i, arr) \
    إذا ((arr).قلم && (arr).اندازه) \
    لكل ((i) = &(arr).قلم[(arr).اندازه - 1]; \
         (i) >= &(arr).قلم[0]; \
         (i)--)

#تعریف_کن darray_enumerate_معکوس(idx, val, arr) \
    إذا ((arr).قلم && (arr).اندازه) \
    لكل ((idx) = (arr).اندازه - 1, (val) = &(arr).قلم[(arr).اندازه - 1]; \
         (val) >= &(arr).قلم[0]; \
         (idx)--, (val)--)
