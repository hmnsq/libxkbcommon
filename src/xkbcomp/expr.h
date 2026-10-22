/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "xkbcomp-priv.h"

bool
ExprResolveLhs(بنية xkb_context *ctx, ثابت ExprDef *expr,
               ثابت حرف **elem_rtrn, ثابت حرف **field_rtrn,
               ExprDef **index_rtrn);

bool
ExprResolveModMask(بنية xkb_context *ctx, ثابت ExprDef *expr,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
ExprResolveMod(بنية xkb_context *ctx, ثابت ExprDef *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
ExprResolveBoolean(بنية xkb_context *ctx, ثابت ExprDef *expr,
                   bool *set_rtrn);

bool
ExprResolveInteger(بنية xkb_context *ctx, ثابت ExprDef *expr,
                   int64_t *val_rtrn);

bool
ExprResolveLevel(بنية xkb_context *ctx, ثابت ExprDef *expr,
                 xkb_level_index_t *level_rtrn);

تعداد xkb_parser_error
ExprResolveGroup(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت ExprDef *expr, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
ExprResolveGroupMask(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت ExprDef *expr, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
ExprResolveButton(بنية xkb_context *ctx, ثابت ExprDef *expr,
                  int64_t *btn_rtrn);

bool
ExprResolveString(بنية xkb_context *ctx, ثابت ExprDef *expr,
                  xkb_atom_t *val_rtrn);

bool
ExprResolveEnum(بنية xkb_context *ctx, ثابت ExprDef *expr,
                uint32_t *val_rtrn, ثابت LookupEntry *values);

bool
ExprResolveMask(بنية xkb_context *ctx, ثابت ExprDef *expr,
                uint32_t *mask_rtrn, ثابت LookupEntry *values);
