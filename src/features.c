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
is_supported_enum_قىممەت_mask(تعداد xkb_enumerations_قىممەتلەر قىممەتلەر, uint32_t قىممەت)
{
    أرجع (قىممەت < UINT32_WIDTH && (قىممەتلەر & (UINT32_C(1) << قىممەت)));
}

ساكن bool
is_supported_enum_قىممەت_array(ثابت uint32_t *قىممەتلەر, size_t size, uint32_t قىممەت)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (قىممەتلەر[v] == قىممەت) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_قىممەت(تعداد xkb_enumerations_قىممەتلەر قىممەتلەر,
                        bool accept_zero, uint32_t قىممەت)
{
    أرجع (accept_zero || قىممەت) && ((uint32_t) قىممەتلەر & قىممەت) == قىممەت;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t قىممەت)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_قىممەت_array(
            xkb_feature_قىممەتلەر, ARRAY_SIZE(xkb_feature_قىممەتلەر), قىممەت
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_قىممەت_array(
            xkb_status_قىممەتلەر, ARRAY_SIZE(xkb_status_قىممەتلەر), قىممەت
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_CONTEXT_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_قىممەت_array(
            xkb_log_level_قىممەتلەر, ARRAY_SIZE(xkb_log_level_قىممەتلەر), قىممەت
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_KEYSYM_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_RMLVO_BUILDER_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_قىممەت_mask(XKB_KEYMAP_FORMAT_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_KEYMAP_COMPILE_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_KEYMAP_SERIALIZE_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_KEYMAP_ئاچقۇچ_ITERATOR_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_قىممەت(
            XKB_STATE_COMPONENT_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_قىممەت_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_A11_Y_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_KEYBOARD_CONTROL_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_قىممەت_mask(XKB_STATE_MODE_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_قىممەت(
            XKB_STATE_MATCH_قىممەتلەر, false, قىممەت
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_قىممەت_mask(XKB_CONSUMED_MODE_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_MACHINE_BUILDER_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_MACHINE_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_قىممەت_mask(XKB_EVENT_TYPE_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_ئاچقۇچ_DIRECTION:
        أرجع is_supported_enum_قىممەت_mask(XKB_ئاچقۇچ_DIRECTION_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_POINTER_MOTION_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_قىممەت_mask(XKB_POINTER_BUTTON_STATE_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_EVENTS_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_قىممەت_mask(XKB_COMPOSE_FORMAT_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_COMPOSE_COMPILE_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_قىممەت_mask(XKB_COMPOSE_STATUS_قىممەتلەر, قىممەت);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_قىممەت(
            XKB_COMPOSE_STATE_FLAGS_قىممەتلەر, true, قىممەت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_قىممەت_mask(XKB_COMPOSE_FEED_RESULT_قىممەتلەر, قىممەت);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
