/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل "ast.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "xkbcomp-priv.h"

bool
عبارت_Resolve_Lhs(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
               ثابت نویسه **elem_rtrn, ثابت نویسه **field_rtrn,
               عبارت_Def **index_rtrn);

bool
عبارت_Resolve_Mod_Mask(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   شمارش mod_type mod_type, ثابت ساختار xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
عبارت_Resolve_Mod(ساختار xkb_context *ctx, ثابت عبارت_Def *def,
               شمارش mod_type mod_type, ثابت ساختار xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
عبارت_Resolve_Boolean(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   bool *set_rtrn);

bool
عبارت_Resolve_Integer(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   int64_t *val_rtrn);

bool
عبارت_Resolve_Level(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                 xkb_level_index_t *level_rtrn);

شمارش xkb_parser_error
عبارت_Resolve_Group(ثابت ساختار xkb_keymap_info *keymap_info,
                 ثابت عبارت_Def *عبارت, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
عبارت_Resolve_Group_Mask(ثابت ساختار xkb_keymap_info *keymap_info,
                     ثابت عبارت_Def *عبارت, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
عبارت_Resolve_Button(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  int64_t *btn_rtrn);

bool
عبارت_Resolve_String(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  xkb_atom_t *val_rtrn);

bool
عبارت_Resolve_Enum(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *val_rtrn, ثابت LookupEntry *مقادیر);

bool
عبارت_Resolve_Mask(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *mask_rtrn, ثابت LookupEntry *مقادیر);
