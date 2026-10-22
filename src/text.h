/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "keymap.h"

/** LUT entry */
வகைவரை கட்டமைப்பு {
    மாறிலி எழுத்து *name;
    uint32_t மதிப்பு;
} LookupEntry;

bool
LookupString(மாறிலி LookupEntry tab[], மாறிலி எழுத்து *string,
              unsigned முழு *மதிப்பு_rtrn);

மாறிலி எழுத்து *
Lookup_மதிப்பு(மாறிலி LookupEntry tab[], unsigned முழு மதிப்பு);

#define GROUP_LAST_INDEX_NAME "last"
எண்ணிக்கை {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

நிலையான உள்வரி uint8_t
format_control_names_offset(எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

வெளி மாறிலி LookupEntry ctrlMaskNames[];
வெளி மாறிலி LookupEntry modComponentMaskNames[];
வெளி மாறிலி LookupEntry groupComponentMaskNames[];
வெளி மாறிலி LookupEntry groupMaskNames[];
வெளி மாறிலி LookupEntry buttonNames[];
வெளி மாறிலி LookupEntry use_Mod_Map_மதிப்பு_Names[];
வெளி மாறிலி LookupEntry actionTypeNames[];
வெளி மாறிலி LookupEntry symInterpretMatchMaskNames[];

மாறிலி எழுத்து *
ModMaskText(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை mod_type type,
            மாறிலி கட்டமைப்பு xkb_mod_set *mods, xkb_mod_mask_t mask);

மாறிலி எழுத்து *
ModIndexText(கட்டமைப்பு xkb_context *ctx, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
             xkb_mod_index_t ndx);

மாறிலி எழுத்து *
ActionTypeText(எண்ணிக்கை xkb_action_type type);

மாறிலி எழுத்து *
KeysymText(கட்டமைப்பு xkb_context *ctx, xkb_keysym_t sym);

மாறிலி எழுத்து *
சாவி_Name_Text(கட்டமைப்பு xkb_context *ctx, xkb_atom_t name);

மாறிலி எழுத்து *
SIMatchText(எண்ணிக்கை xkb_match_operation type);

மாறிலி எழுத்து *
LedStateMaskText(கட்டமைப்பு xkb_context *ctx, மாறிலி LookupEntry *lookup,
                 எண்ணிக்கை xkb_state_component mask);

மாறிலி எழுத்து *
ControlMaskText(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_keymap_format format,
                எண்ணிக்கை xkb_action_controls mask);
