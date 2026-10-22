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
स्थिर अन्तःस्थ uint32_t
hash_buf(ध्रुव वर्ण *string, आकार_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    प्रति (आकार_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    प्रत्यागम hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
संरचना atom_सारणी {
    xkb_atom_t *सूचकांक;
    आकार_t सूचकांक_आकार;
    darray(वर्ण *) वाक्यानि;
};

संरचना atom_सारणी *
atom_सारणी_new(शून्य)
{
    संरचना atom_सारणी *सारणी = शून्यस्मृतिं_देहि(1, परिमाण(*सारणी));
    यदि (!सारणी)
        प्रत्यागम रिक्त;

    darray_init(सारणी->वाक्यानि);
    darray_append(सारणी->वाक्यानि, रिक्त);
    सारणी->सूचकांक_आकार = 4;
    सारणी->सूचकांक = शून्यस्मृतिं_देहि(सारणी->सूचकांक_आकार, परिमाण(*सारणी->सूचकांक));

    प्रत्यागम सारणी;
}

शून्य
atom_सारणी_free(संरचना atom_सारणी *सारणी)
{
    यदि (!सारणी)
        प्रत्यागम;

    वर्ण **string;
    darray_foreach(string, सारणी->वाक्यानि)
        मुक्त(*string);
    darray_free(सारणी->वाक्यानि);
    मुक्त(सारणी->सूचकांक);
    मुक्त(सारणी);
}

darray_आकार_t
atom_सारणी_आकार(संरचना atom_सारणी *सारणी)
{
    प्रत्यागम darray_आकार(सारणी->वाक्यानि);
}

ध्रुव वर्ण *
atom_text(संरचना atom_सारणी *सारणी, xkb_atom_t atom)
{
    प्रतिज्ञा(atom < darray_आकार(सारणी->वाक्यानि));
    प्रत्यागम darray_item(सारणी->वाक्यानि, atom);
}

xkb_atom_t
atom_intern(संरचना atom_सारणी *सारणी, ध्रुव वर्ण *string, आकार_t len, bool सङ्कलय)
{
    /* len(string) > 0.8 * index_size */
    यदि (darray_आकार(सारणी->वाक्यानि) > (सारणी->सूचकांक_आकार / 5) * 4) {
        सारणी->सूचकांक_आकार *= 2;
        xkb_atom_t *tmp = realloc(सारणी->सूचकांक,
                                  सारणी->सूचकांक_आकार * परिमाण(*सारणी->सूचकांक));
        यदि (!tmp)
            प्रत्यागम XKB_ATOM_NONE;
        सारणी->सूचकांक = tmp;
        स्मृतिं_पूरय(सारणी->सूचकांक, 0, सारणी->सूचकांक_आकार * परिमाण(*सारणी->सूचकांक));
        प्रति (darray_आकार_t j = 1; j < darray_आकार(सारणी->वाक्यानि); j++) {
            ध्रुव वर्ण *s = darray_item(सारणी->वाक्यानि, j);
            uint32_t hash = hash_buf(s, वाक्यदैर्घ्यम्(s));
            प्रति (आकार_t i = 0; i < सारणी->सूचकांक_आकार; i++) {
                आकार_t सूचकांक_pos = (hash + i) & (सारणी->सूचकांक_आकार - 1);
                यदि (सूचकांक_pos == 0)
                    अनुवर्त;

                xkb_atom_t atom = सारणी->सूचकांक[सूचकांक_pos];
                यदि (atom == XKB_ATOM_NONE) {
                    सारणी->सूचकांक[सूचकांक_pos] = j;
                    भङ्ग;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    प्रति (आकार_t i = 0; i < सारणी->सूचकांक_आकार; i++) {
        आकार_t सूचकांक_pos = (hash + i) & (सारणी->सूचकांक_आकार - 1);
        यदि (सूचकांक_pos == 0)
            अनुवर्त;

        xkb_atom_t existing_atom = सारणी->सूचकांक[सूचकांक_pos];
        यदि (existing_atom == XKB_ATOM_NONE) {
            यदि (सङ्कलय) {
                xkb_atom_t new_atom = darray_आकार(सारणी->वाक्यानि);
                वर्ण *s = strndup(string, len);
                यदि (!s)
                    प्रत्यागम XKB_ATOM_NONE;
                darray_append(सारणी->वाक्यानि, s);
                सारणी->सूचकांक[सूचकांक_pos] = new_atom;
                प्रत्यागम new_atom;
            } अन्यथा {
                प्रत्यागम XKB_ATOM_NONE;
            }
        }

        ध्रुव वर्ण *existing_मानम् = darray_item(सारणी->वाक्यानि, existing_atom);
        यदि (वाक्ये_तुलय(existing_मानम्, string, len) == 0 && existing_मानम्[len] == '\0')
            प्रत्यागम existing_atom;
    }

    प्रतिज्ञा(!"couldn't find an empty slot during probing");
    प्रत्यागम XKB_ATOM_NONE;
}
