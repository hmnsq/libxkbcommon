/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"

/** Atoms are indexes into a `darray` */
تعريفنوع darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

بنية atom_table;

XKB_EXPORT_PRIVATE بنية atom_table *
atom_table_new(فراغ);

XKB_EXPORT_PRIVATE فراغ
atom_table_free(بنية atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(بنية atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(بنية atom_table *table, ثابت حرف *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ثابت حرف *
atom_text(بنية atom_table *table, xkb_atom_t atom);
