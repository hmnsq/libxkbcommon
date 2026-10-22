/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#आशय once

#समावेश "config.h"

#समावेश <stddef.h>
#समावेश <stdint.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "darray.h"

क्रमागत xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

क्रमागत xkb_कुंजी_kind {
    XKB_कुंजी_PRINTABLE,
    XKB_कुंजी_MODIFIER,
    XKB_कुंजी_MISC,
    अंक_XKB_कुंजी_KIND,
};

काष्ठा xkb_modifier_कुंजी {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    क्रमागत xkb_modifier_kind kind;
};

प्रकार darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
काष्ठा xkb_कुंजी_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(काष्ठा xkb_modifier_कुंजी) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

काष्ठा xkb_कुंजी_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

क्रमागत xkb_status
xkb_कुंजी_set_init(काष्ठा xkb_कुंजी_set *set,
                 काष्ठा xkb_keymap *keymap,
                 स्थिर काष्ठा xkb_कुंजी_set_config *config);

व्योम
xkb_कुंजी_set_destroy(काष्ठा xkb_कुंजी_set *set);

काष्ठा xkb_typing_event {
    xkb_keycode_t keycode;
    क्रमागत xkb_कुंजी_direction direction;
};

प्रकार darray(काष्ठा xkb_typing_event) xkb_typing_events;

काष्ठा xkb_pending_typing_event {
    काष्ठा xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t गणक;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
प्रकार दीर्घ (*xkb_prng_t)(व्योम *);

काष्ठा xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    स्थिर काष्ठा xkb_कुंजी_set *कुंजियाँ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *कुंजियाँ_state;
    xkb_prng_t prng;
    /** Borrowed */
    व्योम *prng_state;
    माप_प्रकार अंक_events;
    /** Realistic typing events */
    काष्ठा xkb_typing_event *events ATTR_COUNTED_BY(अंक_events);
    /** Pending events */
    darray(काष्ठा xkb_pending_typing_event) pending;
    darray_आकार_t अंक_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

क्रमागत {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

काष्ठा xkb_typing_निवेश {
    काष्ठा xkb_typing_event *events;
    माप_प्रकार अंक_events;
};

काष्ठा xkb_typing_निवेश_config {
    xkb_prng_t prng;
    व्योम *prng_state;
    माप_प्रकार निवेश_लंबाई;
};

क्रमागत xkb_status
xkb_typing_निवेश_init(काष्ठा xkb_typing_निवेश *typing,
                      स्थिर काष्ठा xkb_कुंजी_set *कुंजियाँ,
                      स्थिर काष्ठा xkb_typing_निवेश_config *config);

व्योम
xkb_typing_निवेश_destroy(काष्ठा xkb_typing_निवेश *typing);
