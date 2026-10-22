/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ସଂଘ xkb_action actions[ACTION_TYPE_ଅଙ୍କ_ENTRIES];
} ActionsInfo;

ଶୂନ୍ୟ
InitActionsInfo(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ActionsInfo *info);

ଗଣନା xkb_parser_error
HandleActionDef(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info, ActionsInfo *info,
                ଅଚଳ ସଂରଚନା xkb_mod_set *mods, ପଦସମଷ୍ଟି_Def *def,
                ସଂଘ xkb_action *action);

ଗଣନା xkb_parser_error
SetDefaultActionField(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ସଂରଚନା xkb_mod_set *mods,
                      ଅଚଳ ଅକ୍ଷର *elem, ଅଚଳ ଅକ୍ଷର *field, ପଦସମଷ୍ଟି_Def *array_ndx,
                      ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr, ଗଣନା merge_mode merge);
