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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-features.h"

#ifndef UINT32_WIDTH
#define UINT32_WIDTH (حجم(uint32_t) * CHAR_BIT)
#endif

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_TEXT_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V1 < UINT32_WIDTH, "");
static_assert(XKB_KEYMAP_FORMAT_TEXT_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V2 < UINT32_WIDTH, "");
static_assert(XKB_ئاچقۇچ_UP >= 0 &&
              XKB_ئاچقۇچ_UP < UINT32_WIDTH, "");
static_assert(XKB_ئاچقۇچ_DOWN >= 0 &&
              XKB_ئاچقۇچ_DOWN < UINT32_WIDTH, "");
static_assert(XKB_ئاچقۇچ_REPEATED >= 0 &&
              XKB_ئاچقۇچ_REPEATED < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_FRAME >= 0 &&
              XKB_EVENT_TYPE_FRAME < UINT32_WIDTH, "");
static_assert(XKB_EVENT_TYPE_ئاچقۇچ >= 0 &&
              XKB_EVENT_TYPE_ئاچقۇچ < UINT32_WIDTH, "");
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

تعداد xkb_enumerations_قىممەتلەر {
    XKB_RMLVO_BUILDER_FLAGS_قىممەتلەر
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_قىممەتلەر
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_قىممەتلەر
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_NAMES
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_قىممەتلەر
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_MODE
    ,
    XKB_KEYMAP_FORMAT_قىممەتلەر
        = (1u << XKB_KEYMAP_FORMAT_TEXT_V1)
        | (1u << XKB_KEYMAP_FORMAT_TEXT_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_قىممەتلەر
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_UNUSED
        | XKB_KEYMAP_SERIALIZE_STRICT_MODE
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قىممەتلەر
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_ئاچقۇچ_قىممەتلەر
    ,
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS_قىممەتلەر
        = XKB_KEYMAP_ئاچقۇچ_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_ئاچقۇچ_ITERATOR_DESCENDING_ORDER
        | XKB_KEYMAP_ئاچقۇچ_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_ئاچقۇچ_DIRECTION_قىممەتلەر
        = (1u << XKB_ئاچقۇچ_UP)
        | (1u << XKB_ئاچقۇچ_DOWN)
        | (1u << XKB_ئاچقۇچ_REPEATED)
    ,
    XKB_STATE_COMPONENT_قىممەتلەر
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
    XKB_KEYBOARD_CONTROL_FLAGS_قىممەتلەر
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ئاچقۇچلار
        | XKB_KEYBOARD_CONTROL_MOUSE_ئاچقۇچلار
    ,
    XKB_EVENT_TYPE_قىممەتلەر
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_FRAME)
        | (1u << XKB_EVENT_TYPE_ئاچقۇچ)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_POINTER_MOTION)
        | (1u << XKB_EVENT_TYPE_POINTER_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_POINTER_MOTION_FLAGS_قىممەتلەر
        = XKB_POINTER_MOTION_NO_FLAGS
        | XKB_POINTER_MOTION_REPEATS
        | XKB_POINTER_MOTION_ABSOLUTE_X
        | XKB_POINTER_MOTION_ABSOLUTE_Y
    ,
    XKB_POINTER_BUTTON_STATE_قىممەتلەر
        = (1u << XKB_POINTER_BUTTON_RELEASED)
        | (1u << XKB_POINTER_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_قىممەتلەر
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_قىممەتلەر
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_قىممەتلەر
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ئاچقۇچ_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_قىممەتلەر
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_ئاچقۇچلار_NO_SIMULTANEOUS_ئاچقۇچلار
        | XKB_A11_Y_STICKY_ئاچقۇچلار_LATCH_TO_LOCK
        | XKB_A11_Y_LATCH_SIMULTANEOUS_ئاچقۇچلار
    ,
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_قىممەتلەر
        = (1u << XKB_LAYOUT_OUT_OF_RANGE_WRAP)
        | (1u << XKB_LAYOUT_OUT_OF_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_OUT_OF_RANGE_REDIRECT)
    ,
    XKB_STATE_MODE_قىممەتلەر
        = (1u << XKB_STATE_MODE_CLIENT)
        | (1u << XKB_STATE_MODE_SERVER_QUERY)
        | (1u << XKB_STATE_MODE_SERVER)
    ,
    XKB_STATE_MATCH_قىممەتلەر
        = XKB_STATE_MATCH_ANY
        | XKB_STATE_MATCH_ALL
        | XKB_STATE_MATCH_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_MODE_قىممەتلەر
        = (1u << XKB_CONSUMED_MODE_XKB)
        | (1u << XKB_CONSUMED_MODE_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_قىممەتلەر
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_قىممەتلەر
        = (1u << XKB_COMPOSE_FORMAT_TEXT_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_قىممەتلەر
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_STATUS_قىممەتلەر
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_RESULT_قىممەتلەر
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_LOWEST_قىممەت = XKB_ERROR_INVALID,
    ENUM_LOWEST_FLAG_قىممەت = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_قىممەت = XKB_STATE_MATCH_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_قىممەت = XKB_STATE_MATCH_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_rmlvo_builder_flags_قىممەتلەر[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#endif

تعداد xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keysym_flags_قىممەتلەر[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#endif

تعداد xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_context_flags_قىممەتلەر[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_NAMES,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#endif

تعداد xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ساكن ثابت uint32_t xkb_log_level_قىممەتلەر[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

تعداد xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_رەقەم = 5,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keymap_compile_flags_قىممەتلەر[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_MODE,
};
#endif

تعداد xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_MODE,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keymap_format_قىممەتلەر[] = {
    XKB_KEYMAP_FORMAT_TEXT_V1,
    XKB_KEYMAP_FORMAT_TEXT_V2,
};
#endif

تعداد xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_TEXT_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_TEXT_V2,
    XKB_KEYMAP_FORMAT_رەقەم = 2,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keymap_serialize_flags_قىممەتلەر[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_UNUSED,
    XKB_KEYMAP_SERIALIZE_STRICT_MODE,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قىممەتلەر,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_ئاچقۇچ_قىممەتلەر,
};
#endif

تعداد xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_ئاچقۇچ_قىممەتلەر,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keymap_ئاچقۇچ_iterator_flags_قىممەتلەر[] = {
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_DESCENDING_ORDER,
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_INCLUDE_UNBOUND,
};
#endif

تعداد xkb_keymap_ئاچقۇچ_iterator_flags_bounds {
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS_MIN = XKB_KEYMAP_ئاچقۇچ_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS_MAX = XKB_KEYMAP_ئاچقۇچ_ITERATOR_INCLUDE_UNBOUND,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_ئاچقۇچ_direction_قىممەتلەر[] = {
    XKB_ئاچقۇچ_UP,
    XKB_ئاچقۇچ_DOWN,
    XKB_ئاچقۇچ_REPEATED,
};
#endif

تعداد xkb_ئاچقۇچ_direction_bounds {
    XKB_ئاچقۇچ_DIRECTION_MIN = XKB_ئاچقۇچ_UP,
    XKB_ئاچقۇچ_DIRECTION_MAX = XKB_ئاچقۇچ_REPEATED,
    XKB_ئاچقۇچ_DIRECTION_رەقەم = 3,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_state_component_قىممەتلەر[] = {
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

تعداد xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_keyboard_control_flags_قىممەتلەر[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ئاچقۇچلار,
    XKB_KEYBOARD_CONTROL_MOUSE_ئاچقۇچلار,
};
#endif

تعداد xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_ئاچقۇچلار,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_event_type_قىممەتلەر[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_FRAME,
    XKB_EVENT_TYPE_ئاچقۇچ,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_POINTER_MOTION,
    XKB_EVENT_TYPE_POINTER_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#endif

تعداد xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_رەقەم = 8,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_pointer_motion_flags_قىممەتلەر[] = {
    XKB_POINTER_MOTION_NO_FLAGS,
    XKB_POINTER_MOTION_REPEATS,
    XKB_POINTER_MOTION_ABSOLUTE_X,
    XKB_POINTER_MOTION_ABSOLUTE_Y,
};
#endif

تعداد xkb_pointer_motion_flags_bounds {
    _XKB_POINTER_MOTION_FLAGS_MIN = XKB_POINTER_MOTION_NO_FLAGS,
    _XKB_POINTER_MOTION_FLAGS_MAX = XKB_POINTER_MOTION_ABSOLUTE_Y,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_pointer_button_state_قىممەتلەر[] = {
    XKB_POINTER_BUTTON_RELEASED,
    XKB_POINTER_BUTTON_PRESSED,
};
#endif

تعداد xkb_pointer_button_state_bounds {
    _XKB_POINTER_BUTTON_STATE_MIN = XKB_POINTER_BUTTON_RELEASED,
    _XKB_POINTER_BUTTON_STATE_MAX = XKB_POINTER_BUTTON_PRESSED,
    XKB_POINTER_BUTTON_STATE_رەقەم = 2,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_events_flags_قىممەتلەر[] = {
    XKB_EVENTS_NO_FLAGS,
};
#endif

تعداد xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_machine_builder_flags_قىممەتلەر[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#endif

تعداد xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_machine_flags_قىممەتلەر[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ئاچقۇچ_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#endif

تعداد xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_a11y_flags_قىممەتلەر[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_ئاچقۇچلار_NO_SIMULTANEOUS_ئاچقۇچلار,
    XKB_A11_Y_STICKY_ئاچقۇچلار_LATCH_TO_LOCK,
    XKB_A11_Y_LATCH_SIMULTANEOUS_ئاچقۇچلار,
};
#endif

تعداد xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_SIMULTANEOUS_ئاچقۇچلار,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_layout_out_of_range_policy_قىممەتلەر[] = {
    XKB_LAYOUT_OUT_OF_RANGE_WRAP,
    XKB_LAYOUT_OUT_OF_RANGE_CLAMP,
    XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
};
#endif

تعداد xkb_layout_out_of_range_policy_bounds {
    _XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN = XKB_LAYOUT_OUT_OF_RANGE_WRAP,
    _XKB_LAYOUT_OUT_OF_RANGE_POLICY_MAX = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_رەقەم = 3,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_state_mode_قىممەتلەر[] = {
    XKB_STATE_MODE_CLIENT,
    XKB_STATE_MODE_SERVER_QUERY,
    XKB_STATE_MODE_SERVER,
};
#endif

تعداد xkb_state_mode_bounds {
    _XKB_STATE_MODE_MIN = XKB_STATE_MODE_CLIENT,
    _XKB_STATE_MODE_MAX = XKB_STATE_MODE_SERVER,
    XKB_STATE_MODE_رەقەم = 3,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_state_match_قىممەتلەر[] = {
    XKB_STATE_MATCH_ANY,
    XKB_STATE_MATCH_ALL,
    XKB_STATE_MATCH_NON_EXCLUSIVE,
};
#endif

تعداد xkb_state_match_bounds {
    _XKB_STATE_MATCH_MIN = 0 /* implicit minimum */,
    _XKB_STATE_MATCH_MAX = XKB_STATE_MATCH_NON_EXCLUSIVE,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_consumed_mode_قىممەتلەر[] = {
    XKB_CONSUMED_MODE_XKB,
    XKB_CONSUMED_MODE_GTK,
};
#endif

تعداد xkb_consumed_mode_bounds {
    _XKB_CONSUMED_MODE_MIN = XKB_CONSUMED_MODE_XKB,
    _XKB_CONSUMED_MODE_MAX = XKB_CONSUMED_MODE_GTK,
    XKB_CONSUMED_MODE_رەقەم = 2,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_compose_compile_flags_قىممەتلەر[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#endif

تعداد xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_compose_format_قىممەتلەر[] = {
    XKB_COMPOSE_FORMAT_TEXT_V1,
};
#endif

تعداد xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_TEXT_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_TEXT_V1,
    XKB_COMPOSE_FORMAT_رەقەم = 1,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_compose_state_flags_قىممەتلەر[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#endif

تعداد xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_compose_status_قىممەتلەر[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#endif

تعداد xkb_compose_status_bounds {
    _XKB_COMPOSE_STATUS_MIN = XKB_COMPOSE_NOTHING,
    _XKB_COMPOSE_STATUS_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_STATUS_رەقەم = 4,
};

#ifdef ENABLE_PRIVATE_APIS
ساكن ثابت uint32_t xkb_compose_feed_result_قىممەتلەر[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#endif

تعداد xkb_compose_feed_result_bounds {
    _XKB_COMPOSE_FEED_RESULT_MIN = XKB_COMPOSE_FEED_IGNORED,
    _XKB_COMPOSE_FEED_RESULT_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_RESULT_رەقەم = 2,
};
ساكن ثابت uint32_t xkb_status_قىممەتلەر[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
    XKB_ERROR_UNSUPPORTED_STATE_MODE,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_ئاچقۇچ_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_سانى_LIMIT_EXCEEDED,
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

تعداد xkb_status_bounds {
    _XKB_STATUS_MIN = XKB_ERROR_INVALID,
    _XKB_STATUS_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_STATUS_رەقەم = 21,
};
ساكن ثابت uint32_t xkb_feature_قىممەتلەر[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_STATUS,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS,
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
    XKB_FEATURE_ENUM_ئاچقۇچ_DIRECTION,
    XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS,
    XKB_FEATURE_ENUM_POINTER_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_STATUS,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT,
};

تعداد xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT,
    XKB_FEATURE_رەقەم = 29,
};
