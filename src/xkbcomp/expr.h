/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **elem_rtrn, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕ್ಷೇತ್ರ_rtrn,
               ಅಭಿವ್ಯಕ್ತಿ_Def **ಸೂಚ್ಯಂಕ_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   ಎಣಿಕೆ mod_type mod_type, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                   xkb_mod_ಮುಖವಾಡ_t *ಮುಖವಾಡ_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *def,
               ಎಣಿಕೆ mod_type mod_type, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
               xkb_mod_ಸೂಚ್ಯಂಕ_t *ndx_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   bool *ಗಣ_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   int64_t *val_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                 xkb_level_ಸೂಚ್ಯಂಕ_t *level_rtrn);

ಎಣಿಕೆ xkb_parser_error
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                 ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, bool absolute,
                 xkb_layout_ಸೂಚ್ಯಂಕ_t *ಗುಂಪು_rtrn, bool *pending_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                     ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_layout_ಮುಖವಾಡ_t *ಗುಂಪು_rtrn,
                     bool *pending_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  int64_t *btn_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  xkb_atom_t *val_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *val_rtrn, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ಮೌಲ್ಯಗಳು);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *ಮುಖವಾಡ_rtrn, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ಮೌಲ್ಯಗಳು);
