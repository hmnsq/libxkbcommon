/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>

#अन्तर्भाव "utils.h"

प्रकारव्याख्या अचिन्हित पूर्णांक darray_size_t;
#define DARRAY_SIZE_T_WIDTH (आकार(darray_size_t) * अक्षर_बिट)
#define DARRAY_SIZE_MAX अच_पूर्णांक_उच्च

#define darray(type) संरचना {         \
    /** Current size */               \
    darray_size_t size;               \
    /** Count of allocated items */   \
    darray_size_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(size); \
}

#define darray_new() { 0, 0, 0 }

#define darray_init(arr) करा { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} पर्यंत (0)

#define darray_free(arr) करा { \
    मुक्त((arr).item); \
    darray_init(arr); \
} पर्यंत (0)

#define darray_steal(arr, to, to_size) करा { \
    *(to) = (arr).item; \
    जर (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} पर्यंत (0)

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

प्रकारव्याख्या darray (अक्षर)           darray_char;
प्रकारव्याख्या darray (चिन्हित अक्षर)    darray_schar;
प्रकारव्याख्या darray (अचिन्हित अक्षर)  darray_uchar;

प्रकारव्याख्या darray (अक्षर *)         darray_string;

प्रकारव्याख्या darray (लघु)          darray_short;
प्रकारव्याख्या darray (पूर्णांक)            darray_int;
प्रकारव्याख्या darray (दीर्घ)           darray_long;

प्रकारव्याख्या darray (अचिन्हित लघु) darray_ushort;
प्रकारव्याख्या darray (अचिन्हित पूर्णांक)   darray_uint;
प्रकारव्याख्या darray (अचिन्हित दीर्घ)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_वस्तुसंच(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  करा { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} पर्यंत (0)

#define darray_घाला(arr, i, ...) करा { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    स_हटाओ( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * आकार(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} पर्यंत(0)

/*** Insertion (multiple items) ***/

#define darray_append_वस्तुसंच(arr, वस्तुसंच, मोजणी) करा { \
    darray_size_t __count = (मोजणी), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    स_नकल((arr).item + __oldSize, वस्तुसंच, __count * आकार(*(arr).item)); \
} पर्यंत (0)

#define darray_from_वस्तुसंच(arr, वस्तुसंच, मोजणी) करा { \
    darray_size_t __count = (मोजणी); \
    darray_resize(arr, __count); \
    जर (__count != 0) \
        स_नकल((arr).item, वस्तुसंच, __count * आकार(*(arr).item)); \
} पर्यंत (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_वस्तुसंच((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) करा { \
    जर ((arr_from).size > 0) \
        darray_append_वस्तुसंच((arr_to), (arr_from).item, (arr_from).size); \
} पर्यंत (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) करा { \
    darray_size_t __index = (i); \
    जर (__index < (arr).size-1) \
        स_हटाओ( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * आकार(*(arr).item) \
        ); \
    (arr).size--; \
} पर्यंत(0)

/*** String buffer ***/

#define darray_append_string(arr, str) करा { \
    स्थिरांक अक्षर *__str = (str); \
    darray_append_वस्तुसंच(arr, __str, (darray_size_t) म_माप(__str) + 1); \
    (arr).size--; \
} पर्यंत (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_वस्तुसंच((arr), (string), (darray_size_t)म_माप(string) + 1)

#define darray_append_lit(arr, stringLiteral) करा { \
    darray_append_वस्तुसंच(arr, stringLiteral, \
                        (darray_size_t) आकार(stringLiteral)); \
    (arr).size--; \
} पर्यंत (0)

#define darray_appends_nullterminate(arr, वस्तुसंच, मोजणी) करा { \
    darray_size_t __count = (मोजणी), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    स_नकल((arr).item + __oldSize, वस्तुसंच, __count * आकार(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} पर्यंत (0)

#define darray_prepends_nullterminate(arr, वस्तुसंच, मोजणी) करा { \
    darray_size_t __count = (मोजणी), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    स_हटाओ((arr).item + __count, (arr).item, \
            __oldSize * आकार(*(arr).item)); \
    स_नकल((arr).item, वस्तुसंच, __count * आकार(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} पर्यंत (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) करा { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    जर (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        स_रखो(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * आकार(*(arr).item)); \
    } \
} पर्यंत (0)

#define darray_realloc(arr, newAlloc) करा { \
    (arr).item = पुनः_स्मृति((arr).item, \
                         ((arr).alloc = (newAlloc)) * आकार(*(arr).item)); \
} पर्यंत (0)

#define darray_growalloc(arr, need)   करा { \
    darray_size_t __need = (need); \
    जर (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              आकार(*(arr).item))); \
} पर्यंत (0)

#define darray_shrink(arr) करा { \
    जर ((arr).size > 0) \
        (arr).item = पुनः_स्मृति((arr).item, \
                             ((arr).alloc = (arr).size) * आकार(*(arr).item)); \
} पर्यंत (0)

#define darray_max_alloc(itemSize) (अच_पूर्णांक_उच्च / (itemSize))

स्थिर ओळीत darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, माप_प्रकार itemSize)
{
    प्रतिज्ञा(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    जर (alloc == 0)
        alloc = 4;
    पर्यंत (alloc < need)
        alloc *= 2;
    परत alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    जर ((arr).item) \
    साठी ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    जर ((arr).item) \
    साठी ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    जर ((arr).item) \
    साठी ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    जर ((arr).item) \
    साठी ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    जर ((arr).item && (arr).size) \
    साठी ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    जर ((arr).item && (arr).size) \
    साठी ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
