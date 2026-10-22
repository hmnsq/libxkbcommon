/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل <stdbool.h>

#شامل "ast.h"
#شامل "keymap.h"

خالی
InitVMods(ساخت xkb_mod_سیٹ *info, ثابت ساخت xkb_mod_سیٹ *mods, bool reset);

خالی
ملاؤ_Mod_Sets(ساخت xkb_context *ctx, ساخت xkb_mod_سیٹ *into,
             ثابت ساخت xkb_mod_سیٹ *from, شمار ملاؤ_منوال ملاؤ);

bool
HandleVModDef(ساخت xkb_context *ctx, ساخت xkb_mod_سیٹ *mods, VModDef *stmt);
