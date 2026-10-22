/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <xcb/xkb.h>

#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-x11.h"

কাঠামো x11_atom_interner {
    কাঠামো xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    কাঠামো {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    আকার_t অঙ্ক_pending;
    /* Atoms which were already pending but queried again */
    কাঠামো {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    আকার_t অঙ্ক_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    কাঠামো {
        xcb_get_atom_name_cookie_t cookie;
        অক্ষর **out;
    } escaped[4];
    আকার_t অঙ্ক_escaped;
};

শূন্য
x11_atom_interner_init(কাঠামো x11_atom_interner *interner,
                       কাঠামো xkb_context *ctx, xcb_connection_t *conn);

শূন্য
x11_atom_interner_round_trip(কাঠামো x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
শূন্য
x11_atom_interner_adopt_atom(কাঠামো x11_atom_interner *interner,
                             ধ্রুবক xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
শূন্য
x11_atom_interner_get_escaped_atom_name(কাঠামো x11_atom_interner *interner,
                                        xcb_atom_t atom, অক্ষর **out);

গণনা xkb_action_controls
translate_controls_mask(uint32_t wire);
