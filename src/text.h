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
প্রকারলেখ কাঠামো {
    ধ্রুবক অক্ষর *name;
    uint32_t মান;
} LookupEntry;

bool
LookupString(ধ্রুবক LookupEntry tab[], ধ্রুবক অক্ষর *string,
              unsigned পূর্ণসংখ্যা *মান_rtrn);

ধ্রুবক অক্ষর *
Lookup_মান(ধ্রুবক LookupEntry tab[], unsigned পূর্ণসংখ্যা মান);

#define GROUP_LAST_সূচক_NAME "last"
গণনা {
    CONTROL_NAMES_MIN_V1_সূচক = 7,
    CONTROL_NAMES_MIN_V2_সূচক = 0,
};

স্থির অন্তঃরেখ uint8_t
format_control_names_offset(গণনা xkb_keymap_format format)
{
    ফেরত (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_সূচক
        : CONTROL_NAMES_MIN_V2_সূচক;
}

বাহ্যিক ধ্রুবক LookupEntry ctrlMaskNames[];
বাহ্যিক ধ্রুবক LookupEntry modComponentMaskNames[];
বাহ্যিক ধ্রুবক LookupEntry groupComponentMaskNames[];
বাহ্যিক ধ্রুবক LookupEntry groupMaskNames[];
বাহ্যিক ধ্রুবক LookupEntry buttonNames[];
বাহ্যিক ধ্রুবক LookupEntry use_Mod_Map_মান_Names[];
বাহ্যিক ধ্রুবক LookupEntry actionTypeNames[];
বাহ্যিক ধ্রুবক LookupEntry symInterpretMatchMaskNames[];

ধ্রুবক অক্ষর *
ModMaskText(কাঠামো xkb_context *ctx, গণনা mod_type type,
            ধ্রুবক কাঠামো xkb_mod_set *mods, xkb_mod_mask_t mask);

ধ্রুবক অক্ষর *
Mod_সূচক_Text(কাঠামো xkb_context *ctx, ধ্রুবক কাঠামো xkb_mod_set *mods,
             xkb_mod_সূচক_t ndx);

ধ্রুবক অক্ষর *
ActionTypeText(গণনা xkb_action_type type);

ধ্রুবক অক্ষর *
KeysymText(কাঠামো xkb_context *ctx, xkb_keysym_t sym);

ধ্রুবক অক্ষর *
চাবি_Name_Text(কাঠামো xkb_context *ctx, xkb_atom_t name);

ধ্রুবক অক্ষর *
SIMatchText(গণনা xkb_match_operation type);

ধ্রুবক অক্ষর *
LedStateMaskText(কাঠামো xkb_context *ctx, ধ্রুবক LookupEntry *lookup,
                 গণনা xkb_state_component mask);

ধ্রুবক অক্ষর *
ControlMaskText(কাঠামো xkb_context *ctx, গণনা xkb_keymap_format format,
                গণনা xkb_action_controls mask);
