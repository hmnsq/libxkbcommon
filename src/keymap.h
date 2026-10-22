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
#define XKB_MAX_GROUPS 32
#define XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#define XKB_MAX_GROUPS_X11 4

स्थिर ओळीत xkb_layout_index_t
format_max_groups(गणना xkb_keymap_format format)
{
    परत (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#define XKB_MAX_LEDS ((xkb_led_index_t) (आकार(xkb_led_mask_t) * अक्षर_बिट))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#define XKB_MAX_MODS ((xkb_mod_index_t) (आकार(xkb_mod_mask_t) * अक्षर_बिट))

गणना {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#define XKB_MOD_NONE 0xffffffffU

गणना mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#define MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
गणना real_mod_index {
    XKB_MOD_INDEX_SHIFT = 0,
    XKB_MOD_INDEX_CAPS,
    XKB_MOD_INDEX_CTRL,
    XKB_MOD_INDEX_MOD1,
    XKB_MOD_INDEX_MOD2,
    XKB_MOD_INDEX_MOD3,
    XKB_MOD_INDEX_MOD4,
    XKB_MOD_INDEX_MOD5,
    XKB_MOD_INDEX_अंक_ENTRIES
};
static_assert(XKB_MOD_INDEX_अंक_ENTRIES == 8, "Invalid X11 core modifiers");

गणना xkb_action_type {
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
    ACTION_TYPE_REDIRECT_किल्ली,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_अंक_ENTRIES
};

गणना xkb_action_flags {
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
    CONTROL_STICKY_किल्ल्या = (1 << 0),
    CONTROL_MOUSE_किल्ल्या = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_किल्ल्या_ACCEL = (1 << 15),
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
         CONTROL_STICKY_किल्ल्या | CONTROL_MOUSE_किल्ल्या | CONTROL_MOUSE_किल्ल्या_ACCEL | \
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
         CONTROL_STICKY_किल्ल्या | CONTROL_MOUSE_किल्ल्या | CONTROL_MOUSE_किल्ल्या_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(आकार(गणना xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

स्थिर ओळीत गणना xkb_action_controls
format_boolean_controls(गणना xkb_keymap_format format)
{
    परत (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_किल्ल्या ==
    (गणना xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_किल्ल्या,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_किल्ल्या ==
    (गणना xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_किल्ल्या,
    "Private value should match public API"
);


गणना xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

संरचना xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

संरचना xkb_mod_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    संरचना xkb_mods mods;
};

संरचना xkb_group_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
प्रकारव्याख्या uint8_t xkb_overlay_index_t;
/** Keyboard overlay mask */
प्रकारव्याख्या uint8_t xkb_overlay_mask_t;
गणना {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_मोजणी_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_मोजणी = (आकार(xkb_overlay_mask_t) * अक्षर_बिट),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_मोजणी - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_INDEX_MIN_WIDTH = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_मोजणी_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_मोजणी) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_मोजणी ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_मोजणी < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_मोजणी <= (1u << XKB_OVERLAY_INDEX_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

स्थिर ओळीत xkb_overlay_mask_t
overlays_from_controls(गणना xkb_action_controls controls)
{
    परत (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

स्थिर ओळीत गणना xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    परत (गणना xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

स्थिर ओळीत xkb_overlay_index_t
format_max_overlays(गणना xkb_keymap_format format)
{
    परत (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_मोजणी_X11
        : XKB_OVERLAY_मोजणी;
}

संरचना xkb_controls_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    गणना xkb_action_controls ctrls;
};

प्रकारव्याख्या uint8_t xkb_pointer_button_index_t;
प्रकारव्याख्या uint8_t xkb_pointer_button_mask_t;

गणना {
    XKB_POINTER_BUTTON_INDEX_WIDTH =
        आकार(xkb_pointer_button_index_t) * अक्षर_बिट,
    XKB_POINTER_BUTTON_INDEX_MAX =
        (1 << XKB_POINTER_BUTTON_INDEX_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        आकार(xkb_pointer_button_mask_t) * अक्षर_बिट,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_INDEX_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

संरचना xkb_pointer_default_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int8_t मूल्य;
};

गणना {
    XKB_POINTER_DEFAULT_ACTION_मूल्य_WIDTH =
        आकार(((संरचना xkb_pointer_default_action*)0)->मूल्य) * अक्षर_बिट,
    XKB_POINTER_DEFAULT_ACTION_मूल्य_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_मूल्य_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_मूल्य_MAX,
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
    uint8_t मोजणी;
    xkb_pointer_button_index_t button;
};

संरचना xkb_switch_screen_action {
    गणना xkb_action_type type;
    गणना xkb_action_flags flags;
    int8_t screen;
};

संरचना xkb_redirect_किल्ली_action {
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
    xkb_mod_mask_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_mask_t mods;
};

संरचना xkb_private_action {
    गणना xkb_action_type type;
    uint8_t data[7];
};

गणना xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
संरचना xkb_internal_action {
    गणना xkb_action_type type;
    गणना xkb_internal_action_flags flags;
    संघ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

संघ xkb_action {
    गणना xkb_action_type type;
    संरचना xkb_mod_action mods;
    संरचना xkb_group_action group;
    संरचना xkb_controls_action ctrls;
    संरचना xkb_redirect_किल्ली_action redirect;
    संरचना xkb_pointer_default_action dflt;
    संरचना xkb_pointer_motion_action ptr;
    संरचना xkb_pointer_button_action btn;
    संरचना xkb_switch_screen_action screen;
    संरचना xkb_private_action priv;
    संरचना xkb_internal_action internal;
};

संरचना xkb_किल्ली_type_entry {
    xkb_level_index_t level;
    संरचना xkb_mods mods;
    संरचना xkb_mods preserve;
};

संरचना xkb_किल्ली_type {
    xkb_atom_t name;
    संरचना xkb_mods mods;
    bool required;
    xkb_level_index_t अंक_levels;
    xkb_level_index_t अंक_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(अंक_level_names);
    darray_size_t अंक_entries;
    संरचना xkb_किल्ली_type_entry *entries ATTR_COUNTED_BY(अंक_entries);
};

प्रकारव्याख्या uint16_t xkb_action_मोजणी_t;
#define MAX_ACTIONS_PER_LEVEL UINT16_MAX

संरचना xkb_sym_interpret {
    xkb_keysym_t sym;
    गणना xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_index_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_मोजणी_t अंक_actions;
    संघ {
        /* num_actions <= 1 */
        संघ xkb_action action;
        /* num_actions >  1 */
        संघ xkb_action *actions;
    } a;
};

गणना {XKB_STATE_COMPONENT_WIDTH = (आकार(गणना xkb_state_component) * अक्षर_बिट)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

संरचना xkb_led {
    xkb_atom_t name;
    गणना xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    गणना xkb_state_component which_mods;
    संरचना xkb_mods mods;
    गणना xkb_action_controls ctrls;
};

संरचना xkb_किल्ली_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

संरचना xkb_controls {
    अचिन्हित अक्षर groups_wrap;
    संरचना xkb_mods internal;
    संरचना xkb_mods ignore_lock;
    अचिन्हित लघु repeat_delay;
    अचिन्हित लघु repeat_interval;
    अचिन्हित लघु slow_किल्ल्या_delay;
    अचिन्हित लघु debounce_delay;
    अचिन्हित लघु ax_options;
    अचिन्हित लघु ax_timeout;
    अचिन्हित लघु axt_opts_mask;
    अचिन्हित लघु axt_opts_मूल्ये;
    अचिन्हित पूर्णांक axt_ctrls_mask;
    अचिन्हित पूर्णांक axt_ctrls_मूल्ये;
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

प्रकारव्याख्या uint16_t xkb_keysym_मोजणी_t;
#define MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
संरचना xkb_level {
    /** Count of keysyms */
    xkb_keysym_मोजणी_t अंक_syms;
    /** Count of actions */
    xkb_action_मोजणी_t अंक_actions;
    /** Upper keysym */
    संघ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    संघ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    संघ {
        संघ xkb_action action;   /* num_actions == 1 */
        संघ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
संरचना xkb_group {
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
    स्थिरांक संरचना xkb_किल्ली_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    संरचना xkb_level *levels;
};

गणना {
    DEFAULT_किल्ली_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_किल्ली_REPEAT = true,
    FALLBACK_INTERPRET_किल्ली_REPEAT = DEFAULT_किल्ली_REPEAT
};

गणना {
    DEFAULT_किल्ली_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_किल्ली_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_किल्ली_VMODMAP
};

संरचना xkb_किल्ली {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    गणना xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    गणना xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_index_t out_of_range_group_संख्या:8;

    xkb_layout_index_t अंक_groups:8;
    संरचना xkb_group *groups ATTR_COUNTED_BY(अंक_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    संघ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        स्थिरांक संरचना xkb_किल्ली *overlay_किल्ली;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        स्थिरांक संरचना xkb_किल्ली **overlays_किल्ल्या;
    };
};

संरचना xkb_mod {
    xkb_atom_t name;
    गणना mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

संरचना xkb_mod_set {
    संरचना xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_index_t अंक_mods;
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
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(आकार(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(आकार(संरचना xkb_किल्ली)),
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
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(आकार(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(आकार(संरचना xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_SIZE_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
प्रकारव्याख्या संघ {
    संरचना {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_size_t:(DARRAY_SIZE_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    संरचना {
        bool found:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool खालचा:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_size_t index:(DARRAY_SIZE_T_WIDTH - 3);
    } किल्ली;
    /* Only if is_alias = true, for better semantics */
    संरचना {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_SIZE_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

प्रकारव्याख्या संरचना {
    संघ {
        /** Used for parsing */
        गणना merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t किल्ली_Code;
    };
    bool have_चिन्ह;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    संघ {
        xkb_keysym_t किल्ली_Sym;
        xkb_atom_t किल्ली_Name;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
संरचना xkb_keymap {
    संरचना xkb_context *ctx;

    पूर्णांक refcnt;
    गणना xkb_keymap_compile_flags flags;
    गणना xkb_keymap_format format;

    xkb_led_index_t अंक_leds;
    संरचना xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_किल्ली_code;
    xkb_keycode_t max_किल्ली_code;
    xkb_keycode_t अंक_किल्ल्या;
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
    xkb_keycode_t अंक_किल्ल्या_खालचा;
    संरचना xkb_किल्ली *किल्ल्या ATTR_COUNTED_BY(अंक_किल्ल्या);

    संघ {
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
            darray_size_t अंक_किल्ली_names;
            KeycodeMatch *किल्ली_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        संरचना {
            darray_size_t अंक_किल्ली_aliases;
            संरचना xkb_किल्ली_alias *किल्ली_aliases;
        };
    };

    संरचना xkb_किल्ली_type *types;
    darray_size_t अंक_types;

    darray_size_t अंक_sym_interprets;
    संरचना xkb_sym_interpret *sym_interprets;

    darray_size_t _padding;
    darray_size_t अंक_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    संरचना xkb_mod_set mods;
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
    xkb_keycode_t redirect_किल्ली_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_index_t अंक_groups;
    /* Not all groups must have names. */
    xkb_layout_index_t अंक_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(अंक_group_names);

    अक्षर *keycodes_section_name;
    अक्षर *symbols_section_name;
    अक्षर *types_section_name;
    अक्षर *compat_section_name;
};

#define xkb_किल्ल्या_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    साठी ((iter) = (keymap)->किल्ल्या + \
                  ((keymap)->अंक_किल्ल्या_खालचा == 0 ? 0 : (keymap)->min_किल्ली_code); \
         (iter) < (keymap)->किल्ल्या + (keymap)->अंक_किल्ल्या; \
         (iter)++)

#define xkb_level_foreach_action(iter, level)                 \
    साठी (संघ xkb_action                                     \
         *(iter) = (likely((level)->अंक_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * स्थिरांक iter##_last_ = (iter) + (level)->अंक_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#define xkb_mods_foreach(iter, mods_) \
    साठी ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->अंक_mods; \
         (iter)++)

#define xkb_mods_mask_foreach(mask, iter, mods_) \
    साठी ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->अंक_mods; \
         (iter)++, (mask) >>= 1) \
        जर ((mask) & 0x1)

/** Enumerate all modifiers */
#define xkb_mods_enumerate(idx, iter, mods_) \
    साठी ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->अंक_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#define xkb_rmods_enumerate(idx, iter, mods_) \
    साठी ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_INDEX_अंक_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#define xkb_vmods_enumerate(idx, iter, mods_) \
    साठी ((idx) = XKB_MOD_INDEX_अंक_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_INDEX_अंक_ENTRIES]; \
         (idx) < (mods_)->अंक_mods; \
         (idx)++, (iter)++)

#define xkb_leds_foreach(iter, keymap) \
    साठी ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->अंक_leds; \
         (iter)++)

#define xkb_leds_enumerate(idx, iter, keymap) \
    साठी ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->अंक_leds; \
         (idx)++, (iter)++)

शून्य
clear_level(संरचना xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
स्थिर ओळीत संरचना xkb_किल्ली *
Xkb_किल्ली_By_Name(स्थिरांक संरचना xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    जर (name < keymap->अंक_किल्ली_names) {
        स्थिरांक KeycodeMatch match = keymap->किल्ली_names[name];
        जर (match.found) {
            जर (!match.is_alias) {
                प्रतिज्ञा(name == keymap->किल्ल्या[match.किल्ली.index].name);
                परत &keymap->किल्ल्या[match.किल्ली.index];
            } अन्यथा जर (use_aliases) {
                प्रतिज्ञा(match.alias.real ==
                       keymap->किल्ल्या[keymap->किल्ली_names[match.alias.real].किल्ली.index].name);
                परत &keymap->किल्ल्या[keymap->किल्ली_names[match.alias.real].किल्ली.index];
            }
        }
    }
    परत रिक्त;
}

स्थिर ओळीत स्थिरांक संरचना xkb_किल्ली *
Xkb_किल्ली(स्थिरांक संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    जर (kc < keymap->min_किल्ली_code || kc > keymap->max_किल्ली_code) {
        /* Unsupported keycode */
        परत रिक्त;
    } अन्यथा जर (kc < keymap->अंक_किल्ल्या_खालचा) {
        /* Low keycodes */
        परत &keymap->किल्ल्या[kc];
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->अंक_किल्ल्या_खालचा;
        xkb_keycode_t upper = keymap->अंक_किल्ल्या;
        पर्यंत (lower < upper) {
            स्थिरांक xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            स्थिरांक संरचना xkb_किल्ली * स्थिरांक किल्ली = &keymap->किल्ल्या[mid];
            जर (किल्ली->keycode < kc) {
                lower = mid + 1;
            } अन्यथा जर (किल्ली->keycode > kc) {
                upper = mid;
            } अन्यथा {
                परत किल्ली;
            }
        }
        परत रिक्त;
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
स्थिर ओळीत स्थिरांक संरचना xkb_किल्ली *
xkb_keymap_get_next_defined_किल्ली(स्थिरांक संरचना xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    जर (!keymap->अंक_किल्ल्या || kc > XKB_KEYCODE_MAX) {
        परत रिक्त;
    } अन्यथा जर (kc < keymap->min_किल्ली_code) {
        kc = keymap->min_किल्ली_code;
    } अन्यथा जर (kc > keymap->max_किल्ली_code) {
        kc = keymap->max_किल्ली_code;
    }

    जर (kc < keymap->अंक_किल्ल्या_खालचा) {
        /* Low keycodes */
        स्थिरांक संरचना xkb_किल्ली *किल्ली = &keymap->किल्ल्या[kc];
        जर (ascending) {
            स्थिरांक संरचना xkb_किल्ली *end = &keymap->किल्ल्या[keymap->अंक_किल्ल्या_खालचा - 1];
            पर्यंत (किल्ली->name == XKB_ATOM_NONE && किल्ली < end) किल्ली++;
        } अन्यथा {
            स्थिरांक संरचना xkb_किल्ली *end = &keymap->किल्ल्या[keymap->min_किल्ली_code];
            पर्यंत (किल्ली->name == XKB_ATOM_NONE && किल्ली > end) किल्ली--;
        }
        परत (किल्ली->name == XKB_ATOM_NONE) ? रिक्त : किल्ली;
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->अंक_किल्ल्या_खालचा
            ? keymap->अंक_किल्ल्या_खालचा - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->अंक_किल्ल्या - 1;
        स्थिरांक संरचना xkb_किल्ली * candidate = रिक्त;
        पर्यंत (lower <= upper) {
            स्थिरांक xkb_keycode_t mid = lower + (upper - lower) / 2;
            स्थिरांक संरचना xkb_किल्ली * स्थिरांक किल्ली = &keymap->किल्ल्या[mid];
            जर (किल्ली->keycode == kc) {
                परत किल्ली;
            } अन्यथा जर (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                जर (किल्ली->keycode >= kc) {
                    candidate = किल्ली;
                    upper = mid - 1;
                } अन्यथा {
                    lower = mid + 1;
                }
            } अन्यथा {
                /* Looking for the *smallest* defined keycode <= kc */
                जर (किल्ली->keycode <= kc) {
                    candidate = किल्ली;
                    lower = mid + 1;
                } अन्यथा जर (किल्ली->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        परत candidate;
    }
}

स्थिर ओळीत xkb_level_index_t
Xkb_किल्ली_अंक_Levels(स्थिरांक संरचना xkb_किल्ली *किल्ली, xkb_layout_index_t layout)
{
    परत किल्ली->groups[layout].type->अंक_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
स्थिर ओळीत bool
entry_is_active(स्थिरांक संरचना xkb_किल्ली_type_entry *entry)
{
    परत entry->mods.mods == 0 || entry->mods.mask != 0;
}

संरचना xkb_keymap *
xkb_keymap_new(संरचना xkb_context *ctx, स्थिरांक अक्षर* func,
               गणना xkb_keymap_format format,
               गणना xkb_keymap_compile_flags flags);

शून्य
XkbEscapeMapName(अक्षर *name);

xkb_mod_index_t
XkbModNameToIndex(स्थिरांक संरचना xkb_mod_set *mods, xkb_atom_t name,
                  गणना mod_type type);

bool
XkbLevelsSameSyms(स्थिरांक संरचना xkb_level *a, स्थिरांक संरचना xkb_level *b);

bool
action_equal(स्थिरांक संघ xkb_action *a, स्थिरांक संघ xkb_action *b);

bool
XkbLevelsSameActions(स्थिरांक संरचना xkb_level *a, स्थिरांक संरचना xkb_level *b);

xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t अंक_groups,
                      गणना xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_संख्या);

स्थिर ओळीत xkb_layout_index_t
xkb_keymap_किल्ली_effective_layout(स्थिरांक संरचना xkb_किल्ली *किल्ली,
                                xkb_layout_index_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    परत XkbWrapGroupIntoRange((int32_t) layout,
                                 किल्ली->अंक_groups,
                                 किल्ली->out_of_range_group_policy,
                                 किल्ली->out_of_range_group_संख्या);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(संरचना xkb_keymap *keymap, xkb_mod_mask_t mods);

संरचना xkb_level *
xkb_keymap_किल्ली_get_level(संरचना xkb_keymap *keymap, स्थिरांक संरचना xkb_किल्ली *किल्ली,
                         xkb_layout_index_t layout, xkb_level_index_t level);

xkb_action_मोजणी_t
xkb_keymap_किल्ली_get_actions_by_level(स्थिरांक संरचना xkb_keymap *keymap,
                                    स्थिरांक संरचना xkb_किल्ली *किल्ली,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    स्थिरांक संघ xkb_action **actions);

संरचना xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(संरचना xkb_keymap *keymap,
                                  स्थिरांक संरचना xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(संरचना xkb_keymap *keymap,
                                  स्थिरांक संरचना xkb_rule_names *names);
    bool (*keymap_new_from_string)(संरचना xkb_keymap *keymap,
                                   स्थिरांक अक्षर *string, माप_प्रकार length);
    bool (*keymap_new_from_file)(संरचना xkb_keymap *keymap, खाता *file);
    गणना xkb_status (*keymap_serialize)(
        स्थिरांक संरचना xkb_keymap *keymap,
        स्थिरांक संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_result *result
    );
};

बाह्य स्थिरांक संरचना xkb_keymap_format_ops text_v1_keymap_format_ops;

स्थिर ओळीत bool
isModsUnLockOnPressSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    परत format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर ओळीत bool
isGroupLockOnReleaseSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    परत format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर ओळीत bool
isModsLatchOnPressSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    परत format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

स्थिर ओळीत bool
areOverlappingOverlaysSupported(गणना xkb_keymap_format format)
{
    /* Lax bound */
    परत format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
