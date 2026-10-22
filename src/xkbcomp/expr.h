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
রাশিমালা_Resolve_Lhs(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
               ধ্রুবক অক্ষর **elem_rtrn, ধ্রুবক অক্ষর **field_rtrn,
               রাশিমালা_Def **সূচক_rtrn);

bool
রাশিমালা_Resolve_Mod_Mask(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   গণনা mod_type mod_type, ধ্রুবক কাঠামো xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
রাশিমালা_Resolve_Mod(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুবক কাঠামো xkb_mod_set *mods,
               xkb_mod_সূচক_t *ndx_rtrn);

bool
রাশিমালা_Resolve_Boolean(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   bool *set_rtrn);

bool
রাশিমালা_Resolve_Integer(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   int64_t *val_rtrn);

bool
রাশিমালা_Resolve_Level(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                 xkb_level_সূচক_t *level_rtrn);

গণনা xkb_parser_error
রাশিমালা_Resolve_Group(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                 ধ্রুবক রাশিমালা_Def *রাশিমালা, bool absolute,
                 xkb_layout_সূচক_t *group_rtrn, bool *pending_rtrn);

bool
রাশিমালা_Resolve_Group_Mask(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                     ধ্রুবক রাশিমালা_Def *রাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
রাশিমালা_Resolve_Button(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                  int64_t *btn_rtrn);

bool
রাশিমালা_Resolve_String(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                  xkb_atom_t *val_rtrn);

bool
রাশিমালা_Resolve_Enum(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                uint32_t *val_rtrn, ধ্রুবক LookupEntry *মানসমূহ);

bool
রাশিমালা_Resolve_Mask(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                uint32_t *mask_rtrn, ধ্রুবক LookupEntry *মানসমূহ);
