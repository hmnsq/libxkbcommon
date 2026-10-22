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

प्रकारनिर्देश अचिन्हित पूर्णाङ्क darray_size_t;
#define DARRAY_SIZE_T_WIDTH (परिमाण(darray_size_t) * अक्षर_बिट)
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

#define darray_init(arr) गर { \
    (arr).item = 0; (arr).size = 0; (arr).alloc = 0; \
} जबसम्म (0)

#define darray_free(arr) गर { \
    मुक्त((arr).item); \
    darray_init(arr); \
} जबसम्म (0)

#define darray_steal(arr, to, to_size) गर { \
    *(to) = (arr).item; \
    यदि (to_size) \
        *(darray_size_t *) (to_size) = (arr).size; \
    darray_init(arr); \
} जबसम्म (0)

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

प्रकारनिर्देश darray (वर्ण)           darray_char;
प्रकारनिर्देश darray (चिन्हित वर्ण)    darray_schar;
प्रकारनिर्देश darray (अचिन्हित वर्ण)  darray_uchar;

प्रकारनिर्देश darray (वर्ण *)         darray_string;

प्रकारनिर्देश darray (लघु)          darray_short;
प्रकारनिर्देश darray (पूर्णाङ्क)            darray_int;
प्रकारनिर्देश darray (दीर्घ)           darray_long;

प्रकारनिर्देश darray (अचिन्हित लघु) darray_ushort;
प्रकारनिर्देश darray (अचिन्हित पूर्णाङ्क)   darray_uint;
प्रकारनिर्देश darray (अचिन्हित दीर्घ)  darray_ulong;

/*** Access ***/

#define darray_item(arr, i)     ((arr).item[i])
#define darray_वस्तुहरू(arr)       ((arr).item)
#define darray_size(arr)        ((arr).size)
#define darray_empty(arr)       ((arr).size == 0)

#define darray_last(arr, fallback) ( \
    !(arr).size \
        ? (fallback) \
        : (arr).item[(arr).size - 1] \
)

/*** Insertion (single item) ***/

#define darray_append(arr, ...)  गर { \
    darray_resize(arr, (arr).size + 1); \
    (arr).item[(arr).size - 1] = (__VA_ARGS__); \
} जबसम्म (0)

#define darray_घुसाउ(arr, i, ...) गर { \
    darray_size_t __index = (i); \
    darray_resize(arr, (arr).size+1); \
    स_हटाओ( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).size - __index - 1) * परिमाण(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} जबसम्म(0)

/*** Insertion (multiple items) ***/

#define darray_append_वस्तुहरू(arr, वस्तुहरू, गन्ती) गर { \
    darray_size_t __count = (गन्ती), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count); \
    स_नकल((arr).item + __oldSize, वस्तुहरू, __count * परिमाण(*(arr).item)); \
} जबसम्म (0)

#define darray_from_वस्तुहरू(arr, वस्तुहरू, गन्ती) गर { \
    darray_size_t __count = (गन्ती); \
    darray_resize(arr, __count); \
    यदि (__count != 0) \
        स_नकल((arr).item, वस्तुहरू, __count * परिमाण(*(arr).item)); \
} जबसम्म (0)

#define darray_copy(arr_to, arr_from) \
    darray_from_वस्तुहरू((arr_to), (arr_from).item, (arr_from).size)

#define darray_concat(arr_to, arr_from) गर { \
    यदि ((arr_from).size > 0) \
        darray_append_वस्तुहरू((arr_to), (arr_from).item, (arr_from).size); \
} जबसम्म (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#define darray_remove_last(arr) (--(arr).size)

/* Warning, slow: Requires copying all elements after removed item. */
#define darray_remove(arr, i) गर { \
    darray_size_t __index = (i); \
    यदि (__index < (arr).size-1) \
        स_हटाओ( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).size -1 - __index) * परिमाण(*(arr).item) \
        ); \
    (arr).size--; \
} जबसम्म(0)

/*** String buffer ***/

#define darray_append_string(arr, str) गर { \
    अचर वर्ण *__str = (str); \
    darray_append_वस्तुहरू(arr, __str, (darray_size_t) म_माप(__str) + 1); \
    (arr).size--; \
} जबसम्म (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#define darray_append_string0(arr, string) \
    darray_append_वस्तुहरू((arr), (string), (darray_size_t)म_माप(string) + 1)

#define darray_append_lit(arr, stringLiteral) गर { \
    darray_append_वस्तुहरू(arr, stringLiteral, \
                        (darray_size_t) परिमाण(stringLiteral)); \
    (arr).size--; \
} जबसम्म (0)

#define darray_appends_nullterminate(arr, वस्तुहरू, गन्ती) गर { \
    darray_size_t __count = (गन्ती), __oldSize = (arr).size; \
    darray_resize(arr, __oldSize + __count + 1); \
    स_नकल((arr).item + __oldSize, वस्तुहरू, __count * परिमाण(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} जबसम्म (0)

#define darray_prepends_nullterminate(arr, वस्तुहरू, गन्ती) गर { \
    darray_size_t __count = (गन्ती), __oldSize = (arr).size; \
    darray_resize(arr, __count + __oldSize + 1); \
    स_हटाओ((arr).item + __count, (arr).item, \
            __oldSize * परिमाण(*(arr).item)); \
    स_नकल((arr).item, वस्तुहरू, __count * परिमाण(*(arr).item)); \
    (arr).item[(arr).size - 1] = 0; \
    (arr).size--; \
} जबसम्म (0)

/*** Size management ***/

#define darray_resize(arr, newSize) \
    darray_growalloc(arr, (arr).size = (newSize))

#define darray_resize0(arr, newSize) गर { \
    darray_size_t __oldSize = (arr).size, __newSize = (newSize); \
    (arr).size = __newSize; \
    यदि (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        स_रखो(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * परिमाण(*(arr).item)); \
    } \
} जबसम्म (0)

#define darray_realloc(arr, newAlloc) गर { \
    (arr).item = पुनः_स्मृति((arr).item, \
                         ((arr).alloc = (newAlloc)) * परिमाण(*(arr).item)); \
} जबसम्म (0)

#define darray_growalloc(arr, need)   गर { \
    darray_size_t __need = (need); \
    यदि (__need > (arr).alloc) \
        darray_realloc(arr, darray_next_alloc((arr).alloc, __need, \
                                              परिमाण(*(arr).item))); \
} जबसम्म (0)

#define darray_shrink(arr) गर { \
    यदि ((arr).size > 0) \
        (arr).item = पुनः_स्मृति((arr).item, \
                             ((arr).alloc = (arr).size) * परिमाण(*(arr).item)); \
} जबसम्म (0)

#define darray_max_alloc(itemSize) (अच_पूर्णांक_उच्च / (itemSize))

स्थिर अन्तःस्थ darray_size_t
darray_next_alloc(darray_size_t alloc, darray_size_t need, माप_प्रकार itemSize)
{
    प्रतिज्ञा(need < darray_max_alloc(itemSize) / 2); /* Overflow. */
    यदि (alloc == 0)
        alloc = 4;
    जबसम्म (alloc < need)
        alloc *= 2;
    फर्काऊ alloc;
}

/*** Traversal ***/

#define darray_foreach(i, arr) \
    यदि ((arr).item) \
    लागि ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).size]; (i)++)

#define darray_foreach_from(i, arr, from) \
    यदि ((arr).item) \
    लागि ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).size]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#define darray_enumerate(idx, val, arr) \
    यदि ((arr).item) \
    लागि ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_enumerate_from(idx, val, arr, from) \
    यदि ((arr).item) \
    लागि ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).size; \
         (idx)++, (val)++)

#define darray_foreach_reverse(i, arr) \
    यदि ((arr).item && (arr).size) \
    लागि ((i) = &(arr).item[(arr).size - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#define darray_enumerate_reverse(idx, val, arr) \
    यदि ((arr).item && (arr).size) \
    लागि ((idx) = (arr).size - 1, (val) = &(arr).item[(arr).size - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
