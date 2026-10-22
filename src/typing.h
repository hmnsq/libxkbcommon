/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdint.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "darray.h"

গণনা xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

গণনা xkb_চাবি_kind {
    XKB_চাবি_PRINTABLE,
    XKB_চাবি_MODIFIER,
    XKB_চাবি_MISC,
    অঙ্ক_XKB_চাবি_KIND,
};

কাঠামো xkb_modifier_চাবি {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    গণনা xkb_modifier_kind kind;
};

প্রকারলেখ darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
কাঠামো xkb_চাবি_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(কাঠামো xkb_modifier_চাবি) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

কাঠামো xkb_চাবি_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

গণনা xkb_status
xkb_চাবি_set_init(কাঠামো xkb_চাবি_set *set,
                 কাঠামো xkb_keymap *keymap,
                 ধ্রুবক কাঠামো xkb_চাবি_set_config *config);

শূন্য
xkb_চাবি_set_destroy(কাঠামো xkb_চাবি_set *set);

কাঠামো xkb_typing_event {
    xkb_keycode_t keycode;
    গণনা xkb_চাবি_direction direction;
};

প্রকারলেখ darray(কাঠামো xkb_typing_event) xkb_typing_events;

কাঠামো xkb_pending_typing_event {
    কাঠামো xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t গণক;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
প্রকারলেখ long (*xkb_prng_t)(শূন্য *);

কাঠামো xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ধ্রুবক কাঠামো xkb_চাবি_set *চাবিগুলি;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *চাবিগুলি_state;
    xkb_prng_t prng;
    /** Borrowed */
    শূন্য *prng_state;
    আকার_t অঙ্ক_events;
    /** Realistic typing events */
    কাঠামো xkb_typing_event *events ATTR_COUNTED_BY(অঙ্ক_events);
    /** Pending events */
    darray(কাঠামো xkb_pending_typing_event) pending;
    darray_আকার_t অঙ্ক_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

গণনা {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

কাঠামো xkb_typing_প্রবেশ {
    কাঠামো xkb_typing_event *events;
    আকার_t অঙ্ক_events;
};

কাঠামো xkb_typing_প্রবেশ_config {
    xkb_prng_t prng;
    শূন্য *prng_state;
    আকার_t প্রবেশ_দৈর্ঘ্য;
};

গণনা xkb_status
xkb_typing_প্রবেশ_init(কাঠামো xkb_typing_প্রবেশ *typing,
                      ধ্রুবক কাঠামো xkb_চাবি_set *চাবিগুলি,
                      ধ্রুবক কাঠামো xkb_typing_প্রবেশ_config *config);

শূন্য
xkb_typing_প্রবেশ_destroy(কাঠামো xkb_typing_প্রবেশ *typing);
