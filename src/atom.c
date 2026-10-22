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
    لكل (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    أرجع hash;
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
    إذا (!table)
        أرجع عدم;

    darray_init(table->strings);
    darray_append(table->strings, عدم);
    table->index_size = 4;
    table->index = calloc(table->index_size, حجم(*table->index));

    أرجع table;
}

فراغ
atom_table_free(بنية atom_table *table)
{
    إذا (!table)
        أرجع;

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
    أرجع darray_size(table->strings);
}

ثابت حرف *
atom_text(بنية atom_table *table, xkb_atom_t atom)
{
    أكد(atom < darray_size(table->strings));
    أرجع darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(بنية atom_table *table, ثابت حرف *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    إذا (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * حجم(*table->index));
        إذا (!tmp)
            أرجع XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * حجم(*table->index));
        لكل (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ثابت حرف *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            لكل (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                إذا (index_pos == 0)
                    استمر;

                xkb_atom_t atom = table->index[index_pos];
                إذا (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    اكسر;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    لكل (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        إذا (index_pos == 0)
            استمر;

        xkb_atom_t existing_atom = table->index[index_pos];
        إذا (existing_atom == XKB_ATOM_NONE) {
            إذا (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                حرف *s = strndup(string, len);
                إذا (!s)
                    أرجع XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                أرجع new_atom;
            } وإلا {
                أرجع XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_نيلاي = darray_item(table->strings, existing_atom);
        إذا (strncmp(existing_نيلاي, string, len) == 0 && existing_نيلاي[len] == '\0')
            أرجع existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    أرجع XKB_ATOM_NONE;
}
