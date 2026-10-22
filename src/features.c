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
is_supported_enum_ارزښت_ماسک(تعداد xkb_enumerations_ارزښتونه ارزښتونه, uint32_t ارزښت)
{
    أرجع (ارزښت < UINT32_پلنوالی && (ارزښتونه & (UINT32_C(1) << ارزښت)));
}

ساكن bool
is_supported_enum_ارزښت_array(ثابت uint32_t *ارزښتونه, اندازه_t اندازه, uint32_t ارزښت)
{
    لكل (اندازه_t v = 0; v < اندازه; v++) {
        إذا (ارزښتونه[v] == ارزښت) {
            أرجع true;
        }
    }
    أرجع false;
}

ساكن مضمن bool
is_supported_flag_ارزښت(تعداد xkb_enumerations_ارزښتونه ارزښتونه,
                        bool accept_zero, uint32_t ارزښت)
{
    أرجع (accept_zero || ارزښت) && ((uint32_t) ارزښتونه & ارزښت) == ارزښت;
}

bool
xkb_feature_supported(تعداد xkb_feature feature, uint32_t ارزښت)
{
    بدل (feature) {
    حالة XKB_FEATURE_ENUM_FEATURE:
        أرجع is_supported_enum_ارزښت_array(
            xkb_feature_ارزښتونه, ARRAY_اندازه(xkb_feature_ارزښتونه), ارزښت
        );
    حالة XKB_FEATURE_ENUM_حالت:
        أرجع is_supported_enum_ارزښت_array(
            xkb_حالت_ارزښتونه, ARRAY_اندازه(xkb_حالت_ارزښتونه), ارزښت
        );
    حالة XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_CONTEXT_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_LOG_LEVEL:
        أرجع is_supported_enum_ارزښت_array(
            xkb_log_level_ارزښتونه, ARRAY_اندازه(xkb_log_level_ارزښتونه), ارزښت
        );
    حالة XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_KEYSYM_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_RMLVO_BUILDER_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_KEYMAP_FORMAT_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_KEYMAP_COMPILE_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_KEYMAP_SERIALIZE_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_KEYMAP_کلی_ITERATOR_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_KEYMAP_کلی_ITERATOR_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_STATE_COMPONENT:
        أرجع is_supported_flag_ارزښت(
            XKB_STATE_COMPONENT_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_LAYOUT_بهر_د_RANGE_POLICY:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_LAYOUT_بهر_د_RANGE_POLICY_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_A11Y_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_A11_Y_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_KEYBOARD_CONTROL_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_STATE_ډېرپېښ:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_STATE_ډېرپېښ_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_STATE_سمون:
        أرجع is_supported_flag_ارزښت(
            XKB_STATE_سمون_ارزښتونه, false, ارزښت
        );
    حالة XKB_FEATURE_ENUM_CONSUMED_ډېرپېښ:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_CONSUMED_ډېرپېښ_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_MACHINE_BUILDER_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_MACHINE_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_MACHINE_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_EVENT_TYPE:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_EVENT_TYPE_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_کلی_DIRECTION:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_کلی_DIRECTION_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_اشاره_ګر_MOTION_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_اشاره_ګر_MOTION_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_اشاره_ګر_BUTTON_STATE:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_اشاره_ګر_BUTTON_STATE_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_EVENTS_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_EVENTS_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_COMPOSE_FORMAT_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_COMPOSE_COMPILE_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_حالت:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_COMPOSE_حالت_ارزښتونه, ارزښت);
    حالة XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        أرجع is_supported_flag_ارزښت(
            XKB_COMPOSE_STATE_FLAGS_ارزښتونه, true, ارزښت
        );
    حالة XKB_FEATURE_ENUM_COMPOSE_FEED_پایله:
        أرجع is_supported_enum_ارزښت_ماسک(XKB_COMPOSE_FEED_پایله_ارزښتونه, ارزښت);
    افتراضي:
        /* Invalid feature */
        أرجع false;
    }
}
