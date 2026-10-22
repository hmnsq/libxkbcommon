/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"

/** Atoms are indexes into a `darray` */
تعريفنوع darray_ناپ_t xkb_atom_t;

#تعریف_کرو XKB_ATOM_NONE 0

بنية atom_جدول;

XKB_EXPORT_نجی بنية atom_جدول *
atom_جدول_new(فراغ);

XKB_EXPORT_نجی فراغ
atom_جدول_free(بنية atom_جدول *جدول);

XKB_EXPORT_نجی darray_ناپ_t
atom_جدول_ناپ(بنية atom_جدول *جدول);

XKB_EXPORT_نجی xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, ناپ_t len, bool جوڑو);

XKB_EXPORT_نجی ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom);
