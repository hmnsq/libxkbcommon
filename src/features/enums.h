/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-compose.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-features.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UINT32_ਚੌੜਾਈ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UINT32_ਚੌੜਾਈ (ਆਕਾਰ(uint32_t) * CHAR_BIT)
#ਜੇ_ਅੰਤ

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_ਪਾਠ_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_ਪਾਠ_V1 < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_KEYMAP_FORMAT_ਪਾਠ_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_ਪਾਠ_V2 < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_ਕੁੰਜੀ_ਉੱਪਰ >= 0 &&
              XKB_ਕੁੰਜੀ_ਉੱਪਰ < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_ਕੁੰਜੀ_ਥੱਲੇ >= 0 &&
              XKB_ਕੁੰਜੀ_ਥੱਲੇ < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_ਕੁੰਜੀ_REPEATED >= 0 &&
              XKB_ਕੁੰਜੀ_REPEATED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_ਚੌਖਟਾ >= 0 &&
              XKB_EVENT_TYPE_ਚੌਖਟਾ < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_ਕੁੰਜੀ >= 0 &&
              XKB_EVENT_TYPE_ਕੁੰਜੀ < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_ਸੂਚਕ_MOTION >= 0 &&
              XKB_EVENT_TYPE_ਸੂਚਕ_MOTION < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON >= 0 &&
              XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_ਸੂਚਕ_BUTTON_RELEASED >= 0 &&
              XKB_ਸੂਚਕ_BUTTON_RELEASED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_ਸੂਚਕ_BUTTON_PRESSED >= 0 &&
              XKB_ਸੂਚਕ_BUTTON_PRESSED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_STATE_ਬਹੁਲਕ_CLIENT >= 0 &&
              XKB_STATE_ਬਹੁਲਕ_CLIENT < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY >= 0 &&
              XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_STATE_ਬਹੁਲਕ_SERVER >= 0 &&
              XKB_STATE_ਬਹੁਲਕ_SERVER < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_CONSUMED_ਬਹੁਲਕ_XKB >= 0 &&
              XKB_CONSUMED_ਬਹੁਲਕ_XKB < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_CONSUMED_ਬਹੁਲਕ_GTK >= 0 &&
              XKB_CONSUMED_ਬਹੁਲਕ_GTK < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_FORMAT_ਪਾਠ_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_ਪਾਠ_V1 < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_ਚੌੜਾਈ, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_ਚੌੜਾਈ, "");

ਗਿਣਤੀ xkb_enumerations_ਮੁੱਲਾਂ {
    XKB_RMLVO_BUILDER_FLAGS_ਮੁੱਲਾਂ
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_ਮੁੱਲਾਂ
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_ਮੁੱਲਾਂ
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_ਨਾਂ
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_ਮੁੱਲਾਂ
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_ਬਹੁਲਕ
    ,
    XKB_KEYMAP_FORMAT_ਮੁੱਲਾਂ
        = (1u << XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
        | (1u << XKB_KEYMAP_FORMAT_ਪਾਠ_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_ਮੁੱਲਾਂ
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_ਅਣਵਰਤਿਆ
        | XKB_KEYMAP_SERIALIZE_STRICT_ਬਹੁਲਕ
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ਮੁੱਲਾਂ
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ
    ,
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_ਮੁੱਲਾਂ
        = XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_DESCENDING_ਕ੍ਰਮ
        | XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_ਕੁੰਜੀ_DIRECTION_ਮੁੱਲਾਂ
        = (1u << XKB_ਕੁੰਜੀ_ਉੱਪਰ)
        | (1u << XKB_ਕੁੰਜੀ_ਥੱਲੇ)
        | (1u << XKB_ਕੁੰਜੀ_REPEATED)
    ,
    XKB_STATE_COMPONENT_ਮੁੱਲਾਂ
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
    XKB_KEYBOARD_CONTROL_FLAGS_ਮੁੱਲਾਂ
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ
        | XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ
    ,
    XKB_EVENT_TYPE_ਮੁੱਲਾਂ
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_ਚੌਖਟਾ)
        | (1u << XKB_EVENT_TYPE_ਕੁੰਜੀ)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_ਸੂਚਕ_MOTION)
        | (1u << XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_ਸੂਚਕ_MOTION_FLAGS_ਮੁੱਲਾਂ
        = XKB_ਸੂਚਕ_MOTION_NO_FLAGS
        | XKB_ਸੂਚਕ_MOTION_REPEATS
        | XKB_ਸੂਚਕ_MOTION_ABSOLUTE_X
        | XKB_ਸੂਚਕ_MOTION_ABSOLUTE_Y
    ,
    XKB_ਸੂਚਕ_BUTTON_STATE_ਮੁੱਲਾਂ
        = (1u << XKB_ਸੂਚਕ_BUTTON_RELEASED)
        | (1u << XKB_ਸੂਚਕ_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_ਮੁੱਲਾਂ
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_ਮੁੱਲਾਂ
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_ਮੁੱਲਾਂ
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_ਮੁੱਲਾਂ
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_NO_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ
        | XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ
        | XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ
    ,
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_ਮੁੱਲਾਂ
        = (1u << XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP)
        | (1u << XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT)
    ,
    XKB_STATE_ਬਹੁਲਕ_ਮੁੱਲਾਂ
        = (1u << XKB_STATE_ਬਹੁਲਕ_CLIENT)
        | (1u << XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY)
        | (1u << XKB_STATE_ਬਹੁਲਕ_SERVER)
    ,
    XKB_STATE_ਮੇਲ_ਮੁੱਲਾਂ
        = XKB_STATE_ਮੇਲ_ANY
        | XKB_STATE_ਮੇਲ_ALL
        | XKB_STATE_ਮੇਲ_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_ਬਹੁਲਕ_ਮੁੱਲਾਂ
        = (1u << XKB_CONSUMED_ਬਹੁਲਕ_XKB)
        | (1u << XKB_CONSUMED_ਬਹੁਲਕ_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_ਮੁੱਲਾਂ
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_ਮੁੱਲਾਂ
        = (1u << XKB_COMPOSE_FORMAT_ਪਾਠ_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_ਮੁੱਲਾਂ
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_ਸਥਿਤੀ_ਮੁੱਲਾਂ
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_ਨਤੀਜਾ_ਮੁੱਲਾਂ
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਮੁੱਲ = XKB_ERROR_INVALID,
    ENUM_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_FLAG_ਮੁੱਲ = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_ਮੁੱਲ = XKB_STATE_ਮੇਲ_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_ਮੁੱਲ = XKB_STATE_ਮੇਲ_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_rmlvo_builder_flags_ਮੁੱਲਾਂ[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keysym_flags_ਮੁੱਲਾਂ[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_context_flags_ਮੁੱਲਾਂ[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_ਨਾਂ,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ਸਥਿਰ ਅਚਲ uint32_t xkb_log_level_ਮੁੱਲਾਂ[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

ਗਿਣਤੀ xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_ਅੰਕ = 5,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keymap_compile_flags_ਮੁੱਲਾਂ[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_ਬਹੁਲਕ,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_ਬਹੁਲਕ,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keymap_format_ਮੁੱਲਾਂ[] = {
    XKB_KEYMAP_FORMAT_ਪਾਠ_V1,
    XKB_KEYMAP_FORMAT_ਪਾਠ_V2,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_ਪਾਠ_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_ਪਾਠ_V2,
    XKB_KEYMAP_FORMAT_ਅੰਕ = 2,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keymap_serialize_flags_ਮੁੱਲਾਂ[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_ਅਣਵਰਤਿਆ,
    XKB_KEYMAP_SERIALIZE_STRICT_ਬਹੁਲਕ,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ਮੁੱਲਾਂ,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keymap_ਕੁੰਜੀ_iterator_flags_ਮੁੱਲਾਂ[] = {
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_DESCENDING_ਕ੍ਰਮ,
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_INCLUDE_UNBOUND,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keymap_ਕੁੰਜੀ_iterator_flags_bounds {
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_MIN = XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_MAX = XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_INCLUDE_UNBOUND,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_ਕੁੰਜੀ_direction_ਮੁੱਲਾਂ[] = {
    XKB_ਕੁੰਜੀ_ਉੱਪਰ,
    XKB_ਕੁੰਜੀ_ਥੱਲੇ,
    XKB_ਕੁੰਜੀ_REPEATED,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction_bounds {
    XKB_ਕੁੰਜੀ_DIRECTION_MIN = XKB_ਕੁੰਜੀ_ਉੱਪਰ,
    XKB_ਕੁੰਜੀ_DIRECTION_MAX = XKB_ਕੁੰਜੀ_REPEATED,
    XKB_ਕੁੰਜੀ_DIRECTION_ਅੰਕ = 3,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_state_component_ਮੁੱਲਾਂ[] = {
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
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_keyboard_control_flags_ਮੁੱਲਾਂ[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ,
    XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_event_type_ਮੁੱਲਾਂ[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_ਚੌਖਟਾ,
    XKB_EVENT_TYPE_ਕੁੰਜੀ,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_ਸੂਚਕ_MOTION,
    XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_ਅੰਕ = 8,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_ਸੂਚਕ_motion_flags_ਮੁੱਲਾਂ[] = {
    XKB_ਸੂਚਕ_MOTION_NO_FLAGS,
    XKB_ਸੂਚਕ_MOTION_REPEATS,
    XKB_ਸੂਚਕ_MOTION_ABSOLUTE_X,
    XKB_ਸੂਚਕ_MOTION_ABSOLUTE_Y,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_ਸੂਚਕ_motion_flags_bounds {
    XKB_ਸੂਚਕ_MOTION_FLAGS_MIN = XKB_ਸੂਚਕ_MOTION_NO_FLAGS,
    XKB_ਸੂਚਕ_MOTION_FLAGS_MAX = XKB_ਸੂਚਕ_MOTION_ABSOLUTE_Y,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_ਸੂਚਕ_button_state_ਮੁੱਲਾਂ[] = {
    XKB_ਸੂਚਕ_BUTTON_RELEASED,
    XKB_ਸੂਚਕ_BUTTON_PRESSED,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_ਸੂਚਕ_button_state_bounds {
    XKB_ਸੂਚਕ_BUTTON_STATE_MIN = XKB_ਸੂਚਕ_BUTTON_RELEASED,
    XKB_ਸੂਚਕ_BUTTON_STATE_MAX = XKB_ਸੂਚਕ_BUTTON_PRESSED,
    XKB_ਸੂਚਕ_BUTTON_STATE_ਅੰਕ = 2,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_events_flags_ਮੁੱਲਾਂ[] = {
    XKB_EVENTS_NO_FLAGS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_machine_builder_flags_ਮੁੱਲਾਂ[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_machine_flags_ਮੁੱਲਾਂ[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_a11y_flags_ਮੁੱਲਾਂ[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_NO_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ,
    XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ,
    XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy_ਮੁੱਲਾਂ[] = {
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP,
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP,
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy_bounds {
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_MIN = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP,
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_MAX = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT,
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_ਅੰਕ = 3,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_state_ਬਹੁਲਕ_ਮੁੱਲਾਂ[] = {
    XKB_STATE_ਬਹੁਲਕ_CLIENT,
    XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY,
    XKB_STATE_ਬਹੁਲਕ_SERVER,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_bounds {
    XKB_STATE_ਬਹੁਲਕ_MIN = XKB_STATE_ਬਹੁਲਕ_CLIENT,
    XKB_STATE_ਬਹੁਲਕ_MAX = XKB_STATE_ਬਹੁਲਕ_SERVER,
    XKB_STATE_ਬਹੁਲਕ_ਅੰਕ = 3,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_state_ਮੇਲ_ਮੁੱਲਾਂ[] = {
    XKB_STATE_ਮੇਲ_ANY,
    XKB_STATE_ਮੇਲ_ALL,
    XKB_STATE_ਮੇਲ_NON_EXCLUSIVE,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_state_ਮੇਲ_bounds {
    XKB_STATE_ਮੇਲ_MIN = 0 /* implicit minimum */,
    XKB_STATE_ਮੇਲ_MAX = XKB_STATE_ਮੇਲ_NON_EXCLUSIVE,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_consumed_ਬਹੁਲਕ_ਮੁੱਲਾਂ[] = {
    XKB_CONSUMED_ਬਹੁਲਕ_XKB,
    XKB_CONSUMED_ਬਹੁਲਕ_GTK,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_consumed_ਬਹੁਲਕ_bounds {
    XKB_CONSUMED_ਬਹੁਲਕ_MIN = XKB_CONSUMED_ਬਹੁਲਕ_XKB,
    XKB_CONSUMED_ਬਹੁਲਕ_MAX = XKB_CONSUMED_ਬਹੁਲਕ_GTK,
    XKB_CONSUMED_ਬਹੁਲਕ_ਅੰਕ = 2,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_compose_compile_flags_ਮੁੱਲਾਂ[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_compose_format_ਮੁੱਲਾਂ[] = {
    XKB_COMPOSE_FORMAT_ਪਾਠ_V1,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_ਪਾਠ_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_ਪਾਠ_V1,
    XKB_COMPOSE_FORMAT_ਅੰਕ = 1,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_compose_state_flags_ਮੁੱਲਾਂ[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_compose_ਸਥਿਤੀ_ਮੁੱਲਾਂ[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_compose_ਸਥਿਤੀ_bounds {
    XKB_COMPOSE_ਸਥਿਤੀ_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_ਸਥਿਤੀ_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_ਸਥਿਤੀ_ਅੰਕ = 4,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
ਸਥਿਰ ਅਚਲ uint32_t xkb_compose_feed_ਨਤੀਜਾ_ਮੁੱਲਾਂ[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ xkb_compose_feed_ਨਤੀਜਾ_bounds {
    XKB_COMPOSE_FEED_ਨਤੀਜਾ_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ਨਤੀਜਾ_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_ਨਤੀਜਾ_ਅੰਕ = 2,
};
ਸਥਿਰ ਅਚਲ uint32_t xkb_ਸਥਿਤੀ_ਮੁੱਲਾਂ[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
    XKB_ERROR_UNSUPPORTED_STATE_ਬਹੁਲਕ,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_ਕੁੰਜੀ_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_ਗਿਣਤੀ_ਹੱਦਬੰਦੀ_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ,
    XKB_ERROR_ABI_INVALID_STRUCT_ਮਾਪ,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_ਵੰਡ_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

ਗਿਣਤੀ xkb_ਸਥਿਤੀ_bounds {
    XKB_ਸਥਿਤੀ_MIN = XKB_ERROR_INVALID,
    XKB_ਸਥਿਤੀ_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_ਸਥਿਤੀ_ਅੰਕ = 21,
};
ਸਥਿਰ ਅਚਲ uint32_t xkb_feature_ਮੁੱਲਾਂ[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_ਸਥਿਤੀ,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_ਬਹੁਲਕ,
    XKB_FEATURE_ENUM_STATE_ਮੇਲ,
    XKB_FEATURE_ENUM_CONSUMED_ਬਹੁਲਕ,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_ਕੁੰਜੀ_DIRECTION,
    XKB_FEATURE_ENUM_ਸੂਚਕ_MOTION_FLAGS,
    XKB_FEATURE_ENUM_ਸੂਚਕ_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_ਸਥਿਤੀ,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_ਨਤੀਜਾ,
};

ਗਿਣਤੀ xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_ਨਤੀਜਾ,
    XKB_FEATURE_ਅੰਕ = 29,
};
