/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
               ಧ್ರುವ ವರ್ಣ **elem_rtrn, ಧ್ರುವ ವರ್ಣ **field_rtrn,
               ಅಭಿವ್ಯಕ್ತಿ_Def **index_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   ಗಣನಾ mod_type mod_type, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *def,
               ಗಣನಾ mod_type mod_type, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   bool *set_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   int64_t *val_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                 xkb_level_index_t *level_rtrn);

ಗಣನಾ xkb_parser_error
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                 ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group_Mask(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                     ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  int64_t *btn_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  xkb_atom_t *val_rtrn);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *val_rtrn, ಧ್ರುವ LookupEntry *ಮೌಲ್ಯಗಳು);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *mask_rtrn, ಧ್ರುವ LookupEntry *ಮೌಲ್ಯಗಳು);
