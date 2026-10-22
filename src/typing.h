/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "darray.h"

गणना xkb_modifier_विधा {
    /** Shift, Ctrl, Alt: press — other key(s) — release */
    XKB_MOD_समुच्चयः,
    /** sticky-keys tap: press — release — exactly one other key */
    XKB_MOD_LATCH,
    /** CapsLock, NumLock: press — release, toggles persistent state */
    XKB_MOD_तालकम्,
};

गणना xkb_कुञ्चिका_विधा {
    XKB_कुञ्चिका_PRINTABLE,
    XKB_कुञ्चिका_MODIFIER,
    XKB_कुञ्चिका_MISC,
    अङ्कः_XKB_कुञ्चिका_विधा,
};

संरचना xkb_modifier_कुञ्चिका {
    xkb_keycode_t keycode;
    xkb_mod_आवरणम्_t mods;
    गणना xkb_modifier_विधा विधा;
};

प्रकारनिर्देश darray(xkb_keycode_t) xkb_keycodes_t;

/** Keys classified by category */
संरचना xkb_कुञ्चिका_समुच्चयः {
    /** Printable keys: alphanum, punctuation, symbols, etc. */
    xkb_keycodes_t printable;
    /** Modifier keys */
    darray(संरचना xkb_modifier_कुञ्चिका) modifiers;
    /** Miscellaneous: arrows, editing, functions, system, etc. */
    xkb_keycodes_t misc;
};

संरचना xkb_कुञ्चिका_समुच्चयः_config {
    xkb_keycode_t min;
    xkb_keycode_t max;
};

गणना xkb_स्थितिः
xkb_कुञ्चिका_समुच्चयः_init(संरचना xkb_कुञ्चिका_समुच्चयः *समुच्चयः,
                 संरचना xkb_keymap *keymap,
                 ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः_config *config);

शून्य
xkb_कुञ्चिका_समुच्चयः_destroy(संरचना xkb_कुञ्चिका_समुच्चयः *समुच्चयः);

संरचना xkb_typing_event {
    xkb_keycode_t keycode;
    गणना xkb_कुञ्चिका_direction direction;
};

प्रकारनिर्देश darray(संरचना xkb_typing_event) xkb_typing_events;

संरचना xkb_pending_typing_event {
    संरचना xkb_typing_event event;
    xkb_mod_आवरणम्_t mods;
    uint8_t गणक;
};

/** PRNG: return values must be between 0 and 2^31 - 1 (see random()) */
प्रकारनिर्देश दीर्घ (*xkb_prng_t)(शून्य *);

संरचना xkb_typing_state {
    /** Keys used for typing, classified by category. Borrowed */
    ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः *कुञ्चिकाः;
    /** Keys state: pressed (1) or released (0) */
    uintptr_t *कुञ्चिकाः_state;
    xkb_prng_t prng;
    /** Borrowed */
    शून्य *prng_state;
    आकार_t अङ्कः_events;
    /** Realistic typing events */
    संरचना xkb_typing_event *events ATTR_COUNTED_BY(अङ्कः_events);
    /** Pending events */
    darray(संरचना xkb_pending_typing_event) pending;
    darray_आकार_t अङ्कः_pending;
    /** Effective mods */
    xkb_mod_आवरणम्_t mods;
};

गणना {
    XKB_TYPING_EVENT_PRINTABLE_तात्कालिकम्_RELEASE_PERCENT = 75,
    XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS = 1,
    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS = 2,
    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS = 10,
    XKB_TYPING_EVENT_MISC_तात्कालिकम्_RELEASE_PERCENT = 100,
    XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS = 0,
};

संरचना xkb_typing_निवेशः {
    संरचना xkb_typing_event *events;
    आकार_t अङ्कः_events;
};

संरचना xkb_typing_निवेशः_config {
    xkb_prng_t prng;
    शून्य *prng_state;
    आकार_t निवेशः_दीर्घता;
};

गणना xkb_स्थितिः
xkb_typing_निवेशः_init(संरचना xkb_typing_निवेशः *typing,
                      ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः *कुञ्चिकाः,
                      ध्रुव संरचना xkb_typing_निवेशः_config *config);

शून्य
xkb_typing_निवेशः_destroy(संरचना xkb_typing_निवेशः *typing);
