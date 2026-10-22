/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "ast.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "xkbcomp-priv.h"

bool
व्यंजक_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **field_rtrn,
               व्यंजक_Def **index_rtrn);

bool
व्यंजक_Resolve_Mod_Mask(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
व्यंजक_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
व्यंजक_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   bool *set_rtrn);

bool
व्यंजक_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   int64_t *val_rtrn);

bool
व्यंजक_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                 xkb_level_index_t *level_rtrn);

गणना xkb_parser_error
व्यंजक_Resolve_Group(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यंजक_Def *व्यंजक, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
व्यंजक_Resolve_Group_Mask(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यंजक_Def *व्यंजक, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
व्यंजक_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                  int64_t *btn_rtrn);

bool
व्यंजक_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                  xkb_atom_t *val_rtrn);

bool
व्यंजक_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                uint32_t *val_rtrn, ध्रुव LookupEntry *मूल्यावली);

bool
व्यंजक_Resolve_Mask(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                uint32_t *mask_rtrn, ध्रुव LookupEntry *मूल्यावली);
