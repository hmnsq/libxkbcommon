/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <xcb/xkb.h>

#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-x11.h"

ಸಂರಚನಾ x11_atom_interner {
    ಸಂರಚನಾ xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ಸಂರಚನಾ {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t ಅಂಕೆ_pending;
    /* Atoms which were already pending but queried again */
    ಸಂರಚನಾ {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t ಅಂಕೆ_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ಸಂರಚನಾ {
        xcb_get_atom_name_cookie_t cookie;
        ವರ್ಣ **out;
    } escaped[4];
    size_t ಅಂಕೆ_escaped;
};

ಶೂನ್ಯ
x11_atom_interner_init(ಸಂರಚನಾ x11_atom_interner *interner,
                       ಸಂರಚನಾ xkb_context *ctx, xcb_connection_t *conn);

ಶೂನ್ಯ
x11_atom_interner_round_trip(ಸಂರಚನಾ x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
ಶೂನ್ಯ
x11_atom_interner_adopt_atom(ಸಂರಚನಾ x11_atom_interner *interner,
                             ಧ್ರುವ xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
ಶೂನ್ಯ
x11_atom_interner_get_escaped_atom_name(ಸಂರಚನಾ x11_atom_interner *interner,
                                        xcb_atom_t atom, ವರ್ಣ **out);

ಗಣನಾ xkb_action_controls
translate_controls_mask(uint32_t wire);
