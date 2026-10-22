/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <xcb/xkb.h>

#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-x11.h"

সংরচনা x11_atom_interner {
    সংরচনা xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    সংরচনা {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t অঙ্ক_pending;
    /* Atoms which were already pending but queried again */
    সংরচনা {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t অঙ্ক_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    সংরচনা {
        xcb_get_atom_name_cookie_t cookie;
        বর্ণ **out;
    } escaped[4];
    size_t অঙ্ক_escaped;
};

শূন্য
x11_atom_interner_init(সংরচনা x11_atom_interner *interner,
                       সংরচনা xkb_context *ctx, xcb_connection_t *conn);

শূন্য
x11_atom_interner_round_trip(সংরচনা x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
শূন্য
x11_atom_interner_adopt_atom(সংরচনা x11_atom_interner *interner,
                             ধ্রুব xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
শূন্য
x11_atom_interner_get_escaped_atom_name(সংরচনা x11_atom_interner *interner,
                                        xcb_atom_t atom, বর্ণ **out);

গণনা xkb_action_controls
translate_controls_mask(uint32_t wire);
