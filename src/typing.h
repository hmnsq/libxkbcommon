/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#شامل "config.h"

#شامل <stddef.h>
#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "darray.h"

شمار xkb_modifier_قسم {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_سیٹ,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_تالا,
};

شمار xkb_کلید_قسم {
    XKB_کلید_PRINTABLE,
    XKB_کلید_MODIFIER,
    XKB_کلید_MISC,
    عدد_XKB_کلید_قسم,
};

ساخت xkb_modifier_کلید {
    xkb_keycode_t keycode;
    xkb_mod_نقاب_t mods;
    شمار xkb_modifier_قسم قسم;
};

قسمتعریف darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ساخت xkb_کلید_سیٹ {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ساخت xkb_modifier_کلید) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ساخت xkb_کلید_سیٹ_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

شمار xkb_حالت
xkb_کلید_سیٹ_init(ساخت xkb_کلید_سیٹ *سیٹ,
                 ساخت xkb_keymap *keymap,
                 ثابت ساخت xkb_کلید_سیٹ_config *config);

خالی
xkb_کلید_سیٹ_destroy(ساخت xkb_کلید_سیٹ *سیٹ);

ساخت xkb_typing_event {
    xkb_keycode_t keycode;
    شمار xkb_کلید_direction direction;
};

قسمتعریف darray(ساخت xkb_typing_event) xkb_typing_events;

ساخت xkb_pending_typing_event {
    ساخت xkb_typing_event event;
    xkb_mod_نقاب_t mods;
    uint8_t شمار_2;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
قسمتعریف طویل (*xkb_prng_t)(خالی *);

ساخت xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ثابت ساخت xkb_کلید_سیٹ *کلیدیں;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *کلیدیں_state;
    xkb_prng_t prng;
    /** Borrowed */
    خالی *prng_state;
    سائز_t عدد_events;
    /** Realistic typing events */
    ساخت xkb_typing_event *events ATTR_COUNTED_BY(عدد_events);
    /** Pending events */
    darray(ساخت xkb_pending_typing_event) pending;
    darray_سائز_t عدد_pending;
    /** Effective mods */
    xkb_mod_نقاب_t mods;
};

شمار {
    XKB_TYPING_EVENT_PRINTABLE_فوری_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_فوری_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ساخت xkb_typing_داخلہ {
    ساخت xkb_typing_event *events;
    سائز_t عدد_events;
};

ساخت xkb_typing_داخلہ_config {
    xkb_prng_t prng;
    خالی *prng_state;
    سائز_t داخلہ_لمبائی;
};

شمار xkb_حالت
xkb_typing_داخلہ_init(ساخت xkb_typing_داخلہ *typing,
                      ثابت ساخت xkb_کلید_سیٹ *کلیدیں,
                      ثابت ساخت xkb_typing_داخلہ_config *config);

خالی
xkb_typing_داخلہ_destroy(ساخت xkb_typing_داخلہ *typing);
