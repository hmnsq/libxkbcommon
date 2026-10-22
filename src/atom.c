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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t
hash_buf(ଅଚଳ ଅକ୍ଷର *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    ପାଇଁ (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ଫେରନ୍ତୁ hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ସଂରଚନା atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(ଅକ୍ଷର *) strings;
};

ସଂରଚନା atom_table *
atom_table_new(ଶୂନ୍ୟ)
{
    ସଂରଚନା atom_table *table = calloc(1, ଆକାର(*table));
    ଯଦି (!table)
        ଫେରନ୍ତୁ ଖାଲି;

    darray_init(table->strings);
    darray_append(table->strings, ଖାଲି);
    table->index_size = 4;
    table->index = calloc(table->index_size, ଆକାର(*table->index));

    ଫେରନ୍ତୁ table;
}

ଶୂନ୍ୟ
atom_table_free(ସଂରଚନା atom_table *table)
{
    ଯଦି (!table)
        ଫେରନ୍ତୁ;

    ଅକ୍ଷର **string;
    darray_foreach(string, table->strings)
        ମୁକ୍ତ(*string);
    darray_free(table->strings);
    ମୁକ୍ତ(table->index);
    ମୁକ୍ତ(table);
}

darray_size_t
atom_table_size(ସଂରଚନା atom_table *table)
{
    ଫେରନ୍ତୁ darray_size(table->strings);
}

ଅଚଳ ଅକ୍ଷର *
atom_text(ସଂରଚନା atom_table *table, xkb_atom_t atom)
{
    ପ୍ରତିଜ୍ଞା(atom < darray_size(table->strings));
    ଫେରନ୍ତୁ darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(ସଂରଚନା atom_table *table, ଅଚଳ ଅକ୍ଷର *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    ଯଦି (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * ଆକାର(*table->index));
        ଯଦି (!tmp)
            ଫେରନ୍ତୁ XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * ଆକାର(*table->index));
        ପାଇଁ (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            ଅଚଳ ଅକ୍ଷର *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            ପାଇଁ (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                ଯଦି (index_pos == 0)
                    ଜାରି;

                xkb_atom_t atom = table->index[index_pos];
                ଯଦି (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    ଭାଙ୍ଗ;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    ପାଇଁ (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        ଯଦି (index_pos == 0)
            ଜାରି;

        xkb_atom_t existing_atom = table->index[index_pos];
        ଯଦି (existing_atom == XKB_ATOM_NONE) {
            ଯଦି (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                ଅକ୍ଷର *s = strndup(string, len);
                ଯଦି (!s)
                    ଫେରନ୍ତୁ XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                ଫେରନ୍ତୁ new_atom;
            } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ XKB_ATOM_NONE;
            }
        }

        ଅଚଳ ଅକ୍ଷର *existing_ମୂଲ୍ୟ = darray_item(table->strings, existing_atom);
        ଯଦି (strncmp(existing_ମୂଲ୍ୟ, string, len) == 0 && existing_ମୂଲ୍ୟ[len] == '\0')
            ଫେରନ୍ତୁ existing_atom;
    }

    ପ୍ରତିଜ୍ଞା(!"couldn't find an empty slot during probing");
    ଫେରନ୍ତୁ XKB_ATOM_NONE;
}
