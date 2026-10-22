/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <xcb/xkb.h>

#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-x11.h"

ರಚನೆ x11_atom_interner {
    ರಚನೆ xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ರಚನೆ {
        xcb_atom_t from;
        xkb_atom_t *ಹೊರಹೋಗು;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    ಗಾತ್ರ_t ಅಂಕೆ_pending;
    /* Atoms which were already pending but queried again */
    ರಚನೆ {
        xcb_atom_t from;
        xkb_atom_t *ಹೊರಹೋಗು;
    } copies[128];
    ಗಾತ್ರ_t ಅಂಕೆ_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ರಚನೆ {
        xcb_get_atom_name_cookie_t cookie;
        ಅಕ್ಷರ **ಹೊರಹೋಗು;
    } escaped[4];
    ಗಾತ್ರ_t ಅಂಕೆ_escaped;
};

ಶೂನ್ಯ
x11_atom_interner_init(ರಚನೆ x11_atom_interner *interner,
                       ರಚನೆ xkb_context *ctx, xcb_connection_t *conn);

ಶೂನ್ಯ
x11_atom_interner_ಸುತ್ತು_trip(ರಚನೆ x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
ಶೂನ್ಯ
x11_atom_interner_adopt_atom(ರಚನೆ x11_atom_interner *interner,
                             ಸ್ಥಿರಾಂಕ xcb_atom_t atom, xkb_atom_t *ಹೊರಹೋಗು);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
ಶೂನ್ಯ
x11_atom_interner_get_escaped_atom_name(ರಚನೆ x11_atom_interner *interner,
                                        xcb_atom_t atom, ಅಕ್ಷರ **ಹೊರಹೋಗು);

ಎಣಿಕೆ xkb_action_controls
translate_controls_ಮುಖವಾಡ(uint32_t wire);
