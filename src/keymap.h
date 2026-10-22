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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "atom.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "rmlvo.h"
#ضمن "utils.h"
#ضمن "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#define XKB_MAX_GROUPS 32
#define XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#define XKB_MAX_GROUPS_X11 4

ساكن مضمن xkb_layout_index_t
format_max_groups(تعداد xkb_keymap_format format)
{
    החזר (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#define XKB_MAX_LEDS ((xkb_led_index_t) (حجم(xkb_led_mask_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#define XKB_MAX_MODS ((xkb_mod_index_t) (حجم(xkb_mod_mask_t) * CHAR_BIT))

تعداد {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#define XKB_MOD_NONE 0xffffffffU

تعداد mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#define MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
تعداد real_mod_index {
    XKB_MOD_INDEX_SHIFT = 0,
    XKB_MOD_INDEX_CAPS,
    XKB_MOD_INDEX_CTRL,
    XKB_MOD_INDEX_MOD1,
    XKB_MOD_INDEX_MOD2,
    XKB_MOD_INDEX_MOD3,
    XKB_MOD_INDEX_MOD4,
    XKB_MOD_INDEX_MOD5,
    _XKB_MOD_INDEX_NUM_ENTRIES
};
static_assert(_XKB_MOD_INDEX_NUM_ENTRIES == 8, "Invalid X11 core modifiers");

تعداد xkb_action_type {
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
    ACTION_TYPE_REDIRECT_KEY,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    _ACTION_TYPE_NUM_ENTRIES
};

تعداد xkb_action_flags {
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

تعداد {
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
تعداد xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_KEYS = (1 << 0),
    CONTROL_MOUSE_KEYS = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_KEYS_ACCEL = (1 << 15),
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
         CONTROL_STICKY_KEYS | CONTROL_MOUSE_KEYS | CONTROL_MOUSE_KEYS_ACCEL | \
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
         CONTROL_STICKY_KEYS | CONTROL_MOUSE_KEYS | CONTROL_MOUSE_KEYS_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(حجم(تعداد xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ساكن مضمن تعداد xkb_action_controls
format_boolean_controls(تعداد xkb_keymap_format format)
{
    החזר (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_KEYS ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_KEYS ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_KEYS,
    "Private value should match public API"
);


تعداد xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

بنية xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

بنية xkb_mod_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    بنية xkb_mods mods;
};

بنية xkb_group_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
تعريفنوع uint8_t xkb_overlay_index_t;
/** Keyboard overlay mask */
تعريفنوع uint8_t xkb_overlay_mask_t;
تعداد {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_COUNT_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_COUNT = (حجم(xkb_overlay_mask_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_COUNT - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_INDEX_MIN_WIDTH = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_COUNT_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_COUNT) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_COUNT ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_COUNT < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_COUNT <= (1u << XKB_OVERLAY_INDEX_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

ساكن مضمن xkb_overlay_mask_t
overlays_from_controls(تعداد xkb_action_controls controls)
{
    החזר (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ساكن مضمن تعداد xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    החזר (تعداد xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ساكن مضمن xkb_overlay_index_t
format_max_overlays(تعداد xkb_keymap_format format)
{
    החזר (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_COUNT_X11
        : XKB_OVERLAY_COUNT;
}

بنية xkb_controls_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    تعداد xkb_action_controls ctrls;
};

تعريفنوع uint8_t xkb_pointer_button_index_t;
تعريفنوع uint8_t xkb_pointer_button_mask_t;

تعداد {
    XKB_POINTER_BUTTON_INDEX_WIDTH =
        حجم(xkb_pointer_button_index_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_INDEX_MAX =
        (1 << XKB_POINTER_BUTTON_INDEX_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        حجم(xkb_pointer_button_mask_t) * CHAR_BIT,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_INDEX_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

بنية xkb_pointer_default_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t value;
};

تعداد {
    XKB_POINTER_DEFAULT_ACTION_VALUE_WIDTH =
        حجم(((بنية xkb_pointer_default_action*)0)->value) * CHAR_BIT,
    XKB_POINTER_DEFAULT_ACTION_VALUE_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_VALUE_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_VALUE_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

بنية xkb_pointer_motion_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

بنية xkb_pointer_button_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    uint8_t count;
    xkb_pointer_button_index_t button;
};

بنية xkb_switch_screen_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t screen;
};

بنية xkb_redirect_key_action {
    تعداد xkb_action_type type;
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

بنية xkb_private_action {
    تعداد xkb_action_type type;
    uint8_t data[7];
};

تعداد xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
بنية xkb_internal_action {
    تعداد xkb_action_type type;
    تعداد xkb_internal_action_flags flags;
    اتحاد {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

اتحاد xkb_action {
    تعداد xkb_action_type type;
    بنية xkb_mod_action mods;
    بنية xkb_group_action group;
    بنية xkb_controls_action ctrls;
    بنية xkb_redirect_key_action redirect;
    بنية xkb_pointer_default_action dflt;
    بنية xkb_pointer_motion_action ptr;
    بنية xkb_pointer_button_action btn;
    بنية xkb_switch_screen_action screen;
    بنية xkb_private_action priv;
    بنية xkb_internal_action internal;
};

بنية xkb_key_type_entry {
    xkb_level_index_t level;
    بنية xkb_mods mods;
    بنية xkb_mods preserve;
};

بنية xkb_key_type {
    xkb_atom_t name;
    بنية xkb_mods mods;
    bool required;
    xkb_level_index_t num_levels;
    xkb_level_index_t num_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(num_level_names);
    darray_size_t num_entries;
    بنية xkb_key_type_entry *entries ATTR_COUNTED_BY(num_entries);
};

تعريفنوع uint16_t xkb_action_count_t;
#define MAX_ACTIONS_PER_LEVEL UINT16_MAX

بنية xkb_sym_interpret {
    xkb_keysym_t sym;
    تعداد xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_index_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_count_t num_actions;
    اتحاد {
        /* num_actions <= 1 */
        اتحاد xkb_action action;
        /* num_actions >  1 */
        اتحاد xkb_action *actions;
    } a;
};

تعداد {XKB_STATE_COMPONENT_WIDTH = (حجم(تعداد xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

بنية xkb_led {
    xkb_atom_t name;
    تعداد xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    تعداد xkb_state_component which_mods;
    بنية xkb_mods mods;
    تعداد xkb_action_controls ctrls;
};

بنية xkb_key_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

بنية xkb_controls {
    unsigned حرف groups_wrap;
    بنية xkb_mods internal;
    بنية xkb_mods ignore_lock;
    unsigned short repeat_delay;
    unsigned short repeat_interval;
    unsigned short slow_keys_delay;
    unsigned short debounce_delay;
    unsigned short ax_options;
    unsigned short ax_timeout;
    unsigned short axt_opts_mask;
    unsigned short axt_opts_values;
    unsigned صحيح axt_ctrls_mask;
    unsigned صحيح axt_ctrls_values;
};

تعداد xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

تعريفنوع uint16_t xkb_keysym_count_t;
#define MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
بنية xkb_level {
    /** Count of keysyms */
    xkb_keysym_count_t num_syms;
    /** Count of actions */
    xkb_action_count_t num_actions;
    /** Upper keysym */
    اتحاد {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    اتحاد {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    اتحاد {
        اتحاد xkb_action action;   /* num_actions == 1 */
        اتحاد xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
بنية xkb_group {
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
    ثابت بنية xkb_key_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    بنية xkb_level *levels;
};

تعداد {
    DEFAULT_KEY_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_KEY_REPEAT = true,
    FALLBACK_INTERPRET_KEY_REPEAT = DEFAULT_KEY_REPEAT
};

تعداد {
    DEFAULT_KEY_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_KEY_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_KEY_VMODMAP
};

بنية xkb_key {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    تعداد xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    تعداد xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_index_t out_of_range_group_number:8;

    xkb_layout_index_t num_groups:8;
    بنية xkb_group *groups ATTR_COUNTED_BY(num_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ثابت بنية xkb_key *overlay_key;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_key **overlays_keys;
    };
};

بنية xkb_mod {
    xkb_atom_t name;
    تعداد mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

بنية xkb_mod_set {
    بنية xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_index_t num_mods;
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
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(بنية xkb_key)),
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
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(بنية xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_SIZE_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
تعريفنوع اتحاد {
    بنية {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_size_t:(DARRAY_SIZE_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    بنية {
        bool found:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool low:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_size_t index:(DARRAY_SIZE_T_WIDTH - 3);
    } key;
    /* Only if is_alias = true, for better semantics */
    بنية {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_SIZE_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

تعريفنوع بنية {
    اتحاد {
        /** Used for parsing */
        تعداد merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t keyCode;
    };
    bool haveSymbol;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    اتحاد {
        xkb_keysym_t keySym;
        xkb_atom_t keyName;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
بنية xkb_keymap {
    بنية xkb_context *ctx;

    صحيح refcnt;
    تعداد xkb_keymap_compile_flags flags;
    تعداد xkb_keymap_format format;

    xkb_led_index_t num_leds;
    بنية xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_key_code;
    xkb_keycode_t max_key_code;
    xkb_keycode_t num_keys;
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
    xkb_keycode_t num_keys_low;
    بنية xkb_key *keys ATTR_COUNTED_BY(num_keys);

    اتحاد {
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
        بنية {
            darray_size_t num_key_names;
            KeycodeMatch *key_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        بنية {
            darray_size_t num_key_aliases;
            بنية xkb_key_alias *key_aliases;
        };
    };

    بنية xkb_key_type *types;
    darray_size_t num_types;

    darray_size_t num_sym_interprets;
    بنية xkb_sym_interpret *sym_interprets;

    darray_size_t _padding;
    darray_size_t num_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    بنية xkb_mod_set mods;
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
    xkb_keycode_t redirect_key_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_index_t num_groups;
    /* Not all groups must have names. */
    xkb_layout_index_t num_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(num_group_names);

    حرف *keycodes_section_name;
    حرف *symbols_section_name;
    حرف *types_section_name;
    حرف *compat_section_name;
};

#define xkb_keys_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    עבור ((iter) = (keymap)->keys + \
                  ((keymap)->num_keys_low == 0 ? 0 : (keymap)->min_key_code); \
         (iter) < (keymap)->keys + (keymap)->num_keys; \
         (iter)++)

#define xkb_level_foreach_action(iter, level)                 \
    עבור (اتحاد xkb_action                                     \
         *(iter) = (likely((level)->num_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ثابت iter##_last_ = (iter) + (level)->num_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#define xkb_mods_foreach(iter, mods_) \
    עבור ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->num_mods; \
         (iter)++)

#define xkb_mods_mask_foreach(mask, iter, mods_) \
    עבור ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->num_mods; \
         (iter)++, (mask) >>= 1) \
        אם ((mask) & 0x1)

/** Enumerate all modifiers */
#define xkb_mods_enumerate(idx, iter, mods_) \
    עבור ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->num_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#define xkb_rmods_enumerate(idx, iter, mods_) \
    עבור ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < _XKB_MOD_INDEX_NUM_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#define xkb_vmods_enumerate(idx, iter, mods_) \
    עבור ((idx) = _XKB_MOD_INDEX_NUM_ENTRIES, \
         (iter) = &(mods_)->mods[_XKB_MOD_INDEX_NUM_ENTRIES]; \
         (idx) < (mods_)->num_mods; \
         (idx)++, (iter)++)

#define xkb_leds_foreach(iter, keymap) \
    עבור ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->num_leds; \
         (iter)++)

#define xkb_leds_enumerate(idx, iter, keymap) \
    עבור ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->num_leds; \
         (idx)++, (iter)++)

فراغ
clear_level(بنية xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ساكن مضمن بنية xkb_key *
XkbKeyByName(ثابت بنية xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    אם (name < keymap->num_key_names) {
        ثابت KeycodeMatch match = keymap->key_names[name];
        אם (match.found) {
            אם (!match.is_alias) {
                أكد(name == keymap->keys[match.key.index].name);
                החזר &keymap->keys[match.key.index];
            } אחרת אם (use_aliases) {
                أكد(match.alias.real ==
                       keymap->keys[keymap->key_names[match.alias.real].key.index].name);
                החזר &keymap->keys[keymap->key_names[match.alias.real].key.index];
            }
        }
    }
    החזר عدم;
}

ساكن مضمن ثابت بنية xkb_key *
XkbKey(ثابت بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    אם (kc < keymap->min_key_code || kc > keymap->max_key_code) {
        /* Unsupported keycode */
        החזר عدم;
    } אחרת אם (kc < keymap->num_keys_low) {
        /* Low keycodes */
        החזר &keymap->keys[kc];
    } אחרת {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->num_keys_low;
        xkb_keycode_t upper = keymap->num_keys;
        כלעוד (lower < upper) {
            ثابت xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            ثابت بنية xkb_key * ثابت key = &keymap->keys[mid];
            אם (key->keycode < kc) {
                lower = mid + 1;
            } אחרת אם (key->keycode > kc) {
                upper = mid;
            } אחרת {
                החזר key;
            }
        }
        החזר عدم;
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
ساكن مضمن ثابت بنية xkb_key *
xkb_keymap_get_next_defined_key(ثابت بنية xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    אם (!keymap->num_keys || kc > XKB_KEYCODE_MAX) {
        החזר عدم;
    } אחרת אם (kc < keymap->min_key_code) {
        kc = keymap->min_key_code;
    } אחרת אם (kc > keymap->max_key_code) {
        kc = keymap->max_key_code;
    }

    אם (kc < keymap->num_keys_low) {
        /* Low keycodes */
        ثابت بنية xkb_key *key = &keymap->keys[kc];
        אם (ascending) {
            ثابت بنية xkb_key *end = &keymap->keys[keymap->num_keys_low - 1];
            כלעוד (key->name == XKB_ATOM_NONE && key < end) key++;
        } אחרת {
            ثابت بنية xkb_key *end = &keymap->keys[keymap->min_key_code];
            כלעוד (key->name == XKB_ATOM_NONE && key > end) key--;
        }
        החזר (key->name == XKB_ATOM_NONE) ? عدم : key;
    } אחרת {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->num_keys_low
            ? keymap->num_keys_low - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->num_keys - 1;
        ثابت بنية xkb_key * candidate = عدم;
        כלעוד (lower <= upper) {
            ثابت xkb_keycode_t mid = lower + (upper - lower) / 2;
            ثابت بنية xkb_key * ثابت key = &keymap->keys[mid];
            אם (key->keycode == kc) {
                החזר key;
            } אחרת אם (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                אם (key->keycode >= kc) {
                    candidate = key;
                    upper = mid - 1;
                } אחרת {
                    lower = mid + 1;
                }
            } אחרת {
                /* Looking for the *smallest* defined keycode <= kc */
                אם (key->keycode <= kc) {
                    candidate = key;
                    lower = mid + 1;
                } אחרת אם (key->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        החזר candidate;
    }
}

ساكن مضمن xkb_level_index_t
XkbKeyNumLevels(ثابت بنية xkb_key *key, xkb_layout_index_t layout)
{
    החזר key->groups[layout].type->num_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ساكن مضمن bool
entry_is_active(ثابت بنية xkb_key_type_entry *entry)
{
    החזר entry->mods.mods == 0 || entry->mods.mask != 0;
}

بنية xkb_keymap *
xkb_keymap_new(بنية xkb_context *ctx, ثابت حرف* func,
               تعداد xkb_keymap_format format,
               تعداد xkb_keymap_compile_flags flags);

فراغ
XkbEscapeMapName(حرف *name);

xkb_mod_index_t
XkbModNameToIndex(ثابت بنية xkb_mod_set *mods, xkb_atom_t name,
                  تعداد mod_type type);

bool
XkbLevelsSameSyms(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b);

bool
XkbLevelsSameActions(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t num_groups,
                      تعداد xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_number);

ساكن مضمن xkb_layout_index_t
xkb_keymap_key_effective_layout(ثابت بنية xkb_key *key,
                                xkb_layout_index_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    החזר XkbWrapGroupIntoRange((int32_t) layout,
                                 key->num_groups,
                                 key->out_of_range_group_policy,
                                 key->out_of_range_group_number);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(بنية xkb_keymap *keymap, xkb_mod_mask_t mods);

بنية xkb_level *
xkb_keymap_key_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_key *key,
                         xkb_layout_index_t layout, xkb_level_index_t level);

xkb_action_count_t
xkb_keymap_key_get_actions_by_level(ثابت بنية xkb_keymap *keymap,
                                    ثابت بنية xkb_key *key,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    ثابت اتحاد xkb_action **actions);

بنية xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_rule_names *names);
    bool (*keymap_new_from_string)(بنية xkb_keymap *keymap,
                                   ثابت حرف *string, size_t length);
    bool (*keymap_new_from_file)(بنية xkb_keymap *keymap, FILE *file);
    تعداد xkb_status (*keymap_serialize)(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_result *result
    );
};

خارجي ثابت بنية xkb_keymap_format_ops text_v1_keymap_format_ops;

ساكن مضمن bool
isModsUnLockOnPressSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    החזר format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ساكن مضمن bool
isGroupLockOnReleaseSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    החזר format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ساكن مضمن bool
isModsLatchOnPressSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    החזר format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

ساكن مضمن bool
areOverlappingOverlaysSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    החזר format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
