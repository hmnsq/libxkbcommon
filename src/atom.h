/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "darray.h"
#include "utils.h"

/** Atoms are indexes into a `darray` */
typedef darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

sa_nracanaa atom_table;

XKB_EXPORT_PRIVATE sa_nracanaa atom_table *
atom_table_new(void);

XKB_EXPORT_PRIVATE void
atom_table_free(sa_nracanaa atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(sa_nracanaa atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(sa_nracanaa atom_table *table, const var_na *string, size_t len, bool add);

XKB_EXPORT_PRIVATE const var_na *
atom_text(sa_nracanaa atom_table *table, xkb_atom_t atom);
