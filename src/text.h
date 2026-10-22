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
प्रकारनिर्देश संरचना {
    ध्रुव वर्ण *name;
    uint32_t मानम्;
} LookupEntry;

bool
LookupString(ध्रुव LookupEntry tab[], ध्रुव वर्ण *string,
              unsigned पूर्णाङ्क *मानम्_rtrn);

ध्रुव वर्ण *
Lookup_मानम्(ध्रुव LookupEntry tab[], unsigned पूर्णाङ्क मानम्);

#define GROUP_LAST_INDEX_NAME "last"
गणना {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

स्थिर अन्तःस्थ uint8_t
format_control_names_offset(गणना xkb_keymap_format format)
{
    पटिनिवत्त (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

बाह्य ध्रुव LookupEntry ctrlMaskNames[];
बाह्य ध्रुव LookupEntry modComponentMaskNames[];
बाह्य ध्रुव LookupEntry groupComponentMaskNames[];
बाह्य ध्रुव LookupEntry groupMaskNames[];
बाह्य ध्रुव LookupEntry buttonNames[];
बाह्य ध्रुव LookupEntry use_Mod_Map_मानम्_Names[];
बाह्य ध्रुव LookupEntry actionTypeNames[];
बाह्य ध्रुव LookupEntry symInterpretMatchMaskNames[];

ध्रुव वर्ण *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            ध्रुव संरचना xkb_mod_set *mods, xkb_mod_mask_t mask);

ध्रुव वर्ण *
ModIndexText(संरचना xkb_context *ctx, ध्रुव संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx);

ध्रुव वर्ण *
ActionTypeText(गणना xkb_action_type type);

ध्रुव वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym);

ध्रुव वर्ण *
कुञ्चिका_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name);

ध्रुव वर्ण *
SIMatchText(गणना xkb_match_operation type);

ध्रुव वर्ण *
LedStateMaskText(संरचना xkb_context *ctx, ध्रुव LookupEntry *lookup,
                 गणना xkb_state_component mask);

ध्रुव वर्ण *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask);
