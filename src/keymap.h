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
#ਹਦਾਇਤ once

 /* Don't use compat names in internal code. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _XKBCOMMON_COMPAT_H

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MAX_GROUPS 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MAX_GROUPS_X11 4

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_layout_ਸੂਚਕਾਂਕ_t
format_max_groups(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MAX_LEDS ((xkb_led_ਸੂਚਕਾਂਕ_t) (ਆਕਾਰ(xkb_led_ਨਕਾਬ_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MAX_MODS ((xkb_mod_ਸੂਚਕਾਂਕ_t) (ਆਕਾਰ(xkb_mod_ਨਕਾਬ_t) * CHAR_BIT))

ਗਿਣਤੀ {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MOD_NONE 0xffffffffU

ਗਿਣਤੀ mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MOD_REAL_ਨਕਾਬ_ALL ((xkb_mod_ਨਕਾਬ_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
ਗਿਣਤੀ real_mod_ਸੂਚਕਾਂਕ {
    XKB_MOD_ਸੂਚਕਾਂਕ_ਖਿਸਕਾਓ = 0,
    XKB_MOD_ਸੂਚਕਾਂਕ_CAPS,
    XKB_MOD_ਸੂਚਕਾਂਕ_CTRL,
    XKB_MOD_ਸੂਚਕਾਂਕ_MOD1,
    XKB_MOD_ਸੂਚਕਾਂਕ_MOD2,
    XKB_MOD_ਸੂਚਕਾਂਕ_MOD3,
    XKB_MOD_ਸੂਚਕਾਂਕ_MOD4,
    XKB_MOD_ਸੂਚਕਾਂਕ_MOD5,
    XKB_MOD_ਸੂਚਕਾਂਕ_ਅੰਕ_ENTRIES
};
static_assert(XKB_MOD_ਸੂਚਕਾਂਕ_ਅੰਕ_ENTRIES == 8, "Invalid X11 core modifiers");

ਗਿਣਤੀ xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_ਸੈੱਟ,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_ਤਾਲਾ,
    ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ,
    ACTION_TYPE_ਸਮੂਹ_LATCH,
    ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_ਤਾਲਾ,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_ਸੈੱਟ,
    ACTION_TYPE_CTRL_ਤਾਲਾ,
    ACTION_TYPE_REDIRECT_ਕੁੰਜੀ,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_ਨਿੱਜੀ,
    ACTION_TYPE_ਅੰਦਰੂਨੀ, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_ਅੰਕ_ENTRIES
};

ਗਿਣਤੀ xkb_action_flags {
    ACTION_ਤਾਲਾ_CLEAR = (1 << 0),
    ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ = (1 << 1),
    ACTION_ਤਾਲਾ_NO_ਤਾਲਾ = (1 << 2),
    ACTION_ਤਾਲਾ_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_ਤਾਲਾ_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_ਤਾਲਾ_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_ਸਮੂਹ_ਤਾਲਾ_ON_MODIFIER_RELEASE = (1 << 16),
};

ਗਿਣਤੀ {
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
ਗਿਣਤੀ xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_ਕੁੰਜੀਆਂ = (1 << 0),
    CONTROL_MOUSE_ਕੁੰਜੀਆਂ = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_ਸਮੂਹ_ਤਾਲਾ = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    ਆਖਰੀ_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ਕੁੰਜੀਆਂ | CONTROL_MOUSE_ਕੁੰਜੀਆਂ | CONTROL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_ਸਮੂਹ_ਤਾਲਾ | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_ਕੁੰਜੀਆਂ | CONTROL_MOUSE_ਕੁੰਜੀਆਂ | CONTROL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_ਸਮੂਹ_ਤਾਲਾ),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(ਆਕਾਰ(ਗਿਣਤੀ xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)ਆਖਰੀ_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_action_controls
format_boolean_controls(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_ਕੁੰਜੀਆਂ ==
    (ਗਿਣਤੀ xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_ਕੁੰਜੀਆਂ ==
    (ਗਿਣਤੀ xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ,
    "Private value should match public API"
);


ਗਿਣਤੀ xkb_ਮੇਲ_operation {
    ਮੇਲ_NONE,
    ਮੇਲ_ANY_OR_NONE,
    ਮੇਲ_ANY,
    ਮੇਲ_ALL,
    ਮੇਲ_EXACTLY,
};

ਬਣਤਰ xkb_mods {
    xkb_mod_ਨਕਾਬ_t mods;       /* original real+virtual mods in definition */
    xkb_mod_ਨਕਾਬ_t ਨਕਾਬ;       /* computed effective mask */
};

ਬਣਤਰ xkb_mod_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    ਬਣਤਰ xkb_mods mods;
};

ਬਣਤਰ xkb_ਸਮੂਹ_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    int32_t ਸਮੂਹ;
};

/** Keyboard overlay index or count */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t xkb_overlay_ਸੂਚਕਾਂਕ_t;
/** Keyboard overlay mask */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t xkb_overlay_ਨਕਾਬ_t;
ਗਿਣਤੀ {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ਗਿਣਤੀ_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_ਗਿਣਤੀ = (ਆਕਾਰ(xkb_overlay_ਨਕਾਬ_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_ਗਿਣਤੀ - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_ਸੂਚਕਾਂਕ_MIN_ਚੌੜਾਈ = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_ਗਿਣਤੀ_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_ਗਿਣਤੀ) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_ਗਿਣਤੀ ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_ਗਿਣਤੀ < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_ਗਿਣਤੀ <= (1u << XKB_OVERLAY_ਸੂਚਕਾਂਕ_MIN_ਚੌੜਾਈ) - 1,
              "Cannot encode overlay index or count in a nibble");

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_overlay_ਨਕਾਬ_t
overlays_from_controls(ਗਿਣਤੀ xkb_action_controls controls)
{
    ਵਾਪਸ (xkb_overlay_ਨਕਾਬ_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_action_controls
overlays_ਮੰਜ਼ਿਲ_controls(xkb_overlay_ਨਕਾਬ_t overlays)
{
    ਵਾਪਸ (ਗਿਣਤੀ xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_overlay_ਸੂਚਕਾਂਕ_t
format_max_overlays(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
        ? XKB_OVERLAY_ਗਿਣਤੀ_X11
        : XKB_OVERLAY_ਗਿਣਤੀ;
}

ਬਣਤਰ xkb_controls_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    ਗਿਣਤੀ xkb_action_controls ctrls;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t xkb_ਸੂਚਕ_button_ਨਕਾਬ_t;

ਗਿਣਤੀ {
    XKB_ਸੂਚਕ_BUTTON_ਸੂਚਕਾਂਕ_ਚੌੜਾਈ =
        ਆਕਾਰ(xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t) * CHAR_BIT,
    XKB_ਸੂਚਕ_BUTTON_ਸੂਚਕਾਂਕ_MAX =
        (1 << XKB_ਸੂਚਕ_BUTTON_ਸੂਚਕਾਂਕ_ਚੌੜਾਈ) - 1,
    XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ =
        ਆਕਾਰ(xkb_ਸੂਚਕ_button_ਨਕਾਬ_t) * CHAR_BIT,
    XKB_ਸੂਚਕ_BUTTON_DEFAULT = 0,
    XKB_ਸੂਚਕ_BUTTON_MIN = 1,
    XKB_ਸੂਚਕ_BUTTON_MAX = 5,
};

static_assert(XKB_ਸੂਚਕ_BUTTON_MAX <= XKB_ਸੂਚਕ_BUTTON_ਸੂਚਕਾਂਕ_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_ਸੂਚਕ_BUTTON_MAX <= XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ,
              "xkb_pointer_button_mask_t cannot store button mask");

ਬਣਤਰ xkb_ਸੂਚਕ_default_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    int8_t ਮੁੱਲ;
};

ਗਿਣਤੀ {
    XKB_ਸੂਚਕ_DEFAULT_ACTION_ਮੁੱਲ_ਚੌੜਾਈ =
        ਆਕਾਰ(((ਬਣਤਰ xkb_ਸੂਚਕ_default_action*)0)->ਮੁੱਲ) * CHAR_BIT,
    XKB_ਸੂਚਕ_DEFAULT_ACTION_ਮੁੱਲ_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_ਸੂਚਕ_DEFAULT_ACTION_ਮੁੱਲ_ਚੌੜਾਈ - 1)) - 1,
};

static_assert((uint32_t)XKB_ਸੂਚਕ_BUTTON_MAX <=
              (uint32_t)XKB_ਸੂਚਕ_DEFAULT_ACTION_ਮੁੱਲ_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

ਬਣਤਰ xkb_ਸੂਚਕ_motion_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

ਬਣਤਰ xkb_ਸੂਚਕ_button_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    uint8_t ਗਿਣਤੀ_2;
    xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t button;
};

ਬਣਤਰ xkb_switch_screen_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_action_flags flags;
    int8_t screen;
};

ਬਣਤਰ xkb_redirect_ਕੁੰਜੀ_action {
    ਗਿਣਤੀ xkb_action_type type;
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
    xkb_mod_ਨਕਾਬ_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_ਨਕਾਬ_t mods;
};

ਬਣਤਰ xkb_ਨਿੱਜੀ_action {
    ਗਿਣਤੀ xkb_action_type type;
    uint8_t ਅੰਕੜੇ[7];
};

ਗਿਣਤੀ xkb_ਅੰਦਰੂਨੀ_action_flags {
    ਅੰਦਰੂਨੀ_BREAKS_ਸਮੂਹ_LATCH = (1 << 0),
    ਅੰਦਰੂਨੀ_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
ਬਣਤਰ xkb_ਅੰਦਰੂਨੀ_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਗਿਣਤੀ xkb_ਅੰਦਰੂਨੀ_action_flags flags;
    ਸੰਘ {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_ਨਕਾਬ_t clear_latched_mods;
    };
};

ਸੰਘ xkb_action {
    ਗਿਣਤੀ xkb_action_type type;
    ਬਣਤਰ xkb_mod_action mods;
    ਬਣਤਰ xkb_ਸਮੂਹ_action ਸਮੂਹ;
    ਬਣਤਰ xkb_controls_action ctrls;
    ਬਣਤਰ xkb_redirect_ਕੁੰਜੀ_action redirect;
    ਬਣਤਰ xkb_ਸੂਚਕ_default_action dflt;
    ਬਣਤਰ xkb_ਸੂਚਕ_motion_action ptr;
    ਬਣਤਰ xkb_ਸੂਚਕ_button_action btn;
    ਬਣਤਰ xkb_switch_screen_action screen;
    ਬਣਤਰ xkb_ਨਿੱਜੀ_action priv;
    ਬਣਤਰ xkb_ਅੰਦਰੂਨੀ_action ਅੰਦਰੂਨੀ;
};

ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ {
    xkb_level_ਸੂਚਕਾਂਕ_t level;
    ਬਣਤਰ xkb_mods mods;
    ਬਣਤਰ xkb_mods preserve;
};

ਬਣਤਰ xkb_ਕੁੰਜੀ_type {
    xkb_atom_t name;
    ਬਣਤਰ xkb_mods mods;
    bool required;
    xkb_level_ਸੂਚਕਾਂਕ_t ਅੰਕ_ਪੱਧਰ;
    xkb_level_ਸੂਚਕਾਂਕ_t ਅੰਕ_level_ਨਾਂ;
    xkb_atom_t *level_ਨਾਂ ATTR_COUNTED_BY(ਅੰਕ_level_ਨਾਂ);
    darray_ਮਾਪ_t ਅੰਕ_entries;
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *entries ATTR_COUNTED_BY(ਅੰਕ_entries);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t xkb_action_ਗਿਣਤੀ_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL UINT16_MAX

ਬਣਤਰ xkb_sym_interpret {
    xkb_keysym_t sym;
    ਗਿਣਤੀ xkb_ਮੇਲ_operation ਮੇਲ;
    xkb_mod_ਨਕਾਬ_t mods;
    xkb_mod_ਸੂਚਕਾਂਕ_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_ਗਿਣਤੀ_t ਅੰਕ_actions;
    ਸੰਘ {
        /* num_actions <= 1 */
        ਸੰਘ xkb_action action;
        /* num_actions >  1 */
        ਸੰਘ xkb_action *actions;
    } a;
};

ਗਿਣਤੀ {XKB_STATE_COMPONENT_ਚੌੜਾਈ = (ਆਕਾਰ(ਗਿਣਤੀ xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_ਚੌੜਾਈ) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

ਬਣਤਰ xkb_led {
    xkb_atom_t name;
    ਗਿਣਤੀ xkb_state_component which_groups:(XKB_STATE_COMPONENT_ਚੌੜਾਈ - 1);
    bool pending_groups:1;
    xkb_layout_ਨਕਾਬ_t groups;
    ਗਿਣਤੀ xkb_state_component which_mods;
    ਬਣਤਰ xkb_mods mods;
    ਗਿਣਤੀ xkb_action_controls ctrls;
};

ਬਣਤਰ xkb_ਕੁੰਜੀ_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

ਬਣਤਰ xkb_controls {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ groups_wrap;
    ਬਣਤਰ xkb_mods ਅੰਦਰੂਨੀ;
    ਬਣਤਰ xkb_mods ignore_ਤਾਲਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ repeat_delay;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ repeat_interval;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ slow_ਕੁੰਜੀਆਂ_delay;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ debounce_delay;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ax_options;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ax_timeout;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ axt_opts_ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ axt_opts_ਮੁੱਲਾਂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axt_ctrls_ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axt_ctrls_ਮੁੱਲਾਂ;
};

ਗਿਣਤੀ xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t xkb_keysym_ਗਿਣਤੀ_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_KEYSYMS_ਪ੍ਰਤੀ_LEVEL UINT16_MAX

/** A key level */
ਬਣਤਰ xkb_level {
    /** Count of keysyms */
    xkb_keysym_ਗਿਣਤੀ_t ਅੰਕ_syms;
    /** Count of actions */
    xkb_action_ਗਿਣਤੀ_t ਅੰਕ_actions;
    /** Upper keysym */
    ਸੰਘ {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t ਉੱਤਲਾ;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_ਉੱਤਲਾ;
    };
    /** Keysyms */
    ਸੰਘ {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    ਸੰਘ {
        ਸੰਘ xkb_action action;   /* num_actions == 1 */
        ਸੰਘ xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
ਬਣਤਰ xkb_ਸਮੂਹ {
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
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    ਬਣਤਰ xkb_level *ਪੱਧਰ;
};

ਗਿਣਤੀ {
    DEFAULT_ਕੁੰਜੀ_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_ਕੁੰਜੀ_REPEAT = true,
    FALLBACK_INTERPRET_ਕੁੰਜੀ_REPEAT = DEFAULT_ਕੁੰਜੀ_REPEAT
};

ਗਿਣਤੀ {
    DEFAULT_ਕੁੰਜੀ_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_ਕੁੰਜੀ_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_ਕੁੰਜੀ_VMODMAP
};

ਬਣਤਰ xkb_ਕੁੰਜੀ {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    ਗਿਣਤੀ xkb_explicit_components explicit;

    xkb_mod_ਨਕਾਬ_t modmap;
    xkb_mod_ਨਕਾਬ_t vmodmap;

    xkb_overlay_ਨਕਾਬ_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ:1;
    ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy:4;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ:8;

    xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups:8;
    ਬਣਤਰ xkb_ਸਮੂਹ *groups ATTR_COUNTED_BY(ਅੰਕ_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ਸੰਘ {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *overlay_ਕੁੰਜੀ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **overlays_ਕੁੰਜੀਆਂ;
    };
};

ਬਣਤਰ xkb_mod {
    xkb_atom_t name;
    ਗਿਣਤੀ mod_type type;
    xkb_mod_ਨਕਾਬ_t mapping; /* vmod -> real mod mapping */
};

ਬਣਤਰ xkb_mod_ਸੈੱਟ {
    ਬਣਤਰ xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_ਸੂਚਕਾਂਕ_t ਅੰਕ_mods;
    xkb_mod_ਨਕਾਬ_t explicit_vmods;
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ਆਕਾਰ(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(ਆਕਾਰ(ਬਣਤਰ xkb_ਕੁੰਜੀ)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ਆਕਾਰ(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(ਆਕਾਰ(ਬਣਤਰ xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_ਮਾਪ_T_ਚੌੜਾਈ - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੰਘ {
    ਬਣਤਰ {
        /** If true, the index is valid */
        bool ਮਿਲਿਆ:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_ਮਾਪ_t:(DARRAY_ਮਾਪ_T_ਚੌੜਾਈ - 3);
    };
    /* Only if is_alias = false, for better semantics */
    ਬਣਤਰ {
        bool ਮਿਲਿਆ:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool ਨੀਵਾਂ:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_ਮਾਪ_t ਸੂਚਕਾਂਕ:(DARRAY_ਮਾਪ_T_ਚੌੜਾਈ - 3);
    } ਕੁੰਜੀ;
    /* Only if is_alias = true, for better semantics */
    ਬਣਤਰ {
        bool ਮਿਲਿਆ:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_ਮਾਪ_T_ਚੌੜਾਈ - 3);
    } alias;
} Keycode_ਮੇਲ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਸੰਘ {
        /** Used for parsing */
        ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
        /** Used for serializing */
        xkb_keycode_t ਕੁੰਜੀ_ਕੋਡ;
    };
    bool have_ਚਿੰਨ੍ਹ;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_ਨਕਾਬ_t mods;
    ਸੰਘ {
        xkb_keysym_t ਕੁੰਜੀ_Sym;
        xkb_atom_t ਕੁੰਜੀ_Name;
    } u;
} Mod_Map_ਇੰਦਰਾਜ;

/** Common keyboard description structure */
ਬਣਤਰ xkb_keymap {
    ਬਣਤਰ xkb_context *ctx;

    ਪੂਰਨ refcnt;
    ਗਿਣਤੀ xkb_keymap_compile_flags flags;
    ਗਿਣਤੀ xkb_keymap_format format;

    xkb_led_ਸੂਚਕਾਂਕ_t ਅੰਕ_leds;
    ਬਣਤਰ xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_ਕੁੰਜੀ_ਕੋਡ;
    xkb_keycode_t max_ਕੁੰਜੀ_ਕੋਡ;
    xkb_keycode_t ਅੰਕ_ਕੁੰਜੀਆਂ;
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
    xkb_keycode_t ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ;
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀਆਂ ATTR_COUNTED_BY(ਅੰਕ_ਕੁੰਜੀਆਂ);

    ਸੰਘ {
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
        ਬਣਤਰ {
            darray_ਮਾਪ_t ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ;
            Keycode_ਮੇਲ *ਕੁੰਜੀ_ਨਾਂ;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        ਬਣਤਰ {
            darray_ਮਾਪ_t ਅੰਕ_ਕੁੰਜੀ_aliases;
            ਬਣਤਰ xkb_ਕੁੰਜੀ_alias *ਕੁੰਜੀ_aliases;
        };
    };

    ਬਣਤਰ xkb_ਕੁੰਜੀ_type *types;
    darray_ਮਾਪ_t ਅੰਕ_types;

    darray_ਮਾਪ_t ਅੰਕ_sym_interprets;
    ਬਣਤਰ xkb_sym_interpret *sym_interprets;

    darray_ਮਾਪ_t _padding;
    darray_ਮਾਪ_t ਅੰਕ_modmaps;
    Mod_Map_ਇੰਦਰਾਜ *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    ਬਣਤਰ xkb_mod_ਸੈੱਟ mods;
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
    xkb_mod_ਨਕਾਬ_t canonical_state_ਨਕਾਬ;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_ਕੁੰਜੀ_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups;
    /* Not all groups must have names. */
    xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_ਸਮੂਹ_ਨਾਂ;
    xkb_atom_t *ਸਮੂਹ_ਨਾਂ ATTR_COUNTED_BY(ਅੰਕ_ਸਮੂਹ_ਨਾਂ);

    ਅੱਖਰ *keycodes_section_name;
    ਅੱਖਰ *symbols_section_name;
    ਅੱਖਰ *types_section_name;
    ਅੱਖਰ *compat_section_name;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_ਕੁੰਜੀਆਂ_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    ਲਈ ((iter) = (keymap)->ਕੁੰਜੀਆਂ + \
                  ((keymap)->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ == 0 ? 0 : (keymap)->min_ਕੁੰਜੀ_ਕੋਡ); \
         (iter) < (keymap)->ਕੁੰਜੀਆਂ + (keymap)->ਅੰਕ_ਕੁੰਜੀਆਂ; \
         (iter)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_level_foreach_action(iter, level)                 \
    ਲਈ (ਸੰਘ xkb_action                                     \
         *(iter) = (likely((level)->ਅੰਕ_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * ਅਚਲ iter##_last_ = (iter) + (level)->ਅੰਕ_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_mods_foreach(iter, mods_) \
    ਲਈ ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->ਅੰਕ_mods; \
         (iter)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_mods_ਨਕਾਬ_foreach(ਨਕਾਬ, iter, mods_) \
    ਲਈ ((iter) = (mods_)->mods; \
         (ਨਕਾਬ) && (iter) < (mods_)->mods + (mods_)->ਅੰਕ_mods; \
         (iter)++, (ਨਕਾਬ) >>= 1) \
        ਜੇ ((ਨਕਾਬ) & 0x1)

/** Enumerate all modifiers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_mods_enumerate(idx, iter, mods_) \
    ਲਈ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->ਅੰਕ_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_rmods_enumerate(idx, iter, mods_) \
    ਲਈ ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_ਸੂਚਕਾਂਕ_ਅੰਕ_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_vmods_enumerate(idx, iter, mods_) \
    ਲਈ ((idx) = XKB_MOD_ਸੂਚਕਾਂਕ_ਅੰਕ_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_ਸੂਚਕਾਂਕ_ਅੰਕ_ENTRIES]; \
         (idx) < (mods_)->ਅੰਕ_mods; \
         (idx)++, (iter)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_leds_foreach(iter, keymap) \
    ਲਈ ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->ਅੰਕ_leds; \
         (iter)++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_leds_enumerate(idx, iter, keymap) \
    ਲਈ ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->ਅੰਕ_leds; \
         (idx)++, (iter)++)

ਖਾਲੀ
clear_level(ਬਣਤਰ xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ xkb_ਕੁੰਜੀ *
Xkb_ਕੁੰਜੀ_By_Name(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    ਜੇ (name < keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ) {
        ਅਚਲ Keycode_ਮੇਲ ਮੇਲ = keymap->ਕੁੰਜੀ_ਨਾਂ[name];
        ਜੇ (ਮੇਲ.ਮਿਲਿਆ) {
            ਜੇ (!ਮੇਲ.is_alias) {
                ਪ੍ਰਤਿਜ੍ਞਾ(name == keymap->ਕੁੰਜੀਆਂ[ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ].name);
                ਵਾਪਸ &keymap->ਕੁੰਜੀਆਂ[ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ];
            } ਨਹੀਂਤਾਂ ਜੇ (use_aliases) {
                ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.alias.real ==
                       keymap->ਕੁੰਜੀਆਂ[keymap->ਕੁੰਜੀ_ਨਾਂ[ਮੇਲ.alias.real].ਕੁੰਜੀ.ਸੂਚਕਾਂਕ].name);
                ਵਾਪਸ &keymap->ਕੁੰਜੀਆਂ[keymap->ਕੁੰਜੀ_ਨਾਂ[ਮੇਲ.alias.real].ਕੁੰਜੀ.ਸੂਚਕਾਂਕ];
            }
        }
    }
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *
Xkb_ਕੁੰਜੀ(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ਜੇ (kc < keymap->min_ਕੁੰਜੀ_ਕੋਡ || kc > keymap->max_ਕੁੰਜੀ_ਕੋਡ) {
        /* Unsupported keycode */
        ਵਾਪਸ ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ ਜੇ (kc < keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ) {
        /* Low keycodes */
        ਵਾਪਸ &keymap->ਕੁੰਜੀਆਂ[kc];
    } ਨਹੀਂਤਾਂ {
        /* High keycodes: use binary search */
        xkb_keycode_t ਹੇਠਲਾ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ;
        xkb_keycode_t ਉੱਤਲਾ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ;
        ਜਦੋਂ (ਹੇਠਲਾ < ਉੱਤਲਾ) {
            ਅਚਲ xkb_keycode_t mid = ਹੇਠਲਾ + (ਉੱਤਲਾ - 1 - ਹੇਠਲਾ) / 2;
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[mid];
            ਜੇ (ਕੁੰਜੀ->keycode < kc) {
                ਹੇਠਲਾ = mid + 1;
            } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->keycode > kc) {
                ਉੱਤਲਾ = mid;
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ ਕੁੰਜੀ;
            }
        }
        ਵਾਪਸ ਨਿਰਾ;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *
xkb_keymap_get_ਅਗਲਾ_defined_ਕੁੰਜੀ(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    ਜੇ (!keymap->ਅੰਕ_ਕੁੰਜੀਆਂ || kc > XKB_KEYCODE_MAX) {
        ਵਾਪਸ ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ ਜੇ (kc < keymap->min_ਕੁੰਜੀ_ਕੋਡ) {
        kc = keymap->min_ਕੁੰਜੀ_ਕੋਡ;
    } ਨਹੀਂਤਾਂ ਜੇ (kc > keymap->max_ਕੁੰਜੀ_ਕੋਡ) {
        kc = keymap->max_ਕੁੰਜੀ_ਕੋਡ;
    }

    ਜੇ (kc < keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ) {
        /* Low keycodes */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[kc];
        ਜੇ (ascending) {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਸਿਰਾ = &keymap->ਕੁੰਜੀਆਂ[keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ - 1];
            ਜਦੋਂ (ਕੁੰਜੀ->name == XKB_ATOM_NONE && ਕੁੰਜੀ < ਸਿਰਾ) ਕੁੰਜੀ++;
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਸਿਰਾ = &keymap->ਕੁੰਜੀਆਂ[keymap->min_ਕੁੰਜੀ_ਕੋਡ];
            ਜਦੋਂ (ਕੁੰਜੀ->name == XKB_ATOM_NONE && ਕੁੰਜੀ > ਸਿਰਾ) ਕੁੰਜੀ--;
        }
        ਵਾਪਸ (ਕੁੰਜੀ->name == XKB_ATOM_NONE) ? ਨਿਰਾ : ਕੁੰਜੀ;
    } ਨਹੀਂਤਾਂ {
        /* High keycodes: use binary search */
        xkb_keycode_t ਹੇਠਲਾ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ
            ? keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t ਉੱਤਲਾ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ - 1;
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * candidate = ਨਿਰਾ;
        ਜਦੋਂ (ਹੇਠਲਾ <= ਉੱਤਲਾ) {
            ਅਚਲ xkb_keycode_t mid = ਹੇਠਲਾ + (ਉੱਤਲਾ - ਹੇਠਲਾ) / 2;
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[mid];
            ਜੇ (ਕੁੰਜੀ->keycode == kc) {
                ਵਾਪਸ ਕੁੰਜੀ;
            } ਨਹੀਂਤਾਂ ਜੇ (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                ਜੇ (ਕੁੰਜੀ->keycode >= kc) {
                    candidate = ਕੁੰਜੀ;
                    ਉੱਤਲਾ = mid - 1;
                } ਨਹੀਂਤਾਂ {
                    ਹੇਠਲਾ = mid + 1;
                }
            } ਨਹੀਂਤਾਂ {
                /* Looking for the *smallest* defined keycode <= kc */
                ਜੇ (ਕੁੰਜੀ->keycode <= kc) {
                    candidate = ਕੁੰਜੀ;
                    ਹੇਠਲਾ = mid + 1;
                } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->keycode > kc) {
                    ਉੱਤਲਾ = mid - 1;
                }
            }
        }
        ਵਾਪਸ candidate;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_level_ਸੂਚਕਾਂਕ_t
Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ, xkb_layout_ਸੂਚਕਾਂਕ_t layout)
{
    ਵਾਪਸ ਕੁੰਜੀ->groups[layout].type->ਅੰਕ_ਪੱਧਰ;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਇੰਦਰਾਜ_is_ਸਰਗਰਮ(ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਵਾਪਸ ਇੰਦਰਾਜ->mods.mods == 0 || ਇੰਦਰਾਜ->mods.ਨਕਾਬ != 0;
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* func,
               ਗਿਣਤੀ xkb_keymap_format format,
               ਗਿਣਤੀ xkb_keymap_compile_flags flags);

ਖਾਲੀ
XkbEscapeMapName(ਅੱਖਰ *name);

xkb_mod_ਸੂਚਕਾਂਕ_t
Xkb_Mod_Name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, xkb_atom_t name,
                  ਗਿਣਤੀ mod_type type);

bool
Xkb_ਪੱਧਰ_Same_Syms(ਅਚਲ ਬਣਤਰ xkb_level *a, ਅਚਲ ਬਣਤਰ xkb_level *b);

bool
action_equal(ਅਚਲ ਸੰਘ xkb_action *a, ਅਚਲ ਸੰਘ xkb_action *b);

bool
Xkb_ਪੱਧਰ_Same_Actions(ਅਚਲ ਬਣਤਰ xkb_level *a, ਅਚਲ ਬਣਤਰ xkb_level *b);

xkb_layout_ਸੂਚਕਾਂਕ_t
Xkb_Wrap_ਸਮੂਹ_Into_Range(int32_t ਸਮੂਹ,
                      xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups,
                      ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy,
                      xkb_layout_ਸੂਚਕਾਂਕ_t ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ);

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਕੁੰਜੀ_effective_layout(ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                xkb_layout_ਸੂਚਕਾਂਕ_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    ਵਾਪਸ Xkb_Wrap_ਸਮੂਹ_Into_Range((int32_t) layout,
                                 ਕੁੰਜੀ->ਅੰਕ_groups,
                                 ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy,
                                 ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ);
}

XKB_EXPORT_ਨਿੱਜੀ xkb_mod_ਨਕਾਬ_t
mod_ਨਕਾਬ_get_effective(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਨਕਾਬ_t mods);

ਬਣਤਰ xkb_level *
xkb_keymap_ਕੁੰਜੀ_get_level(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                         xkb_layout_ਸੂਚਕਾਂਕ_t layout, xkb_level_ਸੂਚਕਾਂਕ_t level);

xkb_action_ਗਿਣਤੀ_t
xkb_keymap_ਕੁੰਜੀ_get_actions_by_level(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                    xkb_layout_ਸੂਚਕਾਂਕ_t layout,
                                    xkb_level_ਸੂਚਕਾਂਕ_t level,
                                    ਅਚਲ ਸੰਘ xkb_action **actions);

ਬਣਤਰ xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(ਬਣਤਰ xkb_keymap *keymap,
                                  ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_ਨਾਂ)(ਬਣਤਰ xkb_keymap *keymap,
                                  ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *ਨਾਂ);
    bool (*keymap_new_from_string)(ਬਣਤਰ xkb_keymap *keymap,
                                   ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t ਲੰਮਾਈ);
    bool (*keymap_new_from_file)(ਬਣਤਰ xkb_keymap *keymap, FILE *file);
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ (*keymap_serialize)(
        ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
        ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
        ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *ਨਤੀਜਾ
    );
};

ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops ਪਾਠ_v1_keymap_format_ops;

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_Mods_Un_ਤਾਲਾ_On_Press_Supported(ਗਿਣਤੀ xkb_keymap_format format)
{
    /* Lax bound */
    ਵਾਪਸ format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ਸਮੂਹ_ਤਾਲਾ_On_Release_Supported(ਗਿਣਤੀ xkb_keymap_format format)
{
    /* Lax bound */
    ਵਾਪਸ format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
isModsLatchOnPressSupported(ਗਿਣਤੀ xkb_keymap_format format)
{
    /* Lax bound */
    ਵਾਪਸ format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
areOverlappingOverlaysSupported(ਗਿਣਤੀ xkb_keymap_format format)
{
    /* Lax bound */
    ਵਾਪਸ format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2;
}
