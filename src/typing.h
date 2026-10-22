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

गणना xkb_किल्ली_kind {
    XKB_किल्ली_PRINTABLE,
    XKB_किल्ली_MODIFIER,
    XKB_किल्ली_MISC,
    अंक_XKB_किल्ली_KIND,
};

संरचना xkb_modifier_किल्ली {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    गणना xkb_modifier_kind kind;
};

प्रकारनिर्देश darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
संरचना xkb_किल्ली_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(संरचना xkb_modifier_किल्ली) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

संरचना xkb_किल्ली_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

गणना xkb_status
xkb_किल्ली_set_init(संरचना xkb_किल्ली_set *set,
                 संरचना xkb_keymap *keymap,
                 ध्रुव संरचना xkb_किल्ली_set_config *config);

शून्य
xkb_किल्ली_set_destroy(संरचना xkb_किल्ली_set *set);

संरचना xkb_typing_event {
    xkb_keycode_t keycode;
    गणना xkb_किल्ली_direction direction;
};

प्रकारनिर्देश darray(संरचना xkb_typing_event) xkb_typing_events;

संरचना xkb_pending_typing_event {
    संरचना xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
प्रकारनिर्देश long (*xkb_prng_t)(शून्य *);

संरचना xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ध्रुव संरचना xkb_किल्ली_set *किल्ल्या;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *किल्ल्या_state;
    xkb_prng_t prng;
    /** Borrowed */
    शून्य *prng_state;
    size_t अंक_events;
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

संरचना xkb_typing_आदान {
    संरचना xkb_typing_event *events;
    size_t अंक_events;
};

संरचना xkb_typing_आदान_config {
    xkb_prng_t prng;
    शून्य *prng_state;
    size_t आदान_length;
};

गणना xkb_status
xkb_typing_आदान_init(संरचना xkb_typing_आदान *typing,
                      ध्रुव संरचना xkb_किल्ली_set *किल्ल्या,
                      ध्रुव संरचना xkb_typing_आदान_config *config);

शून्य
xkb_typing_आदान_destroy(संरचना xkb_typing_आदान *typing);
