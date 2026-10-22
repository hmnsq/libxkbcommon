/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "keymap.h"

ശൂന്യം
InitVMods(ഘടന xkb_mod_set *info, സ്ഥിരം ഘടന xkb_mod_set *mods, bool reset);

ശൂന്യം
MergeModSets(ഘടന xkb_context *ctx, ഘടന xkb_mod_set *into,
             സ്ഥിരം ഘടന xkb_mod_set *from, എണ്ണൽ merge_mode merge);

bool
HandleVModDef(ഘടന xkb_context *ctx, ഘടന xkb_mod_set *mods, VModDef *stmt);
