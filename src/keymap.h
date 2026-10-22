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
#رهنمود once

 /* Don't use compat names in internal code. */
#تعریف_کن _XKBCOMMON_COMPAT_H

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
#تعریف_کن XKB_MAX_GROUPS 32
#تعریف_کن XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#تعریف_کن XKB_MAX_GROUPS_X11 4

ساكن مضمن xkb_layout_شاخص_t
format_max_groups(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#تعریف_کن XKB_MAX_LEDS ((xkb_led_شاخص_t) (حجم(xkb_led_ماسک_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#تعریف_کن XKB_MAX_MODS ((xkb_mod_شاخص_t) (حجم(xkb_mod_ماسک_t) * CHAR_BIT))

تعداد {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#تعریف_کن XKB_MOD_NONE 0xffffffffU

تعداد mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#تعریف_کن MOD_REAL_ماسک_ALL ((xkb_mod_ماسک_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
تعداد real_mod_شاخص {
    XKB_MOD_شاخص_لغزش = 0,
    XKB_MOD_شاخص_CAPS,
    XKB_MOD_شاخص_CTRL,
    XKB_MOD_شاخص_MOD1,
    XKB_MOD_شاخص_MOD2,
    XKB_MOD_شاخص_MOD3,
    XKB_MOD_شاخص_MOD4,
    XKB_MOD_شاخص_MOD5,
    XKB_MOD_شاخص_رقم_ENTRIES
};
static_assert(XKB_MOD_شاخص_رقم_ENTRIES == 8, "Invalid X11 core modifiers");

تعداد xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_مجموعه,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_قفل,
    ACTION_TYPE_گروه_مجموعه,
    ACTION_TYPE_گروه_LATCH,
    ACTION_TYPE_گروه_قفل,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_قفل,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_مجموعه,
    ACTION_TYPE_CTRL_قفل,
    ACTION_TYPE_REDIRECT_کلید,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_خصوصی,
    ACTION_TYPE_داخلی, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_رقم_ENTRIES
};

تعداد xkb_action_flags {
    ACTION_قفل_CLEAR = (1 << 0),
    ACTION_LATCH_مقصد_قفل = (1 << 1),
    ACTION_قفل_NO_قفل = (1 << 2),
    ACTION_قفل_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_قفل_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_قفل_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_گروه_قفل_ON_MODIFIER_RELEASE = (1 << 16),
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
    CONTROL_STICKY_کلیدها = (1 << 0),
    CONTROL_MOUSE_کلیدها = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_کلیدها_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_گروه_قفل = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    آخرین_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_کلیدها | CONTROL_MOUSE_کلیدها | CONTROL_MOUSE_کلیدها_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_گروه_قفل | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_کلیدها | CONTROL_MOUSE_کلیدها | CONTROL_MOUSE_کلیدها_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_گروه_قفل),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(حجم(تعداد xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)آخرین_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ساكن مضمن تعداد xkb_action_controls
format_boolean_controls(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_کلیدها ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_کلیدها ==
    (تعداد xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_کلیدها,
    "Private value should match public API"
);


تعداد xkb_تطابق_operation {
    تطابق_NONE,
    تطابق_ANY_OR_NONE,
    تطابق_ANY,
    تطابق_ALL,
    تطابق_EXACTLY,
};

بنية xkb_mods {
    xkb_mod_ماسک_t mods;       /* original real+virtual mods in definition */
    xkb_mod_ماسک_t ماسک;       /* computed effective mask */
};

بنية xkb_mod_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    بنية xkb_mods mods;
};

بنية xkb_گروه_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int32_t گروه;
};

/** Keyboard overlay index or count */
تعريفنوع uint8_t xkb_overlay_شاخص_t;
/** Keyboard overlay mask */
تعريفنوع uint8_t xkb_overlay_ماسک_t;
تعداد {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_شمار_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_شمار = (حجم(xkb_overlay_ماسک_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_شمار - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_شاخص_MIN_عرض = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_شمار_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_شمار) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_شمار ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_شمار < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_شمار <= (1u << XKB_OVERLAY_شاخص_MIN_عرض) - 1,
              "Cannot encode overlay index or count in a nibble");

ساكن مضمن xkb_overlay_ماسک_t
overlays_from_controls(تعداد xkb_action_controls controls)
{
    أرجع (xkb_overlay_ماسک_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ساكن مضمن تعداد xkb_action_controls
overlays_مقصد_controls(xkb_overlay_ماسک_t overlays)
{
    أرجع (تعداد xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ساكن مضمن xkb_overlay_شاخص_t
format_max_overlays(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? XKB_OVERLAY_شمار_X11
        : XKB_OVERLAY_شمار;
}

بنية xkb_controls_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    تعداد xkb_action_controls ctrls;
};

تعريفنوع uint8_t xkb_اشاره_گر_button_شاخص_t;
تعريفنوع uint8_t xkb_اشاره_گر_button_ماسک_t;

تعداد {
    XKB_اشاره_گر_BUTTON_شاخص_عرض =
        حجم(xkb_اشاره_گر_button_شاخص_t) * CHAR_BIT,
    XKB_اشاره_گر_BUTTON_شاخص_MAX =
        (1 << XKB_اشاره_گر_BUTTON_شاخص_عرض) - 1,
    XKB_اشاره_گر_BUTTON_ماسک_عرض =
        حجم(xkb_اشاره_گر_button_ماسک_t) * CHAR_BIT,
    XKB_اشاره_گر_BUTTON_DEFAULT = 0,
    XKB_اشاره_گر_BUTTON_MIN = 1,
    XKB_اشاره_گر_BUTTON_MAX = 5,
};

static_assert(XKB_اشاره_گر_BUTTON_MAX <= XKB_اشاره_گر_BUTTON_شاخص_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_اشاره_گر_BUTTON_MAX <= XKB_اشاره_گر_BUTTON_ماسک_عرض,
              "xkb_pointer_button_mask_t cannot store button mask");

بنية xkb_اشاره_گر_default_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t مقدار;
};

تعداد {
    XKB_اشاره_گر_DEFAULT_ACTION_مقدار_عرض =
        حجم(((بنية xkb_اشاره_گر_default_action*)0)->مقدار) * CHAR_BIT,
    XKB_اشاره_گر_DEFAULT_ACTION_مقدار_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_اشاره_گر_DEFAULT_ACTION_مقدار_عرض - 1)) - 1,
};

static_assert((uint32_t)XKB_اشاره_گر_BUTTON_MAX <=
              (uint32_t)XKB_اشاره_گر_DEFAULT_ACTION_مقدار_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

بنية xkb_اشاره_گر_motion_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

بنية xkb_اشاره_گر_button_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    uint8_t شمار;
    xkb_اشاره_گر_button_شاخص_t button;
};

بنية xkb_switch_screen_action {
    تعداد xkb_action_type type;
    تعداد xkb_action_flags flags;
    int8_t screen;
};

بنية xkb_redirect_کلید_action {
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
    xkb_mod_ماسک_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_ماسک_t mods;
};

بنية xkb_خصوصی_action {
    تعداد xkb_action_type type;
    uint8_t داده[7];
};

تعداد xkb_داخلی_action_flags {
    داخلی_BREAKS_گروه_LATCH = (1 << 0),
    داخلی_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
بنية xkb_داخلی_action {
    تعداد xkb_action_type type;
    تعداد xkb_داخلی_action_flags flags;
    اتحاد {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_ماسک_t clear_latched_mods;
    };
};

اتحاد xkb_action {
    تعداد xkb_action_type type;
    بنية xkb_mod_action mods;
    بنية xkb_گروه_action گروه;
    بنية xkb_controls_action ctrls;
    بنية xkb_redirect_کلید_action redirect;
    بنية xkb_اشاره_گر_default_action dflt;
    بنية xkb_اشاره_گر_motion_action ptr;
    بنية xkb_اشاره_گر_button_action btn;
    بنية xkb_switch_screen_action screen;
    بنية xkb_خصوصی_action priv;
    بنية xkb_داخلی_action داخلی;
};

بنية xkb_کلید_type_مدخل {
    xkb_level_شاخص_t level;
    بنية xkb_mods mods;
    بنية xkb_mods preserve;
};

بنية xkb_کلید_type {
    xkb_atom_t name;
    بنية xkb_mods mods;
    bool required;
    xkb_level_شاخص_t رقم_سطوح;
    xkb_level_شاخص_t رقم_level_نامها;
    xkb_atom_t *level_نامها ATTR_COUNTED_BY(رقم_level_نامها);
    darray_اندازه_t رقم_entries;
    بنية xkb_کلید_type_مدخل *entries ATTR_COUNTED_BY(رقم_entries);
};

تعريفنوع uint16_t xkb_action_شمار_t;
#تعریف_کن MAX_ACTIONS_در_هر_LEVEL UINT16_MAX

بنية xkb_sym_interpret {
    xkb_keysym_t sym;
    تعداد xkb_تطابق_operation تطابق;
    xkb_mod_ماسک_t mods;
    xkb_mod_شاخص_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_شمار_t رقم_actions;
    اتحاد {
        /* num_actions <= 1 */
        اتحاد xkb_action action;
        /* num_actions >  1 */
        اتحاد xkb_action *actions;
    } a;
};

تعداد {XKB_STATE_COMPONENT_عرض = (حجم(تعداد xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_عرض) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

بنية xkb_led {
    xkb_atom_t name;
    تعداد xkb_state_component which_groups:(XKB_STATE_COMPONENT_عرض - 1);
    bool pending_groups:1;
    xkb_layout_ماسک_t groups;
    تعداد xkb_state_component which_mods;
    بنية xkb_mods mods;
    تعداد xkb_action_controls ctrls;
};

بنية xkb_کلید_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

بنية xkb_controls {
    بیعلامت حرف groups_wrap;
    بنية xkb_mods داخلی;
    بنية xkb_mods ignore_قفل;
    بیعلامت کوتاه repeat_delay;
    بیعلامت کوتاه repeat_interval;
    بیعلامت کوتاه slow_کلیدها_delay;
    بیعلامت کوتاه debounce_delay;
    بیعلامت کوتاه ax_options;
    بیعلامت کوتاه ax_timeout;
    بیعلامت کوتاه axt_opts_ماسک;
    بیعلامت کوتاه axt_opts_مقادیر;
    بیعلامت صحيح axt_ctrls_ماسک;
    بیعلامت صحيح axt_ctrls_مقادیر;
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

تعريفنوع uint16_t xkb_keysym_شمار_t;
#تعریف_کن MAX_KEYSYMS_در_هر_LEVEL UINT16_MAX

/** A key level */
بنية xkb_level {
    /** Count of keysyms */
    xkb_keysym_شمار_t رقم_syms;
    /** Count of actions */
    xkb_action_شمار_t رقم_actions;
    /** Upper keysym */
    اتحاد {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t فوقانی;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_فوقانی;
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
بنية xkb_گروه {
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
    ثابت بنية xkb_کلید_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    بنية xkb_level *سطوح;
};

تعداد {
    DEFAULT_کلید_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_کلید_REPEAT = true,
    FALLBACK_INTERPRET_کلید_REPEAT = DEFAULT_کلید_REPEAT
};

تعداد {
    DEFAULT_کلید_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_کلید_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_کلید_VMODMAP
};

بنية xkb_کلید {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    تعداد xkb_explicit_components explicit;

    xkb_mod_ماسک_t modmap;
    xkb_mod_ماسک_t vmodmap;

    xkb_overlay_ماسک_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool بیرون_از_range_pending_گروه:1;
    تعداد xkb_layout_بیرون_از_range_policy بیرون_از_range_گروه_policy:4;
    xkb_layout_شاخص_t بیرون_از_range_گروه_عدد:8;

    xkb_layout_شاخص_t رقم_groups:8;
    بنية xkb_گروه *groups ATTR_COUNTED_BY(رقم_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    اتحاد {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ثابت بنية xkb_کلید *overlay_کلید;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ثابت بنية xkb_کلید **overlays_کلیدها;
    };
};

بنية xkb_mod {
    xkb_atom_t name;
    تعداد mod_type type;
    xkb_mod_ماسک_t mapping; /* vmod -> real mod mapping */
};

بنية xkb_mod_مجموعه {
    بنية xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_شاخص_t رقم_mods;
    xkb_mod_ماسک_t explicit_vmods;
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
#تعریف_کن XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(حجم(بنية xkb_کلید)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#تعریف_کن XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(حجم(بنية xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_اندازه_T_عرض - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
تعريفنوع اتحاد {
    بنية {
        /** If true, the index is valid */
        bool یافت:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_اندازه_t:(DARRAY_اندازه_T_عرض - 3);
    };
    /* Only if is_alias = false, for better semantics */
    بنية {
        bool یافت:1;
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
        darray_اندازه_t شاخص:(DARRAY_اندازه_T_عرض - 3);
    } کلید;
    /* Only if is_alias = true, for better semantics */
    بنية {
        bool یافت:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_اندازه_T_عرض - 3);
    } alias;
} Keycode_تطابق;

تعريفنوع بنية {
    اتحاد {
        /** Used for parsing */
        تعداد ادغام_نما ادغام;
        /** Used for serializing */
        xkb_keycode_t کلید_کد;
    };
    bool have_نماد;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_ماسک_t mods;
    اتحاد {
        xkb_keysym_t کلید_Sym;
        xkb_atom_t کلید_Name;
    } u;
} Mod_Map_مدخل;

/** Common keyboard description structure */
بنية xkb_keymap {
    بنية xkb_context *ctx;

    صحيح refcnt;
    تعداد xkb_keymap_compile_flags flags;
    تعداد xkb_keymap_format format;

    xkb_led_شاخص_t رقم_leds;
    بنية xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_کلید_کد;
    xkb_keycode_t max_کلید_کد;
    xkb_keycode_t رقم_کلیدها;
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
    xkb_keycode_t رقم_کلیدها_کم;
    بنية xkb_کلید *کلیدها ATTR_COUNTED_BY(رقم_کلیدها);

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
            darray_اندازه_t رقم_کلید_نامها;
            Keycode_تطابق *کلید_نامها;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        بنية {
            darray_اندازه_t رقم_کلید_aliases;
            بنية xkb_کلید_alias *کلید_aliases;
        };
    };

    بنية xkb_کلید_type *types;
    darray_اندازه_t رقم_types;

    darray_اندازه_t رقم_sym_interprets;
    بنية xkb_sym_interpret *sym_interprets;

    darray_اندازه_t _padding;
    darray_اندازه_t رقم_modmaps;
    Mod_Map_مدخل *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    بنية xkb_mod_مجموعه mods;
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
    xkb_mod_ماسک_t canonical_state_ماسک;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_کلید_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_شاخص_t رقم_groups;
    /* Not all groups must have names. */
    xkb_layout_شاخص_t رقم_گروه_نامها;
    xkb_atom_t *گروه_نامها ATTR_COUNTED_BY(رقم_گروه_نامها);

    حرف *keycodes_section_name;
    حرف *symbols_section_name;
    حرف *types_section_name;
    حرف *compat_section_name;
};

#تعریف_کن xkb_کلیدها_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    لكل ((iter) = (keymap)->کلیدها + \
                  ((keymap)->رقم_کلیدها_کم == 0 ? 0 : (keymap)->min_کلید_کد); \
         (iter) < (keymap)->کلیدها + (keymap)->رقم_کلیدها; \
         (iter)++)

#تعریف_کن xkb_level_foreach_action(iter, level)                 \
    لكل (اتحاد xkb_action                                     \
         *(iter) = (likely((level)->رقم_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ثابت iter##_last_ = (iter) + (level)->رقم_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#تعریف_کن xkb_mods_foreach(iter, mods_) \
    لكل ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->رقم_mods; \
         (iter)++)

#تعریف_کن xkb_mods_ماسک_foreach(ماسک, iter, mods_) \
    لكل ((iter) = (mods_)->mods; \
         (ماسک) && (iter) < (mods_)->mods + (mods_)->رقم_mods; \
         (iter)++, (ماسک) >>= 1) \
        إذا ((ماسک) & 0x1)

/** Enumerate all modifiers */
#تعریف_کن xkb_mods_enumerate(idx, iter, mods_) \
    لكل ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->رقم_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#تعریف_کن xkb_rmods_enumerate(idx, iter, mods_) \
    لكل ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_شاخص_رقم_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#تعریف_کن xkb_vmods_enumerate(idx, iter, mods_) \
    لكل ((idx) = XKB_MOD_شاخص_رقم_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_شاخص_رقم_ENTRIES]; \
         (idx) < (mods_)->رقم_mods; \
         (idx)++, (iter)++)

#تعریف_کن xkb_leds_foreach(iter, keymap) \
    لكل ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->رقم_leds; \
         (iter)++)

#تعریف_کن xkb_leds_enumerate(idx, iter, keymap) \
    لكل ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->رقم_leds; \
         (idx)++, (iter)++)

فراغ
clear_level(بنية xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ساكن مضمن بنية xkb_کلید *
Xkb_کلید_By_Name(ثابت بنية xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    إذا (name < keymap->رقم_کلید_نامها) {
        ثابت Keycode_تطابق تطابق = keymap->کلید_نامها[name];
        إذا (تطابق.یافت) {
            إذا (!تطابق.is_alias) {
                أكد(name == keymap->کلیدها[تطابق.کلید.شاخص].name);
                أرجع &keymap->کلیدها[تطابق.کلید.شاخص];
            } وإلا إذا (use_aliases) {
                أكد(تطابق.alias.real ==
                       keymap->کلیدها[keymap->کلید_نامها[تطابق.alias.real].کلید.شاخص].name);
                أرجع &keymap->کلیدها[keymap->کلید_نامها[تطابق.alias.real].کلید.شاخص];
            }
        }
    }
    أرجع عدم;
}

ساكن مضمن ثابت بنية xkb_کلید *
Xkb_کلید(ثابت بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    إذا (kc < keymap->min_کلید_کد || kc > keymap->max_کلید_کد) {
        /* Unsupported keycode */
        أرجع عدم;
    } وإلا إذا (kc < keymap->رقم_کلیدها_کم) {
        /* Low keycodes */
        أرجع &keymap->کلیدها[kc];
    } وإلا {
        /* High keycodes: use binary search */
        xkb_keycode_t پایینی = keymap->رقم_کلیدها_کم;
        xkb_keycode_t فوقانی = keymap->رقم_کلیدها;
        طالما (پایینی < فوقانی) {
            ثابت xkb_keycode_t mid = پایینی + (فوقانی - 1 - پایینی) / 2;
            ثابت بنية xkb_کلید * ثابت کلید = &keymap->کلیدها[mid];
            إذا (کلید->keycode < kc) {
                پایینی = mid + 1;
            } وإلا إذا (کلید->keycode > kc) {
                فوقانی = mid;
            } وإلا {
                أرجع کلید;
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
ساكن مضمن ثابت بنية xkb_کلید *
xkb_keymap_get_بعدی_defined_کلید(ثابت بنية xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    إذا (!keymap->رقم_کلیدها || kc > XKB_KEYCODE_MAX) {
        أرجع عدم;
    } وإلا إذا (kc < keymap->min_کلید_کد) {
        kc = keymap->min_کلید_کد;
    } وإلا إذا (kc > keymap->max_کلید_کد) {
        kc = keymap->max_کلید_کد;
    }

    إذا (kc < keymap->رقم_کلیدها_کم) {
        /* Low keycodes */
        ثابت بنية xkb_کلید *کلید = &keymap->کلیدها[kc];
        إذا (ascending) {
            ثابت بنية xkb_کلید *انتها = &keymap->کلیدها[keymap->رقم_کلیدها_کم - 1];
            طالما (کلید->name == XKB_ATOM_NONE && کلید < انتها) کلید++;
        } وإلا {
            ثابت بنية xkb_کلید *انتها = &keymap->کلیدها[keymap->min_کلید_کد];
            طالما (کلید->name == XKB_ATOM_NONE && کلید > انتها) کلید--;
        }
        أرجع (کلید->name == XKB_ATOM_NONE) ? عدم : کلید;
    } وإلا {
        /* High keycodes: use binary search */
        xkb_keycode_t پایینی = keymap->رقم_کلیدها_کم
            ? keymap->رقم_کلیدها_کم - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t فوقانی = keymap->رقم_کلیدها - 1;
        ثابت بنية xkb_کلید * candidate = عدم;
        طالما (پایینی <= فوقانی) {
            ثابت xkb_keycode_t mid = پایینی + (فوقانی - پایینی) / 2;
            ثابت بنية xkb_کلید * ثابت کلید = &keymap->کلیدها[mid];
            إذا (کلید->keycode == kc) {
                أرجع کلید;
            } وإلا إذا (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                إذا (کلید->keycode >= kc) {
                    candidate = کلید;
                    فوقانی = mid - 1;
                } وإلا {
                    پایینی = mid + 1;
                }
            } وإلا {
                /* Looking for the *smallest* defined keycode <= kc */
                إذا (کلید->keycode <= kc) {
                    candidate = کلید;
                    پایینی = mid + 1;
                } وإلا إذا (کلید->keycode > kc) {
                    فوقانی = mid - 1;
                }
            }
        }
        أرجع candidate;
    }
}

ساكن مضمن xkb_level_شاخص_t
Xkb_کلید_رقم_سطوح(ثابت بنية xkb_کلید *کلید, xkb_layout_شاخص_t layout)
{
    أرجع کلید->groups[layout].type->رقم_سطوح;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ساكن مضمن bool
مدخل_is_فعال(ثابت بنية xkb_کلید_type_مدخل *مدخل)
{
    أرجع مدخل->mods.mods == 0 || مدخل->mods.ماسک != 0;
}

بنية xkb_keymap *
xkb_keymap_new(بنية xkb_context *ctx, ثابت حرف* func,
               تعداد xkb_keymap_format format,
               تعداد xkb_keymap_compile_flags flags);

فراغ
XkbEscapeMapName(حرف *name);

xkb_mod_شاخص_t
Xkb_Mod_Name_مقصد_شاخص(ثابت بنية xkb_mod_مجموعه *mods, xkb_atom_t name,
                  تعداد mod_type type);

bool
Xkb_سطوح_Same_Syms(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

bool
action_equal(ثابت اتحاد xkb_action *a, ثابت اتحاد xkb_action *b);

bool
Xkb_سطوح_Same_Actions(ثابت بنية xkb_level *a, ثابت بنية xkb_level *b);

xkb_layout_شاخص_t
Xkb_Wrap_گروه_Into_Range(int32_t گروه,
                      xkb_layout_شاخص_t رقم_groups,
                      تعداد xkb_layout_بیرون_از_range_policy بیرون_از_range_گروه_policy,
                      xkb_layout_شاخص_t بیرون_از_range_گروه_عدد);

ساكن مضمن xkb_layout_شاخص_t
xkb_keymap_کلید_effective_layout(ثابت بنية xkb_کلید *کلید,
                                xkb_layout_شاخص_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    أرجع Xkb_Wrap_گروه_Into_Range((int32_t) layout,
                                 کلید->رقم_groups,
                                 کلید->بیرون_از_range_گروه_policy,
                                 کلید->بیرون_از_range_گروه_عدد);
}

XKB_EXPORT_خصوصی xkb_mod_ماسک_t
mod_ماسک_get_effective(بنية xkb_keymap *keymap, xkb_mod_ماسک_t mods);

بنية xkb_level *
xkb_keymap_کلید_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_کلید *کلید,
                         xkb_layout_شاخص_t layout, xkb_level_شاخص_t level);

xkb_action_شمار_t
xkb_keymap_کلید_get_actions_by_level(ثابت بنية xkb_keymap *keymap,
                                    ثابت بنية xkb_کلید *کلید,
                                    xkb_layout_شاخص_t layout,
                                    xkb_level_شاخص_t level,
                                    ثابت اتحاد xkb_action **actions);

بنية xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_نامها)(بنية xkb_keymap *keymap,
                                  ثابت بنية xkb_قاعده_نامها *نامها);
    bool (*keymap_new_from_string)(بنية xkb_keymap *keymap,
                                   ثابت حرف *string, اندازه_t درازی);
    bool (*keymap_new_from_file)(بنية xkb_keymap *keymap, FILE *file);
    تعداد xkb_وضعیت (*keymap_serialize)(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_نتیجه *نتیجه
    );
};

خارجي ثابت بنية xkb_keymap_format_ops متن_v1_keymap_format_ops;

ساكن مضمن bool
is_Mods_Un_قفل_On_Press_Supported(تعداد xkb_keymap_format format)
{
    /* Lax bound */
    أرجع format >= XKB_KEYMAP_FORMAT_متن_V2;
}

ساكن مضمن bool
is_گروه_قفل_On_Release_Supported(تعداد xkb_keymap_format format)
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
