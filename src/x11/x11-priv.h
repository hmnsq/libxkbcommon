/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <xcb/xkb.h>

#अन्तर्भाव "keymap.h"
#अन्तर्भाव "xkbcommon/xkbcommon-x11.h"

संरचना x11_atom_interner {
    संरचना xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    संरचना {
        xcb_atom_t from;
        xkb_atom_t *बहिर्गमनम्;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    आकार_t अङ्कः_pending;
    /* Atoms which were already pending but queried again */
    संरचना {
        xcb_atom_t from;
        xkb_atom_t *बहिर्गमनम्;
    } copies[128];
    आकार_t अङ्कः_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    संरचना {
        xcb_get_atom_name_cookie_t cookie;
        वर्ण **बहिर्गमनम्;
    } escaped[4];
    आकार_t अङ्कः_escaped;
};

शून्य
x11_atom_interner_init(संरचना x11_atom_interner *interner,
                       संरचना xkb_context *ctx, xcb_connection_t *conn);

शून्य
x11_atom_interner_आवर्तनम्_trip(संरचना x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
शून्य
x11_atom_interner_adopt_atom(संरचना x11_atom_interner *interner,
                             ध्रुव xcb_atom_t atom, xkb_atom_t *बहिर्गमनम्);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
शून्य
x11_atom_interner_get_escaped_atom_name(संरचना x11_atom_interner *interner,
                                        xcb_atom_t atom, वर्ण **बहिर्गमनम्);

गणना xkb_action_controls
translate_controls_आवरणम्(uint32_t wire);
