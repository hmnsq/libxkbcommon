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
    أرجع (value < UINT32_WIDTH && (values & (UINT32_C(1) << value)));
}

ساكن bool
is_supported_enum_value_array(ثابت uint32_t *values, size_t size, uint32_t value)
{
    لكل (size_t v = 0; v < size; v++) {
        𐡄𐡍 (values[v] == value) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_value(تعداد xkb_enumerations_values values,
                        bool accept_zero, uint32_t value)
{
    أرجع (accept_zero || value) && ((uint32_t) values & value) == value;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t value)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_value_array(
            xkb_feature_values, ARRAY_SIZE(xkb_feature_values), value
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_value_array(
            xkb_status_values, ARRAY_SIZE(xkb_status_values), value
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_value(
            XKB_CONTEXT_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_value_array(
            xkb_log_level_values, ARRAY_SIZE(xkb_log_level_values), value
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_value(
            XKB_KEYSYM_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_value(
            XKB_RMLVO_BUILDER_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_value_mask(XKB_KEYMAP_FORMAT_VALUES, value);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_value(
            XKB_KEYMAP_COMPILE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_value(
            XKB_KEYMAP_SERIALIZE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_KEY_ITERATOR_FLAGS:
        أرجع is_supported_flag_value(
            XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_value(
            XKB_STATE_COMPONENT_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_value_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_VALUES, value);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_value(
            XKB_A11Y_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_value(
            XKB_KEYBOARD_CONTROL_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_value_mask(XKB_STATE_MODE_VALUES, value);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_value(
            XKB_STATE_MATCH_VALUES, false, value
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_value_mask(XKB_CONSUMED_MODE_VALUES, value);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_value(
            XKB_MACHINE_BUILDER_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_value(
            XKB_MACHINE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_value_mask(XKB_EVENT_TYPE_VALUES, value);
    حالة XKB_FEATURE_ENUM_KEY_DIRECTION:
        أرجع is_supported_enum_value_mask(XKB_KEY_DIRECTION_VALUES, value);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_value(
            XKB_POINTER_MOTION_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_value_mask(XKB_POINTER_BUTTON_STATE_VALUES, value);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_value(
            XKB_EVENTS_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_value_mask(XKB_COMPOSE_FORMAT_VALUES, value);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_value(
            XKB_COMPOSE_COMPILE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_value_mask(XKB_COMPOSE_STATUS_VALUES, value);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_value(
            XKB_COMPOSE_STATE_FLAGS_VALUES, true, value
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_value_mask(XKB_COMPOSE_FEED_RESULT_VALUES, value);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
