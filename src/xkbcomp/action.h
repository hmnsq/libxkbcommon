/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಸಙ್ಘ xkb_action actions[ACTION_TYPE_ಅಂಕೆ_ENTRIES];
} ActionsInfo;

ಶೂನ್ಯ
InitActionsInfo(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ActionsInfo *info);

ಗಣನಾ xkb_parser_error
HandleActionDef(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, ಅಭಿವ್ಯಕ್ತಿ_Def *def,
                ಸಙ್ಘ xkb_action *action);

ಗಣನಾ xkb_parser_error
SetDefaultActionField(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ಸಂರಚನಾ xkb_mod_set *mods,
                      ಧ್ರುವ ವರ್ಣ *elem, ಧ್ರುವ ವರ್ಣ *field, ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr, ಗಣನಾ merge_mode merge);
