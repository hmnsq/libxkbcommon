/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن <stdbool.h>

#ضمن "ast.h"
#ضمن "keymap.h"

فراغ
InitVMods(بنية xkb_mod_set *info, ثابت بنية xkb_mod_set *mods, bool reset);

فراغ
MergeModSets(بنية xkb_context *ctx, بنية xkb_mod_set *into,
             ثابت بنية xkb_mod_set *from, تعداد merge_mode merge);

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_set *mods, VModDef *stmt);
