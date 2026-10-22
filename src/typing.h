/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#شامل "config.h"

#شامل <stddef.h>
#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "darray.h"

شمارش xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

شمارش xkb_کلید_kind {
    XKB_کلید_PRINTABLE,
    XKB_کلید_MODIFIER,
    XKB_کلید_MISC,
    رقم_XKB_کلید_KIND,
};

ساختار xkb_modifier_کلید {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    شمارش xkb_modifier_kind kind;
};

تعریفنوع darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ساختار xkb_کلید_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ساختار xkb_modifier_کلید) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ساختار xkb_کلید_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

شمارش xkb_status
xkb_کلید_set_init(ساختار xkb_کلید_set *set,
                 ساختار xkb_keymap *keymap,
                 ثابت ساختار xkb_کلید_set_config *config);

تهی
xkb_کلید_set_destroy(ساختار xkb_کلید_set *set);

ساختار xkb_typing_event {
    xkb_keycode_t keycode;
    شمارش xkb_کلید_direction direction;
};

تعریفنوع darray(ساختار xkb_typing_event) xkb_typing_events;

ساختار xkb_pending_typing_event {
    ساختار xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
تعریفنوع long (*xkb_prng_t)(تهی *);

ساختار xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ثابت ساختار xkb_کلید_set *کلیدها;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *کلیدها_state;
    xkb_prng_t prng;
    /** Borrowed */
    تهی *prng_state;
    size_t رقم_events;
    /** Realistic typing events */
    ساختار xkb_typing_event *events ATTR_COUNTED_BY(رقم_events);
    /** Pending events */
    darray(ساختار xkb_pending_typing_event) pending;
    darray_size_t رقم_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

شمارش {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ساختار xkb_typing_درون_داد {
    ساختار xkb_typing_event *events;
    size_t رقم_events;
};

ساختار xkb_typing_درون_داد_config {
    xkb_prng_t prng;
    تهی *prng_state;
    size_t درون_داد_length;
};

شمارش xkb_status
xkb_typing_درون_داد_init(ساختار xkb_typing_درون_داد *typing,
                      ثابت ساختار xkb_کلید_set *کلیدها,
                      ثابت ساختار xkb_typing_درون_داد_config *config);

تهی
xkb_typing_درون_داد_destroy(ساختار xkb_typing_درون_داد *typing);
