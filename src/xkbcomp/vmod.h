/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"

ಶೂನ್ಯ
InitVMods(ಸಂರಚನಾ xkb_mod_set *info, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, bool reset);

ಶೂನ್ಯ
MergeModSets(ಸಂರಚನಾ xkb_context *ctx, ಸಂರಚನಾ xkb_mod_set *into,
             ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *from, ಗಣನಾ merge_mode merge);

bool
HandleVModDef(ಸಂರಚನಾ xkb_context *ctx, ಸಂರಚನಾ xkb_mod_set *mods, VModDef *stmt);
