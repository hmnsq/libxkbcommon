/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-compose.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-features.h"

#यदि_अपरिभाषितम् UINT32_विस्तारः
#परिभाषय UINT32_विस्तारः (परिमाण(uint32_t) * CHAR_BIT)
#यदि_अन्तः

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_TEXT_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V1 < UINT32_विस्तारः, "");
static_assert(XKB_KEYMAP_FORMAT_TEXT_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_TEXT_V2 < UINT32_विस्तारः, "");
static_assert(XKB_कुञ्चिका_ऊर्ध्वम् >= 0 &&
              XKB_कुञ्चिका_ऊर्ध्वम् < UINT32_विस्तारः, "");
static_assert(XKB_कुञ्चिका_अधस्तात् >= 0 &&
              XKB_कुञ्चिका_अधस्तात् < UINT32_विस्तारः, "");
static_assert(XKB_कुञ्चिका_REPEATED >= 0 &&
              XKB_कुञ्चिका_REPEATED < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_FRAME >= 0 &&
              XKB_EVENT_TYPE_FRAME < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_कुञ्चिका >= 0 &&
              XKB_EVENT_TYPE_कुञ्चिका < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_POINTER_MOTION >= 0 &&
              XKB_EVENT_TYPE_POINTER_MOTION < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_POINTER_BUTTON >= 0 &&
              XKB_EVENT_TYPE_POINTER_BUTTON < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_विस्तारः, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_विस्तारः, "");
static_assert(XKB_POINTER_BUTTON_RELEASED >= 0 &&
              XKB_POINTER_BUTTON_RELEASED < UINT32_विस्तारः, "");
static_assert(XKB_POINTER_BUTTON_PRESSED >= 0 &&
              XKB_POINTER_BUTTON_PRESSED < UINT32_विस्तारः, "");
static_assert(XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP < UINT32_विस्तारः, "");
static_assert(XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP < UINT32_विस्तारः, "");
static_assert(XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT < UINT32_विस्तारः, "");
static_assert(XKB_STATE_बहुलकम्_CLIENT >= 0 &&
              XKB_STATE_बहुलकम्_CLIENT < UINT32_विस्तारः, "");
static_assert(XKB_STATE_बहुलकम्_SERVER_QUERY >= 0 &&
              XKB_STATE_बहुलकम्_SERVER_QUERY < UINT32_विस्तारः, "");
static_assert(XKB_STATE_बहुलकम्_SERVER >= 0 &&
              XKB_STATE_बहुलकम्_SERVER < UINT32_विस्तारः, "");
static_assert(XKB_CONSUMED_बहुलकम्_XKB >= 0 &&
              XKB_CONSUMED_बहुलकम्_XKB < UINT32_विस्तारः, "");
static_assert(XKB_CONSUMED_बहुलकम्_GTK >= 0 &&
              XKB_CONSUMED_बहुलकम्_GTK < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_FORMAT_TEXT_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_TEXT_V1 < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_विस्तारः, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_विस्तारः, "");

गणना xkb_enumerations_मानानि {
    XKB_RMLVO_BUILDER_FLAGS_मानानि
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_मानानि
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_मानानि
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_नामानि
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_मानानि
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_बहुलकम्
    ,
    XKB_KEYMAP_FORMAT_मानानि
        = (1u << XKB_KEYMAP_FORMAT_TEXT_V1)
        | (1u << XKB_KEYMAP_FORMAT_TEXT_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_मानानि
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_अप्रयुक्तम्
        | XKB_KEYMAP_SERIALIZE_STRICT_बहुलकम्
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानानि
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि
    ,
    XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_मानानि
        = XKB_KEYMAP_कुञ्चिका_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_कुञ्चिका_ITERATOR_DESCENDING_क्रमः
        | XKB_KEYMAP_कुञ्चिका_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_कुञ्चिका_DIRECTION_मानानि
        = (1u << XKB_कुञ्चिका_ऊर्ध्वम्)
        | (1u << XKB_कुञ्चिका_अधस्तात्)
        | (1u << XKB_कुञ्चिका_REPEATED)
    ,
    XKB_STATE_COMPONENT_मानानि
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
    XKB_KEYBOARD_CONTROL_FLAGS_मानानि
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः
        | XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः
    ,
    XKB_EVENT_TYPE_मानानि
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_FRAME)
        | (1u << XKB_EVENT_TYPE_कुञ्चिका)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_POINTER_MOTION)
        | (1u << XKB_EVENT_TYPE_POINTER_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_POINTER_MOTION_FLAGS_मानानि
        = XKB_POINTER_MOTION_NO_FLAGS
        | XKB_POINTER_MOTION_REPEATS
        | XKB_POINTER_MOTION_ABSOLUTE_X
        | XKB_POINTER_MOTION_ABSOLUTE_Y
    ,
    XKB_POINTER_BUTTON_STATE_मानानि
        = (1u << XKB_POINTER_BUTTON_RELEASED)
        | (1u << XKB_POINTER_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_मानानि
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_मानानि
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_मानानि
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_मानानि
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_कुञ्चिकाः_NO_युगपत्_कुञ्चिकाः
        | XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_गन्तव्यम्_तालकम्
        | XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः
    ,
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_मानानि
        = (1u << XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP)
        | (1u << XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT)
    ,
    XKB_STATE_बहुलकम्_मानानि
        = (1u << XKB_STATE_बहुलकम्_CLIENT)
        | (1u << XKB_STATE_बहुलकम्_SERVER_QUERY)
        | (1u << XKB_STATE_बहुलकम्_SERVER)
    ,
    XKB_STATE_मेलनम्_मानानि
        = XKB_STATE_मेलनम्_ANY
        | XKB_STATE_मेलनम्_ALL
        | XKB_STATE_मेलनम्_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_बहुलकम्_मानानि
        = (1u << XKB_CONSUMED_बहुलकम्_XKB)
        | (1u << XKB_CONSUMED_बहुलकम्_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_मानानि
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_मानानि
        = (1u << XKB_COMPOSE_FORMAT_TEXT_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_मानानि
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_स्थितिः_मानानि
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_परिणाम_मानानि
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_निम्नतमम्_मानम् = XKB_ERROR_INVALID,
    ENUM_निम्नतमम्_FLAG_मानम् = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_मानम् = XKB_STATE_मेलनम्_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_मानम् = XKB_STATE_मेलनम्_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_rmlvo_builder_flags_मानानि[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#यदि_अन्तः

गणना xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keysym_flags_मानानि[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#यदि_अन्तः

गणना xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_context_flags_मानानि[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_नामानि,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#यदि_अन्तः

गणना xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
स्थिर ध्रुव uint32_t xkb_log_level_मानानि[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

गणना xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_अङ्कः = 5,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keymap_compile_flags_मानानि[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_बहुलकम्,
};
#यदि_अन्तः

गणना xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_बहुलकम्,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keymap_format_मानानि[] = {
    XKB_KEYMAP_FORMAT_TEXT_V1,
    XKB_KEYMAP_FORMAT_TEXT_V2,
};
#यदि_अन्तः

गणना xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_TEXT_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_TEXT_V2,
    XKB_KEYMAP_FORMAT_अङ्कः = 2,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keymap_serialize_flags_मानानि[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_अप्रयुक्तम्,
    XKB_KEYMAP_SERIALIZE_STRICT_बहुलकम्,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानानि,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि,
};
#यदि_अन्तः

गणना xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keymap_कुञ्चिका_iterator_flags_मानानि[] = {
    XKB_KEYMAP_कुञ्चिका_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_कुञ्चिका_ITERATOR_DESCENDING_क्रमः,
    XKB_KEYMAP_कुञ्चिका_ITERATOR_INCLUDE_UNBOUND,
};
#यदि_अन्तः

गणना xkb_keymap_कुञ्चिका_iterator_flags_bounds {
    XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_MIN = XKB_KEYMAP_कुञ्चिका_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_MAX = XKB_KEYMAP_कुञ्चिका_ITERATOR_INCLUDE_UNBOUND,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_कुञ्चिका_direction_मानानि[] = {
    XKB_कुञ्चिका_ऊर्ध्वम्,
    XKB_कुञ्चिका_अधस्तात्,
    XKB_कुञ्चिका_REPEATED,
};
#यदि_अन्तः

गणना xkb_कुञ्चिका_direction_bounds {
    XKB_कुञ्चिका_DIRECTION_MIN = XKB_कुञ्चिका_ऊर्ध्वम्,
    XKB_कुञ्चिका_DIRECTION_MAX = XKB_कुञ्चिका_REPEATED,
    XKB_कुञ्चिका_DIRECTION_अङ्कः = 3,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_state_component_मानानि[] = {
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
#यदि_अन्तः

गणना xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_keyboard_control_flags_मानानि[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
    XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः,
};
#यदि_अन्तः

गणना xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_event_type_मानानि[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_FRAME,
    XKB_EVENT_TYPE_कुञ्चिका,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_POINTER_MOTION,
    XKB_EVENT_TYPE_POINTER_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#यदि_अन्तः

गणना xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_अङ्कः = 8,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_pointer_motion_flags_मानानि[] = {
    XKB_POINTER_MOTION_NO_FLAGS,
    XKB_POINTER_MOTION_REPEATS,
    XKB_POINTER_MOTION_ABSOLUTE_X,
    XKB_POINTER_MOTION_ABSOLUTE_Y,
};
#यदि_अन्तः

गणना xkb_pointer_motion_flags_bounds {
    _XKB_POINTER_MOTION_FLAGS_MIN = XKB_POINTER_MOTION_NO_FLAGS,
    _XKB_POINTER_MOTION_FLAGS_MAX = XKB_POINTER_MOTION_ABSOLUTE_Y,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_pointer_button_state_मानानि[] = {
    XKB_POINTER_BUTTON_RELEASED,
    XKB_POINTER_BUTTON_PRESSED,
};
#यदि_अन्तः

गणना xkb_pointer_button_state_bounds {
    _XKB_POINTER_BUTTON_STATE_MIN = XKB_POINTER_BUTTON_RELEASED,
    _XKB_POINTER_BUTTON_STATE_MAX = XKB_POINTER_BUTTON_PRESSED,
    XKB_POINTER_BUTTON_STATE_अङ्कः = 2,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_events_flags_मानानि[] = {
    XKB_EVENTS_NO_FLAGS,
};
#यदि_अन्तः

गणना xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_machine_builder_flags_मानानि[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#यदि_अन्तः

गणना xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_machine_flags_मानानि[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#यदि_अन्तः

गणना xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_a11y_flags_मानानि[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_कुञ्चिकाः_NO_युगपत्_कुञ्चिकाः,
    XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_गन्तव्यम्_तालकम्,
    XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः,
};
#यदि_अन्तः

गणना xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy_मानानि[] = {
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP,
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP,
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT,
};
#यदि_अन्तः

गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy_bounds {
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_MIN = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP,
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_MAX = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT,
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_अङ्कः = 3,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_state_बहुलकम्_मानानि[] = {
    XKB_STATE_बहुलकम्_CLIENT,
    XKB_STATE_बहुलकम्_SERVER_QUERY,
    XKB_STATE_बहुलकम्_SERVER,
};
#यदि_अन्तः

गणना xkb_state_बहुलकम्_bounds {
    XKB_STATE_बहुलकम्_MIN = XKB_STATE_बहुलकम्_CLIENT,
    XKB_STATE_बहुलकम्_MAX = XKB_STATE_बहुलकम्_SERVER,
    XKB_STATE_बहुलकम्_अङ्कः = 3,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_state_मेलनम्_मानानि[] = {
    XKB_STATE_मेलनम्_ANY,
    XKB_STATE_मेलनम्_ALL,
    XKB_STATE_मेलनम्_NON_EXCLUSIVE,
};
#यदि_अन्तः

गणना xkb_state_मेलनम्_bounds {
    XKB_STATE_मेलनम्_MIN = 0 /* implicit minimum */,
    XKB_STATE_मेलनम्_MAX = XKB_STATE_मेलनम्_NON_EXCLUSIVE,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_consumed_बहुलकम्_मानानि[] = {
    XKB_CONSUMED_बहुलकम्_XKB,
    XKB_CONSUMED_बहुलकम्_GTK,
};
#यदि_अन्तः

गणना xkb_consumed_बहुलकम्_bounds {
    XKB_CONSUMED_बहुलकम्_MIN = XKB_CONSUMED_बहुलकम्_XKB,
    XKB_CONSUMED_बहुलकम्_MAX = XKB_CONSUMED_बहुलकम्_GTK,
    XKB_CONSUMED_बहुलकम्_अङ्कः = 2,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_compose_compile_flags_मानानि[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#यदि_अन्तः

गणना xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_compose_format_मानानि[] = {
    XKB_COMPOSE_FORMAT_TEXT_V1,
};
#यदि_अन्तः

गणना xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_TEXT_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_TEXT_V1,
    XKB_COMPOSE_FORMAT_अङ्कः = 1,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_compose_state_flags_मानानि[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#यदि_अन्तः

गणना xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_compose_स्थितिः_मानानि[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#यदि_अन्तः

गणना xkb_compose_स्थितिः_bounds {
    XKB_COMPOSE_स्थितिः_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_स्थितिः_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_स्थितिः_अङ्कः = 4,
};

#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
स्थिर ध्रुव uint32_t xkb_compose_feed_परिणाम_मानानि[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#यदि_अन्तः

गणना xkb_compose_feed_परिणाम_bounds {
    XKB_COMPOSE_FEED_परिणाम_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_परिणाम_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_परिणाम_अङ्कः = 2,
};
स्थिर ध्रुव uint32_t xkb_स्थितिः_मानानि[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
    XKB_ERROR_UNSUPPORTED_STATE_बहुलकम्,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_कुञ्चिका_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_गणनम्_सीमा_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_बहुलकम्,
    XKB_ERROR_ABI_INVALID_STRUCT_आकार,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_आवण्टनम्_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

गणना xkb_स्थितिः_bounds {
    XKB_स्थितिः_MIN = XKB_ERROR_INVALID,
    XKB_स्थितिः_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_स्थितिः_अङ्कः = 21,
};
स्थिर ध्रुव uint32_t xkb_feature_मानानि[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_स्थितिः,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_कुञ्चिका_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_बहुलकम्,
    XKB_FEATURE_ENUM_STATE_मेलनम्,
    XKB_FEATURE_ENUM_CONSUMED_बहुलकम्,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_कुञ्चिका_DIRECTION,
    XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS,
    XKB_FEATURE_ENUM_POINTER_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_स्थितिः,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_परिणाम,
};

गणना xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_परिणाम,
    XKB_FEATURE_अङ्कः = 29,
};
