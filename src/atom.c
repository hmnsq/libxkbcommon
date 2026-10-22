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
স্থির অন্তঃস্থ uint32_t
hash_buf(ধ্রুব বর্ণ *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    প্রতি (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    প্রত্যাগম hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
সংরচনা atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(বর্ণ *) strings;
};

সংরচনা atom_table *
atom_table_new(শূন্য)
{
    সংরচনা atom_table *table = calloc(1, পরিমাণ(*table));
    যদি (!table)
        প্রত্যাগম রিক্ত;

    darray_init(table->strings);
    darray_append(table->strings, রিক্ত);
    table->index_size = 4;
    table->index = calloc(table->index_size, পরিমাণ(*table->index));

    প্রত্যাগম table;
}

শূন্য
atom_table_free(সংরচনা atom_table *table)
{
    যদি (!table)
        প্রত্যাগম;

    বর্ণ **string;
    darray_foreach(string, table->strings)
        মুক্ত(*string);
    darray_free(table->strings);
    মুক্ত(table->index);
    মুক্ত(table);
}

darray_size_t
atom_table_size(সংরচনা atom_table *table)
{
    প্রত্যাগম darray_size(table->strings);
}

ধ্রুব বর্ণ *
atom_text(সংরচনা atom_table *table, xkb_atom_t atom)
{
    প্রতিজ্ঞা(atom < darray_size(table->strings));
    প্রত্যাগম darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(সংরচনা atom_table *table, ধ্রুব বর্ণ *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    যদি (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * পরিমাণ(*table->index));
        যদি (!tmp)
            প্রত্যাগম XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * পরিমাণ(*table->index));
        প্রতি (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ধ্রুব বর্ণ *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            প্রতি (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                যদি (index_pos == 0)
                    অনুবর্ত;

                xkb_atom_t atom = table->index[index_pos];
                যদি (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    ভঙ্গ;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    প্রতি (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        যদি (index_pos == 0)
            অনুবর্ত;

        xkb_atom_t existing_atom = table->index[index_pos];
        যদি (existing_atom == XKB_ATOM_NONE) {
            যদি (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                বর্ণ *s = strndup(string, len);
                যদি (!s)
                    প্রত্যাগম XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                প্রত্যাগম new_atom;
            } অন্যথা {
                প্রত্যাগম XKB_ATOM_NONE;
            }
        }

        ধ্রুব বর্ণ *existing_মান = darray_item(table->strings, existing_atom);
        যদি (strncmp(existing_মান, string, len) == 0 && existing_মান[len] == '\0')
            প্রত্যাগম existing_atom;
    }

    প্রতিজ্ঞা(!"couldn't find an empty slot during probing");
    প্রত্যাগম XKB_ATOM_NONE;
}
