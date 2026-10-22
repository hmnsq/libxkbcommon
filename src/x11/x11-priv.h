/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <xcb/xkb.h>

#समावेश "keymap.h"
#समावेश "xkbcommon/xkbcommon-x11.h"

काष्ठा x11_atom_interner {
    काष्ठा xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    काष्ठा {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    माप_प्रकार अंक_pending;
    /* Atoms which were already pending but queried again */
    काष्ठा {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    माप_प्रकार अंक_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    काष्ठा {
        xcb_get_atom_name_cookie_t cookie;
        अक्षर **out;
    } escaped[4];
    माप_प्रकार अंक_escaped;
};

व्योम
x11_atom_interner_init(काष्ठा x11_atom_interner *interner,
                       काष्ठा xkb_context *ctx, xcb_connection_t *conn);

व्योम
x11_atom_interner_round_trip(काष्ठा x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
व्योम
x11_atom_interner_adopt_atom(काष्ठा x11_atom_interner *interner,
                             स्थिर xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
व्योम
x11_atom_interner_get_escaped_atom_name(काष्ठा x11_atom_interner *interner,
                                        xcb_atom_t atom, अक्षर **out);

क्रमागत xkb_action_controls
translate_controls_mask(uint32_t wire);
