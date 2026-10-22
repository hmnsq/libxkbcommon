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
hash_buf(ثابت حرف *string, اندازه_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    لكل (اندازه_t i = 0; i < (len + 1) / 2; i++) {
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
    xkb_atom_t *شاخص;
    اندازه_t شاخص_اندازه;
    darray(حرف *) مزي_ګانې;
};

بنية atom_جدول *
atom_جدول_new(فراغ)
{
    بنية atom_جدول *جدول = حافظه_صفر_ورکړه(1, حجم(*جدول));
    إذا (!جدول)
        أرجع عدم;

    darray_init(جدول->مزي_ګانې);
    darray_append(جدول->مزي_ګانې, عدم);
    جدول->شاخص_اندازه = 4;
    جدول->شاخص = حافظه_صفر_ورکړه(جدول->شاخص_اندازه, حجم(*جدول->شاخص));

    أرجع جدول;
}

فراغ
atom_جدول_free(بنية atom_جدول *جدول)
{
    إذا (!جدول)
        أرجع;

    حرف **string;
    darray_foreach(string, جدول->مزي_ګانې)
        حرر(*string);
    darray_free(جدول->مزي_ګانې);
    حرر(جدول->شاخص);
    حرر(جدول);
}

darray_اندازه_t
atom_جدول_اندازه(بنية atom_جدول *جدول)
{
    أرجع darray_اندازه(جدول->مزي_ګانې);
}

ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom)
{
    أكد(atom < darray_اندازه(جدول->مزي_ګانې));
    أرجع darray_توکی(جدول->مزي_ګانې, atom);
}

xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, اندازه_t len, bool جمع_کړه)
{
    /* len(string) > 0.8 * index_size */
    إذا (darray_اندازه(جدول->مزي_ګانې) > (جدول->شاخص_اندازه / 5) * 4) {
        جدول->شاخص_اندازه *= 2;
        xkb_atom_t *tmp = realloc(جدول->شاخص,
                                  جدول->شاخص_اندازه * حجم(*جدول->شاخص));
        إذا (!tmp)
            أرجع XKB_ATOM_NONE;
        جدول->شاخص = tmp;
        memset(جدول->شاخص, 0, جدول->شاخص_اندازه * حجم(*جدول->شاخص));
        لكل (darray_اندازه_t j = 1; j < darray_اندازه(جدول->مزي_ګانې); j++) {
            ثابت حرف *s = darray_توکی(جدول->مزي_ګانې, j);
            uint32_t hash = hash_buf(s, مزي_اوږدوالی(s));
            لكل (اندازه_t i = 0; i < جدول->شاخص_اندازه; i++) {
                اندازه_t شاخص_pos = (hash + i) & (جدول->شاخص_اندازه - 1);
                إذا (شاخص_pos == 0)
                    استمر;

                xkb_atom_t atom = جدول->شاخص[شاخص_pos];
                إذا (atom == XKB_ATOM_NONE) {
                    جدول->شاخص[شاخص_pos] = j;
                    اكسر;
                }
            }
        }
    }

    uint32_t hash = hash_buf(string, len);
    لكل (اندازه_t i = 0; i < جدول->شاخص_اندازه; i++) {
        اندازه_t شاخص_pos = (hash + i) & (جدول->شاخص_اندازه - 1);
        إذا (شاخص_pos == 0)
            استمر;

        xkb_atom_t existing_atom = جدول->شاخص[شاخص_pos];
        إذا (existing_atom == XKB_ATOM_NONE) {
            إذا (جمع_کړه) {
                xkb_atom_t new_atom = darray_اندازه(جدول->مزي_ګانې);
                حرف *s = strndup(string, len);
                إذا (!s)
                    أرجع XKB_ATOM_NONE;
                darray_append(جدول->مزي_ګانې, s);
                جدول->شاخص[شاخص_pos] = new_atom;
                أرجع new_atom;
            } وإلا {
                أرجع XKB_ATOM_NONE;
            }
        }

        ثابت حرف *existing_ارزښت = darray_توکی(جدول->مزي_ګانې, existing_atom);
        إذا (مزي_پرتله(existing_ارزښت, string, len) == 0 && existing_ارزښت[len] == '\0')
            أرجع existing_atom;
    }

    أكد(!"couldn't find an empty slot during probing");
    أرجع XKB_ATOM_NONE;
}
