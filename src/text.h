/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "keymap.h"

/** LUT entry */
પ્રકારવ્યાખ્યા સંરચના {
    અચલ અક્ષર *name;
    uint32_t કિંમત;
} LookupEntry;

bool
LookupString(અચલ LookupEntry tab[], અચલ અક્ષર *string,
              unsigned પૂર્ણાંક *કિંમત_rtrn);

અચલ અક્ષર *
Lookup_કિંમત(અચલ LookupEntry tab[], unsigned પૂર્ણાંક કિંમત);

#define GROUP_LAST_INDEX_NAME "last"
ગણના {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

સ્થિર અંતઃરેખ uint8_t
format_control_names_offset(ગણના xkb_keymap_format format)
{
    પાછા (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

બાહ્ય અચલ LookupEntry ctrlMaskNames[];
બાહ્ય અચલ LookupEntry modComponentMaskNames[];
બાહ્ય અચલ LookupEntry groupComponentMaskNames[];
બાહ્ય અચલ LookupEntry groupMaskNames[];
બાહ્ય અચલ LookupEntry buttonNames[];
બાહ્ય અચલ LookupEntry use_Mod_Map_કિંમત_Names[];
બાહ્ય અચલ LookupEntry actionTypeNames[];
બાહ્ય અચલ LookupEntry symInterpretMatchMaskNames[];

અચલ અક્ષર *
ModMaskText(સંરચના xkb_context *ctx, ગણના mod_type type,
            અચલ સંરચના xkb_mod_set *mods, xkb_mod_mask_t mask);

અચલ અક્ષર *
ModIndexText(સંરચના xkb_context *ctx, અચલ સંરચના xkb_mod_set *mods,
             xkb_mod_index_t ndx);

અચલ અક્ષર *
ActionTypeText(ગણના xkb_action_type type);

અચલ અક્ષર *
KeysymText(સંરચના xkb_context *ctx, xkb_keysym_t sym);

અચલ અક્ષર *
ચાવી_Name_Text(સંરચના xkb_context *ctx, xkb_atom_t name);

અચલ અક્ષર *
SIMatchText(ગણના xkb_match_operation type);

અચલ અક્ષર *
LedStateMaskText(સંરચના xkb_context *ctx, અચલ LookupEntry *lookup,
                 ગણના xkb_state_component mask);

અચલ અક્ષર *
ControlMaskText(સંરચના xkb_context *ctx, ગણના xkb_keymap_format format,
                ગણના xkb_action_controls mask);
