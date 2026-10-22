/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#هدايت once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن <stdbool.h>

#ضمن "ast.h"
#ضمن "keymap.h"

فراغ
InitVMods(بنية xkb_mod_سيٽ *info, ثابت بنية xkb_mod_سيٽ *mods, bool reset);

فراغ
ملايو_Mod_Sets(بنية xkb_context *ctx, بنية xkb_mod_سيٽ *into,
             ثابت بنية xkb_mod_سيٽ *from, تعداد ملايو_گهڻائي ملايو);

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_سيٽ *mods, VModDef *stmt);
