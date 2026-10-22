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

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <string.h>

#समावेश "atom.h"
#समावेश "darray.h"
#समावेश "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
अटल अंतरभूत uint32_t
hash_buf(स्थिर अक्षर *string, माप_प्रकार len)
{
    uint32_t hash = UINT32_C(2166136261);
    क्रम (माप_प्रकार i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    वापस hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
काष्ठा atom_table {
    xkb_atom_t *सूचकांक;
    माप_प्रकार सूचकांक_आकार;
    darray(अक्षर *) strings;
};

काष्ठा atom_table *
atom_table_new(व्योम)
{
    काष्ठा atom_table *table = सुस्मृति(1, माप(*table));
    अगर (!table)
        वापस शून्य;

    darray_init(table->strings);
    darray_append(table->strings, शून्य);
    table->सूचकांक_आकार = 4;
    table->सूचकांक = सुस्मृति(table->सूचकांक_आकार, माप(*table->सूचकांक));

    वापस table;
}

व्योम
atom_table_free(काष्ठा atom_table *table)
{
    अगर (!table)
        वापस;

    अक्षर **string;
    darray_foreach(string, table->strings)
        मुक्त(*string);
    darray_free(table->strings);
    मुक्त(table->सूचकांक);
    मुक्त(table);
}

darray_आकार_t
atom_table_आकार(काष्ठा atom_table *table)
{
    वापस darray_आकार(table->strings);
}

स्थिर अक्षर *
atom_text(काष्ठा atom_table *table, xkb_atom_t atom)
{
    निश्चित(atom < darray_आकार(table->strings));
    वापस darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(काष्ठा atom_table *table, स्थिर अक्षर *string, माप_प्रकार len, bool add)
{
    /* len(string) > 0.8 * index_size */
    अगर (darray_आकार(table->strings) > (table->सूचकांक_आकार / 5) * 4) {
        table->सूचकांक_आकार *= 2;
        xkb_atom_t *tmp = पुनः_स्मृति(table->सूचकांक,
                                  table->सूचकांक_आकार * माप(*table->सूचकांक));
        अगर (!tmp)
            वापस XKB_ATOM_NONE;
        table->सूचकांक = tmp;
        स_रखो(table->सूचकांक, 0, table->सूचकांक_आकार * माप(*table->सूचकांक));
        क्रम (darray_आकार_t j = 1; j < darray_आकार(table->strings); j++) {
            स्थिर अक्षर *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, म_माप(s));
            क्रम (माप_प्रकार i = 0; i < table->सूचकांक_आकार; i++) {
                माप_प्रकार सूचकांक_pos = (hash + i) & (table->सूचकांक_आकार - 1);
                अगर (सूचकांक_pos == 0)
                    जारी;

                xkb_atom_t atom = table->सूचकांक[सूचकांक_pos];
                अगर (atom == XKB_ATOM_NONE) {
                    table->सूचकांक[सूचकांक_pos] = j;
                    अवरोध;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    क्रम (माप_प्रकार i = 0; i < table->सूचकांक_आकार; i++) {
        माप_प्रकार सूचकांक_pos = (hash + i) & (table->सूचकांक_आकार - 1);
        अगर (सूचकांक_pos == 0)
            जारी;

        xkb_atom_t existing_atom = table->सूचकांक[सूचकांक_pos];
        अगर (existing_atom == XKB_ATOM_NONE) {
            अगर (add) {
                xkb_atom_t new_atom = darray_आकार(table->strings);
                अक्षर *s = strndup(string, len);
                अगर (!s)
                    वापस XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->सूचकांक[सूचकांक_pos] = new_atom;
                वापस new_atom;
            } अन्यथा {
                वापस XKB_ATOM_NONE;
            }
        }

        स्थिर अक्षर *existing_मूल्य = darray_item(table->strings, existing_atom);
        अगर (म_भेदन(existing_मूल्य, string, len) == 0 && existing_मूल्य[len] == '\0')
            वापस existing_atom;
    }

    निश्चित(!"couldn't find an empty slot during probing");
    वापस XKB_ATOM_NONE;
}
