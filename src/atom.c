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
hash_buf(ثابت حرف *string, ناپ_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    لكل (ناپ_t i = 0; i < (len + 1) / 2; i++) {
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
    xkb_atom_t *اشاریہ;
    ناپ_t اشاریہ_ناپ;
    darray(حرف *) لڑیاں;
};

بنية atom_جدول *
atom_جدول_new(فراغ)
{
    بنية atom_جدول *جدول = صفر_حافظہ_دیو(1, حجم(*جدول));
    إذا (!جدول)
        أرجع عدم;

    darray_init(جدول->لڑیاں);
    darray_append(جدول->لڑیاں, عدم);
    جدول->اشاریہ_ناپ = 4;
    جدول->اشاریہ = صفر_حافظہ_دیو(جدول->اشاریہ_ناپ, حجم(*جدول->اشاریہ));

    أرجع جدول;
}

فراغ
atom_جدول_free(بنية atom_جدول *جدول)
{
    إذا (!جدول)
        أرجع;

    حرف **string;
    darray_foreach(string, جدول->لڑیاں)
        حرر(*string);
    darray_free(جدول->لڑیاں);
    حرر(جدول->اشاریہ);
    حرر(جدول);
}

darray_ناپ_t
atom_جدول_ناپ(بنية atom_جدول *جدول)
{
    أرجع darray_ناپ(جدول->لڑیاں);
}

ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom)
{
    أكد(atom < darray_ناپ(جدول->لڑیاں));
    أرجع darray_شے(جدول->لڑیاں, atom);
}

xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, ناپ_t len, bool جوڑو)
{
    /* len(string) > 0.8 * index_size */
    إذا (darray_ناپ(جدول->لڑیاں) > (جدول->اشاریہ_ناپ / 5) * 4) {
        جدول->اشاریہ_ناپ *= 2;
        xkb_atom_t *tmp = realloc(جدول->اشاریہ,
                                  جدول->اشاریہ_ناپ * حجم(*جدول->اشاریہ));
        إذا (!tmp)
            أرجع XKB_ATOM_NONE;
        جدول->اشاریہ = tmp;
        memset(جدول->اشاریہ, 0, جدول->اشاریہ_ناپ * حجم(*جدول->اشاریہ));
        لكل (darray_ناپ_t j = 1; j < darray_ناپ(جدول->لڑیاں); j++) {
            ثابت حرف *s = darray_شے(جدول->لڑیاں, j);
            uint32_t hash = hash_buf(s, لڑی_لمبائی(s));
            لكل (ناپ_t i = 0; i < جدول->اشاریہ_ناپ; i++) {
                ناپ_t اشاریہ_pos = (hash + i) & (جدول->اشاریہ_ناپ - 1);
                إذا (اشاریہ_pos == 0)
                    استمر;

                xkb_atom_t atom = جدول->اشاریہ[اشاریہ_pos];
                إذا (atom == XKB_ATOM_NONE) {
                    جدول->اشاریہ[اشاریہ_pos] = j;
                    اكسر;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    لكل (ناپ_t i = 0; i < جدول->اشاریہ_ناپ; i++) {
        ناپ_t اشاریہ_pos = (hash + i) & (جدول->اشاریہ_ناپ - 1);
        إذا (اشاریہ_pos == 0)
            استمر;

        xkb_atom_t existing_atom = جدول->اشاریہ[اشاریہ_pos];
        إذا (existing_atom == XKB_ATOM_NONE) {
            إذا (جوڑو) {
                xkb_atom_t new_atom = darray_ناپ(جدول->لڑیاں);
                حرف *s = strndup(string, len);
                إذا (!s)
                    أرجع XKB_ATOM_NONE;
                darray_append(جدول->لڑیاں, s);
                جدول->اشاریہ[اشاریہ_pos] = new_atom;
                أرجع new_atom;
            } وإلا {
                أرجع XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_قدر = darray_شے(جدول->لڑیاں, existing_atom);
        إذا (لڑی_مقابلہ(existing_قدر, string, len) == 0 && existing_قدر[len] == '\0')
            أرجع existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    أرجع XKB_ATOM_NONE;
}
