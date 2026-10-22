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
व्यञ्जकम्_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **field_rtrn,
               व्यञ्जकम्_Def **index_rtrn);

bool
व्यञ्जकम्_Resolve_Mod_Mask(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
व्यञ्जकम्_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
व्यञ्जकम्_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   bool *set_rtrn);

bool
व्यञ्जकम्_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   int64_t *val_rtrn);

bool
व्यञ्जकम्_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                 xkb_level_index_t *level_rtrn);

गणना xkb_parser_error
व्यञ्जकम्_Resolve_Group(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
व्यञ्जकम्_Resolve_Group_Mask(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
व्यञ्जकम्_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  int64_t *btn_rtrn);

bool
व्यञ्जकम्_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  xkb_atom_t *val_rtrn);

bool
व्यञ्जकम्_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *val_rtrn, ध्रुव LookupEntry *मानानि);

bool
व्यञ्जकम्_Resolve_Mask(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *mask_rtrn, ध्रुव LookupEntry *मानानि);
