/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#رهنمود once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن <stdbool.h>

#ضمن "ast.h"
#ضمن "keymap.h"

فراغ
InitVMods(بنية xkb_mod_مجموعه *info, ثابت بنية xkb_mod_مجموعه *mods, bool reset);

فراغ
ادغام_Mod_Sets(بنية xkb_context *ctx, بنية xkb_mod_مجموعه *into,
             ثابت بنية xkb_mod_مجموعه *from, تعداد ادغام_نما ادغام);

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_مجموعه *mods, VModDef *stmt);
