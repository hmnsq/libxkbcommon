/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "xkbcomp-priv.h"

bool
கோவை_Resolve_Lhs(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
               மாறிலி எழுத்து **elem_rtrn, மாறிலி எழுத்து **field_rtrn,
               கோவை_Def **index_rtrn);

bool
கோவை_Resolve_Mod_Mask(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   எண்ணிக்கை mod_type mod_type, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
கோவை_Resolve_Mod(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *def,
               எண்ணிக்கை mod_type mod_type, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
கோவை_Resolve_Boolean(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   bool *set_rtrn);

bool
கோவை_Resolve_Integer(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   int64_t *val_rtrn);

bool
கோவை_Resolve_Level(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                 xkb_level_index_t *level_rtrn);

எண்ணிக்கை xkb_parser_error
கோவை_Resolve_Group(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                 மாறிலி கோவை_Def *கோவை, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
கோவை_Resolve_Group_Mask(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                     மாறிலி கோவை_Def *கோவை, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
கோவை_Resolve_Button(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                  int64_t *btn_rtrn);

bool
கோவை_Resolve_String(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                  xkb_atom_t *val_rtrn);

bool
கோவை_Resolve_Enum(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                uint32_t *val_rtrn, மாறிலி LookupEntry *மதிப்புகள்);

bool
கோவை_Resolve_Mask(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                uint32_t *mask_rtrn, மாறிலி LookupEntry *மதிப்புகள்);
