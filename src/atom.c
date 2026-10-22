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

#include "config.h"

#include <assert.h>
#include <string.h>

#include "atom.h"
#include "darray.h"
#include "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
static anta_hstha uint32_t
hash_buf(const var_na *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    དོན་ལུ (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    ལོག hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
sa_nracanaa atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(var_na *) strings;
};

sa_nracanaa atom_table *
atom_table_new(void)
{
    sa_nracanaa atom_table *table = calloc(1, parimaa_na(*table));
    གལ་སྲིད (!table)
        ལོག NULL;

    darray_init(table->strings);
    darray_append(table->strings, NULL);
    table->index_size = 4;
    table->index = calloc(table->index_size, parimaa_na(*table->index));

    ལོག table;
}

void
atom_table_free(sa_nracanaa atom_table *table)
{
    གལ་སྲིད (!table)
        ལོག;

    var_na **string;
    darray_foreach(string, table->strings)
        free(*string);
    darray_free(table->strings);
    free(table->index);
    free(table);
}

darray_size_t
atom_table_size(sa_nracanaa atom_table *table)
{
    ལོག darray_size(table->strings);
}

const var_na *
atom_text(sa_nracanaa atom_table *table, xkb_atom_t atom)
{
    assert(atom < darray_size(table->strings));
    ལོག darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(sa_nracanaa atom_table *table, const var_na *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    གལ་སྲིད (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * parimaa_na(*table->index));
        གལ་སྲིད (!tmp)
            ལོག XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * parimaa_na(*table->index));
        དོན་ལུ (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            const var_na *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            དོན་ལུ (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                གལ་སྲིད (index_pos == 0)
                    continue;

                xkb_atom_t atom = table->index[index_pos];
                གལ་སྲིད (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    bha_nga;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    དོན་ལུ (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        གལ་སྲིད (index_pos == 0)
            continue;

        xkb_atom_t existing_atom = table->index[index_pos];
        གལ་སྲིད (existing_atom == XKB_ATOM_NONE) {
            གལ་སྲིད (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                var_na *s = strndup(string, len);
                གལ་སྲིད (!s)
                    ལོག XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                ལོག new_atom;
            } དེ་མིན {
                ལོག XKB_ATOM_NONE;
            }
        }

        const var_na *existing_value = darray_item(table->strings, existing_atom);
        གལ་སྲིད (strncmp(existing_value, string, len) == 0 && existing_value[len] == '\0')
            ལོག existing_atom;
    }

    assert(!"couldn't find an empty slot during probing");
    ལོག XKB_ATOM_NONE;
}
