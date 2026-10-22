/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"

/** Atoms are indexes into a `darray` */
প্রকারলেখ darray_আকার_t xkb_atom_t;

#define XKB_ATOM_NONE 0

কাঠামো atom_table;

XKB_EXPORT_PRIVATE কাঠামো atom_table *
atom_table_new(শূন্য);

XKB_EXPORT_PRIVATE শূন্য
atom_table_free(কাঠামো atom_table *table);

XKB_EXPORT_PRIVATE darray_আকার_t
atom_table_আকার(কাঠামো atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(কাঠামো atom_table *table, ধ্রুবক অক্ষর *string, আকার_t len, bool add);

XKB_EXPORT_PRIVATE ধ্রুবক অক্ষর *
atom_text(কাঠামো atom_table *table, xkb_atom_t atom);
