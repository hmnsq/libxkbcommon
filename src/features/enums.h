/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#لارښوونه once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-features.h"

#که_نه_تعریف_شوی UINT32_پلنوالی
#تعریف_کړه UINT32_پلنوالی (حجم(uint32_t) * CHAR_BIT)
#د_که_پای

/*
 * Enumerations whose values can be ORed
 */

static_assert(XKB_KEYMAP_FORMAT_متن_V1 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V1 < UINT32_پلنوالی, "");
static_assert(XKB_KEYMAP_FORMAT_متن_V2 >= 0 &&
              XKB_KEYMAP_FORMAT_متن_V2 < UINT32_پلنوالی, "");
static_assert(XKB_کلی_پورته >= 0 &&
              XKB_کلی_پورته < UINT32_پلنوالی, "");
static_assert(XKB_کلی_ښکته >= 0 &&
              XKB_کلی_ښکته < UINT32_پلنوالی, "");
static_assert(XKB_کلی_REPEATED >= 0 &&
              XKB_کلی_REPEATED < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_INVALID >= 0 &&
              XKB_EVENT_TYPE_INVALID < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_چوکاټ >= 0 &&
              XKB_EVENT_TYPE_چوکاټ < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_کلی >= 0 &&
              XKB_EVENT_TYPE_کلی < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_STATE_COMPONENTS >= 0 &&
              XKB_EVENT_TYPE_STATE_COMPONENTS < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_اشاره_ګر_MOTION >= 0 &&
              XKB_EVENT_TYPE_اشاره_ګر_MOTION < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_اشاره_ګر_BUTTON >= 0 &&
              XKB_EVENT_TYPE_اشاره_ګر_BUTTON < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER >= 0 &&
              XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER < UINT32_پلنوالی, "");
static_assert(XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE >= 0 &&
              XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE < UINT32_پلنوالی, "");
static_assert(XKB_اشاره_ګر_BUTTON_RELEASED >= 0 &&
              XKB_اشاره_ګر_BUTTON_RELEASED < UINT32_پلنوالی, "");
static_assert(XKB_اشاره_ګر_BUTTON_PRESSED >= 0 &&
              XKB_اشاره_ګر_BUTTON_PRESSED < UINT32_پلنوالی, "");
static_assert(XKB_LAYOUT_بهر_د_RANGE_WRAP >= 0 &&
              XKB_LAYOUT_بهر_د_RANGE_WRAP < UINT32_پلنوالی, "");
static_assert(XKB_LAYOUT_بهر_د_RANGE_CLAMP >= 0 &&
              XKB_LAYOUT_بهر_د_RANGE_CLAMP < UINT32_پلنوالی, "");
static_assert(XKB_LAYOUT_بهر_د_RANGE_REDIRECT >= 0 &&
              XKB_LAYOUT_بهر_د_RANGE_REDIRECT < UINT32_پلنوالی, "");
static_assert(XKB_STATE_ډېرپېښ_CLIENT >= 0 &&
              XKB_STATE_ډېرپېښ_CLIENT < UINT32_پلنوالی, "");
static_assert(XKB_STATE_ډېرپېښ_SERVER_QUERY >= 0 &&
              XKB_STATE_ډېرپېښ_SERVER_QUERY < UINT32_پلنوالی, "");
static_assert(XKB_STATE_ډېرپېښ_SERVER >= 0 &&
              XKB_STATE_ډېرپېښ_SERVER < UINT32_پلنوالی, "");
static_assert(XKB_CONSUMED_ډېرپېښ_XKB >= 0 &&
              XKB_CONSUMED_ډېرپېښ_XKB < UINT32_پلنوالی, "");
static_assert(XKB_CONSUMED_ډېرپېښ_GTK >= 0 &&
              XKB_CONSUMED_ډېرپېښ_GTK < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_FORMAT_متن_V1 >= 0 &&
              XKB_COMPOSE_FORMAT_متن_V1 < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_NOTHING >= 0 &&
              XKB_COMPOSE_NOTHING < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_COMPOSING >= 0 &&
              XKB_COMPOSE_COMPOSING < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_COMPOSED >= 0 &&
              XKB_COMPOSE_COMPOSED < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_CANCELLED >= 0 &&
              XKB_COMPOSE_CANCELLED < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_FEED_IGNORED >= 0 &&
              XKB_COMPOSE_FEED_IGNORED < UINT32_پلنوالی, "");
static_assert(XKB_COMPOSE_FEED_ACCEPTED >= 0 &&
              XKB_COMPOSE_FEED_ACCEPTED < UINT32_پلنوالی, "");

تعداد xkb_enumerations_ارزښتونه {
    XKB_RMLVO_BUILDER_FLAGS_ارزښتونه
        = XKB_RMLVO_BUILDER_NO_FLAGS
    ,
    XKB_KEYSYM_FLAGS_ارزښتونه
        = XKB_KEYSYM_NO_FLAGS
        | XKB_KEYSYM_CASE_INSENSITIVE
    ,
    XKB_CONTEXT_FLAGS_ارزښتونه
        = XKB_CONTEXT_NO_FLAGS
        | XKB_CONTEXT_NO_DEFAULT_INCLUDES
        | XKB_CONTEXT_NO_ENVIRONMENT_نومونه
        | XKB_CONTEXT_NO_SECURE_GETENV
    ,
    XKB_KEYMAP_COMPILE_FLAGS_ارزښتونه
        = XKB_KEYMAP_COMPILE_NO_FLAGS
        | XKB_KEYMAP_COMPILE_STRICT_ډېرپېښ
    ,
    XKB_KEYMAP_FORMAT_ارزښتونه
        = (1u << XKB_KEYMAP_FORMAT_متن_V1)
        | (1u << XKB_KEYMAP_FORMAT_متن_V2)
    ,
    XKB_KEYMAP_SERIALIZE_FLAGS_ارزښتونه
        = XKB_KEYMAP_SERIALIZE_NO_FLAGS
        | XKB_KEYMAP_SERIALIZE_PRETTY
        | XKB_KEYMAP_SERIALIZE_KEEP_ناکارول_شوی
        | XKB_KEYMAP_SERIALIZE_STRICT_ډېرپېښ
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ارزښتونه
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS
        | XKB_KEYMAP_SERIALIZE_EXPLICIT_کلی_ارزښتونه
    ,
    XKB_KEYMAP_کلی_ITERATOR_FLAGS_ارزښتونه
        = XKB_KEYMAP_کلی_ITERATOR_NO_FLAGS
        | XKB_KEYMAP_کلی_ITERATOR_DESCENDING_ترتیب
        | XKB_KEYMAP_کلی_ITERATOR_INCLUDE_UNBOUND
    ,
    XKB_کلی_DIRECTION_ارزښتونه
        = (1u << XKB_کلی_پورته)
        | (1u << XKB_کلی_ښکته)
        | (1u << XKB_کلی_REPEATED)
    ,
    XKB_STATE_COMPONENT_ارزښتونه
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
    XKB_KEYBOARD_CONTROL_FLAGS_ارزښتونه
        = XKB_KEYBOARD_CONTROL_NO_FLAGS
        | XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې
        | XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې
    ,
    XKB_EVENT_TYPE_ارزښتونه
        = (1u << XKB_EVENT_TYPE_INVALID)
        | (1u << XKB_EVENT_TYPE_چوکاټ)
        | (1u << XKB_EVENT_TYPE_کلی)
        | (1u << XKB_EVENT_TYPE_STATE_COMPONENTS)
        | (1u << XKB_EVENT_TYPE_اشاره_ګر_MOTION)
        | (1u << XKB_EVENT_TYPE_اشاره_ګر_BUTTON)
        | (1u << XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER)
        | (1u << XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
    ,
    XKB_اشاره_ګر_MOTION_FLAGS_ارزښتونه
        = XKB_اشاره_ګر_MOTION_NO_FLAGS
        | XKB_اشاره_ګر_MOTION_REPEATS
        | XKB_اشاره_ګر_MOTION_ABSOLUTE_X
        | XKB_اشاره_ګر_MOTION_ABSOLUTE_Y
    ,
    XKB_اشاره_ګر_BUTTON_STATE_ارزښتونه
        = (1u << XKB_اشاره_ګر_BUTTON_RELEASED)
        | (1u << XKB_اشاره_ګر_BUTTON_PRESSED)
    ,
    XKB_EVENTS_FLAGS_ارزښتونه
        = XKB_EVENTS_NO_FLAGS
    ,
    XKB_MACHINE_BUILDER_FLAGS_ارزښتونه
        = XKB_MACHINE_BUILDER_NO_FLAGS
    ,
    XKB_MACHINE_FLAGS_ارزښتونه
        = XKB_MACHINE_NO_FLAGS
        | XKB_MACHINE_SERVER_ACTIONS
        | XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلی_FIX
        | XKB_MACHINE_UNLOCK_MODS_ON_PRESS
    ,
    XKB_A11_Y_FLAGS_ارزښتونه
        = XKB_A11Y_NO_FLAGS
        | XKB_A11_Y_STICKY_کلیګانې_NO_همزمانه_کلیګانې
        | XKB_A11_Y_STICKY_کلیګانې_LATCH_منزل_قلف
        | XKB_A11_Y_LATCH_همزمانه_کلیګانې
    ,
    XKB_LAYOUT_بهر_د_RANGE_POLICY_ارزښتونه
        = (1u << XKB_LAYOUT_بهر_د_RANGE_WRAP)
        | (1u << XKB_LAYOUT_بهر_د_RANGE_CLAMP)
        | (1u << XKB_LAYOUT_بهر_د_RANGE_REDIRECT)
    ,
    XKB_STATE_ډېرپېښ_ارزښتونه
        = (1u << XKB_STATE_ډېرپېښ_CLIENT)
        | (1u << XKB_STATE_ډېرپېښ_SERVER_QUERY)
        | (1u << XKB_STATE_ډېرپېښ_SERVER)
    ,
    XKB_STATE_سمون_ارزښتونه
        = XKB_STATE_سمون_ANY
        | XKB_STATE_سمون_ALL
        | XKB_STATE_سمون_NON_EXCLUSIVE
    ,
    XKB_CONSUMED_ډېرپېښ_ارزښتونه
        = (1u << XKB_CONSUMED_ډېرپېښ_XKB)
        | (1u << XKB_CONSUMED_ډېرپېښ_GTK)
    ,
    XKB_COMPOSE_COMPILE_FLAGS_ارزښتونه
        = XKB_COMPOSE_COMPILE_NO_FLAGS
    ,
    XKB_COMPOSE_FORMAT_ارزښتونه
        = (1u << XKB_COMPOSE_FORMAT_متن_V1)
    ,
    XKB_COMPOSE_STATE_FLAGS_ارزښتونه
        = XKB_COMPOSE_STATE_NO_FLAGS
    ,
    XKB_COMPOSE_حالت_ارزښتونه
        = (1u << XKB_COMPOSE_NOTHING)
        | (1u << XKB_COMPOSE_COMPOSING)
        | (1u << XKB_COMPOSE_COMPOSED)
        | (1u << XKB_COMPOSE_CANCELLED)
    ,
    XKB_COMPOSE_FEED_پایله_ارزښتونه
        = (1u << XKB_COMPOSE_FEED_IGNORED)
        | (1u << XKB_COMPOSE_FEED_ACCEPTED)
    ,
    ENUM_ټیټ_ترین_ارزښت = XKB_ERROR_INVALID,
    ENUM_ټیټ_ترین_FLAG_ارزښت = XKB_RMLVO_BUILDER_NO_FLAGS,
    ENUM_HIGHEST_ارزښت = XKB_STATE_سمون_NON_EXCLUSIVE,
    ENUM_HIGHEST_FLAG_ارزښت = XKB_STATE_سمون_NON_EXCLUSIVE,
};

/*
 * Explicit and implict values of enumerations
 */

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_rmlvo_builder_flags_ارزښتونه[] = {
    XKB_RMLVO_BUILDER_NO_FLAGS,
};
#د_که_پای

تعداد xkb_rmlvo_builder_flags_bounds {
    _XKB_RMLVO_BUILDER_FLAGS_MIN = XKB_RMLVO_BUILDER_NO_FLAGS,
    _XKB_RMLVO_BUILDER_FLAGS_MAX = XKB_RMLVO_BUILDER_NO_FLAGS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keysym_flags_ارزښتونه[] = {
    XKB_KEYSYM_NO_FLAGS,
    XKB_KEYSYM_CASE_INSENSITIVE,
};
#د_که_پای

تعداد xkb_keysym_flags_bounds {
    _XKB_KEYSYM_FLAGS_MIN = XKB_KEYSYM_NO_FLAGS,
    _XKB_KEYSYM_FLAGS_MAX = XKB_KEYSYM_CASE_INSENSITIVE,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_context_flags_ارزښتونه[] = {
    XKB_CONTEXT_NO_FLAGS,
    XKB_CONTEXT_NO_DEFAULT_INCLUDES,
    XKB_CONTEXT_NO_ENVIRONMENT_نومونه,
    XKB_CONTEXT_NO_SECURE_GETENV,
};
#د_که_پای

تعداد xkb_context_flags_bounds {
    _XKB_CONTEXT_FLAGS_MIN = XKB_CONTEXT_NO_FLAGS,
    _XKB_CONTEXT_FLAGS_MAX = XKB_CONTEXT_NO_SECURE_GETENV,
};
ساكن ثابت uint32_t xkb_log_level_ارزښتونه[] = {
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

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keymap_compile_flags_ارزښتونه[] = {
    XKB_KEYMAP_COMPILE_NO_FLAGS,
    XKB_KEYMAP_COMPILE_STRICT_ډېرپېښ,
};
#د_که_پای

تعداد xkb_keymap_compile_flags_bounds {
    _XKB_KEYMAP_COMPILE_FLAGS_MIN = XKB_KEYMAP_COMPILE_NO_FLAGS,
    _XKB_KEYMAP_COMPILE_FLAGS_MAX = XKB_KEYMAP_COMPILE_STRICT_ډېرپېښ,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keymap_format_ارزښتونه[] = {
    XKB_KEYMAP_FORMAT_متن_V1,
    XKB_KEYMAP_FORMAT_متن_V2,
};
#د_که_پای

تعداد xkb_keymap_format_bounds {
    _XKB_KEYMAP_FORMAT_MIN = XKB_KEYMAP_FORMAT_متن_V1,
    _XKB_KEYMAP_FORMAT_MAX = XKB_KEYMAP_FORMAT_متن_V2,
    XKB_KEYMAP_FORMAT_عدد = 2,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keymap_serialize_flags_ارزښتونه[] = {
    XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    XKB_KEYMAP_SERIALIZE_PRETTY,
    XKB_KEYMAP_SERIALIZE_KEEP_ناکارول_شوی,
    XKB_KEYMAP_SERIALIZE_STRICT_ډېرپېښ,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ارزښتونه,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS,
    XKB_KEYMAP_SERIALIZE_EXPLICIT_کلی_ارزښتونه,
};
#د_که_پای

تعداد xkb_keymap_serialize_flags_bounds {
    _XKB_KEYMAP_SERIALIZE_FLAGS_MIN = XKB_KEYMAP_SERIALIZE_NO_FLAGS,
    _XKB_KEYMAP_SERIALIZE_FLAGS_MAX = XKB_KEYMAP_SERIALIZE_EXPLICIT_کلی_ارزښتونه,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keymap_کلی_iterator_flags_ارزښتونه[] = {
    XKB_KEYMAP_کلی_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلی_ITERATOR_DESCENDING_ترتیب,
    XKB_KEYMAP_کلی_ITERATOR_INCLUDE_UNBOUND,
};
#د_که_پای

تعداد xkb_keymap_کلی_iterator_flags_bounds {
    XKB_KEYMAP_کلی_ITERATOR_FLAGS_MIN = XKB_KEYMAP_کلی_ITERATOR_NO_FLAGS,
    XKB_KEYMAP_کلی_ITERATOR_FLAGS_MAX = XKB_KEYMAP_کلی_ITERATOR_INCLUDE_UNBOUND,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_کلی_direction_ارزښتونه[] = {
    XKB_کلی_پورته,
    XKB_کلی_ښکته,
    XKB_کلی_REPEATED,
};
#د_که_پای

تعداد xkb_کلی_direction_bounds {
    XKB_کلی_DIRECTION_MIN = XKB_کلی_پورته,
    XKB_کلی_DIRECTION_MAX = XKB_کلی_REPEATED,
    XKB_کلی_DIRECTION_عدد = 3,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_state_component_ارزښتونه[] = {
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
#د_که_پای

تعداد xkb_state_component_bounds {
    _XKB_STATE_COMPONENT_MIN = 0 /* implicit minimum */,
    _XKB_STATE_COMPONENT_MAX = XKB_STATE_CONTROLS_EFFECTIVE,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_keyboard_control_flags_ارزښتونه[] = {
    XKB_KEYBOARD_CONTROL_NO_FLAGS,
    XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې,
    XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې,
};
#د_که_پای

تعداد xkb_keyboard_control_flags_bounds {
    _XKB_KEYBOARD_CONTROL_FLAGS_MIN = XKB_KEYBOARD_CONTROL_NO_FLAGS,
    _XKB_KEYBOARD_CONTROL_FLAGS_MAX = XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_event_type_ارزښتونه[] = {
    XKB_EVENT_TYPE_INVALID,
    XKB_EVENT_TYPE_چوکاټ,
    XKB_EVENT_TYPE_کلی,
    XKB_EVENT_TYPE_STATE_COMPONENTS,
    XKB_EVENT_TYPE_اشاره_ګر_MOTION,
    XKB_EVENT_TYPE_اشاره_ګر_BUTTON,
    XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
};
#د_که_پای

تعداد xkb_event_type_bounds {
    _XKB_EVENT_TYPE_MIN = XKB_EVENT_TYPE_INVALID,
    _XKB_EVENT_TYPE_MAX = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
    XKB_EVENT_TYPE_عدد = 8,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_اشاره_ګر_motion_flags_ارزښتونه[] = {
    XKB_اشاره_ګر_MOTION_NO_FLAGS,
    XKB_اشاره_ګر_MOTION_REPEATS,
    XKB_اشاره_ګر_MOTION_ABSOLUTE_X,
    XKB_اشاره_ګر_MOTION_ABSOLUTE_Y,
};
#د_که_پای

تعداد xkb_اشاره_ګر_motion_flags_bounds {
    XKB_اشاره_ګر_MOTION_FLAGS_MIN = XKB_اشاره_ګر_MOTION_NO_FLAGS,
    XKB_اشاره_ګر_MOTION_FLAGS_MAX = XKB_اشاره_ګر_MOTION_ABSOLUTE_Y,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_اشاره_ګر_button_state_ارزښتونه[] = {
    XKB_اشاره_ګر_BUTTON_RELEASED,
    XKB_اشاره_ګر_BUTTON_PRESSED,
};
#د_که_پای

تعداد xkb_اشاره_ګر_button_state_bounds {
    XKB_اشاره_ګر_BUTTON_STATE_MIN = XKB_اشاره_ګر_BUTTON_RELEASED,
    XKB_اشاره_ګر_BUTTON_STATE_MAX = XKB_اشاره_ګر_BUTTON_PRESSED,
    XKB_اشاره_ګر_BUTTON_STATE_عدد = 2,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_events_flags_ارزښتونه[] = {
    XKB_EVENTS_NO_FLAGS,
};
#د_که_پای

تعداد xkb_events_flags_bounds {
    _XKB_EVENTS_FLAGS_MIN = XKB_EVENTS_NO_FLAGS,
    _XKB_EVENTS_FLAGS_MAX = XKB_EVENTS_NO_FLAGS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_machine_builder_flags_ارزښتونه[] = {
    XKB_MACHINE_BUILDER_NO_FLAGS,
};
#د_که_پای

تعداد xkb_machine_builder_flags_bounds {
    _XKB_MACHINE_BUILDER_FLAGS_MIN = XKB_MACHINE_BUILDER_NO_FLAGS,
    _XKB_MACHINE_BUILDER_FLAGS_MAX = XKB_MACHINE_BUILDER_NO_FLAGS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_machine_flags_ارزښتونه[] = {
    XKB_MACHINE_NO_FLAGS,
    XKB_MACHINE_SERVER_ACTIONS,
    XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلی_FIX,
    XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};
#د_که_پای

تعداد xkb_machine_flags_bounds {
    _XKB_MACHINE_FLAGS_MIN = XKB_MACHINE_NO_FLAGS,
    _XKB_MACHINE_FLAGS_MAX = XKB_MACHINE_UNLOCK_MODS_ON_PRESS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_a11y_flags_ارزښتونه[] = {
    XKB_A11Y_NO_FLAGS,
    XKB_A11_Y_STICKY_کلیګانې_NO_همزمانه_کلیګانې,
    XKB_A11_Y_STICKY_کلیګانې_LATCH_منزل_قلف,
    XKB_A11_Y_LATCH_همزمانه_کلیګانې,
};
#د_که_پای

تعداد xkb_a11y_flags_bounds {
    _XKB_A11Y_FLAGS_MIN = XKB_A11Y_NO_FLAGS,
    _XKB_A11Y_FLAGS_MAX = XKB_A11_Y_LATCH_همزمانه_کلیګانې,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_layout_بهر_د_range_policy_ارزښتونه[] = {
    XKB_LAYOUT_بهر_د_RANGE_WRAP,
    XKB_LAYOUT_بهر_د_RANGE_CLAMP,
    XKB_LAYOUT_بهر_د_RANGE_REDIRECT,
};
#د_که_پای

تعداد xkb_layout_بهر_د_range_policy_bounds {
    XKB_LAYOUT_بهر_د_RANGE_POLICY_MIN = XKB_LAYOUT_بهر_د_RANGE_WRAP,
    XKB_LAYOUT_بهر_د_RANGE_POLICY_MAX = XKB_LAYOUT_بهر_د_RANGE_REDIRECT,
    XKB_LAYOUT_بهر_د_RANGE_POLICY_عدد = 3,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_state_ډېرپېښ_ارزښتونه[] = {
    XKB_STATE_ډېرپېښ_CLIENT,
    XKB_STATE_ډېرپېښ_SERVER_QUERY,
    XKB_STATE_ډېرپېښ_SERVER,
};
#د_که_پای

تعداد xkb_state_ډېرپېښ_bounds {
    XKB_STATE_ډېرپېښ_MIN = XKB_STATE_ډېرپېښ_CLIENT,
    XKB_STATE_ډېرپېښ_MAX = XKB_STATE_ډېرپېښ_SERVER,
    XKB_STATE_ډېرپېښ_عدد = 3,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_state_سمون_ارزښتونه[] = {
    XKB_STATE_سمون_ANY,
    XKB_STATE_سمون_ALL,
    XKB_STATE_سمون_NON_EXCLUSIVE,
};
#د_که_پای

تعداد xkb_state_سمون_bounds {
    XKB_STATE_سمون_MIN = 0 /* implicit minimum */,
    XKB_STATE_سمون_MAX = XKB_STATE_سمون_NON_EXCLUSIVE,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_consumed_ډېرپېښ_ارزښتونه[] = {
    XKB_CONSUMED_ډېرپېښ_XKB,
    XKB_CONSUMED_ډېرپېښ_GTK,
};
#د_که_پای

تعداد xkb_consumed_ډېرپېښ_bounds {
    XKB_CONSUMED_ډېرپېښ_MIN = XKB_CONSUMED_ډېرپېښ_XKB,
    XKB_CONSUMED_ډېرپېښ_MAX = XKB_CONSUMED_ډېرپېښ_GTK,
    XKB_CONSUMED_ډېرپېښ_عدد = 2,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_compose_compile_flags_ارزښتونه[] = {
    XKB_COMPOSE_COMPILE_NO_FLAGS,
};
#د_که_پای

تعداد xkb_compose_compile_flags_bounds {
    _XKB_COMPOSE_COMPILE_FLAGS_MIN = XKB_COMPOSE_COMPILE_NO_FLAGS,
    _XKB_COMPOSE_COMPILE_FLAGS_MAX = XKB_COMPOSE_COMPILE_NO_FLAGS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_compose_format_ارزښتونه[] = {
    XKB_COMPOSE_FORMAT_متن_V1,
};
#د_که_پای

تعداد xkb_compose_format_bounds {
    _XKB_COMPOSE_FORMAT_MIN = XKB_COMPOSE_FORMAT_متن_V1,
    _XKB_COMPOSE_FORMAT_MAX = XKB_COMPOSE_FORMAT_متن_V1,
    XKB_COMPOSE_FORMAT_عدد = 1,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_compose_state_flags_ارزښتونه[] = {
    XKB_COMPOSE_STATE_NO_FLAGS,
};
#د_که_پای

تعداد xkb_compose_state_flags_bounds {
    _XKB_COMPOSE_STATE_FLAGS_MIN = XKB_COMPOSE_STATE_NO_FLAGS,
    _XKB_COMPOSE_STATE_FLAGS_MAX = XKB_COMPOSE_STATE_NO_FLAGS,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_compose_حالت_ارزښتونه[] = {
    XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_COMPOSING,
    XKB_COMPOSE_COMPOSED,
    XKB_COMPOSE_CANCELLED,
};
#د_که_پای

تعداد xkb_compose_حالت_bounds {
    XKB_COMPOSE_حالت_MIN = XKB_COMPOSE_NOTHING,
    XKB_COMPOSE_حالت_MAX = XKB_COMPOSE_CANCELLED,
    XKB_COMPOSE_حالت_عدد = 4,
};

#که_تعریف_شوی ENABLE_شخصي_APIS
ساكن ثابت uint32_t xkb_compose_feed_پایله_ارزښتونه[] = {
    XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_ACCEPTED,
};
#د_که_پای

تعداد xkb_compose_feed_پایله_bounds {
    XKB_COMPOSE_FEED_پایله_MIN = XKB_COMPOSE_FEED_IGNORED,
    XKB_COMPOSE_FEED_پایله_MAX = XKB_COMPOSE_FEED_ACCEPTED,
    XKB_COMPOSE_FEED_پایله_عدد = 2,
};
ساكن ثابت uint32_t xkb_حالت_ارزښتونه[] = {
    (uint32_t)XKB_ERROR_INVALID,
    XKB_SUCCESS,
    XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
    XKB_ERROR_UNSUPPORTED_STATE_ډېرپېښ,
    XKB_ERROR_EVENT_TYPE_MISMATCH,
    XKB_ERROR_UNSUPPORTED_کلی_ITERATOR_FLAGS,
    XKB_ERROR_UNSUPPORTED_LAYOUT_بهر_د_RANGE_POLICY,
    XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص,
    XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT,
    XKB_ERROR_LAYOUT_شمېر_حد_EXCEEDED,
    XKB_ERROR_UNSUPPORTED_A11Y_FLAGS,
    XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ,
    XKB_ERROR_ABI_INVALID_STRUCT_اندازه,
    XKB_ERROR_INVALID_KEYCODE,
    XKB_ERROR_ویش_FAILURE,
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
ساكن ثابت uint32_t xkb_feature_ارزښتونه[] = {
    XKB_FEATURE_ENUM_FEATURE,
    XKB_FEATURE_ENUM_حالت,
    XKB_FEATURE_ENUM_CONTEXT_FLAGS,
    XKB_FEATURE_ENUM_LOG_LEVEL,
    XKB_FEATURE_ENUM_KEYSYM_FLAGS,
    XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_FORMAT,
    XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS,
    XKB_FEATURE_ENUM_KEYMAP_کلی_ITERATOR_FLAGS,
    XKB_FEATURE_ENUM_STATE_COMPONENT,
    XKB_FEATURE_ENUM_LAYOUT_بهر_د_RANGE_POLICY,
    XKB_FEATURE_ENUM_A11Y_FLAGS,
    XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS,
    XKB_FEATURE_ENUM_STATE_ډېرپېښ,
    XKB_FEATURE_ENUM_STATE_سمون,
    XKB_FEATURE_ENUM_CONSUMED_ډېرپېښ,
    XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS,
    XKB_FEATURE_ENUM_MACHINE_FLAGS,
    XKB_FEATURE_ENUM_EVENT_TYPE,
    XKB_FEATURE_ENUM_کلی_DIRECTION,
    XKB_FEATURE_ENUM_اشاره_ګر_MOTION_FLAGS,
    XKB_FEATURE_ENUM_اشاره_ګر_BUTTON_STATE,
    XKB_FEATURE_ENUM_EVENTS_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FORMAT,
    XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_حالت,
    XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS,
    XKB_FEATURE_ENUM_COMPOSE_FEED_پایله,
};

تعداد xkb_feature_bounds {
    _XKB_FEATURE_MIN = XKB_FEATURE_ENUM_FEATURE,
    _XKB_FEATURE_MAX = XKB_FEATURE_ENUM_COMPOSE_FEED_پایله,
    XKB_FEATURE_عدد = 29,
};
