/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon-features.h"
#شامل "features/enums.h"
#شامل "utils.h"

ساکن bool
is_supported_enum_قدر_نقاب(شمار xkb_enumerations_اقدار اقدار, uint32_t قدر)
{
    واپس (قدر < UINT32_چوڑائی && (اقدار & (UINT32_C(1) << قدر)));
}

ساکن bool
is_supported_enum_قدر_array(ثابت uint32_t *اقدار, سائز_t سائز, uint32_t قدر)
{
    برائے (سائز_t v = 0; v < سائز; v++) {
        اگر (اقدار[v] == قدر) {
            واپس true;
        }
    }
    واپس false;
}

ساکن اندرونی bool
is_supported_flag_قدر(شمار xkb_enumerations_اقدار اقدار,
                        bool accept_zero, uint32_t قدر)
{
    واپس (accept_zero || قدر) && ((uint32_t) اقدار & قدر) == قدر;
}

bool
xkb_feature_supported(شمار xkb_feature feature, uint32_t قدر)
{
    انتخاب (feature) {
    حالت XKB_FEATURE_ENUM_FEATURE:
        واپس is_supported_enum_قدر_array(
            xkb_feature_اقدار, ARRAY_سائز(xkb_feature_اقدار), قدر
        );
    حالت XKB_FEATURE_ENUM_حالت:
        واپس is_supported_enum_قدر_array(
            xkb_حالت_اقدار, ARRAY_سائز(xkb_حالت_اقدار), قدر
        );
    حالت XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_CONTEXT_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_LOG_LEVEL:
        واپس is_supported_enum_قدر_array(
            xkb_log_level_اقدار, ARRAY_سائز(xkb_log_level_اقدار), قدر
        );
    حالت XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_KEYSYM_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_RMLVO_BUILDER_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        واپس is_supported_enum_قدر_نقاب(XKB_KEYMAP_FORMAT_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_KEYMAP_COMPILE_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_KEYMAP_SERIALIZE_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_STATE_COMPONENT:
        واپس is_supported_flag_قدر(
            XKB_STATE_COMPONENT_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_LAYOUT_برآمد_کا_RANGE_POLICY:
        واپس is_supported_enum_قدر_نقاب(XKB_LAYOUT_برآمد_کا_RANGE_POLICY_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_A11Y_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_A11_Y_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_KEYBOARD_CONTROL_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_STATE_منوال:
        واپس is_supported_enum_قدر_نقاب(XKB_STATE_منوال_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_STATE_مطابقت:
        واپس is_supported_flag_قدر(
            XKB_STATE_مطابقت_اقدار, false, قدر
        );
    حالت XKB_FEATURE_ENUM_CONSUMED_منوال:
        واپس is_supported_enum_قدر_نقاب(XKB_CONSUMED_منوال_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_MACHINE_BUILDER_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_MACHINE_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_MACHINE_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_EVENT_TYPE:
        واپس is_supported_enum_قدر_نقاب(XKB_EVENT_TYPE_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_کلید_DIRECTION:
        واپس is_supported_enum_قدر_نقاب(XKB_کلید_DIRECTION_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_اشارہ_گر_MOTION_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_اشارہ_گر_MOTION_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_اشارہ_گر_BUTTON_STATE:
        واپس is_supported_enum_قدر_نقاب(XKB_اشارہ_گر_BUTTON_STATE_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_EVENTS_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_EVENTS_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        واپس is_supported_enum_قدر_نقاب(XKB_COMPOSE_FORMAT_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_COMPOSE_COMPILE_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_حالت:
        واپس is_supported_enum_قدر_نقاب(XKB_COMPOSE_حالت_اقدار, قدر);
    حالت XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        واپس is_supported_flag_قدر(
            XKB_COMPOSE_STATE_FLAGS_اقدار, true, قدر
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجہ:
        واپس is_supported_enum_قدر_نقاب(XKB_COMPOSE_FEED_نتیجہ_اقدار, قدر);
    طےشدہ:
        /* Invalid feature */
        واپس false;
    }
}
