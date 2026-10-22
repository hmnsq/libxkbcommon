/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "darray.h"

تعداد xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

تعداد xkb_مبودي_kind {
    XKB_مبودي_PRINTABLE,
    XKB_مبودي_MODIFIER,
    XKB_مبودي_MISC,
    لمب_XKB_مبودي_KIND,
};

بنية xkb_modifier_مبودي {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    تعداد xkb_modifier_kind kind;
};

تعريفنوع darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
بنية xkb_مبودي_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(بنية xkb_modifier_مبودي) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

بنية xkb_مبودي_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

تعداد xkb_status
xkb_مبودي_set_init(بنية xkb_مبودي_set *set,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_مبودي_set_config *config);

فراغ
xkb_مبودي_set_destroy(بنية xkb_مبودي_set *set);

بنية xkb_typing_event {
    xkb_keycode_t keycode;
    تعداد xkb_مبودي_direction direction;
};

تعريفنوع darray(بنية xkb_typing_event) xkb_typing_events;

بنية xkb_pending_typing_event {
    بنية xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
تعريفنوع long (*xkb_prng_t)(فراغ *);

بنية xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ثابت بنية xkb_مبودي_set *مبوداي;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *مبوداي_state;
    xkb_prng_t prng;
    /** Borrowed */
    فراغ *prng_state;
    size_t لمب_events;
    /** Realistic typing events */
    بنية xkb_typing_event *events ATTR_COUNTED_BY(لمب_events);
    /** Pending events */
    darray(بنية xkb_pending_typing_event) pending;
    darray_size_t لمب_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

تعداد {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

بنية xkb_typing_شغرو {
    بنية xkb_typing_event *events;
    size_t لمب_events;
};

بنية xkb_typing_شغرو_config {
    xkb_prng_t prng;
    فراغ *prng_state;
    size_t شغرو_length;
};

تعداد xkb_status
xkb_typing_شغرو_init(بنية xkb_typing_شغرو *typing,
                      ثابت بنية xkb_مبودي_set *مبوداي,
                      ثابت بنية xkb_typing_شغرو_config *config);

فراغ
xkb_typing_شغرو_destroy(بنية xkb_typing_شغرو *typing);
