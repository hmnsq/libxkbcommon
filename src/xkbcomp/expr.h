/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

bool
ପଦସମଷ୍ଟି_Resolve_Lhs(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
               ଅଚଳ ଅକ୍ଷର **elem_rtrn, ଅଚଳ ଅକ୍ଷର **field_rtrn,
               ପଦସମଷ୍ଟି_Def **index_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Mod_Mask(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   ଗଣନା mod_type mod_type, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Mod(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *def,
               ଗଣନା mod_type mod_type, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Boolean(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   bool *set_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Integer(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   int64_t *val_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Level(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                 xkb_level_index_t *level_rtrn);

ଗଣନା xkb_parser_error
ପଦସମଷ୍ଟି_Resolve_Group(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                 ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Group_Mask(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                     ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Button(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                  int64_t *btn_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_String(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                  xkb_atom_t *val_rtrn);

bool
ପଦସମଷ୍ଟି_Resolve_Enum(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                uint32_t *val_rtrn, ଅଚଳ LookupEntry *ମୂଲ୍ୟଗୁଡ଼ିକ);

bool
ପଦସମଷ୍ଟି_Resolve_Mask(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                uint32_t *mask_rtrn, ଅଚଳ LookupEntry *ମୂଲ୍ୟଗୁଡ଼ିକ);
