/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-compose.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcommon/xkbcommon-features.h"

#నిర్వచితం_కాకపోతే UINT32_వెడల్పు
#నిర్వచించు UINT32_వెడల్పు (పరిమాణం(uint32_t) * CHAR_BIT)
#అయితే_ముగింపు

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_పాఠ్యం_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_పాఠ్యం_V1 < UINT32_వెడల్పు, "");
static_assert(XKB_KEYMAP_FORMAT_పాఠ్యం_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_పాఠ్యం_V2 < UINT32_వెడల్పు, "");
static_assert(XKB_కీలకం_పైకి >= 0 &&
              XKB_కీలకం_పైకి < UINT32_వెడల్పు, "");
static_assert(XKB_కీలకం_కిందికి >= 0 &&
              XKB_కీలకం_కిందికి < UINT32_వెడల్పు, "");
static_assert(XKB_కీలకం_REPEATED >= 0 &&
              XKB_కీలకం_REPEATED < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_చట్రం >= 0 &&
              XKB_EVENT_TYPE_చట్రం < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_కీలకం >= 0 &&
              XKB_EVENT_TYPE_కీలకం < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_సూచిక_MOTION >= 0 &&
              XKB_EVENT_TYPE_సూచిక_MOTION < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_సూచిక_BUTTON >= 0 &&
              XKB_EVENT_TYPE_సూచిక_BUTTON < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_వెడల్పు, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_వెడల్పు, "");
static_assert(XKB_సూచిక_BUTTON_RELEASED >= 0 &&
              XKB_సూచిక_BUTTON_RELEASED < UINT32_వెడల్పు, "");
static_assert(XKB_సూచిక_BUTTON_PRESSED >= 0 &&
              XKB_సూచిక_BUTTON_PRESSED < UINT32_వెడల్పు, "");
static_assert(XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP < UINT32_వెడల్పు, "");
static_assert(XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP < UINT32_వెడల్పు, "");
static_assert(XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT < UINT32_వెడల్పు, "");
static_assert(XKB_STATE_బహుళకం_CLIENT >= 0 &&
              XKB_STATE_బహుళకం_CLIENT < UINT32_వెడల్పు, "");
static_assert(XKB_STATE_బహుళకం_SERVER_QUERY >= 0 &&
              XKB_STATE_బహుళకం_SERVER_QUERY < UINT32_వెడల్పు, "");
static_assert(XKB_STATE_బహుళకం_SERVER >= 0 &&
              XKB_STATE_బహుళకం_SERVER < UINT32_వెడల్పు, "");
static_assert(XKB_CONSUMED_బహుళకం_XKB >= 0 &&
              XKB_CONSUMED_బహుళకం_XKB < UINT32_వెడల్పు, "");
static_assert(XKB_CONSUMED_బహుళకం_GTK >= 0 &&
              XKB_CONSUMED_బహుళకం_GTK < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_FORMAT_పాఠ్యం_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_పాఠ్యం_V1 < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_వెడల్పు, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_వెడల్పు, "");

గణన xkb_enumerations_విలువలు {
    XKB_RMLVO_BUILDER_FLAGS_విలువలు
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_విలువలు
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_విలువలు
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_పేర్లు
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_విలువలు
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_బహుళకం
    ,
    XKB_KEYMAP_FORMAT_విలువలు
        = (1u << XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
        | (1u << XKB_KEYMAP_FORMAT_పాఠ్యం_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_విలువలు
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_వాడని
        | XKB_KEYMAP_SERIALIZE_STRICT_బహుళకం
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_విలువలు
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు
    ,
    XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_విలువలు
        = XKB_KEYMAP_కీలకం_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_కీలకం_ITERATOR_DESCENDING_క్రమం
        | XKB_KEYMAP_కీలకం_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_కీలకం_DIRECTION_విలువలు
        = (1u << XKB_కీలకం_పైకి)
        | (1u << XKB_కీలకం_కిందికి)
        | (1u << XKB_కీలకం_REPEATED)
    ,
    XKB_STATE_COMPONENT_విలువలు
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
    XKB_KEYBOARD_CONTROL_FLAGS_విలువలు
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు
        | XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు
    ,
    XKB_EVENT_TYPE_విలువలు
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_చట్రం)
        | (1u << XKB_EVENT_TYPE_కీలకం)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_సూచిక_MOTION)
        | (1u << XKB_EVENT_TYPE_సూచిక_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_సూచిక_MOTION_FLAGS_విలువలు
        = XKB_సూచిక_MOTION_NO_FLAGS
        | XKB_సూచిక_MOTION_REPEATS
        | XKB_సూచిక_MOTION_ABSOLUTE_X
        | XKB_సూచిక_MOTION_ABSOLUTE_Y
    ,
    XKB_సూచిక_BUTTON_STATE_విలువలు
        = (1u << XKB_సూచిక_BUTTON_RELEASED)
        | (1u << XKB_సూచిక_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_విలువలు
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_విలువలు
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_విలువలు
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_కీలకం_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_విలువలు
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_కీలకాలు_NO_ఏకకాల_కీలకాలు
        | XKB_A11_Y_STICKY_కీలకాలు_LATCH_గమ్యం_తాళం
        | XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు
    ,
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_విలువలు
        = (1u << XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP)
        | (1u << XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT)
    ,
    XKB_STATE_బహుళకం_విలువలు
        = (1u << XKB_STATE_బహుళకం_CLIENT)
        | (1u << XKB_STATE_బహుళకం_SERVER_QUERY)
        | (1u << XKB_STATE_బహుళకం_SERVER)
    ,
    XKB_STATE_సరిపోలిక_విలువలు
        = XKB_STATE_సరిపోలిక_ANY
        | XKB_STATE_సరిపోలిక_ALL
        | XKB_STATE_సరిపోలిక_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_బహుళకం_విలువలు
        = (1u << XKB_CONSUMED_బహుళకం_XKB)
        | (1u << XKB_CONSUMED_బహుళకం_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_విలువలు
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_విలువలు
        = (1u << XKB_COMPOSE_FORMAT_పాఠ్యం_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_విలువలు
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_స్థితి_విలువలు
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_ఫలితం_విలువలు
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_అత్యల్ప_విలువ = XKB_ERROR_INVALID,
    ENUM_అత్యల్ప_FLAG_విలువ = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_విలువ = XKB_STATE_సరిపోలిక_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_విలువ = XKB_STATE_సరిపోలిక_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_rmlvo_builder_flags_విలువలు[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#అయితే_ముగింపు

గణన xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keysym_flags_విలువలు[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#అయితే_ముగింపు

గణన xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_context_flags_విలువలు[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_పేర్లు,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#అయితే_ముగింపు

గణన xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
స్థిర స్థిరరాశి uint32_t xkb_log_level_విలువలు[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

గణన xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_అంకె = 5,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keymap_compile_flags_విలువలు[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_బహుళకం,
};
#అయితే_ముగింపు

గణన xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_బహుళకం,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keymap_format_విలువలు[] = {
    XKB_KEYMAP_FORMAT_పాఠ్యం_V1,
    XKB_KEYMAP_FORMAT_పాఠ్యం_V2,
};
#అయితే_ముగింపు

గణన xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_పాఠ్యం_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_పాఠ్యం_V2,
    XKB_KEYMAP_FORMAT_అంకె = 2,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keymap_serialize_flags_విలువలు[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_వాడని,
    XKB_KEYMAP_SERIALIZE_STRICT_బహుళకం,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_విలువలు,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు,
};
#అయితే_ముగింపు

గణన xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keymap_కీలకం_iterator_flags_విలువలు[] = {
    XKB_KEYMAP_కీలకం_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_కీలకం_ITERATOR_DESCENDING_క్రమం,
    XKB_KEYMAP_కీలకం_ITERATOR_INCLUDE_UNBOUND,
};
#అయితే_ముగింపు

గణన xkb_keymap_కీలకం_iterator_flags_bounds {
    XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_MIN = XKB_KEYMAP_కీలకం_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_MAX = XKB_KEYMAP_కీలకం_ITERATOR_INCLUDE_UNBOUND,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_కీలకం_direction_విలువలు[] = {
    XKB_కీలకం_పైకి,
    XKB_కీలకం_కిందికి,
    XKB_కీలకం_REPEATED,
};
#అయితే_ముగింపు

గణన xkb_కీలకం_direction_bounds {
    XKB_కీలకం_DIRECTION_MIN = XKB_కీలకం_పైకి,
    XKB_కీలకం_DIRECTION_MAX = XKB_కీలకం_REPEATED,
    XKB_కీలకం_DIRECTION_అంకె = 3,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_state_component_విలువలు[] = {
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
#అయితే_ముగింపు

గణన xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_keyboard_control_flags_విలువలు[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు,
    XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు,
};
#అయితే_ముగింపు

గణన xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_event_type_విలువలు[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_చట్రం,
    XKB_EVENT_TYPE_కీలకం,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_సూచిక_MOTION,
    XKB_EVENT_TYPE_సూచిక_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#అయితే_ముగింపు

గణన xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_అంకె = 8,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_సూచిక_motion_flags_విలువలు[] = {
    XKB_సూచిక_MOTION_NO_FLAGS,
    XKB_సూచిక_MOTION_REPEATS,
    XKB_సూచిక_MOTION_ABSOLUTE_X,
    XKB_సూచిక_MOTION_ABSOLUTE_Y,
};
#అయితే_ముగింపు

గణన xkb_సూచిక_motion_flags_bounds {
    XKB_సూచిక_MOTION_FLAGS_MIN = XKB_సూచిక_MOTION_NO_FLAGS,
    XKB_సూచిక_MOTION_FLAGS_MAX = XKB_సూచిక_MOTION_ABSOLUTE_Y,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_సూచిక_button_state_విలువలు[] = {
    XKB_సూచిక_BUTTON_RELEASED,
    XKB_సూచిక_BUTTON_PRESSED,
};
#అయితే_ముగింపు

గణన xkb_సూచిక_button_state_bounds {
    XKB_సూచిక_BUTTON_STATE_MIN = XKB_సూచిక_BUTTON_RELEASED,
    XKB_సూచిక_BUTTON_STATE_MAX = XKB_సూచిక_BUTTON_PRESSED,
    XKB_సూచిక_BUTTON_STATE_అంకె = 2,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_events_flags_విలువలు[] = {
    XKB_EVENTS_NO_FLAGS,
};
#అయితే_ముగింపు

గణన xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_machine_builder_flags_విలువలు[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#అయితే_ముగింపు

గణన xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_machine_flags_విలువలు[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_కీలకం_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#అయితే_ముగింపు

గణన xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_a11y_flags_విలువలు[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_కీలకాలు_NO_ఏకకాల_కీలకాలు,
    XKB_A11_Y_STICKY_కీలకాలు_LATCH_గమ్యం_తాళం,
    XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు,
};
#అయితే_ముగింపు

గణన xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_layout_బహిర్గతం_యొక్క_range_policy_విలువలు[] = {
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP,
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP,
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT,
};
#అయితే_ముగింపు

గణన xkb_layout_బహిర్గతం_యొక్క_range_policy_bounds {
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_MIN = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP,
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_MAX = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT,
    XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_అంకె = 3,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_state_బహుళకం_విలువలు[] = {
    XKB_STATE_బహుళకం_CLIENT,
    XKB_STATE_బహుళకం_SERVER_QUERY,
    XKB_STATE_బహుళకం_SERVER,
};
#అయితే_ముగింపు

గణన xkb_state_బహుళకం_bounds {
    XKB_STATE_బహుళకం_MIN = XKB_STATE_బహుళకం_CLIENT,
    XKB_STATE_బహుళకం_MAX = XKB_STATE_బహుళకం_SERVER,
    XKB_STATE_బహుళకం_అంకె = 3,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_state_సరిపోలిక_విలువలు[] = {
    XKB_STATE_సరిపోలిక_ANY,
    XKB_STATE_సరిపోలిక_ALL,
    XKB_STATE_సరిపోలిక_NON_EXCLUSIVE,
};
#అయితే_ముగింపు

గణన xkb_state_సరిపోలిక_bounds {
    XKB_STATE_సరిపోలిక_MIN = 0 /* implicit minimum */,
    XKB_STATE_సరిపోలిక_MAX = XKB_STATE_సరిపోలిక_NON_EXCLUSIVE,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_consumed_బహుళకం_విలువలు[] = {
    XKB_CONSUMED_బహుళకం_XKB,
    XKB_CONSUMED_బహుళకం_GTK,
};
#అయితే_ముగింపు

గణన xkb_consumed_బహుళకం_bounds {
    XKB_CONSUMED_బహుళకం_MIN = XKB_CONSUMED_బహుళకం_XKB,
    XKB_CONSUMED_బహుళకం_MAX = XKB_CONSUMED_బహుళకం_GTK,
    XKB_CONSUMED_బహుళకం_అంకె = 2,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_compose_compile_flags_విలువలు[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#అయితే_ముగింపు

గణన xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_compose_format_విలువలు[] = {
    XKB_COMPOSE_FORMAT_పాఠ్యం_V1,
};
#అయితే_ముగింపు

గణన xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_పాఠ్యం_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_పాఠ్యం_V1,
    XKB_COMPOSE_FORMAT_అంకె = 1,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_compose_state_flags_విలువలు[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#అయితే_ముగింపు

గణన xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_compose_స్థితి_విలువలు[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#అయితే_ముగింపు

గణన xkb_compose_స్థితి_bounds {
    XKB_COMPOSE_స్థితి_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_స్థితి_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_స్థితి_అంకె = 4,
};

#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
స్థిర స్థిరరాశి uint32_t xkb_compose_feed_ఫలితం_విలువలు[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#అయితే_ముగింపు

గణన xkb_compose_feed_ఫలితం_bounds {
    XKB_COMPOSE_FEED_ఫలితం_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ఫలితం_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_ఫలితం_అంకె = 2,
};
స్థిర స్థిరరాశి uint32_t xkb_స్థితి_విలువలు[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
    XKB_ERROR_UNSUPPORTED_STATE_బహుళకం,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_కీలకం_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_లెక్క_హద్దు_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_బహుళకం,
    XKB_ERROR_ABI_INVALID_STRUCT_కొలత,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_కేటాయింపు_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

గణన xkb_స్థితి_bounds {
    XKB_స్థితి_MIN = XKB_ERROR_INVALID,
    XKB_స్థితి_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_స్థితి_అంకె = 21,
};
స్థిర స్థిరరాశి uint32_t xkb_feature_విలువలు[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_స్థితి,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_కీలకం_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_బహుళకం,
    XKB_FEATURE_ENUM_STATE_సరిపోలిక,
    XKB_FEATURE_ENUM_CONSUMED_బహుళకం,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_కీలకం_DIRECTION,
    XKB_FEATURE_ENUM_సూచిక_MOTION_FLAGS,
    XKB_FEATURE_ENUM_సూచిక_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_స్థితి,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_ఫలితం,
};

గణన xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_ఫలితం,
    XKB_FEATURE_అంకె = 29,
};
