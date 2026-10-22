/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stddef.h>
#شامل <stdint.h>
#شامل <stdbool.h>

#شامل "darray.h"
#شامل "utils.h"

/** Atoms are indexes into a `darray` */
قسمتعریف darray_سائز_t xkb_atom_t;

#تعریف_کرو XKB_ATOM_NONE 0

ساخت atom_جدول;

XKB_EXPORT_نجی ساخت atom_جدول *
atom_جدول_new(خالی);

XKB_EXPORT_نجی خالی
atom_جدول_free(ساخت atom_جدول *جدول);

XKB_EXPORT_نجی darray_سائز_t
atom_جدول_سائز(ساخت atom_جدول *جدول);

XKB_EXPORT_نجی xkb_atom_t
atom_intern(ساخت atom_جدول *جدول, ثابت حرف *string, سائز_t len, bool جوڑو);

XKB_EXPORT_نجی ثابت حرف *
atom_متن(ساخت atom_جدول *جدول, xkb_atom_t atom);
