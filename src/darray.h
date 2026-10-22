/*
 * Copyright (C) 2011 Joseph Adams <joeyadams3.14159@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

/* Originally taken from: https://ccodearchive.net/info/darray.html
 * But modified for xkbcommon. */

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>

#అన్తర్భావ "utils.h"

రకనిర్వచనం గుర్తులేని పూర్ణాంకం darray_కొలత_t;
#నిర్వచించు DARRAY_కొలత_T_వెడల్పు (పరిమాణం(darray_కొలత_t) * CHAR_BIT)
#నిర్వచించు DARRAY_కొలత_MAX UINT_MAX

#నిర్వచించు darray(type) నిర్మాణం {         \
    /** Current size */               \
    darray_కొలత_t కొలత;               \
    /** Count of allocated items */   \
    darray_కొలత_t alloc;              \
    /** Array of items */             \
    type *అంశం ATTR_COUNTED_BY(కొలత); \
}

#నిర్వచించు darray_new() { 0, 0, 0 }

#నిర్వచించు darray_init(arr) చేయి { \
    (arr).అంశం = 0; (arr).కొలత = 0; (arr).alloc = 0; \
} అయితే (0)

#నిర్వచించు darray_free(arr) చేయి { \
    విడుదల((arr).అంశం); \
    darray_init(arr); \
} అయితే (0)

#నిర్వచించు darray_steal(arr, గమ్యం, గమ్యం_కొలత) చేయి { \
    *(గమ్యం) = (arr).అంశం; \
    ఒకవేళ (గమ్యం_కొలత) \
        *(darray_కొలత_t *) (గమ్యం_కొలత) = (arr).కొలత; \
    darray_init(arr); \
} అయితే (0)

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

రకనిర్వచనం darray (అక్షరం)           darray_char;
రకనిర్వచనం darray (గుర్తుగల అక్షరం)    darray_schar;
రకనిర్వచనం darray (గుర్తులేని అక్షరం)  darray_uchar;

రకనిర్వచనం darray (అక్షరం *)         darray_string;

రకనిర్వచనం darray (పొట్టి)          darray_short;
రకనిర్వచనం darray (పూర్ణాంకం)            darray_int;
రకనిర్వచనం darray (దీర్ఘ)           darray_long;

రకనిర్వచనం darray (గుర్తులేని పొట్టి) darray_ushort;
రకనిర్వచనం darray (గుర్తులేని పూర్ణాంకం)   darray_uint;
రకనిర్వచనం darray (గుర్తులేని దీర్ఘ)  darray_ulong;

/*** Access ***/

#నిర్వచించు darray_అంశం(arr, i)     ((arr).అంశం[i])
#నిర్వచించు darray_వస్తువులు(arr)       ((arr).అంశం)
#నిర్వచించు darray_కొలత(arr)        ((arr).కొలత)
#నిర్వచించు darray_ఖాళీలు(arr)       ((arr).కొలత == 0)

#నిర్వచించు darray_చివరి(arr, fallback) ( \
    !(arr).కొలత \
        ? (fallback) \
        : (arr).అంశం[(arr).కొలత - 1] \
)

/*** Insertion (single item) ***/

#నిర్వచించు darray_append(arr, ...)  చేయి { \
    darray_resize(arr, (arr).కొలత + 1); \
    (arr).అంశం[(arr).కొలత - 1] = (__VA_ARGS__); \
} అయితే (0)

#నిర్వచించు darray_చొప్పించు(arr, i, ...) చేయి { \
    darray_కొలత_t __index = (i); \
    darray_resize(arr, (arr).కొలత+1); \
    memmove( \
        (arr).అంశం + __index + 1, \
        (arr).అంశం + __index, \
        ((arr).కొలత - __index - 1) * పరిమాణం(*(arr).అంశం) \
    ); \
    (arr).అంశం[__index] = (__VA_ARGS__); \
} అయితే(0)

/*** Insertion (multiple items) ***/

#నిర్వచించు darray_append_వస్తువులు(arr, వస్తువులు, లెక్క) చేయి { \
    darray_కొలత_t __count = (లెక్క), __oldSize = (arr).కొలత; \
    darray_resize(arr, __oldSize + __count); \
    జ్ఞాపకం_నకలు((arr).అంశం + __oldSize, వస్తువులు, __count * పరిమాణం(*(arr).అంశం)); \
} అయితే (0)

#నిర్వచించు darray_from_వస్తువులు(arr, వస్తువులు, లెక్క) చేయి { \
    darray_కొలత_t __count = (లెక్క); \
    darray_resize(arr, __count); \
    ఒకవేళ (__count != 0) \
        జ్ఞాపకం_నకలు((arr).అంశం, వస్తువులు, __count * పరిమాణం(*(arr).అంశం)); \
} అయితే (0)

#నిర్వచించు darray_copy(arr_గమ్యం, arr_from) \
    darray_from_వస్తువులు((arr_గమ్యం), (arr_from).అంశం, (arr_from).కొలత)

#నిర్వచించు darray_concat(arr_గమ్యం, arr_from) చేయి { \
    ఒకవేళ ((arr_from).కొలత > 0) \
        darray_append_వస్తువులు((arr_గమ్యం), (arr_from).అంశం, (arr_from).కొలత); \
} అయితే (0)

/*** Removal ***/

/* Warning: Do not call darray_remove_last on an empty darray. */
#నిర్వచించు darray_remove_చివరి(arr) (--(arr).కొలత)

/* Warning, slow: Requires copying all elements after removed item. */
#నిర్వచించు darray_remove(arr, i) చేయి { \
    darray_కొలత_t __index = (i); \
    ఒకవేళ (__index < (arr).కొలత-1) \
        memmove( \
            &(arr).అంశం[__index], \
            &(arr).అంశం[__index + 1], \
            ((arr).కొలత -1 - __index) * పరిమాణం(*(arr).అంశం) \
        ); \
    (arr).కొలత--; \
} అయితే(0)

/*** String buffer ***/

#నిర్వచించు darray_append_string(arr, str) చేయి { \
    స్థిరరాశి అక్షరం *__str = (str); \
    darray_append_వస్తువులు(arr, __str, (darray_కొలత_t) అక్షరశ్రేణి_పొడవు(__str) + 1); \
    (arr).కొలత--; \
} అయితే (0)

/* Same as `darray_append_string` but do count the final '\0' in the size */
#నిర్వచించు darray_append_string0(arr, string) \
    darray_append_వస్తువులు((arr), (string), (darray_కొలత_t)అక్షరశ్రేణి_పొడవు(string) + 1)

#నిర్వచించు darray_append_lit(arr, stringLiteral) చేయి { \
    darray_append_వస్తువులు(arr, stringLiteral, \
                        (darray_కొలత_t) పరిమాణం(stringLiteral)); \
    (arr).కొలత--; \
} అయితే (0)

#నిర్వచించు darray_appends_nullterminate(arr, వస్తువులు, లెక్క) చేయి { \
    darray_కొలత_t __count = (లెక్క), __oldSize = (arr).కొలత; \
    darray_resize(arr, __oldSize + __count + 1); \
    జ్ఞాపకం_నకలు((arr).అంశం + __oldSize, వస్తువులు, __count * పరిమాణం(*(arr).అంశం)); \
    (arr).అంశం[(arr).కొలత - 1] = 0; \
    (arr).కొలత--; \
} అయితే (0)

#నిర్వచించు darray_prepends_nullterminate(arr, వస్తువులు, లెక్క) చేయి { \
    darray_కొలత_t __count = (లెక్క), __oldSize = (arr).కొలత; \
    darray_resize(arr, __count + __oldSize + 1); \
    memmove((arr).అంశం + __count, (arr).అంశం, \
            __oldSize * పరిమాణం(*(arr).అంశం)); \
    జ్ఞాపకం_నకలు((arr).అంశం, వస్తువులు, __count * పరిమాణం(*(arr).అంశం)); \
    (arr).అంశం[(arr).కొలత - 1] = 0; \
    (arr).కొలత--; \
} అయితే (0)

/*** Size management ***/

#నిర్వచించు darray_resize(arr, new_కొలత) \
    darray_growalloc(arr, (arr).కొలత = (new_కొలత))

#నిర్వచించు darray_resize0(arr, new_కొలత) చేయి { \
    darray_కొలత_t __oldSize = (arr).కొలత, __newSize = (new_కొలత); \
    (arr).కొలత = __newSize; \
    ఒకవేళ (__newSize > __oldSize) { \
        darray_growalloc(arr, __newSize); \
        memset(&(arr).అంశం[__oldSize], 0, \
               (__newSize - __oldSize) * పరిమాణం(*(arr).అంశం)); \
    } \
} అయితే (0)

#నిర్వచించు darray_realloc(arr, newAlloc) చేయి { \
    (arr).అంశం = realloc((arr).అంశం, \
                         ((arr).alloc = (newAlloc)) * పరిమాణం(*(arr).అంశం)); \
} అయితే (0)

#నిర్వచించు darray_growalloc(arr, need)   చేయి { \
    darray_కొలత_t __need = (need); \
    ఒకవేళ (__need > (arr).alloc) \
        darray_realloc(arr, darray_తదుపరి_alloc((arr).alloc, __need, \
                                              పరిమాణం(*(arr).అంశం))); \
} అయితే (0)

#నిర్వచించు darray_shrink(arr) చేయి { \
    ఒకవేళ ((arr).కొలత > 0) \
        (arr).అంశం = realloc((arr).అంశం, \
                             ((arr).alloc = (arr).కొలత) * పరిమాణం(*(arr).అంశం)); \
} అయితే (0)

#నిర్వచించు darray_max_alloc(అంశం_కొలత) (UINT_MAX / (అంశం_కొలత))

స్థిర అంతఃవరుస darray_కొలత_t
darray_తదుపరి_alloc(darray_కొలత_t alloc, darray_కొలత_t need, కొలత_t అంశం_కొలత)
{
    ప్రతిజ్ఞా(need < darray_max_alloc(అంశం_కొలత) / 2); /* Overflow. */
    ఒకవేళ (alloc == 0)
        alloc = 4;
    అయితే (alloc < need)
        alloc *= 2;
    తిరిగి alloc;
}

/*** Traversal ***/

#నిర్వచించు darray_foreach(i, arr) \
    ఒకవేళ ((arr).అంశం) \
    కొరకు ((i) = &(arr).అంశం[0]; (i) < &(arr).అంశం[(arr).కొలత]; (i)++)

#నిర్వచించు darray_foreach_from(i, arr, from) \
    ఒకవేళ ((arr).అంశం) \
    కొరకు ((i) = &(arr).అంశం[from]; (i) < &(arr).అంశం[(arr).కొలత]; (i)++)

/* Iterate on index and value at the same time, like Python's enumerate. */
#నిర్వచించు darray_enumerate(idx, val, arr) \
    ఒకవేళ ((arr).అంశం) \
    కొరకు ((idx) = 0, (val) = &(arr).అంశం[0]; \
         (idx) < (arr).కొలత; \
         (idx)++, (val)++)

#నిర్వచించు darray_enumerate_from(idx, val, arr, from) \
    ఒకవేళ ((arr).అంశం) \
    కొరకు ((idx) = (from), (val) = &(arr).అంశం[from]; \
         (idx) < (arr).కొలత; \
         (idx)++, (val)++)

#నిర్వచించు darray_foreach_తిరగేయి(i, arr) \
    ఒకవేళ ((arr).అంశం && (arr).కొలత) \
    కొరకు ((i) = &(arr).అంశం[(arr).కొలత - 1]; \
         (i) >= &(arr).అంశం[0]; \
         (i)--)

#నిర్వచించు darray_enumerate_తిరగేయి(idx, val, arr) \
    ఒకవేళ ((arr).అంశం && (arr).కొలత) \
    కొరకు ((idx) = (arr).కొలత - 1, (val) = &(arr).అంశం[(arr).కొలత - 1]; \
         (val) >= &(arr).అంశం[0]; \
         (idx)--, (val)--)
