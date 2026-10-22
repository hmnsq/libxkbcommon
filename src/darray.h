/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <assert.h>
#ضمن <limits.h>

#ضمن "utils.h"

تعريفنوع بنانشان صحيح darray_سائيز_t;
#وصف_ڪريو DARRAY_سائيز_T_ويڪر (حجم(darray_سائيز_t) * CHAR_BIT)
#وصف_ڪريو DARRAY_سائيز_MAX UINT_MAX

#وصف_ڪريو darray(type) بنية {         \
    /** Current size */               \
    darray_سائيز_t سائيز;               \
    /** Count of allocated items */   \
    darray_سائيز_t alloc;              \
    /** Array of items */             \
    type *شي ATTR_COUNTED_BY(سائيز); \
}

#وصف_ڪريو darray_new() { 0, 0, 0 }

#وصف_ڪريو darray_init(arr) افعل { \
    (arr).شي = 0; (arr).سائيز = 0; (arr).alloc = 0; \
} طالما (0)

#وصف_ڪريو darray_free(arr) افعل { \
    حرر((arr).شي); \
    darray_init(arr); \
} طالما (0)

#وصف_ڪريو darray_steal(arr, منزل, منزل_سائيز) افعل { \
    *(منزل) = (arr).شي; \
    إذا (منزل_سائيز) \
        *(darray_سائيز_t *) (منزل_سائيز) = (arr).سائيز; \
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
تعريفنوع darray (نشانوارو حرف)    darray_schar;
تعريفنوع darray (بنانشان حرف)  darray_uchar;

تعريفنوع darray (حرف *)         darray_string;

تعريفنوع darray (ننڍو)          darray_short;
تعريفنوع darray (صحيح)            darray_int;
تعريفنوع darray (ڊگهو)           darray_long;

تعريفنوع darray (بنانشان ننڍو) darray_ushort;
تعريفنوع darray (بنانشان صحيح)   darray_uint;
تعريفنوع darray (بنانشان ڊگهو)  darray_ulong;

/*** Access ***/

#وصف_ڪريو darray_شي(arr, i)     ((arr).شي[i])
#وصف_ڪريو darray_شيون(arr)       ((arr).شي)
#وصف_ڪريو darray_سائيز(arr)        ((arr).سائيز)
#وصف_ڪريو darray_خالي_جايون(arr)       ((arr).سائيز == 0)

#وصف_ڪريو darray_آخري(arr, fallback) ( \
    !(arr).سائيز \
        ? (fallback) \
        : (arr).شي[(arr).سائيز - 1] \
)

/*** Insertion (single item) ***/

#وصف_ڪريو darray_append(arr, ...)  افعل { \
    darray_resize(arr, (arr).سائيز + 1); \
    (arr).شي[(arr).سائيز - 1] = (__VA_ARGS__); \
} طالما (0)

#وصف_ڪريو darray_وجهو(arr, i, ...) افعل { \
    darray_سائيز_t __index = (i); \
    darray_resize(arr, (arr).سائيز+1); \
    memmove( \
        (arr).شي + __index + 1, \
        (arr).شي + __index, \
        ((arr).سائيز - __index - 1) * حجم(*(arr).شي) \
    ); \
    (arr).شي[__index] = (__VA_ARGS__); \
} طالما(0)

/*** Insertion (multiple items) ***/

#وصف_ڪريو darray_append_شيون(arr, شيون, ڳڻپ) افعل { \
    darray_سائيز_t __count = (ڳڻپ), __oldSize = (arr).سائيز; \
    darray_resize(arr, __oldSize + __count); \
    يادگيري_نقل((arr).شي + __oldSize, شيون, __count * حجم(*(arr).شي)); \
} طالما (0)

#وصف_ڪريو darray_from_شيون(arr, شيون, ڳڻپ) افعل { \
    darray_سائيز_t __count = (ڳڻپ); \
    darray_resize(arr, __count); \
    إذا (__count != 0) \
        يادگيري_نقل((arr).شي, شيون, __count * حجم(*(arr).شي)); \
} طالما (0)

#وصف_ڪريو darray_copy(arr_منزل, arr_from) \
    darray_from_شيون((arr_منزل), (arr_from).شي, (arr_from).سائيز)

#وصف_ڪريو darray_concat(arr_منزل, arr_from) افعل { \
    إذا ((arr_from).سائيز > 0) \
        darray_append_شيون((arr_منزل), (arr_from).شي, (arr_from).سائيز); \
} طالما (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#وصف_ڪريو darray_remove_آخري(arr) (--(arr).سائيز)

/* Warning, slow: Requires copying all elements after removed item. */
#وصف_ڪريو darray_remove(arr, i) افعل { \
    darray_سائيز_t __index = (i); \
    إذا (__index < (arr).سائيز-1) \
        memmove( \
            &(arr).شي[__index], \
            &(arr).شي[__index + 1], \
            ((arr).سائيز -1 - __index) * حجم(*(arr).شي) \
        ); \
    (arr).سائيز--; \
} طالما(0)

/*** String buffer ***/

#وصف_ڪريو darray_append_string(arr, str) افعل { \
    ثابت حرف *__str = (str); \
    darray_append_شيون(arr, __str, (darray_سائيز_t) لڙي_ڊيگهه(__str) + 1); \
    (arr).سائيز--; \
} طالما (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#وصف_ڪريو darray_append_string0(arr, string) \
    darray_append_شيون((arr), (string), (darray_سائيز_t)لڙي_ڊيگهه(string) + 1)

#وصف_ڪريو darray_append_lit(arr, stringLiteral) افعل { \
    darray_append_شيون(arr, stringLiteral, \
                        (darray_سائيز_t) حجم(stringLiteral)); \
    (arr).سائيز--; \
} طالما (0)

#وصف_ڪريو darray_appends_nullterminate(arr, شيون, ڳڻپ) افعل { \
    darray_سائيز_t __count = (ڳڻپ), __oldSize = (arr).سائيز; \
    darray_resize(arr, __oldSize + __count + 1); \
    يادگيري_نقل((arr).شي + __oldSize, شيون, __count * حجم(*(arr).شي)); \
    (arr).شي[(arr).سائيز - 1] = 0; \
    (arr).سائيز--; \
} طالما (0)

#وصف_ڪريو darray_prepends_nullterminate(arr, شيون, ڳڻپ) افعل { \
    darray_سائيز_t __count = (ڳڻپ), __oldSize = (arr).سائيز; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).شي + __count, (arr).شي, \
            __oldSize * حجم(*(arr).شي)); \
    يادگيري_نقل((arr).شي, شيون, __count * حجم(*(arr).شي)); \
    (arr).شي[(arr).سائيز - 1] = 0; \
    (arr).سائيز--; \
} طالما (0)

/*** Size management ***/

#وصف_ڪريو darray_resize(arr, new_سائيز) \
    darray_growalloc(arr, (arr).سائيز = (new_سائيز))

#وصف_ڪريو darray_resize0(arr, new_سائيز) افعل { \
    darray_سائيز_t __oldSize = (arr).سائيز, __newSize = (new_سائيز); \
    (arr).سائيز = __newSize; \
    إذا (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).شي[__oldSize], 0, \
               (__newSize - __oldSize) * حجم(*(arr).شي)); \
    } \
} طالما (0)

#وصف_ڪريو darray_realloc(arr, newAlloc) افعل { \
    (arr).شي = realloc((arr).شي, \
                         ((arr).alloc = (newAlloc)) * حجم(*(arr).شي)); \
} طالما (0)

#وصف_ڪريو darray_growalloc(arr, need)   افعل { \
    darray_سائيز_t __need = (need); \
    إذا (__need > (arr).alloc) \
        darray_realloc(arr, darray_ايندڙ_alloc((arr).alloc, __need, \
                                              حجم(*(arr).شي))); \
} طالما (0)

#وصف_ڪريو darray_shrink(arr) افعل { \
    إذا ((arr).سائيز > 0) \
        (arr).شي = realloc((arr).شي, \
                             ((arr).alloc = (arr).سائيز) * حجم(*(arr).شي)); \
} طالما (0)

#وصف_ڪريو darray_max_alloc(شي_سائيز) (UINT_MAX / (شي_سائيز))

ساكن مضمن darray_سائيز_t
darray_ايندڙ_alloc(darray_سائيز_t alloc, darray_سائيز_t need, سائيز_t شي_سائيز)
{
    أكد(need < darray_max_alloc(شي_سائيز) / 2); /* Overflow. */
    إذا (alloc == 0)
        alloc = 4;
    طالما (alloc < need)
        alloc *= 2;
    أرجع alloc;
}

/*** Traversal ***/

#وصف_ڪريو darray_foreach(i, arr) \
    إذا ((arr).شي) \
    لكل ((i) = &(arr).شي[0]; (i) < &(arr).شي[(arr).سائيز]; (i)++)

#وصف_ڪريو darray_foreach_from(i, arr, from) \
    إذا ((arr).شي) \
    لكل ((i) = &(arr).شي[from]; (i) < &(arr).شي[(arr).سائيز]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#وصف_ڪريو darray_enumerate(idx, val, arr) \
    إذا ((arr).شي) \
    لكل ((idx) = 0, (val) = &(arr).شي[0]; \
         (idx) < (arr).سائيز; \
         (idx)++, (val)++)

#وصف_ڪريو darray_enumerate_from(idx, val, arr, from) \
    إذا ((arr).شي) \
    لكل ((idx) = (from), (val) = &(arr).شي[from]; \
         (idx) < (arr).سائيز; \
         (idx)++, (val)++)

#وصف_ڪريو darray_foreach_ابتو(i, arr) \
    إذا ((arr).شي && (arr).سائيز) \
    لكل ((i) = &(arr).شي[(arr).سائيز - 1]; \
         (i) >= &(arr).شي[0]; \
         (i)--)

#وصف_ڪريو darray_enumerate_ابتو(idx, val, arr) \
    إذا ((arr).شي && (arr).سائيز) \
    لكل ((idx) = (arr).سائيز - 1, (val) = &(arr).شي[(arr).سائيز - 1]; \
         (val) >= &(arr).شي[0]; \
         (idx)--, (val)--)
