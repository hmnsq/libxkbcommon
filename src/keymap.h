/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For HPND
 * Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 *
 * SPDX-License-Identifier: MIT-open-group AND HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Dan Nicholson <dbn.lists@gmail.com>
 */
#निर्देशः once

 /* Don't use compat names in internal code. */
#परिभाषय _XKBCOMMON_COMPAT_H

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "atom.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "rmlvo.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#परिभाषय XKB_MAX_GROUPS 32
#परिभाषय XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#परिभाषय XKB_MAX_GROUPS_X11 4

स्थिर अन्तःस्थ xkb_layout_सूचकांक_t
format_max_groups(गणना xkb_keymap_format format)
{
    प्रत्यागम (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#परिभाषय XKB_MAX_LEDS ((xkb_led_सूचकांक_t) (परिमाण(xkb_led_आवरणम्_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#परिभाषय XKB_MAX_MODS ((xkb_mod_सूचकांक_t) (परिमाण(xkb_mod_आवरणम्_t) * CHAR_BIT))

गणना {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#परिभाषय XKB_MOD_NONE 0xffffffffU

गणना mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#परिभाषय MOD_REAL_आवरणम्_ALL ((xkb_mod_आवरणम्_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
गणना real_mod_सूचकांक {
    XKB_MOD_सूचकांक_सरणम् = 0,
    XKB_MOD_सूचकांक_CAPS,
    XKB_MOD_सूचकांक_CTRL,
    XKB_MOD_सूचकांक_MOD1,
    XKB_MOD_सूचकांक_MOD2,
    XKB_MOD_सूचकांक_MOD3,
    XKB_MOD_सूचकांक_MOD4,
    XKB_MOD_सूचकांक_MOD5,
    XKB_MOD_सूचकांक_अङ्कः_ENTRIES
};
static_assert(XKB_MOD_सूचकांक_अङ्कः_ENTRIES == 8, "Invalid X11 core modifiers");

गणना xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_समुच्चयः,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_तालकम्,
    ACTION_TYPE_गणः_समुच्चयः,
    ACTION_TYPE_गणः_LATCH,
    ACTION_TYPE_गणः_तालकम्,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_तालकम्,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_समुच्चयः,
    ACTION_TYPE_CTRL_तालकम्,
    ACTION_TYPE_REDIRECT_कुञ्चिका,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_वैयक्तिकम्,
    ACTION_TYPE_आन्तरिकम्, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_अङ्कः_ENTRIES
};

गणना xkb_action_flags {
    ACTION_तालकम्_CLEAR = (1 << 0),
    ACTION_LATCH_गन्तव्यम्_तालकम् = (1 << 1),
    ACTION_तालकम्_NO_तालकम् = (1 << 2),
    ACTION_तालकम्_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_तालकम्_ON_RELEASE = (1 << 10),
    ACTION_UNLOCK_ON_PRESS = (1 << 11),
    ACTION_LATCH_ON_PRESS = (1 << 12),
    ACTION_PENDING_COMPUTATION = (1 << 13),
    /**
     * Indicate that the action has an explicit `unlockOnPress` value.
     *
     * Used *during* keymap compilation.
     */
    ACTION_EXPLICIT_UNLOCK_ON_PRESS = (1 << 14),
    /**
     * Indicate that the action has an explicit `unlockOnPress` value.
     *
     * Used *during* keymap compilation.
     */
    ACTION_EXPLICIT_तालकम्_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_गणः_तालकम्_ON_MODIFIER_RELEASE = (1 << 16),
};

गणना {
    CONTROL_OVERLAY1_LOG2 = 23,
    CONTROL_OVERLAY2_LOG2,
    CONTROL_OVERLAY3_LOG2,
    CONTROL_OVERLAY4_LOG2,
    CONTROL_OVERLAY5_LOG2,
    CONTROL_OVERLAY6_LOG2,
    CONTROL_OVERLAY7_LOG2,
    CONTROL_OVERLAY8_LOG2,
    _MAX_CONTROL_OVERLAY_LOG2,
};

/**
 * This is the general version of the *public* `xkb_keyboard_control_flags` enum.
 * We do not expose the following enum, as it does not make sense to expose
 * controls whose effects we do not support.
 * However, we should enforce both enum to share the same values.
 *
 * @warning Encoding is different from X11.
 */
गणना xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_कुञ्चिकाः = (1 << 0),
    CONTROL_MOUSE_कुञ्चिकाः = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_कुञ्चिकाः_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_गणः_तालकम् = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    चरमः_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_कुञ्चिकाः | CONTROL_MOUSE_कुञ्चिकाः | CONTROL_MOUSE_कुञ्चिकाः_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_गणः_तालकम् | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_कुञ्चिकाः | CONTROL_MOUSE_कुञ्चिकाः | CONTROL_MOUSE_कुञ्चिकाः_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_गणः_तालकम्),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(परिमाण(गणना xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)चरमः_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

स्थिर अन्तःस्थ गणना xkb_action_controls
format_boolean_controls(गणना xkb_keymap_format format)
{
    प्रत्यागम (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_कुञ्चिकाः ==
    (गणना xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_कुञ्चिकाः ==
    (गणना xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः,
    "Private value should match public API"
);


गणना xkb_मेलनम्_operation {
    मेलनम्_NONE,
    मेलनम्_ANY_OR_NONE,
    मेलनम्_ANY,
    मेलनम्_ALL,
    मेलनम्_EXACTLY,
};

संरचना xkb_mods {
    xkb_mod_आवरणम्_t mods;       /* original real+virtual mods in definition */
    xkb_mod_आवरणम्_t आवरणम्;       /* computed effective mask */
};

संरचना xkb_mod_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    संरचना xkb_mods mods;
};

संरचना xkb_गणः_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int32_t गणः;
};

/** Keyboard overlay index or count */
प्रकारनिर्देश uint8_t xkb_overlay_सूचकांक_t;
/** Keyboard overlay mask */
प्रकारनिर्देश uint8_t xkb_overlay_आवरणम्_t;
गणना {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_गणनम्_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_गणनम् = (परिमाण(xkb_overlay_आवरणम्_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_गणनम् - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_सूचकांक_MIN_विस्तारः = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_गणनम्_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_गणनम्) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_गणनम् ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_गणनम् < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_गणनम् <= (1u << XKB_OVERLAY_सूचकांक_MIN_विस्तारः) - 1,
              "Cannot encode overlay index or count in a nibble");

स्थिर अन्तःस्थ xkb_overlay_आवरणम्_t
overlays_from_controls(गणना xkb_action_controls controls)
{
    प्रत्यागम (xkb_overlay_आवरणम्_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

स्थिर अन्तःस्थ गणना xkb_action_controls
overlays_गन्तव्यम्_controls(xkb_overlay_आवरणम्_t overlays)
{
    प्रत्यागम (गणना xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

स्थिर अन्तःस्थ xkb_overlay_सूचकांक_t
format_max_overlays(गणना xkb_keymap_format format)
{
    प्रत्यागम (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_गणनम्_X11
        : XKB_OVERLAY_गणनम्;
}

संरचना xkb_controls_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    गणना xkb_action_controls ctrls;
};

प्रकारनिर्देश uint8_t xkb_pointer_button_सूचकांक_t;
प्रकारनिर्देश uint8_t xkb_pointer_button_आवरणम्_t;

गणना {
    XKB_POINTER_BUTTON_सूचकांक_विस्तारः =
        परिमाण(xkb_pointer_button_सूचकांक_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_सूचकांक_MAX =
        (1 << XKB_POINTER_BUTTON_सूचकांक_विस्तारः) - 1,
    XKB_POINTER_BUTTON_आवरणम्_विस्तारः =
        परिमाण(xkb_pointer_button_आवरणम्_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_सूचकांक_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_आवरणम्_विस्तारः,
              "xkb_pointer_button_mask_t cannot store button mask");

संरचना xkb_pointer_default_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int8_t मानम्;
};

गणना {
    XKB_POINTER_DEFAULT_ACTION_मानम्_विस्तारः =
        परिमाण(((संरचना xkb_pointer_default_action*)0)->मानम्) * CHAR_BIT,
    XKB_POINTER_DEFAULT_ACTION_मानम्_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_मानम्_विस्तारः - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_मानम्_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

संरचना xkb_pointer_motion_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

संरचना xkb_pointer_button_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    uint8_t गणनम्;
    xkb_pointer_button_सूचकांक_t button;
};

संरचना xkb_switch_screen_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int8_t screen;
};

संरचना xkb_redirect_कुञ्चिका_action {
    गणना xkb_action_type type;
    xkb_keycode_t keycode;
    /*
     * Next 2 fields are only used for parsing and serializing.
     *
     * We do not use `struct xkb_mods` here, because *both* fields denote
     * *virtual* modifier indices, contrary to `xkb_mods`, where `mods` and
     * `mask` denote respectively virtual and real mods.
     *
     * Ideally we would use 2 `xkb_mods` structs, but this would increase the
     * `xkb_action` union type.
     */
    /** Affected virtual modifiers */
    xkb_mod_आवरणम्_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_आवरणम्_t mods;
};

संरचना xkb_वैयक्तिकम्_action {
    गणना xkb_action_type type;
    uint8_t दत्त[7];
};

गणना xkb_आन्तरिकम्_action_flags {
    आन्तरिकम्_BREAKS_गणः_LATCH = (1 << 0),
    आन्तरिकम्_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
संरचना xkb_आन्तरिकम्_action {
    गणना xkb_action_type type;
    गणना xkb_आन्तरिकम्_action_flags flags;
    सङ्घ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_आवरणम्_t clear_latched_mods;
    };
};

सङ्घ xkb_action {
    गणना xkb_action_type type;
    संरचना xkb_mod_action mods;
    संरचना xkb_गणः_action गणः;
    संरचना xkb_controls_action ctrls;
    संरचना xkb_redirect_कुञ्चिका_action redirect;
    संरचना xkb_pointer_default_action dflt;
    संरचना xkb_pointer_motion_action ptr;
    संरचना xkb_pointer_button_action btn;
    संरचना xkb_switch_screen_action screen;
    संरचना xkb_वैयक्तिकम्_action priv;
    संरचना xkb_आन्तरिकम्_action आन्तरिकम्;
};

संरचना xkb_कुञ्चिका_type_प्रविष्टिः {
    xkb_level_सूचकांक_t level;
    संरचना xkb_mods mods;
    संरचना xkb_mods preserve;
};

संरचना xkb_कुञ्चिका_type {
    xkb_atom_t name;
    संरचना xkb_mods mods;
    bool required;
    xkb_level_सूचकांक_t अङ्कः_स्तराः;
    xkb_level_सूचकांक_t अङ्कः_level_नामानि;
    xkb_atom_t *level_नामानि ATTR_COUNTED_BY(अङ्कः_level_नामानि);
    darray_आकार_t अङ्कः_entries;
    संरचना xkb_कुञ्चिका_type_प्रविष्टिः *entries ATTR_COUNTED_BY(अङ्कः_entries);
};

प्रकारनिर्देश uint16_t xkb_action_गणनम्_t;
#परिभाषय MAX_ACTIONS_प्रत्येकम्_LEVEL UINT16_MAX

संरचना xkb_sym_interpret {
    xkb_keysym_t sym;
    गणना xkb_मेलनम्_operation मेलनम्;
    xkb_mod_आवरणम्_t mods;
    xkb_mod_सूचकांक_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_गणनम्_t अङ्कः_actions;
    सङ्घ {
        /* num_actions <= 1 */
        सङ्घ xkb_action action;
        /* num_actions >  1 */
        सङ्घ xkb_action *actions;
    } a;
};

गणना {XKB_STATE_COMPONENT_विस्तारः = (परिमाण(गणना xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_विस्तारः) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

संरचना xkb_led {
    xkb_atom_t name;
    गणना xkb_state_component which_groups:(XKB_STATE_COMPONENT_विस्तारः - 1);
    bool pending_groups:1;
    xkb_layout_आवरणम्_t groups;
    गणना xkb_state_component which_mods;
    संरचना xkb_mods mods;
    गणना xkb_action_controls ctrls;
};

संरचना xkb_कुञ्चिका_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

संरचना xkb_controls {
    अचिह्नित वर्ण groups_wrap;
    संरचना xkb_mods आन्तरिकम्;
    संरचना xkb_mods ignore_तालकम्;
    अचिह्नित ह्रस्व repeat_delay;
    अचिह्नित ह्रस्व repeat_interval;
    अचिह्नित ह्रस्व slow_कुञ्चिकाः_delay;
    अचिह्नित ह्रस्व debounce_delay;
    अचिह्नित ह्रस्व ax_options;
    अचिह्नित ह्रस्व ax_timeout;
    अचिह्नित ह्रस्व axt_opts_आवरणम्;
    अचिह्नित ह्रस्व axt_opts_मानानि;
    अचिह्नित पूर्णाङ्क axt_ctrls_आवरणम्;
    अचिह्नित पूर्णाङ्क axt_ctrls_मानानि;
};

गणना xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

प्रकारनिर्देश uint16_t xkb_keysym_गणनम्_t;
#परिभाषय MAX_KEYSYMS_प्रत्येकम्_LEVEL UINT16_MAX

/** A key level */
संरचना xkb_level {
    /** Count of keysyms */
    xkb_keysym_गणनम्_t अङ्कः_syms;
    /** Count of actions */
    xkb_action_गणनम्_t अङ्कः_actions;
    /** Upper keysym */
    सङ्घ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t ऊर्ध्वस्थम्;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_ऊर्ध्वस्थम्;
    };
    /** Keysyms */
    सङ्घ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    सङ्घ {
        सङ्घ xkb_action action;   /* num_actions == 1 */
        सङ्घ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
संरचना xkb_गणः {
    /** Flag that indicates whether a group has some *explicit* symbols */
    bool explicit_symbols:1;
    /**
     * Flag that indicates whether a group has explicit actions. In case it has,
     * compatibility interpretations will not be used on it.
     * See also EXPLICIT_INTERP flag at key level.
     */
    bool explicit_actions:1;
    /** Flag that indicates whether a group has some *implicit* actions */
    bool implicit_actions:1;
    /**
     * Flag that indicates whether a group has an explicit key type. In case it
     * has, type detection will not be used on it.
     */
    bool explicit_type:1;
    /**
     * Key type of the group. Points to an entry in keymap->types.
     */
    ध्रुव संरचना xkb_कुञ्चिका_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    संरचना xkb_level *स्तराः;
};

गणना {
    DEFAULT_कुञ्चिका_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_कुञ्चिका_REPEAT = true,
    FALLBACK_INTERPRET_कुञ्चिका_REPEAT = DEFAULT_कुञ्चिका_REPEAT
};

गणना {
    DEFAULT_कुञ्चिका_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_कुञ्चिका_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_कुञ्चिका_VMODMAP
};

संरचना xkb_कुञ्चिका {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    गणना xkb_explicit_components explicit;

    xkb_mod_आवरणम्_t modmap;
    xkb_mod_आवरणम्_t vmodmap;

    xkb_overlay_आवरणम्_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool बहिर्गमनम्_सम्बन्धि_range_pending_गणः:1;
    गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy बहिर्गमनम्_सम्बन्धि_range_गणः_policy:4;
    xkb_layout_सूचकांक_t बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या:8;

    xkb_layout_सूचकांक_t अङ्कः_groups:8;
    संरचना xkb_गणः *groups ATTR_COUNTED_BY(अङ्कः_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    सङ्घ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ध्रुव संरचना xkb_कुञ्चिका *overlay_कुञ्चिका;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः;
    };
};

संरचना xkb_mod {
    xkb_atom_t name;
    गणना mod_type type;
    xkb_mod_आवरणम्_t mapping; /* vmod -> real mod mapping */
};

संरचना xkb_mod_समुच्चयः {
    संरचना xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_सूचकांक_t अङ्कः_mods;
    xkb_mod_आवरणम्_t explicit_vmods;
};

/*
 * Implementation with continuous arrays does not allow efficient mapping of
 * sparse keycodes. Indeed, allowing the API max valid keycode XKB_KEYCODE_MAX
 * could result in memory exhaustion or memory waste (sparse arrays) with huge
 * enough valid values. However usual keycodes form a contiguous range with a
 * that maximum that rarely exceeds 1000. In order to handle the whole keycode
 * range, we define a threshold for the continuous array storage, above which we
 * use a more space-efficient storage for sparse arrays.
 *
 * The current limit is arbitrary and serves as a minimal security.
 */
#परिभाषय XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(परिमाण(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(परिमाण(संरचना xkb_कुञ्चिका)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#परिभाषय XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(परिमाण(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(परिमाण(संरचना xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_आकार_T_विस्तारः - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
प्रकारनिर्देश सङ्घ {
    संरचना {
        /** If true, the index is valid */
        bool प्राप्तम्:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_आकार_t:(DARRAY_आकार_T_विस्तारः - 3);
    };
    /* Only if is_alias = false, for better semantics */
    संरचना {
        bool प्राप्तम्:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool नीचः:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_आकार_t सूचकांक:(DARRAY_आकार_T_विस्तारः - 3);
    } कुञ्चिका;
    /* Only if is_alias = true, for better semantics */
    संरचना {
        bool प्राप्तम्:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_आकार_T_विस्तारः - 3);
    } alias;
} Keycode_मेलनम्;

प्रकारनिर्देश संरचना {
    सङ्घ {
        /** Used for parsing */
        गणना merge_बहुलकम् merge;
        /** Used for serializing */
        xkb_keycode_t कुञ्चिका_संहिता;
    };
    bool have_चिह्नम्;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_आवरणम्_t mods;
    सङ्घ {
        xkb_keysym_t कुञ्चिका_Sym;
        xkb_atom_t कुञ्चिका_Name;
    } u;
} Mod_Map_प्रविष्टिः;

/** Common keyboard description structure */
संरचना xkb_keymap {
    संरचना xkb_context *ctx;

    पूर्णाङ्क refcnt;
    गणना xkb_keymap_compile_flags flags;
    गणना xkb_keymap_format format;

    xkb_led_सूचकांक_t अङ्कः_leds;
    संरचना xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_कुञ्चिका_संहिता;
    xkb_keycode_t max_कुञ्चिका_संहिता;
    xkb_keycode_t अङ्कः_कुञ्चिकाः;
    /*
     * Keys are divided into 2 ordered (possibly empty) lists:
     *
     *   Low keycodes (≤ XKB_KEYCODE_MAX_CONTIGUOUS)
     *     Stored contiguously at indexes [0..num_keys_low).
     *     Fast O(1) access.
     *   High keycodes (> XKB_KEYCODE_MAX_CONTIGUOUS)
     *     Stored noncontiguously at indexes [num_keys_low..num_keys).
     *     Slow access via a binary search.
     */
    xkb_keycode_t अङ्कः_कुञ्चिकाः_नीचः;
    संरचना xkb_कुञ्चिका *कुञ्चिकाः ATTR_COUNTED_BY(अङ्कः_कुञ्चिकाः);

    सङ्घ {
        /**
         * Keycode name atom -> key index lookup table
         *
         * ⚠️ Used only *during* compilation
         *
         * Given that:
         * - the number of atoms is usually < 1k;
         * - the keycode section usually appears first;
         * then the first atoms will be mostly the keycodes and their aliases,
         * so we can achieve O(1) lookup of the key names by using a simple array.
         */
        संरचना {
            darray_आकार_t अङ्कः_कुञ्चिका_नामानि;
            Keycode_मेलनम् *कुञ्चिका_नामानि;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        संरचना {
            darray_आकार_t अङ्कः_कुञ्चिका_aliases;
            संरचना xkb_कुञ्चिका_alias *कुञ्चिका_aliases;
        };
    };

    संरचना xkb_कुञ्चिका_type *types;
    darray_आकार_t अङ्कः_types;

    darray_आकार_t अङ्कः_sym_interprets;
    संरचना xkb_sym_interpret *sym_interprets;

    darray_आकार_t _padding;
    darray_आकार_t अङ्कः_modmaps;
    Mod_Map_प्रविष्टिः *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    संरचना xkb_mod_समुच्चयः mods;
    /**
     * Modifier mask of the *canonical* state, i.e. the mask with the *smallest*
     * population count that denotes all bits used to encode the modifiers in
     * the keyboard state. It is equal to the bitwise OR of *real* modifiers and
     * all *virtual* modifiers mappings.
     *
     * [WARNING] The bits that do not correspond to *real* modifiers should
     * *not* be interpreted as corresponding to indices of virtual modifiers of
     * the keymap. Indeed, one may use explicit vmod mapping with an arbitrary
     * value.
     *
     * E.g. if M1 is the only vmod and it is defined by:
     *
     *     virtual_modifiers M1=0x80000000; // 1 << (32 - 1)
     *
     * then the 32th bit of a modifier mask input does *not* denote the 32th
     * virtual modifier of the keymap, but merely the encoding of the mapping of
     * M1.
     *
     * In the API, any input mask should be preprocessed to resolve the bits
     * that do not match the canonical mask.
     */
    xkb_mod_आवरणम्_t canonical_state_आवरणम्;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_कुञ्चिका_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_सूचकांक_t अङ्कः_groups;
    /* Not all groups must have names. */
    xkb_layout_सूचकांक_t अङ्कः_गणः_नामानि;
    xkb_atom_t *गणः_नामानि ATTR_COUNTED_BY(अङ्कः_गणः_नामानि);

    वर्ण *keycodes_section_name;
    वर्ण *symbols_section_name;
    वर्ण *types_section_name;
    वर्ण *compat_section_name;
};

#परिभाषय xkb_कुञ्चिकाः_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    प्रति ((iter) = (keymap)->कुञ्चिकाः + \
                  ((keymap)->अङ्कः_कुञ्चिकाः_नीचः == 0 ? 0 : (keymap)->min_कुञ्चिका_संहिता); \
         (iter) < (keymap)->कुञ्चिकाः + (keymap)->अङ्कः_कुञ्चिकाः; \
         (iter)++)

#परिभाषय xkb_level_foreach_action(iter, level)                 \
    प्रति (सङ्घ xkb_action                                     \
         *(iter) = (likely((level)->अङ्कः_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ध्रुव iter##_last_ = (iter) + (level)->अङ्कः_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#परिभाषय xkb_mods_foreach(iter, mods_) \
    प्रति ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->अङ्कः_mods; \
         (iter)++)

#परिभाषय xkb_mods_आवरणम्_foreach(आवरणम्, iter, mods_) \
    प्रति ((iter) = (mods_)->mods; \
         (आवरणम्) && (iter) < (mods_)->mods + (mods_)->अङ्कः_mods; \
         (iter)++, (आवरणम्) >>= 1) \
        यदि ((आवरणम्) & 0x1)

/** Enumerate all modifiers */
#परिभाषय xkb_mods_enumerate(idx, iter, mods_) \
    प्रति ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->अङ्कः_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#परिभाषय xkb_rmods_enumerate(idx, iter, mods_) \
    प्रति ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_सूचकांक_अङ्कः_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#परिभाषय xkb_vmods_enumerate(idx, iter, mods_) \
    प्रति ((idx) = XKB_MOD_सूचकांक_अङ्कः_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_सूचकांक_अङ्कः_ENTRIES]; \
         (idx) < (mods_)->अङ्कः_mods; \
         (idx)++, (iter)++)

#परिभाषय xkb_leds_foreach(iter, keymap) \
    प्रति ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->अङ्कः_leds; \
         (iter)++)

#परिभाषय xkb_leds_enumerate(idx, iter, keymap) \
    प्रति ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->अङ्कः_leds; \
         (idx)++, (iter)++)

शून्य
clear_level(संरचना xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
स्थिर अन्तःस्थ संरचना xkb_कुञ्चिका *
Xkb_कुञ्चिका_By_Name(ध्रुव संरचना xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    यदि (name < keymap->अङ्कः_कुञ्चिका_नामानि) {
        ध्रुव Keycode_मेलनम् मेलनम् = keymap->कुञ्चिका_नामानि[name];
        यदि (मेलनम्.प्राप्तम्) {
            यदि (!मेलनम्.is_alias) {
                प्रतिज्ञा(name == keymap->कुञ्चिकाः[मेलनम्.कुञ्चिका.सूचकांक].name);
                प्रत्यागम &keymap->कुञ्चिकाः[मेलनम्.कुञ्चिका.सूचकांक];
            } अन्यथा यदि (use_aliases) {
                प्रतिज्ञा(मेलनम्.alias.real ==
                       keymap->कुञ्चिकाः[keymap->कुञ्चिका_नामानि[मेलनम्.alias.real].कुञ्चिका.सूचकांक].name);
                प्रत्यागम &keymap->कुञ्चिकाः[keymap->कुञ्चिका_नामानि[मेलनम्.alias.real].कुञ्चिका.सूचकांक];
            }
        }
    }
    प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ ध्रुव संरचना xkb_कुञ्चिका *
Xkb_कुञ्चिका(ध्रुव संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    यदि (kc < keymap->min_कुञ्चिका_संहिता || kc > keymap->max_कुञ्चिका_संहिता) {
        /* Unsupported keycode */
        प्रत्यागम रिक्त;
    } अन्यथा यदि (kc < keymap->अङ्कः_कुञ्चिकाः_नीचः) {
        /* Low keycodes */
        प्रत्यागम &keymap->कुञ्चिकाः[kc];
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t अधस्तनम् = keymap->अङ्कः_कुञ्चिकाः_नीचः;
        xkb_keycode_t ऊर्ध्वस्थम् = keymap->अङ्कः_कुञ्चिकाः;
        यावत् (अधस्तनम् < ऊर्ध्वस्थम्) {
            ध्रुव xkb_keycode_t mid = अधस्तनम् + (ऊर्ध्वस्थम् - 1 - अधस्तनम्) / 2;
            ध्रुव संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिका = &keymap->कुञ्चिकाः[mid];
            यदि (कुञ्चिका->keycode < kc) {
                अधस्तनम् = mid + 1;
            } अन्यथा यदि (कुञ्चिका->keycode > kc) {
                ऊर्ध्वस्थम् = mid;
            } अन्यथा {
                प्रत्यागम कुञ्चिका;
            }
        }
        प्रत्यागम रिक्त;
    }
}

/**
 * Returns the the next *defined* key (i.e. with a name) in the given
 * direction, starting from kc (inclusive). I.e. the key with either:
 * - the smallest keycode >= kc if ascending = true, or
 * - the largest keycode <= kc if ascending = false.
 *
 * Returns NULL if no such key exists.
 */
स्थिर अन्तःस्थ ध्रुव संरचना xkb_कुञ्चिका *
xkb_keymap_get_अग्र_defined_कुञ्चिका(ध्रुव संरचना xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    यदि (!keymap->अङ्कः_कुञ्चिकाः || kc > XKB_KEYCODE_MAX) {
        प्रत्यागम रिक्त;
    } अन्यथा यदि (kc < keymap->min_कुञ्चिका_संहिता) {
        kc = keymap->min_कुञ्चिका_संहिता;
    } अन्यथा यदि (kc > keymap->max_कुञ्चिका_संहिता) {
        kc = keymap->max_कुञ्चिका_संहिता;
    }

    यदि (kc < keymap->अङ्कः_कुञ्चिकाः_नीचः) {
        /* Low keycodes */
        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = &keymap->कुञ्चिकाः[kc];
        यदि (ascending) {
            ध्रुव संरचना xkb_कुञ्चिका *अन्तः = &keymap->कुञ्चिकाः[keymap->अङ्कः_कुञ्चिकाः_नीचः - 1];
            यावत् (कुञ्चिका->name == XKB_ATOM_NONE && कुञ्चिका < अन्तः) कुञ्चिका++;
        } अन्यथा {
            ध्रुव संरचना xkb_कुञ्चिका *अन्तः = &keymap->कुञ्चिकाः[keymap->min_कुञ्चिका_संहिता];
            यावत् (कुञ्चिका->name == XKB_ATOM_NONE && कुञ्चिका > अन्तः) कुञ्चिका--;
        }
        प्रत्यागम (कुञ्चिका->name == XKB_ATOM_NONE) ? रिक्त : कुञ्चिका;
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t अधस्तनम् = keymap->अङ्कः_कुञ्चिकाः_नीचः
            ? keymap->अङ्कः_कुञ्चिकाः_नीचः - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t ऊर्ध्वस्थम् = keymap->अङ्कः_कुञ्चिकाः - 1;
        ध्रुव संरचना xkb_कुञ्चिका * candidate = रिक्त;
        यावत् (अधस्तनम् <= ऊर्ध्वस्थम्) {
            ध्रुव xkb_keycode_t mid = अधस्तनम् + (ऊर्ध्वस्थम् - अधस्तनम्) / 2;
            ध्रुव संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिका = &keymap->कुञ्चिकाः[mid];
            यदि (कुञ्चिका->keycode == kc) {
                प्रत्यागम कुञ्चिका;
            } अन्यथा यदि (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                यदि (कुञ्चिका->keycode >= kc) {
                    candidate = कुञ्चिका;
                    ऊर्ध्वस्थम् = mid - 1;
                } अन्यथा {
                    अधस्तनम् = mid + 1;
                }
            } अन्यथा {
                /* Looking for the *smallest* defined keycode <= kc */
                यदि (कुञ्चिका->keycode <= kc) {
                    candidate = कुञ्चिका;
                    अधस्तनम् = mid + 1;
                } अन्यथा यदि (कुञ्चिका->keycode > kc) {
                    ऊर्ध्वस्थम् = mid - 1;
                }
            }
        }
        प्रत्यागम candidate;
    }
}

स्थिर अन्तःस्थ xkb_level_सूचकांक_t
Xkb_कुञ्चिका_अङ्कः_स्तराः(ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका, xkb_layout_सूचकांक_t layout)
{
    प्रत्यागम कुञ्चिका->groups[layout].type->अङ्कः_स्तराः;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
स्थिर अन्तःस्थ bool
प्रविष्टिः_is_सक्रिय(ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः)
{
    प्रत्यागम प्रविष्टिः->mods.mods == 0 || प्रविष्टिः->mods.आवरणम् != 0;
}

संरचना xkb_keymap *
xkb_keymap_new(संरचना xkb_context *ctx, ध्रुव वर्ण* func,
               गणना xkb_keymap_format format,
               गणना xkb_keymap_compile_flags flags);

शून्य
XkbEscapeMapName(वर्ण *name);

xkb_mod_सूचकांक_t
Xkb_Mod_Name_गन्तव्यम्_सूचकांक(ध्रुव संरचना xkb_mod_समुच्चयः *mods, xkb_atom_t name,
                  गणना mod_type type);

bool
Xkb_स्तराः_Same_Syms(ध्रुव संरचना xkb_level *a, ध्रुव संरचना xkb_level *b);

bool
action_equal(ध्रुव सङ्घ xkb_action *a, ध्रुव सङ्घ xkb_action *b);

bool
Xkb_स्तराः_Same_Actions(ध्रुव संरचना xkb_level *a, ध्रुव संरचना xkb_level *b);

xkb_layout_सूचकांक_t
Xkb_Wrap_गणः_Into_Range(int32_t गणः,
                      xkb_layout_सूचकांक_t अङ्कः_groups,
                      गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy बहिर्गमनम्_सम्बन्धि_range_गणः_policy,
                      xkb_layout_सूचकांक_t बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या);

स्थिर अन्तःस्थ xkb_layout_सूचकांक_t
xkb_keymap_कुञ्चिका_effective_layout(ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                xkb_layout_सूचकांक_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    प्रत्यागम Xkb_Wrap_गणः_Into_Range((int32_t) layout,
                                 कुञ्चिका->अङ्कः_groups,
                                 कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy,
                                 कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या);
}

XKB_EXPORT_वैयक्तिकम् xkb_mod_आवरणम्_t
mod_आवरणम्_get_effective(संरचना xkb_keymap *keymap, xkb_mod_आवरणम्_t mods);

संरचना xkb_level *
xkb_keymap_कुञ्चिका_get_level(संरचना xkb_keymap *keymap, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         xkb_layout_सूचकांक_t layout, xkb_level_सूचकांक_t level);

xkb_action_गणनम्_t
xkb_keymap_कुञ्चिका_get_actions_by_level(ध्रुव संरचना xkb_keymap *keymap,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    xkb_layout_सूचकांक_t layout,
                                    xkb_level_सूचकांक_t level,
                                    ध्रुव सङ्घ xkb_action **actions);

संरचना xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(संरचना xkb_keymap *keymap,
                                  ध्रुव संरचना xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_नामानि)(संरचना xkb_keymap *keymap,
                                  ध्रुव संरचना xkb_नियमः_नामानि *नामानि);
    bool (*keymap_new_from_string)(संरचना xkb_keymap *keymap,
                                   ध्रुव वर्ण *string, आकार_t दीर्घता);
    bool (*keymap_new_from_file)(संरचना xkb_keymap *keymap, FILE *file);
    गणना xkb_स्थितिः (*keymap_serialize)(
        ध्रुव संरचना xkb_keymap *keymap,
        ध्रुव संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_परिणाम *परिणाम
    );
};

बाह्य ध्रुव संरचना xkb_keymap_format_ops text_v1_keymap_format_ops;

स्थिर अन्तःस्थ bool
is_Mods_Un_तालकम्_On_Press_Supported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    प्रत्यागम format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर अन्तःस्थ bool
is_गणः_तालकम्_On_Release_Supported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    प्रत्यागम format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर अन्तःस्थ bool
isModsLatchOnPressSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    प्रत्यागम format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर अन्तःस्थ bool
areOverlappingOverlaysSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    प्रत्यागम format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
