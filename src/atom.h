/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"

/** Atoms are indexes into a `darray` */
تعريفنوع darray_سائيز_t xkb_atom_t;

#وصف_ڪريو XKB_ATOM_NONE 0

بنية atom_جدول;

XKB_EXPORT_ذاتي بنية atom_جدول *
atom_جدول_new(فراغ);

XKB_EXPORT_ذاتي فراغ
atom_جدول_free(بنية atom_جدول *جدول);

XKB_EXPORT_ذاتي darray_سائيز_t
atom_جدول_سائيز(بنية atom_جدول *جدول);

XKB_EXPORT_ذاتي xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, سائيز_t len, bool جوڙيو);

XKB_EXPORT_ذاتي ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom);
