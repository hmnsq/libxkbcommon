/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "ast.h"
#समावेश "keymap.h"
#समावेश "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
प्रकार काष्ठा {
    जोड़ xkb_action actions[ACTION_TYPE_अंक_ENTRIES];
} ActionsInfo;

व्योम
InitActionsInfo(स्थिर काष्ठा xkb_keymap *keymap, ActionsInfo *info);

क्रमागत xkb_parser_error
HandleActionDef(स्थिर काष्ठा xkb_keymap_info *keymap_info, ActionsInfo *info,
                स्थिर काष्ठा xkb_mod_set *mods, व्यंजक_Def *def,
                जोड़ xkb_action *action);

क्रमागत xkb_parser_error
SetDefaultActionField(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                      ActionsInfo *info, काष्ठा xkb_mod_set *mods,
                      स्थिर अक्षर *elem, स्थिर अक्षर *field, व्यंजक_Def *array_ndx,
                      व्यंजक_Def **मूल्य_ptr, क्रमागत merge_mode merge);
