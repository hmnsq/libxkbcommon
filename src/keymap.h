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
#pragma once

 /* Don't use compat names in internal code. */
#define _XKBCOMMON_COMPAT_H

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
#define XKB_MAX_GROUPS 32
#define XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#define XKB_MAX_GROUPS_X11 4

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_layout_index_t
format_max_groups(ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#define XKB_MAX_LEDS ((xkb_led_index_t) (ಪರಿಮಾಣ(xkb_led_mask_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#define XKB_MAX_MODS ((xkb_mod_index_t) (ಪರಿಮಾಣ(xkb_mod_mask_t) * CHAR_BIT))

ಗಣನಾ {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#define XKB_MOD_NONE 0xffffffffU

ಗಣನಾ mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#define MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
ಗಣನಾ real_mod_index {
    XKB_MOD_INDEX_SHIFT = 0,
    XKB_MOD_INDEX_CAPS,
    XKB_MOD_INDEX_CTRL,
    XKB_MOD_INDEX_MOD1,
    XKB_MOD_INDEX_MOD2,
    XKB_MOD_INDEX_MOD3,
    XKB_MOD_INDEX_MOD4,
    XKB_MOD_INDEX_MOD5,
    XKB_MOD_INDEX_ಅಂಕೆ_ENTRIES
};
static_assert(XKB_MOD_INDEX_ಅಂಕೆ_ENTRIES == 8, "Invalid X11 core modifiers");

ಗಣನಾ xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_SET,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_LOCK,
    ACTION_TYPE_GROUP_SET,
    ACTION_TYPE_GROUP_LATCH,
    ACTION_TYPE_GROUP_LOCK,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_LOCK,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_SET,
    ACTION_TYPE_CTRL_LOCK,
    ACTION_TYPE_REDIRECT_ಕೀಲಿ,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_ಅಂಕೆ_ENTRIES
};

ಗಣನಾ xkb_action_flags {
    ACTION_LOCK_CLEAR = (1 << 0),
    ACTION_LATCH_TO_LOCK = (1 << 1),
    ACTION_LOCK_NO_LOCK = (1 << 2),
    ACTION_LOCK_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_LOCK_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_LOCK_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE = (1 << 16),
};

ಗಣನಾ {
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
ಗಣನಾ xkb_action_controls {
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
    CONTROL_IGNORE_GROUP_LOCK = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    _LAST_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು | CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_GROUP_LOCK | CONTROL_GROUPS_WRAP),
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
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(ಪರಿಮಾಣ(ಗಣನಾ xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_action_controls
format_boolean_controls(ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_ಕೀಲಿಗಳು ==
    (ಗಣನಾ xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_ಕೀಲಿಗಳು ==
    (ಗಣನಾ xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು,
    "Private value should match public API"
);


ಗಣನಾ xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

ಸಂರಚನಾ xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

ಸಂರಚನಾ xkb_mod_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    ಸಂರಚನಾ xkb_mods mods;
};

ಸಂರಚನಾ xkb_group_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t xkb_overlay_index_t;
/** Keyboard overlay mask */
ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t xkb_overlay_mask_t;
ಗಣನಾ {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ಎಣಿಕೆ_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_ಎಣಿಕೆ = (ಪರಿಮಾಣ(xkb_overlay_mask_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_ಎಣಿಕೆ - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_INDEX_MIN_WIDTH = 4, /* nibble */

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
static_assert(XKB_OVERLAY_ಎಣಿಕೆ <= (1u << XKB_OVERLAY_INDEX_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_overlay_mask_t
overlays_from_controls(ಗಣನಾ xkb_action_controls controls)
{
    ಪ್ರತ್ಯಾಗಮ (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    ಪ್ರತ್ಯಾಗಮ (ಗಣನಾ xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_overlay_index_t
format_max_overlays(ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_ಎಣಿಕೆ_X11
        : XKB_OVERLAY_ಎಣಿಕೆ;
}

ಸಂರಚನಾ xkb_controls_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    ಗಣನಾ xkb_action_controls ctrls;
};

ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t xkb_pointer_button_index_t;
ಪ್ರಕಾರನಿರ್ದೇಶ uint8_t xkb_pointer_button_mask_t;

ಗಣನಾ {
    XKB_POINTER_BUTTON_INDEX_WIDTH =
        ಪರಿಮಾಣ(xkb_pointer_button_index_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_INDEX_MAX =
        (1 << XKB_POINTER_BUTTON_INDEX_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        ಪರಿಮಾಣ(xkb_pointer_button_mask_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_INDEX_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

ಸಂರಚನಾ xkb_pointer_default_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    int8_t ಮೌಲ್ಯ;
};

ಗಣನಾ {
    XKB_POINTER_DEFAULT_ACTION_ಮೌಲ್ಯ_WIDTH =
        ಪರಿಮಾಣ(((ಸಂರಚನಾ xkb_pointer_default_action*)0)->ಮೌಲ್ಯ) * CHAR_BIT,
    XKB_POINTER_DEFAULT_ACTION_ಮೌಲ್ಯ_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_ಮೌಲ್ಯ_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_ಮೌಲ್ಯ_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

ಸಂರಚನಾ xkb_pointer_motion_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

ಸಂರಚನಾ xkb_pointer_button_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    uint8_t ಎಣಿಕೆ;
    xkb_pointer_button_index_t button;
};

ಸಂರಚನಾ xkb_switch_screen_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_action_flags flags;
    int8_t screen;
};

ಸಂರಚನಾ xkb_redirect_ಕೀಲಿ_action {
    ಗಣನಾ xkb_action_type type;
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
    xkb_mod_mask_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_mask_t mods;
};

ಸಂರಚನಾ xkb_private_action {
    ಗಣನಾ xkb_action_type type;
    uint8_t data[7];
};

ಗಣನಾ xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
ಸಂರಚನಾ xkb_internal_action {
    ಗಣನಾ xkb_action_type type;
    ಗಣನಾ xkb_internal_action_flags flags;
    ಸಙ್ಘ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

ಸಙ್ಘ xkb_action {
    ಗಣನಾ xkb_action_type type;
    ಸಂರಚನಾ xkb_mod_action mods;
    ಸಂರಚನಾ xkb_group_action group;
    ಸಂರಚನಾ xkb_controls_action ctrls;
    ಸಂರಚನಾ xkb_redirect_ಕೀಲಿ_action redirect;
    ಸಂರಚನಾ xkb_pointer_default_action dflt;
    ಸಂರಚನಾ xkb_pointer_motion_action ptr;
    ಸಂರಚನಾ xkb_pointer_button_action btn;
    ಸಂರಚನಾ xkb_switch_screen_action screen;
    ಸಂರಚನಾ xkb_private_action priv;
    ಸಂರಚನಾ xkb_internal_action internal;
};

ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry {
    xkb_level_index_t level;
    ಸಂರಚನಾ xkb_mods mods;
    ಸಂರಚನಾ xkb_mods preserve;
};

ಸಂರಚನಾ xkb_ಕೀಲಿ_type {
    xkb_atom_t name;
    ಸಂರಚನಾ xkb_mods mods;
    bool required;
    xkb_level_index_t ಅಂಕೆ_levels;
    xkb_level_index_t ಅಂಕೆ_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(ಅಂಕೆ_level_names);
    darray_size_t ಅಂಕೆ_entries;
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entries ATTR_COUNTED_BY(ಅಂಕೆ_entries);
};

ಪ್ರಕಾರನಿರ್ದೇಶ uint16_t xkb_action_ಎಣಿಕೆ_t;
#define MAX_ACTIONS_PER_LEVEL UINT16_MAX

ಸಂರಚನಾ xkb_sym_interpret {
    xkb_keysym_t sym;
    ಗಣನಾ xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_index_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_ಎಣಿಕೆ_t ಅಂಕೆ_actions;
    ಸಙ್ಘ {
        /* num_actions <= 1 */
        ಸಙ್ಘ xkb_action action;
        /* num_actions >  1 */
        ಸಙ್ಘ xkb_action *actions;
    } a;
};

ಗಣನಾ {XKB_STATE_COMPONENT_WIDTH = (ಪರಿಮಾಣ(ಗಣನಾ xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

ಸಂರಚನಾ xkb_led {
    xkb_atom_t name;
    ಗಣನಾ xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    ಗಣನಾ xkb_state_component which_mods;
    ಸಂರಚನಾ xkb_mods mods;
    ಗಣನಾ xkb_action_controls ctrls;
};

ಸಂರಚನಾ xkb_ಕೀಲಿ_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

ಸಂರಚನಾ xkb_controls {
    unsigned ವರ್ಣ groups_wrap;
    ಸಂರಚನಾ xkb_mods internal;
    ಸಂರಚನಾ xkb_mods ignore_lock;
    unsigned short repeat_delay;
    unsigned short repeat_interval;
    unsigned short slow_ಕೀಲಿಗಳು_delay;
    unsigned short debounce_delay;
    unsigned short ax_options;
    unsigned short ax_timeout;
    unsigned short axt_opts_mask;
    unsigned short axt_opts_ಮೌಲ್ಯಗಳು;
    unsigned ಪೂರ್ಣಾಙ್ಕ axt_ctrls_mask;
    unsigned ಪೂರ್ಣಾಙ್ಕ axt_ctrls_ಮೌಲ್ಯಗಳು;
};

ಗಣನಾ xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

ಪ್ರಕಾರನಿರ್ದೇಶ uint16_t xkb_keysym_ಎಣಿಕೆ_t;
#define MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
ಸಂರಚನಾ xkb_level {
    /** Count of keysyms */
    xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms;
    /** Count of actions */
    xkb_action_ಎಣಿಕೆ_t ಅಂಕೆ_actions;
    /** Upper keysym */
    ಸಙ್ಘ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    ಸಙ್ಘ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    ಸಙ್ಘ {
        ಸಙ್ಘ xkb_action action;   /* num_actions == 1 */
        ಸಙ್ಘ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
ಸಂರಚನಾ xkb_group {
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
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    ಸಂರಚನಾ xkb_level *levels;
};

ಗಣನಾ {
    DEFAULT_ಕೀಲಿ_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT = true,
    FALLBACK_INTERPRET_ಕೀಲಿ_REPEAT = DEFAULT_ಕೀಲಿ_REPEAT
};

ಗಣನಾ {
    DEFAULT_ಕೀಲಿ_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_ಕೀಲಿ_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_ಕೀಲಿ_VMODMAP
};

ಸಂರಚನಾ xkb_ಕೀಲಿ {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    ಗಣನಾ xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    ಗಣನಾ xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_index_t out_of_range_group_ಸಂಖ್ಯೆ:8;

    xkb_layout_index_t ಅಂಕೆ_groups:8;
    ಸಂರಚನಾ xkb_group *groups ATTR_COUNTED_BY(ಅಂಕೆ_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ಸಙ್ಘ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು;
    };
};

ಸಂರಚನಾ xkb_mod {
    xkb_atom_t name;
    ಗಣನಾ mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

ಸಂರಚನಾ xkb_mod_set {
    ಸಂರಚನಾ xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_index_t ಅಂಕೆ_mods;
    xkb_mod_mask_t explicit_vmods;
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
#define XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ಪರಿಮಾಣ(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_ಕೀಲಿ)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#define XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ಪರಿಮಾಣ(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_SIZE_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಙ್ಘ {
    ಸಂರಚನಾ {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_size_t:(DARRAY_SIZE_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    ಸಂರಚನಾ {
        bool found:1;
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
        darray_size_t index:(DARRAY_SIZE_T_WIDTH - 3);
    } ಕೀಲಿ;
    /* Only if is_alias = true, for better semantics */
    ಸಂರಚನಾ {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_SIZE_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಸಙ್ಘ {
        /** Used for parsing */
        ಗಣನಾ merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t ಕೀಲಿ_Code;
    };
    bool have_ಚಿಹ್ನೆ;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    ಸಙ್ಘ {
        xkb_keysym_t ಕೀಲಿ_Sym;
        xkb_atom_t ಕೀಲಿ_Name;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
ಸಂರಚನಾ xkb_keymap {
    ಸಂರಚನಾ xkb_context *ctx;

    ಪೂರ್ಣಾಙ್ಕ refcnt;
    ಗಣನಾ xkb_keymap_compile_flags flags;
    ಗಣನಾ xkb_keymap_format format;

    xkb_led_index_t ಅಂಕೆ_leds;
    ಸಂರಚನಾ xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_ಕೀಲಿ_code;
    xkb_keycode_t max_ಕೀಲಿ_code;
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
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿಗಳು ATTR_COUNTED_BY(ಅಂಕೆ_ಕೀಲಿಗಳು);

    ಸಙ್ಘ {
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
        ಸಂರಚನಾ {
            darray_size_t ಅಂಕೆ_ಕೀಲಿ_names;
            KeycodeMatch *ಕೀಲಿ_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        ಸಂರಚನಾ {
            darray_size_t ಅಂಕೆ_ಕೀಲಿ_aliases;
            ಸಂರಚನಾ xkb_ಕೀಲಿ_alias *ಕೀಲಿ_aliases;
        };
    };

    ಸಂರಚನಾ xkb_ಕೀಲಿ_type *types;
    darray_size_t ಅಂಕೆ_types;

    darray_size_t ಅಂಕೆ_sym_interprets;
    ಸಂರಚನಾ xkb_sym_interpret *sym_interprets;

    darray_size_t _padding;
    darray_size_t ಅಂಕೆ_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    ಸಂರಚನಾ xkb_mod_set mods;
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
    xkb_mod_mask_t canonical_state_mask;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_ಕೀಲಿ_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_index_t ಅಂಕೆ_groups;
    /* Not all groups must have names. */
    xkb_layout_index_t ಅಂಕೆ_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(ಅಂಕೆ_group_names);

    ವರ್ಣ *keycodes_section_name;
    ವರ್ಣ *symbols_section_name;
    ವರ್ಣ *types_section_name;
    ವರ್ಣ *compat_section_name;
};

#define xkb_ಕೀಲಿಗಳು_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    ಪ್ರತಿ ((iter) = (keymap)->ಕೀಲಿಗಳು + \
                  ((keymap)->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ == 0 ? 0 : (keymap)->min_ಕೀಲಿ_code); \
         (iter) < (keymap)->ಕೀಲಿಗಳು + (keymap)->ಅಂಕೆ_ಕೀಲಿಗಳು; \
         (iter)++)

#define xkb_level_foreach_action(iter, level)                 \
    ಪ್ರತಿ (ಸಙ್ಘ xkb_action                                     \
         *(iter) = (likely((level)->ಅಂಕೆ_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ಧ್ರುವ iter##_last_ = (iter) + (level)->ಅಂಕೆ_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#define xkb_mods_foreach(iter, mods_) \
    ಪ್ರತಿ ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->ಅಂಕೆ_mods; \
         (iter)++)

#define xkb_mods_mask_foreach(mask, iter, mods_) \
    ಪ್ರತಿ ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->ಅಂಕೆ_mods; \
         (iter)++, (mask) >>= 1) \
        ಯದಿ ((mask) & 0x1)

/** Enumerate all modifiers */
#define xkb_mods_enumerate(idx, iter, mods_) \
    ಪ್ರತಿ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->ಅಂಕೆ_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#define xkb_rmods_enumerate(idx, iter, mods_) \
    ಪ್ರತಿ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_INDEX_ಅಂಕೆ_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#define xkb_vmods_enumerate(idx, iter, mods_) \
    ಪ್ರತಿ ((idx) = XKB_MOD_INDEX_ಅಂಕೆ_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_INDEX_ಅಂಕೆ_ENTRIES]; \
         (idx) < (mods_)->ಅಂಕೆ_mods; \
         (idx)++, (iter)++)

#define xkb_leds_foreach(iter, keymap) \
    ಪ್ರತಿ ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->ಅಂಕೆ_leds; \
         (iter)++)

#define xkb_leds_enumerate(idx, iter, keymap) \
    ಪ್ರತಿ ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->ಅಂಕೆ_leds; \
         (idx)++, (iter)++)

ಶೂನ್ಯ
clear_level(ಸಂರಚನಾ xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ xkb_ಕೀಲಿ *
Xkb_ಕೀಲಿ_By_Name(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    ಯದಿ (name < keymap->ಅಂಕೆ_ಕೀಲಿ_names) {
        ಧ್ರುವ KeycodeMatch match = keymap->ಕೀಲಿ_names[name];
        ಯದಿ (match.found) {
            ಯದಿ (!match.is_alias) {
                ಪ್ರತಿಜ್ಞಾ(name == keymap->ಕೀಲಿಗಳು[match.ಕೀಲಿ.index].name);
                ಪ್ರತ್ಯಾಗಮ &keymap->ಕೀಲಿಗಳು[match.ಕೀಲಿ.index];
            } ಅನ್ಯಥಾ ಯದಿ (use_aliases) {
                ಪ್ರತಿಜ್ಞಾ(match.alias.real ==
                       keymap->ಕೀಲಿಗಳು[keymap->ಕೀಲಿ_names[match.alias.real].ಕೀಲಿ.index].name);
                ಪ್ರತ್ಯಾಗಮ &keymap->ಕೀಲಿಗಳು[keymap->ಕೀಲಿ_names[match.alias.real].ಕೀಲಿ.index];
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *
Xkb_ಕೀಲಿ(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಯದಿ (kc < keymap->min_ಕೀಲಿ_code || kc > keymap->max_ಕೀಲಿ_code) {
        /* Unsupported keycode */
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    } ಅನ್ಯಥಾ ಯದಿ (kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ) {
        /* Low keycodes */
        ಪ್ರತ್ಯಾಗಮ &keymap->ಕೀಲಿಗಳು[kc];
    } ಅನ್ಯಥಾ {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;
        xkb_keycode_t upper = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು;
        ಯಾವತ್ (lower < upper) {
            ಧ್ರುವ xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[mid];
            ಯದಿ (ಕೀಲಿ->keycode < kc) {
                lower = mid + 1;
            } ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->keycode > kc) {
                upper = mid;
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *
xkb_keymap_get_next_defined_ಕೀಲಿ(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    ಯದಿ (!keymap->ಅಂಕೆ_ಕೀಲಿಗಳು || kc > XKB_KEYCODE_MAX) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    } ಅನ್ಯಥಾ ಯದಿ (kc < keymap->min_ಕೀಲಿ_code) {
        kc = keymap->min_ಕೀಲಿ_code;
    } ಅನ್ಯಥಾ ಯದಿ (kc > keymap->max_ಕೀಲಿ_code) {
        kc = keymap->max_ಕೀಲಿ_code;
    }

    ಯದಿ (kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ) {
        /* Low keycodes */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[kc];
        ಯದಿ (ascending) {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *end = &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ - 1];
            ಯಾವತ್ (ಕೀಲಿ->name == XKB_ATOM_NONE && ಕೀಲಿ < end) ಕೀಲಿ++;
        } ಅನ್ಯಥಾ {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *end = &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_code];
            ಯಾವತ್ (ಕೀಲಿ->name == XKB_ATOM_NONE && ಕೀಲಿ > end) ಕೀಲಿ--;
        }
        ಪ್ರತ್ಯಾಗಮ (ಕೀಲಿ->name == XKB_ATOM_NONE) ? ರಿಕ್ತ : ಕೀಲಿ;
    } ಅನ್ಯಥಾ {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ
            ? keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1;
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * candidate = ರಿಕ್ತ;
        ಯಾವತ್ (lower <= upper) {
            ಧ್ರುವ xkb_keycode_t mid = lower + (upper - lower) / 2;
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[mid];
            ಯದಿ (ಕೀಲಿ->keycode == kc) {
                ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
            } ಅನ್ಯಥಾ ಯದಿ (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                ಯದಿ (ಕೀಲಿ->keycode >= kc) {
                    candidate = ಕೀಲಿ;
                    upper = mid - 1;
                } ಅನ್ಯಥಾ {
                    lower = mid + 1;
                }
            } ಅನ್ಯಥಾ {
                /* Looking for the *smallest* defined keycode <= kc */
                ಯದಿ (ಕೀಲಿ->keycode <= kc) {
                    candidate = ಕೀಲಿ;
                    lower = mid + 1;
                } ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ candidate;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_level_index_t
Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_index_t layout)
{
    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ->groups[layout].type->ಅಂಕೆ_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
entry_is_active(ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry)
{
    ಪ್ರತ್ಯಾಗಮ entry->mods.mods == 0 || entry->mods.mask != 0;
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* func,
               ಗಣನಾ xkb_keymap_format format,
               ಗಣನಾ xkb_keymap_compile_flags flags);

ಶೂನ್ಯ
XkbEscapeMapName(ವರ್ಣ *name);

xkb_mod_index_t
XkbModNameToIndex(ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, xkb_atom_t name,
                  ಗಣನಾ mod_type type);

bool
XkbLevelsSameSyms(ಧ್ರುವ ಸಂರಚನಾ xkb_level *a, ಧ್ರುವ ಸಂರಚನಾ xkb_level *b);

bool
action_equal(ಧ್ರುವ ಸಙ್ಘ xkb_action *a, ಧ್ರುವ ಸಙ್ಘ xkb_action *b);

bool
XkbLevelsSameActions(ಧ್ರುವ ಸಂರಚನಾ xkb_level *a, ಧ್ರುವ ಸಂರಚನಾ xkb_level *b);

xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t ಅಂಕೆ_groups,
                      ಗಣನಾ xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_ಸಂಖ್ಯೆ);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_layout_index_t
xkb_keymap_ಕೀಲಿ_effective_layout(ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                xkb_layout_index_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    ಪ್ರತ್ಯಾಗಮ XkbWrapGroupIntoRange((int32_t) layout,
                                 ಕೀಲಿ->ಅಂಕೆ_groups,
                                 ಕೀಲಿ->out_of_range_group_policy,
                                 ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_mask_t mods);

ಸಂರಚನಾ xkb_level *
xkb_keymap_ಕೀಲಿ_get_level(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_index_t layout, xkb_level_index_t level);

xkb_action_ಎಣಿಕೆ_t
xkb_keymap_ಕೀಲಿ_get_actions_by_level(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                                    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ಧ್ರುವ ಸಙ್ಘ xkb_action **actions);

ಸಂರಚನಾ xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(ಸಂರಚನಾ xkb_keymap *keymap,
                                  ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(ಸಂರಚನಾ xkb_keymap *keymap,
                                  ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *names);
    bool (*keymap_new_from_string)(ಸಂರಚನಾ xkb_keymap *keymap,
                                   ಧ್ರುವ ವರ್ಣ *string, size_t length);
    bool (*keymap_new_from_file)(ಸಂರಚನಾ xkb_keymap *keymap, FILE *file);
    ಗಣನಾ xkb_status (*keymap_serialize)(
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
        ಸಂರಚನಾ xkb_keymap_serialize_result *result
    );
};

ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops text_v1_keymap_format_ops;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
isModsUnLockOnPressSupported(ಗಣನಾ xkb_keymap_format format)
{
    /* Lax bound */
    ಪ್ರತ್ಯಾಗಮ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
isGroupLockOnReleaseSupported(ಗಣನಾ xkb_keymap_format format)
{
    /* Lax bound */
    ಪ್ರತ್ಯಾಗಮ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
isModsLatchOnPressSupported(ಗಣನಾ xkb_keymap_format format)
{
    /* Lax bound */
    ಪ್ರತ್ಯಾಗಮ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
areOverlappingOverlaysSupported(ಗಣನಾ xkb_keymap_format format)
{
    /* Lax bound */
    ಪ್ರತ್ಯಾಗಮ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
