/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#رهنمود once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
عبارت_Resolve_Lhs(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
               ثابت حرف **elem_rtrn, ثابت حرف **فیلد_rtrn,
               عبارت_Def **شاخص_rtrn);

bool
عبارت_Resolve_Mod_ماسک(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_مجموعه *mods,
                   xkb_mod_ماسک_t *ماسک_rtrn);

bool
عبارت_Resolve_Mod(بنية xkb_context *ctx, ثابت عبارت_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_مجموعه *mods,
               xkb_mod_شاخص_t *ndx_rtrn);

bool
عبارت_Resolve_Boolean(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   bool *مجموعه_rtrn);

bool
عبارت_Resolve_Integer(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   int64_t *val_rtrn);

bool
عبارت_Resolve_Level(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                 xkb_level_شاخص_t *level_rtrn);

تعداد xkb_parser_error
عبارت_Resolve_گروه(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت عبارت_Def *عبارت, bool absolute,
                 xkb_layout_شاخص_t *گروه_rtrn, bool *pending_rtrn);

bool
عبارت_Resolve_گروه_ماسک(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت عبارت_Def *عبارت, xkb_layout_ماسک_t *گروه_rtrn,
                     bool *pending_rtrn);

bool
عبارت_Resolve_Button(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  int64_t *btn_rtrn);

bool
عبارت_Resolve_String(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  xkb_atom_t *val_rtrn);

bool
عبارت_Resolve_Enum(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *val_rtrn, ثابت Lookup_مدخل *مقادیر);

bool
عبارت_Resolve_ماسک(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *ماسک_rtrn, ثابت Lookup_مدخل *مقادیر);
