/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

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
قسمتعریف ساخت {
    اتحاد xkb_action actions[ACTION_TYPE_عدد_ENTRIES];
} ActionsInfo;

خالی
InitActionsInfo(ثابت ساخت xkb_keymap *keymap, ActionsInfo *info);

شمار xkb_parser_error
HandleActionDef(ثابت ساخت xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت ساخت xkb_mod_سیٹ *mods, اظہاریہ_Def *def,
                اتحاد xkb_action *action);

شمار xkb_parser_error
سیٹ_Default_Action_میدان(ثابت ساخت xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ساخت xkb_mod_سیٹ *mods,
                      ثابت حرف *elem, ثابت حرف *میدان_2, اظہاریہ_Def *array_ndx,
                      اظہاریہ_Def **قدر_ptr, شمار ملاؤ_منوال ملاؤ);
