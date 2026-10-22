/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-compose.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-features.h"

#ifndef UINT32_WIDTH
#define UINT32_WIDTH (വലുപ്പം(uint32_t) * CHAR_BIT)
#endif

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_TEXT_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V1 < UINT32_WIDTH, "");
static_assert(XKB_KEYMAP_FORMAT_TEXT_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V2 < UINT32_WIDTH, "");
static_assert(XKB_താക്കോൽ_UP >= 0 &&
              XKB_താക്കോൽ_UP < UINT32_WIDTH, "");
static_assert(XKB_താക്കോൽ_DOWN >= 0 &&
              XKB_താക്കോൽ_DOWN < UINT32_WIDTH, "");
static_assert(XKB_താക്കോൽ_REPEATED >= 0 &&
              XKB_താക്കോൽ_REPEATED < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_FRAME >= 0 &&
              XKB_EVENT_TYPE_FRAME < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_താക്കോൽ >= 0 &&
              XKB_EVENT_TYPE_താക്കോൽ < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_POINTER_MOTION >= 0 &&
              XKB_EVENT_TYPE_POINTER_MOTION < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_POINTER_BUTTON >= 0 &&
              XKB_EVENT_TYPE_POINTER_BUTTON < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_WIDTH, "");
static_assert(XKB_POINTER_BUTTON_RELEASED >= 0 &&
              XKB_POINTER_BUTTON_RELEASED < UINT32_WIDTH, "");
static_assert(XKB_POINTER_BUTTON_PRESSED >= 0 &&
              XKB_POINTER_BUTTON_PRESSED < UINT32_WIDTH, "");
static_assert(XKB_LAYOUT_OUT_OF_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_OUT_OF_RANGE_WRAP < UINT32_WIDTH, "");
static_assert(XKB_LAYOUT_OUT_OF_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_OUT_OF_RANGE_CLAMP < UINT32_WIDTH, "");
static_assert(XKB_LAYOUT_OUT_OF_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_OUT_OF_RANGE_REDIRECT < UINT32_WIDTH, "");
static_assert(XKB_STATE_MODE_CLIENT >= 0 &&
              XKB_STATE_MODE_CLIENT < UINT32_WIDTH, "");
static_assert(XKB_STATE_MODE_SERVER_QUERY >= 0 &&
              XKB_STATE_MODE_SERVER_QUERY < UINT32_WIDTH, "");
static_assert(XKB_STATE_MODE_SERVER >= 0 &&
              XKB_STATE_MODE_SERVER < UINT32_WIDTH, "");
static_assert(XKB_CONSUMED_MODE_XKB >= 0 &&
              XKB_CONSUMED_MODE_XKB < UINT32_WIDTH, "");
static_assert(XKB_CONSUMED_MODE_GTK >= 0 &&
              XKB_CONSUMED_MODE_GTK < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_FORMAT_TEXT_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_TEXT_V1 < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_WIDTH, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_WIDTH, "");

എണ്ണൽ xkb_enumerations_മൂല്യങ്ങൾ {
    XKB_RMLVO_BUILDER_FLAGS_മൂല്യങ്ങൾ
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_മൂല്യങ്ങൾ
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_മൂല്യങ്ങൾ
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_NAMES
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_മൂല്യങ്ങൾ
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_MODE
    ,
    XKB_KEYMAP_FORMAT_മൂല്യങ്ങൾ
        = (1u << XKB_KEYMAP_FORMAT_TEXT_V1)
        | (1u << XKB_KEYMAP_FORMAT_TEXT_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_മൂല്യങ്ങൾ
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_UNUSED
        | XKB_KEYMAP_SERIALIZE_STRICT_MODE
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_മൂല്യങ്ങൾ
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ
    ,
    XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_മൂല്യങ്ങൾ
        = XKB_KEYMAP_താക്കോൽ_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_താക്കോൽ_ITERATOR_DESCENDING_ORDER
        | XKB_KEYMAP_താക്കോൽ_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_താക്കോൽ_DIRECTION_മൂല്യങ്ങൾ
        = (1u << XKB_താക്കോൽ_UP)
        | (1u << XKB_താക്കോൽ_DOWN)
        | (1u << XKB_താക്കോൽ_REPEATED)
    ,
    XKB_STATE_COMPONENT_മൂല്യങ്ങൾ
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
    XKB_KEYBOARD_CONTROL_FLAGS_മൂല്യങ്ങൾ
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ
        | XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ
    ,
    XKB_EVENT_TYPE_മൂല്യങ്ങൾ
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_FRAME)
        | (1u << XKB_EVENT_TYPE_താക്കോൽ)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_POINTER_MOTION)
        | (1u << XKB_EVENT_TYPE_POINTER_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_POINTER_MOTION_FLAGS_മൂല്യങ്ങൾ
        = XKB_POINTER_MOTION_NO_FLAGS
        | XKB_POINTER_MOTION_REPEATS
        | XKB_POINTER_MOTION_ABSOLUTE_X
        | XKB_POINTER_MOTION_ABSOLUTE_Y
    ,
    XKB_POINTER_BUTTON_STATE_മൂല്യങ്ങൾ
        = (1u << XKB_POINTER_BUTTON_RELEASED)
        | (1u << XKB_POINTER_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_മൂല്യങ്ങൾ
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_മൂല്യങ്ങൾ
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_മൂല്യങ്ങൾ
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_മൂല്യങ്ങൾ
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_താക്കോലുകൾ_NO_SIMULTANEOUS_താക്കോലുകൾ
        | XKB_A11_Y_STICKY_താക്കോലുകൾ_LATCH_TO_LOCK
        | XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ
    ,
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_മൂല്യങ്ങൾ
        = (1u << XKB_LAYOUT_OUT_OF_RANGE_WRAP)
        | (1u << XKB_LAYOUT_OUT_OF_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_OUT_OF_RANGE_REDIRECT)
    ,
    XKB_STATE_MODE_മൂല്യങ്ങൾ
        = (1u << XKB_STATE_MODE_CLIENT)
        | (1u << XKB_STATE_MODE_SERVER_QUERY)
        | (1u << XKB_STATE_MODE_SERVER)
    ,
    XKB_STATE_MATCH_മൂല്യങ്ങൾ
        = XKB_STATE_MATCH_ANY
        | XKB_STATE_MATCH_ALL
        | XKB_STATE_MATCH_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_MODE_മൂല്യങ്ങൾ
        = (1u << XKB_CONSUMED_MODE_XKB)
        | (1u << XKB_CONSUMED_MODE_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_മൂല്യങ്ങൾ
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_മൂല്യങ്ങൾ
        = (1u << XKB_COMPOSE_FORMAT_TEXT_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_മൂല്യങ്ങൾ
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_STATUS_മൂല്യങ്ങൾ
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_RESULT_മൂല്യങ്ങൾ
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_LOWEST_മൂല്യം = XKB_ERROR_INVALID,
    ENUM_LOWEST_FLAG_മൂല്യം = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_മൂല്യം = XKB_STATE_MATCH_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_മൂല്യം = XKB_STATE_MATCH_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_rmlvo_builder_flags_മൂല്യങ്ങൾ[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#endif

എണ്ണൽ xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keysym_flags_മൂല്യങ്ങൾ[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#endif

എണ്ണൽ xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_context_flags_മൂല്യങ്ങൾ[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_NAMES,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#endif

എണ്ണൽ xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
സ്ഥിതം സ്ഥിരം uint32_t xkb_log_level_മൂല്യങ്ങൾ[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

എണ്ണൽ xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_അക്കം = 5,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keymap_compile_flags_മൂല്യങ്ങൾ[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_MODE,
};
#endif

എണ്ണൽ xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_MODE,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keymap_format_മൂല്യങ്ങൾ[] = {
    XKB_KEYMAP_FORMAT_TEXT_V1,
    XKB_KEYMAP_FORMAT_TEXT_V2,
};
#endif

എണ്ണൽ xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_TEXT_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_TEXT_V2,
    XKB_KEYMAP_FORMAT_അക്കം = 2,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keymap_serialize_flags_മൂല്യങ്ങൾ[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_UNUSED,
    XKB_KEYMAP_SERIALIZE_STRICT_MODE,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_മൂല്യങ്ങൾ,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ,
};
#endif

എണ്ണൽ xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keymap_താക്കോൽ_iterator_flags_മൂല്യങ്ങൾ[] = {
    XKB_KEYMAP_താക്കോൽ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_താക്കോൽ_ITERATOR_DESCENDING_ORDER,
    XKB_KEYMAP_താക്കോൽ_ITERATOR_INCLUDE_UNBOUND,
};
#endif

എണ്ണൽ xkb_keymap_താക്കോൽ_iterator_flags_bounds {
    XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_MIN = XKB_KEYMAP_താക്കോൽ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_MAX = XKB_KEYMAP_താക്കോൽ_ITERATOR_INCLUDE_UNBOUND,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_താക്കോൽ_direction_മൂല്യങ്ങൾ[] = {
    XKB_താക്കോൽ_UP,
    XKB_താക്കോൽ_DOWN,
    XKB_താക്കോൽ_REPEATED,
};
#endif

എണ്ണൽ xkb_താക്കോൽ_direction_bounds {
    XKB_താക്കോൽ_DIRECTION_MIN = XKB_താക്കോൽ_UP,
    XKB_താക്കോൽ_DIRECTION_MAX = XKB_താക്കോൽ_REPEATED,
    XKB_താക്കോൽ_DIRECTION_അക്കം = 3,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_state_component_മൂല്യങ്ങൾ[] = {
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
#endif

എണ്ണൽ xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_keyboard_control_flags_മൂല്യങ്ങൾ[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ,
    XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ,
};
#endif

എണ്ണൽ xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_event_type_മൂല്യങ്ങൾ[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_FRAME,
    XKB_EVENT_TYPE_താക്കോൽ,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_POINTER_MOTION,
    XKB_EVENT_TYPE_POINTER_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#endif

എണ്ണൽ xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_അക്കം = 8,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_pointer_motion_flags_മൂല്യങ്ങൾ[] = {
    XKB_POINTER_MOTION_NO_FLAGS,
    XKB_POINTER_MOTION_REPEATS,
    XKB_POINTER_MOTION_ABSOLUTE_X,
    XKB_POINTER_MOTION_ABSOLUTE_Y,
};
#endif

എണ്ണൽ xkb_pointer_motion_flags_bounds {
    _XKB_POINTER_MOTION_FLAGS_MIN = XKB_POINTER_MOTION_NO_FLAGS,
    _XKB_POINTER_MOTION_FLAGS_MAX = XKB_POINTER_MOTION_ABSOLUTE_Y,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_pointer_button_state_മൂല്യങ്ങൾ[] = {
    XKB_POINTER_BUTTON_RELEASED,
    XKB_POINTER_BUTTON_PRESSED,
};
#endif

എണ്ണൽ xkb_pointer_button_state_bounds {
    _XKB_POINTER_BUTTON_STATE_MIN = XKB_POINTER_BUTTON_RELEASED,
    _XKB_POINTER_BUTTON_STATE_MAX = XKB_POINTER_BUTTON_PRESSED,
    XKB_POINTER_BUTTON_STATE_അക്കം = 2,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_events_flags_മൂല്യങ്ങൾ[] = {
    XKB_EVENTS_NO_FLAGS,
};
#endif

എണ്ണൽ xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_machine_builder_flags_മൂല്യങ്ങൾ[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#endif

എണ്ണൽ xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_machine_flags_മൂല്യങ്ങൾ[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#endif

എണ്ണൽ xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_a11y_flags_മൂല്യങ്ങൾ[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_താക്കോലുകൾ_NO_SIMULTANEOUS_താക്കോലുകൾ,
    XKB_A11_Y_STICKY_താക്കോലുകൾ_LATCH_TO_LOCK,
    XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ,
};
#endif

എണ്ണൽ xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_layout_out_of_range_policy_മൂല്യങ്ങൾ[] = {
    XKB_LAYOUT_OUT_OF_RANGE_WRAP,
    XKB_LAYOUT_OUT_OF_RANGE_CLAMP,
    XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
};
#endif

എണ്ണൽ xkb_layout_out_of_range_policy_bounds {
    _XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN = XKB_LAYOUT_OUT_OF_RANGE_WRAP,
    _XKB_LAYOUT_OUT_OF_RANGE_POLICY_MAX = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_അക്കം = 3,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_state_mode_മൂല്യങ്ങൾ[] = {
    XKB_STATE_MODE_CLIENT,
    XKB_STATE_MODE_SERVER_QUERY,
    XKB_STATE_MODE_SERVER,
};
#endif

എണ്ണൽ xkb_state_mode_bounds {
    _XKB_STATE_MODE_MIN = XKB_STATE_MODE_CLIENT,
    _XKB_STATE_MODE_MAX = XKB_STATE_MODE_SERVER,
    XKB_STATE_MODE_അക്കം = 3,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_state_match_മൂല്യങ്ങൾ[] = {
    XKB_STATE_MATCH_ANY,
    XKB_STATE_MATCH_ALL,
    XKB_STATE_MATCH_NON_EXCLUSIVE,
};
#endif

എണ്ണൽ xkb_state_match_bounds {
    _XKB_STATE_MATCH_MIN = 0 /* implicit minimum */,
    _XKB_STATE_MATCH_MAX = XKB_STATE_MATCH_NON_EXCLUSIVE,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_consumed_mode_മൂല്യങ്ങൾ[] = {
    XKB_CONSUMED_MODE_XKB,
    XKB_CONSUMED_MODE_GTK,
};
#endif

എണ്ണൽ xkb_consumed_mode_bounds {
    _XKB_CONSUMED_MODE_MIN = XKB_CONSUMED_MODE_XKB,
    _XKB_CONSUMED_MODE_MAX = XKB_CONSUMED_MODE_GTK,
    XKB_CONSUMED_MODE_അക്കം = 2,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_compose_compile_flags_മൂല്യങ്ങൾ[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#endif

എണ്ണൽ xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_compose_format_മൂല്യങ്ങൾ[] = {
    XKB_COMPOSE_FORMAT_TEXT_V1,
};
#endif

എണ്ണൽ xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_TEXT_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_TEXT_V1,
    XKB_COMPOSE_FORMAT_അക്കം = 1,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_compose_state_flags_മൂല്യങ്ങൾ[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#endif

എണ്ണൽ xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_compose_status_മൂല്യങ്ങൾ[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#endif

എണ്ണൽ xkb_compose_status_bounds {
    _XKB_COMPOSE_STATUS_MIN = XKB_COMPOSE_NOTHING,
    _XKB_COMPOSE_STATUS_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_STATUS_അക്കം = 4,
};

#ifdef ENABLE_PRIVATE_APIS
സ്ഥിതം സ്ഥിരം uint32_t xkb_compose_feed_result_മൂല്യങ്ങൾ[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#endif

എണ്ണൽ xkb_compose_feed_result_bounds {
    _XKB_COMPOSE_FEED_RESULT_MIN = XKB_COMPOSE_FEED_IGNORED,
    _XKB_COMPOSE_FEED_RESULT_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_RESULT_അക്കം = 2,
};
സ്ഥിതം സ്ഥിരം uint32_t xkb_status_മൂല്യങ്ങൾ[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
    XKB_ERROR_UNSUPPORTED_STATE_MODE,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_താക്കോൽ_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_എണ്ണം_LIMIT_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_MODE,
    XKB_ERROR_ABI_INVALID_STRUCT_SIZE,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_ALLOCATION_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

എണ്ണൽ xkb_status_bounds {
    _XKB_STATUS_MIN = XKB_ERROR_INVALID,
    _XKB_STATUS_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_STATUS_അക്കം = 21,
};
സ്ഥിതം സ്ഥിരം uint32_t xkb_feature_മൂല്യങ്ങൾ[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_STATUS,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_താക്കോൽ_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_MODE,
    XKB_FEATURE_ENUM_STATE_MATCH,
    XKB_FEATURE_ENUM_CONSUMED_MODE,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_താക്കോൽ_DIRECTION,
    XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS,
    XKB_FEATURE_ENUM_POINTER_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_STATUS,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT,
};

എണ്ണൽ xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT,
    XKB_FEATURE_അക്കം = 29,
};
