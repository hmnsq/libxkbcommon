/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <xcb/xkb.h>

#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-x11.h"

ସଂରଚନା x11_atom_interner {
    ସଂରଚନା xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ସଂରଚନା {
        xcb_atom_t from;
        xkb_atom_t *out;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    size_t ଅଙ୍କ_pending;
    /* Atoms which were already pending but queried again */
    ସଂରଚନା {
        xcb_atom_t from;
        xkb_atom_t *out;
    } copies[128];
    size_t ଅଙ୍କ_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ସଂରଚନା {
        xcb_get_atom_name_cookie_t cookie;
        ଅକ୍ଷର **out;
    } escaped[4];
    size_t ଅଙ୍କ_escaped;
};

ଶୂନ୍ୟ
x11_atom_interner_init(ସଂରଚନା x11_atom_interner *interner,
                       ସଂରଚନା xkb_context *ctx, xcb_connection_t *conn);

ଶୂନ୍ୟ
x11_atom_interner_round_trip(ସଂରଚନା x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
ଶୂନ୍ୟ
x11_atom_interner_adopt_atom(ସଂରଚନା x11_atom_interner *interner,
                             ଅଚଳ xcb_atom_t atom, xkb_atom_t *out);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
ଶୂନ୍ୟ
x11_atom_interner_get_escaped_atom_name(ସଂରଚନା x11_atom_interner *interner,
                                        xcb_atom_t atom, ଅକ୍ଷର **out);

ଗଣନା xkb_action_controls
translate_controls_mask(uint32_t wire);
