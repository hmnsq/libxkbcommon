/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"

ಶೂನ್ಯ
InitVMods(ರಚನೆ xkb_mod_ಗಣ *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, bool reset);

ಶೂನ್ಯ
ವಿಲೀನ_Mod_Sets(ರಚನೆ xkb_context *ctx, ರಚನೆ xkb_mod_ಗಣ *into,
             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *from, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ);

bool
HandleVModDef(ರಚನೆ xkb_context *ctx, ರಚನೆ xkb_mod_ಗಣ *mods, VModDef *stmt);
