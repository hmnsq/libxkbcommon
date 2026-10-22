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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "rmlvo.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#define XKB_MAX_GROUPS 32
#define XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#define XKB_MAX_GROUPS_X11 4

സ്ഥിതം വരിയിൽ xkb_layout_index_t
format_max_groups(എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#define XKB_MAX_LEDS ((xkb_led_index_t) (വലുപ്പം(xkb_led_mask_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#define XKB_MAX_MODS ((xkb_mod_index_t) (വലുപ്പം(xkb_mod_mask_t) * CHAR_BIT))

എണ്ണൽ {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#define XKB_MOD_NONE 0xffffffffU

എണ്ണൽ mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#define MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
എണ്ണൽ real_mod_index {
    XKB_MOD_INDEX_SHIFT = 0,
    XKB_MOD_INDEX_CAPS,
    XKB_MOD_INDEX_CTRL,
    XKB_MOD_INDEX_MOD1,
    XKB_MOD_INDEX_MOD2,
    XKB_MOD_INDEX_MOD3,
    XKB_MOD_INDEX_MOD4,
    XKB_MOD_INDEX_MOD5,
    XKB_MOD_INDEX_അക്കം_ENTRIES
};
static_assert(XKB_MOD_INDEX_അക്കം_ENTRIES == 8, "Invalid X11 core modifiers");

എണ്ണൽ xkb_action_type {
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
    ACTION_TYPE_REDIRECT_താക്കോൽ,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_അക്കം_ENTRIES
};

എണ്ണൽ xkb_action_flags {
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

എണ്ണൽ {
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
എണ്ണൽ xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_താക്കോലുകൾ = (1 << 0),
    CONTROL_MOUSE_താക്കോലുകൾ = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_താക്കോലുകൾ_ACCEL = (1 << 15),
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
         CONTROL_STICKY_താക്കോലുകൾ | CONTROL_MOUSE_താക്കോലുകൾ | CONTROL_MOUSE_താക്കോലുകൾ_ACCEL | \
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
         CONTROL_STICKY_താക്കോലുകൾ | CONTROL_MOUSE_താക്കോലുകൾ | CONTROL_MOUSE_താക്കോലുകൾ_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(വലുപ്പം(എണ്ണൽ xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_action_controls
format_boolean_controls(എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_താക്കോലുകൾ ==
    (എണ്ണൽ xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_താക്കോലുകൾ ==
    (എണ്ണൽ xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ,
    "Private value should match public API"
);


എണ്ണൽ xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

ഘടന xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

ഘടന xkb_mod_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    ഘടന xkb_mods mods;
};

ഘടന xkb_group_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
തരനിർവചനം uint8_t xkb_overlay_index_t;
/** Keyboard overlay mask */
തരനിർവചനം uint8_t xkb_overlay_mask_t;
എണ്ണൽ {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_എണ്ണം_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_എണ്ണം = (വലുപ്പം(xkb_overlay_mask_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_എണ്ണം - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_INDEX_MIN_WIDTH = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_എണ്ണം_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_എണ്ണം) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_എണ്ണം ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_എണ്ണം < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_എണ്ണം <= (1u << XKB_OVERLAY_INDEX_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

സ്ഥിതം വരിയിൽ xkb_overlay_mask_t
overlays_from_controls(എണ്ണൽ xkb_action_controls controls)
{
    തിരികെ (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    തിരികെ (എണ്ണൽ xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

സ്ഥിതം വരിയിൽ xkb_overlay_index_t
format_max_overlays(എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_എണ്ണം_X11
        : XKB_OVERLAY_എണ്ണം;
}

ഘടന xkb_controls_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    എണ്ണൽ xkb_action_controls ctrls;
};

തരനിർവചനം uint8_t xkb_pointer_button_index_t;
തരനിർവചനം uint8_t xkb_pointer_button_mask_t;

എണ്ണൽ {
    XKB_POINTER_BUTTON_INDEX_WIDTH =
        വലുപ്പം(xkb_pointer_button_index_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_INDEX_MAX =
        (1 << XKB_POINTER_BUTTON_INDEX_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        വലുപ്പം(xkb_pointer_button_mask_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_INDEX_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

ഘടന xkb_pointer_default_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    int8_t മൂല്യം;
};

എണ്ണൽ {
    XKB_POINTER_DEFAULT_ACTION_മൂല്യം_WIDTH =
        വലുപ്പം(((ഘടന xkb_pointer_default_action*)0)->മൂല്യം) * CHAR_BIT,
    XKB_POINTER_DEFAULT_ACTION_മൂല്യം_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_മൂല്യം_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_മൂല്യം_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

ഘടന xkb_pointer_motion_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

ഘടന xkb_pointer_button_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    uint8_t എണ്ണം;
    xkb_pointer_button_index_t button;
};

ഘടന xkb_switch_screen_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_action_flags flags;
    int8_t screen;
};

ഘടന xkb_redirect_താക്കോൽ_action {
    എണ്ണൽ xkb_action_type type;
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

ഘടന xkb_private_action {
    എണ്ണൽ xkb_action_type type;
    uint8_t data[7];
};

എണ്ണൽ xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
ഘടന xkb_internal_action {
    എണ്ണൽ xkb_action_type type;
    എണ്ണൽ xkb_internal_action_flags flags;
    സംഘം {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

സംഘം xkb_action {
    എണ്ണൽ xkb_action_type type;
    ഘടന xkb_mod_action mods;
    ഘടന xkb_group_action group;
    ഘടന xkb_controls_action ctrls;
    ഘടന xkb_redirect_താക്കോൽ_action redirect;
    ഘടന xkb_pointer_default_action dflt;
    ഘടന xkb_pointer_motion_action ptr;
    ഘടന xkb_pointer_button_action btn;
    ഘടന xkb_switch_screen_action screen;
    ഘടന xkb_private_action priv;
    ഘടന xkb_internal_action internal;
};

ഘടന xkb_താക്കോൽ_type_entry {
    xkb_level_index_t level;
    ഘടന xkb_mods mods;
    ഘടന xkb_mods preserve;
};

ഘടന xkb_താക്കോൽ_type {
    xkb_atom_t name;
    ഘടന xkb_mods mods;
    bool required;
    xkb_level_index_t അക്കം_levels;
    xkb_level_index_t അക്കം_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(അക്കം_level_names);
    darray_size_t അക്കം_entries;
    ഘടന xkb_താക്കോൽ_type_entry *entries ATTR_COUNTED_BY(അക്കം_entries);
};

തരനിർവചനം uint16_t xkb_action_എണ്ണം_t;
#define MAX_ACTIONS_PER_LEVEL UINT16_MAX

ഘടന xkb_sym_interpret {
    xkb_keysym_t sym;
    എണ്ണൽ xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_index_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_എണ്ണം_t അക്കം_actions;
    സംഘം {
        /* num_actions <= 1 */
        സംഘം xkb_action action;
        /* num_actions >  1 */
        സംഘം xkb_action *actions;
    } a;
};

എണ്ണൽ {XKB_STATE_COMPONENT_WIDTH = (വലുപ്പം(എണ്ണൽ xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

ഘടന xkb_led {
    xkb_atom_t name;
    എണ്ണൽ xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    എണ്ണൽ xkb_state_component which_mods;
    ഘടന xkb_mods mods;
    എണ്ണൽ xkb_action_controls ctrls;
};

ഘടന xkb_താക്കോൽ_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

ഘടന xkb_controls {
    unsigned അക്ഷരം groups_wrap;
    ഘടന xkb_mods internal;
    ഘടന xkb_mods ignore_lock;
    unsigned short repeat_delay;
    unsigned short repeat_interval;
    unsigned short slow_താക്കോലുകൾ_delay;
    unsigned short debounce_delay;
    unsigned short ax_options;
    unsigned short ax_timeout;
    unsigned short axt_opts_mask;
    unsigned short axt_opts_മൂല്യങ്ങൾ;
    unsigned പൂർണ്ണസംഖ്യ axt_ctrls_mask;
    unsigned പൂർണ്ണസംഖ്യ axt_ctrls_മൂല്യങ്ങൾ;
};

എണ്ണൽ xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

തരനിർവചനം uint16_t xkb_keysym_എണ്ണം_t;
#define MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
ഘടന xkb_level {
    /** Count of keysyms */
    xkb_keysym_എണ്ണം_t അക്കം_syms;
    /** Count of actions */
    xkb_action_എണ്ണം_t അക്കം_actions;
    /** Upper keysym */
    സംഘം {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    സംഘം {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    സംഘം {
        സംഘം xkb_action action;   /* num_actions == 1 */
        സംഘം xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
ഘടന xkb_group {
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
    സ്ഥിരം ഘടന xkb_താക്കോൽ_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    ഘടന xkb_level *levels;
};

എണ്ണൽ {
    DEFAULT_താക്കോൽ_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_താക്കോൽ_REPEAT = true,
    FALLBACK_INTERPRET_താക്കോൽ_REPEAT = DEFAULT_താക്കോൽ_REPEAT
};

എണ്ണൽ {
    DEFAULT_താക്കോൽ_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_താക്കോൽ_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_താക്കോൽ_VMODMAP
};

ഘടന xkb_താക്കോൽ {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    എണ്ണൽ xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    എണ്ണൽ xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_index_t out_of_range_group_സംഖ്യ:8;

    xkb_layout_index_t അക്കം_groups:8;
    ഘടന xkb_group *groups ATTR_COUNTED_BY(അക്കം_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    സംഘം {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        സ്ഥിരം ഘടന xkb_താക്കോൽ *overlay_താക്കോൽ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        സ്ഥിരം ഘടന xkb_താക്കോൽ **overlays_താക്കോലുകൾ;
    };
};

ഘടന xkb_mod {
    xkb_atom_t name;
    എണ്ണൽ mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

ഘടന xkb_mod_set {
    ഘടന xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_index_t അക്കം_mods;
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
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(വലുപ്പം(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(വലുപ്പം(ഘടന xkb_താക്കോൽ)),
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
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(വലുപ്പം(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(വലുപ്പം(ഘടന xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_SIZE_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
തരനിർവചനം സംഘം {
    ഘടന {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_size_t:(DARRAY_SIZE_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    ഘടന {
        bool found:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool താഴെയുള്ള:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_size_t index:(DARRAY_SIZE_T_WIDTH - 3);
    } താക്കോൽ;
    /* Only if is_alias = true, for better semantics */
    ഘടന {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_SIZE_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

തരനിർവചനം ഘടന {
    സംഘം {
        /** Used for parsing */
        എണ്ണൽ merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t താക്കോൽ_Code;
    };
    bool have_ചിഹ്നം;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    സംഘം {
        xkb_keysym_t താക്കോൽ_Sym;
        xkb_atom_t താക്കോൽ_Name;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
ഘടന xkb_keymap {
    ഘടന xkb_context *ctx;

    പൂർണ്ണസംഖ്യ refcnt;
    എണ്ണൽ xkb_keymap_compile_flags flags;
    എണ്ണൽ xkb_keymap_format format;

    xkb_led_index_t അക്കം_leds;
    ഘടന xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_താക്കോൽ_code;
    xkb_keycode_t max_താക്കോൽ_code;
    xkb_keycode_t അക്കം_താക്കോലുകൾ;
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
    xkb_keycode_t അക്കം_താക്കോലുകൾ_താഴെയുള്ള;
    ഘടന xkb_താക്കോൽ *താക്കോലുകൾ ATTR_COUNTED_BY(അക്കം_താക്കോലുകൾ);

    സംഘം {
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
        ഘടന {
            darray_size_t അക്കം_താക്കോൽ_names;
            KeycodeMatch *താക്കോൽ_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        ഘടന {
            darray_size_t അക്കം_താക്കോൽ_aliases;
            ഘടന xkb_താക്കോൽ_alias *താക്കോൽ_aliases;
        };
    };

    ഘടന xkb_താക്കോൽ_type *types;
    darray_size_t അക്കം_types;

    darray_size_t അക്കം_sym_interprets;
    ഘടന xkb_sym_interpret *sym_interprets;

    darray_size_t _padding;
    darray_size_t അക്കം_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    ഘടന xkb_mod_set mods;
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
    xkb_keycode_t redirect_താക്കോൽ_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_index_t അക്കം_groups;
    /* Not all groups must have names. */
    xkb_layout_index_t അക്കം_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(അക്കം_group_names);

    അക്ഷരം *keycodes_section_name;
    അക്ഷരം *symbols_section_name;
    അക്ഷരം *types_section_name;
    അക്ഷരം *compat_section_name;
};

#define xkb_താക്കോലുകൾ_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    വേണ്ടി ((iter) = (keymap)->താക്കോലുകൾ + \
                  ((keymap)->അക്കം_താക്കോലുകൾ_താഴെയുള്ള == 0 ? 0 : (keymap)->min_താക്കോൽ_code); \
         (iter) < (keymap)->താക്കോലുകൾ + (keymap)->അക്കം_താക്കോലുകൾ; \
         (iter)++)

#define xkb_level_foreach_action(iter, level)                 \
    വേണ്ടി (സംഘം xkb_action                                     \
         *(iter) = (likely((level)->അക്കം_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * സ്ഥിരം iter##_last_ = (iter) + (level)->അക്കം_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#define xkb_mods_foreach(iter, mods_) \
    വേണ്ടി ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->അക്കം_mods; \
         (iter)++)

#define xkb_mods_mask_foreach(mask, iter, mods_) \
    വേണ്ടി ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->അക്കം_mods; \
         (iter)++, (mask) >>= 1) \
        എങ്കിൽ ((mask) & 0x1)

/** Enumerate all modifiers */
#define xkb_mods_enumerate(idx, iter, mods_) \
    വേണ്ടി ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->അക്കം_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#define xkb_rmods_enumerate(idx, iter, mods_) \
    വേണ്ടി ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_INDEX_അക്കം_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#define xkb_vmods_enumerate(idx, iter, mods_) \
    വേണ്ടി ((idx) = XKB_MOD_INDEX_അക്കം_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_INDEX_അക്കം_ENTRIES]; \
         (idx) < (mods_)->അക്കം_mods; \
         (idx)++, (iter)++)

#define xkb_leds_foreach(iter, keymap) \
    വേണ്ടി ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->അക്കം_leds; \
         (iter)++)

#define xkb_leds_enumerate(idx, iter, keymap) \
    വേണ്ടി ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->അക്കം_leds; \
         (idx)++, (iter)++)

ശൂന്യം
clear_level(ഘടന xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
സ്ഥിതം വരിയിൽ ഘടന xkb_താക്കോൽ *
Xkb_താക്കോൽ_By_Name(സ്ഥിരം ഘടന xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    എങ്കിൽ (name < keymap->അക്കം_താക്കോൽ_names) {
        സ്ഥിരം KeycodeMatch match = keymap->താക്കോൽ_names[name];
        എങ്കിൽ (match.found) {
            എങ്കിൽ (!match.is_alias) {
                പ്രതിജ്ഞാ(name == keymap->താക്കോലുകൾ[match.താക്കോൽ.index].name);
                തിരികെ &keymap->താക്കോലുകൾ[match.താക്കോൽ.index];
            } അല്ലെങ്കിൽ എങ്കിൽ (use_aliases) {
                പ്രതിജ്ഞാ(match.alias.real ==
                       keymap->താക്കോലുകൾ[keymap->താക്കോൽ_names[match.alias.real].താക്കോൽ.index].name);
                തിരികെ &keymap->താക്കോലുകൾ[keymap->താക്കോൽ_names[match.alias.real].താക്കോൽ.index];
            }
        }
    }
    തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന xkb_താക്കോൽ *
Xkb_താക്കോൽ(സ്ഥിരം ഘടന xkb_keymap *keymap, xkb_keycode_t kc)
{
    എങ്കിൽ (kc < keymap->min_താക്കോൽ_code || kc > keymap->max_താക്കോൽ_code) {
        /* Unsupported keycode */
        തിരികെ ശൂന്യ;
    } അല്ലെങ്കിൽ എങ്കിൽ (kc < keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള) {
        /* Low keycodes */
        തിരികെ &keymap->താക്കോലുകൾ[kc];
    } അല്ലെങ്കിൽ {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള;
        xkb_keycode_t upper = keymap->അക്കം_താക്കോലുകൾ;
        ആകുമ്പോൾ (lower < upper) {
            സ്ഥിരം xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം താക്കോൽ = &keymap->താക്കോലുകൾ[mid];
            എങ്കിൽ (താക്കോൽ->keycode < kc) {
                lower = mid + 1;
            } അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->keycode > kc) {
                upper = mid;
            } അല്ലെങ്കിൽ {
                തിരികെ താക്കോൽ;
            }
        }
        തിരികെ ശൂന്യ;
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
സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന xkb_താക്കോൽ *
xkb_keymap_get_next_defined_താക്കോൽ(സ്ഥിരം ഘടന xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    എങ്കിൽ (!keymap->അക്കം_താക്കോലുകൾ || kc > XKB_KEYCODE_MAX) {
        തിരികെ ശൂന്യ;
    } അല്ലെങ്കിൽ എങ്കിൽ (kc < keymap->min_താക്കോൽ_code) {
        kc = keymap->min_താക്കോൽ_code;
    } അല്ലെങ്കിൽ എങ്കിൽ (kc > keymap->max_താക്കോൽ_code) {
        kc = keymap->max_താക്കോൽ_code;
    }

    എങ്കിൽ (kc < keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള) {
        /* Low keycodes */
        സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = &keymap->താക്കോലുകൾ[kc];
        എങ്കിൽ (ascending) {
            സ്ഥിരം ഘടന xkb_താക്കോൽ *end = &keymap->താക്കോലുകൾ[keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള - 1];
            ആകുമ്പോൾ (താക്കോൽ->name == XKB_ATOM_NONE && താക്കോൽ < end) താക്കോൽ++;
        } അല്ലെങ്കിൽ {
            സ്ഥിരം ഘടന xkb_താക്കോൽ *end = &keymap->താക്കോലുകൾ[keymap->min_താക്കോൽ_code];
            ആകുമ്പോൾ (താക്കോൽ->name == XKB_ATOM_NONE && താക്കോൽ > end) താക്കോൽ--;
        }
        തിരികെ (താക്കോൽ->name == XKB_ATOM_NONE) ? ശൂന്യ : താക്കോൽ;
    } അല്ലെങ്കിൽ {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള
            ? keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->അക്കം_താക്കോലുകൾ - 1;
        സ്ഥിരം ഘടന xkb_താക്കോൽ * candidate = ശൂന്യ;
        ആകുമ്പോൾ (lower <= upper) {
            സ്ഥിരം xkb_keycode_t mid = lower + (upper - lower) / 2;
            സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം താക്കോൽ = &keymap->താക്കോലുകൾ[mid];
            എങ്കിൽ (താക്കോൽ->keycode == kc) {
                തിരികെ താക്കോൽ;
            } അല്ലെങ്കിൽ എങ്കിൽ (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                എങ്കിൽ (താക്കോൽ->keycode >= kc) {
                    candidate = താക്കോൽ;
                    upper = mid - 1;
                } അല്ലെങ്കിൽ {
                    lower = mid + 1;
                }
            } അല്ലെങ്കിൽ {
                /* Looking for the *smallest* defined keycode <= kc */
                എങ്കിൽ (താക്കോൽ->keycode <= kc) {
                    candidate = താക്കോൽ;
                    lower = mid + 1;
                } അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        തിരികെ candidate;
    }
}

സ്ഥിതം വരിയിൽ xkb_level_index_t
Xkb_താക്കോൽ_അക്കം_Levels(സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ, xkb_layout_index_t layout)
{
    തിരികെ താക്കോൽ->groups[layout].type->അക്കം_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
സ്ഥിതം വരിയിൽ bool
entry_is_active(സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry *entry)
{
    തിരികെ entry->mods.mods == 0 || entry->mods.mask != 0;
}

ഘടന xkb_keymap *
xkb_keymap_new(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* func,
               എണ്ണൽ xkb_keymap_format format,
               എണ്ണൽ xkb_keymap_compile_flags flags);

ശൂന്യം
XkbEscapeMapName(അക്ഷരം *name);

xkb_mod_index_t
XkbModNameToIndex(സ്ഥിരം ഘടന xkb_mod_set *mods, xkb_atom_t name,
                  എണ്ണൽ mod_type type);

bool
XkbLevelsSameSyms(സ്ഥിരം ഘടന xkb_level *a, സ്ഥിരം ഘടന xkb_level *b);

bool
action_equal(സ്ഥിരം സംഘം xkb_action *a, സ്ഥിരം സംഘം xkb_action *b);

bool
XkbLevelsSameActions(സ്ഥിരം ഘടന xkb_level *a, സ്ഥിരം ഘടന xkb_level *b);

xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t അക്കം_groups,
                      എണ്ണൽ xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_സംഖ്യ);

സ്ഥിതം വരിയിൽ xkb_layout_index_t
xkb_keymap_താക്കോൽ_effective_layout(സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                xkb_layout_index_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    തിരികെ XkbWrapGroupIntoRange((int32_t) layout,
                                 താക്കോൽ->അക്കം_groups,
                                 താക്കോൽ->out_of_range_group_policy,
                                 താക്കോൽ->out_of_range_group_സംഖ്യ);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(ഘടന xkb_keymap *keymap, xkb_mod_mask_t mods);

ഘടന xkb_level *
xkb_keymap_താക്കോൽ_get_level(ഘടന xkb_keymap *keymap, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                         xkb_layout_index_t layout, xkb_level_index_t level);

xkb_action_എണ്ണം_t
xkb_keymap_താക്കോൽ_get_actions_by_level(സ്ഥിരം ഘടന xkb_keymap *keymap,
                                    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    സ്ഥിരം സംഘം xkb_action **actions);

ഘടന xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(ഘടന xkb_keymap *keymap,
                                  സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(ഘടന xkb_keymap *keymap,
                                  സ്ഥിരം ഘടന xkb_rule_names *names);
    bool (*keymap_new_from_string)(ഘടന xkb_keymap *keymap,
                                   സ്ഥിരം അക്ഷരം *string, size_t length);
    bool (*keymap_new_from_file)(ഘടന xkb_keymap *keymap, FILE *file);
    എണ്ണൽ xkb_status (*keymap_serialize)(
        സ്ഥിരം ഘടന xkb_keymap *keymap,
        സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
        ഘടന xkb_keymap_serialize_result *result
    );
};

ബാഹ്യ സ്ഥിരം ഘടന xkb_keymap_format_ops text_v1_keymap_format_ops;

സ്ഥിതം വരിയിൽ bool
isModsUnLockOnPressSupported(എണ്ണൽ xkb_keymap_format format)
{
    /* Lax bound */
    തിരികെ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

സ്ഥിതം വരിയിൽ bool
isGroupLockOnReleaseSupported(എണ്ണൽ xkb_keymap_format format)
{
    /* Lax bound */
    തിരികെ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

സ്ഥിതം വരിയിൽ bool
isModsLatchOnPressSupported(എണ്ണൽ xkb_keymap_format format)
{
    /* Lax bound */
    തിരികെ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

സ്ഥിതം വരിയിൽ bool
areOverlappingOverlaysSupported(എണ്ണൽ xkb_keymap_format format)
{
    /* Lax bound */
    തിരികെ format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
