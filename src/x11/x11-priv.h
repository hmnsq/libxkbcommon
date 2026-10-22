/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <xcb/xkb.h>

#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-x11.h"

સંરચના x11_atom_interner {
    સંરચના xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    સંરચના {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t અંક_pending;
    /* Atoms which were already pending but queried again */
    સંરચના {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t અંક_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    સંરચના {
        xcb_get_atom_name_cookie_t cookie;
        અક્ષર **out;
    } escaped[4];
    size_t અંક_escaped;
};

શૂન્ય
x11_atom_interner_init(સંરચના x11_atom_interner *interner,
                       સંરચના xkb_context *ctx, xcb_connection_t *conn);

શૂન્ય
x11_atom_interner_round_trip(સંરચના x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
શૂન્ય
x11_atom_interner_adopt_atom(સંરચના x11_atom_interner *interner,
                             અચલ xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
શૂન્ય
x11_atom_interner_get_escaped_atom_name(સંરચના x11_atom_interner *interner,
                                        xcb_atom_t atom, અક્ષર **out);

ગણના xkb_action_controls
translate_controls_mask(uint32_t wire);
