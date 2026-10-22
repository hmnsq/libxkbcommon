/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن <stdbool.h>

#ضمن "ast.h"
#ضمن "keymap.h"

فراغ
InitVMods(بنية xkb_mod_سیٹ *info, ثابت بنية xkb_mod_سیٹ *mods, bool reset);

فراغ
ملاؤ_Mod_Sets(بنية xkb_context *ctx, بنية xkb_mod_سیٹ *into,
             ثابت بنية xkb_mod_سیٹ *from, تعداد ملاؤ_منوال ملاؤ);

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_سیٹ *mods, VModDef *stmt);
