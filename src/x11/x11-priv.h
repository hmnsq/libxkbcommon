/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <xcb/xkb.h>

#شامل "keymap.h"
#شامل "xkbcommon/xkbcommon-x11.h"

ساختار x11_atom_interner {
    ساختار xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ساختار {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t رقم_pending;
    /* Atoms which were already pending but queried again */
    ساختار {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t رقم_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ساختار {
        xcb_get_atom_name_cookie_t cookie;
        نویسه **out;
    } escaped[4];
    size_t رقم_escaped;
};

تهی
x11_atom_interner_init(ساختار x11_atom_interner *interner,
                       ساختار xkb_context *ctx, xcb_connection_t *conn);

تهی
x11_atom_interner_round_trip(ساختار x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
تهی
x11_atom_interner_adopt_atom(ساختار x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
تهی
x11_atom_interner_get_escaped_atom_name(ساختار x11_atom_interner *interner,
                                        xcb_atom_t atom, نویسه **out);

شمارش xkb_action_controls
translate_controls_mask(uint32_t wire);
