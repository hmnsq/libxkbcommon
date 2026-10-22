/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <xcb/xkb.h>

#ضمن "keymap.h"
#ضمن "xkbcommon/xkbcommon-x11.h"

بنية x11_atom_interner {
    بنية xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    بنية {
        xcb_atom_t from;
        xkb_atom_t *باہرلا;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    ناپ_t عدد_pending;
    /* Atoms which were already pending but queried again */
    بنية {
        xcb_atom_t from;
        xkb_atom_t *باہرلا;
    } copies[128];
    ناپ_t عدد_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    بنية {
        xcb_get_atom_name_cookie_t cookie;
        حرف **باہرلا;
    } escaped[4];
    ناپ_t عدد_escaped;
};

فراغ
x11_atom_interner_init(بنية x11_atom_interner *interner,
                       بنية xkb_context *ctx, xcb_connection_t *conn);

فراغ
x11_atom_interner_گیڑا_trip(بنية x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
فراغ
x11_atom_interner_adopt_atom(بنية x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *باہرلا);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **باہرلا);

تعداد xkb_action_controls
translate_controls_نقاب(uint32_t wire);
