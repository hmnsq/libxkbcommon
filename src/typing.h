/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"

ଗଣନା xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

ଗଣନା xkb_ଚାବି_kind {
    XKB_ଚାବି_PRINTABLE,
    XKB_ଚାବି_MODIFIER,
    XKB_ଚାବି_MISC,
    ଅଙ୍କ_XKB_ଚାବି_KIND,
};

ସଂରଚନା xkb_modifier_ଚାବି {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    ଗଣନା xkb_modifier_kind kind;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ସଂରଚନା xkb_ଚାବି_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ସଂରଚନା xkb_modifier_ଚାବି) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ସଂରଚନା xkb_ଚାବି_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

ଗଣନା xkb_status
xkb_ଚାବି_set_init(ସଂରଚନା xkb_ଚାବି_set *set,
                 ସଂରଚନା xkb_keymap *keymap,
                 ଅଚଳ ସଂରଚନା xkb_ଚାବି_set_config *config);

ଶୂନ୍ୟ
xkb_ଚାବି_set_destroy(ସଂରଚନା xkb_ଚାବି_set *set);

ସଂରଚନା xkb_typing_event {
    xkb_keycode_t keycode;
    ଗଣନା xkb_ଚାବି_direction direction;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା xkb_typing_event) xkb_typing_events;

ସଂରଚନା xkb_pending_typing_event {
    ସଂରଚନା xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
ପ୍ରକାରବ୍ୟାଖ୍ୟା long (*xkb_prng_t)(ଶୂନ୍ୟ *);

ସଂରଚନା xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି_set *ଚାବିଗୁଡ଼ିକ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *ଚାବିଗୁଡ଼ିକ_state;
    xkb_prng_t prng;
    /** Borrowed */
    ଶୂନ୍ୟ *prng_state;
    size_t ଅଙ୍କ_events;
    /** Realistic typing events */
    ସଂରଚନା xkb_typing_event *events ATTR_COUNTED_BY(ଅଙ୍କ_events);
    /** Pending events */
    darray(ସଂରଚନା xkb_pending_typing_event) pending;
    darray_size_t ଅଙ୍କ_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

ଗଣନା {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ସଂରଚନା xkb_typing_ନିବେଶ {
    ସଂରଚନା xkb_typing_event *events;
    size_t ଅଙ୍କ_events;
};

ସଂରଚନା xkb_typing_ନିବେଶ_config {
    xkb_prng_t prng;
    ଶୂନ୍ୟ *prng_state;
    size_t ନିବେଶ_length;
};

ଗଣନା xkb_status
xkb_typing_ନିବେଶ_init(ସଂରଚନା xkb_typing_ନିବେଶ *typing,
                      ଅଚଳ ସଂରଚନା xkb_ଚାବି_set *ଚାବିଗୁଡ଼ିକ,
                      ଅଚଳ ସଂରଚନା xkb_typing_ନିବେଶ_config *config);

ଶୂନ୍ୟ
xkb_typing_ନିବେଶ_destroy(ସଂରଚନା xkb_typing_ନିବେଶ *typing);
