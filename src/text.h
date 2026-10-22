/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "keymap.h"

/** LUT entry */
प्रकारव्याख्या संरचना {
    स्थिरांक अक्षर *name;
    uint32_t मूल्य;
} LookupEntry;

bool
LookupString(स्थिरांक LookupEntry tab[], स्थिरांक अक्षर *string,
              अचिन्हित पूर्णांक *मूल्य_rtrn);

स्थिरांक अक्षर *
Lookup_मूल्य(स्थिरांक LookupEntry tab[], अचिन्हित पूर्णांक मूल्य);

#define GROUP_LAST_INDEX_NAME "last"
गणना {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

स्थिर ओळीत uint8_t
format_control_names_offset(गणना xkb_keymap_format format)
{
    परत (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

बाह्य स्थिरांक LookupEntry ctrlMaskNames[];
बाह्य स्थिरांक LookupEntry modComponentMaskNames[];
बाह्य स्थिरांक LookupEntry groupComponentMaskNames[];
बाह्य स्थिरांक LookupEntry groupMaskNames[];
बाह्य स्थिरांक LookupEntry buttonNames[];
बाह्य स्थिरांक LookupEntry use_Mod_Map_मूल्य_Names[];
बाह्य स्थिरांक LookupEntry actionTypeNames[];
बाह्य स्थिरांक LookupEntry symInterpretMatchMaskNames[];

स्थिरांक अक्षर *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            स्थिरांक संरचना xkb_mod_set *mods, xkb_mod_mask_t mask);

स्थिरांक अक्षर *
ModIndexText(संरचना xkb_context *ctx, स्थिरांक संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx);

स्थिरांक अक्षर *
ActionTypeText(गणना xkb_action_type type);

स्थिरांक अक्षर *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym);

स्थिरांक अक्षर *
किल्ली_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name);

स्थिरांक अक्षर *
SIMatchText(गणना xkb_match_operation type);

स्थिरांक अक्षर *
LedStateMaskText(संरचना xkb_context *ctx, स्थिरांक LookupEntry *lookup,
                 गणना xkb_state_component mask);

स्थिरांक अक्षर *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask);
