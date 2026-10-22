/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

/** Atoms are indexes into a `darray` */
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

ସଂରଚନା atom_table;

XKB_EXPORT_PRIVATE ସଂରଚନା atom_table *
atom_table_new(ଶୂନ୍ୟ);

XKB_EXPORT_PRIVATE ଶୂନ୍ୟ
atom_table_free(ସଂରଚନା atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(ସଂରଚନା atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(ସଂରଚନା atom_table *table, ଅଚଳ ଅକ୍ଷର *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ଅଚଳ ଅକ୍ଷର *
atom_text(ସଂରଚନା atom_table *table, xkb_atom_t atom);
