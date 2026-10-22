/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"

/** Atoms are indexes into a `darray` */
प्रकारनिर्देश darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

संरचना atom_table;

XKB_EXPORT_PRIVATE संरचना atom_table *
atom_table_new(शून्य);

XKB_EXPORT_PRIVATE शून्य
atom_table_free(संरचना atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(संरचना atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(संरचना atom_table *table, ध्रुव वर्ण *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ध्रुव वर्ण *
atom_text(संरचना atom_table *table, xkb_atom_t atom);
