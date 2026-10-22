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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "atom.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
स्थिर ओळीत uint32_t
hash_buf(स्थिरांक अक्षर *string, माप_प्रकार len)
{
    uint32_t hash = UINT32_C(2166136261);
    साठी (माप_प्रकार i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    परत hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
संरचना atom_table {
    xkb_atom_t *index;
    माप_प्रकार index_size;
    darray(अक्षर *) strings;
};

संरचना atom_table *
atom_table_new(शून्य)
{
    संरचना atom_table *table = सुस्मृति(1, आकार(*table));
    जर (!table)
        परत रिक्त;

    darray_init(table->strings);
    darray_append(table->strings, रिक्त);
    table->index_size = 4;
    table->index = सुस्मृति(table->index_size, आकार(*table->index));

    परत table;
}

शून्य
atom_table_free(संरचना atom_table *table)
{
    जर (!table)
        परत;

    अक्षर **string;
    darray_foreach(string, table->strings)
        मुक्त(*string);
    darray_free(table->strings);
    मुक्त(table->index);
    मुक्त(table);
}

darray_size_t
atom_table_size(संरचना atom_table *table)
{
    परत darray_size(table->strings);
}

स्थिरांक अक्षर *
atom_text(संरचना atom_table *table, xkb_atom_t atom)
{
    प्रतिज्ञा(atom < darray_size(table->strings));
    परत darray_item(table->strings, atom);
}

xkb_atom_t
atom_intern(संरचना atom_table *table, स्थिरांक अक्षर *string, माप_प्रकार len, bool add)
{
    /* len(string) > 0.8 * index_size */
    जर (darray_size(table->strings) > (table->index_size / 5) * 4) {
        table->index_size *= 2;
        xkb_atom_t *tmp = पुनः_स्मृति(table->index,
                                  table->index_size * आकार(*table->index));
        जर (!tmp)
            परत XKB_ATOM_NONE;
        table->index = tmp;
        स_रखो(table->index, 0, table->index_size * आकार(*table->index));
        साठी (darray_size_t j = 1; j < darray_size(table->strings); j++) {
            स्थिरांक अक्षर *s = darray_item(table->strings, j);
            uint32_t hash = hash_buf(s, म_माप(s));
            साठी (माप_प्रकार i = 0; i < table->index_size; i++) {
                माप_प्रकार index_pos = (hash + i) & (table->index_size - 1);
                जर (index_pos == 0)
                    सुरूठेवा;

                xkb_atom_t atom = table->index[index_pos];
                जर (atom == XKB_ATOM_NONE) {
                    table->index[index_pos] = j;
                    थांबा;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    साठी (माप_प्रकार i = 0; i < table->index_size; i++) {
        माप_प्रकार index_pos = (hash + i) & (table->index_size - 1);
        जर (index_pos == 0)
            सुरूठेवा;

        xkb_atom_t existing_atom = table->index[index_pos];
        जर (existing_atom == XKB_ATOM_NONE) {
            जर (add) {
                xkb_atom_t new_atom = darray_size(table->strings);
                अक्षर *s = strndup(string, len);
                जर (!s)
                    परत XKB_ATOM_NONE;
                darray_append(table->strings, s);
                table->index[index_pos] = new_atom;
                परत new_atom;
            } अन्यथा {
                परत XKB_ATOM_NONE;
            }
        }

        स्थिरांक अक्षर *existing_मूल्य = darray_item(table->strings, existing_atom);
        जर (म_भेदन(existing_मूल्य, string, len) == 0 && existing_मूल्य[len] == '\0')
            परत existing_atom;
    }

    प्रतिज्ञा(!"couldn't find an empty slot during probing");
    परत XKB_ATOM_NONE;
}
