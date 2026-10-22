/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "darray.h"

تعداد xkb_modifier_ډول {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_سېټ,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_قلف,
};

تعداد xkb_کلی_ډول {
    XKB_کلی_PRINTABLE,
    XKB_کلی_MODIFIER,
    XKB_کلی_MISC,
    عدد_XKB_کلی_ډول,
};

بنية xkb_modifier_کلی {
    xkb_keycode_t keycode;
    xkb_mod_ماسک_t mods;
    تعداد xkb_modifier_ډول ډول;
};

تعريفنوع darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
بنية xkb_کلی_سېټ {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(بنية xkb_modifier_کلی) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

بنية xkb_کلی_سېټ_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

تعداد xkb_حالت
xkb_کلی_سېټ_init(بنية xkb_کلی_سېټ *سېټ,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_کلی_سېټ_config *config);

فراغ
xkb_کلی_سېټ_destroy(بنية xkb_کلی_سېټ *سېټ);

بنية xkb_typing_event {
    xkb_keycode_t keycode;
    تعداد xkb_کلی_direction direction;
};

تعريفنوع darray(بنية xkb_typing_event) xkb_typing_events;

بنية xkb_pending_typing_event {
    بنية xkb_typing_event event;
    xkb_mod_ماسک_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
تعريفنوع اوږد (*xkb_prng_t)(فراغ *);

بنية xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ثابت بنية xkb_کلی_سېټ *کلیګانې;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *کلیګانې_state;
    xkb_prng_t prng;
    /** Borrowed */
    فراغ *prng_state;
    اندازه_t عدد_events;
    /** Realistic typing events */
    بنية xkb_typing_event *events ATTR_COUNTED_BY(عدد_events);
    /** Pending events */
    darray(بنية xkb_pending_typing_event) pending;
    darray_اندازه_t عدد_pending;
    /** Effective mods */
    xkb_mod_ماسک_t mods;
};

تعداد {
    XKB_TYPING_EVENT_PRINTABLE_سمدستي_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_سمدستي_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

بنية xkb_typing_ننوت {
    بنية xkb_typing_event *events;
    اندازه_t عدد_events;
};

بنية xkb_typing_ننوت_config {
    xkb_prng_t prng;
    فراغ *prng_state;
    اندازه_t ننوت_اوږدتیا;
};

تعداد xkb_حالت
xkb_typing_ننوت_init(بنية xkb_typing_ننوت *typing,
                      ثابت بنية xkb_کلی_سېټ *کلیګانې,
                      ثابت بنية xkb_typing_ننوت_config *config);

فراغ
xkb_typing_ننوت_destroy(بنية xkb_typing_ننوت *typing);
