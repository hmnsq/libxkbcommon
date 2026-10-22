/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"

/** LUT entry */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    uint32_t ಮೌಲ್ಯ;
} Lookup_ನಮೂದು;

bool
LookupString(ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು tab[], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string,
              ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಮೌಲ್ಯ_rtrn);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Lookup_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು tab[], ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ);

#ವ್ಯಾಖ್ಯಾನಿಸು ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME "last"
ಎಣಿಕೆ {
    CONTROL_ಹೆಸರುಗಳು_MIN_V1_ಸೂಚ್ಯಂಕ = 7,
    CONTROL_ಹೆಸರುಗಳು_MIN_V2_ಸೂಚ್ಯಂಕ = 0,
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint8_t
format_control_ಹೆಸರುಗಳು_ಸ್ಥಾನಾಂತರ(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
        ? CONTROL_ಹೆಸರುಗಳು_MIN_V1_ಸೂಚ್ಯಂಕ
        : CONTROL_ಹೆಸರುಗಳು_MIN_V2_ಸೂಚ್ಯಂಕ;
}

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ctrl_ಮುಖವಾಡ_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು mod_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಗುಂಪು_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು button_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು use_Mod_Map_ಮೌಲ್ಯ_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು action_Type_ಹೆಸರುಗಳು[];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು sym_Interpret_ಹೊಂದಿಕೆ_ಮುಖವಾಡ_ಹೆಸರುಗಳು[];

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ mod_type type,
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Mod_ಸೂಚ್ಯಂಕ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
             xkb_mod_ಸೂಚ್ಯಂಕ_t ndx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Action_Type_ಪಠ್ಯ(ಎಣಿಕೆ xkb_action_type type);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Keysym_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_keysym_t sym);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕೀಲಿ_Name_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_atom_t name);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
SIMatch_ಪಠ್ಯ(ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation type);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Led_State_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *lookup,
                 ಎಣಿಕೆ xkb_state_component ಮುಖವಾಡ);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Control_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_keymap_format format,
                ಎಣಿಕೆ xkb_action_controls ಮುಖವಾಡ);
