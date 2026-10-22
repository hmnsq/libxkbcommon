/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "darray.h"

గణన xkb_modifier_రకం {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_సమితి,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_తాళం,
};

గణన xkb_కీలకం_రకం {
    XKB_కీలకం_PRINTABLE,
    XKB_కీలకం_MODIFIER,
    XKB_కీలకం_MISC,
    అంకె_XKB_కీలకం_రకం,
};

నిర్మాణం xkb_modifier_కీలకం {
    xkb_keycode_t keycode;
    xkb_mod_ముసుగు_t mods;
    గణన xkb_modifier_రకం రకం;
};

రకనిర్వచనం darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
నిర్మాణం xkb_కీలకం_సమితి {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(నిర్మాణం xkb_modifier_కీలకం) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

నిర్మాణం xkb_కీలకం_సమితి_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

గణన xkb_స్థితి
xkb_కీలకం_సమితి_init(నిర్మాణం xkb_కీలకం_సమితి *సమితి,
                 నిర్మాణం xkb_keymap *keymap,
                 స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి_config *config);

శూన్య
xkb_కీలకం_సమితి_destroy(నిర్మాణం xkb_కీలకం_సమితి *సమితి);

నిర్మాణం xkb_typing_event {
    xkb_keycode_t keycode;
    గణన xkb_కీలకం_direction direction;
};

రకనిర్వచనం darray(నిర్మాణం xkb_typing_event) xkb_typing_events;

నిర్మాణం xkb_pending_typing_event {
    నిర్మాణం xkb_typing_event event;
    xkb_mod_ముసుగు_t mods;
    uint8_t counter;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
రకనిర్వచనం దీర్ఘ (*xkb_prng_t)(శూన్య *);

నిర్మాణం xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి *కీలకాలు;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *కీలకాలు_state;
    xkb_prng_t prng;
    /** Borrowed */
    శూన్య *prng_state;
    కొలత_t అంకె_events;
    /** Realistic typing events */
    నిర్మాణం xkb_typing_event *events ATTR_COUNTED_BY(అంకె_events);
    /** Pending events */
    darray(నిర్మాణం xkb_pending_typing_event) pending;
    darray_కొలత_t అంకె_pending;
    /** Effective mods */
    xkb_mod_ముసుగు_t mods;
};

గణన {
    XKB_TYPING_EVENT_PRINTABLE_తక్షణ_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_తక్షణ_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

నిర్మాణం xkb_typing_ఆదానం {
    నిర్మాణం xkb_typing_event *events;
    కొలత_t అంకె_events;
};

నిర్మాణం xkb_typing_ఆదానం_config {
    xkb_prng_t prng;
    శూన్య *prng_state;
    కొలత_t ఆదానం_నిడివి;
};

గణన xkb_స్థితి
xkb_typing_ఆదానం_init(నిర్మాణం xkb_typing_ఆదానం *typing,
                      స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి *కీలకాలు,
                      స్థిరరాశి నిర్మాణం xkb_typing_ఆదానం_config *config);

శూన్య
xkb_typing_ఆదానం_destroy(నిర్మాణం xkb_typing_ఆదానం *typing);
