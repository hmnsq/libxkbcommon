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
is_supported_enum_مقدار_ماسک(تعداد xkb_enumerations_مقادیر مقادیر, uint32_t مقدار)
{
    أرجع (مقدار < UINT32_عرض && (مقادیر & (UINT32_C(1) << مقدار)));
}

ساكن bool
is_supported_enum_مقدار_array(ثابت uint32_t *مقادیر, اندازه_t اندازه, uint32_t مقدار)
{
    لكل (اندازه_t v = 0; v < اندازه; v++) {
        إذا (مقادیر[v] == مقدار) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_مقدار(تعداد xkb_enumerations_مقادیر مقادیر,
                        bool accept_zero, uint32_t مقدار)
{
    أرجع (accept_zero || مقدار) && ((uint32_t) مقادیر & مقدار) == مقدار;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t مقدار)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_مقدار_array(
            xkb_feature_مقادیر, ARRAY_اندازه(xkb_feature_مقادیر), مقدار
        );
    حالة XKB_FEATURE_ENUM_وضعیت:
        أرجع is_supported_enum_مقدار_array(
            xkb_وضعیت_مقادیر, ARRAY_اندازه(xkb_وضعیت_مقادیر), مقدار
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_CONTEXT_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_مقدار_array(
            xkb_log_level_مقادیر, ARRAY_اندازه(xkb_log_level_مقادیر), مقدار
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_KEYSYM_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_RMLVO_BUILDER_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_مقدار_ماسک(XKB_KEYMAP_FORMAT_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_KEYMAP_COMPILE_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_KEYMAP_SERIALIZE_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_کلید_ITERATOR_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_مقدار(
            XKB_STATE_COMPONENT_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_بیرون_از_RANGE_POLICY:
        أرجع is_supported_enum_مقدار_ماسک(XKB_LAYOUT_بیرون_از_RANGE_POLICY_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_A11_Y_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_KEYBOARD_CONTROL_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_STATE_نما:
        أرجع is_supported_enum_مقدار_ماسک(XKB_STATE_نما_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_STATE_تطابق:
        أرجع is_supported_flag_مقدار(
            XKB_STATE_تطابق_مقادیر, false, مقدار
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_نما:
        أرجع is_supported_enum_مقدار_ماسک(XKB_CONSUMED_نما_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_MACHINE_BUILDER_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_MACHINE_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_مقدار_ماسک(XKB_EVENT_TYPE_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_کلید_DIRECTION:
        أرجع is_supported_enum_مقدار_ماسک(XKB_کلید_DIRECTION_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_اشاره_گر_MOTION_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_اشاره_گر_MOTION_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_اشاره_گر_BUTTON_STATE:
        أرجع is_supported_enum_مقدار_ماسک(XKB_اشاره_گر_BUTTON_STATE_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_EVENTS_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_مقدار_ماسک(XKB_COMPOSE_FORMAT_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_COMPOSE_COMPILE_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_وضعیت:
        أرجع is_supported_enum_مقدار_ماسک(XKB_COMPOSE_وضعیت_مقادیر, مقدار);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_مقدار(
            XKB_COMPOSE_STATE_FLAGS_مقادیر, true, مقدار
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجه:
        أرجع is_supported_enum_مقدار_ماسک(XKB_COMPOSE_FEED_نتیجه_مقادیر, مقدار);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
