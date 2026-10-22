/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
प्रकारव्याख्या संरचना {
    संघ xkb_action actions[ACTION_TYPE_अंक_ENTRIES];
} ActionsInfo;

शून्य
InitActionsInfo(स्थिरांक संरचना xkb_keymap *keymap, ActionsInfo *info);

गणना xkb_parser_error
HandleActionDef(स्थिरांक संरचना xkb_keymap_info *keymap_info, ActionsInfo *info,
                स्थिरांक संरचना xkb_mod_set *mods, अभिव्यक्ती_Def *def,
                संघ xkb_action *action);

गणना xkb_parser_error
SetDefaultActionField(स्थिरांक संरचना xkb_keymap_info *keymap_info,
                      ActionsInfo *info, संरचना xkb_mod_set *mods,
                      स्थिरांक अक्षर *elem, स्थिरांक अक्षर *field, अभिव्यक्ती_Def *array_ndx,
                      अभिव्यक्ती_Def **मूल्य_ptr, गणना merge_mode merge);
