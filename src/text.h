/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"

/** LUT entry */
تعريفنوع بنية {
    ثابت حرف *name;
    uint32_t قدر;
} LookupEntry;

bool
LookupString(ثابت LookupEntry tab[], ثابت حرف *string,
              unsigned صحيح *قدر_rtrn);

ثابت حرف *
Lookup_قدر(ثابت LookupEntry tab[], unsigned صحيح قدر);

#define GROUP_LAST_INDEX_NAME "last"
تعداد {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

ساكن مضمن uint8_t
format_control_names_offset(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

خارجي ثابت LookupEntry ctrlMaskNames[];
خارجي ثابت LookupEntry modComponentMaskNames[];
خارجي ثابت LookupEntry groupComponentMaskNames[];
خارجي ثابت LookupEntry groupMaskNames[];
خارجي ثابت LookupEntry buttonNames[];
خارجي ثابت LookupEntry use_Mod_Map_قدر_Names[];
خارجي ثابت LookupEntry actionTypeNames[];
خارجي ثابت LookupEntry symInterpretMatchMaskNames[];

ثابت حرف *
ModMaskText(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_set *mods, xkb_mod_mask_t mask);

ثابت حرف *
ModIndexText(بنية xkb_context *ctx, ثابت بنية xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ثابت حرف *
ActionTypeText(تعداد xkb_action_type type);

ثابت حرف *
KeysymText(بنية xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
کلید_Name_Text(بنية xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatchText(تعداد xkb_match_operation type);

ثابت حرف *
LedStateMaskText(بنية xkb_context *ctx, ثابت LookupEntry *lookup,
                 تعداد xkb_state_component mask);

ثابت حرف *
ControlMaskText(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls mask);
