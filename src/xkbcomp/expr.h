/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
فرچ_Resolve_Lhs(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
               ثابت حرف **elem_rtrn, ثابت حرف **field_rtrn,
               فرچ_Def **index_rtrn);

bool
فرچ_Resolve_Mod_Mask(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
فرچ_Resolve_Mod(بنية xkb_context *ctx, ثابت فرچ_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
فرچ_Resolve_Boolean(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                   bool *set_rtrn);

bool
فرچ_Resolve_Integer(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                   int64_t *val_rtrn);

bool
فرچ_Resolve_Level(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                 xkb_level_index_t *level_rtrn);

تعداد xkb_parser_error
فرچ_Resolve_Group(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت فرچ_Def *فرچ, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
فرچ_Resolve_Group_Mask(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت فرچ_Def *فرچ, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
فرچ_Resolve_Button(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                  int64_t *btn_rtrn);

bool
فرچ_Resolve_String(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                  xkb_atom_t *val_rtrn);

bool
فرچ_Resolve_Enum(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                uint32_t *val_rtrn, ثابت LookupEntry *درجوجي);

bool
فرچ_Resolve_Mask(بنية xkb_context *ctx, ثابت فرچ_Def *فرچ,
                uint32_t *mask_rtrn, ثابت LookupEntry *درجوجي);
