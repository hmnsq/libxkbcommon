/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"

ਖਾਲੀ
InitVMods(ਬਣਤਰ xkb_mod_ਸੈੱਟ *info, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, bool reset);

ਖਾਲੀ
ਮਿਲਾਓ_Mod_Sets(ਬਣਤਰ xkb_context *ctx, ਬਣਤਰ xkb_mod_ਸੈੱਟ *into,
             ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *from, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ);

bool
HandleVModDef(ਬਣਤਰ xkb_context *ctx, ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, VModDef *stmt);
