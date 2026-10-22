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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
സ്ഥിതം വരിയിൽ uint32_t
hash_buf(സ്ഥിരം അക്ഷരം *string, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    വേണ്ടി (size_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    തിരികെ hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ഘടന atom_table {
    xkb_atom_t *index;
    size_t index_size;
    darray(അക്ഷരം *) strings;
};

ഘടന atom_table *
atom_table_new(ശൂന്യം)
{
    ഘടന atom_table *table = calloc(1, വലുപ്പം(*table));
    എങ്കിൽ (!table)
        തിരികെ ശൂന്യ;

    darray_init(table->strings);
    darray_append(table->strings, ശൂന്യ);
    table->index_size = 4;
    table->index = calloc(table->index_size, വലുപ്പം(*table->index));

    തിരികെ table;
}

ശൂന്യം
atom_table_free(ഘടന atom_table *table)
{
    എങ്കിൽ (!table)
        തിരികെ;

    അക്ഷരം **string;
    darray_foreach(string, table->strings)
        സ്വതന്ത്രം(*string);
    darray_free(table->strings);
    സ്വതന്ത്രം(table->index);
    സ്വതന്ത്രം(table);
}

darray_size_t
atom_table_size(ഘടന atom_table *table)
{
    തിരികെ darray_size(table->strings);
}

സ്ഥിരം അക്ഷരം *
atom_text(ഘടന atom_table *table, xkb_atom_t atom)
{
    പ്രതിജ്ഞാ(atom < darray_size(table->strings));
    തിരികെ darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(ഘടന atom_table *table, സ്ഥിരം അക്ഷരം *string, size_t len, bool add)
{
    /* len(string) > 0.8 * index_size */
    എങ്കിൽ (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = realloc(table->index,
                                  table->index_size * വലുപ്പം(*table->index));
        എങ്കിൽ (!tmp)
            തിരികെ XKB_ATOM_NONE;
        table->index = tmp;
        memset(table->index, 0, table->index_size * വലുപ്പം(*table->index));
        വേണ്ടി (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            സ്ഥിരം അക്ഷരം *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, strlen(s));
            വേണ്ടി (size_t i = 0; i < table->index_size; i++) {
                size_t index_pos = (hash + i) & (table->index_size - 1);
                എങ്കിൽ (index_pos == 0)
                    തുടരുക;

                xkb_atom_t atom = table->index[index_pos];
                എങ്കിൽ (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    നിർത്തുക;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    വേണ്ടി (size_t i = 0; i < table->index_size; i++) {
        size_t index_pos = (hash + i) & (table->index_size - 1);
        എങ്കിൽ (index_pos == 0)
            തുടരുക;

        xkb_atom_t existing_atom = table->index[index_pos];
        എങ്കിൽ (existing_atom == XKB_ATOM_NONE) {
            എങ്കിൽ (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                അക്ഷരം *s = strndup(string, len);
                എങ്കിൽ (!s)
                    തിരികെ XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                തിരികെ new_atom;
            } അല്ലെങ്കിൽ {
                തിരികെ XKB_ATOM_NONE;
            }
        }

        സ്ഥിരം അക്ഷരം *existing_മൂല്യം = darray_item(table->strings, existing_atom);
        എങ്കിൽ (strncmp(existing_മൂല്യം, string, len) == 0 && existing_മൂല്യം[len] == '\0')
            തിരികെ existing_atom;
    }

    പ്രതിജ്ഞാ(!"couldn't find an empty slot during probing");
    തിരികെ XKB_ATOM_NONE;
}
