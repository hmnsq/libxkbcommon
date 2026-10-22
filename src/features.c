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
is_supported_enum_نيلاي_mask(تعداد xkb_enumerations_نيلاي_سموا نيلاي_سموا, uint32_t نيلاي)
{
    أرجع (نيلاي < UINT32_WIDTH && (نيلاي_سموا & (UINT32_C(1) << نيلاي)));
}

ساكن bool
is_supported_enum_نيلاي_array(ثابت uint32_t *نيلاي_سموا, size_t size, uint32_t نيلاي)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (نيلاي_سموا[v] == نيلاي) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_نيلاي(تعداد xkb_enumerations_نيلاي_سموا نيلاي_سموا,
                        bool accept_zero, uint32_t نيلاي)
{
    أرجع (accept_zero || نيلاي) && ((uint32_t) نيلاي_سموا & نيلاي) == نيلاي;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t نيلاي)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_نيلاي_array(
            xkb_feature_نيلاي_سموا, ARRAY_SIZE(xkb_feature_نيلاي_سموا), نيلاي
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_نيلاي_array(
            xkb_status_نيلاي_سموا, ARRAY_SIZE(xkb_status_نيلاي_سموا), نيلاي
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_CONTEXT_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_نيلاي_array(
            xkb_log_level_نيلاي_سموا, ARRAY_SIZE(xkb_log_level_نيلاي_سموا), نيلاي
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_KEYSYM_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_RMLVO_BUILDER_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_نيلاي_mask(XKB_KEYMAP_FORMAT_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_KEYMAP_COMPILE_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_KEYMAP_SERIALIZE_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_کونچي_ITERATOR_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_KEYMAP_کونچي_ITERATOR_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_نيلاي(
            XKB_STATE_COMPONENT_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_نيلاي_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_A11_Y_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_KEYBOARD_CONTROL_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_نيلاي_mask(XKB_STATE_MODE_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_نيلاي(
            XKB_STATE_MATCH_نيلاي_سموا, false, نيلاي
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_نيلاي_mask(XKB_CONSUMED_MODE_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_MACHINE_BUILDER_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_MACHINE_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_نيلاي_mask(XKB_EVENT_TYPE_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_کونچي_DIRECTION:
        أرجع is_supported_enum_نيلاي_mask(XKB_کونچي_DIRECTION_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_POINTER_MOTION_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_نيلاي_mask(XKB_POINTER_BUTTON_STATE_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_EVENTS_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_نيلاي_mask(XKB_COMPOSE_FORMAT_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_COMPOSE_COMPILE_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_نيلاي_mask(XKB_COMPOSE_STATUS_نيلاي_سموا, نيلاي);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_نيلاي(
            XKB_COMPOSE_STATE_FLAGS_نيلاي_سموا, true, نيلاي
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_نيلاي_mask(XKB_COMPOSE_FEED_RESULT_نيلاي_سموا, نيلاي);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
