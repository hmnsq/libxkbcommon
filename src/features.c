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

ایستا bool
is_supported_enum_مقدار_mask(شمارش xkb_enumerations_مقادیر مقادیر, uint32_t مقدار)
{
    بازگشت (مقدار < UINT32_WIDTH && (مقادیر & (UINT32_C(1) << مقدار)));
}

ایستا bool
is_supported_enum_مقدار_array(ثابت uint32_t *مقادیر, size_t size, uint32_t مقدار)
{
    برای (size_t v = 0; v < size; v++) {
        اگر (مقادیر[v] == مقدار) {
            بازگشت true;
        }
    }
    بازگشت false;
}

ایستا درونخطی bool
is_supported_flag_مقدار(شمارش xkb_enumerations_مقادیر مقادیر,
                        bool accept_zero, uint32_t مقدار)
{
    بازگشت (accept_zero || مقدار) && ((uint32_t) مقادیر & مقدار) == مقدار;
}

bool
xkb_feature_supported(شمارش xkb_feature feature, uint32_t مقدار)
{
    انتخاب (feature) {
    حالت XKB_FEATURE_ENUM_FEATURE:
        بازگشت is_supported_enum_مقدار_array(
            xkb_feature_مقادیر, ARRAY_SIZE(xkb_feature_مقادیر), مقدار
        );
    حالت XKB_FEATURE_ENUM_STATUS:
        بازگشت is_supported_enum_مقدار_array(
            xkb_status_مقادیر, ARRAY_SIZE(xkb_status_مقادیر), مقدار
        );
    حالت XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_CONTEXT_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_LOG_LEVEL:
        بازگشت is_supported_enum_مقدار_array(
            xkb_log_level_مقادیر, ARRAY_SIZE(xkb_log_level_مقادیر), مقدار
        );
    حالت XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_KEYSYM_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_RMLVO_BUILDER_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        بازگشت is_supported_enum_مقدار_mask(XKB_KEYMAP_FORMAT_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_KEYMAP_COMPILE_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_KEYMAP_SERIALIZE_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_STATE_COMPONENT:
        بازگشت is_supported_flag_مقدار(
            XKB_STATE_COMPONENT_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        بازگشت is_supported_enum_مقدار_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_A11Y_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_A11_Y_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_KEYBOARD_CONTROL_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_STATE_MODE:
        بازگشت is_supported_enum_مقدار_mask(XKB_STATE_MODE_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_STATE_MATCH:
        بازگشت is_supported_flag_مقدار(
            XKB_STATE_MATCH_مقادیر, false, مقدار
        );
    حالت XKB_FEATURE_ENUM_CONSUMED_MODE:
        بازگشت is_supported_enum_مقدار_mask(XKB_CONSUMED_MODE_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_MACHINE_BUILDER_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_MACHINE_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_MACHINE_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_EVENT_TYPE:
        بازگشت is_supported_enum_مقدار_mask(XKB_EVENT_TYPE_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_کلید_DIRECTION:
        بازگشت is_supported_enum_مقدار_mask(XKB_کلید_DIRECTION_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_POINTER_MOTION_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        بازگشت is_supported_enum_مقدار_mask(XKB_POINTER_BUTTON_STATE_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_EVENTS_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_EVENTS_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        بازگشت is_supported_enum_مقدار_mask(XKB_COMPOSE_FORMAT_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_COMPOSE_COMPILE_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_STATUS:
        بازگشت is_supported_enum_مقدار_mask(XKB_COMPOSE_STATUS_مقادیر, مقدار);
    حالت XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        بازگشت is_supported_flag_مقدار(
            XKB_COMPOSE_STATE_FLAGS_مقادیر, true, مقدار
        );
    حالت XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        بازگشت is_supported_enum_مقدار_mask(XKB_COMPOSE_FEED_RESULT_مقادیر, مقدار);
    پیشفرض:
        /* Invalid feature */
        بازگشت false;
    }
}
