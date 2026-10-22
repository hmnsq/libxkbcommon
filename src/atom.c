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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t
hash_buf(ಧ್ರುವ ವರ್ಣ *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    ಪ್ರತಿ (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ಪ್ರತ್ಯಾಗಮ hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ಸಂರಚನಾ atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(ವರ್ಣ *) strings;
};

ಸಂರಚನಾ atom_table *
atom_table_new(ಶೂನ್ಯ)
{
    ಸಂರಚನಾ atom_table *table = calloc(1, ಪರಿಮಾಣ(*table));
    ಯದಿ (!table)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    darray_init(table->strings);
    darray_append(table->strings, ರಿಕ್ತ);
    table->index_size = 4;
    table->index = calloc(table->index_size, ಪರಿಮಾಣ(*table->index));

    ಪ್ರತ್ಯಾಗಮ table;
}

ಶೂನ್ಯ
atom_table_free(ಸಂರಚನಾ atom_table *table)
{
    ಯದಿ (!table)
        ಪ್ರತ್ಯಾಗಮ;

    ವರ್ಣ **string;
    darray_foreach(string, table->strings)
        ಮುಕ್ತ(*string);
    darray_free(table->strings);
    ಮುಕ್ತ(table->index);
    ಮುಕ್ತ(table);
}

darray_size_t
atom_table_size(ಸಂರಚನಾ atom_table *table)
{
    ಪ್ರತ್ಯಾಗಮ darray_size(table->strings);
}

ಧ್ರುವ ವರ್ಣ *
atom_text(ಸಂರಚನಾ atom_table *table, xkb_atom_t atom)
{
    ಪ್ರತಿಜ್ಞಾ(atom < darray_size(table->strings));
    ಪ್ರತ್ಯಾಗಮ darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(ಸಂರಚನಾ atom_table *table, ಧ್ರುವ ವರ್ಣ *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    ಯದಿ (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * ಪರಿಮಾಣ(*table->index));
        ಯದಿ (!tmp)
            ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * ಪರಿಮಾಣ(*table->index));
        ಪ್ರತಿ (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ಧ್ರುವ ವರ್ಣ *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            ಪ್ರತಿ (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                ಯದಿ (index_pos == 0)
                    ಅನುವರ್ತ;

                xkb_atom_t atom = table->index[index_pos];
                ಯದಿ (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    ಭಙ್ಗ;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    ಪ್ರತಿ (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        ಯದಿ (index_pos == 0)
            ಅನುವರ್ತ;

        xkb_atom_t existing_atom = table->index[index_pos];
        ಯದಿ (existing_atom == XKB_ATOM_NONE) {
            ಯದಿ (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                ವರ್ಣ *s = strndup(string, len);
                ಯದಿ (!s)
                    ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                ಪ್ರತ್ಯಾಗಮ new_atom;
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;
            }
        }

        ಧ್ರುವ ವರ್ಣ *existing_ಮೌಲ್ಯ = darray_item(table->strings, existing_atom);
        ಯದಿ (strncmp(existing_ಮೌಲ್ಯ, string, len) == 0 && existing_ಮೌಲ್ಯ[len] == '\0')
            ಪ್ರತ್ಯಾಗಮ existing_atom;
    }

    ಪ್ರತಿಜ್ಞಾ(!"couldn't find an empty slot during probing");
    ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;
}
