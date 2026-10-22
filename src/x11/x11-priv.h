/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <xcb/xkb.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-x11.h"

ਬਣਤਰ x11_atom_interner {
    ਬਣਤਰ xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    ਬਣਤਰ {
        xcb_atom_t from;
        xkb_atom_t *ਬਾਹਰਲਾ;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    ਮਾਪ_t ਅੰਕ_pending;
    /* Atoms which were already pending but queried again */
    ਬਣਤਰ {
        xcb_atom_t from;
        xkb_atom_t *ਬਾਹਰਲਾ;
    } copies[128];
    ਮਾਪ_t ਅੰਕ_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    ਬਣਤਰ {
        xcb_get_atom_name_cookie_t cookie;
        ਅੱਖਰ **ਬਾਹਰਲਾ;
    } escaped[4];
    ਮਾਪ_t ਅੰਕ_escaped;
};

ਖਾਲੀ
x11_atom_interner_init(ਬਣਤਰ x11_atom_interner *interner,
                       ਬਣਤਰ xkb_context *ctx, xcb_connection_t *conn);

ਖਾਲੀ
x11_atom_interner_ਗੇੜਾ_trip(ਬਣਤਰ x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
ਖਾਲੀ
x11_atom_interner_adopt_atom(ਬਣਤਰ x11_atom_interner *interner,
                             ਅਚਲ xcb_atom_t atom, xkb_atom_t *ਬਾਹਰਲਾ);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
ਖਾਲੀ
x11_atom_interner_get_escaped_atom_name(ਬਣਤਰ x11_atom_interner *interner,
                                        xcb_atom_t atom, ਅੱਖਰ **ਬਾਹਰਲਾ);

ਗਿਣਤੀ xkb_action_controls
translate_controls_ਨਕਾਬ(uint32_t wire);
