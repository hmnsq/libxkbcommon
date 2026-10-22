/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <xcb/xkb.h>

#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-x11.h"

ഘടന x11_atom_interner {
    ഘടന xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ഘടന {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t അക്കം_pending;
    /* Atoms which were already pending but queried again */
    ഘടന {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t അക്കം_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ഘടന {
        xcb_get_atom_name_cookie_t cookie;
        അക്ഷരം **out;
    } escaped[4];
    size_t അക്കം_escaped;
};

ശൂന്യം
x11_atom_interner_init(ഘടന x11_atom_interner *interner,
                       ഘടന xkb_context *ctx, xcb_connection_t *conn);

ശൂന്യം
x11_atom_interner_round_trip(ഘടന x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
ശൂന്യം
x11_atom_interner_adopt_atom(ഘടന x11_atom_interner *interner,
                             സ്ഥിരം xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
ശൂന്യം
x11_atom_interner_get_escaped_atom_name(ഘടന x11_atom_interner *interner,
                                        xcb_atom_t atom, അക്ഷരം **out);

എണ്ണൽ xkb_action_controls
translate_controls_mask(uint32_t wire);
