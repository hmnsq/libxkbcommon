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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <string.h>

#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ساكن مضمن uint32_t
hash_buf(ثابت حرف *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    עבור (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    החזר hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
بنية atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(حرف *) strings;
};

بنية atom_table *
atom_table_new(فراغ)
{
    بنية atom_table *table = calloc(1, حجم(*table));
    אם (!table)
        החזר عدم;

    darray_init(table->strings);
    darray_append(table->strings, عدم);
    table->index_size = 4;
    table->index = calloc(table->index_size, حجم(*table->index));

    החזר table;
}

فراغ
atom_table_free(بنية atom_table *table)
{
    אם (!table)
        החזר;

    حرف **string;
    darray_foreach(string, table->strings)
        حرر(*string);
    darray_free(table->strings);
    حرر(table->index);
    حرر(table);
}

darray_size_t
atom_table_size(بنية atom_table *table)
{
    החזר darray_size(table->strings);
}

ثابت حرف *
atom_text(بنية atom_table *table, xkb_atom_t atom)
{
    أكد(atom < darray_size(table->strings));
    החזר darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(بنية atom_table *table, ثابت حرف *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    אם (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * حجم(*table->index));
        אם (!tmp)
            החזר XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * حجم(*table->index));
        עבור (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ثابت حرف *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            עבור (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                אם (index_pos == 0)
                    استمر;

                xkb_atom_t atom = table->index[index_pos];
                אם (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    اكسر;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    עבור (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        אם (index_pos == 0)
            استمر;

        xkb_atom_t existing_atom = table->index[index_pos];
        אם (existing_atom == XKB_ATOM_NONE) {
            אם (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                حرف *s = strndup(string, len);
                אם (!s)
                    החזר XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                החזר new_atom;
            } אחרת {
                החזר XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_value = darray_item(table->strings, existing_atom);
        אם (strncmp(existing_value, string, len) == 0 && existing_value[len] == '\0')
            החזר existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    החזר XKB_ATOM_NONE;
}
