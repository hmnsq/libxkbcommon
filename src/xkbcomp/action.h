/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
પ્રકારવ્યાખ્યા સંરચના {
    સંઘ xkb_action actions[ACTION_TYPE_અંક_ENTRIES];
} ActionsInfo;

શૂન્ય
InitActionsInfo(અચલ સંરચના xkb_keymap *keymap, ActionsInfo *info);

ગણના xkb_parser_error
HandleActionDef(અચલ સંરચના xkb_keymap_info *keymap_info, ActionsInfo *info,
                અચલ સંરચના xkb_mod_set *mods, પદાવલિ_Def *def,
                સંઘ xkb_action *action);

ગણના xkb_parser_error
SetDefaultActionField(અચલ સંરચના xkb_keymap_info *keymap_info,
                      ActionsInfo *info, સંરચના xkb_mod_set *mods,
                      અચલ અક્ષર *elem, અચલ અક્ષર *field, પદાવલિ_Def *array_ndx,
                      પદાવલિ_Def **કિંમત_ptr, ગણના merge_mode merge);
