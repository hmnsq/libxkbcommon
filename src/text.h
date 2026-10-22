/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "keymap.h"

/** LUT entry */
প্রকারনির্দেশ সংরচনা {
    ধ্রুব বর্ণ *name;
    uint32_t মান;
} LookupEntry;

bool
LookupString(ধ্রুব LookupEntry tab[], ধ্রুব বর্ণ *string,
              unsigned পূর্ণাঙ্ক *মান_rtrn);

ধ্রুব বর্ণ *
Lookup_মান(ধ্রুব LookupEntry tab[], unsigned পূর্ণাঙ্ক মান);

#define GROUP_LAST_INDEX_NAME "last"
গণনা {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

স্থির অন্তঃস্থ uint8_t
format_control_names_offset(গণনা xkb_keymap_format format)
{
    প্রত্যাগম (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

বাহ্য ধ্রুব LookupEntry ctrlMaskNames[];
বাহ্য ধ্রুব LookupEntry modComponentMaskNames[];
বাহ্য ধ্রুব LookupEntry groupComponentMaskNames[];
বাহ্য ধ্রুব LookupEntry groupMaskNames[];
বাহ্য ধ্রুব LookupEntry buttonNames[];
বাহ্য ধ্রুব LookupEntry use_Mod_Map_মান_Names[];
বাহ্য ধ্রুব LookupEntry actionTypeNames[];
বাহ্য ধ্রুব LookupEntry symInterpretMatchMaskNames[];

ধ্রুব বর্ণ *
ModMaskText(সংরচনা xkb_context *ctx, গণনা mod_type type,
            ধ্রুব সংরচনা xkb_mod_set *mods, xkb_mod_mask_t mask);

ধ্রুব বর্ণ *
ModIndexText(সংরচনা xkb_context *ctx, ধ্রুব সংরচনা xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ধ্রুব বর্ণ *
ActionTypeText(গণনা xkb_action_type type);

ধ্রুব বর্ণ *
KeysymText(সংরচনা xkb_context *ctx, xkb_keysym_t sym);

ধ্রুব বর্ণ *
চাবি_Name_Text(সংরচনা xkb_context *ctx, xkb_atom_t name);

ধ্রুব বর্ণ *
SIMatchText(গণনা xkb_match_operation type);

ধ্রুব বর্ণ *
LedStateMaskText(সংরচনা xkb_context *ctx, ধ্রুব LookupEntry *lookup,
                 গণনা xkb_state_component mask);

ধ্রুব বর্ণ *
ControlMaskText(সংরচনা xkb_context *ctx, গণনা xkb_keymap_format format,
                গণনা xkb_action_controls mask);
