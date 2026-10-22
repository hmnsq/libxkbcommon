/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "utils.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ darray_ಗಾತ್ರ_t;
#ವ್ಯಾಖ್ಯಾನಿಸು DARRAY_ಗಾತ್ರ_T_ಅಗಲ (ಗಾತ್ರ(darray_ಗಾತ್ರ_t) * CHAR_BIT)
#ವ್ಯಾಖ್ಯಾನಿಸು DARRAY_ಗಾತ್ರ_MAX UINT_MAX

#ವ್ಯಾಖ್ಯಾನಿಸು darray(type) ರಚನೆ {         \
    /** Current size */               \
    darray_ಗಾತ್ರ_t ಗಾತ್ರ_2;               \
    /** Count of allocated items */   \
    darray_ಗಾತ್ರ_t alloc;              \
    /** Array of items */             \
    type *ಅಂಶ ATTR_COUNTED_BY(ಗಾತ್ರ_2); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು darray_new() { 0, 0, 0 }

#ವ್ಯಾಖ್ಯಾನಿಸು darray_init(arr) ಮಾಡು { \
    (arr).ಅಂಶ = 0; (arr).ಗಾತ್ರ_2 = 0; (arr).alloc = 0; \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_free(arr) ಮಾಡು { \
    ಬಿಡುಗಡೆ((arr).ಅಂಶ); \
    darray_init(arr); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_steal(arr, ಗಮ್ಯ, ಗಮ್ಯ_ಗಾತ್ರ) ಮಾಡು { \
    *(ಗಮ್ಯ) = (arr).ಅಂಶ; \
    ಆದರೆ (ಗಮ್ಯ_ಗಾತ್ರ) \
        *(darray_ಗಾತ್ರ_t *) (ಗಮ್ಯ_ಗಾತ್ರ) = (arr).ಗಾತ್ರ_2; \
    darray_init(arr); \
} ಆಗ (0)

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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಅಕ್ಷರ)           darray_char;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)    darray_schar;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)  darray_uchar;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಅಕ್ಷರ *)         darray_string;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಗಿಡ್ಡ)          darray_short;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಪೂರ್ಣಾಂಕ)            darray_int;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ದೀರ್ಘ)           darray_long;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ) darray_ushort;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)   darray_uint;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)  darray_ulong;

/*** Access ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_ಅಂಶ(arr, i)     ((arr).ಅಂಶ[i])
#ವ್ಯಾಖ್ಯಾನಿಸು darray_ವಸ್ತುಗಳು(arr)       ((arr).ಅಂಶ)
#ವ್ಯಾಖ್ಯಾನಿಸು darray_ಗಾತ್ರ(arr)        ((arr).ಗಾತ್ರ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು darray_ಖಾಲಿಗಳು(arr)       ((arr).ಗಾತ್ರ_2 == 0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_ಕೊನೆಯ(arr, fallback) ( \
    !(arr).ಗಾತ್ರ_2 \
        ? (fallback) \
        : (arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1] \
)

/*** Insertion (single item) ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_append(arr, ...)  ಮಾಡು { \
    darray_resize(arr, (arr).ಗಾತ್ರ_2 + 1); \
    (arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1] = (__VA_ARGS__); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_ಸೇರಿಸು(arr, i, ...) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __index = (i); \
    darray_resize(arr, (arr).ಗಾತ್ರ_2+1); \
    memmove( \
        (arr).ಅಂಶ + __index + 1, \
        (arr).ಅಂಶ + __index, \
        ((arr).ಗಾತ್ರ_2 - __index - 1) * ಗಾತ್ರ(*(arr).ಅಂಶ) \
    ); \
    (arr).ಅಂಶ[__index] = (__VA_ARGS__); \
} ಆಗ(0)

/*** Insertion (multiple items) ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_append_ವಸ್ತುಗಳು(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ_2) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __count = (ಎಣಿಕೆ_2), __oldSize = (arr).ಗಾತ್ರ_2; \
    darray_resize(arr, __oldSize + __count); \
    ಸ್ಮರಣೆ_ನಕಲು((arr).ಅಂಶ + __oldSize, ವಸ್ತುಗಳು, __count * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_from_ವಸ್ತುಗಳು(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ_2) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __count = (ಎಣಿಕೆ_2); \
    darray_resize(arr, __count); \
    ಆದರೆ (__count != 0) \
        ಸ್ಮರಣೆ_ನಕಲು((arr).ಅಂಶ, ವಸ್ತುಗಳು, __count * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_copy(arr_ಗಮ್ಯ, arr_from) \
    darray_from_ವಸ್ತುಗಳು((arr_ಗಮ್ಯ), (arr_from).ಅಂಶ, (arr_from).ಗಾತ್ರ_2)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_concat(arr_ಗಮ್ಯ, arr_from) ಮಾಡು { \
    ಆದರೆ ((arr_from).ಗಾತ್ರ_2 > 0) \
        darray_append_ವಸ್ತುಗಳು((arr_ಗಮ್ಯ), (arr_from).ಅಂಶ, (arr_from).ಗಾತ್ರ_2); \
} ಆಗ (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#ವ್ಯಾಖ್ಯಾನಿಸು darray_remove_ಕೊನೆಯ(arr) (--(arr).ಗಾತ್ರ_2)

/* Warning, slow: Requires copying all elements after removed item. */
#ವ್ಯಾಖ್ಯಾನಿಸು darray_remove(arr, i) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __index = (i); \
    ಆದರೆ (__index < (arr).ಗಾತ್ರ_2-1) \
        memmove( \
            &(arr).ಅಂಶ[__index], \
            &(arr).ಅಂಶ[__index + 1], \
            ((arr).ಗಾತ್ರ_2 -1 - __index) * ಗಾತ್ರ(*(arr).ಅಂಶ) \
        ); \
    (arr).ಗಾತ್ರ_2--; \
} ಆಗ(0)

/*** String buffer ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_append_string(arr, str) ಮಾಡು { \
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *__str = (str); \
    darray_append_ವಸ್ತುಗಳು(arr, __str, (darray_ಗಾತ್ರ_t) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(__str) + 1); \
    (arr).ಗಾತ್ರ_2--; \
} ಆಗ (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#ವ್ಯಾಖ್ಯಾನಿಸು darray_append_string0(arr, string) \
    darray_append_ವಸ್ತುಗಳು((arr), (string), (darray_ಗಾತ್ರ_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string) + 1)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_append_lit(arr, stringLiteral) ಮಾಡು { \
    darray_append_ವಸ್ತುಗಳು(arr, stringLiteral, \
                        (darray_ಗಾತ್ರ_t) ಗಾತ್ರ(stringLiteral)); \
    (arr).ಗಾತ್ರ_2--; \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_appends_nullterminate(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ_2) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __count = (ಎಣಿಕೆ_2), __oldSize = (arr).ಗಾತ್ರ_2; \
    darray_resize(arr, __oldSize + __count + 1); \
    ಸ್ಮರಣೆ_ನಕಲು((arr).ಅಂಶ + __oldSize, ವಸ್ತುಗಳು, __count * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
    (arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1] = 0; \
    (arr).ಗಾತ್ರ_2--; \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_prepends_nullterminate(arr, ವಸ್ತುಗಳು, ಎಣಿಕೆ_2) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __count = (ಎಣಿಕೆ_2), __oldSize = (arr).ಗಾತ್ರ_2; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).ಅಂಶ + __count, (arr).ಅಂಶ, \
            __oldSize * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
    ಸ್ಮರಣೆ_ನಕಲು((arr).ಅಂಶ, ವಸ್ತುಗಳು, __count * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
    (arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1] = 0; \
    (arr).ಗಾತ್ರ_2--; \
} ಆಗ (0)

/*** Size management ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_resize(arr, new_ಗಾತ್ರ) \
    darray_growalloc(arr, (arr).ಗಾತ್ರ_2 = (new_ಗಾತ್ರ))

#ವ್ಯಾಖ್ಯಾನಿಸು darray_resize0(arr, new_ಗಾತ್ರ) ಮಾಡು { \
    darray_ಗಾತ್ರ_t __oldSize = (arr).ಗಾತ್ರ_2, __newSize = (new_ಗಾತ್ರ); \
    (arr).ಗಾತ್ರ_2 = __newSize; \
    ಆದರೆ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).ಅಂಶ[__oldSize], 0, \
               (__newSize - __oldSize) * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
    } \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_realloc(arr, newAlloc) ಮಾಡು { \
    (arr).ಅಂಶ = realloc((arr).ಅಂಶ, \
                         ((arr).alloc = (newAlloc)) * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_growalloc(arr, need)   ಮಾಡು { \
    darray_ಗಾತ್ರ_t __need = (need); \
    ಆದರೆ (__need > (arr).alloc) \
        darray_realloc(arr, darray_ಮುಂದಿನ_alloc((arr).alloc, __need, \
                                              ಗಾತ್ರ(*(arr).ಅಂಶ))); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_shrink(arr) ಮಾಡು { \
    ಆದರೆ ((arr).ಗಾತ್ರ_2 > 0) \
        (arr).ಅಂಶ = realloc((arr).ಅಂಶ, \
                             ((arr).alloc = (arr).ಗಾತ್ರ_2) * ಗಾತ್ರ(*(arr).ಅಂಶ)); \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_max_alloc(ಅಂಶ_ಗಾತ್ರ) (UINT_MAX / (ಅಂಶ_ಗಾತ್ರ))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ darray_ಗಾತ್ರ_t
darray_ಮುಂದಿನ_alloc(darray_ಗಾತ್ರ_t alloc, darray_ಗಾತ್ರ_t need, ಗಾತ್ರ_t ಅಂಶ_ಗಾತ್ರ)
{
    ಪ್ರತಿಜ್ಞಾ(need < darray_max_alloc(ಅಂಶ_ಗಾತ್ರ) / 2); /* Overflow. */
    ಆದರೆ (alloc == 0)
        alloc = 4;
    ಆಗ (alloc < need)
        alloc *= 2;
    ಹಿಂತಿರುಗಿ alloc;
}

/*** Traversal ***/

#ವ್ಯಾಖ್ಯಾನಿಸು darray_foreach(i, arr) \
    ಆದರೆ ((arr).ಅಂಶ) \
    ಗಾಗಿ ((i) = &(arr).ಅಂಶ[0]; (i) < &(arr).ಅಂಶ[(arr).ಗಾತ್ರ_2]; (i)++)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_foreach_from(i, arr, from) \
    ಆದರೆ ((arr).ಅಂಶ) \
    ಗಾಗಿ ((i) = &(arr).ಅಂಶ[from]; (i) < &(arr).ಅಂಶ[(arr).ಗಾತ್ರ_2]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#ವ್ಯಾಖ್ಯಾನಿಸು darray_enumerate(idx, val, arr) \
    ಆದರೆ ((arr).ಅಂಶ) \
    ಗಾಗಿ ((idx) = 0, (val) = &(arr).ಅಂಶ[0]; \
         (idx) < (arr).ಗಾತ್ರ_2; \
         (idx)++, (val)++)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_enumerate_from(idx, val, arr, from) \
    ಆದರೆ ((arr).ಅಂಶ) \
    ಗಾಗಿ ((idx) = (from), (val) = &(arr).ಅಂಶ[from]; \
         (idx) < (arr).ಗಾತ್ರ_2; \
         (idx)++, (val)++)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_foreach_ಹಿಮ್ಮುಖ(i, arr) \
    ಆದರೆ ((arr).ಅಂಶ && (arr).ಗಾತ್ರ_2) \
    ಗಾಗಿ ((i) = &(arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1]; \
         (i) >= &(arr).ಅಂಶ[0]; \
         (i)--)

#ವ್ಯಾಖ್ಯಾನಿಸು darray_enumerate_ಹಿಮ್ಮುಖ(idx, val, arr) \
    ಆದರೆ ((arr).ಅಂಶ && (arr).ಗಾತ್ರ_2) \
    ಗಾಗಿ ((idx) = (arr).ಗಾತ್ರ_2 - 1, (val) = &(arr).ಅಂಶ[(arr).ಗಾತ್ರ_2 - 1]; \
         (val) >= &(arr).ಅಂಶ[0]; \
         (idx)--, (val)--)
