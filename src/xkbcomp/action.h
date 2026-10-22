/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "xkbcomp-priv.h"

/*
 * This struct contains the default values which every new action
 * (e.g. in an interpret statement) starts off with. It can be
 * modified within the files (see calls to SetDefaultActionField).
 */
வகைவரை கட்டமைப்பு {
    ஒன்றிப்பு xkb_action actions[ACTION_TYPE_இலக்கம்_ENTRIES];
} ActionsInfo;

வெறுமை
InitActionsInfo(மாறிலி கட்டமைப்பு xkb_keymap *keymap, ActionsInfo *info);

எண்ணிக்கை xkb_parser_error
HandleActionDef(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info, ActionsInfo *info,
                மாறிலி கட்டமைப்பு xkb_mod_set *mods, கோவை_Def *def,
                ஒன்றிப்பு xkb_action *action);

எண்ணிக்கை xkb_parser_error
SetDefaultActionField(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                      ActionsInfo *info, கட்டமைப்பு xkb_mod_set *mods,
                      மாறிலி எழுத்து *elem, மாறிலி எழுத்து *field, கோவை_Def *array_ndx,
                      கோவை_Def **மதிப்பு_ptr, எண்ணிக்கை merge_mode merge);
