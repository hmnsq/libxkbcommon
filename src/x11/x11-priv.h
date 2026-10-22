/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <xcb/xkb.h>

#شامل "keymap.h"
#شامل "xkbcommon/xkbcommon-x11.h"

ساخت x11_atom_interner {
    ساخت xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ساخت {
        xcb_atom_t from;
        xkb_atom_t *برآمد;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    سائز_t عدد_pending;
    /* Atoms which were already pending but queried again */
    ساخت {
        xcb_atom_t from;
        xkb_atom_t *برآمد;
    } copies[128];
    سائز_t عدد_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ساخت {
        xcb_get_atom_name_cookie_t cookie;
        حرف **برآمد;
    } escaped[4];
    سائز_t عدد_escaped;
};

خالی
x11_atom_interner_init(ساخت x11_atom_interner *interner,
                       ساخت xkb_context *ctx, xcb_connection_t *conn);

خالی
x11_atom_interner_باری_trip(ساخت x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
خالی
x11_atom_interner_adopt_atom(ساخت x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *برآمد);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
خالی
x11_atom_interner_get_escaped_atom_name(ساخت x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **برآمد);

شمار xkb_action_controls
translate_controls_نقاب(uint32_t wire);
