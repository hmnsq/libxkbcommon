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

#شامل "config.h"

#شامل <assert.h>
#شامل <string.h>

#شامل "atom.h"
#شامل "darray.h"
#شامل "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ایستا درونخطی uint32_t
hash_buf(ثابت نویسه *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    برای (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    بازگشت hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ساختار atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(نویسه *) strings;
};

ساختار atom_table *
atom_table_new(تهی)
{
    ساختار atom_table *table = calloc(1, اندازه(*table));
    اگر (!table)
        بازگشت پوچ;

    darray_init(table->strings);
    darray_append(table->strings, پوچ);
    table->index_size = 4;
    table->index = calloc(table->index_size, اندازه(*table->index));

    بازگشت table;
}

تهی
atom_table_free(ساختار atom_table *table)
{
    اگر (!table)
        بازگشت;

    نویسه **string;
    darray_foreach(string, table->strings)
        آزاد(*string);
    darray_free(table->strings);
    آزاد(table->index);
    آزاد(table);
}

darray_size_t
atom_table_size(ساختار atom_table *table)
{
    بازگشت darray_size(table->strings);
}

ثابت نویسه *
atom_text(ساختار atom_table *table, xkb_atom_t atom)
{
    أكد(atom < darray_size(table->strings));
    بازگشت darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(ساختار atom_table *table, ثابت نویسه *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    اگر (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * اندازه(*table->index));
        اگر (!tmp)
            بازگشت XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * اندازه(*table->index));
        برای (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ثابت نویسه *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            برای (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                اگر (index_pos == 0)
                    ادامه;

                xkb_atom_t atom = table->index[index_pos];
                اگر (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    بشکن;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    برای (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        اگر (index_pos == 0)
            ادامه;

        xkb_atom_t existing_atom = table->index[index_pos];
        اگر (existing_atom == XKB_ATOM_NONE) {
            اگر (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                نویسه *s = strndup(string, len);
                اگر (!s)
                    بازگشت XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                بازگشت new_atom;
            } وگرنه {
                بازگشت XKB_ATOM_NONE;
            }
        }

        ثابت نویسه *existing_مقدار = darray_item(table->strings, existing_atom);
        اگر (strncmp(existing_مقدار, string, len) == 0 && existing_مقدار[len] == '\0')
            بازگشت existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    بازگشت XKB_ATOM_NONE;
}
