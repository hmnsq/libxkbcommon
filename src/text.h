/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "keymap.h"

/** LUT entry */
प्रकारनिर्देश संरचना {
    ध्रुव वर्ण *name;
    uint32_t मानम्;
} Lookup_प्रविष्टिः;

bool
LookupString(ध्रुव Lookup_प्रविष्टिः tab[], ध्रुव वर्ण *string,
              अचिह्नित पूर्णाङ्क *मानम्_rtrn);

ध्रुव वर्ण *
Lookup_मानम्(ध्रुव Lookup_प्रविष्टिः tab[], अचिह्नित पूर्णाङ्क मानम्);

#परिभाषय गणः_चरमः_सूचकांक_NAME "last"
गणना {
    CONTROL_नामानि_MIN_V1_सूचकांक = 7,
    CONTROL_नामानि_MIN_V2_सूचकांक = 0,
};

स्थिर अन्तःस्थ uint8_t
format_control_नामानि_विस्थापनम्(गणना xkb_keymap_format format)
{
    प्रत्यागम (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_नामानि_MIN_V1_सूचकांक
        : CONTROL_नामानि_MIN_V2_सूचकांक;
}

बाह्य ध्रुव Lookup_प्रविष्टिः ctrl_आवरणम्_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः mod_Component_आवरणम्_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः गणः_Component_आवरणम्_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः गणः_आवरणम्_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः button_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः use_Mod_Map_मानम्_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः action_Type_नामानि[];
बाह्य ध्रुव Lookup_प्रविष्टिः sym_Interpret_मेलनम्_आवरणम्_नामानि[];

ध्रुव वर्ण *
Mod_आवरणम्_Text(संरचना xkb_context *ctx, गणना mod_type type,
            ध्रुव संरचना xkb_mod_समुच्चयः *mods, xkb_mod_आवरणम्_t आवरणम्);

ध्रुव वर्ण *
Mod_सूचकांक_Text(संरचना xkb_context *ctx, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
             xkb_mod_सूचकांक_t ndx);

ध्रुव वर्ण *
ActionTypeText(गणना xkb_action_type type);

ध्रुव वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym);

ध्रुव वर्ण *
कुञ्चिका_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name);

ध्रुव वर्ण *
SIMatchText(गणना xkb_मेलनम्_operation type);

ध्रुव वर्ण *
Led_State_आवरणम्_Text(संरचना xkb_context *ctx, ध्रुव Lookup_प्रविष्टिः *lookup,
                 गणना xkb_state_component आवरणम्);

ध्रुव वर्ण *
Control_आवरणम्_Text(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls आवरणम्);
