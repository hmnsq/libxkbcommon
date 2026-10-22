/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಒಕ್ಕೂಟ xkb_action actions[ACTION_TYPE_ಅಂಕೆ_ENTRIES];
} ActionsInfo;

ಶೂನ್ಯ
InitActionsInfo(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ActionsInfo *info);

ಎಣಿಕೆ xkb_parser_error
HandleActionDef(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, ಅಭಿವ್ಯಕ್ತಿ_Def *def,
                ಒಕ್ಕೂಟ xkb_action *action);

ಎಣಿಕೆ xkb_parser_error
ಗಣ_Default_Action_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ರಚನೆ xkb_mod_ಗಣ *mods,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ);
