/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "keymap.h"

வெறுமை
InitVMods(கட்டமைப்பு xkb_mod_set *info, மாறிலி கட்டமைப்பு xkb_mod_set *mods, bool reset);

வெறுமை
MergeModSets(கட்டமைப்பு xkb_context *ctx, கட்டமைப்பு xkb_mod_set *into,
             மாறிலி கட்டமைப்பு xkb_mod_set *from, எண்ணிக்கை merge_mode merge);

bool
HandleVModDef(கட்டமைப்பு xkb_context *ctx, கட்டமைப்பு xkb_mod_set *mods, VModDef *stmt);
