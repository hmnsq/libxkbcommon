/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
প্রকারলেখ কাঠামো {
    সংঘ xkb_action actions[ACTION_TYPE_অঙ্ক_ENTRIES];
} ActionsInfo;

শূন্য
InitActionsInfo(ধ্রুবক কাঠামো xkb_keymap *keymap, ActionsInfo *info);

গণনা xkb_parser_error
HandleActionDef(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info, ActionsInfo *info,
                ধ্রুবক কাঠামো xkb_mod_set *mods, রাশিমালা_Def *def,
                সংঘ xkb_action *action);

গণনা xkb_parser_error
SetDefaultActionField(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                      ActionsInfo *info, কাঠামো xkb_mod_set *mods,
                      ধ্রুবক অক্ষর *elem, ধ্রুবক অক্ষর *field, রাশিমালা_Def *array_ndx,
                      রাশিমালা_Def **মান_ptr, গণনা merge_mode merge);
