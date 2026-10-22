/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"

/** LUT entry */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಧ್ರುವ ವರ್ಣ *name;
    uint32_t ಮೌಲ್ಯ;
} LookupEntry;

bool
LookupString(ಧ್ರುವ LookupEntry tab[], ಧ್ರುವ ವರ್ಣ *string,
              unsigned ಪೂರ್ಣಾಙ್ಕ *ಮೌಲ್ಯ_rtrn);

ಧ್ರುವ ವರ್ಣ *
Lookup_ಮೌಲ್ಯ(ಧ್ರುವ LookupEntry tab[], unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ);

#define GROUP_LAST_INDEX_NAME "last"
ಗಣನಾ {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint8_t
format_control_names_offset(ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

ಬಾಹ್ಯ ಧ್ರುವ LookupEntry ctrlMaskNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry modComponentMaskNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry groupComponentMaskNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry groupMaskNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry buttonNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry use_Mod_Map_ಮೌಲ್ಯ_Names[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry actionTypeNames[];
ಬಾಹ್ಯ ಧ್ರುವ LookupEntry symInterpretMatchMaskNames[];

ಧ್ರುವ ವರ್ಣ *
ModMaskText(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ mod_type type,
            ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, xkb_mod_mask_t mask);

ಧ್ರುವ ವರ್ಣ *
ModIndexText(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ಧ್ರುವ ವರ್ಣ *
ActionTypeText(ಗಣನಾ xkb_action_type type);

ಧ್ರುವ ವರ್ಣ *
KeysymText(ಸಂರಚನಾ xkb_context *ctx, xkb_keysym_t sym);

ಧ್ರುವ ವರ್ಣ *
ಕೀಲಿ_Name_Text(ಸಂರಚನಾ xkb_context *ctx, xkb_atom_t name);

ಧ್ರುವ ವರ್ಣ *
SIMatchText(ಗಣನಾ xkb_match_operation type);

ಧ್ರುವ ವರ್ಣ *
LedStateMaskText(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ LookupEntry *lookup,
                 ಗಣನಾ xkb_state_component mask);

ಧ್ರುವ ವರ್ಣ *
ControlMaskText(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_keymap_format format,
                ಗಣನಾ xkb_action_controls mask);
