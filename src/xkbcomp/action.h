/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"
#شامل "ast.h"
#شامل "keymap.h"
#شامل "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
تعریفنوع ساختار {
    اجتماع xkb_action actions[ACTION_TYPE_رقم_ENTRIES];
} ActionsInfo;

تهی
InitActionsInfo(ثابت ساختار xkb_keymap *keymap, ActionsInfo *info);

شمارش xkb_parser_error
HandleActionDef(ثابت ساختار xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت ساختار xkb_mod_set *mods, عبارت_Def *def,
                اجتماع xkb_action *action);

شمارش xkb_parser_error
SetDefaultActionField(ثابت ساختار xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ساختار xkb_mod_set *mods,
                      ثابت نویسه *elem, ثابت نویسه *field, عبارت_Def *array_ndx,
                      عبارت_Def **مقدار_ptr, شمارش merge_mode merge);
