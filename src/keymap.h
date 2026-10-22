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
#आशय once

 /* Don't use compat names in internal code. */
#घोषणा _XKBCOMMON_COMPAT_H

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>
#समावेश <stdint.h>
#समावेश <stdbool.h>

#समावेश "xkbcommon/xkbcommon.h"

#समावेश "atom.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "rmlvo.h"
#समावेश "utils.h"
#समावेश "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#घोषणा XKB_MAX_GROUPS 32
#घोषणा XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#घोषणा XKB_MAX_GROUPS_X11 4

अटल अंतरभूत xkb_layout_सूचकांक_t
format_max_groups(क्रमागत xkb_keymap_format format)
{
    वापस (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#घोषणा XKB_MAX_LEDS ((xkb_led_सूचकांक_t) (माप(xkb_led_mask_t) * अक्षर_बिट))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#घोषणा XKB_MAX_MODS ((xkb_mod_सूचकांक_t) (माप(xkb_mod_mask_t) * अक्षर_बिट))

क्रमागत {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#घोषणा XKB_MOD_NONE 0xffffffffU

क्रमागत mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#घोषणा MOD_REAL_MASK_ALL ((xkb_mod_mask_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
क्रमागत real_mod_सूचकांक {
    XKB_MOD_सूचकांक_SHIFT = 0,
    XKB_MOD_सूचकांक_CAPS,
    XKB_MOD_सूचकांक_CTRL,
    XKB_MOD_सूचकांक_MOD1,
    XKB_MOD_सूचकांक_MOD2,
    XKB_MOD_सूचकांक_MOD3,
    XKB_MOD_सूचकांक_MOD4,
    XKB_MOD_सूचकांक_MOD5,
    XKB_MOD_सूचकांक_अंक_ENTRIES
};
static_assert(XKB_MOD_सूचकांक_अंक_ENTRIES == 8, "Invalid X11 core modifiers");

क्रमागत xkb_action_type {
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
    ACTION_TYPE_REDIRECT_कुंजी,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_PRIVATE,
    ACTION_TYPE_INTERNAL, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_अंक_ENTRIES
};

क्रमागत xkb_action_flags {
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

क्रमागत {
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
क्रमागत xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_कुंजियाँ = (1 << 0),
    CONTROL_MOUSE_कुंजियाँ = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_कुंजियाँ_ACCEL = (1 << 15),
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
         CONTROL_STICKY_कुंजियाँ | CONTROL_MOUSE_कुंजियाँ | CONTROL_MOUSE_कुंजियाँ_ACCEL | \
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
         CONTROL_STICKY_कुंजियाँ | CONTROL_MOUSE_कुंजियाँ | CONTROL_MOUSE_कुंजियाँ_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_GROUP_LOCK),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(माप(क्रमागत xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)_LAST_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

अटल अंतरभूत क्रमागत xkb_action_controls
format_boolean_controls(क्रमागत xkb_keymap_format format)
{
    वापस (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_कुंजियाँ ==
    (क्रमागत xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुंजियाँ,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_कुंजियाँ ==
    (क्रमागत xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_कुंजियाँ,
    "Private value should match public API"
);


क्रमागत xkb_match_operation {
    MATCH_NONE,
    MATCH_ANY_OR_NONE,
    MATCH_ANY,
    MATCH_ALL,
    MATCH_EXACTLY,
};

काष्ठा xkb_mods {
    xkb_mod_mask_t mods;       /* original real+virtual mods in definition */
    xkb_mod_mask_t mask;       /* computed effective mask */
};

काष्ठा xkb_mod_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    काष्ठा xkb_mods mods;
};

काष्ठा xkb_group_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    int32_t group;
};

/** Keyboard overlay index or count */
प्रकार uint8_t xkb_overlay_सूचकांक_t;
/** Keyboard overlay mask */
प्रकार uint8_t xkb_overlay_mask_t;
क्रमागत {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_गिनती_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_गिनती = (माप(xkb_overlay_mask_t) * अक्षर_बिट),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_गिनती - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_सूचकांक_MIN_WIDTH = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_गिनती_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_गिनती) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_गिनती ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_गिनती < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_गिनती <= (1u << XKB_OVERLAY_सूचकांक_MIN_WIDTH) - 1,
              "Cannot encode overlay index or count in a nibble");

अटल अंतरभूत xkb_overlay_mask_t
overlays_from_controls(क्रमागत xkb_action_controls controls)
{
    वापस (xkb_overlay_mask_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

अटल अंतरभूत क्रमागत xkb_action_controls
overlays_to_controls(xkb_overlay_mask_t overlays)
{
    वापस (क्रमागत xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

अटल अंतरभूत xkb_overlay_सूचकांक_t
format_max_overlays(क्रमागत xkb_keymap_format format)
{
    वापस (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? XKB_OVERLAY_गिनती_X11
        : XKB_OVERLAY_गिनती;
}

काष्ठा xkb_controls_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    क्रमागत xkb_action_controls ctrls;
};

प्रकार uint8_t xkb_pointer_button_सूचकांक_t;
प्रकार uint8_t xkb_pointer_button_mask_t;

क्रमागत {
    XKB_POINTER_BUTTON_सूचकांक_WIDTH =
        माप(xkb_pointer_button_सूचकांक_t) * अक्षर_बिट,
    XKB_POINTER_BUTTON_सूचकांक_MAX =
        (1 << XKB_POINTER_BUTTON_सूचकांक_WIDTH) - 1,
    XKB_POINTER_BUTTON_MASK_WIDTH =
        माप(xkb_pointer_button_mask_t) * अक्षर_बिट,
    XKB_POINTER_BUTTON_DEFAULT = 0,
    XKB_POINTER_BUTTON_MIN = 1,
    XKB_POINTER_BUTTON_MAX = 5,
};

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_सूचकांक_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
              "xkb_pointer_button_mask_t cannot store button mask");

काष्ठा xkb_pointer_default_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    int8_t मूल्य;
};

क्रमागत {
    XKB_POINTER_DEFAULT_ACTION_मूल्य_WIDTH =
        माप(((काष्ठा xkb_pointer_default_action*)0)->मूल्य) * अक्षर_बिट,
    XKB_POINTER_DEFAULT_ACTION_मूल्य_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_POINTER_DEFAULT_ACTION_मूल्य_WIDTH - 1)) - 1,
};

static_assert((uint32_t)XKB_POINTER_BUTTON_MAX <=
              (uint32_t)XKB_POINTER_DEFAULT_ACTION_मूल्य_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

काष्ठा xkb_pointer_motion_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

काष्ठा xkb_pointer_button_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    uint8_t गिनती;
    xkb_pointer_button_सूचकांक_t button;
};

काष्ठा xkb_switch_screen_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_action_flags flags;
    int8_t screen;
};

काष्ठा xkb_redirect_कुंजी_action {
    क्रमागत xkb_action_type type;
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

काष्ठा xkb_private_action {
    क्रमागत xkb_action_type type;
    uint8_t तथ्य[7];
};

क्रमागत xkb_internal_action_flags {
    INTERNAL_BREAKS_GROUP_LATCH = (1 << 0),
    INTERNAL_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
काष्ठा xkb_internal_action {
    क्रमागत xkb_action_type type;
    क्रमागत xkb_internal_action_flags flags;
    जोड़ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_mask_t clear_latched_mods;
    };
};

जोड़ xkb_action {
    क्रमागत xkb_action_type type;
    काष्ठा xkb_mod_action mods;
    काष्ठा xkb_group_action group;
    काष्ठा xkb_controls_action ctrls;
    काष्ठा xkb_redirect_कुंजी_action redirect;
    काष्ठा xkb_pointer_default_action dflt;
    काष्ठा xkb_pointer_motion_action ptr;
    काष्ठा xkb_pointer_button_action btn;
    काष्ठा xkb_switch_screen_action screen;
    काष्ठा xkb_private_action priv;
    काष्ठा xkb_internal_action internal;
};

काष्ठा xkb_कुंजी_type_entry {
    xkb_level_सूचकांक_t level;
    काष्ठा xkb_mods mods;
    काष्ठा xkb_mods preserve;
};

काष्ठा xkb_कुंजी_type {
    xkb_atom_t name;
    काष्ठा xkb_mods mods;
    bool required;
    xkb_level_सूचकांक_t अंक_levels;
    xkb_level_सूचकांक_t अंक_level_names;
    xkb_atom_t *level_names ATTR_COUNTED_BY(अंक_level_names);
    darray_आकार_t अंक_entries;
    काष्ठा xkb_कुंजी_type_entry *entries ATTR_COUNTED_BY(अंक_entries);
};

प्रकार uint16_t xkb_action_गिनती_t;
#घोषणा MAX_ACTIONS_PER_LEVEL UINT16_MAX

काष्ठा xkb_sym_interpret {
    xkb_keysym_t sym;
    क्रमागत xkb_match_operation match;
    xkb_mod_mask_t mods;
    xkb_mod_सूचकांक_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_गिनती_t अंक_actions;
    जोड़ {
        /* num_actions <= 1 */
        जोड़ xkb_action action;
        /* num_actions >  1 */
        जोड़ xkb_action *actions;
    } a;
};

क्रमागत {XKB_STATE_COMPONENT_WIDTH = (माप(क्रमागत xkb_state_component) * अक्षर_बिट)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_WIDTH) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

काष्ठा xkb_led {
    xkb_atom_t name;
    क्रमागत xkb_state_component which_groups:(XKB_STATE_COMPONENT_WIDTH - 1);
    bool pending_groups:1;
    xkb_layout_mask_t groups;
    क्रमागत xkb_state_component which_mods;
    काष्ठा xkb_mods mods;
    क्रमागत xkb_action_controls ctrls;
};

काष्ठा xkb_कुंजी_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

काष्ठा xkb_controls {
    अचिन्हित अक्षर groups_wrap;
    काष्ठा xkb_mods internal;
    काष्ठा xkb_mods ignore_lock;
    अचिन्हित लघु repeat_delay;
    अचिन्हित लघु repeat_interval;
    अचिन्हित लघु slow_कुंजियाँ_delay;
    अचिन्हित लघु debounce_delay;
    अचिन्हित लघु ax_options;
    अचिन्हित लघु ax_timeout;
    अचिन्हित लघु axt_opts_mask;
    अचिन्हित लघु axt_opts_मूल्यावली;
    अचिन्हित पूर्णांक axt_ctrls_mask;
    अचिन्हित पूर्णांक axt_ctrls_मूल्यावली;
};

क्रमागत xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

प्रकार uint16_t xkb_keysym_गिनती_t;
#घोषणा MAX_KEYSYMS_PER_LEVEL UINT16_MAX

/** A key level */
काष्ठा xkb_level {
    /** Count of keysyms */
    xkb_keysym_गिनती_t अंक_syms;
    /** Count of actions */
    xkb_action_गिनती_t अंक_actions;
    /** Upper keysym */
    जोड़ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t upper;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_upper;
    };
    /** Keysyms */
    जोड़ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    जोड़ {
        जोड़ xkb_action action;   /* num_actions == 1 */
        जोड़ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
काष्ठा xkb_group {
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
    स्थिर काष्ठा xkb_कुंजी_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    काष्ठा xkb_level *levels;
};

क्रमागत {
    DEFAULT_कुंजी_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_कुंजी_REPEAT = true,
    FALLBACK_INTERPRET_कुंजी_REPEAT = DEFAULT_कुंजी_REPEAT
};

क्रमागत {
    DEFAULT_कुंजी_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_कुंजी_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_कुंजी_VMODMAP
};

काष्ठा xkb_कुंजी {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    क्रमागत xkb_explicit_components explicit;

    xkb_mod_mask_t modmap;
    xkb_mod_mask_t vmodmap;

    xkb_overlay_mask_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool out_of_range_pending_group:1;
    क्रमागत xkb_layout_out_of_range_policy out_of_range_group_policy:4;
    xkb_layout_सूचकांक_t out_of_range_group_संख्या:8;

    xkb_layout_सूचकांक_t अंक_groups:8;
    काष्ठा xkb_group *groups ATTR_COUNTED_BY(अंक_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    जोड़ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        स्थिर काष्ठा xkb_कुंजी *overlay_कुंजी;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        स्थिर काष्ठा xkb_कुंजी **overlays_कुंजियाँ;
    };
};

काष्ठा xkb_mod {
    xkb_atom_t name;
    क्रमागत mod_type type;
    xkb_mod_mask_t mapping; /* vmod -> real mod mapping */
};

काष्ठा xkb_mod_set {
    काष्ठा xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_सूचकांक_t अंक_mods;
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
#घोषणा XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(माप(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(माप(काष्ठा xkb_कुंजी)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#घोषणा XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(माप(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(माप(काष्ठा xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_आकार_T_WIDTH - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
प्रकार जोड़ {
    काष्ठा {
        /** If true, the index is valid */
        bool found:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_आकार_t:(DARRAY_आकार_T_WIDTH - 3);
    };
    /* Only if is_alias = false, for better semantics */
    काष्ठा {
        bool found:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool निचला:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_आकार_t सूचकांक:(DARRAY_आकार_T_WIDTH - 3);
    } कुंजी;
    /* Only if is_alias = true, for better semantics */
    काष्ठा {
        bool found:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_आकार_T_WIDTH - 3);
    } alias;
} KeycodeMatch;

प्रकार काष्ठा {
    जोड़ {
        /** Used for parsing */
        क्रमागत merge_mode merge;
        /** Used for serializing */
        xkb_keycode_t कुंजी_Code;
    };
    bool have_प्रतीक;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_mask_t mods;
    जोड़ {
        xkb_keysym_t कुंजी_Sym;
        xkb_atom_t कुंजी_Name;
    } u;
} ModMapEntry;

/** Common keyboard description structure */
काष्ठा xkb_keymap {
    काष्ठा xkb_context *ctx;

    पूर्णांक refcnt;
    क्रमागत xkb_keymap_compile_flags flags;
    क्रमागत xkb_keymap_format format;

    xkb_led_सूचकांक_t अंक_leds;
    काष्ठा xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_कुंजी_code;
    xkb_keycode_t max_कुंजी_code;
    xkb_keycode_t अंक_कुंजियाँ;
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
    xkb_keycode_t अंक_कुंजियाँ_निचला;
    काष्ठा xkb_कुंजी *कुंजियाँ ATTR_COUNTED_BY(अंक_कुंजियाँ);

    जोड़ {
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
        काष्ठा {
            darray_आकार_t अंक_कुंजी_names;
            KeycodeMatch *कुंजी_names;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        काष्ठा {
            darray_आकार_t अंक_कुंजी_aliases;
            काष्ठा xkb_कुंजी_alias *कुंजी_aliases;
        };
    };

    काष्ठा xkb_कुंजी_type *types;
    darray_आकार_t अंक_types;

    darray_आकार_t अंक_sym_interprets;
    काष्ठा xkb_sym_interpret *sym_interprets;

    darray_आकार_t _padding;
    darray_आकार_t अंक_modmaps;
    ModMapEntry *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    काष्ठा xkb_mod_set mods;
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
    xkb_keycode_t redirect_कुंजी_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_सूचकांक_t अंक_groups;
    /* Not all groups must have names. */
    xkb_layout_सूचकांक_t अंक_group_names;
    xkb_atom_t *group_names ATTR_COUNTED_BY(अंक_group_names);

    अक्षर *keycodes_section_name;
    अक्षर *symbols_section_name;
    अक्षर *types_section_name;
    अक्षर *compat_section_name;
};

#घोषणा xkb_कुंजियाँ_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    क्रम ((iter) = (keymap)->कुंजियाँ + \
                  ((keymap)->अंक_कुंजियाँ_निचला == 0 ? 0 : (keymap)->min_कुंजी_code); \
         (iter) < (keymap)->कुंजियाँ + (keymap)->अंक_कुंजियाँ; \
         (iter)++)

#घोषणा xkb_level_foreach_action(iter, level)                 \
    क्रम (जोड़ xkb_action                                     \
         *(iter) = (likely((level)->अंक_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * स्थिर iter##_last_ = (iter) + (level)->अंक_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#घोषणा xkb_mods_foreach(iter, mods_) \
    क्रम ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->अंक_mods; \
         (iter)++)

#घोषणा xkb_mods_mask_foreach(mask, iter, mods_) \
    क्रम ((iter) = (mods_)->mods; \
         (mask) && (iter) < (mods_)->mods + (mods_)->अंक_mods; \
         (iter)++, (mask) >>= 1) \
        अगर ((mask) & 0x1)

/** Enumerate all modifiers */
#घोषणा xkb_mods_enumerate(idx, iter, mods_) \
    क्रम ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->अंक_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#घोषणा xkb_rmods_enumerate(idx, iter, mods_) \
    क्रम ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_सूचकांक_अंक_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#घोषणा xkb_vmods_enumerate(idx, iter, mods_) \
    क्रम ((idx) = XKB_MOD_सूचकांक_अंक_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_सूचकांक_अंक_ENTRIES]; \
         (idx) < (mods_)->अंक_mods; \
         (idx)++, (iter)++)

#घोषणा xkb_leds_foreach(iter, keymap) \
    क्रम ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->अंक_leds; \
         (iter)++)

#घोषणा xkb_leds_enumerate(idx, iter, keymap) \
    क्रम ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->अंक_leds; \
         (idx)++, (iter)++)

व्योम
clear_level(काष्ठा xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
अटल अंतरभूत काष्ठा xkb_कुंजी *
Xkb_कुंजी_By_Name(स्थिर काष्ठा xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    अगर (name < keymap->अंक_कुंजी_names) {
        स्थिर KeycodeMatch match = keymap->कुंजी_names[name];
        अगर (match.found) {
            अगर (!match.is_alias) {
                निश्चित(name == keymap->कुंजियाँ[match.कुंजी.सूचकांक].name);
                वापस &keymap->कुंजियाँ[match.कुंजी.सूचकांक];
            } अन्यथा अगर (use_aliases) {
                निश्चित(match.alias.real ==
                       keymap->कुंजियाँ[keymap->कुंजी_names[match.alias.real].कुंजी.सूचकांक].name);
                वापस &keymap->कुंजियाँ[keymap->कुंजी_names[match.alias.real].कुंजी.सूचकांक];
            }
        }
    }
    वापस शून्य;
}

अटल अंतरभूत स्थिर काष्ठा xkb_कुंजी *
Xkb_कुंजी(स्थिर काष्ठा xkb_keymap *keymap, xkb_keycode_t kc)
{
    अगर (kc < keymap->min_कुंजी_code || kc > keymap->max_कुंजी_code) {
        /* Unsupported keycode */
        वापस शून्य;
    } अन्यथा अगर (kc < keymap->अंक_कुंजियाँ_निचला) {
        /* Low keycodes */
        वापस &keymap->कुंजियाँ[kc];
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->अंक_कुंजियाँ_निचला;
        xkb_keycode_t upper = keymap->अंक_कुंजियाँ;
        जबतक (lower < upper) {
            स्थिर xkb_keycode_t mid = lower + (upper - 1 - lower) / 2;
            स्थिर काष्ठा xkb_कुंजी * स्थिर कुंजी = &keymap->कुंजियाँ[mid];
            अगर (कुंजी->keycode < kc) {
                lower = mid + 1;
            } अन्यथा अगर (कुंजी->keycode > kc) {
                upper = mid;
            } अन्यथा {
                वापस कुंजी;
            }
        }
        वापस शून्य;
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
अटल अंतरभूत स्थिर काष्ठा xkb_कुंजी *
xkb_keymap_get_अग्र_defined_कुंजी(स्थिर काष्ठा xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    अगर (!keymap->अंक_कुंजियाँ || kc > XKB_KEYCODE_MAX) {
        वापस शून्य;
    } अन्यथा अगर (kc < keymap->min_कुंजी_code) {
        kc = keymap->min_कुंजी_code;
    } अन्यथा अगर (kc > keymap->max_कुंजी_code) {
        kc = keymap->max_कुंजी_code;
    }

    अगर (kc < keymap->अंक_कुंजियाँ_निचला) {
        /* Low keycodes */
        स्थिर काष्ठा xkb_कुंजी *कुंजी = &keymap->कुंजियाँ[kc];
        अगर (ascending) {
            स्थिर काष्ठा xkb_कुंजी *end = &keymap->कुंजियाँ[keymap->अंक_कुंजियाँ_निचला - 1];
            जबतक (कुंजी->name == XKB_ATOM_NONE && कुंजी < end) कुंजी++;
        } अन्यथा {
            स्थिर काष्ठा xkb_कुंजी *end = &keymap->कुंजियाँ[keymap->min_कुंजी_code];
            जबतक (कुंजी->name == XKB_ATOM_NONE && कुंजी > end) कुंजी--;
        }
        वापस (कुंजी->name == XKB_ATOM_NONE) ? शून्य : कुंजी;
    } अन्यथा {
        /* High keycodes: use binary search */
        xkb_keycode_t lower = keymap->अंक_कुंजियाँ_निचला
            ? keymap->अंक_कुंजियाँ_निचला - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t upper = keymap->अंक_कुंजियाँ - 1;
        स्थिर काष्ठा xkb_कुंजी * candidate = शून्य;
        जबतक (lower <= upper) {
            स्थिर xkb_keycode_t mid = lower + (upper - lower) / 2;
            स्थिर काष्ठा xkb_कुंजी * स्थिर कुंजी = &keymap->कुंजियाँ[mid];
            अगर (कुंजी->keycode == kc) {
                वापस कुंजी;
            } अन्यथा अगर (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                अगर (कुंजी->keycode >= kc) {
                    candidate = कुंजी;
                    upper = mid - 1;
                } अन्यथा {
                    lower = mid + 1;
                }
            } अन्यथा {
                /* Looking for the *smallest* defined keycode <= kc */
                अगर (कुंजी->keycode <= kc) {
                    candidate = कुंजी;
                    lower = mid + 1;
                } अन्यथा अगर (कुंजी->keycode > kc) {
                    upper = mid - 1;
                }
            }
        }
        वापस candidate;
    }
}

अटल अंतरभूत xkb_level_सूचकांक_t
Xkb_कुंजी_अंक_Levels(स्थिर काष्ठा xkb_कुंजी *कुंजी, xkb_layout_सूचकांक_t layout)
{
    वापस कुंजी->groups[layout].type->अंक_levels;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
अटल अंतरभूत bool
entry_is_active(स्थिर काष्ठा xkb_कुंजी_type_entry *entry)
{
    वापस entry->mods.mods == 0 || entry->mods.mask != 0;
}

काष्ठा xkb_keymap *
xkb_keymap_new(काष्ठा xkb_context *ctx, स्थिर अक्षर* func,
               क्रमागत xkb_keymap_format format,
               क्रमागत xkb_keymap_compile_flags flags);

व्योम
XkbEscapeMapName(अक्षर *name);

xkb_mod_सूचकांक_t
Xkb_Mod_Name_To_सूचकांक(स्थिर काष्ठा xkb_mod_set *mods, xkb_atom_t name,
                  क्रमागत mod_type type);

bool
XkbLevelsSameSyms(स्थिर काष्ठा xkb_level *a, स्थिर काष्ठा xkb_level *b);

bool
action_equal(स्थिर जोड़ xkb_action *a, स्थिर जोड़ xkb_action *b);

bool
XkbLevelsSameActions(स्थिर काष्ठा xkb_level *a, स्थिर काष्ठा xkb_level *b);

xkb_layout_सूचकांक_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_सूचकांक_t अंक_groups,
                      क्रमागत xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_सूचकांक_t out_of_range_group_संख्या);

अटल अंतरभूत xkb_layout_सूचकांक_t
xkb_keymap_कुंजी_effective_layout(स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                xkb_layout_सूचकांक_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    वापस XkbWrapGroupIntoRange((int32_t) layout,
                                 कुंजी->अंक_groups,
                                 कुंजी->out_of_range_group_policy,
                                 कुंजी->out_of_range_group_संख्या);
}

XKB_EXPORT_PRIVATE xkb_mod_mask_t
mod_mask_get_effective(काष्ठा xkb_keymap *keymap, xkb_mod_mask_t mods);

काष्ठा xkb_level *
xkb_keymap_कुंजी_get_level(काष्ठा xkb_keymap *keymap, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                         xkb_layout_सूचकांक_t layout, xkb_level_सूचकांक_t level);

xkb_action_गिनती_t
xkb_keymap_कुंजी_get_actions_by_level(स्थिर काष्ठा xkb_keymap *keymap,
                                    स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                    xkb_layout_सूचकांक_t layout,
                                    xkb_level_सूचकांक_t level,
                                    स्थिर जोड़ xkb_action **actions);

काष्ठा xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(काष्ठा xkb_keymap *keymap,
                                  स्थिर काष्ठा xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_names)(काष्ठा xkb_keymap *keymap,
                                  स्थिर काष्ठा xkb_rule_names *names);
    bool (*keymap_new_from_string)(काष्ठा xkb_keymap *keymap,
                                   स्थिर अक्षर *string, माप_प्रकार लंबाई);
    bool (*keymap_new_from_file)(काष्ठा xkb_keymap *keymap, खाता *file);
    क्रमागत xkb_status (*keymap_serialize)(
        स्थिर काष्ठा xkb_keymap *keymap,
        स्थिर काष्ठा xkb_keymap_serialize_config *config,
        काष्ठा xkb_keymap_serialize_परिणाम *परिणाम
    );
};

बाह्य स्थिर काष्ठा xkb_keymap_format_ops text_v1_keymap_format_ops;

अटल अंतरभूत bool
isModsUnLockOnPressSupported(क्रमागत xkb_keymap_format format)
{
    /* Lax bound */
    वापस format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

अटल अंतरभूत bool
isGroupLockOnReleaseSupported(क्रमागत xkb_keymap_format format)
{
    /* Lax bound */
    वापस format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

अटल अंतरभूत bool
isModsLatchOnPressSupported(क्रमागत xkb_keymap_format format)
{
    /* Lax bound */
    वापस format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}

अटल अंतरभूत bool
areOverlappingOverlaysSupported(क्रमागत xkb_keymap_format format)
{
    /* Lax bound */
    वापस format >= XKB_KEYMAP_FORMAT_TEXT_V2;
}
