/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "ast.h"
#ضمن "keymap.h"
#ضمن "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
تعريفنوع بنية {
    اتحاد xkb_action actions[ACTION_TYPE_عدد_ENTRIES];
} ActionsInfo;

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info);

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_سېټ *mods, بیان_Def *def,
                اتحاد xkb_action *action);

تعداد xkb_parser_error
سېټ_Default_Action_ډګر(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_سېټ *mods,
                      ثابت حرف *elem, ثابت حرف *ډګر_2, بیان_Def *array_ndx,
                      بیان_Def **ارزښت_ptr, تعداد یوځای_ډېرپېښ یوځای);
