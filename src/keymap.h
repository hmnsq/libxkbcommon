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
#ಸೂಚನೆ once

 /* Don't use compat names in internal code. */
#ವ್ಯಾಖ್ಯಾನಿಸು _XKBCOMMON_COMPAT_H

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MAX_GROUPS 32
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MAX_GROUPS_X11 4

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_layout_ಸೂಚ್ಯಂಕ_t
format_max_groups(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MAX_LEDS ((xkb_led_ಸೂಚ್ಯಂಕ_t) (ಗಾತ್ರ(xkb_led_ಮುಖವಾಡ_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MAX_MODS ((xkb_mod_ಸೂಚ್ಯಂಕ_t) (ಗಾತ್ರ(xkb_mod_ಮುಖವಾಡ_t) * CHAR_BIT))

ಎಣಿಕೆ {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MOD_NONE 0xffffffffU

ಎಣಿಕೆ mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#ವ್ಯಾಖ್ಯಾನಿಸು MOD_REAL_ಮುಖವಾಡ_ALL ((xkb_mod_ಮುಖವಾಡ_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
ಎಣಿಕೆ real_mod_ಸೂಚ್ಯಂಕ {
    XKB_MOD_ಸೂಚ್ಯಂಕ_ಸರಿಸು = 0,
    XKB_MOD_ಸೂಚ್ಯಂಕ_CAPS,
    XKB_MOD_ಸೂಚ್ಯಂಕ_CTRL,
    XKB_MOD_ಸೂಚ್ಯಂಕ_MOD1,
    XKB_MOD_ಸೂಚ್ಯಂಕ_MOD2,
    XKB_MOD_ಸೂಚ್ಯಂಕ_MOD3,
    XKB_MOD_ಸೂಚ್ಯಂಕ_MOD4,
    XKB_MOD_ಸೂಚ್ಯಂಕ_MOD5,
    XKB_MOD_ಸೂಚ್ಯಂಕ_ಅಂಕೆ_ENTRIES
};
static_assert(XKB_MOD_ಸೂಚ್ಯಂಕ_ಅಂಕೆ_ENTRIES == 8, "Invalid X11 core modifiers");

ಎಣಿಕೆ xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_ಗಣ,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_ಬೀಗ,
    ACTION_TYPE_ಗುಂಪು_ಗಣ,
    ACTION_TYPE_ಗುಂಪು_LATCH,
    ACTION_TYPE_ಗುಂಪು_ಬೀಗ,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_ಬೀಗ,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_ಗಣ,
    ACTION_TYPE_CTRL_ಬೀಗ,
    ACTION_TYPE_REDIRECT_ಕೀಲಿ,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_ಖಾಸಗಿ,
    ACTION_TYPE_ಆಂತರಿಕ, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_ಅಂಕೆ_ENTRIES
};

ಎಣಿಕೆ xkb_action_flags {
    ACTION_ಬೀಗ_CLEAR = (1 << 0),
    ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ = (1 << 1),
    ACTION_ಬೀಗ_NO_ಬೀಗ = (1 << 2),
    ACTION_ಬೀಗ_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_ಬೀಗ_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_ಬೀಗ_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_ಗುಂಪು_ಬೀಗ_ON_MODIFIER_RELEASE = (1 << 16),
};

ಎಣಿಕೆ {
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
ಎಣಿಕೆ xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_ಕೀಲಿಗಳು = (1 << 0),
    CONTROL_MOUSE_ಕೀಲಿಗಳು = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_ಗುಂಪು_ಬೀಗ = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    ಕೊನೆಯ_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_ಗುಂಪು_ಬೀಗ | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_ಗುಂಪು_ಬೀಗ),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(ಗಾತ್ರ(ಎಣಿಕೆ xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)ಕೊನೆಯ_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_action_controls
format_boolean_controls(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_ಕೀಲಿಗಳು ==
    (ಎಣಿಕೆ xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_ಕೀಲಿಗಳು ==
    (ಎಣಿಕೆ xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು,
    "Private value should match public API"
);


ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation {
    ಹೊಂದಿಕೆ_NONE,
    ಹೊಂದಿಕೆ_ANY_OR_NONE,
    ಹೊಂದಿಕೆ_ANY,
    ಹೊಂದಿಕೆ_ALL,
    ಹೊಂದಿಕೆ_EXACTLY,
};

ರಚನೆ xkb_mods {
    xkb_mod_ಮುಖವಾಡ_t mods;       /* original real+virtual mods in definition */
    xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ;       /* computed effective mask */
};

ರಚನೆ xkb_mod_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    ರಚನೆ xkb_mods mods;
};

ರಚನೆ xkb_ಗುಂಪು_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    int32_t ಗುಂಪು;
};

/** Keyboard overlay index or count */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t xkb_overlay_ಸೂಚ್ಯಂಕ_t;
/** Keyboard overlay mask */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t xkb_overlay_ಮುಖವಾಡ_t;
ಎಣಿಕೆ {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ಎಣಿಕೆ_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_ಎಣಿಕೆ = (ಗಾತ್ರ(xkb_overlay_ಮುಖವಾಡ_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_ಎಣಿಕೆ - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_ಸೂಚ್ಯಂಕ_MIN_ಅಗಲ = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_ಎಣಿಕೆ_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_ಎಣಿಕೆ) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_ಎಣಿಕೆ ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_ಎಣಿಕೆ < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_ಎಣಿಕೆ <= (1u << XKB_OVERLAY_ಸೂಚ್ಯಂಕ_MIN_ಅಗಲ) - 1,
              "Cannot encode overlay index or count in a nibble");

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_overlay_ಮುಖವಾಡ_t
overlays_from_controls(ಎಣಿಕೆ xkb_action_controls controls)
{
    ಹಿಂತಿರುಗಿ (xkb_overlay_ಮುಖವಾಡ_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_action_controls
overlays_ಗಮ್ಯ_controls(xkb_overlay_ಮುಖವಾಡ_t overlays)
{
    ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_overlay_ಸೂಚ್ಯಂಕ_t
format_max_overlays(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
        ? XKB_OVERLAY_ಎಣಿಕೆ_X11
        : XKB_OVERLAY_ಎಣಿಕೆ;
}

ರಚನೆ xkb_controls_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    ಎಣಿಕೆ xkb_action_controls ctrls;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t xkb_ಸೂಚಕ_button_ಮುಖವಾಡ_t;

ಎಣಿಕೆ {
    XKB_ಸೂಚಕ_BUTTON_ಸೂಚ್ಯಂಕ_ಅಗಲ =
        ಗಾತ್ರ(xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t) * CHAR_BIT,
    XKB_ಸೂಚಕ_BUTTON_ಸೂಚ್ಯಂಕ_MAX =
        (1 << XKB_ಸೂಚಕ_BUTTON_ಸೂಚ್ಯಂಕ_ಅಗಲ) - 1,
    XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ =
        ಗಾತ್ರ(xkb_ಸೂಚಕ_button_ಮುಖವಾಡ_t) * CHAR_BIT,
    XKB_ಸೂಚಕ_BUTTON_DEFAULT = 0,
    XKB_ಸೂಚಕ_BUTTON_MIN = 1,
    XKB_ಸೂಚಕ_BUTTON_MAX = 5,
};

static_assert(XKB_ಸೂಚಕ_BUTTON_MAX <= XKB_ಸೂಚಕ_BUTTON_ಸೂಚ್ಯಂಕ_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_ಸೂಚಕ_BUTTON_MAX <= XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ,
              "xkb_pointer_button_mask_t cannot store button mask");

ರಚನೆ xkb_ಸೂಚಕ_default_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    int8_t ಮೌಲ್ಯ;
};

ಎಣಿಕೆ {
    XKB_ಸೂಚಕ_DEFAULT_ACTION_ಮೌಲ್ಯ_ಅಗಲ =
        ಗಾತ್ರ(((ರಚನೆ xkb_ಸೂಚಕ_default_action*)0)->ಮೌಲ್ಯ) * CHAR_BIT,
    XKB_ಸೂಚಕ_DEFAULT_ACTION_ಮೌಲ್ಯ_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_ಸೂಚಕ_DEFAULT_ACTION_ಮೌಲ್ಯ_ಅಗಲ - 1)) - 1,
};

static_assert((uint32_t)XKB_ಸೂಚಕ_BUTTON_MAX <=
              (uint32_t)XKB_ಸೂಚಕ_DEFAULT_ACTION_ಮೌಲ್ಯ_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

ರಚನೆ xkb_ಸೂಚಕ_motion_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

ರಚನೆ xkb_ಸೂಚಕ_button_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    uint8_t ಎಣಿಕೆ_2;
    xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t button;
};

ರಚನೆ xkb_switch_screen_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_action_flags flags;
    int8_t screen;
};

ರಚನೆ xkb_redirect_ಕೀಲಿ_action {
    ಎಣಿಕೆ xkb_action_type type;
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
    xkb_mod_ಮುಖವಾಡ_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_ಮುಖವಾಡ_t mods;
};

ರಚನೆ xkb_ಖಾಸಗಿ_action {
    ಎಣಿಕೆ xkb_action_type type;
    uint8_t ದತ್ತಾಂಶ[7];
};

ಎಣಿಕೆ xkb_ಆಂತರಿಕ_action_flags {
    ಆಂತರಿಕ_BREAKS_ಗುಂಪು_LATCH = (1 << 0),
    ಆಂತರಿಕ_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
ರಚನೆ xkb_ಆಂತರಿಕ_action {
    ಎಣಿಕೆ xkb_action_type type;
    ಎಣಿಕೆ xkb_ಆಂತರಿಕ_action_flags flags;
    ಒಕ್ಕೂಟ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_ಮುಖವಾಡ_t clear_latched_mods;
    };
};

ಒಕ್ಕೂಟ xkb_action {
    ಎಣಿಕೆ xkb_action_type type;
    ರಚನೆ xkb_mod_action mods;
    ರಚನೆ xkb_ಗುಂಪು_action ಗುಂಪು;
    ರಚನೆ xkb_controls_action ctrls;
    ರಚನೆ xkb_redirect_ಕೀಲಿ_action redirect;
    ರಚನೆ xkb_ಸೂಚಕ_default_action dflt;
    ರಚನೆ xkb_ಸೂಚಕ_motion_action ptr;
    ರಚನೆ xkb_ಸೂಚಕ_button_action btn;
    ರಚನೆ xkb_switch_screen_action screen;
    ರಚನೆ xkb_ಖಾಸಗಿ_action priv;
    ರಚನೆ xkb_ಆಂತರಿಕ_action ಆಂತರಿಕ;
};

ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು {
    xkb_level_ಸೂಚ್ಯಂಕ_t level;
    ರಚನೆ xkb_mods mods;
    ರಚನೆ xkb_mods preserve;
};

ರಚನೆ xkb_ಕೀಲಿ_type {
    xkb_atom_t name;
    ರಚನೆ xkb_mods mods;
    bool required;
    xkb_level_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_ಮಟ್ಟಗಳು;
    xkb_level_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_level_ಹೆಸರುಗಳು;
    xkb_atom_t *level_ಹೆಸರುಗಳು ATTR_COUNTED_BY(ಅಂಕೆ_level_ಹೆಸರುಗಳು);
    darray_ಗಾತ್ರ_t ಅಂಕೆ_entries;
    ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *entries ATTR_COUNTED_BY(ಅಂಕೆ_entries);
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint16_t xkb_action_ಎಣಿಕೆ_t;
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ACTIONS_ಪ್ರತಿ_LEVEL UINT16_MAX

ರಚನೆ xkb_sym_interpret {
    xkb_keysym_t sym;
    ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation ಹೊಂದಿಕೆ;
    xkb_mod_ಮುಖವಾಡ_t mods;
    xkb_mod_ಸೂಚ್ಯಂಕ_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_ಎಣಿಕೆ_t ಅಂಕೆ_actions;
    ಒಕ್ಕೂಟ {
        /* num_actions <= 1 */
        ಒಕ್ಕೂಟ xkb_action action;
        /* num_actions >  1 */
        ಒಕ್ಕೂಟ xkb_action *actions;
    } a;
};

ಎಣಿಕೆ {XKB_STATE_COMPONENT_ಅಗಲ = (ಗಾತ್ರ(ಎಣಿಕೆ xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_ಅಗಲ) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

ರಚನೆ xkb_led {
    xkb_atom_t name;
    ಎಣಿಕೆ xkb_state_component which_groups:(XKB_STATE_COMPONENT_ಅಗಲ - 1);
    bool pending_groups:1;
    xkb_layout_ಮುಖವಾಡ_t groups;
    ಎಣಿಕೆ xkb_state_component which_mods;
    ರಚನೆ xkb_mods mods;
    ಎಣಿಕೆ xkb_action_controls ctrls;
};

ರಚನೆ xkb_ಕೀಲಿ_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

ರಚನೆ xkb_controls {
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ groups_wrap;
    ರಚನೆ xkb_mods ಆಂತರಿಕ;
    ರಚನೆ xkb_mods ignore_ಬೀಗ;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ repeat_delay;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ repeat_interval;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ slow_ಕೀಲಿಗಳು_delay;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ debounce_delay;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ax_options;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ax_timeout;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ axt_opts_ಮುಖವಾಡ;
    ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ axt_opts_ಮೌಲ್ಯಗಳು;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ axt_ctrls_ಮುಖವಾಡ;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ axt_ctrls_ಮೌಲ್ಯಗಳು;
};

ಎಣಿಕೆ xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint16_t xkb_keysym_ಎಣಿಕೆ_t;
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_KEYSYMS_ಪ್ರತಿ_LEVEL UINT16_MAX

/** A key level */
ರಚನೆ xkb_level {
    /** Count of keysyms */
    xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms;
    /** Count of actions */
    xkb_action_ಎಣಿಕೆ_t ಅಂಕೆ_actions;
    /** Upper keysym */
    ಒಕ್ಕೂಟ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t ಊರ್ಧ್ವ;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_ಊರ್ಧ್ವ;
    };
    /** Keysyms */
    ಒಕ್ಕೂಟ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    ಒಕ್ಕೂಟ {
        ಒಕ್ಕೂಟ xkb_action action;   /* num_actions == 1 */
        ಒಕ್ಕೂಟ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
ರಚನೆ xkb_ಗುಂಪು {
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
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    ರಚನೆ xkb_level *ಮಟ್ಟಗಳು;
};

ಎಣಿಕೆ {
    DEFAULT_ಕೀಲಿ_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT = true,
    FALLBACK_INTERPRET_ಕೀಲಿ_REPEAT = DEFAULT_ಕೀಲಿ_REPEAT
};

ಎಣಿಕೆ {
    DEFAULT_ಕೀಲಿ_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_ಕೀಲಿ_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_ಕೀಲಿ_VMODMAP
};

ರಚನೆ xkb_ಕೀಲಿ {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    ಎಣಿಕೆ xkb_explicit_components explicit;

    xkb_mod_ಮುಖವಾಡ_t modmap;
    xkb_mod_ಮುಖವಾಡ_t vmodmap;

    xkb_overlay_ಮುಖವಾಡ_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು:1;
    ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy:4;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ:8;

    xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups:8;
    ರಚನೆ xkb_ಗುಂಪು *groups ATTR_COUNTED_BY(ಅಂಕೆ_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ಒಕ್ಕೂಟ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು;
    };
};

ರಚನೆ xkb_mod {
    xkb_atom_t name;
    ಎಣಿಕೆ mod_type type;
    xkb_mod_ಮುಖವಾಡ_t mapping; /* vmod -> real mod mapping */
};

ರಚನೆ xkb_mod_ಗಣ {
    ರಚನೆ xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_mods;
    xkb_mod_ಮುಖವಾಡ_t explicit_vmods;
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
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ಗಾತ್ರ(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ಗಾತ್ರ(ರಚನೆ xkb_ಕೀಲಿ)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ಗಾತ್ರ(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ಗಾತ್ರ(ರಚನೆ xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_ಗಾತ್ರ_T_ಅಗಲ - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಒಕ್ಕೂಟ {
    ರಚನೆ {
        /** If true, the index is valid */
        bool ಸಿಕ್ಕಿತು:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_ಗಾತ್ರ_t:(DARRAY_ಗಾತ್ರ_T_ಅಗಲ - 3);
    };
    /* Only if is_alias = false, for better semantics */
    ರಚನೆ {
        bool ಸಿಕ್ಕಿತು:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool ಕೆಳಗಿನ:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ:(DARRAY_ಗಾತ್ರ_T_ಅಗಲ - 3);
    } ಕೀಲಿ;
    /* Only if is_alias = true, for better semantics */
    ರಚನೆ {
        bool ಸಿಕ್ಕಿತು:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_ಗಾತ್ರ_T_ಅಗಲ - 3);
    } alias;
} Keycode_ಹೊಂದಿಕೆ;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಒಕ್ಕೂಟ {
        /** Used for parsing */
        ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
        /** Used for serializing */
        xkb_keycode_t ಕೀಲಿ_ಸಂಕೇತ;
    };
    bool have_ಚಿಹ್ನೆ;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_ಮುಖವಾಡ_t mods;
    ಒಕ್ಕೂಟ {
        xkb_keysym_t ಕೀಲಿ_Sym;
        xkb_atom_t ಕೀಲಿ_Name;
    } u;
} Mod_Map_ನಮೂದು;

/** Common keyboard description structure */
ರಚನೆ xkb_keymap {
    ರಚನೆ xkb_context *ctx;

    ಪೂರ್ಣಾಂಕ refcnt;
    ಎಣಿಕೆ xkb_keymap_compile_flags flags;
    ಎಣಿಕೆ xkb_keymap_format format;

    xkb_led_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_leds;
    ರಚನೆ xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_ಕೀಲಿ_ಸಂಕೇತ;
    xkb_keycode_t max_ಕೀಲಿ_ಸಂಕೇತ;
    xkb_keycode_t ಅಂಕೆ_ಕೀಲಿಗಳು;
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
    xkb_keycode_t ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿಗಳು ATTR_COUNTED_BY(ಅಂಕೆ_ಕೀಲಿಗಳು);

    ಒಕ್ಕೂಟ {
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
        ರಚನೆ {
            darray_ಗಾತ್ರ_t ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು;
            Keycode_ಹೊಂದಿಕೆ *ಕೀಲಿ_ಹೆಸರುಗಳು;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        ರಚನೆ {
            darray_ಗಾತ್ರ_t ಅಂಕೆ_ಕೀಲಿ_aliases;
            ರಚನೆ xkb_ಕೀಲಿ_alias *ಕೀಲಿ_aliases;
        };
    };

    ರಚನೆ xkb_ಕೀಲಿ_type *types;
    darray_ಗಾತ್ರ_t ಅಂಕೆ_types;

    darray_ಗಾತ್ರ_t ಅಂಕೆ_sym_interprets;
    ರಚನೆ xkb_sym_interpret *sym_interprets;

    darray_ಗಾತ್ರ_t _padding;
    darray_ಗಾತ್ರ_t ಅಂಕೆ_modmaps;
    Mod_Map_ನಮೂದು *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    ರಚನೆ xkb_mod_ಗಣ mods;
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
    xkb_mod_ಮುಖವಾಡ_t canonical_state_ಮುಖವಾಡ;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_ಕೀಲಿ_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups;
    /* Not all groups must have names. */
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು;
    xkb_atom_t *ಗುಂಪು_ಹೆಸರುಗಳು ATTR_COUNTED_BY(ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು);

    ಅಕ್ಷರ *keycodes_section_name;
    ಅಕ್ಷರ *symbols_section_name;
    ಅಕ್ಷರ *types_section_name;
    ಅಕ್ಷರ *compat_section_name;
};

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_ಕೀಲಿಗಳು_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    ಗಾಗಿ ((iter) = (keymap)->ಕೀಲಿಗಳು + \
                  ((keymap)->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ == 0 ? 0 : (keymap)->min_ಕೀಲಿ_ಸಂಕೇತ); \
         (iter) < (keymap)->ಕೀಲಿಗಳು + (keymap)->ಅಂಕೆ_ಕೀಲಿಗಳು; \
         (iter)++)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_level_foreach_action(iter, level)                 \
    ಗಾಗಿ (ಒಕ್ಕೂಟ xkb_action                                     \
         *(iter) = (likely((level)->ಅಂಕೆ_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ಸ್ಥಿರಾಂಕ iter##_last_ = (iter) + (level)->ಅಂಕೆ_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_mods_foreach(iter, mods_) \
    ಗಾಗಿ ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->ಅಂಕೆ_mods; \
         (iter)++)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_mods_ಮುಖವಾಡ_foreach(ಮುಖವಾಡ, iter, mods_) \
    ಗಾಗಿ ((iter) = (mods_)->mods; \
         (ಮುಖವಾಡ) && (iter) < (mods_)->mods + (mods_)->ಅಂಕೆ_mods; \
         (iter)++, (ಮುಖವಾಡ) >>= 1) \
        ಆದರೆ ((ಮುಖವಾಡ) & 0x1)

/** Enumerate all modifiers */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_mods_enumerate(idx, iter, mods_) \
    ಗಾಗಿ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->ಅಂಕೆ_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_rmods_enumerate(idx, iter, mods_) \
    ಗಾಗಿ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_ಸೂಚ್ಯಂಕ_ಅಂಕೆ_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_vmods_enumerate(idx, iter, mods_) \
    ಗಾಗಿ ((idx) = XKB_MOD_ಸೂಚ್ಯಂಕ_ಅಂಕೆ_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_ಸೂಚ್ಯಂಕ_ಅಂಕೆ_ENTRIES]; \
         (idx) < (mods_)->ಅಂಕೆ_mods; \
         (idx)++, (iter)++)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_leds_foreach(iter, keymap) \
    ಗಾಗಿ ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->ಅಂಕೆ_leds; \
         (iter)++)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_leds_enumerate(idx, iter, keymap) \
    ಗಾಗಿ ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->ಅಂಕೆ_leds; \
         (idx)++, (iter)++)

ಶೂನ್ಯ
clear_level(ರಚನೆ xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ xkb_ಕೀಲಿ *
Xkb_ಕೀಲಿ_By_Name(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    ಆದರೆ (name < keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು) {
        ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ = keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[name];
        ಆದರೆ (ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು) {
            ಆದರೆ (!ಹೊಂದಿಕೆ.is_alias) {
                ಪ್ರತಿಜ್ಞಾ(name == keymap->ಕೀಲಿಗಳು[ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ].name);
                ಹಿಂತಿರುಗಿ &keymap->ಕೀಲಿಗಳು[ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ];
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (use_aliases) {
                ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.alias.real ==
                       keymap->ಕೀಲಿಗಳು[keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[ಹೊಂದಿಕೆ.alias.real].ಕೀಲಿ.ಸೂಚ್ಯಂಕ].name);
                ಹಿಂತಿರುಗಿ &keymap->ಕೀಲಿಗಳು[keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[ಹೊಂದಿಕೆ.alias.real].ಕೀಲಿ.ಸೂಚ್ಯಂಕ];
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *
Xkb_ಕೀಲಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಆದರೆ (kc < keymap->min_ಕೀಲಿ_ಸಂಕೇತ || kc > keymap->max_ಕೀಲಿ_ಸಂಕೇತ) {
        /* Unsupported keycode */
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ) {
        /* Low keycodes */
        ಹಿಂತಿರುಗಿ &keymap->ಕೀಲಿಗಳು[kc];
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* High keycodes: use binary search */
        xkb_keycode_t ಅಧೋ = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;
        xkb_keycode_t ಊರ್ಧ್ವ = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು;
        ಆಗ (ಅಧೋ < ಊರ್ಧ್ವ) {
            ಸ್ಥಿರಾಂಕ xkb_keycode_t mid = ಅಧೋ + (ಊರ್ಧ್ವ - 1 - ಅಧೋ) / 2;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[mid];
            ಆದರೆ (ಕೀಲಿ->keycode < kc) {
                ಅಧೋ = mid + 1;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->keycode > kc) {
                ಊರ್ಧ್ವ = mid;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ ಕೀಲಿ;
            }
        }
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *
xkb_keymap_get_ಮುಂದಿನ_defined_ಕೀಲಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    ಆದರೆ (!keymap->ಅಂಕೆ_ಕೀಲಿಗಳು || kc > XKB_KEYCODE_MAX) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kc < keymap->min_ಕೀಲಿ_ಸಂಕೇತ) {
        kc = keymap->min_ಕೀಲಿ_ಸಂಕೇತ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kc > keymap->max_ಕೀಲಿ_ಸಂಕೇತ) {
        kc = keymap->max_ಕೀಲಿ_ಸಂಕೇತ;
    }

    ಆದರೆ (kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ) {
        /* Low keycodes */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[kc];
        ಆದರೆ (ascending) {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೊನೆ = &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ - 1];
            ಆಗ (ಕೀಲಿ->name == XKB_ATOM_NONE && ಕೀಲಿ < ಕೊನೆ) ಕೀಲಿ++;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೊನೆ = &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_ಸಂಕೇತ];
            ಆಗ (ಕೀಲಿ->name == XKB_ATOM_NONE && ಕೀಲಿ > ಕೊನೆ) ಕೀಲಿ--;
        }
        ಹಿಂತಿರುಗಿ (ಕೀಲಿ->name == XKB_ATOM_NONE) ? ಖಾಲಿ : ಕೀಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* High keycodes: use binary search */
        xkb_keycode_t ಅಧೋ = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ
            ? keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t ಊರ್ಧ್ವ = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * candidate = ಖಾಲಿ;
        ಆಗ (ಅಧೋ <= ಊರ್ಧ್ವ) {
            ಸ್ಥಿರಾಂಕ xkb_keycode_t mid = ಅಧೋ + (ಊರ್ಧ್ವ - ಅಧೋ) / 2;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[mid];
            ಆದರೆ (ಕೀಲಿ->keycode == kc) {
                ಹಿಂತಿರುಗಿ ಕೀಲಿ;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                ಆದರೆ (ಕೀಲಿ->keycode >= kc) {
                    candidate = ಕೀಲಿ;
                    ಊರ್ಧ್ವ = mid - 1;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಅಧೋ = mid + 1;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Looking for the *smallest* defined keycode <= kc */
                ಆದರೆ (ಕೀಲಿ->keycode <= kc) {
                    candidate = ಕೀಲಿ;
                    ಅಧೋ = mid + 1;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->keycode > kc) {
                    ಊರ್ಧ್ವ = mid - 1;
                }
            }
        }
        ಹಿಂತಿರುಗಿ candidate;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_level_ಸೂಚ್ಯಂಕ_t
Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_ಸೂಚ್ಯಂಕ_t layout)
{
    ಹಿಂತಿರುಗಿ ಕೀಲಿ->groups[layout].type->ಅಂಕೆ_ಮಟ್ಟಗಳು;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ನಮೂದು_is_ಸಕ್ರಿಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು)
{
    ಹಿಂತಿರುಗಿ ನಮೂದು->mods.mods == 0 || ನಮೂದು->mods.ಮುಖವಾಡ != 0;
}

ರಚನೆ xkb_keymap *
xkb_keymap_new(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* func,
               ಎಣಿಕೆ xkb_keymap_format format,
               ಎಣಿಕೆ xkb_keymap_compile_flags flags);

ಶೂನ್ಯ
XkbEscapeMapName(ಅಕ್ಷರ *name);

xkb_mod_ಸೂಚ್ಯಂಕ_t
Xkb_Mod_Name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, xkb_atom_t name,
                  ಎಣಿಕೆ mod_type type);

bool
Xkb_ಮಟ್ಟಗಳು_Same_Syms(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *a, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *b);

bool
action_equal(ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *a, ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *b);

bool
Xkb_ಮಟ್ಟಗಳು_Same_Actions(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *a, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level *b);

xkb_layout_ಸೂಚ್ಯಂಕ_t
Xkb_Wrap_ಗುಂಪು_Into_Range(int32_t ಗುಂಪು,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups,
                      ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಕೀಲಿ_effective_layout(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                xkb_layout_ಸೂಚ್ಯಂಕ_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    ಹಿಂತಿರುಗಿ Xkb_Wrap_ಗುಂಪು_Into_Range((int32_t) layout,
                                 ಕೀಲಿ->ಅಂಕೆ_groups,
                                 ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy,
                                 ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ);
}

XKB_EXPORT_ಖಾಸಗಿ xkb_mod_ಮುಖವಾಡ_t
mod_ಮುಖವಾಡ_get_effective(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಮುಖವಾಡ_t mods);

ರಚನೆ xkb_level *
xkb_keymap_ಕೀಲಿ_get_level(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_ಸೂಚ್ಯಂಕ_t layout, xkb_level_ಸೂಚ್ಯಂಕ_t level);

xkb_action_ಎಣಿಕೆ_t
xkb_keymap_ಕೀಲಿ_get_actions_by_level(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    xkb_layout_ಸೂಚ್ಯಂಕ_t layout,
                                    xkb_level_ಸೂಚ್ಯಂಕ_t level,
                                    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action **actions);

ರಚನೆ xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(ರಚನೆ xkb_keymap *keymap,
                                  ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_ಹೆಸರುಗಳು)(ರಚನೆ xkb_keymap *keymap,
                                  ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *ಹೆಸರುಗಳು);
    bool (*keymap_new_from_string)(ರಚನೆ xkb_keymap *keymap,
                                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t ಉದ್ದಳತೆ);
    bool (*keymap_new_from_file)(ರಚನೆ xkb_keymap *keymap, FILE *file);
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ (*keymap_serialize)(
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
        ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ
    );
};

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops ಪಠ್ಯ_v1_keymap_format_ops;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_Mods_Un_ಬೀಗ_On_Press_Supported(ಎಣಿಕೆ xkb_keymap_format format)
{
    /* Lax bound */
    ಹಿಂತಿರುಗಿ format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ಗುಂಪು_ಬೀಗ_On_Release_Supported(ಎಣಿಕೆ xkb_keymap_format format)
{
    /* Lax bound */
    ಹಿಂತಿರುಗಿ format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
isModsLatchOnPressSupported(ಎಣಿಕೆ xkb_keymap_format format)
{
    /* Lax bound */
    ಹಿಂತಿರುಗಿ format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
areOverlappingOverlaysSupported(ಎಣಿಕೆ xkb_keymap_format format)
{
    /* Lax bound */
    ಹಿಂತಿರುಗಿ format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2;
}
