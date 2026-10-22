/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "utils.h"

تعريفنوع بنانشانی صحيح darray_ناپ_t;
#تعریف_کرو DARRAY_ناپ_T_چوڑائی (حجم(darray_ناپ_t) * CHAR_BIT)
#تعریف_کرو DARRAY_ناپ_MAX UINT_MAX

#تعریف_کرو darray(type) بنية {         \
    /** Current size */               \
    darray_ناپ_t ناپ;               \
    /** Count of allocated items */   \
    darray_ناپ_t alloc;              \
    /** Array of items */             \
    type *شے ATTR_COUNTED_BY(ناپ); \
}

#تعریف_کرو darray_new() { 0, 0, 0 }

#تعریف_کرو darray_init(arr) افعل { \
    (arr).شے = 0; (arr).ناپ = 0; (arr).alloc = 0; \
} طالما (0)

#تعریف_کرو darray_free(arr) افعل { \
    حرر((arr).شے); \
    darray_init(arr); \
} طالما (0)

#تعریف_کرو darray_steal(arr, منزل, منزل_ناپ) افعل { \
    *(منزل) = (arr).شے; \
    إذا (منزل_ناپ) \
        *(darray_ناپ_t *) (منزل_ناپ) = (arr).ناپ; \
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
تعريفنوع darray (نشانیوالا حرف)    darray_schar;
تعريفنوع darray (بنانشانی حرف)  darray_uchar;

تعريفنوع darray (حرف *)         darray_string;

تعريفنوع darray (چھوٹا)          darray_short;
تعريفنوع darray (صحيح)            darray_int;
تعريفنوع darray (لما)           darray_long;

تعريفنوع darray (بنانشانی چھوٹا) darray_ushort;
تعريفنوع darray (بنانشانی صحيح)   darray_uint;
تعريفنوع darray (بنانشانی لما)  darray_ulong;

/*** Access ***/

#تعریف_کرو darray_شے(arr, i)     ((arr).شے[i])
#تعریف_کرو darray_اشیاء(arr)       ((arr).شے)
#تعریف_کرو darray_ناپ(arr)        ((arr).ناپ)
#تعریف_کرو darray_خالی_تھاواں(arr)       ((arr).ناپ == 0)

#تعریف_کرو darray_آخری(arr, fallback) ( \
    !(arr).ناپ \
        ? (fallback) \
        : (arr).شے[(arr).ناپ - 1] \
)

/*** Insertion (single item) ***/

#تعریف_کرو darray_append(arr, ...)  افعل { \
    darray_resize(arr, (arr).ناپ + 1); \
    (arr).شے[(arr).ناپ - 1] = (__VA_ARGS__); \
} طالما (0)

#تعریف_کرو darray_داخل_کرو(arr, i, ...) افعل { \
    darray_ناپ_t __index = (i); \
    darray_resize(arr, (arr).ناپ+1); \
    memmove( \
        (arr).شے + __index + 1, \
        (arr).شے + __index, \
        ((arr).ناپ - __index - 1) * حجم(*(arr).شے) \
    ); \
    (arr).شے[__index] = (__VA_ARGS__); \
} طالما(0)

/*** Insertion (multiple items) ***/

#تعریف_کرو darray_append_اشیاء(arr, اشیاء, گنتی) افعل { \
    darray_ناپ_t __count = (گنتی), __oldSize = (arr).ناپ; \
    darray_resize(arr, __oldSize + __count); \
    حافظہ_نقل((arr).شے + __oldSize, اشیاء, __count * حجم(*(arr).شے)); \
} طالما (0)

#تعریف_کرو darray_from_اشیاء(arr, اشیاء, گنتی) افعل { \
    darray_ناپ_t __count = (گنتی); \
    darray_resize(arr, __count); \
    إذا (__count != 0) \
        حافظہ_نقل((arr).شے, اشیاء, __count * حجم(*(arr).شے)); \
} طالما (0)

#تعریف_کرو darray_copy(arr_منزل, arr_from) \
    darray_from_اشیاء((arr_منزل), (arr_from).شے, (arr_from).ناپ)

#تعریف_کرو darray_concat(arr_منزل, arr_from) افعل { \
    إذا ((arr_from).ناپ > 0) \
        darray_append_اشیاء((arr_منزل), (arr_from).شے, (arr_from).ناپ); \
} طالما (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#تعریف_کرو darray_remove_آخری(arr) (--(arr).ناپ)

/* Warning, slow: Requires copying all elements after removed item. */
#تعریف_کرو darray_remove(arr, i) افعل { \
    darray_ناپ_t __index = (i); \
    إذا (__index < (arr).ناپ-1) \
        memmove( \
            &(arr).شے[__index], \
            &(arr).شے[__index + 1], \
            ((arr).ناپ -1 - __index) * حجم(*(arr).شے) \
        ); \
    (arr).ناپ--; \
} طالما(0)

/*** String buffer ***/

#تعریف_کرو darray_append_string(arr, str) افعل { \
    ثابت حرف *__str = (str); \
    darray_append_اشیاء(arr, __str, (darray_ناپ_t) لڑی_لمبائی(__str) + 1); \
    (arr).ناپ--; \
} طالما (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#تعریف_کرو darray_append_string0(arr, string) \
    darray_append_اشیاء((arr), (string), (darray_ناپ_t)لڑی_لمبائی(string) + 1)

#تعریف_کرو darray_append_lit(arr, stringLiteral) افعل { \
    darray_append_اشیاء(arr, stringLiteral, \
                        (darray_ناپ_t) حجم(stringLiteral)); \
    (arr).ناپ--; \
} طالما (0)

#تعریف_کرو darray_appends_nullterminate(arr, اشیاء, گنتی) افعل { \
    darray_ناپ_t __count = (گنتی), __oldSize = (arr).ناپ; \
    darray_resize(arr, __oldSize + __count + 1); \
    حافظہ_نقل((arr).شے + __oldSize, اشیاء, __count * حجم(*(arr).شے)); \
    (arr).شے[(arr).ناپ - 1] = 0; \
    (arr).ناپ--; \
} طالما (0)

#تعریف_کرو darray_prepends_nullterminate(arr, اشیاء, گنتی) افعل { \
    darray_ناپ_t __count = (گنتی), __oldSize = (arr).ناپ; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).شے + __count, (arr).شے, \
            __oldSize * حجم(*(arr).شے)); \
    حافظہ_نقل((arr).شے, اشیاء, __count * حجم(*(arr).شے)); \
    (arr).شے[(arr).ناپ - 1] = 0; \
    (arr).ناپ--; \
} طالما (0)

/*** Size management ***/

#تعریف_کرو darray_resize(arr, new_ناپ) \
    darray_growalloc(arr, (arr).ناپ = (new_ناپ))

#تعریف_کرو darray_resize0(arr, new_ناپ) افعل { \
    darray_ناپ_t __oldSize = (arr).ناپ, __newSize = (new_ناپ); \
    (arr).ناپ = __newSize; \
    إذا (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).شے[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).شے)); \
    } \
} طالما (0)

#تعریف_کرو darray_realloc(arr, newAlloc) افعل { \
    (arr).شے = realloc((arr).شے, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).شے)); \
} طالما (0)

#تعریف_کرو darray_growalloc(arr, need)   افعل { \
    darray_ناپ_t __need = (need); \
    إذا (__need > (arr).alloc) \
        darray_realloc(arr, darray_اگلا_alloc((arr).alloc, __need, \
                                              حجم(*(arr).شے))); \
} طالما (0)

#تعریف_کرو darray_shrink(arr) افعل { \
    إذا ((arr).ناپ > 0) \
        (arr).شے = realloc((arr).شے, \
                             ((arr).alloc = (arr).ناپ) * حجم(*(arr).شے)); \
} طالما (0)

#تعریف_کرو darray_max_alloc(شے_ناپ) (UINT_MAX / (شے_ناپ))

ساكن مضمن darray_ناپ_t
darray_اگلا_alloc(darray_ناپ_t alloc, darray_ناپ_t need, ناپ_t شے_ناپ)
{
    أكد(need < darray_max_alloc(شے_ناپ) / 2); /* Overflow. */
    إذا (alloc == 0)
        alloc = 4;
    طالما (alloc < need)
        alloc *= 2;
    أرجع alloc;
}

/*** Traversal ***/

#تعریف_کرو darray_foreach(i, arr) \
    إذا ((arr).شے) \
    لكل ((i) = &(arr).شے[0]; (i) < &(arr).شے[(arr).ناپ]; (i)++)

#تعریف_کرو darray_foreach_from(i, arr, from) \
    إذا ((arr).شے) \
    لكل ((i) = &(arr).شے[from]; (i) < &(arr).شے[(arr).ناپ]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#تعریف_کرو darray_enumerate(idx, val, arr) \
    إذا ((arr).شے) \
    لكل ((idx) = 0, (val) = &(arr).شے[0]; \
         (idx) < (arr).ناپ; \
         (idx)++, (val)++)

#تعریف_کرو darray_enumerate_from(idx, val, arr, from) \
    إذا ((arr).شے) \
    لكل ((idx) = (from), (val) = &(arr).شے[from]; \
         (idx) < (arr).ناپ; \
         (idx)++, (val)++)

#تعریف_کرو darray_foreach_الٹ(i, arr) \
    إذا ((arr).شے && (arr).ناپ) \
    لكل ((i) = &(arr).شے[(arr).ناپ - 1]; \
         (i) >= &(arr).شے[0]; \
         (i)--)

#تعریف_کرو darray_enumerate_الٹ(idx, val, arr) \
    إذا ((arr).شے && (arr).ناپ) \
    لكل ((idx) = (arr).ناپ - 1, (val) = &(arr).شے[(arr).ناپ - 1]; \
         (val) >= &(arr).شے[0]; \
         (idx)--, (val)--)
