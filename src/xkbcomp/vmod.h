/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

#অন্তর্ভাৱ <stdbool.h>

#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "keymap.h"

শূন্য
InitVMods(সংরচনা xkb_mod_set *info, ধ্রুৱ সংরচনা xkb_mod_set *mods, bool reset);

শূন্য
MergeModSets(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *into,
             ধ্রুৱ সংরচনা xkb_mod_set *from, গণনা merge_mode merge);

bool
HandleVModDef(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *mods, VModDef *stmt);
