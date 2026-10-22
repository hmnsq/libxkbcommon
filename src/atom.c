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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <string.h>

#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ساكن مضمن uint32_t
hash_buf(ثابت حرف *string, سائيز_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    لكل (سائيز_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    أرجع hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
بنية atom_جدول {
    xkb_atom_t *اشاريو;
    سائيز_t اشاريو_سائيز;
    darray(حرف *) لڙيون;
};

بنية atom_جدول *
atom_جدول_new(فراغ)
{
    بنية atom_جدول *جدول = صفر_يادگيري_ڏيو(1, حجم(*جدول));
    إذا (!جدول)
        أرجع عدم;

    darray_init(جدول->لڙيون);
    darray_append(جدول->لڙيون, عدم);
    جدول->اشاريو_سائيز = 4;
    جدول->اشاريو = صفر_يادگيري_ڏيو(جدول->اشاريو_سائيز, حجم(*جدول->اشاريو));

    أرجع جدول;
}

فراغ
atom_جدول_free(بنية atom_جدول *جدول)
{
    إذا (!جدول)
        أرجع;

    حرف **string;
    darray_foreach(string, جدول->لڙيون)
        حرر(*string);
    darray_free(جدول->لڙيون);
    حرر(جدول->اشاريو);
    حرر(جدول);
}

darray_سائيز_t
atom_جدول_سائيز(بنية atom_جدول *جدول)
{
    أرجع darray_سائيز(جدول->لڙيون);
}

ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom)
{
    أكد(atom < darray_سائيز(جدول->لڙيون));
    أرجع darray_شي(جدول->لڙيون, atom);
}

xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, سائيز_t len, bool جوڙيو)
{
    /* len(string) > 0.8 * index_size */
    إذا (darray_سائيز(جدول->لڙيون) > (جدول->اشاريو_سائيز / 5) * 4) {
        جدول->اشاريو_سائيز *= 2;
        xkb_atom_t *tmp = realloc(جدول->اشاريو,
                                  جدول->اشاريو_سائيز * حجم(*جدول->اشاريو));
        إذا (!tmp)
            أرجع XKB_ATOM_NONE;
        جدول->اشاريو = tmp;
        memset(جدول->اشاريو, 0, جدول->اشاريو_سائيز * حجم(*جدول->اشاريو));
        لكل (darray_سائيز_t j = 1; j < darray_سائيز(جدول->لڙيون); j++) {
            ثابت حرف *s = darray_شي(جدول->لڙيون, j);
            uint32_t hash = hash_buf(s, لڙي_ڊيگهه(s));
            لكل (سائيز_t i = 0; i < جدول->اشاريو_سائيز; i++) {
                سائيز_t اشاريو_pos = (hash + i) & (جدول->اشاريو_سائيز - 1);
                إذا (اشاريو_pos == 0)
                    استمر;

                xkb_atom_t atom = جدول->اشاريو[اشاريو_pos];
                إذا (atom == XKB_ATOM_NONE) {
                    جدول->اشاريو[اشاريو_pos] = j;
                    اكسر;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    لكل (سائيز_t i = 0; i < جدول->اشاريو_سائيز; i++) {
        سائيز_t اشاريو_pos = (hash + i) & (جدول->اشاريو_سائيز - 1);
        إذا (اشاريو_pos == 0)
            استمر;

        xkb_atom_t existing_atom = جدول->اشاريو[اشاريو_pos];
        إذا (existing_atom == XKB_ATOM_NONE) {
            إذا (جوڙيو) {
                xkb_atom_t new_atom = darray_سائيز(جدول->لڙيون);
                حرف *s = strndup(string, len);
                إذا (!s)
                    أرجع XKB_ATOM_NONE;
                darray_append(جدول->لڙيون, s);
                جدول->اشاريو[اشاريو_pos] = new_atom;
                أرجع new_atom;
            } وإلا {
                أرجع XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_قدر = darray_شي(جدول->لڙيون, existing_atom);
        إذا (لڙي_ڀيٽ(existing_قدر, string, len) == 0 && existing_قدر[len] == '\0')
            أرجع existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    أرجع XKB_ATOM_NONE;
}
