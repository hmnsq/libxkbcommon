/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل "ast.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "xkbcomp-priv.h"

bool
اظہاریہ_Resolve_Lhs(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
               ثابت حرف **elem_rtrn, ثابت حرف **میدان_rtrn,
               اظہاریہ_Def **اشاریہ_rtrn);

bool
اظہاریہ_Resolve_Mod_نقاب(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   شمار mod_type mod_type, ثابت ساخت xkb_mod_سیٹ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn);

bool
اظہاریہ_Resolve_Mod(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *def,
               شمار mod_type mod_type, ثابت ساخت xkb_mod_سیٹ *mods,
               xkb_mod_اشاریہ_t *ndx_rtrn);

bool
اظہاریہ_Resolve_Boolean(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   bool *سیٹ_rtrn);

bool
اظہاریہ_Resolve_Integer(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   int64_t *val_rtrn);

bool
اظہاریہ_Resolve_Level(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                 xkb_level_اشاریہ_t *level_rtrn);

شمار xkb_parser_error
اظہاریہ_Resolve_گروہ(ثابت ساخت xkb_keymap_info *keymap_info,
                 ثابت اظہاریہ_Def *اظہاریہ, bool absolute,
                 xkb_layout_اشاریہ_t *گروہ_rtrn, bool *pending_rtrn);

bool
اظہاریہ_Resolve_گروہ_نقاب(ثابت ساخت xkb_keymap_info *keymap_info,
                     ثابت اظہاریہ_Def *اظہاریہ, xkb_layout_نقاب_t *گروہ_rtrn,
                     bool *pending_rtrn);

bool
اظہاریہ_Resolve_Button(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  int64_t *btn_rtrn);

bool
اظہاریہ_Resolve_String(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  xkb_atom_t *val_rtrn);

bool
اظہاریہ_Resolve_Enum(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *val_rtrn, ثابت Lookup_اندراج *اقدار);

bool
اظہاریہ_Resolve_نقاب(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *نقاب_rtrn, ثابت Lookup_اندراج *اقدار);
