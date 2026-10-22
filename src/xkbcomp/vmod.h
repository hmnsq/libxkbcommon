/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن <stdbool.h>

#ضمن "ast.h"
#ضمن "keymap.h"

فراغ
InitVMods(بنية xkb_mod_سېټ *info, ثابت بنية xkb_mod_سېټ *mods, bool reset);

فراغ
یوځای_Mod_Sets(بنية xkb_context *ctx, بنية xkb_mod_سېټ *into,
             ثابت بنية xkb_mod_سېټ *from, تعداد یوځای_ډېرپېښ یوځای);

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_سېټ *mods, VModDef *stmt);
