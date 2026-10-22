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
#هدايت once

 /* Don't use compat names in internal code. */
#وصف_ڪريو _XKBCOMMON_COMPAT_H

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
#وصف_ڪريو XKB_MAX_GROUPS 32
#وصف_ڪريو XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#وصف_ڪريو XKB_MAX_GROUPS_X11 4

ساكن مضمن xkb_layout_اشاريو_t
format_max_groups(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#وصف_ڪريو XKB_MAX_LEDS ((xkb_led_اشاريو_t) (حجم(xkb_led_نقاب_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#وصف_ڪريو XKB_MAX_MODS ((xkb_mod_اشاريو_t) (حجم(xkb_mod_نقاب_t) * CHAR_BIT))

تعداد {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#وصف_ڪريو XKB_MOD_NONE 0xffffffffU

تعداد mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#وصف_ڪريو MOD_REAL_نقاب_ALL ((xkb_mod_نقاب_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
تعداد real_mod_اشاريو {
    XKB_MOD_اشاريو_سرڪايو = 0,
    XKB_MOD_اشاريو_CAPS,
    XKB_MOD_اشاريو_CTRL,
    XKB_MOD_اشاريو_MOD1,
    XKB_MOD_اشاريو_MOD2,
    XKB_MOD_اشاريو_MOD3,
    XKB_MOD_اشاريو_MOD4,
    XKB_MOD_اشاريو_MOD5,
    XKB_MOD_اشاريو_انگ_ENTRIES
};
static_assert(XKB_MOD_اشاريو_انگ_ENTRIES == 8, "Invalid X11 core modifiers");

تعداد xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_سيٽ,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_تالو,
    ACTION_TYPE_گروهه_سيٽ,
    ACTION_TYPE_گروهه_LATCH,
    ACTION_TYPE_گروهه_تالو,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_تالو,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_سيٽ,
    ACTION_TYPE_CTRL_تالو,
    ACTION_TYPE_REDIRECT_ڪنجي,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_ذاتي,
    ACTION_TYPE_اندروني, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_انگ_ENTRIES
};

تعداد xkb_action_flags {
    ACTION_تالو_CLEAR = (1 << 0),
    ACTION_LATCH_منزل_تالو = (1 << 1),
    ACTION_تالو_NO_تالو = (1 << 2),
    ACTION_تالو_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_تالو_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_تالو_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_گروهه_تالو_ON_MODIFIER_RELEASE = (1 << 16),
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
    CONTROL_STICKY_ڪنجيون = (1 << 0),
    CONTROL_MOUSE_ڪنجيون = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_ڪنجيون_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_گروهه_تالو = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    آخري_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ڪنجيون | CONTROL_MOUSE_ڪنجيون | CONTROL_MOUSE_ڪنجيون_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_گروهه_تالو | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ڪنجيون | CONTROL_MOUSE_ڪنجيون | CONTROL_MOUSE_ڪنجيون_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_گروهه_تالو),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(حجم(تعداد xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)آخري_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ساكن مضمن تعداد xkb_action_controls
format_boolean_controls(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_ڪنجيون ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ڪنجيون,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_ڪنجيون ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_ڪنجيون,
    "Private value should match public API"
);


تعداد xkb_ميلاپ_operation {
    ميلاپ_NONE,
    ميلاپ_ANY_OR_NONE,
    ميلاپ_ANY,
    ميلاپ_ALL,
    ميلاپ_EXACTLY,
};

بنية xkb_mods {
    xkb_mod_نقاب_t mods;       /* original real+virtual mods in definition */
    xkb_mod_نقاب_t نقاب;       /* computed effective mask */
};

بنية xkb_mod_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    بنية xkb_mods mods;
};

بنية xkb_گروهه_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int32_t گروهه;
};

/** Keyboard overlay index or count */
تعريفنوع uint8_t xkb_overlay_اشاريو_t;
/** Keyboard overlay mask */
تعريفنوع uint8_t xkb_overlay_نقاب_t;
تعداد {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ڳڻپ_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_ڳڻپ = (حجم(xkb_overlay_نقاب_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_ڳڻپ - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_اشاريو_MIN_ويڪر = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_ڳڻپ_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_ڳڻپ) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_ڳڻپ ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_ڳڻپ < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_ڳڻپ <= (1u << XKB_OVERLAY_اشاريو_MIN_ويڪر) - 1,
              "Cannot encode overlay index or count in a nibble");

ساكن مضمن xkb_overlay_نقاب_t
overlays_from_controls(تعداد xkb_action_controls controls)
{
    أرجع (xkb_overlay_نقاب_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ساكن مضمن تعداد xkb_action_controls
overlays_منزل_controls(xkb_overlay_نقاب_t overlays)
{
    أرجع (تعداد xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ساكن مضمن xkb_overlay_اشاريو_t
format_max_overlays(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_OVERLAY_ڳڻپ_X11
        : XKB_OVERLAY_ڳڻپ;
}

بنية xkb_controls_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    تعداد xkb_action_controls ctrls;
};

تعريفنوع uint8_t xkb_اشارو_button_اشاريو_t;
تعريفنوع uint8_t xkb_اشارو_button_نقاب_t;

تعداد {
    XKB_اشارو_BUTTON_اشاريو_ويڪر =
        حجم(xkb_اشارو_button_اشاريو_t) * CHAR_BIT,
    XKB_اشارو_BUTTON_اشاريو_MAX =
        (1 << XKB_اشارو_BUTTON_اشاريو_ويڪر) - 1,
    XKB_اشارو_BUTTON_نقاب_ويڪر =
        حجم(xkb_اشارو_button_نقاب_t) * CHAR_BIT,
    XKB_اشارو_BUTTON_DEFAULT = 0,
    XKB_اشارو_BUTTON_MIN = 1,
    XKB_اشارو_BUTTON_MAX = 5,
};

static_assert(XKB_اشارو_BUTTON_MAX <= XKB_اشارو_BUTTON_اشاريو_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_اشارو_BUTTON_MAX <= XKB_اشارو_BUTTON_نقاب_ويڪر,
              "xkb_pointer_button_mask_t cannot store button mask");

بنية xkb_اشارو_default_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t قدر;
};

تعداد {
    XKB_اشارو_DEFAULT_ACTION_قدر_ويڪر =
        حجم(((بنية xkb_اشارو_default_action*)0)->قدر) * CHAR_BIT,
    XKB_اشارو_DEFAULT_ACTION_قدر_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_اشارو_DEFAULT_ACTION_قدر_ويڪر - 1)) - 1,
};

static_assert((uint32_t)XKB_اشارو_BUTTON_MAX <=
              (uint32_t)XKB_اشارو_DEFAULT_ACTION_قدر_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

بنية xkb_اشارو_motion_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

بنية xkb_اشارو_button_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    uint8_t ڳڻپ;
    xkb_اشارو_button_اشاريو_t button;
};

بنية xkb_switch_screen_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t screen;
};

بنية xkb_redirect_ڪنجي_action {
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
    xkb_mod_نقاب_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_نقاب_t mods;
};

بنية xkb_ذاتي_action {
    تعداد xkb_action_type type;
    uint8_t معلومات[7];
};

تعداد xkb_اندروني_action_flags {
    اندروني_BREAKS_گروهه_LATCH = (1 << 0),
    اندروني_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
بنية xkb_اندروني_action {
    تعداد xkb_action_type type;
    تعداد xkb_اندروني_action_flags flags;
    اتحاد {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_نقاب_t clear_latched_mods;
    };
};

اتحاد xkb_action {
    تعداد xkb_action_type type;
    بنية xkb_mod_action mods;
    بنية xkb_گروهه_action گروهه;
    بنية xkb_controls_action ctrls;
    بنية xkb_redirect_ڪنجي_action redirect;
    بنية xkb_اشارو_default_action dflt;
    بنية xkb_اشارو_motion_action ptr;
    بنية xkb_اشارو_button_action btn;
    بنية xkb_switch_screen_action screen;
    بنية xkb_ذاتي_action priv;
    بنية xkb_اندروني_action اندروني;
};

بنية xkb_ڪنجي_type_داخلا {
    xkb_level_اشاريو_t level;
    بنية xkb_mods mods;
    بنية xkb_mods preserve;
};

بنية xkb_ڪنجي_type {
    xkb_atom_t name;
    بنية xkb_mods mods;
    bool required;
    xkb_level_اشاريو_t انگ_سطحون;
    xkb_level_اشاريو_t انگ_level_نالا;
    xkb_atom_t *level_نالا ATTR_COUNTED_BY(انگ_level_نالا);
    darray_سائيز_t انگ_entries;
    بنية xkb_ڪنجي_type_داخلا *entries ATTR_COUNTED_BY(انگ_entries);
};

تعريفنوع uint16_t xkb_action_ڳڻپ_t;
#وصف_ڪريو MAX_ACTIONS_في_LEVEL UINT16_MAX

بنية xkb_sym_interpret {
    xkb_keysym_t sym;
    تعداد xkb_ميلاپ_operation ميلاپ;
    xkb_mod_نقاب_t mods;
    xkb_mod_اشاريو_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_ڳڻپ_t انگ_actions;
    اتحاد {
        /* num_actions <= 1 */
        اتحاد xkb_action action;
        /* num_actions >  1 */
        اتحاد xkb_action *actions;
    } a;
};

تعداد {XKB_STATE_COMPONENT_ويڪر = (حجم(تعداد xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_ويڪر) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

بنية xkb_led {
    xkb_atom_t name;
    تعداد xkb_state_component which_groups:(XKB_STATE_COMPONENT_ويڪر - 1);
    bool pending_groups:1;
    xkb_layout_نقاب_t groups;
    تعداد xkb_state_component which_mods;
    بنية xkb_mods mods;
    تعداد xkb_action_controls ctrls;
};

بنية xkb_ڪنجي_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

بنية xkb_controls {
    بنانشان حرف groups_wrap;
    بنية xkb_mods اندروني;
    بنية xkb_mods ignore_تالو;
    بنانشان ننڍو repeat_delay;
    بنانشان ننڍو repeat_interval;
    بنانشان ننڍو slow_ڪنجيون_delay;
    بنانشان ننڍو debounce_delay;
    بنانشان ننڍو ax_options;
    بنانشان ننڍو ax_timeout;
    بنانشان ننڍو axt_opts_نقاب;
    بنانشان ننڍو axt_opts_قدرون;
    بنانشان صحيح axt_ctrls_نقاب;
    بنانشان صحيح axt_ctrls_قدرون;
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

تعريفنوع uint16_t xkb_keysym_ڳڻپ_t;
#وصف_ڪريو MAX_KEYSYMS_في_LEVEL UINT16_MAX

/** A key level */
بنية xkb_level {
    /** Count of keysyms */
    xkb_keysym_ڳڻپ_t انگ_syms;
    /** Count of actions */
    xkb_action_ڳڻپ_t انگ_actions;
    /** Upper keysym */
    اتحاد {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t مٿانهون;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_مٿانهون;
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
بنية xkb_گروهه {
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
    ثابت بنية xkb_ڪنجي_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    بنية xkb_level *سطحون;
};

تعداد {
    DEFAULT_ڪنجي_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_ڪنجي_REPEAT = true,
    FALLBACK_INTERPRET_ڪنجي_REPEAT = DEFAULT_ڪنجي_REPEAT
};

تعداد {
    DEFAULT_ڪنجي_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_ڪنجي_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_ڪنجي_VMODMAP
};

بنية xkb_ڪنجي {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    تعداد xkb_explicit_components explicit;

    xkb_mod_نقاب_t modmap;
    xkb_mod_نقاب_t vmodmap;

    xkb_overlay_نقاب_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool ٻاهريون_جو_range_pending_گروهه:1;
    تعداد xkb_layout_ٻاهريون_جو_range_policy ٻاهريون_جو_range_گروهه_policy:4;
    xkb_layout_اشاريو_t ٻاهريون_جو_range_گروهه_عدد:8;

    xkb_layout_اشاريو_t انگ_groups:8;
    بنية xkb_گروهه *groups ATTR_COUNTED_BY(انگ_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ثابت بنية xkb_ڪنجي *overlay_ڪنجي;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_ڪنجي **overlays_ڪنجيون;
    };
};

بنية xkb_mod {
    xkb_atom_t name;
    تعداد mod_type type;
    xkb_mod_نقاب_t mapping; /* vmod -> real mod mapping */
};

بنية xkb_mod_سيٽ {
    بنية xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_اشاريو_t انگ_mods;
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
#وصف_ڪريو XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(بنية xkb_ڪنجي)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#وصف_ڪريو XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(بنية xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_سائيز_T_ويڪر - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
تعريفنوع اتحاد {
    بنية {
        /** If true, the index is valid */
        bool مليو:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_سائيز_t:(DARRAY_سائيز_T_ويڪر - 3);
    };
    /* Only if is_alias = false, for better semantics */
    بنية {
        bool مليو:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool هيٺيون:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_سائيز_t اشاريو:(DARRAY_سائيز_T_ويڪر - 3);
    } ڪنجي;
    /* Only if is_alias = true, for better semantics */
    بنية {
        bool مليو:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_سائيز_T_ويڪر - 3);
    } alias;
} Keycode_ميلاپ;

تعريفنوع بنية {
    اتحاد {
        /** Used for parsing */
        تعداد ملايو_گهڻائي ملايو;
        /** Used for serializing */
        xkb_keycode_t ڪنجي_ڪوڊ;
    };
    bool have_نشان;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_نقاب_t mods;
    اتحاد {
        xkb_keysym_t ڪنجي_Sym;
        xkb_atom_t ڪنجي_Name;
    } u;
} Mod_Map_داخلا;

/** Common keyboard description structure */
بنية xkb_keymap {
    بنية xkb_context *ctx;

    صحيح refcnt;
    تعداد xkb_keymap_compile_flags flags;
    تعداد xkb_keymap_format format;

    xkb_led_اشاريو_t انگ_leds;
    بنية xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_ڪنجي_ڪوڊ;
    xkb_keycode_t max_ڪنجي_ڪوڊ;
    xkb_keycode_t انگ_ڪنجيون;
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
    xkb_keycode_t انگ_ڪنجيون_هيٺيون;
    بنية xkb_ڪنجي *ڪنجيون ATTR_COUNTED_BY(انگ_ڪنجيون);

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
            darray_سائيز_t انگ_ڪنجي_نالا;
            Keycode_ميلاپ *ڪنجي_نالا;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        بنية {
            darray_سائيز_t انگ_ڪنجي_aliases;
            بنية xkb_ڪنجي_alias *ڪنجي_aliases;
        };
    };

    بنية xkb_ڪنجي_type *types;
    darray_سائيز_t انگ_types;

    darray_سائيز_t انگ_sym_interprets;
    بنية xkb_sym_interpret *sym_interprets;

    darray_سائيز_t _padding;
    darray_سائيز_t انگ_modmaps;
    Mod_Map_داخلا *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    بنية xkb_mod_سيٽ mods;
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
    xkb_keycode_t redirect_ڪنجي_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_اشاريو_t انگ_groups;
    /* Not all groups must have names. */
    xkb_layout_اشاريو_t انگ_گروهه_نالا;
    xkb_atom_t *گروهه_نالا ATTR_COUNTED_BY(انگ_گروهه_نالا);

    حرف *keycodes_section_name;
    حرف *symbols_section_name;
    حرف *types_section_name;
    حرف *compat_section_name;
};

#وصف_ڪريو xkb_ڪنجيون_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    لكل ((iter) = (keymap)->ڪنجيون + \
                  ((keymap)->انگ_ڪنجيون_هيٺيون == 0 ? 0 : (keymap)->min_ڪنجي_ڪوڊ); \
         (iter) < (keymap)->ڪنجيون + (keymap)->انگ_ڪنجيون; \
         (iter)++)

#وصف_ڪريو xkb_level_foreach_action(iter, level)                 \
    لكل (اتحاد xkb_action                                     \
         *(iter) = (likely((level)->انگ_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ثابت iter##_last_ = (iter) + (level)->انگ_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#وصف_ڪريو xkb_mods_foreach(iter, mods_) \
    لكل ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->انگ_mods; \
         (iter)++)

#وصف_ڪريو xkb_mods_نقاب_foreach(نقاب, iter, mods_) \
    لكل ((iter) = (mods_)->mods; \
         (نقاب) && (iter) < (mods_)->mods + (mods_)->انگ_mods; \
         (iter)++, (نقاب) >>= 1) \
        إذا ((نقاب) & 0x1)

/** Enumerate all modifiers */
#وصف_ڪريو xkb_mods_enumerate(idx, iter, mods_) \
    لكل ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->انگ_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#وصف_ڪريو xkb_rmods_enumerate(idx, iter, mods_) \
    لكل ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_اشاريو_انگ_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#وصف_ڪريو xkb_vmods_enumerate(idx, iter, mods_) \
    لكل ((idx) = XKB_MOD_اشاريو_انگ_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_اشاريو_انگ_ENTRIES]; \
         (idx) < (mods_)->انگ_mods; \
         (idx)++, (iter)++)

#وصف_ڪريو xkb_leds_foreach(iter, keymap) \
    لكل ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->انگ_leds; \
         (iter)++)

#وصف_ڪريو xkb_leds_enumerate(idx, iter, keymap) \
    لكل ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->انگ_leds; \
         (idx)++, (iter)++)

فراغ
clear_level(بنية xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ساكن مضمن بنية xkb_ڪنجي *
Xkb_ڪنجي_By_Name(ثابت بنية xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    إذا (name < keymap->انگ_ڪنجي_نالا) {
        ثابت Keycode_ميلاپ ميلاپ = keymap->ڪنجي_نالا[name];
        إذا (ميلاپ.مليو) {
            إذا (!ميلاپ.is_alias) {
                أكد(name == keymap->ڪنجيون[ميلاپ.ڪنجي.اشاريو].name);
                أرجع &keymap->ڪنجيون[ميلاپ.ڪنجي.اشاريو];
            } وإلا إذا (use_aliases) {
                أكد(ميلاپ.alias.real ==
                       keymap->ڪنجيون[keymap->ڪنجي_نالا[ميلاپ.alias.real].ڪنجي.اشاريو].name);
                أرجع &keymap->ڪنجيون[keymap->ڪنجي_نالا[ميلاپ.alias.real].ڪنجي.اشاريو];
            }
        }
    }
    أرجع عدم;
}

ساكن مضمن ثابت بنية xkb_ڪنجي *
Xkb_ڪنجي(ثابت بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    إذا (kc < keymap->min_ڪنجي_ڪوڊ || kc > keymap->max_ڪنجي_ڪوڊ) {
        /* Unsupported keycode */
        أرجع عدم;
    } وإلا إذا (kc < keymap->انگ_ڪنجيون_هيٺيون) {
        /* Low keycodes */
        أرجع &keymap->ڪنجيون[kc];
    } وإلا {
        /* High keycodes: use binary search */
        xkb_keycode_t هيٺاهون = keymap->انگ_ڪنجيون_هيٺيون;
        xkb_keycode_t مٿانهون = keymap->انگ_ڪنجيون;
        طالما (هيٺاهون < مٿانهون) {
            ثابت xkb_keycode_t mid = هيٺاهون + (مٿانهون - 1 - هيٺاهون) / 2;
            ثابت بنية xkb_ڪنجي * ثابت ڪنجي = &keymap->ڪنجيون[mid];
            إذا (ڪنجي->keycode < kc) {
                هيٺاهون = mid + 1;
            } وإلا إذا (ڪنجي->keycode > kc) {
                مٿانهون = mid;
            } وإلا {
                أرجع ڪنجي;
            }
        }
        أرجع عدم;
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
ساكن مضمن ثابت بنية xkb_ڪنجي *
xkb_keymap_get_ايندڙ_defined_ڪنجي(ثابت بنية xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    إذا (!keymap->انگ_ڪنجيون || kc > XKB_KEYCODE_MAX) {
        أرجع عدم;
    } وإلا إذا (kc < keymap->min_ڪنجي_ڪوڊ) {
        kc = keymap->min_ڪنجي_ڪوڊ;
    } وإلا إذا (kc > keymap->max_ڪنجي_ڪوڊ) {
        kc = keymap->max_ڪنجي_ڪوڊ;
    }

    إذا (kc < keymap->انگ_ڪنجيون_هيٺيون) {
        /* Low keycodes */
        ثابت بنية xkb_ڪنجي *ڪنجي = &keymap->ڪنجيون[kc];
        إذا (ascending) {
            ثابت بنية xkb_ڪنجي *پڇاڙي = &keymap->ڪنجيون[keymap->انگ_ڪنجيون_هيٺيون - 1];
            طالما (ڪنجي->name == XKB_ATOM_NONE && ڪنجي < پڇاڙي) ڪنجي++;
        } وإلا {
            ثابت بنية xkb_ڪنجي *پڇاڙي = &keymap->ڪنجيون[keymap->min_ڪنجي_ڪوڊ];
            طالما (ڪنجي->name == XKB_ATOM_NONE && ڪنجي > پڇاڙي) ڪنجي--;
        }
        أرجع (ڪنجي->name == XKB_ATOM_NONE) ? عدم : ڪنجي;
    } وإلا {
        /* High keycodes: use binary search */
        xkb_keycode_t هيٺاهون = keymap->انگ_ڪنجيون_هيٺيون
            ? keymap->انگ_ڪنجيون_هيٺيون - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t مٿانهون = keymap->انگ_ڪنجيون - 1;
        ثابت بنية xkb_ڪنجي * candidate = عدم;
        طالما (هيٺاهون <= مٿانهون) {
            ثابت xkb_keycode_t mid = هيٺاهون + (مٿانهون - هيٺاهون) / 2;
            ثابت بنية xkb_ڪنجي * ثابت ڪنجي = &keymap->ڪنجيون[mid];
            إذا (ڪنجي->keycode == kc) {
                أرجع ڪنجي;
            } وإلا إذا (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                إذا (ڪنجي->keycode >= kc) {
                    candidate = ڪنجي;
                    مٿانهون = mid - 1;
                } وإلا {
                    هيٺاهون = mid + 1;
                }
            } وإلا {
                /* Looking for the *smallest* defined keycode <= kc */
                إذا (ڪنجي->keycode <= kc) {
                    candidate = ڪنجي;
                    هيٺاهون = mid + 1;
                } وإلا إذا (ڪنجي->keycode > kc) {
                    مٿانهون = mid - 1;
                }
            }
        }
        أرجع candidate;
    }
}

ساكن مضمن xkb_level_اشاريو_t
Xkb_ڪنجي_انگ_سطحون(ثابت بنية xkb_ڪنجي *ڪنجي, xkb_layout_اشاريو_t layout)
{
    أرجع ڪنجي->groups[layout].type->انگ_سطحون;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ساكن مضمن bool
داخلا_is_سرگرم(ثابت بنية xkb_ڪنجي_type_داخلا *داخلا)
{
    أرجع داخلا->mods.mods == 0 || داخلا->mods.نقاب != 0;
}

بنية xkb_keymap *
xkb_keymap_new(بنية xkb_context *ctx, ثابت حرف* func,
               تعداد xkb_keymap_format format,
               تعداد xkb_keymap_compile_flags flags);

فراغ
XkbEscapeMapName(حرف *name);

xkb_mod_اشاريو_t
Xkb_Mod_Name_منزل_اشاريو(ثابت بنية xkb_mod_سيٽ *mods, xkb_atom_t name,
                  تعداد mod_type type);

bool
Xkb_سطحون_Same_Syms(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b);

bool
Xkb_سطحون_Same_Actions(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

xkb_layout_اشاريو_t
Xkb_Wrap_گروهه_Into_Range(int32_t گروهه,
                      xkb_layout_اشاريو_t انگ_groups,
                      تعداد xkb_layout_ٻاهريون_جو_range_policy ٻاهريون_جو_range_گروهه_policy,
                      xkb_layout_اشاريو_t ٻاهريون_جو_range_گروهه_عدد);

ساكن مضمن xkb_layout_اشاريو_t
xkb_keymap_ڪنجي_effective_layout(ثابت بنية xkb_ڪنجي *ڪنجي,
                                xkb_layout_اشاريو_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    أرجع Xkb_Wrap_گروهه_Into_Range((int32_t) layout,
                                 ڪنجي->انگ_groups,
                                 ڪنجي->ٻاهريون_جو_range_گروهه_policy,
                                 ڪنجي->ٻاهريون_جو_range_گروهه_عدد);
}

XKB_EXPORT_ذاتي xkb_mod_نقاب_t
mod_نقاب_get_effective(بنية xkb_keymap *keymap, xkb_mod_نقاب_t mods);

بنية xkb_level *
xkb_keymap_ڪنجي_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_ڪنجي *ڪنجي,
                         xkb_layout_اشاريو_t layout, xkb_level_اشاريو_t level);

xkb_action_ڳڻپ_t
xkb_keymap_ڪنجي_get_actions_by_level(ثابت بنية xkb_keymap *keymap,
                                    ثابت بنية xkb_ڪنجي *ڪنجي,
                                    xkb_layout_اشاريو_t layout,
                                    xkb_level_اشاريو_t level,
                                    ثابت اتحاد xkb_action **actions);

بنية xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_نالا)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_قاعدو_نالا *نالا);
    bool (*keymap_new_from_string)(بنية xkb_keymap *keymap,
                                   ثابت حرف *string, سائيز_t ڊيگهائي);
    bool (*keymap_new_from_file)(بنية xkb_keymap *keymap, FILE *file);
    تعداد xkb_حالت (*keymap_serialize)(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_نتيجو *نتيجو
    );
};

خارجي ثابت بنية xkb_keymap_format_ops متن_v1_keymap_format_ops;

ساكن مضمن bool
is_Mods_Un_تالو_On_Press_Supported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    أرجع format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساكن مضمن bool
is_گروهه_تالو_On_Release_Supported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    أرجع format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساكن مضمن bool
isModsLatchOnPressSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    أرجع format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساكن مضمن bool
areOverlappingOverlaysSupported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    أرجع format >= XKB_KEYMAP_FORMAT_متن_V2;
}
