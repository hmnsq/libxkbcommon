/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
প্রকারনির্দেশ সংরচনা {
    সঙ্ঘ xkb_action actions[ACTION_TYPE_অঙ্ক_ENTRIES];
} ActionsInfo;

শূন্য
InitActionsInfo(ধ্রুৱ সংরচনা xkb_keymap *keymap, ActionsInfo *info);

গণনা xkb_parser_error
HandleActionDef(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info, ActionsInfo *info,
                ধ্রুৱ সংরচনা xkb_mod_set *mods, ৰাশিমালা_Def *def,
                সঙ্ঘ xkb_action *action);

গণনা xkb_parser_error
SetDefaultActionField(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                      ActionsInfo *info, সংরচনা xkb_mod_set *mods,
                      ধ্রুৱ ৱর্ণ *elem, ধ্রুৱ ৱর্ণ *field, ৰাশিমালা_Def *array_ndx,
                      ৰাশিমালা_Def **মান_ptr, গণনা merge_mode merge);
