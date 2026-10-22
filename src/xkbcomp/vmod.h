/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل <stdbool.h>

#شامل "ast.h"
#شامل "keymap.h"

تهی
InitVMods(ساختار xkb_mod_set *info, ثابت ساختار xkb_mod_set *mods, bool reset);

تهی
MergeModSets(ساختار xkb_context *ctx, ساختار xkb_mod_set *into,
             ثابت ساختار xkb_mod_set *from, شمارش merge_mode merge);

bool
HandleVModDef(ساختار xkb_context *ctx, ساختار xkb_mod_set *mods, VModDef *stmt);
