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
प्रकारनिर्देश संरचना {
    सङ्घ xkb_action actions[ACTION_TYPE_अङ्क_ENTRIES];
} ActionsInfo;

शून्य
InitActionsInfo(अचर संरचना xkb_keymap *keymap, ActionsInfo *info);

गणना xkb_parser_error
HandleActionDef(अचर संरचना xkb_keymap_info *keymap_info, ActionsInfo *info,
                अचर संरचना xkb_mod_set *mods, अभिव्यक्ति_Def *def,
                सङ्घ xkb_action *action);

गणना xkb_parser_error
SetDefaultActionField(अचर संरचना xkb_keymap_info *keymap_info,
                      ActionsInfo *info, संरचना xkb_mod_set *mods,
                      अचर वर्ण *elem, अचर वर्ण *field, अभिव्यक्ति_Def *array_ndx,
                      अभिव्यक्ति_Def **मान_ptr, गणना merge_mode merge);
