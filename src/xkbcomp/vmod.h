/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"

ଶୂନ୍ୟ
InitVMods(ସଂରଚନା xkb_mod_set *info, ଅଚଳ ସଂରଚନା xkb_mod_set *mods, bool reset);

ଶୂନ୍ୟ
MergeModSets(ସଂରଚନା xkb_context *ctx, ସଂରଚନା xkb_mod_set *into,
             ଅଚଳ ସଂରଚନା xkb_mod_set *from, ଗଣନା merge_mode merge);

bool
HandleVModDef(ସଂରଚନା xkb_context *ctx, ସଂରଚନା xkb_mod_set *mods, VModDef *stmt);
