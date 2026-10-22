/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "darray.h"

ಗಣನಾ xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

ಗಣನಾ xkb_ಕೀಲಿ_kind {
    XKB_ಕೀಲಿ_PRINTABLE,
    XKB_ಕೀಲಿ_MODIFIER,
    XKB_ಕೀಲಿ_MISC,
    ಅಂಕೆ_XKB_ಕೀಲಿ_KIND,
};

ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    ಗಣನಾ xkb_modifier_kind kind;
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ಸಂರಚನಾ xkb_ಕೀಲಿ_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ಸಂರಚನಾ xkb_ಕೀಲಿ_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

ಗಣನಾ xkb_status
xkb_ಕೀಲಿ_set_init(ಸಂರಚನಾ xkb_ಕೀಲಿ_set *set,
                 ಸಂರಚನಾ xkb_keymap *keymap,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set_config *config);

ಶೂನ್ಯ
xkb_ಕೀಲಿ_set_destroy(ಸಂರಚನಾ xkb_ಕೀಲಿ_set *set);

ಸಂರಚನಾ xkb_typing_event {
    xkb_keycode_t keycode;
    ಗಣನಾ xkb_ಕೀಲಿ_direction direction;
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ xkb_typing_event) xkb_typing_events;

ಸಂರಚನಾ xkb_pending_typing_event {
    ಸಂರಚನಾ xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
ಪ್ರಕಾರನಿರ್ದೇಶ long (*xkb_prng_t)(ಶೂನ್ಯ *);

ಸಂರಚನಾ xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set *ಕೀಲಿಗಳು;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *ಕೀಲಿಗಳು_state;
    xkb_prng_t prng;
    /** Borrowed */
    ಶೂನ್ಯ *prng_state;
    size_t ಅಂಕೆ_events;
    /** Realistic typing events */
    ಸಂರಚನಾ xkb_typing_event *events ATTR_COUNTED_BY(ಅಂಕೆ_events);
    /** Pending events */
    darray(ಸಂರಚನಾ xkb_pending_typing_event) pending;
    darray_size_t ಅಂಕೆ_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

ಗಣನಾ {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು {
    ಸಂರಚನಾ xkb_typing_event *events;
    size_t ಅಂಕೆ_events;
};

ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು_config {
    xkb_prng_t prng;
    ಶೂನ್ಯ *prng_state;
    size_t ಒಳಹರಿವು_length;
};

ಗಣನಾ xkb_status
xkb_typing_ಒಳಹರಿವು_init(ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು *typing,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set *ಕೀಲಿಗಳು,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು_config *config);

ಶೂನ್ಯ
xkb_typing_ಒಳಹರಿವು_destroy(ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು *typing);
