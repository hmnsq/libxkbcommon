/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"

bool
ৰাশিমালা_Resolve_Lhs(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
               ধ্রুৱ ৱর্ণ **elem_rtrn, ধ্রুৱ ৱর্ণ **field_rtrn,
               ৰাশিমালা_Def **index_rtrn);

bool
ৰাশিমালা_Resolve_Mod_Mask(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   গণনা mod_type mod_type, ধ্রুৱ সংরচনা xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
ৰাশিমালা_Resolve_Mod(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুৱ সংরচনা xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
ৰাশিমালা_Resolve_Boolean(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   bool *set_rtrn);

bool
ৰাশিমালা_Resolve_Integer(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   int64_t *val_rtrn);

bool
ৰাশিমালা_Resolve_Level(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                 xkb_level_index_t *level_rtrn);

গণনা xkb_parser_error
ৰাশিমালা_Resolve_Group(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                 ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
ৰাশিমালা_Resolve_Group_Mask(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                     ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
ৰাশিমালা_Resolve_Button(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                  int64_t *btn_rtrn);

bool
ৰাশিমালা_Resolve_String(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                  xkb_atom_t *val_rtrn);

bool
ৰাশিমালা_Resolve_Enum(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                uint32_t *val_rtrn, ধ্রুৱ LookupEntry *মানসমূহ);

bool
ৰাশিমালা_Resolve_Mask(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                uint32_t *mask_rtrn, ধ্রুৱ LookupEntry *মানসমূহ);
