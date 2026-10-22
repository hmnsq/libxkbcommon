/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"

#समावेश <stdbool.h>

#समावेश "ast.h"
#समावेश "keymap.h"

व्योम
InitVMods(काष्ठा xkb_mod_set *info, स्थिर काष्ठा xkb_mod_set *mods, bool reset);

व्योम
MergeModSets(काष्ठा xkb_context *ctx, काष्ठा xkb_mod_set *into,
             स्थिर काष्ठा xkb_mod_set *from, क्रमागत merge_mode merge);

bool
HandleVModDef(काष्ठा xkb_context *ctx, काष्ठा xkb_mod_set *mods, VModDef *stmt);
