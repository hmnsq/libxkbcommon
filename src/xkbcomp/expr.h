/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"

#समावेश "ast.h"
#समावेश "keymap.h"
#समावेश "text.h"
#समावेश "xkbcomp-priv.h"

bool
व्यंजक_Resolve_Lhs(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
               स्थिर अक्षर **elem_rtrn, स्थिर अक्षर **field_rtrn,
               व्यंजक_Def **सूचकांक_rtrn);

bool
व्यंजक_Resolve_Mod_Mask(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   क्रमागत mod_type mod_type, स्थिर काष्ठा xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
व्यंजक_Resolve_Mod(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *def,
               क्रमागत mod_type mod_type, स्थिर काष्ठा xkb_mod_set *mods,
               xkb_mod_सूचकांक_t *ndx_rtrn);

bool
व्यंजक_Resolve_Boolean(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   bool *set_rtrn);

bool
व्यंजक_Resolve_Integer(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   int64_t *val_rtrn);

bool
व्यंजक_Resolve_Level(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                 xkb_level_सूचकांक_t *level_rtrn);

क्रमागत xkb_parser_error
व्यंजक_Resolve_Group(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                 स्थिर व्यंजक_Def *व्यंजक, bool absolute,
                 xkb_layout_सूचकांक_t *group_rtrn, bool *pending_rtrn);

bool
व्यंजक_Resolve_Group_Mask(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                     स्थिर व्यंजक_Def *व्यंजक, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
व्यंजक_Resolve_Button(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                  int64_t *btn_rtrn);

bool
व्यंजक_Resolve_String(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                  xkb_atom_t *val_rtrn);

bool
व्यंजक_Resolve_Enum(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                uint32_t *val_rtrn, स्थिर LookupEntry *मूल्यावली);

bool
व्यंजक_Resolve_Mask(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                uint32_t *mask_rtrn, स्थिर LookupEntry *मूल्यावली);
