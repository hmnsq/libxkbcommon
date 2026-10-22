/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ہدایت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-features.h"

#جے_بنا_تعریف UINT32_چوڑائی
#تعریف_کرو UINT32_چوڑائی (حجم(uint32_t) * CHAR_BIT)
#جے_انت

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_متن_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V1 < UINT32_چوڑائی, "");
static_assert(XKB_KEYMAP_FORMAT_متن_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V2 < UINT32_چوڑائی, "");
static_assert(XKB_کلید_اتے >= 0 &&
              XKB_کلید_اتے < UINT32_چوڑائی, "");
static_assert(XKB_کلید_تھلے >= 0 &&
              XKB_کلید_تھلے < UINT32_چوڑائی, "");
static_assert(XKB_کلید_REPEATED >= 0 &&
              XKB_کلید_REPEATED < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_چوکھٹا >= 0 &&
              XKB_EVENT_TYPE_چوکھٹا < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_کلید >= 0 &&
              XKB_EVENT_TYPE_کلید < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_اشارہ_گر_MOTION >= 0 &&
              XKB_EVENT_TYPE_اشارہ_گر_MOTION < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_اشارہ_گر_BUTTON >= 0 &&
              XKB_EVENT_TYPE_اشارہ_گر_BUTTON < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_چوڑائی, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_چوڑائی, "");
static_assert(XKB_اشارہ_گر_BUTTON_RELEASED >= 0 &&
              XKB_اشارہ_گر_BUTTON_RELEASED < UINT32_چوڑائی, "");
static_assert(XKB_اشارہ_گر_BUTTON_PRESSED >= 0 &&
              XKB_اشارہ_گر_BUTTON_PRESSED < UINT32_چوڑائی, "");
static_assert(XKB_LAYOUT_باہرلا_دا_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_باہرلا_دا_RANGE_WRAP < UINT32_چوڑائی, "");
static_assert(XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP < UINT32_چوڑائی, "");
static_assert(XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT < UINT32_چوڑائی, "");
static_assert(XKB_STATE_منوال_CLIENT >= 0 &&
              XKB_STATE_منوال_CLIENT < UINT32_چوڑائی, "");
static_assert(XKB_STATE_منوال_SERVER_QUERY >= 0 &&
              XKB_STATE_منوال_SERVER_QUERY < UINT32_چوڑائی, "");
static_assert(XKB_STATE_منوال_SERVER >= 0 &&
              XKB_STATE_منوال_SERVER < UINT32_چوڑائی, "");
static_assert(XKB_CONSUMED_منوال_XKB >= 0 &&
              XKB_CONSUMED_منوال_XKB < UINT32_چوڑائی, "");
static_assert(XKB_CONSUMED_منوال_GTK >= 0 &&
              XKB_CONSUMED_منوال_GTK < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_FORMAT_متن_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_متن_V1 < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_چوڑائی, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_چوڑائی, "");

تعداد xkb_enumerations_اقدار {
    XKB_RMLVO_BUILDER_FLAGS_اقدار
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_اقدار
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_اقدار
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_ناں
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_اقدار
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_منوال
    ,
    XKB_KEYMAP_FORMAT_اقدار
        = (1u << XKB_KEYMAP_FORMAT_متن_V1)
        | (1u << XKB_KEYMAP_FORMAT_متن_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_اقدار
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_بن_ورتیا
        | XKB_KEYMAP_SERIALIZE_STRICT_منوال
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار
    ,
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار
        = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب
        | XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_کلید_DIRECTION_اقدار
        = (1u << XKB_کلید_اتے)
        | (1u << XKB_کلید_تھلے)
        | (1u << XKB_کلید_REPEATED)
    ,
    XKB_STATE_COMPONENT_اقدار
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
    XKB_KEYBOARD_CONTROL_FLAGS_اقدار
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں
        | XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں
    ,
    XKB_EVENT_TYPE_اقدار
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_چوکھٹا)
        | (1u << XKB_EVENT_TYPE_کلید)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_اشارہ_گر_MOTION)
        | (1u << XKB_EVENT_TYPE_اشارہ_گر_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_اشارہ_گر_MOTION_FLAGS_اقدار
        = XKB_اشارہ_گر_MOTION_NO_FLAGS
        | XKB_اشارہ_گر_MOTION_REPEATS
        | XKB_اشارہ_گر_MOTION_ABSOLUTE_X
        | XKB_اشارہ_گر_MOTION_ABSOLUTE_Y
    ,
    XKB_اشارہ_گر_BUTTON_STATE_اقدار
        = (1u << XKB_اشارہ_گر_BUTTON_RELEASED)
        | (1u << XKB_اشارہ_گر_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_اقدار
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_اقدار
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_اقدار
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_اقدار
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_کلیدیں_NO_اکو_ویلے_کلیدیں
        | XKB_A11_Y_STICKY_کلیدیں_LATCH_منزل_جندرا
        | XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں
    ,
    XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_اقدار
        = (1u << XKB_LAYOUT_باہرلا_دا_RANGE_WRAP)
        | (1u << XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT)
    ,
    XKB_STATE_منوال_اقدار
        = (1u << XKB_STATE_منوال_CLIENT)
        | (1u << XKB_STATE_منوال_SERVER_QUERY)
        | (1u << XKB_STATE_منوال_SERVER)
    ,
    XKB_STATE_میل_اقدار
        = XKB_STATE_میل_ANY
        | XKB_STATE_میل_ALL
        | XKB_STATE_میل_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_منوال_اقدار
        = (1u << XKB_CONSUMED_منوال_XKB)
        | (1u << XKB_CONSUMED_منوال_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_اقدار
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_اقدار
        = (1u << XKB_COMPOSE_FORMAT_متن_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_اقدار
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_حالت_اقدار
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_نتیجہ_اقدار
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_سبھ_توں_نیواں_قدر = XKB_ERROR_INVALID,
    ENUM_سبھ_توں_نیواں_FLAG_قدر = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_قدر = XKB_STATE_میل_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_قدر = XKB_STATE_میل_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_rmlvo_builder_flags_اقدار[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#جے_انت

تعداد xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keysym_flags_اقدار[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#جے_انت

تعداد xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_context_flags_اقدار[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_ناں,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#جے_انت

تعداد xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ساكن ثابت uint32_t xkb_log_level_اقدار[] = {
    XKB_LOG_LEVEL_CRITICAL,
    XKB_LOG_LEVEL_ERROR,
    XKB_LOG_LEVEL_WARNING,
    XKB_LOG_LEVEL_INFO,
    XKB_LOG_LEVEL_DEBUG,
};

تعداد xkb_log_level_bounds {
    _XKB_LOG_LEVEL_MIN = XKB_LOG_LEVEL_CRITICAL,
    _XKB_LOG_LEVEL_MAX = XKB_LOG_LEVEL_DEBUG,
    XKB_LOG_LEVEL_عدد = 5,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keymap_compile_flags_اقدار[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_منوال,
};
#جے_انت

تعداد xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_منوال,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keymap_format_اقدار[] = {
    XKB_KEYMAP_FORMAT_متن_V1,
    XKB_KEYMAP_FORMAT_متن_V2,
};
#جے_انت

تعداد xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_متن_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_متن_V2,
    XKB_KEYMAP_FORMAT_عدد = 2,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keymap_serialize_flags_اقدار[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_بن_ورتیا,
    XKB_KEYMAP_SERIALIZE_STRICT_منوال,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار,
};
#جے_انت

تعداد xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keymap_کلید_iterator_flags_اقدار[] = {
    XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب,
    XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND,
};
#جے_انت

تعداد xkb_keymap_کلید_iterator_flags_bounds {
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_MIN = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلید_ITERATOR_FLAGS_MAX = XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_کلید_direction_اقدار[] = {
    XKB_کلید_اتے,
    XKB_کلید_تھلے,
    XKB_کلید_REPEATED,
};
#جے_انت

تعداد xkb_کلید_direction_bounds {
    XKB_کلید_DIRECTION_MIN = XKB_کلید_اتے,
    XKB_کلید_DIRECTION_MAX = XKB_کلید_REPEATED,
    XKB_کلید_DIRECTION_عدد = 3,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_state_component_اقدار[] = {
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
#جے_انت

تعداد xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_keyboard_control_flags_اقدار[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
    XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں,
};
#جے_انت

تعداد xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_event_type_اقدار[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_چوکھٹا,
    XKB_EVENT_TYPE_کلید,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_اشارہ_گر_MOTION,
    XKB_EVENT_TYPE_اشارہ_گر_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#جے_انت

تعداد xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_عدد = 8,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_اشارہ_گر_motion_flags_اقدار[] = {
    XKB_اشارہ_گر_MOTION_NO_FLAGS,
    XKB_اشارہ_گر_MOTION_REPEATS,
    XKB_اشارہ_گر_MOTION_ABSOLUTE_X,
    XKB_اشارہ_گر_MOTION_ABSOLUTE_Y,
};
#جے_انت

تعداد xkb_اشارہ_گر_motion_flags_bounds {
    XKB_اشارہ_گر_MOTION_FLAGS_MIN = XKB_اشارہ_گر_MOTION_NO_FLAGS,
    XKB_اشارہ_گر_MOTION_FLAGS_MAX = XKB_اشارہ_گر_MOTION_ABSOLUTE_Y,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_اشارہ_گر_button_state_اقدار[] = {
    XKB_اشارہ_گر_BUTTON_RELEASED,
    XKB_اشارہ_گر_BUTTON_PRESSED,
};
#جے_انت

تعداد xkb_اشارہ_گر_button_state_bounds {
    XKB_اشارہ_گر_BUTTON_STATE_MIN = XKB_اشارہ_گر_BUTTON_RELEASED,
    XKB_اشارہ_گر_BUTTON_STATE_MAX = XKB_اشارہ_گر_BUTTON_PRESSED,
    XKB_اشارہ_گر_BUTTON_STATE_عدد = 2,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_events_flags_اقدار[] = {
    XKB_EVENTS_NO_FLAGS,
};
#جے_انت

تعداد xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_machine_builder_flags_اقدار[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#جے_انت

تعداد xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_machine_flags_اقدار[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#جے_انت

تعداد xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_a11y_flags_اقدار[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_کلیدیں_NO_اکو_ویلے_کلیدیں,
    XKB_A11_Y_STICKY_کلیدیں_LATCH_منزل_جندرا,
    XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں,
};
#جے_انت

تعداد xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_layout_باہرلا_دا_range_policy_اقدار[] = {
    XKB_LAYOUT_باہرلا_دا_RANGE_WRAP,
    XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP,
    XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT,
};
#جے_انت

تعداد xkb_layout_باہرلا_دا_range_policy_bounds {
    XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_MIN = XKB_LAYOUT_باہرلا_دا_RANGE_WRAP,
    XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_MAX = XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT,
    XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_عدد = 3,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_state_منوال_اقدار[] = {
    XKB_STATE_منوال_CLIENT,
    XKB_STATE_منوال_SERVER_QUERY,
    XKB_STATE_منوال_SERVER,
};
#جے_انت

تعداد xkb_state_منوال_bounds {
    XKB_STATE_منوال_MIN = XKB_STATE_منوال_CLIENT,
    XKB_STATE_منوال_MAX = XKB_STATE_منوال_SERVER,
    XKB_STATE_منوال_عدد = 3,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_state_میل_اقدار[] = {
    XKB_STATE_میل_ANY,
    XKB_STATE_میل_ALL,
    XKB_STATE_میل_NON_EXCLUSIVE,
};
#جے_انت

تعداد xkb_state_میل_bounds {
    XKB_STATE_میل_MIN = 0 /* implicit minimum */,
    XKB_STATE_میل_MAX = XKB_STATE_میل_NON_EXCLUSIVE,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_consumed_منوال_اقدار[] = {
    XKB_CONSUMED_منوال_XKB,
    XKB_CONSUMED_منوال_GTK,
};
#جے_انت

تعداد xkb_consumed_منوال_bounds {
    XKB_CONSUMED_منوال_MIN = XKB_CONSUMED_منوال_XKB,
    XKB_CONSUMED_منوال_MAX = XKB_CONSUMED_منوال_GTK,
    XKB_CONSUMED_منوال_عدد = 2,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_compose_compile_flags_اقدار[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#جے_انت

تعداد xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_compose_format_اقدار[] = {
    XKB_COMPOSE_FORMAT_متن_V1,
};
#جے_انت

تعداد xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_متن_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_متن_V1,
    XKB_COMPOSE_FORMAT_عدد = 1,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_compose_state_flags_اقدار[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#جے_انت

تعداد xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_compose_حالت_اقدار[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#جے_انت

تعداد xkb_compose_حالت_bounds {
    XKB_COMPOSE_حالت_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_حالت_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_حالت_عدد = 4,
};

#جے_تعریف_شدہ ENABLE_نجی_APIS
ساكن ثابت uint32_t xkb_compose_feed_نتیجہ_اقدار[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#جے_انت

تعداد xkb_compose_feed_نتیجہ_bounds {
    XKB_COMPOSE_FEED_نتیجہ_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_نتیجہ_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_نتیجہ_عدد = 2,
};
ساكن ثابت uint32_t xkb_حالت_اقدار[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
    XKB_ERROR_UNSUPPORTED_STATE_منوال,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_باہرلا_دا_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_گنتی_حد_بندی_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_منوال,
    XKB_ERROR_ABI_INVALID_STRUCT_ناپ,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_ونڈ_FAILURE,
    XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS,
    XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS,
    XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS,
    XKB_ERROR_ABI_FORWARD_COMPAT,
    XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS,
    XKB_ERROR_ABI_BACKWARD_COMPAT,
};

تعداد xkb_حالت_bounds {
    XKB_حالت_MIN = XKB_ERROR_INVALID,
    XKB_حالت_MAX = XKB_ERROR_ABI_BACKWARD_COMPAT,
    XKB_حالت_عدد = 21,
};
ساكن ثابت uint32_t xkb_feature_اقدار[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_حالت,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_باہرلا_دا_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_منوال,
    XKB_FEATURE_ENUM_STATE_میل,
    XKB_FEATURE_ENUM_CONSUMED_منوال,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_کلید_DIRECTION,
    XKB_FEATURE_ENUM_اشارہ_گر_MOTION_FLAGS,
    XKB_FEATURE_ENUM_اشارہ_گر_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_حالت,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجہ,
};

تعداد xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجہ,
    XKB_FEATURE_عدد = 29,
};
