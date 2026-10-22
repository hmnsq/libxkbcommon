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
#ہدایت once

 /* Don't use compat names in internal code. */
#تعریف_کرو _XKBCOMMON_COMPAT_H

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"

#شامل "atom.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "rmlvo.h"
#شامل "utils.h"
#شامل "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#تعریف_کرو XKB_MAX_GROUPS 32
#تعریف_کرو XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#تعریف_کرو XKB_MAX_GROUPS_X11 4

ساکن اندرونی xkb_layout_اشاریہ_t
format_max_groups(شمار xkb_keymap_format format)
{
    واپس (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#تعریف_کرو XKB_MAX_LEDS ((xkb_led_اشاریہ_t) (حجم(xkb_led_نقاب_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#تعریف_کرو XKB_MAX_MODS ((xkb_mod_اشاریہ_t) (حجم(xkb_mod_نقاب_t) * CHAR_BIT))

شمار {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#تعریف_کرو XKB_MOD_NONE 0xffffffffU

شمار mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#تعریف_کرو MOD_REAL_نقاب_ALL ((xkb_mod_نقاب_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
شمار real_mod_اشاریہ {
    XKB_MOD_اشاریہ_سرکاؤ = 0,
    XKB_MOD_اشاریہ_CAPS,
    XKB_MOD_اشاریہ_CTRL,
    XKB_MOD_اشاریہ_MOD1,
    XKB_MOD_اشاریہ_MOD2,
    XKB_MOD_اشاریہ_MOD3,
    XKB_MOD_اشاریہ_MOD4,
    XKB_MOD_اشاریہ_MOD5,
    XKB_MOD_اشاریہ_عدد_ENTRIES
};
static_assert(XKB_MOD_اشاریہ_عدد_ENTRIES == 8, "Invalid X11 core modifiers");

شمار xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_سیٹ,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_تالا,
    ACTION_TYPE_گروہ_سیٹ,
    ACTION_TYPE_گروہ_LATCH,
    ACTION_TYPE_گروہ_تالا,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_تالا,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_سیٹ,
    ACTION_TYPE_CTRL_تالا,
    ACTION_TYPE_REDIRECT_کلید,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_نجی,
    ACTION_TYPE_اندرونی, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_عدد_ENTRIES
};

شمار xkb_action_flags {
    ACTION_تالا_CLEAR = (1 << 0),
    ACTION_LATCH_مقصد_تالا = (1 << 1),
    ACTION_تالا_NO_تالا = (1 << 2),
    ACTION_تالا_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_تالا_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_تالا_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_گروہ_تالا_ON_MODIFIER_RELEASE = (1 << 16),
};

شمار {
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
شمار xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_کلیدیں = (1 << 0),
    CONTROL_MOUSE_کلیدیں = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_کلیدیں_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_گروہ_تالا = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    آخری_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_کلیدیں | CONTROL_MOUSE_کلیدیں | CONTROL_MOUSE_کلیدیں_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_گروہ_تالا | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_کلیدیں | CONTROL_MOUSE_کلیدیں | CONTROL_MOUSE_کلیدیں_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_گروہ_تالا),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(حجم(شمار xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)آخری_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ساکن اندرونی شمار xkb_action_controls
format_boolean_controls(شمار xkb_keymap_format format)
{
    واپس (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_کلیدیں ==
    (شمار xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_کلیدیں ==
    (شمار xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں,
    "Private value should match public API"
);


شمار xkb_مطابقت_operation {
    مطابقت_NONE,
    مطابقت_ANY_OR_NONE,
    مطابقت_ANY,
    مطابقت_ALL,
    مطابقت_EXACTLY,
};

ساخت xkb_mods {
    xkb_mod_نقاب_t mods;       /* original real+virtual mods in definition */
    xkb_mod_نقاب_t نقاب;       /* computed effective mask */
};

ساخت xkb_mod_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    ساخت xkb_mods mods;
};

ساخت xkb_گروہ_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    int32_t گروہ;
};

/** Keyboard overlay index or count */
قسمتعریف uint8_t xkb_overlay_اشاریہ_t;
/** Keyboard overlay mask */
قسمتعریف uint8_t xkb_overlay_نقاب_t;
شمار {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_گنتی_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_گنتی = (حجم(xkb_overlay_نقاب_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_گنتی - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_اشاریہ_MIN_چوڑائی = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_گنتی_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_گنتی) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_گنتی ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_گنتی < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_گنتی <= (1u << XKB_OVERLAY_اشاریہ_MIN_چوڑائی) - 1,
              "Cannot encode overlay index or count in a nibble");

ساکن اندرونی xkb_overlay_نقاب_t
overlays_from_controls(شمار xkb_action_controls controls)
{
    واپس (xkb_overlay_نقاب_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ساکن اندرونی شمار xkb_action_controls
overlays_مقصد_controls(xkb_overlay_نقاب_t overlays)
{
    واپس (شمار xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ساکن اندرونی xkb_overlay_اشاریہ_t
format_max_overlays(شمار xkb_keymap_format format)
{
    واپس (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_OVERLAY_گنتی_X11
        : XKB_OVERLAY_گنتی;
}

ساخت xkb_controls_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    شمار xkb_action_controls ctrls;
};

قسمتعریف uint8_t xkb_اشارہ_گر_button_اشاریہ_t;
قسمتعریف uint8_t xkb_اشارہ_گر_button_نقاب_t;

شمار {
    XKB_اشارہ_گر_BUTTON_اشاریہ_چوڑائی =
        حجم(xkb_اشارہ_گر_button_اشاریہ_t) * CHAR_BIT,
    XKB_اشارہ_گر_BUTTON_اشاریہ_MAX =
        (1 << XKB_اشارہ_گر_BUTTON_اشاریہ_چوڑائی) - 1,
    XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی =
        حجم(xkb_اشارہ_گر_button_نقاب_t) * CHAR_BIT,
    XKB_اشارہ_گر_BUTTON_DEFAULT = 0,
    XKB_اشارہ_گر_BUTTON_MIN = 1,
    XKB_اشارہ_گر_BUTTON_MAX = 5,
};

static_assert(XKB_اشارہ_گر_BUTTON_MAX <= XKB_اشارہ_گر_BUTTON_اشاریہ_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_اشارہ_گر_BUTTON_MAX <= XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی,
              "xkb_pointer_button_mask_t cannot store button mask");

ساخت xkb_اشارہ_گر_default_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    int8_t قدر;
};

شمار {
    XKB_اشارہ_گر_DEFAULT_ACTION_قدر_چوڑائی =
        حجم(((ساخت xkb_اشارہ_گر_default_action*)0)->قدر) * CHAR_BIT,
    XKB_اشارہ_گر_DEFAULT_ACTION_قدر_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_اشارہ_گر_DEFAULT_ACTION_قدر_چوڑائی - 1)) - 1,
};

static_assert((uint32_t)XKB_اشارہ_گر_BUTTON_MAX <=
              (uint32_t)XKB_اشارہ_گر_DEFAULT_ACTION_قدر_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

ساخت xkb_اشارہ_گر_motion_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

ساخت xkb_اشارہ_گر_button_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    uint8_t گنتی;
    xkb_اشارہ_گر_button_اشاریہ_t button;
};

ساخت xkb_switch_screen_action {
    شمار xkb_action_type type;
    شمار xkb_action_flags flags;
    int8_t screen;
};

ساخت xkb_redirect_کلید_action {
    شمار xkb_action_type type;
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
    xkb_mod_نقاب_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_نقاب_t mods;
};

ساخت xkb_نجی_action {
    شمار xkb_action_type type;
    uint8_t ڈیٹا[7];
};

شمار xkb_اندرونی_action_flags {
    اندرونی_BREAKS_گروہ_LATCH = (1 << 0),
    اندرونی_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
ساخت xkb_اندرونی_action {
    شمار xkb_action_type type;
    شمار xkb_اندرونی_action_flags flags;
    اتحاد {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_نقاب_t clear_latched_mods;
    };
};

اتحاد xkb_action {
    شمار xkb_action_type type;
    ساخت xkb_mod_action mods;
    ساخت xkb_گروہ_action گروہ;
    ساخت xkb_controls_action ctrls;
    ساخت xkb_redirect_کلید_action redirect;
    ساخت xkb_اشارہ_گر_default_action dflt;
    ساخت xkb_اشارہ_گر_motion_action ptr;
    ساخت xkb_اشارہ_گر_button_action btn;
    ساخت xkb_switch_screen_action screen;
    ساخت xkb_نجی_action priv;
    ساخت xkb_اندرونی_action اندرونی_2;
};

ساخت xkb_کلید_type_اندراج {
    xkb_level_اشاریہ_t level;
    ساخت xkb_mods mods;
    ساخت xkb_mods preserve;
};

ساخت xkb_کلید_type {
    xkb_atom_t name;
    ساخت xkb_mods mods;
    bool required;
    xkb_level_اشاریہ_t عدد_سطحیں;
    xkb_level_اشاریہ_t عدد_level_نام;
    xkb_atom_t *level_نام ATTR_COUNTED_BY(عدد_level_نام);
    darray_سائز_t عدد_entries;
    ساخت xkb_کلید_type_اندراج *entries ATTR_COUNTED_BY(عدد_entries);
};

قسمتعریف uint16_t xkb_action_گنتی_t;
#تعریف_کرو MAX_ACTIONS_فی_LEVEL UINT16_MAX

ساخت xkb_sym_interpret {
    xkb_keysym_t sym;
    شمار xkb_مطابقت_operation مطابقت;
    xkb_mod_نقاب_t mods;
    xkb_mod_اشاریہ_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_گنتی_t عدد_actions;
    اتحاد {
        /* num_actions <= 1 */
        اتحاد xkb_action action;
        /* num_actions >  1 */
        اتحاد xkb_action *actions;
    } a;
};

شمار {XKB_STATE_COMPONENT_چوڑائی = (حجم(شمار xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_چوڑائی) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

ساخت xkb_led {
    xkb_atom_t name;
    شمار xkb_state_component which_groups:(XKB_STATE_COMPONENT_چوڑائی - 1);
    bool pending_groups:1;
    xkb_layout_نقاب_t groups;
    شمار xkb_state_component which_mods;
    ساخت xkb_mods mods;
    شمار xkb_action_controls ctrls;
};

ساخت xkb_کلید_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

ساخت xkb_controls {
    غیرعلامتی حرف groups_wrap;
    ساخت xkb_mods اندرونی_2;
    ساخت xkb_mods ignore_تالا;
    غیرعلامتی مختصر repeat_delay;
    غیرعلامتی مختصر repeat_interval;
    غیرعلامتی مختصر slow_کلیدیں_delay;
    غیرعلامتی مختصر debounce_delay;
    غیرعلامتی مختصر ax_options;
    غیرعلامتی مختصر ax_timeout;
    غیرعلامتی مختصر axt_opts_نقاب;
    غیرعلامتی مختصر axt_opts_اقدار;
    غیرعلامتی صحیح axt_ctrls_نقاب;
    غیرعلامتی صحیح axt_ctrls_اقدار;
};

شمار xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

قسمتعریف uint16_t xkb_keysym_گنتی_t;
#تعریف_کرو MAX_KEYSYMS_فی_LEVEL UINT16_MAX

/** A key level */
ساخت xkb_level {
    /** Count of keysyms */
    xkb_keysym_گنتی_t عدد_syms;
    /** Count of actions */
    xkb_action_گنتی_t عدد_actions;
    /** Upper keysym */
    اتحاد {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t بالائی;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_بالائی;
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
ساخت xkb_گروہ {
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
    ثابت ساخت xkb_کلید_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    ساخت xkb_level *سطحیں;
};

شمار {
    DEFAULT_کلید_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_کلید_REPEAT = true,
    FALLBACK_INTERPRET_کلید_REPEAT = DEFAULT_کلید_REPEAT
};

شمار {
    DEFAULT_کلید_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_کلید_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_کلید_VMODMAP
};

ساخت xkb_کلید {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    شمار xkb_explicit_components explicit;

    xkb_mod_نقاب_t modmap;
    xkb_mod_نقاب_t vmodmap;

    xkb_overlay_نقاب_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool برآمد_کا_range_pending_گروہ:1;
    شمار xkb_layout_برآمد_کا_range_policy برآمد_کا_range_گروہ_policy:4;
    xkb_layout_اشاریہ_t برآمد_کا_range_گروہ_نمبر:8;

    xkb_layout_اشاریہ_t عدد_groups:8;
    ساخت xkb_گروہ *groups ATTR_COUNTED_BY(عدد_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ثابت ساخت xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت ساخت xkb_کلید **overlays_کلیدیں;
    };
};

ساخت xkb_mod {
    xkb_atom_t name;
    شمار mod_type type;
    xkb_mod_نقاب_t mapping; /* vmod -> real mod mapping */
};

ساخت xkb_mod_سیٹ {
    ساخت xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_اشاریہ_t عدد_mods;
    xkb_mod_نقاب_t explicit_vmods;
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
#تعریف_کرو XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(ساخت xkb_کلید)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#تعریف_کرو XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(ساخت xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_سائز_T_چوڑائی - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
قسمتعریف اتحاد {
    ساخت {
        /** If true, the index is valid */
        bool ملا:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_سائز_t:(DARRAY_سائز_T_چوڑائی - 3);
    };
    /* Only if is_alias = false, for better semantics */
    ساخت {
        bool ملا:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool کم:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_سائز_t اشاریہ:(DARRAY_سائز_T_چوڑائی - 3);
    } کلید;
    /* Only if is_alias = true, for better semantics */
    ساخت {
        bool ملا:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_سائز_T_چوڑائی - 3);
    } alias;
} Keycode_مطابقت;

قسمتعریف ساخت {
    اتحاد {
        /** Used for parsing */
        شمار ملاؤ_منوال ملاؤ;
        /** Used for serializing */
        xkb_keycode_t کلید_رمز;
    };
    bool have_علامت;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_نقاب_t mods;
    اتحاد {
        xkb_keysym_t کلید_Sym;
        xkb_atom_t کلید_Name;
    } u;
} Mod_Map_اندراج;

/** Common keyboard description structure */
ساخت xkb_keymap {
    ساخت xkb_context *ctx;

    صحیح refcnt;
    شمار xkb_keymap_compile_flags flags;
    شمار xkb_keymap_format format;

    xkb_led_اشاریہ_t عدد_leds;
    ساخت xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_کلید_رمز;
    xkb_keycode_t max_کلید_رمز;
    xkb_keycode_t عدد_کلیدیں;
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
    xkb_keycode_t عدد_کلیدیں_کم;
    ساخت xkb_کلید *کلیدیں ATTR_COUNTED_BY(عدد_کلیدیں);

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
        ساخت {
            darray_سائز_t عدد_کلید_نام;
            Keycode_مطابقت *کلید_نام;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        ساخت {
            darray_سائز_t عدد_کلید_aliases;
            ساخت xkb_کلید_alias *کلید_aliases;
        };
    };

    ساخت xkb_کلید_type *types;
    darray_سائز_t عدد_types;

    darray_سائز_t عدد_sym_interprets;
    ساخت xkb_sym_interpret *sym_interprets;

    darray_سائز_t _padding;
    darray_سائز_t عدد_modmaps;
    Mod_Map_اندراج *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    ساخت xkb_mod_سیٹ mods;
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
    xkb_mod_نقاب_t canonical_state_نقاب;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_کلید_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_اشاریہ_t عدد_groups;
    /* Not all groups must have names. */
    xkb_layout_اشاریہ_t عدد_گروہ_نام;
    xkb_atom_t *گروہ_نام ATTR_COUNTED_BY(عدد_گروہ_نام);

    حرف *keycodes_section_name;
    حرف *symbols_section_name;
    حرف *types_section_name;
    حرف *compat_section_name;
};

#تعریف_کرو xkb_کلیدیں_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    برائے ((iter) = (keymap)->کلیدیں + \
                  ((keymap)->عدد_کلیدیں_کم == 0 ? 0 : (keymap)->min_کلید_رمز); \
         (iter) < (keymap)->کلیدیں + (keymap)->عدد_کلیدیں; \
         (iter)++)

#تعریف_کرو xkb_level_foreach_action(iter, level)                 \
    برائے (اتحاد xkb_action                                     \
         *(iter) = (likely((level)->عدد_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ثابت iter##_last_ = (iter) + (level)->عدد_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#تعریف_کرو xkb_mods_foreach(iter, mods_) \
    برائے ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->عدد_mods; \
         (iter)++)

#تعریف_کرو xkb_mods_نقاب_foreach(نقاب, iter, mods_) \
    برائے ((iter) = (mods_)->mods; \
         (نقاب) && (iter) < (mods_)->mods + (mods_)->عدد_mods; \
         (iter)++, (نقاب) >>= 1) \
        اگر ((نقاب) & 0x1)

/** Enumerate all modifiers */
#تعریف_کرو xkb_mods_enumerate(idx, iter, mods_) \
    برائے ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->عدد_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#تعریف_کرو xkb_rmods_enumerate(idx, iter, mods_) \
    برائے ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_اشاریہ_عدد_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#تعریف_کرو xkb_vmods_enumerate(idx, iter, mods_) \
    برائے ((idx) = XKB_MOD_اشاریہ_عدد_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_اشاریہ_عدد_ENTRIES]; \
         (idx) < (mods_)->عدد_mods; \
         (idx)++, (iter)++)

#تعریف_کرو xkb_leds_foreach(iter, keymap) \
    برائے ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->عدد_leds; \
         (iter)++)

#تعریف_کرو xkb_leds_enumerate(idx, iter, keymap) \
    برائے ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->عدد_leds; \
         (idx)++, (iter)++)

خالی
clear_level(ساخت xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ساکن اندرونی ساخت xkb_کلید *
Xkb_کلید_By_Name(ثابت ساخت xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    اگر (name < keymap->عدد_کلید_نام) {
        ثابت Keycode_مطابقت مطابقت = keymap->کلید_نام[name];
        اگر (مطابقت.ملا) {
            اگر (!مطابقت.is_alias) {
                أكد(name == keymap->کلیدیں[مطابقت.کلید.اشاریہ].name);
                واپس &keymap->کلیدیں[مطابقت.کلید.اشاریہ];
            } ورنہ اگر (use_aliases) {
                أكد(مطابقت.alias.real ==
                       keymap->کلیدیں[keymap->کلید_نام[مطابقت.alias.real].کلید.اشاریہ].name);
                واپس &keymap->کلیدیں[keymap->کلید_نام[مطابقت.alias.real].کلید.اشاریہ];
            }
        }
    }
    واپس تہی;
}

ساکن اندرونی ثابت ساخت xkb_کلید *
Xkb_کلید(ثابت ساخت xkb_keymap *keymap, xkb_keycode_t kc)
{
    اگر (kc < keymap->min_کلید_رمز || kc > keymap->max_کلید_رمز) {
        /* Unsupported keycode */
        واپس تہی;
    } ورنہ اگر (kc < keymap->عدد_کلیدیں_کم) {
        /* Low keycodes */
        واپس &keymap->کلیدیں[kc];
    } ورنہ {
        /* High keycodes: use binary search */
        xkb_keycode_t زیریں = keymap->عدد_کلیدیں_کم;
        xkb_keycode_t بالائی = keymap->عدد_کلیدیں;
        جب (زیریں < بالائی) {
            ثابت xkb_keycode_t mid = زیریں + (بالائی - 1 - زیریں) / 2;
            ثابت ساخت xkb_کلید * ثابت کلید = &keymap->کلیدیں[mid];
            اگر (کلید->keycode < kc) {
                زیریں = mid + 1;
            } ورنہ اگر (کلید->keycode > kc) {
                بالائی = mid;
            } ورنہ {
                واپس کلید;
            }
        }
        واپس تہی;
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
ساکن اندرونی ثابت ساخت xkb_کلید *
xkb_keymap_get_اگلا_defined_کلید(ثابت ساخت xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    اگر (!keymap->عدد_کلیدیں || kc > XKB_KEYCODE_MAX) {
        واپس تہی;
    } ورنہ اگر (kc < keymap->min_کلید_رمز) {
        kc = keymap->min_کلید_رمز;
    } ورنہ اگر (kc > keymap->max_کلید_رمز) {
        kc = keymap->max_کلید_رمز;
    }

    اگر (kc < keymap->عدد_کلیدیں_کم) {
        /* Low keycodes */
        ثابت ساخت xkb_کلید *کلید = &keymap->کلیدیں[kc];
        اگر (ascending) {
            ثابت ساخت xkb_کلید *انتہا = &keymap->کلیدیں[keymap->عدد_کلیدیں_کم - 1];
            جب (کلید->name == XKB_ATOM_NONE && کلید < انتہا) کلید++;
        } ورنہ {
            ثابت ساخت xkb_کلید *انتہا = &keymap->کلیدیں[keymap->min_کلید_رمز];
            جب (کلید->name == XKB_ATOM_NONE && کلید > انتہا) کلید--;
        }
        واپس (کلید->name == XKB_ATOM_NONE) ? تہی : کلید;
    } ورنہ {
        /* High keycodes: use binary search */
        xkb_keycode_t زیریں = keymap->عدد_کلیدیں_کم
            ? keymap->عدد_کلیدیں_کم - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t بالائی = keymap->عدد_کلیدیں - 1;
        ثابت ساخت xkb_کلید * candidate = تہی;
        جب (زیریں <= بالائی) {
            ثابت xkb_keycode_t mid = زیریں + (بالائی - زیریں) / 2;
            ثابت ساخت xkb_کلید * ثابت کلید = &keymap->کلیدیں[mid];
            اگر (کلید->keycode == kc) {
                واپس کلید;
            } ورنہ اگر (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                اگر (کلید->keycode >= kc) {
                    candidate = کلید;
                    بالائی = mid - 1;
                } ورنہ {
                    زیریں = mid + 1;
                }
            } ورنہ {
                /* Looking for the *smallest* defined keycode <= kc */
                اگر (کلید->keycode <= kc) {
                    candidate = کلید;
                    زیریں = mid + 1;
                } ورنہ اگر (کلید->keycode > kc) {
                    بالائی = mid - 1;
                }
            }
        }
        واپس candidate;
    }
}

ساکن اندرونی xkb_level_اشاریہ_t
Xkb_کلید_عدد_سطحیں(ثابت ساخت xkb_کلید *کلید, xkb_layout_اشاریہ_t layout)
{
    واپس کلید->groups[layout].type->عدد_سطحیں;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ساکن اندرونی bool
اندراج_is_فعال(ثابت ساخت xkb_کلید_type_اندراج *اندراج_2)
{
    واپس اندراج_2->mods.mods == 0 || اندراج_2->mods.نقاب != 0;
}

ساخت xkb_keymap *
xkb_keymap_new(ساخت xkb_context *ctx, ثابت حرف* func,
               شمار xkb_keymap_format format,
               شمار xkb_keymap_compile_flags flags);

خالی
XkbEscapeMapName(حرف *name);

xkb_mod_اشاریہ_t
Xkb_Mod_Name_مقصد_اشاریہ(ثابت ساخت xkb_mod_سیٹ *mods, xkb_atom_t name,
                  شمار mod_type type);

bool
Xkb_سطحیں_Same_Syms(ثابت ساخت xkb_level *a, ثابت ساخت xkb_level *b);

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b);

bool
Xkb_سطحیں_Same_Actions(ثابت ساخت xkb_level *a, ثابت ساخت xkb_level *b);

xkb_layout_اشاریہ_t
Xkb_Wrap_گروہ_Into_Range(int32_t گروہ,
                      xkb_layout_اشاریہ_t عدد_groups,
                      شمار xkb_layout_برآمد_کا_range_policy برآمد_کا_range_گروہ_policy,
                      xkb_layout_اشاریہ_t برآمد_کا_range_گروہ_نمبر);

ساکن اندرونی xkb_layout_اشاریہ_t
xkb_keymap_کلید_effective_layout(ثابت ساخت xkb_کلید *کلید,
                                xkb_layout_اشاریہ_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    واپس Xkb_Wrap_گروہ_Into_Range((int32_t) layout,
                                 کلید->عدد_groups,
                                 کلید->برآمد_کا_range_گروہ_policy,
                                 کلید->برآمد_کا_range_گروہ_نمبر);
}

XKB_EXPORT_نجی xkb_mod_نقاب_t
mod_نقاب_get_effective(ساخت xkb_keymap *keymap, xkb_mod_نقاب_t mods);

ساخت xkb_level *
xkb_keymap_کلید_get_level(ساخت xkb_keymap *keymap, ثابت ساخت xkb_کلید *کلید,
                         xkb_layout_اشاریہ_t layout, xkb_level_اشاریہ_t level);

xkb_action_گنتی_t
xkb_keymap_کلید_get_actions_by_level(ثابت ساخت xkb_keymap *keymap,
                                    ثابت ساخت xkb_کلید *کلید,
                                    xkb_layout_اشاریہ_t layout,
                                    xkb_level_اشاریہ_t level,
                                    ثابت اتحاد xkb_action **actions);

ساخت xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(ساخت xkb_keymap *keymap,
                                  ثابت ساخت xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_نام)(ساخت xkb_keymap *keymap,
                                  ثابت ساخت xkb_قاعدہ_نام *نام);
    bool (*keymap_new_from_string)(ساخت xkb_keymap *keymap,
                                   ثابت حرف *string, سائز_t لمبائی);
    bool (*keymap_new_from_file)(ساخت xkb_keymap *keymap, FILE *file);
    شمار xkb_حالت (*keymap_serialize)(
        ثابت ساخت xkb_keymap *keymap,
        ثابت ساخت xkb_keymap_serialize_config *config,
        ساخت xkb_keymap_serialize_نتیجہ *نتیجہ
    );
};

خارجی ثابت ساخت xkb_keymap_format_ops متن_v1_keymap_format_ops;

ساکن اندرونی bool
is_Mods_Un_تالا_On_Press_Supported(شمار xkb_keymap_format format)
{
    /* Lax bound */
    واپس format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساکن اندرونی bool
is_گروہ_تالا_On_Release_Supported(شمار xkb_keymap_format format)
{
    /* Lax bound */
    واپس format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساکن اندرونی bool
isModsLatchOnPressSupported(شمار xkb_keymap_format format)
{
    /* Lax bound */
    واپس format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساکن اندرونی bool
areOverlappingOverlaysSupported(شمار xkb_keymap_format format)
{
    /* Lax bound */
    واپس format >= XKB_KEYMAP_FORMAT_متن_V2;
}
