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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
நிலையான உள்வரி uint32_t
hash_buf(மாறிலி எழுத்து *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    க்கு (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    திரும்பு hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
கட்டமைப்பு atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(எழுத்து *) strings;
};

கட்டமைப்பு atom_table *
atom_table_new(வெறுமை)
{
    கட்டமைப்பு atom_table *table = calloc(1, அளவு(*table));
    எனில் (!table)
        திரும்பு வெற்று;

    darray_init(table->strings);
    darray_append(table->strings, வெற்று);
    table->index_size = 4;
    table->index = calloc(table->index_size, அளவு(*table->index));

    திரும்பு table;
}

வெறுமை
atom_table_free(கட்டமைப்பு atom_table *table)
{
    எனில் (!table)
        திரும்பு;

    எழுத்து **string;
    darray_foreach(string, table->strings)
        விடுவி(*string);
    darray_free(table->strings);
    விடுவி(table->index);
    விடுவி(table);
}

darray_size_t
atom_table_size(கட்டமைப்பு atom_table *table)
{
    திரும்பு darray_size(table->strings);
}

மாறிலி எழுத்து *
atom_text(கட்டமைப்பு atom_table *table, xkb_atom_t atom)
{
    ப்ரதிஜ்ஞா(atom < darray_size(table->strings));
    திரும்பு darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(கட்டமைப்பு atom_table *table, மாறிலி எழுத்து *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    எனில் (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * அளவு(*table->index));
        எனில் (!tmp)
            திரும்பு XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * அளவு(*table->index));
        க்கு (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            மாறிலி எழுத்து *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            க்கு (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                எனில் (index_pos == 0)
                    தொடர்;

                xkb_atom_t atom = table->index[index_pos];
                எனில் (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    நிறுத்து;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    க்கு (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        எனில் (index_pos == 0)
            தொடர்;

        xkb_atom_t existing_atom = table->index[index_pos];
        எனில் (existing_atom == XKB_ATOM_NONE) {
            எனில் (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                எழுத்து *s = strndup(string, len);
                எனில் (!s)
                    திரும்பு XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                திரும்பு new_atom;
            } இல்லையெனில் {
                திரும்பு XKB_ATOM_NONE;
            }
        }

        மாறிலி எழுத்து *existing_மதிப்பு = darray_item(table->strings, existing_atom);
        எனில் (strncmp(existing_மதிப்பு, string, len) == 0 && existing_மதிப்பு[len] == '\0')
            திரும்பு existing_atom;
    }

    ப்ரதிஜ்ஞா(!"couldn't find an empty slot during probing");
    திரும்பு XKB_ATOM_NONE;
}
