/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <assert.h>
#समावेश <limits.h>

#समावेश "utils.h"

प्रकार अचिन्हित पूर्णांक darray_आकार_t;
#घोषणा DARRAY_आकार_T_WIDTH (माप(darray_आकार_t) * अक्षर_बिट)
#घोषणा DARRAY_आकार_MAX अच_पूर्णांक_उच्च

#घोषणा darray(type) काष्ठा {         \
    /** Current size */               \
    darray_आकार_t आकार;               \
    /** Count of allocated items */   \
    darray_आकार_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(आकार); \
}

#घोषणा darray_new() { 0, 0, 0 }

#घोषणा darray_init(arr) करो { \
    (arr).item = 0; (arr).आकार = 0; (arr).alloc = 0; \
} जबतक (0)

#घोषणा darray_free(arr) करो { \
    मुक्त((arr).item); \
    darray_init(arr); \
} जबतक (0)

#घोषणा darray_steal(arr, to, to_आकार) करो { \
    *(to) = (arr).item; \
    अगर (to_आकार) \
        *(darray_आकार_t *) (to_आकार) = (arr).आकार; \
    darray_init(arr); \
} जबतक (0)

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

प्रकार darray (अक्षर)           darray_char;
प्रकार darray (चिन्हित अक्षर)    darray_schar;
प्रकार darray (अचिन्हित अक्षर)  darray_uchar;

प्रकार darray (अक्षर *)         darray_string;

प्रकार darray (लघु)          darray_short;
प्रकार darray (पूर्णांक)            darray_int;
प्रकार darray (दीर्घ)           darray_long;

प्रकार darray (अचिन्हित लघु) darray_ushort;
प्रकार darray (अचिन्हित पूर्णांक)   darray_uint;
प्रकार darray (अचिन्हित दीर्घ)  darray_ulong;

/*** Access ***/

#घोषणा darray_item(arr, i)     ((arr).item[i])
#घोषणा darray_वस्तुएँ(arr)       ((arr).item)
#घोषणा darray_आकार(arr)        ((arr).आकार)
#घोषणा darray_empty(arr)       ((arr).आकार == 0)

#घोषणा darray_last(arr, fallback) ( \
    !(arr).आकार \
        ? (fallback) \
        : (arr).item[(arr).आकार - 1] \
)

/*** Insertion (single item) ***/

#घोषणा darray_append(arr, ...)  करो { \
    darray_resize(arr, (arr).आकार + 1); \
    (arr).item[(arr).आकार - 1] = (__VA_ARGS__); \
} जबतक (0)

#घोषणा darray_जोड़ो(arr, i, ...) करो { \
    darray_आकार_t __index = (i); \
    darray_resize(arr, (arr).आकार+1); \
    स_हटाओ( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).आकार - __index - 1) * माप(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} जबतक(0)

/*** Insertion (multiple items) ***/

#घोषणा darray_append_वस्तुएँ(arr, वस्तुएँ, गिनती) करो { \
    darray_आकार_t __count = (गिनती), __oldSize = (arr).आकार; \
    darray_resize(arr, __oldSize + __count); \
    स_नकल((arr).item + __oldSize, वस्तुएँ, __count * माप(*(arr).item)); \
} जबतक (0)

#घोषणा darray_from_वस्तुएँ(arr, वस्तुएँ, गिनती) करो { \
    darray_आकार_t __count = (गिनती); \
    darray_resize(arr, __count); \
    अगर (__count != 0) \
        स_नकल((arr).item, वस्तुएँ, __count * माप(*(arr).item)); \
} जबतक (0)

#घोषणा darray_copy(arr_to, arr_from) \
    darray_from_वस्तुएँ((arr_to), (arr_from).item, (arr_from).आकार)

#घोषणा darray_concat(arr_to, arr_from) करो { \
    अगर ((arr_from).आकार > 0) \
        darray_append_वस्तुएँ((arr_to), (arr_from).item, (arr_from).आकार); \
} जबतक (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#घोषणा darray_remove_last(arr) (--(arr).आकार)

/* Warning, slow: Requires copying all elements after removed item. */
#घोषणा darray_remove(arr, i) करो { \
    darray_आकार_t __index = (i); \
    अगर (__index < (arr).आकार-1) \
        स_हटाओ( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).आकार -1 - __index) * माप(*(arr).item) \
        ); \
    (arr).आकार--; \
} जबतक(0)

/*** String buffer ***/

#घोषणा darray_append_string(arr, str) करो { \
    स्थिर अक्षर *__str = (str); \
    darray_append_वस्तुएँ(arr, __str, (darray_आकार_t) म_माप(__str) + 1); \
    (arr).आकार--; \
} जबतक (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#घोषणा darray_append_string0(arr, string) \
    darray_append_वस्तुएँ((arr), (string), (darray_आकार_t)म_माप(string) + 1)

#घोषणा darray_append_lit(arr, stringLiteral) करो { \
    darray_append_वस्तुएँ(arr, stringLiteral, \
                        (darray_आकार_t) माप(stringLiteral)); \
    (arr).आकार--; \
} जबतक (0)

#घोषणा darray_appends_nullterminate(arr, वस्तुएँ, गिनती) करो { \
    darray_आकार_t __count = (गिनती), __oldSize = (arr).आकार; \
    darray_resize(arr, __oldSize + __count + 1); \
    स_नकल((arr).item + __oldSize, वस्तुएँ, __count * माप(*(arr).item)); \
    (arr).item[(arr).आकार - 1] = 0; \
    (arr).आकार--; \
} जबतक (0)

#घोषणा darray_prepends_nullterminate(arr, वस्तुएँ, गिनती) करो { \
    darray_आकार_t __count = (गिनती), __oldSize = (arr).आकार; \
    darray_resize(arr, __count + __oldSize + 1); \
    स_हटाओ((arr).item + __count, (arr).item, \
            __oldSize * माप(*(arr).item)); \
    स_नकल((arr).item, वस्तुएँ, __count * माप(*(arr).item)); \
    (arr).item[(arr).आकार - 1] = 0; \
    (arr).आकार--; \
} जबतक (0)

/*** Size management ***/

#घोषणा darray_resize(arr, new_आकार) \
    darray_growalloc(arr, (arr).आकार = (new_आकार))

#घोषणा darray_resize0(arr, new_आकार) करो { \
    darray_आकार_t __oldSize = (arr).आकार, __newSize = (new_आकार); \
    (arr).आकार = __newSize; \
    अगर (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        स_रखो(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * माप(*(arr).item)); \
    } \
} जबतक (0)

#घोषणा darray_realloc(arr, newAlloc) करो { \
    (arr).item = पुनः_स्मृति((arr).item, \
                         ((arr).alloc = (newAlloc)) * माप(*(arr).item)); \
} जबतक (0)

#घोषणा darray_growalloc(arr, need)   करो { \
    darray_आकार_t __need = (need); \
    अगर (__need > (arr).alloc) \
        darray_realloc(arr, darray_अग्र_alloc((arr).alloc, __need, \
                                              माप(*(arr).item))); \
} जबतक (0)

#घोषणा darray_shrink(arr) करो { \
    अगर ((arr).आकार > 0) \
        (arr).item = पुनः_स्मृति((arr).item, \
                             ((arr).alloc = (arr).आकार) * माप(*(arr).item)); \
} जबतक (0)

#घोषणा darray_max_alloc(item_आकार) (अच_पूर्णांक_उच्च / (item_आकार))

अटल अंतरभूत darray_आकार_t
darray_अग्र_alloc(darray_आकार_t alloc, darray_आकार_t need, माप_प्रकार item_आकार)
{
    निश्चित(need < darray_max_alloc(item_आकार) / 2); /* Overflow. */
    अगर (alloc == 0)
        alloc = 4;
    जबतक (alloc < need)
        alloc *= 2;
    वापस alloc;
}

/*** Traversal ***/

#घोषणा darray_foreach(i, arr) \
    अगर ((arr).item) \
    क्रम ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).आकार]; (i)++)

#घोषणा darray_foreach_from(i, arr, from) \
    अगर ((arr).item) \
    क्रम ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).आकार]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#घोषणा darray_enumerate(idx, val, arr) \
    अगर ((arr).item) \
    क्रम ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).आकार; \
         (idx)++, (val)++)

#घोषणा darray_enumerate_from(idx, val, arr, from) \
    अगर ((arr).item) \
    क्रम ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).आकार; \
         (idx)++, (val)++)

#घोषणा darray_foreach_reverse(i, arr) \
    अगर ((arr).item && (arr).आकार) \
    क्रम ((i) = &(arr).item[(arr).आकार - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#घोषणा darray_enumerate_reverse(idx, val, arr) \
    अगर ((arr).item && (arr).आकार) \
    क्रम ((idx) = (arr).आकार - 1, (val) = &(arr).item[(arr).आकार - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
