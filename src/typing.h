/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"

ਗਿਣਤੀ xkb_modifier_ਕਿਸਮ {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_ਸੈੱਟ,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_ਤਾਲਾ,
};

ਗਿਣਤੀ xkb_ਕੁੰਜੀ_ਕਿਸਮ {
    XKB_ਕੁੰਜੀ_PRINTABLE,
    XKB_ਕੁੰਜੀ_MODIFIER,
    XKB_ਕੁੰਜੀ_MISC,
    ਅੰਕ_XKB_ਕੁੰਜੀ_ਕਿਸਮ,
};

ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ {
    xkb_keycode_t keycode;
    xkb_mod_ਨਕਾਬ_t mods;
    ਗਿਣਤੀ xkb_modifier_ਕਿਸਮ ਕਿਸਮ;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_ਕੁੰਜੀ_ਸੈੱਟ_init(ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਸੈੱਟ,
                 ਬਣਤਰ xkb_keymap *keymap,
                 ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ_config *config);

ਖਾਲੀ
xkb_ਕੁੰਜੀ_ਸੈੱਟ_destroy(ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਸੈੱਟ);

ਬਣਤਰ xkb_typing_event {
    xkb_keycode_t keycode;
    ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ xkb_typing_event) xkb_typing_events;

ਬਣਤਰ xkb_pending_typing_event {
    ਬਣਤਰ xkb_typing_event event;
    xkb_mod_ਨਕਾਬ_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ (*xkb_prng_t)(ਖਾਲੀ *);

ਬਣਤਰ xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਕੁੰਜੀਆਂ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *ਕੁੰਜੀਆਂ_state;
    xkb_prng_t prng;
    /** Borrowed */
    ਖਾਲੀ *prng_state;
    ਮਾਪ_t ਅੰਕ_events;
    /** Realistic typing events */
    ਬਣਤਰ xkb_typing_event *events ATTR_COUNTED_BY(ਅੰਕ_events);
    /** Pending events */
    darray(ਬਣਤਰ xkb_pending_typing_event) pending;
    darray_ਮਾਪ_t ਅੰਕ_pending;
    /** Effective mods */
    xkb_mod_ਨਕਾਬ_t mods;
};

ਗਿਣਤੀ {
    XKB_TYPING_EVENT_PRINTABLE_ਤੁਰੰਤ_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_ਤੁਰੰਤ_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼ {
    ਬਣਤਰ xkb_typing_event *events;
    ਮਾਪ_t ਅੰਕ_events;
};

ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼_config {
    xkb_prng_t prng;
    ਖਾਲੀ *prng_state;
    ਮਾਪ_t ਨਿਵੇਸ਼_ਲੰਮਾਈ;
};

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_typing_ਨਿਵੇਸ਼_init(ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼ *typing,
                      ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਕੁੰਜੀਆਂ,
                      ਅਚਲ ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼_config *config);

ਖਾਲੀ
xkb_typing_ਨਿਵੇਸ਼_destroy(ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼ *typing);
