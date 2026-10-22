/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include "xkbcommon/xkbcommon.h"

#include <stdbool.h>

#include "ast.h"
#include "keymap.h"

void
InitVMods(sa_nracanaa xkb_mod_set *info, const sa_nracanaa xkb_mod_set *mods, bool reset);

void
MergeModSets(sa_nracanaa xkb_context *ctx, sa_nracanaa xkb_mod_set *into,
             const sa_nracanaa xkb_mod_set *from, ga_nanaa merge_mode merge);

bool
HandleVModDef(sa_nracanaa xkb_context *ctx, sa_nracanaa xkb_mod_set *mods, VModDef *stmt);
