/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <xcb/xkb.h>

#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-x11.h"

கட்டமைப்பு x11_atom_interner {
    கட்டமைப்பு xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    கட்டமைப்பு {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t இலக்கம்_pending;
    /* Atoms which were already pending but queried again */
    கட்டமைப்பு {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t இலக்கம்_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    கட்டமைப்பு {
        xcb_get_atom_name_cookie_t cookie;
        எழுத்து **out;
    } escaped[4];
    size_t இலக்கம்_escaped;
};

வெறுமை
x11_atom_interner_init(கட்டமைப்பு x11_atom_interner *interner,
                       கட்டமைப்பு xkb_context *ctx, xcb_connection_t *conn);

வெறுமை
x11_atom_interner_round_trip(கட்டமைப்பு x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
வெறுமை
x11_atom_interner_adopt_atom(கட்டமைப்பு x11_atom_interner *interner,
                             மாறிலி xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
வெறுமை
x11_atom_interner_get_escaped_atom_name(கட்டமைப்பு x11_atom_interner *interner,
                                        xcb_atom_t atom, எழுத்து **out);

எண்ணிக்கை xkb_action_controls
translate_controls_mask(uint32_t wire);
