/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdbool.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "keymap.h"

/** LUT entry */
প্রকারনির্দেশ সংরচনা {
    ধ্রুৱ ৱর্ণ *name;
    uint32_t মান;
} LookupEntry;

bool
LookupString(ধ্রুৱ LookupEntry tab[], ধ্রুৱ ৱর্ণ *string,
              unsigned পূর্ণাঙ্ক *মান_rtrn);

ধ্রুৱ ৱর্ণ *
Lookup_মান(ধ্রুৱ LookupEntry tab[], unsigned পূর্ণাঙ্ক মান);

#define GROUP_LAST_INDEX_NAME "last"
গণনা {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

স্থির অন্তঃস্থ uint8_t
format_control_names_offset(গণনা xkb_keymap_format format)
{
    ঘূৰাই (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

বাহ্য ধ্রুৱ LookupEntry ctrlMaskNames[];
বাহ্য ধ্রুৱ LookupEntry modComponentMaskNames[];
বাহ্য ধ্রুৱ LookupEntry groupComponentMaskNames[];
বাহ্য ধ্রুৱ LookupEntry groupMaskNames[];
বাহ্য ধ্রুৱ LookupEntry buttonNames[];
বাহ্য ধ্রুৱ LookupEntry use_Mod_Map_মান_Names[];
বাহ্য ধ্রুৱ LookupEntry actionTypeNames[];
বাহ্য ধ্রুৱ LookupEntry symInterpretMatchMaskNames[];

ধ্রুৱ ৱর্ণ *
ModMaskText(সংরচনা xkb_context *ctx, গণনা mod_type type,
            ধ্রুৱ সংরচনা xkb_mod_set *mods, xkb_mod_mask_t mask);

ধ্রুৱ ৱর্ণ *
ModIndexText(সংরচনা xkb_context *ctx, ধ্রুৱ সংরচনা xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ধ্রুৱ ৱর্ণ *
ActionTypeText(গণনা xkb_action_type type);

ধ্রুৱ ৱর্ণ *
KeysymText(সংরচনা xkb_context *ctx, xkb_keysym_t sym);

ধ্রুৱ ৱর্ণ *
চাবি_Name_Text(সংরচনা xkb_context *ctx, xkb_atom_t name);

ধ্রুৱ ৱর্ণ *
SIMatchText(গণনা xkb_match_operation type);

ধ্রুৱ ৱর্ণ *
LedStateMaskText(সংরচনা xkb_context *ctx, ধ্রুৱ LookupEntry *lookup,
                 গণনা xkb_state_component mask);

ধ্রুৱ ৱর্ণ *
ControlMaskText(সংরচনা xkb_context *ctx, গণনা xkb_keymap_format format,
                গণনা xkb_action_controls mask);
