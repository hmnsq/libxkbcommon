/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "keymap.h"

শূন্য
InitVMods(সংরচনা xkb_mod_set *info, ধ্রুব সংরচনা xkb_mod_set *mods, bool reset);

শূন্য
MergeModSets(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *into,
             ধ্রুব সংরচনা xkb_mod_set *from, গণনা merge_mode merge);

bool
HandleVModDef(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *mods, VModDef *stmt);
