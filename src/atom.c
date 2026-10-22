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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
સ્થિર અંતઃરેખ uint32_t
hash_buf(અચલ અક્ષર *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    માટે (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    પાછા hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
સંરચના atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(અક્ષર *) strings;
};

સંરચના atom_table *
atom_table_new(શૂન્ય)
{
    સંરચના atom_table *table = calloc(1, કદ(*table));
    જો (!table)
        પાછા ખાલી;

    darray_init(table->strings);
    darray_append(table->strings, ખાલી);
    table->index_size = 4;
    table->index = calloc(table->index_size, કદ(*table->index));

    પાછા table;
}

શૂન્ય
atom_table_free(સંરચના atom_table *table)
{
    જો (!table)
        પાછા;

    અક્ષર **string;
    darray_foreach(string, table->strings)
        મુક્ત(*string);
    darray_free(table->strings);
    મુક્ત(table->index);
    મુક્ત(table);
}

darray_size_t
atom_table_size(સંરચના atom_table *table)
{
    પાછા darray_size(table->strings);
}

અચલ અક્ષર *
atom_text(સંરચના atom_table *table, xkb_atom_t atom)
{
    પ્રતિજ્ઞા(atom < darray_size(table->strings));
    પાછા darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(સંરચના atom_table *table, અચલ અક્ષર *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    જો (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * કદ(*table->index));
        જો (!tmp)
            પાછા XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * કદ(*table->index));
        માટે (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            અચલ અક્ષર *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            માટે (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                જો (index_pos == 0)
                    ચાલુ;

                xkb_atom_t atom = table->index[index_pos];
                જો (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    તોડો;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    માટે (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        જો (index_pos == 0)
            ચાલુ;

        xkb_atom_t existing_atom = table->index[index_pos];
        જો (existing_atom == XKB_ATOM_NONE) {
            જો (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                અક્ષર *s = strndup(string, len);
                જો (!s)
                    પાછા XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                પાછા new_atom;
            } નહીંતર {
                પાછા XKB_ATOM_NONE;
            }
        }

        અચલ અક્ષર *existing_કિંમત = darray_item(table->strings, existing_atom);
        જો (strncmp(existing_કિંમત, string, len) == 0 && existing_કિંમત[len] == '\0')
            પાછા existing_atom;
    }

    પ્રતિજ્ઞા(!"couldn't find an empty slot during probing");
    પાછા XKB_ATOM_NONE;
}
