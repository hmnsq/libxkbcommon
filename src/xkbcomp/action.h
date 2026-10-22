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
    सङ्घ xkb_action actions[ACTION_TYPE_अङ्कः_ENTRIES];
} ActionsInfo;

शून्य
InitActionsInfo(ध्रुव संरचना xkb_keymap *keymap, ActionsInfo *info);

गणना xkb_parser_error
HandleActionDef(ध्रुव संरचना xkb_keymap_info *keymap_info, ActionsInfo *info,
                ध्रुव संरचना xkb_mod_set *mods, व्यञ्जकम्_Def *def,
                सङ्घ xkb_action *action);

गणना xkb_parser_error
SetDefaultActionField(ध्रुव संरचना xkb_keymap_info *keymap_info,
                      ActionsInfo *info, संरचना xkb_mod_set *mods,
                      ध्रुव वर्ण *elem, ध्रुव वर्ण *field, व्यञ्जकम्_Def *array_ndx,
                      व्यञ्जकम्_Def **मानम्_ptr, गणना merge_mode merge);
