/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <xcb/xkb.h>

#అన్తర్భావ "keymap.h"
#అన్తర్భావ "xkbcommon/xkbcommon-x11.h"

నిర్మాణం x11_atom_interner {
    నిర్మాణం xkb_context *ctx;
    xcb_connection_t *conn;
    bool had_error;
    /* Atoms for which we send a GetAtomName request */
    నిర్మాణం {
        xcb_atom_t from;
        xkb_atom_t *బహిర్గతం;
        xcb_get_atom_name_cookie_t cookie;
    } pending[128];
    కొలత_t అంకె_pending;
    /* Atoms which were already pending but queried again */
    నిర్మాణం {
        xcb_atom_t from;
        xkb_atom_t *బహిర్గతం;
    } copies[128];
    కొలత_t అంకె_copies;
    /* These are not interned, but saved directly (after XkbEscapeMapName) */
    నిర్మాణం {
        xcb_get_atom_name_cookie_t cookie;
        అక్షరం **బహిర్గతం;
    } escaped[4];
    కొలత_t అంకె_escaped;
};

శూన్య
x11_atom_interner_init(నిర్మాణం x11_atom_interner *interner,
                       నిర్మాణం xkb_context *ctx, xcb_connection_t *conn);

శూన్య
x11_atom_interner_విడత_trip(నిర్మాణం x11_atom_interner *interner);

/*
 * Make an xkb_atom_t's from X atoms. The actual write is delayed until the next
 * call to x11_atom_interner_round_trip() or when too many atoms are pending.
 */
శూన్య
x11_atom_interner_adopt_atom(నిర్మాణం x11_atom_interner *interner,
                             స్థిరరాశి xcb_atom_t atom, xkb_atom_t *బహిర్గతం);

/*
 * Get a strdup'd and XkbEscapeMapName'd name of an X atom. The actual write is
 * delayed until the next call to x11_atom_interner_round_trip().
 */
శూన్య
x11_atom_interner_get_escaped_atom_name(నిర్మాణం x11_atom_interner *interner,
                                        xcb_atom_t atom, అక్షరం **బహిర్గతం);

గణన xkb_action_controls
translate_controls_ముసుగు(uint32_t wire);
