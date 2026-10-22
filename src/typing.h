/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#هدايت once

#ضمن "config.h"

#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "darray.h"

تعداد xkb_modifier_قسم {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_سيٽ,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_تالو,
};

تعداد xkb_ڪنجي_قسم {
    XKB_ڪنجي_PRINTABLE,
    XKB_ڪنجي_MODIFIER,
    XKB_ڪنجي_MISC,
    انگ_XKB_ڪنجي_قسم,
};

بنية xkb_modifier_ڪنجي {
    xkb_keycode_t keycode;
    xkb_mod_نقاب_t mods;
    تعداد xkb_modifier_قسم قسم;
};

تعريفنوع darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
بنية xkb_ڪنجي_سيٽ {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(بنية xkb_modifier_ڪنجي) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

بنية xkb_ڪنجي_سيٽ_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

تعداد xkb_حالت
xkb_ڪنجي_سيٽ_init(بنية xkb_ڪنجي_سيٽ *سيٽ,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_ڪنجي_سيٽ_config *config);

فراغ
xkb_ڪنجي_سيٽ_destroy(بنية xkb_ڪنجي_سيٽ *سيٽ);

بنية xkb_typing_event {
    xkb_keycode_t keycode;
    تعداد xkb_ڪنجي_direction direction;
};

تعريفنوع darray(بنية xkb_typing_event) xkb_typing_events;

بنية xkb_pending_typing_event {
    بنية xkb_typing_event event;
    xkb_mod_نقاب_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
تعريفنوع ڊگهو (*xkb_prng_t)(فراغ *);

بنية xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ثابت بنية xkb_ڪنجي_سيٽ *ڪنجيون;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *ڪنجيون_state;
    xkb_prng_t prng;
    /** Borrowed */
    فراغ *prng_state;
    سائيز_t انگ_events;
    /** Realistic typing events */
    بنية xkb_typing_event *events ATTR_COUNTED_BY(انگ_events);
    /** Pending events */
    darray(بنية xkb_pending_typing_event) pending;
    darray_سائيز_t انگ_pending;
    /** Effective mods */
    xkb_mod_نقاب_t mods;
};

تعداد {
    XKB_TYPING_EVENT_PRINTABLE_فوري_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_فوري_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

بنية xkb_typing_داخلا {
    بنية xkb_typing_event *events;
    سائيز_t انگ_events;
};

بنية xkb_typing_داخلا_config {
    xkb_prng_t prng;
    فراغ *prng_state;
    سائيز_t داخلا_ڊيگهائي;
};

تعداد xkb_حالت
xkb_typing_داخلا_init(بنية xkb_typing_داخلا *typing,
                      ثابت بنية xkb_ڪنجي_سيٽ *ڪنجيون,
                      ثابت بنية xkb_typing_داخلا_config *config);

فراغ
xkb_typing_داخلا_destroy(بنية xkb_typing_داخلا *typing);
