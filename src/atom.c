/*
 * For MIT-open-group:
 * Copyright 1987, 1998  The Open Group
 *
 * For HPND:
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 * Copyright 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * SPDX-License-Identifier: MIT-open-group AND HPND
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
স্থির অন্তঃরেখ uint32_t
hash_buf(ধ্রুবক অক্ষর *string, আকার_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    জন্য (আকার_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ফেরত hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
কাঠামো atom_table {
    xkb_atom_t *সূচক;
    আকার_t সূচক_আকার;
    darray(অক্ষর *) strings;
};

কাঠামো atom_table *
atom_table_new(শূন্য)
{
    কাঠামো atom_table *table = calloc(1, আকার(*table));
    যদি (!table)
        ফেরত ফাঁকা;

    darray_init(table->strings);
    darray_append(table->strings, ফাঁকা);
    table->সূচক_আকার = 4;
    table->সূচক = calloc(table->সূচক_আকার, আকার(*table->সূচক));

    ফেরত table;
}

শূন্য
atom_table_free(কাঠামো atom_table *table)
{
    যদি (!table)
        ফেরত;

    অক্ষর **string;
    darray_foreach(string, table->strings)
        মুক্ত(*string);
    darray_free(table->strings);
    মুক্ত(table->সূচক);
    মুক্ত(table);
}

darray_আকার_t
atom_table_আকার(কাঠামো atom_table *table)
{
    ফেরত darray_আকার(table->strings);
}

ধ্রুবক অক্ষর *
atom_text(কাঠামো atom_table *table, xkb_atom_t atom)
{
    প্রতিজ্ঞা(atom < darray_আকার(table->strings));
    ফেরত darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(কাঠামো atom_table *table, ধ্রুবক অক্ষর *string, আকার_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    যদি (darray_আকার(table->strings) > (table->সূচক_আকার / 5) * 4) {
        table->সূচক_আকার *= 2;
        xkb_atom_t *tmp = realloc(table->সূচক,
                                  table->সূচক_আকার * আকার(*table->সূচক));
        যদি (!tmp)
            ফেরত XKB_ATOM_NONE;
        table->সূচক = tmp;
        memset(table->সূচক, 0, table->সূচক_আকার * আকার(*table->সূচক));
        জন্য (darray_আকার_t j = 1; j < darray_আকার(table->strings); j++) {
            ধ্রুবক অক্ষর *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            জন্য (আকার_t i = 0; i < table->সূচক_আকার; i++) {
                আকার_t সূচক_pos = (hash + i) & (table->সূচক_আকার - 1);
                যদি (সূচক_pos == 0)
                    চালু;

                xkb_atom_t atom = table->সূচক[সূচক_pos];
                যদি (atom == XKB_ATOM_NONE) {
                    table->সূচক[সূচক_pos] = j;
                    বিরতি;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    জন্য (আকার_t i = 0; i < table->সূচক_আকার; i++) {
        আকার_t সূচক_pos = (hash + i) & (table->সূচক_আকার - 1);
        যদি (সূচক_pos == 0)
            চালু;

        xkb_atom_t existing_atom = table->সূচক[সূচক_pos];
        যদি (existing_atom == XKB_ATOM_NONE) {
            যদি (add) {
                xkb_atom_t new_atom = darray_আকার(table->strings);
                অক্ষর *s = strndup(string, len);
                যদি (!s)
                    ফেরত XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->সূচক[সূচক_pos] = new_atom;
                ফেরত new_atom;
            } অন্যথা {
                ফেরত XKB_ATOM_NONE;
            }
        }

        ধ্রুবক অক্ষর *existing_মান = darray_item(table->strings, existing_atom);
        যদি (strncmp(existing_মান, string, len) == 0 && existing_মান[len] == '\0')
            ফেরত existing_atom;
    }

    প্রতিজ্ঞা(!"couldn't find an empty slot during probing");
    ফেরত XKB_ATOM_NONE;
}
