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
is_supported_enum_قيمة_mask(تعداد xkb_enumerations_قيم قيم, uint32_t قيمة)
{
    أرجع (قيمة < UINT32_WIDTH && (قيم & (UINT32_C(1) << قيمة)));
}

ساكن bool
is_supported_enum_قيمة_array(ثابت uint32_t *قيم, size_t size, uint32_t قيمة)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (قيم[v] == قيمة) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_قيمة(تعداد xkb_enumerations_قيم قيم,
                        bool accept_zero, uint32_t قيمة)
{
    أرجع (accept_zero || قيمة) && ((uint32_t) قيم & قيمة) == قيمة;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t قيمة)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_قيمة_array(
            xkb_feature_قيم, ARRAY_SIZE(xkb_feature_قيم), قيمة
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_قيمة_array(
            xkb_status_قيم, ARRAY_SIZE(xkb_status_قيم), قيمة
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_CONTEXT_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_قيمة_array(
            xkb_log_level_قيم, ARRAY_SIZE(xkb_log_level_قيم), قيمة
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_KEYSYM_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_RMLVO_BUILDER_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_قيمة_mask(XKB_KEYMAP_FORMAT_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_KEYMAP_COMPILE_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_KEYMAP_SERIALIZE_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_مفتاح_ITERATOR_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_KEYMAP_مفتاح_ITERATOR_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_قيمة(
            XKB_STATE_COMPONENT_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_قيمة_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_A11_Y_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_KEYBOARD_CONTROL_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_قيمة_mask(XKB_STATE_MODE_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_قيمة(
            XKB_STATE_MATCH_قيم, false, قيمة
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_قيمة_mask(XKB_CONSUMED_MODE_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_MACHINE_BUILDER_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_MACHINE_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_قيمة_mask(XKB_EVENT_TYPE_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_مفتاح_DIRECTION:
        أرجع is_supported_enum_قيمة_mask(XKB_مفتاح_DIRECTION_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_POINTER_MOTION_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_قيمة_mask(XKB_POINTER_BUTTON_STATE_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_EVENTS_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_قيمة_mask(XKB_COMPOSE_FORMAT_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_COMPOSE_COMPILE_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_قيمة_mask(XKB_COMPOSE_STATUS_قيم, قيمة);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_قيمة(
            XKB_COMPOSE_STATE_FLAGS_قيم, true, قيمة
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_قيمة_mask(XKB_COMPOSE_FEED_RESULT_قيم, قيمة);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
