/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "keymap.h"

/** LUT entry */
تعریفنوع ساختار {
    ثابت نویسه *name;
    uint32_t مقدار;
} LookupEntry;

bool
LookupString(ثابت LookupEntry tab[], ثابت نویسه *string,
              unsigned صحیح *مقدار_rtrn);

ثابت نویسه *
Lookup_مقدار(ثابت LookupEntry tab[], unsigned صحیح مقدار);

#define GROUP_LAST_INDEX_NAME "last"
شمارش {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

ایستا درونخطی uint8_t
format_control_names_offset(شمارش xkb_keymap_format format)
{
    بازگشت (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

بیرونی ثابت LookupEntry ctrlMaskNames[];
بیرونی ثابت LookupEntry modComponentMaskNames[];
بیرونی ثابت LookupEntry groupComponentMaskNames[];
بیرونی ثابت LookupEntry groupMaskNames[];
بیرونی ثابت LookupEntry buttonNames[];
بیرونی ثابت LookupEntry use_Mod_Map_مقدار_Names[];
بیرونی ثابت LookupEntry actionTypeNames[];
بیرونی ثابت LookupEntry symInterpretMatchMaskNames[];

ثابت نویسه *
ModMaskText(ساختار xkb_context *ctx, شمارش mod_type type,
            ثابت ساختار xkb_mod_set *mods, xkb_mod_mask_t mask);

ثابت نویسه *
ModIndexText(ساختار xkb_context *ctx, ثابت ساختار xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ثابت نویسه *
ActionTypeText(شمارش xkb_action_type type);

ثابت نویسه *
KeysymText(ساختار xkb_context *ctx, xkb_keysym_t sym);

ثابت نویسه *
کلید_Name_Text(ساختار xkb_context *ctx, xkb_atom_t name);

ثابت نویسه *
SIMatchText(شمارش xkb_match_operation type);

ثابت نویسه *
LedStateMaskText(ساختار xkb_context *ctx, ثابت LookupEntry *lookup,
                 شمارش xkb_state_component mask);

ثابت نویسه *
ControlMaskText(ساختار xkb_context *ctx, شمارش xkb_keymap_format format,
                شمارش xkb_action_controls mask);
