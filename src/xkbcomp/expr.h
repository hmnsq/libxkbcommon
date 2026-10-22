/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#هدايت once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
اظهار_Resolve_Lhs(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
               ثابت حرف **elem_rtrn, ثابت حرف **ميدان_rtrn,
               اظهار_Def **اشاريو_rtrn);

bool
اظهار_Resolve_Mod_نقاب(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سيٽ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn);

bool
اظهار_Resolve_Mod(بنية xkb_context *ctx, ثابت اظهار_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سيٽ *mods,
               xkb_mod_اشاريو_t *ndx_rtrn);

bool
اظهار_Resolve_Boolean(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   bool *سيٽ_rtrn);

bool
اظهار_Resolve_Integer(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   int64_t *val_rtrn);

bool
اظهار_Resolve_Level(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                 xkb_level_اشاريو_t *level_rtrn);

تعداد xkb_parser_error
اظهار_Resolve_گروهه(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت اظهار_Def *اظهار, bool absolute,
                 xkb_layout_اشاريو_t *گروهه_rtrn, bool *pending_rtrn);

bool
اظهار_Resolve_گروهه_نقاب(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت اظهار_Def *اظهار, xkb_layout_نقاب_t *گروهه_rtrn,
                     bool *pending_rtrn);

bool
اظهار_Resolve_Button(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                  int64_t *btn_rtrn);

bool
اظهار_Resolve_String(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                  xkb_atom_t *val_rtrn);

bool
اظهار_Resolve_Enum(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                uint32_t *val_rtrn, ثابت Lookup_داخلا *قدرون);

bool
اظهار_Resolve_نقاب(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                uint32_t *نقاب_rtrn, ثابت Lookup_داخلا *قدرون);
