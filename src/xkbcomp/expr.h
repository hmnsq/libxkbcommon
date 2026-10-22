/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "xkbcomp-priv.h"

bool
પદાવલિ_Resolve_Lhs(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
               અચલ અક્ષર **elem_rtrn, અચલ અક્ષર **field_rtrn,
               પદાવલિ_Def **index_rtrn);

bool
પદાવલિ_Resolve_Mod_Mask(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   ગણના mod_type mod_type, અચલ સંરચના xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
પદાવલિ_Resolve_Mod(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *def,
               ગણના mod_type mod_type, અચલ સંરચના xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
પદાવલિ_Resolve_Boolean(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   bool *set_rtrn);

bool
પદાવલિ_Resolve_Integer(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   int64_t *val_rtrn);

bool
પદાવલિ_Resolve_Level(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                 xkb_level_index_t *level_rtrn);

ગણના xkb_parser_error
પદાવલિ_Resolve_Group(અચલ સંરચના xkb_keymap_info *keymap_info,
                 અચલ પદાવલિ_Def *પદાવલિ, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
પદાવલિ_Resolve_Group_Mask(અચલ સંરચના xkb_keymap_info *keymap_info,
                     અચલ પદાવલિ_Def *પદાવલિ, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
પદાવલિ_Resolve_Button(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                  int64_t *btn_rtrn);

bool
પદાવલિ_Resolve_String(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                  xkb_atom_t *val_rtrn);

bool
પદાવલિ_Resolve_Enum(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                uint32_t *val_rtrn, અચલ LookupEntry *કિંમતો);

bool
પદાવલિ_Resolve_Mask(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                uint32_t *mask_rtrn, અચલ LookupEntry *કિંમતો);
