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
is_supported_enum_قدر_mask(تعداد xkb_enumerations_اقدار اقدار, uint32_t قدر)
{
    أرجع (قدر < UINT32_WIDTH && (اقدار & (UINT32_C(1) << قدر)));
}

ساكن bool
is_supported_enum_قدر_array(ثابت uint32_t *اقدار, size_t size, uint32_t قدر)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (اقدار[v] == قدر) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_قدر(تعداد xkb_enumerations_اقدار اقدار,
                        bool accept_zero, uint32_t قدر)
{
    أرجع (accept_zero || قدر) && ((uint32_t) اقدار & قدر) == قدر;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t قدر)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_قدر_array(
            xkb_feature_اقدار, ARRAY_SIZE(xkb_feature_اقدار), قدر
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_قدر_array(
            xkb_status_اقدار, ARRAY_SIZE(xkb_status_اقدار), قدر
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_CONTEXT_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_قدر_array(
            xkb_log_level_اقدار, ARRAY_SIZE(xkb_log_level_اقدار), قدر
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYSYM_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_RMLVO_BUILDER_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_قدر_mask(XKB_KEYMAP_FORMAT_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_COMPILE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_SERIALIZE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_قدر(
            XKB_STATE_COMPONENT_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_قدر_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_A11_Y_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYBOARD_CONTROL_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_قدر_mask(XKB_STATE_MODE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_قدر(
            XKB_STATE_MATCH_اقدار, false, قدر
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_قدر_mask(XKB_CONSUMED_MODE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_BUILDER_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_قدر_mask(XKB_EVENT_TYPE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_کلید_DIRECTION:
        أرجع is_supported_enum_قدر_mask(XKB_کلید_DIRECTION_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_POINTER_MOTION_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_قدر_mask(XKB_POINTER_BUTTON_STATE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_EVENTS_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_قدر_mask(XKB_COMPOSE_FORMAT_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_COMPILE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_قدر_mask(XKB_COMPOSE_STATUS_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_STATE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_قدر_mask(XKB_COMPOSE_FEED_RESULT_اقدار, قدر);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
