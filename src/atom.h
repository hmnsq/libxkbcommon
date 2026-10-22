/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"

/** Atoms are indexes into a `darray` */
تعريفنوع darray_اندازه_t xkb_atom_t;

#تعریف_کن XKB_ATOM_NONE 0

بنية atom_جدول;

XKB_EXPORT_خصوصی بنية atom_جدول *
atom_جدول_new(فراغ);

XKB_EXPORT_خصوصی فراغ
atom_جدول_free(بنية atom_جدول *جدول);

XKB_EXPORT_خصوصی darray_اندازه_t
atom_جدول_اندازه(بنية atom_جدول *جدول);

XKB_EXPORT_خصوصی xkb_atom_t
atom_intern(بنية atom_جدول *جدول, ثابت حرف *string, اندازه_t len, bool جمع_کن);

XKB_EXPORT_خصوصی ثابت حرف *
atom_متن(بنية atom_جدول *جدول, xkb_atom_t atom);
