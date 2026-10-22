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
#సూచన once

 /* Don't use compat names in internal code. */
#నిర్వచించు _XKBCOMMON_COMPAT_H

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#అన్తర్భావ "atom.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "rmlvo.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcomp/ast.h"

/* Note: imposed by the size of the xkb_layout_mask_t type (32).
 * This is more than enough though. */
#నిర్వచించు XKB_MAX_GROUPS 32
#నిర్వచించు XKB_ALL_GROUPS ((UINT64_C(1) << XKB_MAX_GROUPS) - UINT64_C(1))
/* Limit imposed by X11 */
#నిర్వచించు XKB_MAX_GROUPS_X11 4

స్థిర అంతఃవరుస xkb_layout_సూచీ_t
format_max_groups(గణన xkb_keymap_format format)
{
    తిరిగి (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
        ? XKB_MAX_GROUPS_X11
        : XKB_MAX_GROUPS;
}

/* Don't allow more leds than we can hold in xkb_led_mask_t. */
#నిర్వచించు XKB_MAX_LEDS ((xkb_led_సూచీ_t) (పరిమాణం(xkb_led_ముసుగు_t) * CHAR_BIT))

/* Don't allow more modifiers than we can hold in xkb_mod_mask_t. */
#నిర్వచించు XKB_MAX_MODS ((xkb_mod_సూచీ_t) (పరిమాణం(xkb_mod_ముసుగు_t) * CHAR_BIT))

గణన {
    /** Mask of all possible modifiers */
    XKB_MOD_ALL = UINT32_MAX,
};

/* Special value to handle modMap None {…} */
#నిర్వచించు XKB_MOD_NONE 0xffffffffU

గణన mod_type {
    /** X11 core modifier */
    MOD_REAL = (1 << 0),
    /** A non-X11 core modifier */
    MOD_VIRT = (1 << 1),
    /** Any modifier */
    MOD_BOTH = (MOD_REAL | MOD_VIRT)
};
#నిర్వచించు MOD_REAL_ముసుగు_ALL ((xkb_mod_ముసుగు_t) 0x000000ff)

/** Predefined (AKA real, core, X11) modifiers. The order is important! */
గణన real_mod_సూచీ {
    XKB_MOD_సూచీ_జరుపు = 0,
    XKB_MOD_సూచీ_CAPS,
    XKB_MOD_సూచీ_CTRL,
    XKB_MOD_సూచీ_MOD1,
    XKB_MOD_సూచీ_MOD2,
    XKB_MOD_సూచీ_MOD3,
    XKB_MOD_సూచీ_MOD4,
    XKB_MOD_సూచీ_MOD5,
    XKB_MOD_సూచీ_అంకె_ENTRIES
};
static_assert(XKB_MOD_సూచీ_అంకె_ENTRIES == 8, "Invalid X11 core modifiers");

గణన xkb_action_type {
    ACTION_TYPE_NONE = 0,
    ACTION_TYPE_VOID, /* xkbcommon extension */
    ACTION_TYPE_MOD_సమితి,
    ACTION_TYPE_MOD_LATCH,
    ACTION_TYPE_MOD_తాళం,
    ACTION_TYPE_సమూహం_సమితి,
    ACTION_TYPE_సమూహం_LATCH,
    ACTION_TYPE_సమూహం_తాళం,
    ACTION_TYPE_PTR_MOVE,
    ACTION_TYPE_PTR_BUTTON,
    ACTION_TYPE_PTR_తాళం,
    ACTION_TYPE_PTR_DEFAULT,
    ACTION_TYPE_TERMINATE,
    ACTION_TYPE_SWITCH_VT,
    ACTION_TYPE_CTRL_సమితి,
    ACTION_TYPE_CTRL_తాళం,
    ACTION_TYPE_REDIRECT_కీలకం,
    ACTION_TYPE_UNSUPPORTED_LEGACY,
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_వ్యక్తిగత,
    ACTION_TYPE_అంతర్గత, /* Action specific and internal to xkbcommon */
    ACTION_TYPE_అంకె_ENTRIES
};

గణన xkb_action_flags {
    ACTION_తాళం_CLEAR = (1 << 0),
    ACTION_LATCH_గమ్యం_తాళం = (1 << 1),
    ACTION_తాళం_NO_తాళం = (1 << 2),
    ACTION_తాళం_NO_UNLOCK = (1 << 3),
    ACTION_MODS_LOOKUP_MODMAP = (1 << 4),
    ACTION_ABSOLUTE_SWITCH = (1 << 5),
    ACTION_ABSOLUTE_X = (1 << 6),
    ACTION_ABSOLUTE_Y = (1 << 7),
    ACTION_REPEAT = (1 << 8),
    ACTION_SAME_SCREEN = (1 << 9),
    ACTION_తాళం_ON_RELEASE = (1 << 10),
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
    ACTION_EXPLICIT_తాళం_ON_RELEASE = (1 << 15),
    /**
     * Indicate that the LockGroup action is elligible to
     * @ref auto-fix-layout-switch-on-modifier-key "automatic behavior fixes".
     *
     * Used *after* keymap compilation.
     */
    ACTION_AUTO_సమూహం_తాళం_ON_MODIFIER_RELEASE = (1 << 16),
};

గణన {
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
గణన xkb_action_controls {
    CONTROL_NONE = 0,
    /* Public API */
    CONTROL_STICKY_కీలకాలు = (1 << 0),
    CONTROL_MOUSE_కీలకాలు = (1 << 1),

    /* Range (1 << 2) .. (1 << 8) for future public controls */

    /* Private API */
    CONTROL_GROUPS_WRAP = (1 << 9),
    CONTROL_REPEAT = (1 << 10),
    CONTROL_SLOW = (1 << 11),
    CONTROL_DEBOUNCE = (1 << 12),
    CONTROL_MOUSE_కీలకాలు_ACCEL = (1 << 15),
    CONTROL_AX = (1 << 16),
    CONTROL_AX_TIMEOUT = (1 << 17),
    CONTROL_AX_FEEDBACK = (1 << 18),
    CONTROL_BELL = (1 << 19),
    CONTROL_IGNORE_సమూహం_తాళం = (1 << 20),
    CONTROL_OVERLAY1 = (1 << CONTROL_OVERLAY1_LOG2),
    CONTROL_OVERLAY2 = (1 << CONTROL_OVERLAY2_LOG2),
    CONTROL_OVERLAY3 = (1 << CONTROL_OVERLAY3_LOG2),
    CONTROL_OVERLAY4 = (1 << CONTROL_OVERLAY4_LOG2),
    CONTROL_OVERLAY5 = (1 << CONTROL_OVERLAY5_LOG2),
    CONTROL_OVERLAY6 = (1 << CONTROL_OVERLAY6_LOG2),
    CONTROL_OVERLAY7 = (1 << CONTROL_OVERLAY7_LOG2),
    CONTROL_OVERLAY8 = (1 << CONTROL_OVERLAY8_LOG2),

    చివరి_CONTROL = CONTROL_OVERLAY8,

    /**
     * All the XKB Controls. If we ever introduce *internal* controls, this mask
     * should not include them.
     */
    CONTROL_ALL_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_కీలకాలు | CONTROL_MOUSE_కీలకాలు | CONTROL_MOUSE_కీలకాలు_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 |
         CONTROL_IGNORE_సమూహం_తాళం | CONTROL_GROUPS_WRAP),
    CONTROL_ALL = \
        (CONTROL_ALL_V1 | CONTROL_OVERLAY3 |
         CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | CONTROL_OVERLAY6 |
         CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
    /* All the boolean controls */
    CONTROL_ALL_BOOLEAN_V1 = \
        (CONTROL_REPEAT | CONTROL_SLOW | CONTROL_DEBOUNCE | \
         CONTROL_STICKY_కీలకాలు | CONTROL_MOUSE_కీలకాలు | CONTROL_MOUSE_కీలకాలు_ACCEL | \
         CONTROL_AX | CONTROL_AX_TIMEOUT | CONTROL_AX_FEEDBACK | \
         CONTROL_BELL | CONTROL_OVERLAY1 | CONTROL_OVERLAY2 | \
         CONTROL_IGNORE_సమూహం_తాళం),
    CONTROL_ALL_BOOLEAN = \
        (CONTROL_ALL_BOOLEAN_V1 | \
         CONTROL_OVERLAY3 | CONTROL_OVERLAY4 | CONTROL_OVERLAY5 | \
         CONTROL_OVERLAY6 | CONTROL_OVERLAY7 | CONTROL_OVERLAY8),
};

static_assert(పరిమాణం(గణన xkb_action_controls) >= 3, "truncated enum");
static_assert((uint32_t)చివరి_CONTROL < (UINT32_C(1) << 31),
              "Cannot ensure enum portability");

స్థిర అంతఃవరుస గణన xkb_action_controls
format_boolean_controls(గణన xkb_keymap_format format)
{
    తిరిగి (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
        ? CONTROL_ALL_BOOLEAN_V1
        : CONTROL_ALL_BOOLEAN;
}

static_assert(
    CONTROL_STICKY_కీలకాలు ==
    (గణన xkb_action_controls) XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు,
    "Private value should match public API"
);

static_assert(
    CONTROL_MOUSE_కీలకాలు ==
    (గణన xkb_action_controls) XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు,
    "Private value should match public API"
);


గణన xkb_సరిపోలిక_operation {
    సరిపోలిక_NONE,
    సరిపోలిక_ANY_OR_NONE,
    సరిపోలిక_ANY,
    సరిపోలిక_ALL,
    సరిపోలిక_EXACTLY,
};

నిర్మాణం xkb_mods {
    xkb_mod_ముసుగు_t mods;       /* original real+virtual mods in definition */
    xkb_mod_ముసుగు_t ముసుగు;       /* computed effective mask */
};

నిర్మాణం xkb_mod_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    నిర్మాణం xkb_mods mods;
};

నిర్మాణం xkb_సమూహం_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    int32_t సమూహం;
};

/** Keyboard overlay index or count */
రకనిర్వచనం uint8_t xkb_overlay_సూచీ_t;
/** Keyboard overlay mask */
రకనిర్వచనం uint8_t xkb_overlay_ముసుగు_t;
గణన {
    /* Indices */

    /** Maximum number of keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_లెక్క_X11 = 2,
    /** Maximum number of keymap v2+ overlays */
    XKB_OVERLAY_లెక్క = (పరిమాణం(xkb_overlay_ముసుగు_t) * CHAR_BIT),
    /** Minimum overlay index */
    XKB_OVERLAY_MIN = 0,
    /** Maximum overlay index */
    XKB_OVERLAY_MAX = XKB_OVERLAY_లెక్క - 1,
    /** Invalid overlay index */
    XKB_OVERLAY_INVALID = UINT8_MAX,
    /** Min bits required to store overlays indices */
    XKB_OVERLAY_సూచీ_MIN_వెడల్పు = 4, /* nibble */

    /* Masks */

    /** Mask of all valid keymap v1 overlays (X11 limit) */
    XKB_OVERLAY_ALL_X11 = ((UINT8_C(1) << XKB_OVERLAY_లెక్క_X11) - 1),
    /** Mask of all valid keymap v2+ overlays */
    XKB_OVERLAY_ALL = ((UINT64_C(1) << XKB_OVERLAY_లెక్క) - 1),
};

static_assert(CONTROL_OVERLAY1 == (1u << CONTROL_OVERLAY1_LOG2) &&
              CONTROL_OVERLAY8 == (1u << CONTROL_OVERLAY8_LOG2),
              "Mismatch Overlays controls encoding");
static_assert(CONTROL_OVERLAY8_LOG2 == _MAX_CONTROL_OVERLAY_LOG2 - 1,
              "Maximum of overlays changed");
static_assert(XKB_OVERLAY_MIN ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY8_LOG2 - 1) &&
              XKB_OVERLAY_లెక్క ==
              (_MAX_CONTROL_OVERLAY_LOG2 - CONTROL_OVERLAY1_LOG2),
              "Overlays types do not match with overlays enum constants in "
              "xkb_action_controls");
static_assert(XKB_OVERLAY_లెక్క < XKB_OVERLAY_INVALID,
              "Cannot encode XKB_OVERLAY_INVALID");
static_assert(XKB_OVERLAY_లెక్క <= (1u << XKB_OVERLAY_సూచీ_MIN_వెడల్పు) - 1,
              "Cannot encode overlay index or count in a nibble");

స్థిర అంతఃవరుస xkb_overlay_ముసుగు_t
overlays_from_controls(గణన xkb_action_controls controls)
{
    తిరిగి (xkb_overlay_ముసుగు_t)(
        (controls >> CONTROL_OVERLAY1_LOG2) &
        XKB_OVERLAY_ALL
    );
}

స్థిర అంతఃవరుస గణన xkb_action_controls
overlays_గమ్యం_controls(xkb_overlay_ముసుగు_t overlays)
{
    తిరిగి (గణన xkb_action_controls)(
        (overlays & XKB_OVERLAY_ALL) << CONTROL_OVERLAY1_LOG2
    );
}

స్థిర అంతఃవరుస xkb_overlay_సూచీ_t
format_max_overlays(గణన xkb_keymap_format format)
{
    తిరిగి (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
        ? XKB_OVERLAY_లెక్క_X11
        : XKB_OVERLAY_లెక్క;
}

నిర్మాణం xkb_controls_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    గణన xkb_action_controls ctrls;
};

రకనిర్వచనం uint8_t xkb_సూచిక_button_సూచీ_t;
రకనిర్వచనం uint8_t xkb_సూచిక_button_ముసుగు_t;

గణన {
    XKB_సూచిక_BUTTON_సూచీ_వెడల్పు =
        పరిమాణం(xkb_సూచిక_button_సూచీ_t) * CHAR_BIT,
    XKB_సూచిక_BUTTON_సూచీ_MAX =
        (1 << XKB_సూచిక_BUTTON_సూచీ_వెడల్పు) - 1,
    XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు =
        పరిమాణం(xkb_సూచిక_button_ముసుగు_t) * CHAR_BIT,
    XKB_సూచిక_BUTTON_DEFAULT = 0,
    XKB_సూచిక_BUTTON_MIN = 1,
    XKB_సూచిక_BUTTON_MAX = 5,
};

static_assert(XKB_సూచిక_BUTTON_MAX <= XKB_సూచిక_BUTTON_సూచీ_MAX,
              "xkb_pointer_button_index_t cannot store button indices");

static_assert(XKB_సూచిక_BUTTON_MAX <= XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు,
              "xkb_pointer_button_mask_t cannot store button mask");

నిర్మాణం xkb_సూచిక_default_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    int8_t విలువ;
};

గణన {
    XKB_సూచిక_DEFAULT_ACTION_విలువ_వెడల్పు =
        పరిమాణం(((నిర్మాణం xkb_సూచిక_default_action*)0)->విలువ) * CHAR_BIT,
    XKB_సూచిక_DEFAULT_ACTION_విలువ_MAX =
        /* NOTE: signed integer */
        (UINT32_C(1) << (XKB_సూచిక_DEFAULT_ACTION_విలువ_వెడల్పు - 1)) - 1,
};

static_assert((uint32_t)XKB_సూచిక_BUTTON_MAX <=
              (uint32_t)XKB_సూచిక_DEFAULT_ACTION_విలువ_MAX,
              "Cannot store button in xkb_pointer_default_action::value");

నిర్మాణం xkb_సూచిక_motion_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    int16_t x;
    int16_t y;
};

నిర్మాణం xkb_సూచిక_button_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    uint8_t లెక్క;
    xkb_సూచిక_button_సూచీ_t button;
};

నిర్మాణం xkb_switch_screen_action {
    గణన xkb_action_type type;
    గణన xkb_action_flags flags;
    int8_t screen;
};

నిర్మాణం xkb_redirect_కీలకం_action {
    గణన xkb_action_type type;
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
    xkb_mod_ముసుగు_t affect_mods;
    /** State of the affected virtual modifiers */
    xkb_mod_ముసుగు_t mods;
};

నిర్మాణం xkb_వ్యక్తిగత_action {
    గణన xkb_action_type type;
    uint8_t దత్తాంశం[7];
};

గణన xkb_అంతర్గత_action_flags {
    అంతర్గత_BREAKS_సమూహం_LATCH = (1 << 0),
    అంతర్గత_BREAKS_MOD_LATCH = (1 << 1),
};

/* Action specific and internal to xkbcommon */
నిర్మాణం xkb_అంతర్గత_action {
    గణన xkb_action_type type;
    గణన xkb_అంతర్గత_action_flags flags;
    సంఘం {
        /* flag == INTERNAL_BREAKS_MOD_LATCH */
        xkb_mod_ముసుగు_t clear_latched_mods;
    };
};

సంఘం xkb_action {
    గణన xkb_action_type type;
    నిర్మాణం xkb_mod_action mods;
    నిర్మాణం xkb_సమూహం_action సమూహం;
    నిర్మాణం xkb_controls_action ctrls;
    నిర్మాణం xkb_redirect_కీలకం_action redirect;
    నిర్మాణం xkb_సూచిక_default_action dflt;
    నిర్మాణం xkb_సూచిక_motion_action ptr;
    నిర్మాణం xkb_సూచిక_button_action btn;
    నిర్మాణం xkb_switch_screen_action screen;
    నిర్మాణం xkb_వ్యక్తిగత_action priv;
    నిర్మాణం xkb_అంతర్గత_action అంతర్గత;
};

నిర్మాణం xkb_కీలకం_type_నమోదు {
    xkb_level_సూచీ_t level;
    నిర్మాణం xkb_mods mods;
    నిర్మాణం xkb_mods preserve;
};

నిర్మాణం xkb_కీలకం_type {
    xkb_atom_t name;
    నిర్మాణం xkb_mods mods;
    bool required;
    xkb_level_సూచీ_t అంకె_స్థాయిలు;
    xkb_level_సూచీ_t అంకె_level_పేర్లు;
    xkb_atom_t *level_పేర్లు ATTR_COUNTED_BY(అంకె_level_పేర్లు);
    darray_కొలత_t అంకె_entries;
    నిర్మాణం xkb_కీలకం_type_నమోదు *entries ATTR_COUNTED_BY(అంకె_entries);
};

రకనిర్వచనం uint16_t xkb_action_లెక్క_t;
#నిర్వచించు MAX_ACTIONS_చొప్పున_LEVEL UINT16_MAX

నిర్మాణం xkb_sym_interpret {
    xkb_keysym_t sym;
    గణన xkb_సరిపోలిక_operation సరిపోలిక;
    xkb_mod_ముసుగు_t mods;
    xkb_mod_సూచీ_t virtual_mod;
    bool level_one_only;
    bool repeat:1;
    bool required:1;
    xkb_action_లెక్క_t అంకె_actions;
    సంఘం {
        /* num_actions <= 1 */
        సంఘం xkb_action action;
        /* num_actions >  1 */
        సంఘం xkb_action *actions;
    } a;
};

గణన {XKB_STATE_COMPONENT_వెడల్పు = (పరిమాణం(గణన xkb_state_component) * CHAR_BIT)};
static_assert(
    (UINT64_C(1) << XKB_STATE_COMPONENT_వెడల్పు) - 1 > XKB_STATE_CONTROLS_EFFECTIVE,
    "Cannot encode xkb_led::pending_groups"
);

నిర్మాణం xkb_led {
    xkb_atom_t name;
    గణన xkb_state_component which_groups:(XKB_STATE_COMPONENT_వెడల్పు - 1);
    bool pending_groups:1;
    xkb_layout_ముసుగు_t groups;
    గణన xkb_state_component which_mods;
    నిర్మాణం xkb_mods mods;
    గణన xkb_action_controls ctrls;
};

నిర్మాణం xkb_కీలకం_alias {
    xkb_atom_t real;
    xkb_atom_t alias;
};

నిర్మాణం xkb_controls {
    గుర్తులేని అక్షరం groups_wrap;
    నిర్మాణం xkb_mods అంతర్గత;
    నిర్మాణం xkb_mods ignore_తాళం;
    గుర్తులేని పొట్టి repeat_delay;
    గుర్తులేని పొట్టి repeat_interval;
    గుర్తులేని పొట్టి slow_కీలకాలు_delay;
    గుర్తులేని పొట్టి debounce_delay;
    గుర్తులేని పొట్టి ax_options;
    గుర్తులేని పొట్టి ax_timeout;
    గుర్తులేని పొట్టి axt_opts_ముసుగు;
    గుర్తులేని పొట్టి axt_opts_విలువలు;
    గుర్తులేని పూర్ణాంకం axt_ctrls_ముసుగు;
    గుర్తులేని పూర్ణాంకం axt_ctrls_విలువలు;
};

గణన xkb_explicit_components {
    EXPLICIT_NONE = 0,
    EXPLICIT_SYMBOLS = (1 << 0),
    EXPLICIT_INTERP = (1 << 1),
    EXPLICIT_TYPES = (1 << 2),
    EXPLICIT_VMODMAP = (1 << 3),
    EXPLICIT_REPEAT = (1 << 4),
    EXPLICIT_OVERLAY = (1 << 5),
};

రకనిర్వచనం uint16_t xkb_keysym_లెక్క_t;
#నిర్వచించు MAX_KEYSYMS_చొప్పున_LEVEL UINT16_MAX

/** A key level */
నిర్మాణం xkb_level {
    /** Count of keysyms */
    xkb_keysym_లెక్క_t అంకె_syms;
    /** Count of actions */
    xkb_action_లెక్క_t అంకె_actions;
    /** Upper keysym */
    సంఘం {
        /** num_syms == 1: Upper keysym */
        xkb_keysym_t పై;
        /** num_syms >  1: Indicate if `syms` contains the upper case keysyms
         *                 after the lower ones. */
        bool has_పై;
    };
    /** Keysyms */
    సంఘం {
        xkb_keysym_t sym;          /* num_syms == 1 */
        xkb_keysym_t *syms;        /* num_syms > 1  */
    } s;
    /** Actions */
    సంఘం {
        సంఘం xkb_action action;   /* num_actions == 1 */
        సంఘం xkb_action *actions; /* num_actions >  1 */
    } a;
};

/**
 * Group in a key
 */
నిర్మాణం xkb_సమూహం {
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
    స్థిరరాశి నిర్మాణం xkb_కీలకం_type *type;
    /**
     * Array of group levels. Use `XkbKeyNumLevels` for the number of levels.
     */
    నిర్మాణం xkb_level *స్థాయిలు;
};

గణన {
    DEFAULT_కీలకం_REPEAT = false,
    /**
     * Any key with no explicit key repeat value that trigger the default
     * interpret, e.g. some explicit symbols in a group with no action that do
     * not trigger any explicit interpret.
     */
    DEFAULT_INTERPRET_కీలకం_REPEAT = true,
    FALLBACK_INTERPRET_కీలకం_REPEAT = DEFAULT_కీలకం_REPEAT
};

గణన {
    DEFAULT_కీలకం_VMODMAP = 0,
    DEFAULT_INTERPRET_VMOD = XKB_MOD_INVALID,
    DEFAULT_INTERPRET_VMODMAP = DEFAULT_కీలకం_VMODMAP,
    FALLBACK_INTERPRET_VMODMAP = DEFAULT_కీలకం_VMODMAP
};

నిర్మాణం xkb_కీలకం {
    xkb_keycode_t keycode;
    xkb_atom_t name;

    గణన xkb_explicit_components explicit;

    xkb_mod_ముసుగు_t modmap;
    xkb_mod_ముసుగు_t vmodmap;

    xkb_overlay_ముసుగు_t overlays;
    bool overlays_inline:1;

    bool repeats:1;
    /** Flag that indicates whether some group has implicit actions */
    bool implicit_actions:1;

    bool బహిర్గతం_యొక్క_range_pending_సమూహం:1;
    గణన xkb_layout_బహిర్గతం_యొక్క_range_policy బహిర్గతం_యొక్క_range_సమూహం_policy:4;
    xkb_layout_సూచీ_t బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య:8;

    xkb_layout_సూచీ_t అంకె_groups:8;
    నిర్మాణం xkb_సమూహం *groups ATTR_COUNTED_BY(అంకె_groups);
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    సంఘం {
        /** Inlined if at most one overlay is set (overlays_inline = true) */
        స్థిరరాశి నిర్మాణం xkb_కీలకం *overlay_కీలకం;
        /**
         * Allocated if 2 or more overlays are defined (overlays_inline = false).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        స్థిరరాశి నిర్మాణం xkb_కీలకం **overlays_కీలకాలు;
    };
};

నిర్మాణం xkb_mod {
    xkb_atom_t name;
    గణన mod_type type;
    xkb_mod_ముసుగు_t mapping; /* vmod -> real mod mapping */
};

నిర్మాణం xkb_mod_సమితి {
    నిర్మాణం xkb_mod mods[XKB_MAX_MODS];
    xkb_mod_సూచీ_t అంకె_mods;
    xkb_mod_ముసుగు_t explicit_vmods;
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
#నిర్వచించు XKB_KEYCODE_MAX_CONTIGUOUS 0xfff
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < XKB_KEYCODE_MAX,
              "Valid keycodes");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(పరిమాణం(xkb_atom_t)),
              "Max keycodes names");
static_assert(XKB_KEYCODE_MAX_CONTIGUOUS < darray_max_alloc(పరిమాణం(నిర్మాణం xkb_కీలకం)),
              "Max keymap keys");

/*
 * Our current implementation with continuous arrays does not allow efficient
 * mapping of levels. Allowing the max valid level UINT32_MAX could result in
 * memory exhaustion or memory waste (sparse arrays) with huge enough valid
 * values. Let’s be more conservative for now. This value should be big enough
 * to satisfy automatically generated keymaps.
 */
#నిర్వచించు XKB_LEVEL_MAX_IMPL 2048
static_assert(XKB_LEVEL_MAX_IMPL < XKB_LEVEL_INVALID,
              "Valid levels");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(పరిమాణం(xkb_atom_t)),
              "Max key types level names");
static_assert(XKB_LEVEL_MAX_IMPL < darray_max_alloc(పరిమాణం(నిర్మాణం xkb_level)),
              "Max keys levels");

static_assert((1LLU << (DARRAY_కొలత_T_వెడల్పు - 3)) - 1 >=
              XKB_KEYCODE_MAX_CONTIGUOUS,
              "Cannot store low keycodes");

/** Keycode store lookup result */
రకనిర్వచనం సంఘం {
    నిర్మాణం {
        /** If true, the index is valid */
        bool దొరికింది:1;
        bool:1;
        /** Whether the corresponding entry is an alias */
        bool is_alias:1;
        darray_కొలత_t:(DARRAY_కొలత_T_వెడల్పు - 3);
    };
    /* Only if is_alias = false, for better semantics */
    నిర్మాణం {
        bool దొరికింది:1;
        /**
         * Whether the corresponding entry is stored in the low or high table
         * of the keycode store
         */
        bool దిగువ:1;
        bool is_alias:1;
        /**
         * - xkb_keycodes: index of the entry in the keycode store
         * - xkb_symbols: index of the entry in xkb_keymap::keys array
         */
        darray_కొలత_t సూచీ:(DARRAY_కొలత_T_వెడల్పు - 3);
    } కీలకం;
    /* Only if is_alias = true, for better semantics */
    నిర్మాణం {
        bool దొరికింది:1;
        bool:1;
        bool is_alias:1;
        /** Real name of the target key */
        xkb_atom_t real:(DARRAY_కొలత_T_వెడల్పు - 3);
    } alias;
} Keycode_సరిపోలిక;

రకనిర్వచనం నిర్మాణం {
    సంఘం {
        /** Used for parsing */
        గణన విలీనం_బహుళకం విలీనం;
        /** Used for serializing */
        xkb_keycode_t కీలకం_సంకేతం;
    };
    bool have_చిహ్నం;
    /** NOTE: 0 means: “remove the key or keysym from the modmap” */
    xkb_mod_ముసుగు_t mods;
    సంఘం {
        xkb_keysym_t కీలకం_Sym;
        xkb_atom_t కీలకం_Name;
    } u;
} Mod_Map_నమోదు;

/** Common keyboard description structure */
నిర్మాణం xkb_keymap {
    నిర్మాణం xkb_context *ctx;

    పూర్ణాంకం refcnt;
    గణన xkb_keymap_compile_flags flags;
    గణన xkb_keymap_format format;

    xkb_led_సూచీ_t అంకె_leds;
    నిర్మాణం xkb_led leds[XKB_MAX_LEDS];

    xkb_keycode_t min_కీలకం_సంకేతం;
    xkb_keycode_t max_కీలకం_సంకేతం;
    xkb_keycode_t అంకె_కీలకాలు;
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
    xkb_keycode_t అంకె_కీలకాలు_దిగువ;
    నిర్మాణం xkb_కీలకం *కీలకాలు ATTR_COUNTED_BY(అంకె_కీలకాలు);

    సంఘం {
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
        నిర్మాణం {
            darray_కొలత_t అంకె_కీలకం_పేర్లు;
            Keycode_సరిపోలిక *కీలకం_పేర్లు;
        };
        /**
         * Aliases in no particular order
         *
         * ⚠️ Used only *after* compilation
         */
        నిర్మాణం {
            darray_కొలత_t అంకె_కీలకం_aliases;
            నిర్మాణం xkb_కీలకం_alias *కీలకం_aliases;
        };
    };

    నిర్మాణం xkb_కీలకం_type *types;
    darray_కొలత_t అంకె_types;

    darray_కొలత_t అంకె_sym_interprets;
    నిర్మాణం xkb_sym_interpret *sym_interprets;

    darray_కొలత_t _padding;
    darray_కొలత_t అంకె_modmaps;
    Mod_Map_నమోదు *modmaps;

    /**
     * Modifiers configuration.
     * This is *internal* to the keymap; other implementations may use different
     * virtual modifiers indices. Ours depends on:
     *   1. the order of the parsing of the keymap components;
     *   2. the order of the virtual modifiers declarations;
     */
    నిర్మాణం xkb_mod_సమితి mods;
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
    xkb_mod_ముసుగు_t canonical_state_ముసుగు;

    /** Encode the “auto” special value of RedirectKey() */
    xkb_keycode_t redirect_కీలకం_auto;

    /* This field has 2 uses:
     * • During parsing: Expected layouts count after RMLVO resolution, if any;
     * • After parsing: Number of groups in the key with the most groups. */
    xkb_layout_సూచీ_t అంకె_groups;
    /* Not all groups must have names. */
    xkb_layout_సూచీ_t అంకె_సమూహం_పేర్లు;
    xkb_atom_t *సమూహం_పేర్లు ATTR_COUNTED_BY(అంకె_సమూహం_పేర్లు);

    అక్షరం *keycodes_section_name;
    అక్షరం *symbols_section_name;
    అక్షరం *types_section_name;
    అక్షరం *compat_section_name;
};

#నిర్వచించు xkb_కీలకాలు_foreach(iter, keymap) \
    /*
     * Start at the first defined low or high keycode:
     * - if there are some low keycodes, the index is min_key_code, because we
     *   skip the previous undefined keycodes;
     * - else the first item is the first high keycode.
     */ \
    కొరకు ((iter) = (keymap)->కీలకాలు + \
                  ((keymap)->అంకె_కీలకాలు_దిగువ == 0 ? 0 : (keymap)->min_కీలకం_సంకేతం); \
         (iter) < (keymap)->కీలకాలు + (keymap)->అంకె_కీలకాలు; \
         (iter)++)

#నిర్వచించు xkb_level_foreach_action(iter, level)                 \
    కొరకు (సంఘం xkb_action                                     \
         *(iter) = (likely((level)->అంకె_actions <= 1)         \
                 ? &(level)->a.action                         \
                 : (level)->a.actions),                       \
         /* NOTE: loop is skipped if num_actions == 0 */      \
         * స్థిరరాశి iter##_last_ = (iter) + (level)->అంకె_actions;\
        (iter) < iter##_last_;                                \
        (iter)++)

#నిర్వచించు xkb_mods_foreach(iter, mods_) \
    కొరకు ((iter) = (mods_)->mods; \
         (iter) < (mods_)->mods + (mods_)->అంకె_mods; \
         (iter)++)

#నిర్వచించు xkb_mods_ముసుగు_foreach(ముసుగు, iter, mods_) \
    కొరకు ((iter) = (mods_)->mods; \
         (ముసుగు) && (iter) < (mods_)->mods + (mods_)->అంకె_mods; \
         (iter)++, (ముసుగు) >>= 1) \
        ఒకవేళ ((ముసుగు) & 0x1)

/** Enumerate all modifiers */
#నిర్వచించు xkb_mods_enumerate(idx, iter, mods_) \
    కొరకు ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < (mods_)->అంకె_mods; \
         (idx)++, (iter)++)

/** Enumerate only real modifiers */
#నిర్వచించు xkb_rmods_enumerate(idx, iter, mods_) \
    కొరకు ((idx) = 0, (iter) = (mods_)->mods; \
         (idx) < XKB_MOD_సూచీ_అంకె_ENTRIES; \
         (idx)++, (iter)++)

/** Enumerate only virtual modifiers */
#నిర్వచించు xkb_vmods_enumerate(idx, iter, mods_) \
    కొరకు ((idx) = XKB_MOD_సూచీ_అంకె_ENTRIES, \
         (iter) = &(mods_)->mods[XKB_MOD_సూచీ_అంకె_ENTRIES]; \
         (idx) < (mods_)->అంకె_mods; \
         (idx)++, (iter)++)

#నిర్వచించు xkb_leds_foreach(iter, keymap) \
    కొరకు ((iter) = (keymap)->leds; \
         (iter) < (keymap)->leds + (keymap)->అంకె_leds; \
         (iter)++)

#నిర్వచించు xkb_leds_enumerate(idx, iter, keymap) \
    కొరకు ((idx) = 0, (iter) = (keymap)->leds; \
         (idx) < (keymap)->అంకె_leds; \
         (idx)++, (iter)++)

శూన్య
clear_level(నిర్మాణం xkb_level *leveli);

/* ⚠️ Only valid before copying symbols to keymap */
స్థిర అంతఃవరుస నిర్మాణం xkb_కీలకం *
Xkb_కీలకం_By_Name(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, xkb_atom_t name, bool use_aliases)
{
    ఒకవేళ (name < keymap->అంకె_కీలకం_పేర్లు) {
        స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక = keymap->కీలకం_పేర్లు[name];
        ఒకవేళ (సరిపోలిక.దొరికింది) {
            ఒకవేళ (!సరిపోలిక.is_alias) {
                ప్రతిజ్ఞా(name == keymap->కీలకాలు[సరిపోలిక.కీలకం.సూచీ].name);
                తిరిగి &keymap->కీలకాలు[సరిపోలిక.కీలకం.సూచీ];
            } లేకపోతే ఒకవేళ (use_aliases) {
                ప్రతిజ్ఞా(సరిపోలిక.alias.real ==
                       keymap->కీలకాలు[keymap->కీలకం_పేర్లు[సరిపోలిక.alias.real].కీలకం.సూచీ].name);
                తిరిగి &keymap->కీలకాలు[keymap->కీలకం_పేర్లు[సరిపోలిక.alias.real].కీలకం.సూచీ];
            }
        }
    }
    తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం xkb_కీలకం *
Xkb_కీలకం(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, xkb_keycode_t kc)
{
    ఒకవేళ (kc < keymap->min_కీలకం_సంకేతం || kc > keymap->max_కీలకం_సంకేతం) {
        /* Unsupported keycode */
        తిరిగి ఖాళీ;
    } లేకపోతే ఒకవేళ (kc < keymap->అంకె_కీలకాలు_దిగువ) {
        /* Low keycodes */
        తిరిగి &keymap->కీలకాలు[kc];
    } లేకపోతే {
        /* High keycodes: use binary search */
        xkb_keycode_t క్రింది = keymap->అంకె_కీలకాలు_దిగువ;
        xkb_keycode_t పై = keymap->అంకె_కీలకాలు;
        అయితే (క్రింది < పై) {
            స్థిరరాశి xkb_keycode_t mid = క్రింది + (పై - 1 - క్రింది) / 2;
            స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి కీలకం = &keymap->కీలకాలు[mid];
            ఒకవేళ (కీలకం->keycode < kc) {
                క్రింది = mid + 1;
            } లేకపోతే ఒకవేళ (కీలకం->keycode > kc) {
                పై = mid;
            } లేకపోతే {
                తిరిగి కీలకం;
            }
        }
        తిరిగి ఖాళీ;
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
స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం xkb_కీలకం *
xkb_keymap_get_తదుపరి_defined_కీలకం(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                                bool ascending, xkb_keycode_t kc)
{
    /* Unsupported keycodes */
    ఒకవేళ (!keymap->అంకె_కీలకాలు || kc > XKB_KEYCODE_MAX) {
        తిరిగి ఖాళీ;
    } లేకపోతే ఒకవేళ (kc < keymap->min_కీలకం_సంకేతం) {
        kc = keymap->min_కీలకం_సంకేతం;
    } లేకపోతే ఒకవేళ (kc > keymap->max_కీలకం_సంకేతం) {
        kc = keymap->max_కీలకం_సంకేతం;
    }

    ఒకవేళ (kc < keymap->అంకె_కీలకాలు_దిగువ) {
        /* Low keycodes */
        స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = &keymap->కీలకాలు[kc];
        ఒకవేళ (ascending) {
            స్థిరరాశి నిర్మాణం xkb_కీలకం *చివర = &keymap->కీలకాలు[keymap->అంకె_కీలకాలు_దిగువ - 1];
            అయితే (కీలకం->name == XKB_ATOM_NONE && కీలకం < చివర) కీలకం++;
        } లేకపోతే {
            స్థిరరాశి నిర్మాణం xkb_కీలకం *చివర = &keymap->కీలకాలు[keymap->min_కీలకం_సంకేతం];
            అయితే (కీలకం->name == XKB_ATOM_NONE && కీలకం > చివర) కీలకం--;
        }
        తిరిగి (కీలకం->name == XKB_ATOM_NONE) ? ఖాళీ : కీలకం;
    } లేకపోతే {
        /* High keycodes: use binary search */
        xkb_keycode_t క్రింది = keymap->అంకె_కీలకాలు_దిగువ
            ? keymap->అంకె_కీలకాలు_దిగువ - 1 /* Last low keycode */
            : 0;                       /* First high keycode */
        xkb_keycode_t పై = keymap->అంకె_కీలకాలు - 1;
        స్థిరరాశి నిర్మాణం xkb_కీలకం * candidate = ఖాళీ;
        అయితే (క్రింది <= పై) {
            స్థిరరాశి xkb_keycode_t mid = క్రింది + (పై - క్రింది) / 2;
            స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి కీలకం = &keymap->కీలకాలు[mid];
            ఒకవేళ (కీలకం->keycode == kc) {
                తిరిగి కీలకం;
            } లేకపోతే ఒకవేళ (ascending) {
                /* Looking for the *smallest* defined keycode >= kc */
                ఒకవేళ (కీలకం->keycode >= kc) {
                    candidate = కీలకం;
                    పై = mid - 1;
                } లేకపోతే {
                    క్రింది = mid + 1;
                }
            } లేకపోతే {
                /* Looking for the *smallest* defined keycode <= kc */
                ఒకవేళ (కీలకం->keycode <= kc) {
                    candidate = కీలకం;
                    క్రింది = mid + 1;
                } లేకపోతే ఒకవేళ (కీలకం->keycode > kc) {
                    పై = mid - 1;
                }
            }
        }
        తిరిగి candidate;
    }
}

స్థిర అంతఃవరుస xkb_level_సూచీ_t
Xkb_కీలకం_అంకె_స్థాయిలు(స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం, xkb_layout_సూచీ_t layout)
{
    తిరిగి కీలకం->groups[layout].type->అంకె_స్థాయిలు;
}

/*
 * Map entries which specify unbound virtual modifiers are not considered.
 * See: the XKB protocol, section “Determining the KeySym Associated with a Key
 * Event”
 *
 * xserver does this with cached entry->active field.
 */
స్థిర అంతఃవరుస bool
నమోదు_is_చురుకైన(స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2)
{
    తిరిగి నమోదు_2->mods.mods == 0 || నమోదు_2->mods.ముసుగు != 0;
}

నిర్మాణం xkb_keymap *
xkb_keymap_new(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* func,
               గణన xkb_keymap_format format,
               గణన xkb_keymap_compile_flags flags);

శూన్య
XkbEscapeMapName(అక్షరం *name);

xkb_mod_సూచీ_t
Xkb_Mod_Name_గమ్యం_సూచీ(స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, xkb_atom_t name,
                  గణన mod_type type);

bool
Xkb_స్థాయిలు_Same_Syms(స్థిరరాశి నిర్మాణం xkb_level *a, స్థిరరాశి నిర్మాణం xkb_level *b);

bool
action_equal(స్థిరరాశి సంఘం xkb_action *a, స్థిరరాశి సంఘం xkb_action *b);

bool
Xkb_స్థాయిలు_Same_Actions(స్థిరరాశి నిర్మాణం xkb_level *a, స్థిరరాశి నిర్మాణం xkb_level *b);

xkb_layout_సూచీ_t
Xkb_Wrap_సమూహం_Into_Range(int32_t సమూహం,
                      xkb_layout_సూచీ_t అంకె_groups,
                      గణన xkb_layout_బహిర్గతం_యొక్క_range_policy బహిర్గతం_యొక్క_range_సమూహం_policy,
                      xkb_layout_సూచీ_t బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య);

స్థిర అంతఃవరుస xkb_layout_సూచీ_t
xkb_keymap_కీలకం_effective_layout(స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                xkb_layout_సూచీ_t layout)
{
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    తిరిగి Xkb_Wrap_సమూహం_Into_Range((int32_t) layout,
                                 కీలకం->అంకె_groups,
                                 కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy,
                                 కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య);
}

XKB_EXPORT_వ్యక్తిగత xkb_mod_ముసుగు_t
mod_ముసుగు_get_effective(నిర్మాణం xkb_keymap *keymap, xkb_mod_ముసుగు_t mods);

నిర్మాణం xkb_level *
xkb_keymap_కీలకం_get_level(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                         xkb_layout_సూచీ_t layout, xkb_level_సూచీ_t level);

xkb_action_లెక్క_t
xkb_keymap_కీలకం_get_actions_by_level(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                                    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                    xkb_layout_సూచీ_t layout,
                                    xkb_level_సూచీ_t level,
                                    స్థిరరాశి సంఘం xkb_action **actions);

నిర్మాణం xkb_keymap_format_ops {
    bool (*keymap_new_from_rmlvo)(నిర్మాణం xkb_keymap *keymap,
                                  స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo);
    bool (*keymap_new_from_పేర్లు)(నిర్మాణం xkb_keymap *keymap,
                                  స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *పేర్లు);
    bool (*keymap_new_from_string)(నిర్మాణం xkb_keymap *keymap,
                                   స్థిరరాశి అక్షరం *string, కొలత_t నిడివి);
    bool (*keymap_new_from_file)(నిర్మాణం xkb_keymap *keymap, FILE *file);
    గణన xkb_స్థితి (*keymap_serialize)(
        స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
        స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
        నిర్మాణం xkb_keymap_serialize_ఫలితం *ఫలితం
    );
};

బాహ్య స్థిరరాశి నిర్మాణం xkb_keymap_format_ops పాఠ్యం_v1_keymap_format_ops;

స్థిర అంతఃవరుస bool
is_Mods_Un_తాళం_On_Press_Supported(గణన xkb_keymap_format format)
{
    /* Lax bound */
    తిరిగి format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2;
}

స్థిర అంతఃవరుస bool
is_సమూహం_తాళం_On_Release_Supported(గణన xkb_keymap_format format)
{
    /* Lax bound */
    తిరిగి format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2;
}

స్థిర అంతఃవరుస bool
isModsLatchOnPressSupported(గణన xkb_keymap_format format)
{
    /* Lax bound */
    తిరిగి format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2;
}

స్థిర అంతఃవరుస bool
areOverlappingOverlaysSupported(గణన xkb_keymap_format format)
{
    /* Lax bound */
    తిరిగి format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2;
}
