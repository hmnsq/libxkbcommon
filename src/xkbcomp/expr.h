/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include "xkbcommon/xkbcommon.h"

#include "ast.h"
#include "keymap.h"
#include "text.h"
#include "xkbcomp-priv.h"

bool
पाहायाग_Resolve_Lhs(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
               const char **elem_rtrn, const char **field_rtrn,
               पाहायाग_Def **index_rtrn);

bool
पाहायाग_Resolve_Mod_Mask(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   enum mod_type mod_type, const struct xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
पाहायाग_Resolve_Mod(struct xkb_context *ctx, const पाहायाग_Def *def,
               enum mod_type mod_type, const struct xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
पाहायाग_Resolve_Boolean(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   bool *set_rtrn);

bool
पाहायाग_Resolve_Integer(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   int64_t *val_rtrn);

bool
पाहायाग_Resolve_Level(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                 xkb_level_index_t *level_rtrn);

enum xkb_parser_error
पाहायाग_Resolve_Group(const struct xkb_keymap_info *keymap_info,
                 const पाहायाग_Def *पाहायाग, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
पाहायाग_Resolve_Group_Mask(const struct xkb_keymap_info *keymap_info,
                     const पाहायाग_Def *पाहायाग, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
पाहायाग_Resolve_Button(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                  int64_t *btn_rtrn);

bool
पाहायाग_Resolve_String(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                  xkb_atom_t *val_rtrn);

bool
पाहायाग_Resolve_Enum(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                uint32_t *val_rtrn, const LookupEntry *मगा_हालागा);

bool
पाहायाग_Resolve_Mask(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                uint32_t *mask_rtrn, const LookupEntry *मगा_हालागा);
