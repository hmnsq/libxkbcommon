/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "darray.h"

ગણના xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

ગણના xkb_ચાવી_kind {
    XKB_ચાવી_PRINTABLE,
    XKB_ચાવી_MODIFIER,
    XKB_ચાવી_MISC,
    અંક_XKB_ચાવી_KIND,
};

સંરચના xkb_modifier_ચાવી {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    ગણના xkb_modifier_kind kind;
};

પ્રકારવ્યાખ્યા darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
સંરચના xkb_ચાવી_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(સંરચના xkb_modifier_ચાવી) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

સંરચના xkb_ચાવી_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

ગણના xkb_status
xkb_ચાવી_set_init(સંરચના xkb_ચાવી_set *set,
                 સંરચના xkb_keymap *keymap,
                 અચલ સંરચના xkb_ચાવી_set_config *config);

શૂન્ય
xkb_ચાવી_set_destroy(સંરચના xkb_ચાવી_set *set);

સંરચના xkb_typing_event {
    xkb_keycode_t keycode;
    ગણના xkb_ચાવી_direction direction;
};

પ્રકારવ્યાખ્યા darray(સંરચના xkb_typing_event) xkb_typing_events;

સંરચના xkb_pending_typing_event {
    સંરચના xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
પ્રકારવ્યાખ્યા long (*xkb_prng_t)(શૂન્ય *);

સંરચના xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    અચલ સંરચના xkb_ચાવી_set *ચાવીઓ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *ચાવીઓ_state;
    xkb_prng_t prng;
    /** Borrowed */
    શૂન્ય *prng_state;
    size_t અંક_events;
    /** Realistic typing events */
    સંરચના xkb_typing_event *events ATTR_COUNTED_BY(અંક_events);
    /** Pending events */
    darray(સંરચના xkb_pending_typing_event) pending;
    darray_size_t અંક_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

ગણના {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

સંરચના xkb_typing_આગત {
    સંરચના xkb_typing_event *events;
    size_t અંક_events;
};

સંરચના xkb_typing_આગત_config {
    xkb_prng_t prng;
    શૂન્ય *prng_state;
    size_t આગત_length;
};

ગણના xkb_status
xkb_typing_આગત_init(સંરચના xkb_typing_આગત *typing,
                      અચલ સંરચના xkb_ચાવી_set *ચાવીઓ,
                      અચલ સંરચના xkb_typing_આગત_config *config);

શૂન્ય
xkb_typing_આગત_destroy(સંરચના xkb_typing_આગત *typing);
