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
static inline uint32_t
hash_buf(const char *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    为 (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    返回 hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
struct atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(char *) strings;
};

struct atom_table *
atom_table_new(void)
{
    struct atom_table *table = calloc(1, sizeof(*table));
    如果 (!table)
        返回 NULL;

    darray_init(table->strings);
    darray_append(table->strings, NULL);
    table->index_size = 4;
    table->index = calloc(table->index_size, sizeof(*table->index));

    返回 table;
}

void
atom_table_free(struct atom_table *table)
{
    如果 (!table)
        返回;

    char **string;
    darray_foreach(string, table->strings)
        free(*string);
    darray_free(table->strings);
    free(table->index);
    free(table);
}

darray_size_t
atom_table_size(struct atom_table *table)
{
    返回 darray_size(table->strings);
}

const char *
atom_text(struct atom_table *table, xkb_atom_t atom)
{
    assert(atom < darray_size(table->strings));
    返回 darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(struct atom_table *table, const char *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    如果 (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * sizeof(*table->index));
        如果 (!tmp)
            返回 XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * sizeof(*table->index));
        为 (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            const char *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            为 (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                如果 (index_pos == 0)
                    continue;

                xkb_atom_t atom = table->index[index_pos];
                如果 (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    break;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    为 (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        如果 (index_pos == 0)
            continue;

        xkb_atom_t existing_atom = table->index[index_pos];
        如果 (existing_atom == XKB_ATOM_NONE) {
            如果 (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                char *s = strndup(string, len);
                如果 (!s)
                    返回 XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                返回 new_atom;
            } 否则 {
                返回 XKB_ATOM_NONE;
            }
        }

        const char *existing_value = darray_item(table->strings, existing_atom);
        如果 (strncmp(existing_value, string, len) == 0 && existing_value[len] == '\0')
            返回 existing_atom;
    }

    assert(!"couldn't find an empty slot during probing");
    返回 XKB_ATOM_NONE;
}
