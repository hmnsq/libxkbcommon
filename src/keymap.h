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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "rmlvo.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#define XKB_MAX_GROUPS 32
#define XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#define XKB_MAX_GROUPS_X11 4

நிலையான உள்வரி xkb_layout_index_t
format_max_groups(எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#define XKB_MAX_LEDS ((xkb_led_index_t) (அளவு(xkb_led_mask_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#define XKB_MAX_MODS ((xkb_mod_index_t) (அளவு(xkb_mod_mask_t) * CHAR_BIT))

எண்ணிக்கை {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#define XKB_MOD_NONE 0xffffffffU

எண்ணிக்கை mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#define MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
எண்ணிக்கை real_mod_index {
    XKB_MOD_INDEX_SHIFT = 0,
    XKB_MOD_INDEX_CAPS,
    XKB_MOD_INDEX_CTRL,
    XKB_MOD_INDEX_MOD1,
    XKB_MOD_INDEX_MOD2,
    XKB_MOD_INDEX_MOD3,
    XKB_MOD_INDEX_MOD4,
    XKB_MOD_INDEX_MOD5,
    XKB_MOD_INDEX_இலக்கம்_ENTRIES
};
static_assert(XKB_MOD_INDEX_இலக்கம்_ENTRIES == 8, "Invalid X11 core modifiers");

எண்ணிக்கை xkb_action_type {
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
    ACTION_TYPE_REDIRECT_சாவி,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_இலக்கம்_ENTRIES
};

எண்ணிக்கை xkb_action_flags {
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

எண்ணிக்கை {
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
எண்ணிக்கை xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_சாவிகள் = (1 << 0),
    CONTROL_MOUSE_சாவிகள் = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_சாவிகள்_ACCEL = (1 << 15),
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
         CONTROL_STICKY_சாவிகள் | CONTROL_MOUSE_சாவிகள் | CONTROL_MOUSE_சாவிகள்_ACCEL | \
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
         CONTROL_STICKY_சாவிகள் | CONTROL_MOUSE_சாவிகள் | CONTROL_MOUSE_சாவிகள்_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(அளவு(எண்ணிக்கை xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

நிலையான உள்வரி எண்ணிக்கை xkb_action_controls
format_boolean_controls(எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_சாவிகள் ==
    (எண்ணிக்கை xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_சாவிகள்,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_சாவிகள் ==
    (எண்ணிக்கை xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_சாவிகள்,
    "Private value should match public API"
);


எண்ணிக்கை xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

கட்டமைப்பு xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

கட்டமைப்பு xkb_mod_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    கட்டமைப்பு xkb_mods mods;
};

கட்டமைப்பு xkb_group_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
வகைவரை uint8_t xkb_overlay_index_t;
/** Keyboard overlay mask */
வகைவரை uint8_t xkb_overlay_mask_t;
எண்ணிக்கை {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_மொத்தம்_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_மொத்தம் = (அளவு(xkb_overlay_mask_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_மொத்தம் - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_INDEX_MIN_WIDTH = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_மொத்தம்_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_மொத்தம்) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_மொத்தம் ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_மொத்தம் < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_மொத்தம் <= (1u << XKB_OVERLAY_INDEX_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

நிலையான உள்வரி xkb_overlay_mask_t
overlays_from_controls(எண்ணிக்கை xkb_action_controls controls)
{
    திரும்பு (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

நிலையான உள்வரி எண்ணிக்கை xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    திரும்பு (எண்ணிக்கை xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

நிலையான உள்வரி xkb_overlay_index_t
format_max_overlays(எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_மொத்தம்_X11
        : XKB_OVERLAY_மொத்தம்;
}

கட்டமைப்பு xkb_controls_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    எண்ணிக்கை xkb_action_controls ctrls;
};

வகைவரை uint8_t xkb_pointer_button_index_t;
வகைவரை uint8_t xkb_pointer_button_mask_t;

எண்ணிக்கை {
    XKB_POINTER_BUTTON_INDEX_WIDTH =
        அளவு(xkb_pointer_button_index_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_INDEX_MAX =
        (1 << XKB_POINTER_BUTTON_INDEX_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        அளவு(xkb_pointer_button_mask_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_INDEX_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

கட்டமைப்பு xkb_pointer_default_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    int8_t மதிப்பு;
};

எண்ணிக்கை {
    XKB_POINTER_DEFAULT_ACTION_மதிப்பு_WIDTH =
        அளவு(((கட்டமைப்பு xkb_pointer_default_action*)0)->மதிப்பு) * CHAR_BIT,
    XKB_POINTER_DEFAULT_ACTION_மதிப்பு_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_மதிப்பு_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_மதிப்பு_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

கட்டமைப்பு xkb_pointer_motion_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

கட்டமைப்பு xkb_pointer_button_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    uint8_t மொத்தம்;
    xkb_pointer_button_index_t button;
};

கட்டமைப்பு xkb_switch_screen_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_action_flags flags;
    int8_t screen;
};

கட்டமைப்பு xkb_redirect_சாவி_action {
    எண்ணிக்கை xkb_action_type type;
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

கட்டமைப்பு xkb_private_action {
    எண்ணிக்கை xkb_action_type type;
    uint8_t data[7];
};

எண்ணிக்கை xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
கட்டமைப்பு xkb_internal_action {
    எண்ணிக்கை xkb_action_type type;
    எண்ணிக்கை xkb_internal_action_flags flags;
    ஒன்றிப்பு {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

ஒன்றிப்பு xkb_action {
    எண்ணிக்கை xkb_action_type type;
    கட்டமைப்பு xkb_mod_action mods;
    கட்டமைப்பு xkb_group_action group;
    கட்டமைப்பு xkb_controls_action ctrls;
    கட்டமைப்பு xkb_redirect_சாவி_action redirect;
    கட்டமைப்பு xkb_pointer_default_action dflt;
    கட்டமைப்பு xkb_pointer_motion_action ptr;
    கட்டமைப்பு xkb_pointer_button_action btn;
    கட்டமைப்பு xkb_switch_screen_action screen;
    கட்டமைப்பு xkb_private_action priv;
    கட்டமைப்பு xkb_internal_action internal;
};

கட்டமைப்பு xkb_சாவி_type_entry {
    xkb_level_index_t level;
    கட்டமைப்பு xkb_mods mods;
    கட்டமைப்பு xkb_mods preserve;
};

கட்டமைப்பு xkb_சாவி_type {
    xkb_atom_t name;
    கட்டமைப்பு xkb_mods mods;
    bool required;
    xkb_level_index_t இலக்கம்_levels;
    xkb_level_index_t இலக்கம்_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(இலக்கம்_level_names);
    darray_size_t இலக்கம்_entries;
    கட்டமைப்பு xkb_சாவி_type_entry *entries ATTR_COUNTED_BY(இலக்கம்_entries);
};

வகைவரை uint16_t xkb_action_மொத்தம்_t;
#define MAX_ACTIONS_PER_LEVEL UINT16_MAX

கட்டமைப்பு xkb_sym_interpret {
    xkb_keysym_t sym;
    எண்ணிக்கை xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_index_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_மொத்தம்_t இலக்கம்_actions;
    ஒன்றிப்பு {
        /* num_actions <= 1 */
        ஒன்றிப்பு xkb_action action;
        /* num_actions >  1 */
        ஒன்றிப்பு xkb_action *actions;
    } a;
};

எண்ணிக்கை {XKB_STATE_COMPONENT_WIDTH = (அளவு(எண்ணிக்கை xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

கட்டமைப்பு xkb_led {
    xkb_atom_t name;
    எண்ணிக்கை xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    எண்ணிக்கை xkb_state_component which_mods;
    கட்டமைப்பு xkb_mods mods;
    எண்ணிக்கை xkb_action_controls ctrls;
};

கட்டமைப்பு xkb_சாவி_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

கட்டமைப்பு xkb_controls {
    unsigned எழுத்து groups_wrap;
    கட்டமைப்பு xkb_mods internal;
    கட்டமைப்பு xkb_mods ignore_lock;
    unsigned short repeat_delay;
    unsigned short repeat_interval;
    unsigned short slow_சாவிகள்_delay;
    unsigned short debounce_delay;
    unsigned short ax_options;
    unsigned short ax_timeout;
    unsigned short axt_opts_mask;
    unsigned short axt_opts_மதிப்புகள்;
    unsigned முழு axt_ctrls_mask;
    unsigned முழு axt_ctrls_மதிப்புகள்;
};

எண்ணிக்கை xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

வகைவரை uint16_t xkb_keysym_மொத்தம்_t;
#define MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
கட்டமைப்பு xkb_level {
    /** Count of keysyms */
    xkb_keysym_மொத்தம்_t இலக்கம்_syms;
    /** Count of actions */
    xkb_action_மொத்தம்_t இலக்கம்_actions;
    /** Upper keysym */
    ஒன்றிப்பு {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    ஒன்றிப்பு {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    ஒன்றிப்பு {
        ஒன்றிப்பு xkb_action action;   /* num_actions == 1 */
        ஒன்றிப்பு xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
கட்டமைப்பு xkb_group {
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
    மாறிலி கட்டமைப்பு xkb_சாவி_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    கட்டமைப்பு xkb_level *levels;
};

எண்ணிக்கை {
    DEFAULT_சாவி_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_சாவி_REPEAT = true,
    FALLBACK_INTERPRET_சாவி_REPEAT = DEFAULT_சாவி_REPEAT
};

எண்ணிக்கை {
    DEFAULT_சாவி_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_சாவி_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_சாவி_VMODMAP
};

கட்டமைப்பு xkb_சாவி {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    எண்ணிக்கை xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    எண்ணிக்கை xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_index_t out_of_range_group_எண்:8;

    xkb_layout_index_t இலக்கம்_groups:8;
    கட்டமைப்பு xkb_group *groups ATTR_COUNTED_BY(இலக்கம்_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ஒன்றிப்பு {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        மாறிலி கட்டமைப்பு xkb_சாவி *overlay_சாவி;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        மாறிலி கட்டமைப்பு xkb_சாவி **overlays_சாவிகள்;
    };
};

கட்டமைப்பு xkb_mod {
    xkb_atom_t name;
    எண்ணிக்கை mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

கட்டமைப்பு xkb_mod_set {
    கட்டமைப்பு xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_index_t இலக்கம்_mods;
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
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(அளவு(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(அளவு(கட்டமைப்பு xkb_சாவி)),
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
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(அளவு(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(அளவு(கட்டமைப்பு xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_SIZE_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
வகைவரை ஒன்றிப்பு {
    கட்டமைப்பு {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_size_t:(DARRAY_SIZE_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    கட்டமைப்பு {
        bool found:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool கீழ்:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_size_t index:(DARRAY_SIZE_T_WIDTH - 3);
    } சாவி;
    /* Only if is_alias = true, for better semantics */
    கட்டமைப்பு {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_SIZE_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

வகைவரை கட்டமைப்பு {
    ஒன்றிப்பு {
        /** Used for parsing */
        எண்ணிக்கை merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t சாவி_Code;
    };
    bool have_குறியீடு;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    ஒன்றிப்பு {
        xkb_keysym_t சாவி_Sym;
        xkb_atom_t சாவி_Name;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
கட்டமைப்பு xkb_keymap {
    கட்டமைப்பு xkb_context *ctx;

    முழு refcnt;
    எண்ணிக்கை xkb_keymap_compile_flags flags;
    எண்ணிக்கை xkb_keymap_format format;

    xkb_led_index_t இலக்கம்_leds;
    கட்டமைப்பு xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_சாவி_code;
    xkb_keycode_t max_சாவி_code;
    xkb_keycode_t இலக்கம்_சாவிகள்;
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
    xkb_keycode_t இலக்கம்_சாவிகள்_கீழ்;
    கட்டமைப்பு xkb_சாவி *சாவிகள் ATTR_COUNTED_BY(இலக்கம்_சாவிகள்);

    ஒன்றிப்பு {
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
        கட்டமைப்பு {
            darray_size_t இலக்கம்_சாவி_names;
            KeycodeMatch *சாவி_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        கட்டமைப்பு {
            darray_size_t இலக்கம்_சாவி_aliases;
            கட்டமைப்பு xkb_சாவி_alias *சாவி_aliases;
        };
    };

    கட்டமைப்பு xkb_சாவி_type *types;
    darray_size_t இலக்கம்_types;

    darray_size_t இலக்கம்_sym_interprets;
    கட்டமைப்பு xkb_sym_interpret *sym_interprets;

    darray_size_t _padding;
    darray_size_t இலக்கம்_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    கட்டமைப்பு xkb_mod_set mods;
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
    xkb_keycode_t redirect_சாவி_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_index_t இலக்கம்_groups;
    /* Not all groups must have names. */
    xkb_layout_index_t இலக்கம்_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(இலக்கம்_group_names);

    எழுத்து *keycodes_section_name;
    எழுத்து *symbols_section_name;
    எழுத்து *types_section_name;
    எழுத்து *compat_section_name;
};

#define xkb_சாவிகள்_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    க்கு ((iter) = (keymap)->சாவிகள் + \
                  ((keymap)->இலக்கம்_சாவிகள்_கீழ் == 0 ? 0 : (keymap)->min_சாவி_code); \
         (iter) < (keymap)->சாவிகள் + (keymap)->இலக்கம்_சாவிகள்; \
         (iter)++)

#define xkb_level_foreach_action(iter, level)                 \
    க்கு (ஒன்றிப்பு xkb_action                                     \
         *(iter) = (likely((level)->இலக்கம்_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * மாறிலி iter##_last_ = (iter) + (level)->இலக்கம்_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#define xkb_mods_foreach(iter, mods_) \
    க்கு ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->இலக்கம்_mods; \
         (iter)++)

#define xkb_mods_mask_foreach(mask, iter, mods_) \
    க்கு ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->இலக்கம்_mods; \
         (iter)++, (mask) >>= 1) \
        எனில் ((mask) & 0x1)

/** Enumerate all modifiers */
#define xkb_mods_enumerate(idx, iter, mods_) \
    க்கு ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->இலக்கம்_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#define xkb_rmods_enumerate(idx, iter, mods_) \
    க்கு ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_INDEX_இலக்கம்_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#define xkb_vmods_enumerate(idx, iter, mods_) \
    க்கு ((idx) = XKB_MOD_INDEX_இலக்கம்_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_INDEX_இலக்கம்_ENTRIES]; \
         (idx) < (mods_)->இலக்கம்_mods; \
         (idx)++, (iter)++)

#define xkb_leds_foreach(iter, keymap) \
    க்கு ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->இலக்கம்_leds; \
         (iter)++)

#define xkb_leds_enumerate(idx, iter, keymap) \
    க்கு ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->இலக்கம்_leds; \
         (idx)++, (iter)++)

வெறுமை
clear_level(கட்டமைப்பு xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
நிலையான உள்வரி கட்டமைப்பு xkb_சாவி *
Xkb_சாவி_By_Name(மாறிலி கட்டமைப்பு xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    எனில் (name < keymap->இலக்கம்_சாவி_names) {
        மாறிலி KeycodeMatch match = keymap->சாவி_names[name];
        எனில் (match.found) {
            எனில் (!match.is_alias) {
                ப்ரதிஜ்ஞா(name == keymap->சாவிகள்[match.சாவி.index].name);
                திரும்பு &keymap->சாவிகள்[match.சாவி.index];
            } இல்லையெனில் எனில் (use_aliases) {
                ப்ரதிஜ்ஞா(match.alias.real ==
                       keymap->சாவிகள்[keymap->சாவி_names[match.alias.real].சாவி.index].name);
                திரும்பு &keymap->சாவிகள்[keymap->சாவி_names[match.alias.real].சாவி.index];
            }
        }
    }
    திரும்பு வெற்று;
}

நிலையான உள்வரி மாறிலி கட்டமைப்பு xkb_சாவி *
Xkb_சாவி(மாறிலி கட்டமைப்பு xkb_keymap *keymap, xkb_keycode_t kc)
{
    எனில் (kc < keymap->min_சாவி_code || kc > keymap->max_சாவி_code) {
        /* Unsupported keycode */
        திரும்பு வெற்று;
    } இல்லையெனில் எனில் (kc < keymap->இலக்கம்_சாவிகள்_கீழ்) {
        /* Low keycodes */
        திரும்பு &keymap->சாவிகள்[kc];
    } இல்லையெனில் {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->இலக்கம்_சாவிகள்_கீழ்;
        xkb_keycode_t upper = keymap->இலக்கம்_சாவிகள்;
        போது (lower < upper) {
            மாறிலி xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி சாவி = &keymap->சாவிகள்[mid];
            எனில் (சாவி->keycode < kc) {
                lower = mid + 1;
            } இல்லையெனில் எனில் (சாவி->keycode > kc) {
                upper = mid;
            } இல்லையெனில் {
                திரும்பு சாவி;
            }
        }
        திரும்பு வெற்று;
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
நிலையான உள்வரி மாறிலி கட்டமைப்பு xkb_சாவி *
xkb_keymap_get_next_defined_சாவி(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    எனில் (!keymap->இலக்கம்_சாவிகள் || kc > XKB_KEYCODE_MAX) {
        திரும்பு வெற்று;
    } இல்லையெனில் எனில் (kc < keymap->min_சாவி_code) {
        kc = keymap->min_சாவி_code;
    } இல்லையெனில் எனில் (kc > keymap->max_சாவி_code) {
        kc = keymap->max_சாவி_code;
    }

    எனில் (kc < keymap->இலக்கம்_சாவிகள்_கீழ்) {
        /* Low keycodes */
        மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = &keymap->சாவிகள்[kc];
        எனில் (ascending) {
            மாறிலி கட்டமைப்பு xkb_சாவி *end = &keymap->சாவிகள்[keymap->இலக்கம்_சாவிகள்_கீழ் - 1];
            போது (சாவி->name == XKB_ATOM_NONE && சாவி < end) சாவி++;
        } இல்லையெனில் {
            மாறிலி கட்டமைப்பு xkb_சாவி *end = &keymap->சாவிகள்[keymap->min_சாவி_code];
            போது (சாவி->name == XKB_ATOM_NONE && சாவி > end) சாவி--;
        }
        திரும்பு (சாவி->name == XKB_ATOM_NONE) ? வெற்று : சாவி;
    } இல்லையெனில் {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->இலக்கம்_சாவிகள்_கீழ்
            ? keymap->இலக்கம்_சாவிகள்_கீழ் - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->இலக்கம்_சாவிகள் - 1;
        மாறிலி கட்டமைப்பு xkb_சாவி * candidate = வெற்று;
        போது (lower <= upper) {
            மாறிலி xkb_keycode_t mid = lower + (upper - lower) / 2;
            மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி சாவி = &keymap->சாவிகள்[mid];
            எனில் (சாவி->keycode == kc) {
                திரும்பு சாவி;
            } இல்லையெனில் எனில் (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                எனில் (சாவி->keycode >= kc) {
                    candidate = சாவி;
                    upper = mid - 1;
                } இல்லையெனில் {
                    lower = mid + 1;
                }
            } இல்லையெனில் {
                /* Looking for the *smallest* defined keycode <= kc */
                எனில் (சாவி->keycode <= kc) {
                    candidate = சாவி;
                    lower = mid + 1;
                } இல்லையெனில் எனில் (சாவி->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        திரும்பு candidate;
    }
}

நிலையான உள்வரி xkb_level_index_t
Xkb_சாவி_இலக்கம்_Levels(மாறிலி கட்டமைப்பு xkb_சாவி *சாவி, xkb_layout_index_t layout)
{
    திரும்பு சாவி->groups[layout].type->இலக்கம்_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
நிலையான உள்வரி bool
entry_is_active(மாறிலி கட்டமைப்பு xkb_சாவி_type_entry *entry)
{
    திரும்பு entry->mods.mods == 0 || entry->mods.mask != 0;
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* func,
               எண்ணிக்கை xkb_keymap_format format,
               எண்ணிக்கை xkb_keymap_compile_flags flags);

வெறுமை
XkbEscapeMapName(எழுத்து *name);

xkb_mod_index_t
XkbModNameToIndex(மாறிலி கட்டமைப்பு xkb_mod_set *mods, xkb_atom_t name,
                  எண்ணிக்கை mod_type type);

bool
XkbLevelsSameSyms(மாறிலி கட்டமைப்பு xkb_level *a, மாறிலி கட்டமைப்பு xkb_level *b);

bool
action_equal(மாறிலி ஒன்றிப்பு xkb_action *a, மாறிலி ஒன்றிப்பு xkb_action *b);

bool
XkbLevelsSameActions(மாறிலி கட்டமைப்பு xkb_level *a, மாறிலி கட்டமைப்பு xkb_level *b);

xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t இலக்கம்_groups,
                      எண்ணிக்கை xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_எண்);

நிலையான உள்வரி xkb_layout_index_t
xkb_keymap_சாவி_effective_layout(மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                xkb_layout_index_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    திரும்பு XkbWrapGroupIntoRange((int32_t) layout,
                                 சாவி->இலக்கம்_groups,
                                 சாவி->out_of_range_group_policy,
                                 சாவி->out_of_range_group_எண்);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_mask_t mods);

கட்டமைப்பு xkb_level *
xkb_keymap_சாவி_get_level(கட்டமைப்பு xkb_keymap *keymap, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                         xkb_layout_index_t layout, xkb_level_index_t level);

xkb_action_மொத்தம்_t
xkb_keymap_சாவி_get_actions_by_level(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                                    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    மாறிலி ஒன்றிப்பு xkb_action **actions);

கட்டமைப்பு xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(கட்டமைப்பு xkb_keymap *keymap,
                                  மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(கட்டமைப்பு xkb_keymap *keymap,
                                  மாறிலி கட்டமைப்பு xkb_rule_names *names);
    bool (*keymap_new_from_string)(கட்டமைப்பு xkb_keymap *keymap,
                                   மாறிலி எழுத்து *string, size_t length);
    bool (*keymap_new_from_file)(கட்டமைப்பு xkb_keymap *keymap, FILE *file);
    எண்ணிக்கை xkb_status (*keymap_serialize)(
        மாறிலி கட்டமைப்பு xkb_keymap *keymap,
        மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
        கட்டமைப்பு xkb_keymap_serialize_result *result
    );
};

வெளி மாறிலி கட்டமைப்பு xkb_keymap_format_ops text_v1_keymap_format_ops;

நிலையான உள்வரி bool
isModsUnLockOnPressSupported(எண்ணிக்கை xkb_keymap_format format)
{
    /* Lax bound */
    திரும்பு format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

நிலையான உள்வரி bool
isGroupLockOnReleaseSupported(எண்ணிக்கை xkb_keymap_format format)
{
    /* Lax bound */
    திரும்பு format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

நிலையான உள்வரி bool
isModsLatchOnPressSupported(எண்ணிக்கை xkb_keymap_format format)
{
    /* Lax bound */
    திரும்பு format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

நிலையான உள்வரி bool
areOverlappingOverlaysSupported(எண்ணிக்கை xkb_keymap_format format)
{
    /* Lax bound */
    திரும்பு format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
