/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stddef.h>
#समावेश <stdint.h>
#समावेश <stdbool.h>

#समावेश "darray.h"
#समावेश "utils.h"

/** Atoms are indexes into a `darray` */
प्रकार darray_आकार_t xkb_atom_t;

#घोषणा XKB_ATOM_NONE 0

काष्ठा atom_table;

XKB_EXPORT_PRIVATE काष्ठा atom_table *
atom_table_new(व्योम);

XKB_EXPORT_PRIVATE व्योम
atom_table_free(काष्ठा atom_table *table);

XKB_EXPORT_PRIVATE darray_आकार_t
atom_table_आकार(काष्ठा atom_table *table);

XKB_EXPORT_PRIVATE xkb_atom_t
atom_intern(काष्ठा atom_table *table, स्थिर अक्षर *string, माप_प्रकार len, bool add);

XKB_EXPORT_PRIVATE स्थिर अक्षर *
atom_text(काष्ठा atom_table *table, xkb_atom_t atom);
