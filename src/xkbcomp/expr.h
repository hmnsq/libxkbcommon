/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

bool
ਵਿਅੰਜਕ_Resolve_Lhs(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
               ਅਚਲ ਅੱਖਰ **elem_rtrn, ਅਚਲ ਅੱਖਰ **ਖੇਤਰ_rtrn,
               ਵਿਅੰਜਕ_Def **ਸੂਚਕਾਂਕ_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   ਗਿਣਤੀ mod_type mod_type, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                   xkb_mod_ਨਕਾਬ_t *ਨਕਾਬ_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Mod(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *def,
               ਗਿਣਤੀ mod_type mod_type, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
               xkb_mod_ਸੂਚਕਾਂਕ_t *ndx_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Boolean(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   bool *ਸੈੱਟ_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Integer(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   int64_t *val_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Level(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                 xkb_level_ਸੂਚਕਾਂਕ_t *level_rtrn);

ਗਿਣਤੀ xkb_parser_error
ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                 ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, bool absolute,
                 xkb_layout_ਸੂਚਕਾਂਕ_t *ਸਮੂਹ_rtrn, bool *pending_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ_ਨਕਾਬ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                     ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, xkb_layout_ਨਕਾਬ_t *ਸਮੂਹ_rtrn,
                     bool *pending_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Button(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                  int64_t *btn_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_String(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                  xkb_atom_t *val_rtrn);

bool
ਵਿਅੰਜਕ_Resolve_Enum(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                uint32_t *val_rtrn, ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਮੁੱਲਾਂ);

bool
ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                uint32_t *ਨਕਾਬ_rtrn, ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਮੁੱਲਾਂ);
