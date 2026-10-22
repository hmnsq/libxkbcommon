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
is_supported_enum_بەها_mask(تعداد xkb_enumerations_بەهاکان بەهاکان, uint32_t بەها)
{
    أرجع (بەها < UINT32_WIDTH && (بەهاکان & (UINT32_C(1) << بەها)));
}

ساكن bool
is_supported_enum_بەها_array(ثابت uint32_t *بەهاکان, size_t size, uint32_t بەها)
{
    لكل (size_t v = 0; v < size; v++) {
        إذا (بەهاکان[v] == بەها) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_بەها(تعداد xkb_enumerations_بەهاکان بەهاکان,
                        bool accept_zero, uint32_t بەها)
{
    أرجع (accept_zero || بەها) && ((uint32_t) بەهاکان & بەها) == بەها;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t بەها)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_بەها_array(
            xkb_feature_بەهاکان, ARRAY_SIZE(xkb_feature_بەهاکان), بەها
        );
    حالة XKB_FEATURE_ENUM_STATUS:
        أرجع is_supported_enum_بەها_array(
            xkb_status_بەهاکان, ARRAY_SIZE(xkb_status_بەهاکان), بەها
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_CONTEXT_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_بەها_array(
            xkb_log_level_بەهاکان, ARRAY_SIZE(xkb_log_level_بەهاکان), بەها
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_KEYSYM_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_RMLVO_BUILDER_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_بەها_mask(XKB_KEYMAP_FORMAT_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_KEYMAP_COMPILE_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_KEYMAP_SERIALIZE_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_کلیل_ITERATOR_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_KEYMAP_کلیل_ITERATOR_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_بەها(
            XKB_STATE_COMPONENT_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        أرجع is_supported_enum_بەها_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_A11_Y_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_KEYBOARD_CONTROL_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_STATE_MODE:
        أرجع is_supported_enum_بەها_mask(XKB_STATE_MODE_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_STATE_MATCH:
        أرجع is_supported_flag_بەها(
            XKB_STATE_MATCH_بەهاکان, false, بەها
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_MODE:
        أرجع is_supported_enum_بەها_mask(XKB_CONSUMED_MODE_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_MACHINE_BUILDER_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_MACHINE_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_بەها_mask(XKB_EVENT_TYPE_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_کلیل_DIRECTION:
        أرجع is_supported_enum_بەها_mask(XKB_کلیل_DIRECTION_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_POINTER_MOTION_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        أرجع is_supported_enum_بەها_mask(XKB_POINTER_BUTTON_STATE_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_EVENTS_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_بەها_mask(XKB_COMPOSE_FORMAT_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_COMPOSE_COMPILE_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_STATUS:
        أرجع is_supported_enum_بەها_mask(XKB_COMPOSE_STATUS_بەهاکان, بەها);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_بەها(
            XKB_COMPOSE_STATE_FLAGS_بەهاکان, true, بەها
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        أرجع is_supported_enum_بەها_mask(XKB_COMPOSE_FEED_RESULT_بەهاکان, بەها);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
