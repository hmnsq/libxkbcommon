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

#شامل "config.h"

#شامل <assert.h>
#شامل <string.h>

#شامل "atom.h"
#شامل "darray.h"
#شامل "utils.h"

/* FNV-1a (http://www.isthe.com/chongo/tech/comp/fnv/). */
ساکن اندرونی uint32_t
hash_buf(ثابت حرف *string, سائز_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    برائے (سائز_t i = 0; i < (len + 1) / 2; i++) {
        hash ^= (uint8_t) string[i];
        hash *= 0x01000193;
        hash ^= (uint8_t) string[len - 1 - i];
        hash *= 0x01000193;
    }
    واپس hash;
}

/*
 * The atom table is an insert-only linear probing hash table
 * mapping strings to atoms. Another array maps the atoms to
 * strings. The atom value is the position in the strings array.
 */
ساخت atom_جدول {
    xkb_atom_t *اشاریہ;
    سائز_t اشاریہ_سائز;
    darray(حرف *) لڑیاں;
};

ساخت atom_جدول *
atom_جدول_new(خالی)
{
    ساخت atom_جدول *جدول = صفر_حافظہ_دو(1, حجم(*جدول));
    اگر (!جدول)
        واپس تہی;

    darray_init(جدول->لڑیاں);
    darray_append(جدول->لڑیاں, تہی);
    جدول->اشاریہ_سائز = 4;
    جدول->اشاریہ = صفر_حافظہ_دو(جدول->اشاریہ_سائز, حجم(*جدول->اشاریہ));

    واپس جدول;
}

خالی
atom_جدول_free(ساخت atom_جدول *جدول)
{
    اگر (!جدول)
        واپس;

    حرف **string;
    darray_foreach(string, جدول->لڑیاں)
        آزاد(*string);
    darray_free(جدول->لڑیاں);
    آزاد(جدول->اشاریہ);
    آزاد(جدول);
}

darray_سائز_t
atom_جدول_سائز(ساخت atom_جدول *جدول)
{
    واپس darray_سائز(جدول->لڑیاں);
}

ثابت حرف *
atom_متن(ساخت atom_جدول *جدول, xkb_atom_t atom)
{
    أكد(atom < darray_سائز(جدول->لڑیاں));
    واپس darray_شے(جدول->لڑیاں, atom);
}

xkb_atom_t
atom_intern(ساخت atom_جدول *جدول, ثابت حرف *string, سائز_t len, bool جوڑو)
{
    /* len(string) > 0.8 * index_size */
    اگر (darray_سائز(جدول->لڑیاں) > (جدول->اشاریہ_سائز / 5) * 4) {
        جدول->اشاریہ_سائز *= 2;
        xkb_atom_t *tmp = realloc(جدول->اشاریہ,
                                  جدول->اشاریہ_سائز * حجم(*جدول->اشاریہ));
        اگر (!tmp)
            واپس XKB_ATOM_NONE;
        جدول->اشاریہ = tmp;
        memset(جدول->اشاریہ, 0, جدول->اشاریہ_سائز * حجم(*جدول->اشاریہ));
        برائے (darray_سائز_t j = 1; j < darray_سائز(جدول->لڑیاں); j++) {
            ثابت حرف *s = darray_شے(جدول->لڑیاں, j);
            uint32_t hash = hash_buf(s, لڑی_لمبائی(s));
            برائے (سائز_t i = 0; i < جدول->اشاریہ_سائز; i++) {
                سائز_t اشاریہ_pos = (hash + i) & (جدول->اشاریہ_سائز - 1);
                اگر (اشاریہ_pos == 0)
                    جاری;

                xkb_atom_t atom = جدول->اشاریہ[اشاریہ_pos];
                اگر (atom == XKB_ATOM_NONE) {
                    جدول->اشاریہ[اشاریہ_pos] = j;
                    توڑو;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    برائے (سائز_t i = 0; i < جدول->اشاریہ_سائز; i++) {
        سائز_t اشاریہ_pos = (hash + i) & (جدول->اشاریہ_سائز - 1);
        اگر (اشاریہ_pos == 0)
            جاری;

        xkb_atom_t existing_atom = جدول->اشاریہ[اشاریہ_pos];
        اگر (existing_atom == XKB_ATOM_NONE) {
            اگر (جوڑو) {
                xkb_atom_t new_atom = darray_سائز(جدول->لڑیاں);
                حرف *s = strndup(string, len);
                اگر (!s)
                    واپس XKB_ATOM_NONE;
                darray_append(جدول->لڑیاں, s);
                جدول->اشاریہ[اشاریہ_pos] = new_atom;
                واپس new_atom;
            } ورنہ {
                واپس XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_قدر = darray_شے(جدول->لڑیاں, existing_atom);
        اگر (لڑی_موازنہ(existing_قدر, string, len) == 0 && existing_قدر[len] == '\0')
            واپس existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    واپس XKB_ATOM_NONE;
}
