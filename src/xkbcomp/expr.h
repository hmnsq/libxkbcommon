/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
بیان_Resolve_Lhs(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
               ثابت حرف **elem_rtrn, ثابت حرف **ډګر_rtrn,
               بیان_Def **شاخص_rtrn);

bool
بیان_Resolve_Mod_ماسک(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سېټ *mods,
                   xkb_mod_ماسک_t *ماسک_rtrn);

bool
بیان_Resolve_Mod(بنية xkb_context *ctx, ثابت بیان_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سېټ *mods,
               xkb_mod_شاخص_t *ndx_rtrn);

bool
بیان_Resolve_Boolean(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   bool *سېټ_rtrn);

bool
بیان_Resolve_Integer(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   int64_t *val_rtrn);

bool
بیان_Resolve_Level(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                 xkb_level_شاخص_t *level_rtrn);

تعداد xkb_parser_error
بیان_Resolve_ډله(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بیان_Def *بیان, bool absolute,
                 xkb_layout_شاخص_t *ډله_rtrn, bool *pending_rtrn);

bool
بیان_Resolve_ډله_ماسک(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت بیان_Def *بیان, xkb_layout_ماسک_t *ډله_rtrn,
                     bool *pending_rtrn);

bool
بیان_Resolve_Button(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                  int64_t *btn_rtrn);

bool
بیان_Resolve_String(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                  xkb_atom_t *val_rtrn);

bool
بیان_Resolve_Enum(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                uint32_t *val_rtrn, ثابت Lookup_ثبت *ارزښتونه);

bool
بیان_Resolve_ماسک(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                uint32_t *ماسک_rtrn, ثابت Lookup_ثبت *ارزښتونه);
