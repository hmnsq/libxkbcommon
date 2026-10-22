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
    अचर वर्ण *name;
    uint32_t मान;
} LookupEntry;

bool
LookupString(अचर LookupEntry tab[], अचर वर्ण *string,
              अचिन्हित पूर्णाङ्क *मान_rtrn);

अचर वर्ण *
Lookup_मान(अचर LookupEntry tab[], अचिन्हित पूर्णाङ्क मान);

#define GROUP_LAST_INDEX_NAME "last"
गणना {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

स्थिर अन्तःस्थ uint8_t
format_control_names_offset(गणना xkb_keymap_format format)
{
    फर्काऊ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

बाह्य अचर LookupEntry ctrlMaskNames[];
बाह्य अचर LookupEntry modComponentMaskNames[];
बाह्य अचर LookupEntry groupComponentMaskNames[];
बाह्य अचर LookupEntry groupMaskNames[];
बाह्य अचर LookupEntry buttonNames[];
बाह्य अचर LookupEntry use_Mod_Map_मान_Names[];
बाह्य अचर LookupEntry actionTypeNames[];
बाह्य अचर LookupEntry symInterpretMatchMaskNames[];

अचर वर्ण *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            अचर संरचना xkb_mod_set *mods, xkb_mod_mask_t mask);

अचर वर्ण *
ModIndexText(संरचना xkb_context *ctx, अचर संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx);

अचर वर्ण *
ActionTypeText(गणना xkb_action_type type);

अचर वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym);

अचर वर्ण *
साँचो_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name);

अचर वर्ण *
SIMatchText(गणना xkb_match_operation type);

अचर वर्ण *
LedStateMaskText(संरचना xkb_context *ctx, अचर LookupEntry *lookup,
                 गणना xkb_state_component mask);

अचर वर्ण *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask);
