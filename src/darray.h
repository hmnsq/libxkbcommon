/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>

#अन्तर्भाव "utils.h"

प्रकारनिर्देश अचिह्नित पूर्णाङ्क darray_आकार_t;
#परिभाषय DARRAY_आकार_T_विस्तारः (परिमाण(darray_आकार_t) * CHAR_BIT)
#परिभाषय DARRAY_आकार_MAX UINT_MAX

#परिभाषय darray(type) संरचना {         \
    /** Current size */               \
    darray_आकार_t आकार;               \
    /** Count of allocated items */   \
    darray_आकार_t alloc;              \
    /** Array of items */             \
    type *item ATTR_COUNTED_BY(आकार); \
}

#परिभाषय darray_new() { 0, 0, 0 }

#परिभाषय darray_init(arr) कुरु { \
    (arr).item = 0; (arr).आकार = 0; (arr).alloc = 0; \
} यावत् (0)

#परिभाषय darray_free(arr) कुरु { \
    मुक्त((arr).item); \
    darray_init(arr); \
} यावत् (0)

#परिभाषय darray_steal(arr, गन्तव्यम्, गन्तव्यम्_आकार) कुरु { \
    *(गन्तव्यम्) = (arr).item; \
    यदि (गन्तव्यम्_आकार) \
        *(darray_आकार_t *) (गन्तव्यम्_आकार) = (arr).आकार; \
    darray_init(arr); \
} यावत् (0)

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
प्रकारनिर्देश darray (चिह्नित वर्ण)    darray_schar;
प्रकारनिर्देश darray (अचिह्नित वर्ण)  darray_uchar;

प्रकारनिर्देश darray (वर्ण *)         darray_string;

प्रकारनिर्देश darray (ह्रस्व)          darray_short;
प्रकारनिर्देश darray (पूर्णाङ्क)            darray_int;
प्रकारनिर्देश darray (दीर्घ)           darray_long;

प्रकारनिर्देश darray (अचिह्नित ह्रस्व) darray_ushort;
प्रकारनिर्देश darray (अचिह्नित पूर्णाङ्क)   darray_uint;
प्रकारनिर्देश darray (अचिह्नित दीर्घ)  darray_ulong;

/*** Access ***/

#परिभाषय darray_item(arr, i)     ((arr).item[i])
#परिभाषय darray_वस्तूनि(arr)       ((arr).item)
#परिभाषय darray_आकार(arr)        ((arr).आकार)
#परिभाषय darray_रिक्तानि(arr)       ((arr).आकार == 0)

#परिभाषय darray_चरमः(arr, fallback) ( \
    !(arr).आकार \
        ? (fallback) \
        : (arr).item[(arr).आकार - 1] \
)

/*** Insertion (single item) ***/

#परिभाषय darray_append(arr, ...)  कुरु { \
    darray_resize(arr, (arr).आकार + 1); \
    (arr).item[(arr).आकार - 1] = (__VA_ARGS__); \
} यावत् (0)

#परिभाषय darray_निवेशय(arr, i, ...) कुरु { \
    darray_आकार_t __index = (i); \
    darray_resize(arr, (arr).आकार+1); \
    memmove( \
        (arr).item + __index + 1, \
        (arr).item + __index, \
        ((arr).आकार - __index - 1) * परिमाण(*(arr).item) \
    ); \
    (arr).item[__index] = (__VA_ARGS__); \
} यावत्(0)

/*** Insertion (multiple items) ***/

#परिभाषय darray_append_वस्तूनि(arr, वस्तूनि, गणनम्) कुरु { \
    darray_आकार_t __count = (गणनम्), __oldSize = (arr).आकार; \
    darray_resize(arr, __oldSize + __count); \
    स्मृतिं_प्रतिलिख((arr).item + __oldSize, वस्तूनि, __count * परिमाण(*(arr).item)); \
} यावत् (0)

#परिभाषय darray_from_वस्तूनि(arr, वस्तूनि, गणनम्) कुरु { \
    darray_आकार_t __count = (गणनम्); \
    darray_resize(arr, __count); \
    यदि (__count != 0) \
        स्मृतिं_प्रतिलिख((arr).item, वस्तूनि, __count * परिमाण(*(arr).item)); \
} यावत् (0)

#परिभाषय darray_copy(arr_गन्तव्यम्, arr_from) \
    darray_from_वस्तूनि((arr_गन्तव्यम्), (arr_from).item, (arr_from).आकार)

#परिभाषय darray_concat(arr_गन्तव्यम्, arr_from) कुरु { \
    यदि ((arr_from).आकार > 0) \
        darray_append_वस्तूनि((arr_गन्तव्यम्), (arr_from).item, (arr_from).आकार); \
} यावत् (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#परिभाषय darray_remove_चरमः(arr) (--(arr).आकार)

/* Warning, slow: Requires copying all elements after removed item. */
#परिभाषय darray_remove(arr, i) कुरु { \
    darray_आकार_t __index = (i); \
    यदि (__index < (arr).आकार-1) \
        memmove( \
            &(arr).item[__index], \
            &(arr).item[__index + 1], \
            ((arr).आकार -1 - __index) * परिमाण(*(arr).item) \
        ); \
    (arr).आकार--; \
} यावत्(0)

/*** String buffer ***/

#परिभाषय darray_append_string(arr, str) कुरु { \
    ध्रुव वर्ण *__str = (str); \
    darray_append_वस्तूनि(arr, __str, (darray_आकार_t) वाक्यदैर्घ्यम्(__str) + 1); \
    (arr).आकार--; \
} यावत् (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#परिभाषय darray_append_string0(arr, string) \
    darray_append_वस्तूनि((arr), (string), (darray_आकार_t)वाक्यदैर्घ्यम्(string) + 1)

#परिभाषय darray_append_lit(arr, stringLiteral) कुरु { \
    darray_append_वस्तूनि(arr, stringLiteral, \
                        (darray_आकार_t) परिमाण(stringLiteral)); \
    (arr).आकार--; \
} यावत् (0)

#परिभाषय darray_appends_nullterminate(arr, वस्तूनि, गणनम्) कुरु { \
    darray_आकार_t __count = (गणनम्), __oldSize = (arr).आकार; \
    darray_resize(arr, __oldSize + __count + 1); \
    स्मृतिं_प्रतिलिख((arr).item + __oldSize, वस्तूनि, __count * परिमाण(*(arr).item)); \
    (arr).item[(arr).आकार - 1] = 0; \
    (arr).आकार--; \
} यावत् (0)

#परिभाषय darray_prepends_nullterminate(arr, वस्तूनि, गणनम्) कुरु { \
    darray_आकार_t __count = (गणनम्), __oldSize = (arr).आकार; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).item + __count, (arr).item, \
            __oldSize * परिमाण(*(arr).item)); \
    स्मृतिं_प्रतिलिख((arr).item, वस्तूनि, __count * परिमाण(*(arr).item)); \
    (arr).item[(arr).आकार - 1] = 0; \
    (arr).आकार--; \
} यावत् (0)

/*** Size management ***/

#परिभाषय darray_resize(arr, new_आकार) \
    darray_growalloc(arr, (arr).आकार = (new_आकार))

#परिभाषय darray_resize0(arr, new_आकार) कुरु { \
    darray_आकार_t __oldSize = (arr).आकार, __newSize = (new_आकार); \
    (arr).आकार = __newSize; \
    यदि (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        स्मृतिं_पूरय(&(arr).item[__oldSize], 0, \
               (__newSize - __oldSize) * परिमाण(*(arr).item)); \
    } \
} यावत् (0)

#परिभाषय darray_realloc(arr, newAlloc) कुरु { \
    (arr).item = realloc((arr).item, \
                         ((arr).alloc = (newAlloc)) * परिमाण(*(arr).item)); \
} यावत् (0)

#परिभाषय darray_growalloc(arr, need)   कुरु { \
    darray_आकार_t __need = (need); \
    यदि (__need > (arr).alloc) \
        darray_realloc(arr, darray_अग्र_alloc((arr).alloc, __need, \
                                              परिमाण(*(arr).item))); \
} यावत् (0)

#परिभाषय darray_shrink(arr) कुरु { \
    यदि ((arr).आकार > 0) \
        (arr).item = realloc((arr).item, \
                             ((arr).alloc = (arr).आकार) * परिमाण(*(arr).item)); \
} यावत् (0)

#परिभाषय darray_max_alloc(item_आकार) (UINT_MAX / (item_आकार))

स्थिर अन्तःस्थ darray_आकार_t
darray_अग्र_alloc(darray_आकार_t alloc, darray_आकार_t need, आकार_t item_आकार)
{
    प्रतिज्ञा(need < darray_max_alloc(item_आकार) / 2); /* Overflow. */
    यदि (alloc == 0)
        alloc = 4;
    यावत् (alloc < need)
        alloc *= 2;
    प्रत्यागम alloc;
}

/*** Traversal ***/

#परिभाषय darray_foreach(i, arr) \
    यदि ((arr).item) \
    प्रति ((i) = &(arr).item[0]; (i) < &(arr).item[(arr).आकार]; (i)++)

#परिभाषय darray_foreach_from(i, arr, from) \
    यदि ((arr).item) \
    प्रति ((i) = &(arr).item[from]; (i) < &(arr).item[(arr).आकार]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#परिभाषय darray_enumerate(idx, val, arr) \
    यदि ((arr).item) \
    प्रति ((idx) = 0, (val) = &(arr).item[0]; \
         (idx) < (arr).आकार; \
         (idx)++, (val)++)

#परिभाषय darray_enumerate_from(idx, val, arr, from) \
    यदि ((arr).item) \
    प्रति ((idx) = (from), (val) = &(arr).item[from]; \
         (idx) < (arr).आकार; \
         (idx)++, (val)++)

#परिभाषय darray_foreach_reverse(i, arr) \
    यदि ((arr).item && (arr).आकार) \
    प्रति ((i) = &(arr).item[(arr).आकार - 1]; \
         (i) >= &(arr).item[0]; \
         (i)--)

#परिभाषय darray_enumerate_reverse(idx, val, arr) \
    यदि ((arr).item && (arr).आकार) \
    प्रति ((idx) = (arr).आकार - 1, (val) = &(arr).item[(arr).आकार - 1]; \
         (val) >= &(arr).item[0]; \
         (idx)--, (val)--)
