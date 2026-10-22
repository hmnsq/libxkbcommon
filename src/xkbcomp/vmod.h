/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "keymap.h"

શૂન્ય
InitVMods(સંરચના xkb_mod_set *info, અચલ સંરચના xkb_mod_set *mods, bool reset);

શૂન્ય
MergeModSets(સંરચના xkb_context *ctx, સંરચના xkb_mod_set *into,
             અચલ સંરચના xkb_mod_set *from, ગણના merge_mode merge);

bool
HandleVModDef(સંરચના xkb_context *ctx, સંરચના xkb_mod_set *mods, VModDef *stmt);
