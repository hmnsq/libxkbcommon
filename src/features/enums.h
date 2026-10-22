/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#رهنمود once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-features.h"

#اگر_تعریف_نشده UINT32_عرض
#تعریف_کن UINT32_عرض (حجم(uint32_t) * CHAR_BIT)
#پایان_اگر

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_متن_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V1 < UINT32_عرض, "");
static_assert(XKB_KEYMAP_FORMAT_متن_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V2 < UINT32_عرض, "");
static_assert(XKB_کلید_به_بالا >= 0 &&
              XKB_کلید_به_بالا < UINT32_عرض, "");
static_assert(XKB_کلید_پایین >= 0 &&
              XKB_کلید_پایین < UINT32_عرض, "");
static_assert(XKB_کلید_REPEATED >= 0 &&
              XKB_کلید_REPEATED < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_قاب >= 0 &&
              XKB_EVENT_TYPE_قاب < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_کلید >= 0 &&
              XKB_EVENT_TYPE_کلید < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_اشاره_گر_MOTION >= 0 &&
              XKB_EVENT_TYPE_اشاره_گر_MOTION < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_اشاره_گر_BUTTON >= 0 &&
              XKB_EVENT_TYPE_اشاره_گر_BUTTON < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_عرض, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_عرض, "");
static_assert(XKB_اشاره_گر_BUTTON_RELEASED >= 0 &&
              XKB_اشاره_گر_BUTTON_RELEASED < UINT32_عرض, "");
static_assert(XKB_اشاره_گر_BUTTON_PRESSED >= 0 &&
              XKB_اشاره_گر_BUTTON_PRESSED < UINT32_عرض, "");
static_assert(XKB_LAYOUT_بیرون_از_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_بیرون_از_RANGE_WRAP < UINT32_عرض, "");
static_assert(XKB_LAYOUT_بیرون_از_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_بیرون_از_RANGE_CLAMP < UINT32_عرض, "");
static_assert(XKB_LAYOUT_بیرون_از_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_بیرون_از_RANGE_REDIRECT < UINT32_عرض, "");
static_assert(XKB_STATE_نما_CLIENT >= 0 &&
              XKB_STATE_نما_CLIENT < UINT32_عرض, "");
static_assert(XKB_STATE_نما_SERVER_QUERY >= 0 &&
              XKB_STATE_نما_SERVER_QUERY < UINT32_عرض, "");
static_assert(XKB_STATE_نما_SERVER >= 0 &&
              XKB_STATE_نما_SERVER < UINT32_عرض, "");
static_assert(XKB_CONSUMED_نما_XKB >= 0 &&
              XKB_CONSUMED_نما_XKB < UINT32_عرض, "");
static_assert(XKB_CONSUMED_نما_GTK >= 0 &&
              XKB_CONSUMED_نما_GTK < UINT32_عرض, "");
static_assert(XKB_COMPOSE_FORMAT_متن_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_متن_V1 < UINT32_عرض, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_عرض, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_عرض, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_عرض, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_عرض, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_عرض, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_عرض, "");

تعداد xkb_enumerations_مقادیر {
    XKB_RMLVO_BUILDER_FLAGS_مقادیر
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_مقادیر
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_مقادیر
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_نامها
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_مقادیر
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_نما
    ,
    XKB_KEYMAP_FORMAT_مقادیر
        = (1u << XKB_KEYMAP_FORMAT_متن_V1)
        | (1u << XKB_KEYMAP_FORMAT_متن_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_مقادیر
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_استفادهنشده
        | XKB_KEYMAP_SERIALIZE_STRICT_نما
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_مقادیر
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر
    ,
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر
        = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب
        | XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_کلید_DIRECTION_مقادیر
        = (1u << XKB_کلید_به_بالا)
        | (1u << XKB_کلید_پایین)
        | (1u << XKB_کلید_REPEATED)
    ,
    XKB_STATE_COMPONENT_مقادیر
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
    XKB_KEYBOARD_CONTROL_FLAGS_مقادیر
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها
        | XKB_KEYBOARD_CONTROL_MOUSE_کلیدها
    ,
    XKB_EVENT_TYPE_مقادیر
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_قاب)
        | (1u << XKB_EVENT_TYPE_کلید)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_اشاره_گر_MOTION)
        | (1u << XKB_EVENT_TYPE_اشاره_گر_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_اشاره_گر_MOTION_FLAGS_مقادیر
        = XKB_اشاره_گر_MOTION_NO_FLAGS
        | XKB_اشاره_گر_MOTION_REPEATS
        | XKB_اشاره_گر_MOTION_ABSOLUTE_X
        | XKB_اشاره_گر_MOTION_ABSOLUTE_Y
    ,
    XKB_اشاره_گر_BUTTON_STATE_مقادیر
        = (1u << XKB_اشاره_گر_BUTTON_RELEASED)
        | (1u << XKB_اشاره_گر_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_مقادیر
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_مقادیر
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_مقادیر
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_مقادیر
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_کلیدها_NO_همزمان_کلیدها
        | XKB_A11_Y_STICKY_کلیدها_LATCH_مقصد_قفل
        | XKB_A11_Y_LATCH_همزمان_کلیدها
    ,
    XKB_LAYOUT_بیرون_از_RANGE_POLICY_مقادیر
        = (1u << XKB_LAYOUT_بیرون_از_RANGE_WRAP)
        | (1u << XKB_LAYOUT_بیرون_از_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_بیرون_از_RANGE_REDIRECT)
    ,
    XKB_STATE_نما_مقادیر
        = (1u << XKB_STATE_نما_CLIENT)
        | (1u << XKB_STATE_نما_SERVER_QUERY)
        | (1u << XKB_STATE_نما_SERVER)
    ,
    XKB_STATE_تطابق_مقادیر
        = XKB_STATE_تطابق_ANY
        | XKB_STATE_تطابق_ALL
        | XKB_STATE_تطابق_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_نما_مقادیر
        = (1u << XKB_CONSUMED_نما_XKB)
        | (1u << XKB_CONSUMED_نما_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_مقادیر
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_مقادیر
        = (1u << XKB_COMPOSE_FORMAT_متن_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_مقادیر
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_وضعیت_مقادیر
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_نتیجه_مقادیر
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_پایینترین_مقدار = XKB_ERROR_INVALID,
    ENUM_پایینترین_FLAG_مقدار = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_مقدار = XKB_STATE_تطابق_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_مقدار = XKB_STATE_تطابق_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_rmlvo_builder_flags_مقادیر[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#پایان_اگر

تعداد xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keysym_flags_مقادیر[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#پایان_اگر

تعداد xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_context_flags_مقادیر[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_نامها,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#پایان_اگر

تعداد xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ساكن ثابت uint32_t xkb_log_level_مقادیر[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

تعداد xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_رقم = 5,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keymap_compile_flags_مقادیر[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_نما,
};
#پایان_اگر

تعداد xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_نما,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keymap_format_مقادیر[] = {
    XKB_KEYMAP_FORMAT_متن_V1,
    XKB_KEYMAP_FORMAT_متن_V2,
};
#پایان_اگر

تعداد xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_متن_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_متن_V2,
    XKB_KEYMAP_FORMAT_رقم = 2,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keymap_serialize_flags_مقادیر[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_استفادهنشده,
    XKB_KEYMAP_SERIALIZE_STRICT_نما,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_مقادیر,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر,
};
#پایان_اگر

تعداد xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keymap_کلید_iterator_flags_مقادیر[] = {
    XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب,
    XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND,
};
#پایان_اگر

تعداد xkb_keymap_کلید_iterator_flags_bounds {
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_MIN = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_MAX = XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_کلید_direction_مقادیر[] = {
    XKB_کلید_به_بالا,
    XKB_کلید_پایین,
    XKB_کلید_REPEATED,
};
#پایان_اگر

تعداد xkb_کلید_direction_bounds {
    XKB_کلید_DIRECTION_MIN = XKB_کلید_به_بالا,
    XKB_کلید_DIRECTION_MAX = XKB_کلید_REPEATED,
    XKB_کلید_DIRECTION_رقم = 3,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_state_component_مقادیر[] = {
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
#پایان_اگر

تعداد xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_keyboard_control_flags_مقادیر[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
    XKB_KEYBOARD_CONTROL_MOUSE_کلیدها,
};
#پایان_اگر

تعداد xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_کلیدها,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_event_type_مقادیر[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_قاب,
    XKB_EVENT_TYPE_کلید,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_اشاره_گر_MOTION,
    XKB_EVENT_TYPE_اشاره_گر_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#پایان_اگر

تعداد xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_رقم = 8,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_اشاره_گر_motion_flags_مقادیر[] = {
    XKB_اشاره_گر_MOTION_NO_FLAGS,
    XKB_اشاره_گر_MOTION_REPEATS,
    XKB_اشاره_گر_MOTION_ABSOLUTE_X,
    XKB_اشاره_گر_MOTION_ABSOLUTE_Y,
};
#پایان_اگر

تعداد xkb_اشاره_گر_motion_flags_bounds {
    XKB_اشاره_گر_MOTION_FLAGS_MIN = XKB_اشاره_گر_MOTION_NO_FLAGS,
    XKB_اشاره_گر_MOTION_FLAGS_MAX = XKB_اشاره_گر_MOTION_ABSOLUTE_Y,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_اشاره_گر_button_state_مقادیر[] = {
    XKB_اشاره_گر_BUTTON_RELEASED,
    XKB_اشاره_گر_BUTTON_PRESSED,
};
#پایان_اگر

تعداد xkb_اشاره_گر_button_state_bounds {
    XKB_اشاره_گر_BUTTON_STATE_MIN = XKB_اشاره_گر_BUTTON_RELEASED,
    XKB_اشاره_گر_BUTTON_STATE_MAX = XKB_اشاره_گر_BUTTON_PRESSED,
    XKB_اشاره_گر_BUTTON_STATE_رقم = 2,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_events_flags_مقادیر[] = {
    XKB_EVENTS_NO_FLAGS,
};
#پایان_اگر

تعداد xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_machine_builder_flags_مقادیر[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#پایان_اگر

تعداد xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_machine_flags_مقادیر[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#پایان_اگر

تعداد xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_a11y_flags_مقادیر[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_کلیدها_NO_همزمان_کلیدها,
    XKB_A11_Y_STICKY_کلیدها_LATCH_مقصد_قفل,
    XKB_A11_Y_LATCH_همزمان_کلیدها,
};
#پایان_اگر

تعداد xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_همزمان_کلیدها,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_layout_بیرون_از_range_policy_مقادیر[] = {
    XKB_LAYOUT_بیرون_از_RANGE_WRAP,
    XKB_LAYOUT_بیرون_از_RANGE_CLAMP,
    XKB_LAYOUT_بیرون_از_RANGE_REDIRECT,
};
#پایان_اگر

تعداد xkb_layout_بیرون_از_range_policy_bounds {
    XKB_LAYOUT_بیرون_از_RANGE_POLICY_MIN = XKB_LAYOUT_بیرون_از_RANGE_WRAP,
    XKB_LAYOUT_بیرون_از_RANGE_POLICY_MAX = XKB_LAYOUT_بیرون_از_RANGE_REDIRECT,
    XKB_LAYOUT_بیرون_از_RANGE_POLICY_رقم = 3,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_state_نما_مقادیر[] = {
    XKB_STATE_نما_CLIENT,
    XKB_STATE_نما_SERVER_QUERY,
    XKB_STATE_نما_SERVER,
};
#پایان_اگر

تعداد xkb_state_نما_bounds {
    XKB_STATE_نما_MIN = XKB_STATE_نما_CLIENT,
    XKB_STATE_نما_MAX = XKB_STATE_نما_SERVER,
    XKB_STATE_نما_رقم = 3,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_state_تطابق_مقادیر[] = {
    XKB_STATE_تطابق_ANY,
    XKB_STATE_تطابق_ALL,
    XKB_STATE_تطابق_NON_EXCLUSIVE,
};
#پایان_اگر

تعداد xkb_state_تطابق_bounds {
    XKB_STATE_تطابق_MIN = 0 /* implicit minimum */,
    XKB_STATE_تطابق_MAX = XKB_STATE_تطابق_NON_EXCLUSIVE,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_consumed_نما_مقادیر[] = {
    XKB_CONSUMED_نما_XKB,
    XKB_CONSUMED_نما_GTK,
};
#پایان_اگر

تعداد xkb_consumed_نما_bounds {
    XKB_CONSUMED_نما_MIN = XKB_CONSUMED_نما_XKB,
    XKB_CONSUMED_نما_MAX = XKB_CONSUMED_نما_GTK,
    XKB_CONSUMED_نما_رقم = 2,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_compose_compile_flags_مقادیر[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#پایان_اگر

تعداد xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_compose_format_مقادیر[] = {
    XKB_COMPOSE_FORMAT_متن_V1,
};
#پایان_اگر

تعداد xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_متن_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_متن_V1,
    XKB_COMPOSE_FORMAT_رقم = 1,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_compose_state_flags_مقادیر[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#پایان_اگر

تعداد xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_compose_وضعیت_مقادیر[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#پایان_اگر

تعداد xkb_compose_وضعیت_bounds {
    XKB_COMPOSE_وضعیت_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_وضعیت_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_وضعیت_رقم = 4,
};

#اگر_تعریف_شده ENABLE_خصوصی_APIS
ساكن ثابت uint32_t xkb_compose_feed_نتیجه_مقادیر[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#پایان_اگر

تعداد xkb_compose_feed_نتیجه_bounds {
    XKB_COMPOSE_FEED_نتیجه_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_نتیجه_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_نتیجه_رقم = 2,
};
ساكن ثابت uint32_t xkb_وضعیت_مقادیر[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
    XKB_ERROR_UNSUPPORTED_STATE_نما,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_بیرون_از_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_شمار_حد_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_نما,
    XKB_ERROR_ABI_INVALID_STRUCT_اندازه,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_تخصیص_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

تعداد xkb_وضعیت_bounds {
    XKB_وضعیت_MIN = XKB_ERROR_INVALID,
    XKB_وضعیت_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_وضعیت_رقم = 21,
};
ساكن ثابت uint32_t xkb_feature_مقادیر[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_وضعیت,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_بیرون_از_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_نما,
    XKB_FEATURE_ENUM_STATE_تطابق,
    XKB_FEATURE_ENUM_CONSUMED_نما,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_کلید_DIRECTION,
    XKB_FEATURE_ENUM_اشاره_گر_MOTION_FLAGS,
    XKB_FEATURE_ENUM_اشاره_گر_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_وضعیت,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجه,
};

تعداد xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجه,
    XKB_FEATURE_رقم = 29,
};
