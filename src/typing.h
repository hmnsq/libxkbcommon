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

गणना xkb_साँचो_kind {
    XKB_साँचो_PRINTABLE,
    XKB_साँचो_MODIFIER,
    XKB_साँचो_MISC,
    अङ्क_XKB_साँचो_KIND,
};

संरचना xkb_modifier_साँचो {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    गणना xkb_modifier_kind kind;
};

प्रकारनिर्देश darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
संरचना xkb_साँचो_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(संरचना xkb_modifier_साँचो) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

संरचना xkb_साँचो_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

गणना xkb_status
xkb_साँचो_set_init(संरचना xkb_साँचो_set *set,
                 संरचना xkb_keymap *keymap,
                 अचर संरचना xkb_साँचो_set_config *config);

शून्य
xkb_साँचो_set_destroy(संरचना xkb_साँचो_set *set);

संरचना xkb_typing_event {
    xkb_keycode_t keycode;
    गणना xkb_साँचो_direction direction;
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
    अचर संरचना xkb_साँचो_set *साँचोहरू;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *साँचोहरू_state;
    xkb_prng_t prng;
    /** Borrowed */
    शून्य *prng_state;
    माप_प्रकार अङ्क_events;
    /** Realistic typing events */
    संरचना xkb_typing_event *events ATTR_COUNTED_BY(अङ्क_events);
    /** Pending events */
    darray(संरचना xkb_pending_typing_event) pending;
    darray_size_t अङ्क_pending;
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

संरचना xkb_typing_आगत {
    संरचना xkb_typing_event *events;
    माप_प्रकार अङ्क_events;
};

संरचना xkb_typing_आगत_config {
    xkb_prng_t prng;
    शून्य *prng_state;
    माप_प्रकार आगत_length;
};

गणना xkb_status
xkb_typing_आगत_init(संरचना xkb_typing_आगत *typing,
                      अचर संरचना xkb_साँचो_set *साँचोहरू,
                      अचर संरचना xkb_typing_आगत_config *config);

शून्य
xkb_typing_आगत_destroy(संरचना xkb_typing_आगत *typing);
