/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon-features.h"
#ضمن "features/enums.h"
#ضمن "utils.h"

ساكن bool
is_supported_enum_value_mask(تعداد xkb_enumerations_values values, uint32_t value)
{
    החזר (value < UINT32_WIDTH && (values & (UINT32_C(1) << value)));
}

ساكن bool
is_supported_enum_value_array(ثابت uint32_t *values, size_t size, uint32_t value)
{
    עבור (size_t v = 0; v < size; v++) {
        אם (values[v] == value) {
            החזר true;
        }
    }
    החזר false;
}

ساكن مضمن bool
is_supported_flag_value(تعداد xkb_enumerations_values values,
                        bool accept_zero, uint32_t value)
{
    החזר (accept_zero || value) && ((uint32_t) values & value) == value;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t value)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        החזר is_supported_enum_value_array(
            xkb_feature_values, ARRAY_SIZE(xkb_feature_values), value
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        החזר is_supported_enum_value_array(
            xkb_status_values, ARRAY_SIZE(xkb_status_values), value
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        החזר is_supported_flag_value(
            XKB_CONTEXT_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        החזר is_supported_enum_value_array(
            xkb_log_level_values, ARRAY_SIZE(xkb_log_level_values), value
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        החזר is_supported_flag_value(
            XKB_KEYSYM_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        החזר is_supported_flag_value(
            XKB_RMLVO_BUILDER_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        החזר is_supported_enum_value_mask(XKB_KEYMAP_FORMAT_VALUES, value);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        החזר is_supported_flag_value(
            XKB_KEYMAP_COMPILE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        החזר is_supported_flag_value(
            XKB_KEYMAP_SERIALIZE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_KEY_ITERATOR_FLAGS:
        החזר is_supported_flag_value(
            XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        החזר is_supported_flag_value(
            XKB_STATE_COMPONENT_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        החזר is_supported_enum_value_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_VALUES, value);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        החזר is_supported_flag_value(
            XKB_A11Y_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        החזר is_supported_flag_value(
            XKB_KEYBOARD_CONTROL_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        החזר is_supported_enum_value_mask(XKB_STATE_MODE_VALUES, value);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        החזר is_supported_flag_value(
            XKB_STATE_MATCH_VALUES, false, value
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        החזר is_supported_enum_value_mask(XKB_CONSUMED_MODE_VALUES, value);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        החזר is_supported_flag_value(
            XKB_MACHINE_BUILDER_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        החזר is_supported_flag_value(
            XKB_MACHINE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        החזר is_supported_enum_value_mask(XKB_EVENT_TYPE_VALUES, value);
    حالة XKB_FEATURE_ENUM_KEY_DIRECTION:
        החזר is_supported_enum_value_mask(XKB_KEY_DIRECTION_VALUES, value);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        החזר is_supported_flag_value(
            XKB_POINTER_MOTION_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        החזר is_supported_enum_value_mask(XKB_POINTER_BUTTON_STATE_VALUES, value);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        החזר is_supported_flag_value(
            XKB_EVENTS_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        החזר is_supported_enum_value_mask(XKB_COMPOSE_FORMAT_VALUES, value);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        החזר is_supported_flag_value(
            XKB_COMPOSE_COMPILE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        החזר is_supported_enum_value_mask(XKB_COMPOSE_STATUS_VALUES, value);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        החזר is_supported_flag_value(
            XKB_COMPOSE_STATE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        החזר is_supported_enum_value_mask(XKB_COMPOSE_FEED_RESULT_VALUES, value);
    افتراضي:
        /* Invalid feature */
        החזר false;
    }
}
