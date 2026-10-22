/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"

/** Atoms are indexes into a `darray` */
வகைவரை darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

கட்டமைப்பு atom_table;

XKB_EXPORT_PRIVATE கட்டமைப்பு atom_table *
atom_table_new(வெறுமை);

XKB_EXPORT_PRIVATE வெறுமை
atom_table_free(கட்டமைப்பு atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(கட்டமைப்பு atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(கட்டமைப்பு atom_table *table, மாறிலி எழுத்து *string, size_t len, bool add);

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
atom_text(கட்டமைப்பு atom_table *table, xkb_atom_t atom);
