/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stddef.h>
#شامل <stdint.h>
#شامل <stdbool.h>

#شامل "darray.h"
#شامل "utils.h"

/** Atoms are indexes into a `darray` */
تعریفنوع darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

ساختار atom_table;

XKB_EXPORT_PRIVATE ساختار atom_table *
atom_table_new(تهی);

XKB_EXPORT_PRIVATE تهی
atom_table_free(ساختار atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(ساختار atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(ساختار atom_table *table, ثابت نویسه *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ثابت نویسه *
atom_text(ساختار atom_table *table, xkb_atom_t atom);
