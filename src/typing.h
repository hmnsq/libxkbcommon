/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdint.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "darray.h"

எண்ணிக்கை xkb_modifier_kind {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_SET,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_LOCK,
};

எண்ணிக்கை xkb_சாவி_kind {
    XKB_சாவி_PRINTABLE,
    XKB_சாவி_MODIFIER,
    XKB_சாவி_MISC,
    இலக்கம்_XKB_சாவி_KIND,
};

கட்டமைப்பு xkb_modifier_சாவி {
    xkb_keycode_t keycode;
    xkb_mod_mask_t mods;
    எண்ணிக்கை xkb_modifier_kind kind;
};

வகைவரை darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
கட்டமைப்பு xkb_சாவி_set {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(கட்டமைப்பு xkb_modifier_சாவி) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

கட்டமைப்பு xkb_சாவி_set_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

எண்ணிக்கை xkb_status
xkb_சாவி_set_init(கட்டமைப்பு xkb_சாவி_set *set,
                 கட்டமைப்பு xkb_keymap *keymap,
                 மாறிலி கட்டமைப்பு xkb_சாவி_set_config *config);

வெறுமை
xkb_சாவி_set_destroy(கட்டமைப்பு xkb_சாவி_set *set);

கட்டமைப்பு xkb_typing_event {
    xkb_keycode_t keycode;
    எண்ணிக்கை xkb_சாவி_direction direction;
};

வகைவரை darray(கட்டமைப்பு xkb_typing_event) xkb_typing_events;

கட்டமைப்பு xkb_pending_typing_event {
    கட்டமைப்பு xkb_typing_event event;
    xkb_mod_mask_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
வகைவரை long (*xkb_prng_t)(வெறுமை *);

கட்டமைப்பு xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    மாறிலி கட்டமைப்பு xkb_சாவி_set *சாவிகள்;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *சாவிகள்_state;
    xkb_prng_t prng;
    /** Borrowed */
    வெறுமை *prng_state;
    size_t இலக்கம்_events;
    /** Realistic typing events */
    கட்டமைப்பு xkb_typing_event *events ATTR_COUNTED_BY(இலக்கம்_events);
    /** Pending events */
    darray(கட்டமைப்பு xkb_pending_typing_event) pending;
    darray_size_t இலக்கம்_pending;
    /** Effective mods */
    xkb_mod_mask_t mods;
};

எண்ணிக்கை {
    XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

கட்டமைப்பு xkb_typing_உள்ளீடு {
    கட்டமைப்பு xkb_typing_event *events;
    size_t இலக்கம்_events;
};

கட்டமைப்பு xkb_typing_உள்ளீடு_config {
    xkb_prng_t prng;
    வெறுமை *prng_state;
    size_t உள்ளீடு_length;
};

எண்ணிக்கை xkb_status
xkb_typing_உள்ளீடு_init(கட்டமைப்பு xkb_typing_உள்ளீடு *typing,
                      மாறிலி கட்டமைப்பு xkb_சாவி_set *சாவிகள்,
                      மாறிலி கட்டமைப்பு xkb_typing_உள்ளீடு_config *config);

வெறுமை
xkb_typing_உள்ளீடு_destroy(கட்டமைப்பு xkb_typing_உள்ளீடு *typing);
