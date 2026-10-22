/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
തരനിർവചനം ഘടന {
    സംഘം xkb_action actions[ACTION_TYPE_അക്കം_ENTRIES];
} ActionsInfo;

ശൂന്യം
InitActionsInfo(സ്ഥിരം ഘടന xkb_keymap *keymap, ActionsInfo *info);

എണ്ണൽ xkb_parser_error
HandleActionDef(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info, ActionsInfo *info,
                സ്ഥിരം ഘടന xkb_mod_set *mods, വ്യഞ്ജകം_Def *def,
                സംഘം xkb_action *action);

എണ്ണൽ xkb_parser_error
SetDefaultActionField(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ഘടന xkb_mod_set *mods,
                      സ്ഥിരം അക്ഷരം *elem, സ്ഥിരം അക്ഷരം *field, വ്യഞ്ജകം_Def *array_ndx,
                      വ്യഞ്ജകം_Def **മൂല്യം_ptr, എണ്ണൽ merge_mode merge);
