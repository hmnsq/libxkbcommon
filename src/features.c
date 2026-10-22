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
is_supported_enum_قدر_نقاب(تعداد xkb_enumerations_اقدار اقدار, uint32_t قدر)
{
    أرجع (قدر < UINT32_چوڑائی && (اقدار & (UINT32_C(1) << قدر)));
}

ساكن bool
is_supported_enum_قدر_array(ثابت uint32_t *اقدار, ناپ_t ناپ, uint32_t قدر)
{
    لكل (ناپ_t v = 0; v < ناپ; v++) {
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
            xkb_feature_اقدار, ARRAY_ناپ(xkb_feature_اقدار), قدر
        );
    حالة XKB_FEATURE_ENUM_حالت:
        أرجع is_supported_enum_قدر_array(
            xkb_حالت_اقدار, ARRAY_ناپ(xkb_حالت_اقدار), قدر
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_CONTEXT_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_قدر_array(
            xkb_log_level_اقدار, ARRAY_ناپ(xkb_log_level_اقدار), قدر
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
        أرجع is_supported_enum_قدر_نقاب(XKB_KEYMAP_FORMAT_اقدار, قدر);
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
    حالة XKB_FEATURE_ENUM_LAYOUT_باہرلا_دا_RANGE_POLICY:
        أرجع is_supported_enum_قدر_نقاب(XKB_LAYOUT_باہرلا_دا_RANGE_POLICY_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_A11_Y_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYBOARD_CONTROL_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_STATE_منوال:
        أرجع is_supported_enum_قدر_نقاب(XKB_STATE_منوال_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_STATE_میل:
        أرجع is_supported_flag_قدر(
            XKB_STATE_میل_اقدار, false, قدر
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_منوال:
        أرجع is_supported_enum_قدر_نقاب(XKB_CONSUMED_منوال_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_BUILDER_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_قدر_نقاب(XKB_EVENT_TYPE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_کلید_DIRECTION:
        أرجع is_supported_enum_قدر_نقاب(XKB_کلید_DIRECTION_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_اشارہ_گر_MOTION_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_اشارہ_گر_MOTION_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_اشارہ_گر_BUTTON_STATE:
        أرجع is_supported_enum_قدر_نقاب(XKB_اشارہ_گر_BUTTON_STATE_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_EVENTS_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_FORMAT_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_COMPILE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_حالت:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_حالت_اقدار, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_STATE_FLAGS_اقدار, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_نتیجہ:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_FEED_نتیجہ_اقدار, قدر);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
