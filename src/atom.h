/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stddef.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <stdbool.h>

#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "utils.h"

/** Atoms are indexes into a `darray` */
প্রকারনির্দেশ darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

সংরচনা atom_table;

XKB_EXPORT_PRIVATE সংরচনা atom_table *
atom_table_new(শূন্য);

XKB_EXPORT_PRIVATE শূন্য
atom_table_free(সংরচনা atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(সংরচনা atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(সংরচনা atom_table *table, ধ্রুৱ ৱর্ণ *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
atom_text(সংরচনা atom_table *table, xkb_atom_t atom);
