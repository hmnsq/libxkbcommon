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
is_supported_enum_درج_mask(تعداد xkb_enumerations_درجوجي درجوجي, uint32_t درج)
{
    أرجع (درج < UINT32_WIDTH && (درجوجي & (UINT32_C(1) << درج)));
}

ساكن bool
is_supported_enum_درج_array(ثابت uint32_t *درجوجي, size_t size, uint32_t درج)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (درجوجي[v] == درج) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_درج(تعداد xkb_enumerations_درجوجي درجوجي,
                        bool accept_zero, uint32_t درج)
{
    أرجع (accept_zero || درج) && ((uint32_t) درجوجي & درج) == درج;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t درج)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_درج_array(
            xkb_feature_درجوجي, ARRAY_SIZE(xkb_feature_درجوجي), درج
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_درج_array(
            xkb_status_درجوجي, ARRAY_SIZE(xkb_status_درجوجي), درج
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_CONTEXT_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_درج_array(
            xkb_log_level_درجوجي, ARRAY_SIZE(xkb_log_level_درجوجي), درج
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_KEYSYM_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_RMLVO_BUILDER_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_درج_mask(XKB_KEYMAP_FORMAT_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_KEYMAP_COMPILE_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_KEYMAP_SERIALIZE_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_مبودي_ITERATOR_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_KEYMAP_مبودي_ITERATOR_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_درج(
            XKB_STATE_COMPONENT_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_درج_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_A11_Y_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_KEYBOARD_CONTROL_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_درج_mask(XKB_STATE_MODE_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_درج(
            XKB_STATE_MATCH_درجوجي, false, درج
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_درج_mask(XKB_CONSUMED_MODE_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_MACHINE_BUILDER_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_MACHINE_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_درج_mask(XKB_EVENT_TYPE_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_مبودي_DIRECTION:
        أرجع is_supported_enum_درج_mask(XKB_مبودي_DIRECTION_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_POINTER_MOTION_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_درج_mask(XKB_POINTER_BUTTON_STATE_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_EVENTS_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_درج_mask(XKB_COMPOSE_FORMAT_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_COMPOSE_COMPILE_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_درج_mask(XKB_COMPOSE_STATUS_درجوجي, درج);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_درج(
            XKB_COMPOSE_STATE_FLAGS_درجوجي, true, درج
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_درج_mask(XKB_COMPOSE_FEED_RESULT_درجوجي, درج);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
