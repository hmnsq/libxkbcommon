/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"

/** Atoms are indexes into a `darray` */
തരനിർവചനം darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

ഘടന atom_table;

XKB_EXPORT_PRIVATE ഘടന atom_table *
atom_table_new(ശൂന്യം);

XKB_EXPORT_PRIVATE ശൂന്യം
atom_table_free(ഘടന atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(ഘടന atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(ഘടന atom_table *table, സ്ഥിരം അക്ഷരം *string, size_t len, bool add);

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
atom_text(ഘടന atom_table *table, xkb_atom_t atom);
