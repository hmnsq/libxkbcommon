/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "atom.h"
#समावेश "keymap.h"

/** LUT entry */
प्रकार काष्ठा {
    स्थिर अक्षर *name;
    uint32_t मूल्य;
} LookupEntry;

bool
LookupString(स्थिर LookupEntry tab[], स्थिर अक्षर *string,
              अचिन्हित पूर्णांक *मूल्य_rtrn);

स्थिर अक्षर *
Lookup_मूल्य(स्थिर LookupEntry tab[], अचिन्हित पूर्णांक मूल्य);

#घोषणा GROUP_LAST_सूचकांक_NAME "last"
क्रमागत {
    CONTROL_NAMES_MIN_V1_सूचकांक = 7,
    CONTROL_NAMES_MIN_V2_सूचकांक = 0,
};

अटल अंतरभूत uint8_t
format_control_names_offset(क्रमागत xkb_keymap_format format)
{
    वापस (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_सूचकांक
        : CONTROL_NAMES_MIN_V2_सूचकांक;
}

बाह्य स्थिर LookupEntry ctrlMaskNames[];
बाह्य स्थिर LookupEntry modComponentMaskNames[];
बाह्य स्थिर LookupEntry groupComponentMaskNames[];
बाह्य स्थिर LookupEntry groupMaskNames[];
बाह्य स्थिर LookupEntry buttonNames[];
बाह्य स्थिर LookupEntry use_Mod_Map_मूल्य_Names[];
बाह्य स्थिर LookupEntry actionTypeNames[];
बाह्य स्थिर LookupEntry symInterpretMatchMaskNames[];

स्थिर अक्षर *
ModMaskText(काष्ठा xkb_context *ctx, क्रमागत mod_type type,
            स्थिर काष्ठा xkb_mod_set *mods, xkb_mod_mask_t mask);

स्थिर अक्षर *
Mod_सूचकांक_Text(काष्ठा xkb_context *ctx, स्थिर काष्ठा xkb_mod_set *mods,
             xkb_mod_सूचकांक_t ndx);

स्थिर अक्षर *
ActionTypeText(क्रमागत xkb_action_type type);

स्थिर अक्षर *
KeysymText(काष्ठा xkb_context *ctx, xkb_keysym_t sym);

स्थिर अक्षर *
कुंजी_Name_Text(काष्ठा xkb_context *ctx, xkb_atom_t name);

स्थिर अक्षर *
SIMatchText(क्रमागत xkb_match_operation type);

स्थिर अक्षर *
LedStateMaskText(काष्ठा xkb_context *ctx, स्थिर LookupEntry *lookup,
                 क्रमागत xkb_state_component mask);

स्थिर अक्षर *
ControlMaskText(काष्ठा xkb_context *ctx, क्रमागत xkb_keymap_format format,
                क्रमागत xkb_action_controls mask);
