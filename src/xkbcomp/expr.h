/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

bool
വ്യഞ്ജകം_Resolve_Lhs(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
               സ്ഥിരം അക്ഷരം **elem_rtrn, സ്ഥിരം അക്ഷരം **field_rtrn,
               വ്യഞ്ജകം_Def **index_rtrn);

bool
വ്യഞ്ജകം_Resolve_Mod_Mask(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   എണ്ണൽ mod_type mod_type, സ്ഥിരം ഘടന xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn);

bool
വ്യഞ്ജകം_Resolve_Mod(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *def,
               എണ്ണൽ mod_type mod_type, സ്ഥിരം ഘടന xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn);

bool
വ്യഞ്ജകം_Resolve_Boolean(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   bool *set_rtrn);

bool
വ്യഞ്ജകം_Resolve_Integer(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   int64_t *val_rtrn);

bool
വ്യഞ്ജകം_Resolve_Level(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                 xkb_level_index_t *level_rtrn);

എണ്ണൽ xkb_parser_error
വ്യഞ്ജകം_Resolve_Group(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                 സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending_rtrn);

bool
വ്യഞ്ജകം_Resolve_Group_Mask(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                     സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn);

bool
വ്യഞ്ജകം_Resolve_Button(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                  int64_t *btn_rtrn);

bool
വ്യഞ്ജകം_Resolve_String(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                  xkb_atom_t *val_rtrn);

bool
വ്യഞ്ജകം_Resolve_Enum(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                uint32_t *val_rtrn, സ്ഥിരം LookupEntry *മൂല്യങ്ങൾ);

bool
വ്യഞ്ജകം_Resolve_Mask(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                uint32_t *mask_rtrn, സ്ഥിരം LookupEntry *മൂല്യങ്ങൾ);
