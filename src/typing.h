/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "darray.h"

എണ്ണൽ xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

എണ്ണൽ xkb_താക്കോൽ_kind {
    XKB_താക്കോൽ_PRINTABLE,
    XKB_താക്കോൽ_MODIFIER,
    XKB_താക്കോൽ_MISC,
    അക്കം_XKB_താക്കോൽ_KIND,
};

ഘടന xkb_modifier_താക്കോൽ {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    എണ്ണൽ xkb_modifier_kind kind;
};

തരനിർവചനം darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
ഘടന xkb_താക്കോൽ_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(ഘടന xkb_modifier_താക്കോൽ) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

ഘടന xkb_താക്കോൽ_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

എണ്ണൽ xkb_status
xkb_താക്കോൽ_set_init(ഘടന xkb_താക്കോൽ_set *set,
                 ഘടന xkb_keymap *keymap,
                 സ്ഥിരം ഘടന xkb_താക്കോൽ_set_config *config);

ശൂന്യം
xkb_താക്കോൽ_set_destroy(ഘടന xkb_താക്കോൽ_set *set);

ഘടന xkb_typing_event {
    xkb_keycode_t keycode;
    എണ്ണൽ xkb_താക്കോൽ_direction direction;
};

തരനിർവചനം darray(ഘടന xkb_typing_event) xkb_typing_events;

ഘടന xkb_pending_typing_event {
    ഘടന xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
തരനിർവചനം long (*xkb_prng_t)(ശൂന്യം *);

ഘടന xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    സ്ഥിരം ഘടന xkb_താക്കോൽ_set *താക്കോലുകൾ;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *താക്കോലുകൾ_state;
    xkb_prng_t prng;
    /** Borrowed */
    ശൂന്യം *prng_state;
    size_t അക്കം_events;
    /** Realistic typing events */
    ഘടന xkb_typing_event *events ATTR_COUNTED_BY(അക്കം_events);
    /** Pending events */
    darray(ഘടന xkb_pending_typing_event) pending;
    darray_size_t അക്കം_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

എണ്ണൽ {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

ഘടന xkb_typing_നിവേശം {
    ഘടന xkb_typing_event *events;
    size_t അക്കം_events;
};

ഘടന xkb_typing_നിവേശം_config {
    xkb_prng_t prng;
    ശൂന്യം *prng_state;
    size_t നിവേശം_length;
};

എണ്ണൽ xkb_status
xkb_typing_നിവേശം_init(ഘടന xkb_typing_നിവേശം *typing,
                      സ്ഥിരം ഘടന xkb_താക്കോൽ_set *താക്കോലുകൾ,
                      സ്ഥിരം ഘടന xkb_typing_നിവേശം_config *config);

ശൂന്യം
xkb_typing_നിവേശം_destroy(ഘടന xkb_typing_നിവേശം *typing);
