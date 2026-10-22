/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-compose.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-features.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UINT32_ಅಗಲ
#ವ್ಯಾಖ್ಯಾನಿಸು UINT32_ಅಗಲ (ಗಾತ್ರ(uint32_t) * CHAR_BIT)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 < UINT32_ಅಗಲ, "");
static_assert(XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2 < UINT32_ಅಗಲ, "");
static_assert(XKB_ಕೀಲಿ_ಮೇಲೆ >= 0 &&
              XKB_ಕೀಲಿ_ಮೇಲೆ < UINT32_ಅಗಲ, "");
static_assert(XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ >= 0 &&
              XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ < UINT32_ಅಗಲ, "");
static_assert(XKB_ಕೀಲಿ_REPEATED >= 0 &&
              XKB_ಕೀಲಿ_REPEATED < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_ಚೌಕಟ್ಟು >= 0 &&
              XKB_EVENT_TYPE_ಚೌಕಟ್ಟು < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_ಕೀಲಿ >= 0 &&
              XKB_EVENT_TYPE_ಕೀಲಿ < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_ಸೂಚಕ_MOTION >= 0 &&
              XKB_EVENT_TYPE_ಸೂಚಕ_MOTION < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON >= 0 &&
              XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_ಅಗಲ, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_ಅಗಲ, "");
static_assert(XKB_ಸೂಚಕ_BUTTON_RELEASED >= 0 &&
              XKB_ಸೂಚಕ_BUTTON_RELEASED < UINT32_ಅಗಲ, "");
static_assert(XKB_ಸೂಚಕ_BUTTON_PRESSED >= 0 &&
              XKB_ಸೂಚಕ_BUTTON_PRESSED < UINT32_ಅಗಲ, "");
static_assert(XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP < UINT32_ಅಗಲ, "");
static_assert(XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP < UINT32_ಅಗಲ, "");
static_assert(XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT < UINT32_ಅಗಲ, "");
static_assert(XKB_STATE_ಬಹುಲಕ_CLIENT >= 0 &&
              XKB_STATE_ಬಹುಲಕ_CLIENT < UINT32_ಅಗಲ, "");
static_assert(XKB_STATE_ಬಹುಲಕ_SERVER_QUERY >= 0 &&
              XKB_STATE_ಬಹುಲಕ_SERVER_QUERY < UINT32_ಅಗಲ, "");
static_assert(XKB_STATE_ಬಹುಲಕ_SERVER >= 0 &&
              XKB_STATE_ಬಹುಲಕ_SERVER < UINT32_ಅಗಲ, "");
static_assert(XKB_CONSUMED_ಬಹುಲಕ_XKB >= 0 &&
              XKB_CONSUMED_ಬಹುಲಕ_XKB < UINT32_ಅಗಲ, "");
static_assert(XKB_CONSUMED_ಬಹುಲಕ_GTK >= 0 &&
              XKB_CONSUMED_ಬಹುಲಕ_GTK < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1 < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_ಅಗಲ, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_ಅಗಲ, "");

ಎಣಿಕೆ xkb_enumerations_ಮೌಲ್ಯಗಳು {
    XKB_RMLVO_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_ಹೆಸರುಗಳು
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_ಬಹುಲಕ
    ,
    XKB_KEYMAP_FORMAT_ಮೌಲ್ಯಗಳು
        = (1u << XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
        | (1u << XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_ಬಳಸದ
        | XKB_KEYMAP_SERIALIZE_STRICT_ಬಹುಲಕ
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು
    ,
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_KEYMAP_ಕೀಲಿ_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ಕ್ರಮ
        | XKB_KEYMAP_ಕೀಲಿ_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_ಕೀಲಿ_DIRECTION_ಮೌಲ್ಯಗಳು
        = (1u << XKB_ಕೀಲಿ_ಮೇಲೆ)
        | (1u << XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ)
        | (1u << XKB_ಕೀಲಿ_REPEATED)
    ,
    XKB_STATE_COMPONENT_ಮೌಲ್ಯಗಳು
        = XKB_STATE_NO_COMPONENT
        | XKB_STATE_MODS_DEPRESSED
        | XKB_STATE_MODS_LATCHED
        | XKB_STATE_MODS_LOCKED
        | XKB_STATE_MODS_EFFECTIVE
        | XKB_STATE_LAYOUT_DEPRESSED
        | XKB_STATE_LAYOUT_LATCHED
        | XKB_STATE_LAYOUT_LOCKED
        | XKB_STATE_LAYOUT_EFFECTIVE
        | XKB_STATE_LEDS
        | XKB_STATE_CONTROLS_EFFECTIVE
    ,
    XKB_KEYBOARD_CONTROL_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು
        | XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು
    ,
    XKB_EVENT_TYPE_ಮೌಲ್ಯಗಳು
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_ಚೌಕಟ್ಟು)
        | (1u << XKB_EVENT_TYPE_ಕೀಲಿ)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_ಸೂಚಕ_MOTION)
        | (1u << XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_ಸೂಚಕ_MOTION_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_ಸೂಚಕ_MOTION_NO_FLAGS
        | XKB_ಸೂಚಕ_MOTION_REPEATS
        | XKB_ಸೂಚಕ_MOTION_ABSOLUTE_X
        | XKB_ಸೂಚಕ_MOTION_ABSOLUTE_Y
    ,
    XKB_ಸೂಚಕ_BUTTON_STATE_ಮೌಲ್ಯಗಳು
        = (1u << XKB_ಸೂಚಕ_BUTTON_RELEASED)
        | (1u << XKB_ಸೂಚಕ_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು
        | XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_ಗಮ್ಯ_ಬೀಗ
        | XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು
    ,
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_ಮೌಲ್ಯಗಳು
        = (1u << XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP)
        | (1u << XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT)
    ,
    XKB_STATE_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು
        = (1u << XKB_STATE_ಬಹುಲಕ_CLIENT)
        | (1u << XKB_STATE_ಬಹುಲಕ_SERVER_QUERY)
        | (1u << XKB_STATE_ಬಹುಲಕ_SERVER)
    ,
    XKB_STATE_ಹೊಂದಿಕೆ_ಮೌಲ್ಯಗಳು
        = XKB_STATE_ಹೊಂದಿಕೆ_ANY
        | XKB_STATE_ಹೊಂದಿಕೆ_ALL
        | XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು
        = (1u << XKB_CONSUMED_ಬಹುಲಕ_XKB)
        | (1u << XKB_CONSUMED_ಬಹುಲಕ_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_ಮೌಲ್ಯಗಳು
        = (1u << XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_ಮೌಲ್ಯಗಳು
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_ಫಲಿತಾಂಶ_ಮೌಲ್ಯಗಳು
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_ಅತಿಕಡಿಮೆ_ಮೌಲ್ಯ = XKB_ERROR_INVALID,
    ENUM_ಅತಿಕಡಿಮೆ_FLAG_ಮೌಲ್ಯ = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_ಮೌಲ್ಯ = XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_ಮೌಲ್ಯ = XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_rmlvo_builder_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keysym_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_context_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_ಹೆಸರುಗಳು,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_log_level_ಮೌಲ್ಯಗಳು[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

ಎಣಿಕೆ xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_ಅಂಕೆ = 5,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keymap_compile_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_ಬಹುಲಕ,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_ಬಹುಲಕ,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keymap_format_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1,
    XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2,
    XKB_KEYMAP_FORMAT_ಅಂಕೆ = 2,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keymap_serialize_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_ಬಳಸದ,
    XKB_KEYMAP_SERIALIZE_STRICT_ಬಹುಲಕ,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keymap_ಕೀಲಿ_iterator_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ಕ್ರಮ,
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_INCLUDE_UNBOUND,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keymap_ಕೀಲಿ_iterator_flags_bounds {
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_MIN = XKB_KEYMAP_ಕೀಲಿ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_MAX = XKB_KEYMAP_ಕೀಲಿ_ITERATOR_INCLUDE_UNBOUND,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_ಕೀಲಿ_direction_ಮೌಲ್ಯಗಳು[] = {
    XKB_ಕೀಲಿ_ಮೇಲೆ,
    XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
    XKB_ಕೀಲಿ_REPEATED,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_ಕೀಲಿ_direction_bounds {
    XKB_ಕೀಲಿ_DIRECTION_MIN = XKB_ಕೀಲಿ_ಮೇಲೆ,
    XKB_ಕೀಲಿ_DIRECTION_MAX = XKB_ಕೀಲಿ_REPEATED,
    XKB_ಕೀಲಿ_DIRECTION_ಅಂಕೆ = 3,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_state_component_ಮೌಲ್ಯಗಳು[] = {
    XKB_STATE_NO_COMPONENT,
    XKB_STATE_MODS_DEPRESSED,
    XKB_STATE_MODS_LATCHED,
    XKB_STATE_MODS_LOCKED,
    XKB_STATE_MODS_EFFECTIVE,
    XKB_STATE_LAYOUT_DEPRESSED,
    XKB_STATE_LAYOUT_LATCHED,
    XKB_STATE_LAYOUT_LOCKED,
    XKB_STATE_LAYOUT_EFFECTIVE,
    XKB_STATE_LEDS,
    XKB_STATE_CONTROLS_EFFECTIVE,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_keyboard_control_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
    XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_event_type_ಮೌಲ್ಯಗಳು[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_ಚೌಕಟ್ಟು,
    XKB_EVENT_TYPE_ಕೀಲಿ,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_ಸೂಚಕ_MOTION,
    XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_ಅಂಕೆ = 8,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_ಸೂಚಕ_motion_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_ಸೂಚಕ_MOTION_NO_FLAGS,
    XKB_ಸೂಚಕ_MOTION_REPEATS,
    XKB_ಸೂಚಕ_MOTION_ABSOLUTE_X,
    XKB_ಸೂಚಕ_MOTION_ABSOLUTE_Y,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_ಸೂಚಕ_motion_flags_bounds {
    XKB_ಸೂಚಕ_MOTION_FLAGS_MIN = XKB_ಸೂಚಕ_MOTION_NO_FLAGS,
    XKB_ಸೂಚಕ_MOTION_FLAGS_MAX = XKB_ಸೂಚಕ_MOTION_ABSOLUTE_Y,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_ಸೂಚಕ_button_state_ಮೌಲ್ಯಗಳು[] = {
    XKB_ಸೂಚಕ_BUTTON_RELEASED,
    XKB_ಸೂಚಕ_BUTTON_PRESSED,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_ಸೂಚಕ_button_state_bounds {
    XKB_ಸೂಚಕ_BUTTON_STATE_MIN = XKB_ಸೂಚಕ_BUTTON_RELEASED,
    XKB_ಸೂಚಕ_BUTTON_STATE_MAX = XKB_ಸೂಚಕ_BUTTON_PRESSED,
    XKB_ಸೂಚಕ_BUTTON_STATE_ಅಂಕೆ = 2,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_events_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_EVENTS_NO_FLAGS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_machine_builder_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_machine_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_a11y_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು,
    XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_ಗಮ್ಯ_ಬೀಗ,
    XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_layout_ಹೊರಹೋಗು_ದ_range_policy_ಮೌಲ್ಯಗಳು[] = {
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP,
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP,
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy_bounds {
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_MIN = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP,
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_MAX = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT,
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_ಅಂಕೆ = 3,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_state_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು[] = {
    XKB_STATE_ಬಹುಲಕ_CLIENT,
    XKB_STATE_ಬಹುಲಕ_SERVER_QUERY,
    XKB_STATE_ಬಹುಲಕ_SERVER,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_bounds {
    XKB_STATE_ಬಹುಲಕ_MIN = XKB_STATE_ಬಹುಲಕ_CLIENT,
    XKB_STATE_ಬಹುಲಕ_MAX = XKB_STATE_ಬಹುಲಕ_SERVER,
    XKB_STATE_ಬಹುಲಕ_ಅಂಕೆ = 3,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_state_ಹೊಂದಿಕೆ_ಮೌಲ್ಯಗಳು[] = {
    XKB_STATE_ಹೊಂದಿಕೆ_ANY,
    XKB_STATE_ಹೊಂದಿಕೆ_ALL,
    XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ_bounds {
    XKB_STATE_ಹೊಂದಿಕೆ_MIN = 0 /* implicit minimum */,
    XKB_STATE_ಹೊಂದಿಕೆ_MAX = XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_consumed_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು[] = {
    XKB_CONSUMED_ಬಹುಲಕ_XKB,
    XKB_CONSUMED_ಬಹುಲಕ_GTK,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_consumed_ಬಹುಲಕ_bounds {
    XKB_CONSUMED_ಬಹುಲಕ_MIN = XKB_CONSUMED_ಬಹುಲಕ_XKB,
    XKB_CONSUMED_ಬಹುಲಕ_MAX = XKB_CONSUMED_ಬಹುಲಕ_GTK,
    XKB_CONSUMED_ಬಹುಲಕ_ಅಂಕೆ = 2,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_compose_compile_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_compose_format_ಮೌಲ್ಯಗಳು[] = {
    XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_ಪಠ್ಯ_V1,
    XKB_COMPOSE_FORMAT_ಅಂಕೆ = 1,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_compose_state_flags_ಮೌಲ್ಯಗಳು[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_compose_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_compose_ಸ್ಥಿತಿ_bounds {
    XKB_COMPOSE_ಸ್ಥಿತಿ_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_ಸ್ಥಿತಿ_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_ಸ್ಥಿತಿ_ಅಂಕೆ = 4,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_compose_feed_ಫಲಿತಾಂಶ_ಮೌಲ್ಯಗಳು[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ xkb_compose_feed_ಫಲಿತಾಂಶ_bounds {
    XKB_COMPOSE_FEED_ಫಲಿತಾಂಶ_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ಫಲಿತಾಂಶ_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_ಫಲಿತಾಂಶ_ಅಂಕೆ = 2,
};
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
    XKB_ERROR_UNSUPPORTED_STATE_ಬಹುಲಕ,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_ಕೀಲಿ_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_ಎಣಿಕೆ_ಎಲ್ಲೆ_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ,
    XKB_ERROR_ABI_INVALID_STRUCT_ಗಾತ್ರ,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_ಹಂಚಿಕೆ_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ_bounds {
    XKB_ಸ್ಥಿತಿ_MIN = XKB_ERROR_INVALID,
    XKB_ಸ್ಥಿತಿ_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_ಸ್ಥಿತಿ_ಅಂಕೆ = 21,
};
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint32_t xkb_feature_ಮೌಲ್ಯಗಳು[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_ಸ್ಥಿತಿ,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_ಬಹುಲಕ,
    XKB_FEATURE_ENUM_STATE_ಹೊಂದಿಕೆ,
    XKB_FEATURE_ENUM_CONSUMED_ಬಹುಲಕ,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_ಕೀಲಿ_DIRECTION,
    XKB_FEATURE_ENUM_ಸೂಚಕ_MOTION_FLAGS,
    XKB_FEATURE_ENUM_ಸೂಚಕ_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_ಸ್ಥಿತಿ,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_ಫಲಿತಾಂಶ,
};

ಎಣಿಕೆ xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_ಫಲಿತಾಂಶ,
    XKB_FEATURE_ಅಂಕೆ = 29,
};
