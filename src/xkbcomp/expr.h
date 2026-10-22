/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
اظہاریہ_Resolve_Lhs(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
               ثابت حرف **elem_rtrn, ثابت حرف **میدان_rtrn,
               اظہاریہ_Def **اشاریہ_rtrn);

bool
اظہاریہ_Resolve_Mod_نقاب(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سیٹ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn);

bool
اظہاریہ_Resolve_Mod(بنية xkb_context *ctx, ثابت اظہاریہ_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سیٹ *mods,
               xkb_mod_اشاریہ_t *ndx_rtrn);

bool
اظہاریہ_Resolve_Boolean(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   bool *سیٹ_rtrn);

bool
اظہاریہ_Resolve_Integer(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   int64_t *val_rtrn);

bool
اظہاریہ_Resolve_Level(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                 xkb_level_اشاریہ_t *level_rtrn);

تعداد xkb_parser_error
اظہاریہ_Resolve_گروہ(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت اظہاریہ_Def *اظہاریہ, bool absolute,
                 xkb_layout_اشاریہ_t *گروہ_rtrn, bool *pending_rtrn);

bool
اظہاریہ_Resolve_گروہ_نقاب(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت اظہاریہ_Def *اظہاریہ, xkb_layout_نقاب_t *گروہ_rtrn,
                     bool *pending_rtrn);

bool
اظہاریہ_Resolve_Button(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  int64_t *btn_rtrn);

bool
اظہاریہ_Resolve_String(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  xkb_atom_t *val_rtrn);

bool
اظہاریہ_Resolve_Enum(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *val_rtrn, ثابت Lookup_اندراج *اقدار);

bool
اظہاریہ_Resolve_نقاب(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *نقاب_rtrn, ثابت Lookup_اندراج *اقدار);
