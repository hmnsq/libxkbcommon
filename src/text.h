/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"

/** LUT entry */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅଚଳ ଅକ୍ଷର *name;
    uint32_t ମୂଲ୍ୟ;
} LookupEntry;

bool
LookupString(ଅଚଳ LookupEntry tab[], ଅଚଳ ଅକ୍ଷର *string,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ମୂଲ୍ୟ_rtrn);

ଅଚଳ ଅକ୍ଷର *
Lookup_ମୂଲ୍ୟ(ଅଚଳ LookupEntry tab[], unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ);

#define GROUP_LAST_INDEX_NAME "last"
ଗଣନା {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint8_t
format_control_names_offset(ଗଣନା xkb_keymap_format format)
{
    ଫେରନ୍ତୁ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

ବାହ୍ୟ ଅଚଳ LookupEntry ctrlMaskNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry modComponentMaskNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry groupComponentMaskNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry groupMaskNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry buttonNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry use_Mod_Map_ମୂଲ୍ୟ_Names[];
ବାହ୍ୟ ଅଚଳ LookupEntry actionTypeNames[];
ବାହ୍ୟ ଅଚଳ LookupEntry symInterpretMatchMaskNames[];

ଅଚଳ ଅକ୍ଷର *
ModMaskText(ସଂରଚନା xkb_context *ctx, ଗଣନା mod_type type,
            ଅଚଳ ସଂରଚନା xkb_mod_set *mods, xkb_mod_mask_t mask);

ଅଚଳ ଅକ୍ଷର *
ModIndexText(ସଂରଚନା xkb_context *ctx, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ଅଚଳ ଅକ୍ଷର *
ActionTypeText(ଗଣନା xkb_action_type type);

ଅଚଳ ଅକ୍ଷର *
KeysymText(ସଂରଚନା xkb_context *ctx, xkb_keysym_t sym);

ଅଚଳ ଅକ୍ଷର *
ଚାବି_Name_Text(ସଂରଚନା xkb_context *ctx, xkb_atom_t name);

ଅଚଳ ଅକ୍ଷର *
SIMatchText(ଗଣନା xkb_match_operation type);

ଅଚଳ ଅକ୍ଷର *
LedStateMaskText(ସଂରଚନା xkb_context *ctx, ଅଚଳ LookupEntry *lookup,
                 ଗଣନା xkb_state_component mask);

ଅଚଳ ଅକ୍ଷର *
ControlMaskText(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_keymap_format format,
                ଗଣନା xkb_action_controls mask);
