/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"

/** Atoms are indexes into a `darray` */
تعريفنوع darray_اندازه_t xkb_atom_t;

#تعریف_کړه XKB_ATOM_NONE 0

بنية atom_جدول;

XKB_EXPORT_شخصي بنية atom_جدول *
atom_جدول_new(فراغ);

XKB_EXPORT_شخصي فراغ
atom_جدول_free(بنية atom_جدول *جدول);

XKB_EXPORT_شخصي darray_اندازه_t
atom_جدول_اندازه(بنية atom_جدول *جدول);

XKB_EXPORT_شخصي xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, اندازه_t len, bool جمع_کړه);

XKB_EXPORT_شخصي ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom);
