/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#شامل <stdlib.h>
#شامل <string.h>
#شامل <assert.h>
#شامل <limits.h>

#شامل "utils.h"

قسمتعریف غیرعلامتی صحیح darray_سائز_t;
#تعریف_کرو DARRAY_سائز_T_چوڑائی (حجم(darray_سائز_t) * CHAR_BIT)
#تعریف_کرو DARRAY_سائز_MAX UINT_MAX

#تعریف_کرو darray(type) ساخت {         \
    /** Current size */               \
    darray_سائز_t سائز;               \
    /** Count of allocated items */   \
    darray_سائز_t alloc;              \
    /** Array of items */             \
    type *شے ATTR_COUNTED_BY(سائز); \
}

#تعریف_کرو darray_new() { 0, 0, 0 }

#تعریف_کرو darray_init(arr) کرو { \
    (arr).شے = 0; (arr).سائز = 0; (arr).alloc = 0; \
} جب (0)

#تعریف_کرو darray_free(arr) کرو { \
    آزاد((arr).شے); \
    darray_init(arr); \
} جب (0)

#تعریف_کرو darray_steal(arr, مقصد, مقصد_سائز) کرو { \
    *(مقصد) = (arr).شے; \
    اگر (مقصد_سائز) \
        *(darray_سائز_t *) (مقصد_سائز) = (arr).سائز; \
    darray_init(arr); \
} جب (0)

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

قسمتعریف darray (حرف)           darray_char;
قسمتعریف darray (علامتی حرف)    darray_schar;
قسمتعریف darray (غیرعلامتی حرف)  darray_uchar;

قسمتعریف darray (حرف *)         darray_string;

قسمتعریف darray (مختصر)          darray_short;
قسمتعریف darray (صحیح)            darray_int;
قسمتعریف darray (طویل)           darray_long;

قسمتعریف darray (غیرعلامتی مختصر) darray_ushort;
قسمتعریف darray (غیرعلامتی صحیح)   darray_uint;
قسمتعریف darray (غیرعلامتی طویل)  darray_ulong;

/*** Access ***/

#تعریف_کرو darray_شے(arr, i)     ((arr).شے[i])
#تعریف_کرو darray_اشیاء(arr)       ((arr).شے)
#تعریف_کرو darray_سائز(arr)        ((arr).سائز)
#تعریف_کرو darray_خالی_خانے(arr)       ((arr).سائز == 0)

#تعریف_کرو darray_آخری(arr, fallback) ( \
    !(arr).سائز \
        ? (fallback) \
        : (arr).شے[(arr).سائز - 1] \
)

/*** Insertion (single item) ***/

#تعریف_کرو darray_append(arr, ...)  کرو { \
    darray_resize(arr, (arr).سائز + 1); \
    (arr).شے[(arr).سائز - 1] = (__VA_ARGS__); \
} جب (0)

#تعریف_کرو darray_داخل_کرو(arr, i, ...) کرو { \
    darray_سائز_t __index = (i); \
    darray_resize(arr, (arr).سائز+1); \
    memmove( \
        (arr).شے + __index + 1, \
        (arr).شے + __index, \
        ((arr).سائز - __index - 1) * حجم(*(arr).شے) \
    ); \
    (arr).شے[__index] = (__VA_ARGS__); \
} جب(0)

/*** Insertion (multiple items) ***/

#تعریف_کرو darray_append_اشیاء(arr, اشیاء, گنتی) کرو { \
    darray_سائز_t __count = (گنتی), __oldSize = (arr).سائز; \
    darray_resize(arr, __oldSize + __count); \
    حافظہ_نقل((arr).شے + __oldSize, اشیاء, __count * حجم(*(arr).شے)); \
} جب (0)

#تعریف_کرو darray_from_اشیاء(arr, اشیاء, گنتی) کرو { \
    darray_سائز_t __count = (گنتی); \
    darray_resize(arr, __count); \
    اگر (__count != 0) \
        حافظہ_نقل((arr).شے, اشیاء, __count * حجم(*(arr).شے)); \
} جب (0)

#تعریف_کرو darray_copy(arr_مقصد, arr_from) \
    darray_from_اشیاء((arr_مقصد), (arr_from).شے, (arr_from).سائز)

#تعریف_کرو darray_concat(arr_مقصد, arr_from) کرو { \
    اگر ((arr_from).سائز > 0) \
        darray_append_اشیاء((arr_مقصد), (arr_from).شے, (arr_from).سائز); \
} جب (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#تعریف_کرو darray_remove_آخری(arr) (--(arr).سائز)

/* Warning, slow: Requires copying all elements after removed item. */
#تعریف_کرو darray_remove(arr, i) کرو { \
    darray_سائز_t __index = (i); \
    اگر (__index < (arr).سائز-1) \
        memmove( \
            &(arr).شے[__index], \
            &(arr).شے[__index + 1], \
            ((arr).سائز -1 - __index) * حجم(*(arr).شے) \
        ); \
    (arr).سائز--; \
} جب(0)

/*** String buffer ***/

#تعریف_کرو darray_append_string(arr, str) کرو { \
    ثابت حرف *__str = (str); \
    darray_append_اشیاء(arr, __str, (darray_سائز_t) لڑی_لمبائی(__str) + 1); \
    (arr).سائز--; \
} جب (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#تعریف_کرو darray_append_string0(arr, string) \
    darray_append_اشیاء((arr), (string), (darray_سائز_t)لڑی_لمبائی(string) + 1)

#تعریف_کرو darray_append_lit(arr, stringLiteral) کرو { \
    darray_append_اشیاء(arr, stringLiteral, \
                        (darray_سائز_t) حجم(stringLiteral)); \
    (arr).سائز--; \
} جب (0)

#تعریف_کرو darray_appends_nullterminate(arr, اشیاء, گنتی) کرو { \
    darray_سائز_t __count = (گنتی), __oldSize = (arr).سائز; \
    darray_resize(arr, __oldSize + __count + 1); \
    حافظہ_نقل((arr).شے + __oldSize, اشیاء, __count * حجم(*(arr).شے)); \
    (arr).شے[(arr).سائز - 1] = 0; \
    (arr).سائز--; \
} جب (0)

#تعریف_کرو darray_prepends_nullterminate(arr, اشیاء, گنتی) کرو { \
    darray_سائز_t __count = (گنتی), __oldSize = (arr).سائز; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).شے + __count, (arr).شے, \
            __oldSize * حجم(*(arr).شے)); \
    حافظہ_نقل((arr).شے, اشیاء, __count * حجم(*(arr).شے)); \
    (arr).شے[(arr).سائز - 1] = 0; \
    (arr).سائز--; \
} جب (0)

/*** Size management ***/

#تعریف_کرو darray_resize(arr, new_سائز) \
    darray_growalloc(arr, (arr).سائز = (new_سائز))

#تعریف_کرو darray_resize0(arr, new_سائز) کرو { \
    darray_سائز_t __oldSize = (arr).سائز, __newSize = (new_سائز); \
    (arr).سائز = __newSize; \
    اگر (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).شے[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).شے)); \
    } \
} جب (0)

#تعریف_کرو darray_realloc(arr, newAlloc) کرو { \
    (arr).شے = realloc((arr).شے, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).شے)); \
} جب (0)

#تعریف_کرو darray_growalloc(arr, need)   کرو { \
    darray_سائز_t __need = (need); \
    اگر (__need > (arr).alloc) \
        darray_realloc(arr, darray_اگلا_alloc((arr).alloc, __need, \
                                              حجم(*(arr).شے))); \
} جب (0)

#تعریف_کرو darray_shrink(arr) کرو { \
    اگر ((arr).سائز > 0) \
        (arr).شے = realloc((arr).شے, \
                             ((arr).alloc = (arr).سائز) * حجم(*(arr).شے)); \
} جب (0)

#تعریف_کرو darray_max_alloc(شے_سائز) (UINT_MAX / (شے_سائز))

ساکن اندرونی darray_سائز_t
darray_اگلا_alloc(darray_سائز_t alloc, darray_سائز_t need, سائز_t شے_سائز)
{
    أكد(need < darray_max_alloc(شے_سائز) / 2); /* Overflow. */
    اگر (alloc == 0)
        alloc = 4;
    جب (alloc < need)
        alloc *= 2;
    واپس alloc;
}

/*** Traversal ***/

#تعریف_کرو darray_foreach(i, arr) \
    اگر ((arr).شے) \
    برائے ((i) = &(arr).شے[0]; (i) < &(arr).شے[(arr).سائز]; (i)++)

#تعریف_کرو darray_foreach_from(i, arr, from) \
    اگر ((arr).شے) \
    برائے ((i) = &(arr).شے[from]; (i) < &(arr).شے[(arr).سائز]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#تعریف_کرو darray_enumerate(idx, val, arr) \
    اگر ((arr).شے) \
    برائے ((idx) = 0, (val) = &(arr).شے[0]; \
         (idx) < (arr).سائز; \
         (idx)++, (val)++)

#تعریف_کرو darray_enumerate_from(idx, val, arr, from) \
    اگر ((arr).شے) \
    برائے ((idx) = (from), (val) = &(arr).شے[from]; \
         (idx) < (arr).سائز; \
         (idx)++, (val)++)

#تعریف_کرو darray_foreach_الٹ(i, arr) \
    اگر ((arr).شے && (arr).سائز) \
    برائے ((i) = &(arr).شے[(arr).سائز - 1]; \
         (i) >= &(arr).شے[0]; \
         (i)--)

#تعریف_کرو darray_enumerate_الٹ(idx, val, arr) \
    اگر ((arr).شے && (arr).سائز) \
    برائے ((idx) = (arr).سائز - 1, (val) = &(arr).شے[(arr).سائز - 1]; \
         (val) >= &(arr).شے[0]; \
         (idx)--, (val)--)
