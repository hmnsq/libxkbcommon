/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "xkbcomp-priv.h"

bool
রাশিমালা_Resolve_Lhs(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
               ধ্রুব বর্ণ **elem_rtrn, ধ্রুব বর্ণ **field_rtrn,
               রাশিমালা_Def **index_rtrn);

bool
রাশিমালা_Resolve_Mod_Mask(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   গণনা mod_type mod_type, ধ্রুব সংরচনা xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
রাশিমালা_Resolve_Mod(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুব সংরচনা xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
রাশিমালা_Resolve_Boolean(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   bool *set_rtrn);

bool
রাশিমালা_Resolve_Integer(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   int64_t *val_rtrn);

bool
রাশিমালা_Resolve_Level(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                 xkb_level_index_t *level_rtrn);

গণনা xkb_parser_error
রাশিমালা_Resolve_Group(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                 ধ্রুব রাশিমালা_Def *রাশিমালা, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
রাশিমালা_Resolve_Group_Mask(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                     ধ্রুব রাশিমালা_Def *রাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
রাশিমালা_Resolve_Button(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                  int64_t *btn_rtrn);

bool
রাশিমালা_Resolve_String(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                  xkb_atom_t *val_rtrn);

bool
রাশিমালা_Resolve_Enum(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                uint32_t *val_rtrn, ধ্রুব LookupEntry *মানসমূহ);

bool
রাশিমালা_Resolve_Mask(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                uint32_t *mask_rtrn, ধ্রুব LookupEntry *মানসমূহ);
