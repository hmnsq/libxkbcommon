/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਸੰਘ xkb_action actions[ACTION_TYPE_ਅੰਕ_ENTRIES];
} ActionsInfo;

ਖਾਲੀ
InitActionsInfo(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ActionsInfo *info);

ਗਿਣਤੀ xkb_parser_error
HandleActionDef(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, ਵਿਅੰਜਕ_Def *def,
                ਸੰਘ xkb_action *action);

ਗਿਣਤੀ xkb_parser_error
ਸੈੱਟ_Default_Action_ਖੇਤਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                      ਅਚਲ ਅੱਖਰ *elem, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਵਿਅੰਜਕ_Def *array_ndx,
                      ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ);
