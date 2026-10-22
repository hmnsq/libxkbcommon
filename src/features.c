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
is_supported_enum_قدر_نقاب(تعداد xkb_enumerations_قدرون قدرون, uint32_t قدر)
{
    أرجع (قدر < UINT32_ويڪر && (قدرون & (UINT32_C(1) << قدر)));
}

ساكن bool
is_supported_enum_قدر_array(ثابت uint32_t *قدرون, سائيز_t سائيز, uint32_t قدر)
{
    لكل (سائيز_t v = 0; v < سائيز; v++) {
        إذا (قدرون[v] == قدر) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_قدر(تعداد xkb_enumerations_قدرون قدرون,
                        bool accept_zero, uint32_t قدر)
{
    أرجع (accept_zero || قدر) && ((uint32_t) قدرون & قدر) == قدر;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t قدر)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_قدر_array(
            xkb_feature_قدرون, ARRAY_سائيز(xkb_feature_قدرون), قدر
        );
    حالة XKB_FEATURE_ENUM_حالت:
        أرجع is_supported_enum_قدر_array(
            xkb_حالت_قدرون, ARRAY_سائيز(xkb_حالت_قدرون), قدر
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_CONTEXT_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_قدر_array(
            xkb_log_level_قدرون, ARRAY_سائيز(xkb_log_level_قدرون), قدر
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYSYM_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_RMLVO_BUILDER_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_قدر_نقاب(XKB_KEYMAP_FORMAT_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_COMPILE_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_SERIALIZE_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_ڪنجي_ITERATOR_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYMAP_ڪنجي_ITERATOR_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_قدر(
            XKB_STATE_COMPONENT_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_ٻاهريون_جو_RANGE_POLICY:
        أرجع is_supported_enum_قدر_نقاب(XKB_LAYOUT_ٻاهريون_جو_RANGE_POLICY_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_A11_Y_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_KEYBOARD_CONTROL_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_STATE_گهڻائي:
        أرجع is_supported_enum_قدر_نقاب(XKB_STATE_گهڻائي_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_STATE_ميلاپ:
        أرجع is_supported_flag_قدر(
            XKB_STATE_ميلاپ_قدرون, false, قدر
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_گهڻائي:
        أرجع is_supported_enum_قدر_نقاب(XKB_CONSUMED_گهڻائي_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_BUILDER_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_MACHINE_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_قدر_نقاب(XKB_EVENT_TYPE_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_ڪنجي_DIRECTION:
        أرجع is_supported_enum_قدر_نقاب(XKB_ڪنجي_DIRECTION_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_اشارو_MOTION_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_اشارو_MOTION_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_اشارو_BUTTON_STATE:
        أرجع is_supported_enum_قدر_نقاب(XKB_اشارو_BUTTON_STATE_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_EVENTS_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_FORMAT_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_COMPILE_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_حالت:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_حالت_قدرون, قدر);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_قدر(
            XKB_COMPOSE_STATE_FLAGS_قدرون, true, قدر
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_نتيجو:
        أرجع is_supported_enum_قدر_نقاب(XKB_COMPOSE_FEED_نتيجو_قدرون, قدر);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
