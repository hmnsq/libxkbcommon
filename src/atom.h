/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"

/** Atoms are indexes into a `darray` */
પ્રકારવ્યાખ્યા darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

સંરચના atom_table;

XKB_EXPORT_PRIVATE સંરચના atom_table *
atom_table_new(શૂન્ય);

XKB_EXPORT_PRIVATE શૂન્ય
atom_table_free(સંરચના atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(સંરચના atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(સંરચના atom_table *table, અચલ અક્ષર *string, size_t len, bool add);

XKB_EXPORT_PRIVATE અચલ અક્ષર *
atom_text(સંરચના atom_table *table, xkb_atom_t atom);
