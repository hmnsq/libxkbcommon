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

تعداد xkb_مفتاح_kind {
    XKB_مفتاح_PRINTABLE,
    XKB_مفتاح_MODIFIER,
    XKB_مفتاح_MISC,
    رقم_XKB_مفتاح_KIND,
};

بنية xkb_modifier_مفتاح {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    تعداد xkb_modifier_kind kind;
};

تعريفنوع darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
بنية xkb_مفتاح_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(بنية xkb_modifier_مفتاح) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

بنية xkb_مفتاح_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

تعداد xkb_status
xkb_مفتاح_set_init(بنية xkb_مفتاح_set *set,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_مفتاح_set_config *config);

فراغ
xkb_مفتاح_set_destroy(بنية xkb_مفتاح_set *set);

بنية xkb_typing_event {
    xkb_keycode_t keycode;
    تعداد xkb_مفتاح_direction direction;
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
    ثابت بنية xkb_مفتاح_set *مفاتيح;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *مفاتيح_state;
    xkb_prng_t prng;
    /** Borrowed */
    فراغ *prng_state;
    size_t رقم_events;
    /** Realistic typing events */
    بنية xkb_typing_event *events ATTR_COUNTED_BY(رقم_events);
    /** Pending events */
    darray(بنية xkb_pending_typing_event) pending;
    darray_size_t رقم_pending;
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

بنية xkb_typing_مدخل {
    بنية xkb_typing_event *events;
    size_t رقم_events;
};

بنية xkb_typing_مدخل_config {
    xkb_prng_t prng;
    فراغ *prng_state;
    size_t مدخل_length;
};

تعداد xkb_status
xkb_typing_مدخل_init(بنية xkb_typing_مدخل *typing,
                      ثابت بنية xkb_مفتاح_set *مفاتيح,
                      ثابت بنية xkb_typing_مدخل_config *config);

فراغ
xkb_typing_مدخل_destroy(بنية xkb_typing_مدخل *typing);
