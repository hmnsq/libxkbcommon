/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

/** Atoms are indexes into a `darray` */
ಪ್ರಕಾರನಿರ್ದೇಶ darray_size_t xkb_atom_t;

#define XKB_ATOM_NONE 0

ಸಂರಚನಾ atom_table;

XKB_EXPORT_PRIVATE ಸಂರಚನಾ atom_table *
atom_table_new(ಶೂನ್ಯ);

XKB_EXPORT_PRIVATE ಶೂನ್ಯ
atom_table_free(ಸಂರಚನಾ atom_table *table);

XKB_EXPORT_PRIVATE darray_size_t
atom_table_size(ಸಂರಚನಾ atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(ಸಂರಚನಾ atom_table *table, ಧ್ರುವ ವರ್ಣ *string, size_t len, bool add);

XKB_EXPORT_PRIVATE ಧ್ರುವ ವರ್ಣ *
atom_text(ಸಂರಚನಾ atom_table *table, xkb_atom_t atom);
