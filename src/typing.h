/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "darray.h"

गणना xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

गणना xkb_कुंजी_kind {
    XKB_कुंजी_PRINTABLE,
    XKB_कुंजी_MODIFIER,
    XKB_कुंजी_MISC,
    अंक_XKB_कुंजी_KIND,
};

संरचना xkb_modifier_कुंजी {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    गणना xkb_modifier_kind kind;
};

प्रकारनिर्देश darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
संरचना xkb_कुंजी_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(संरचना xkb_modifier_कुंजी) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

संरचना xkb_कुंजी_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

गणना xkb_status
xkb_कुंजी_set_init(संरचना xkb_कुंजी_set *set,
                 संरचना xkb_keymap *keymap,
                 ध्रुव संरचना xkb_कुंजी_set_config *config);

शून्य
xkb_कुंजी_set_destroy(संरचना xkb_कुंजी_set *set);

संरचना xkb_typing_event {
    xkb_keycode_t keycode;
    गणना xkb_कुंजी_direction direction;
};

प्रकारनिर्देश darray(संरचना xkb_typing_event) xkb_typing_events;

संरचना xkb_pending_typing_event {
    संरचना xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
प्रकारनिर्देश दीर्घ (*xkb_prng_t)(शून्य *);

संरचना xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ध्रुव संरचना xkb_कुंजी_set *कुंजियाँ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *कुंजियाँ_state;
    xkb_prng_t prng;
    /** Borrowed */
    शून्य *prng_state;
    माप_प्रकार अंक_events;
    /** Realistic typing events */
    संरचना xkb_typing_event *events ATTR_COUNTED_BY(अंक_events);
    /** Pending events */
    darray(संरचना xkb_pending_typing_event) pending;
    darray_size_t अंक_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

गणना {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

संरचना xkb_typing_निवेश {
    संरचना xkb_typing_event *events;
    माप_प्रकार अंक_events;
};

संरचना xkb_typing_निवेश_config {
    xkb_prng_t prng;
    शून्य *prng_state;
    माप_प्रकार निवेश_length;
};

गणना xkb_status
xkb_typing_निवेश_init(संरचना xkb_typing_निवेश *typing,
                      ध्रुव संरचना xkb_कुंजी_set *कुंजियाँ,
                      ध्रुव संरचना xkb_typing_निवेश_config *config);

शून्य
xkb_typing_निवेश_destroy(संरचना xkb_typing_निवेश *typing);
