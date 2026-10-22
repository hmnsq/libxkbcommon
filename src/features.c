/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-features.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਸਥਿਰ bool
is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(ਗਿਣਤੀ xkb_enumerations_ਮੁੱਲਾਂ ਮੁੱਲਾਂ, uint32_t ਮੁੱਲ)
{
    ਵਾਪਸ (ਮੁੱਲ < UINT32_ਚੌੜਾਈ && (ਮੁੱਲਾਂ & (UINT32_C(1) << ਮੁੱਲ)));
}

ਸਥਿਰ bool
is_supported_enum_ਮੁੱਲ_array(ਅਚਲ uint32_t *ਮੁੱਲਾਂ, ਮਾਪ_t ਮਾਪ, uint32_t ਮੁੱਲ)
{
    ਲਈ (ਮਾਪ_t v = 0; v < ਮਾਪ; v++) {
        ਜੇ (ਮੁੱਲਾਂ[v] == ਮੁੱਲ) {
            ਵਾਪਸ true;
        }
    }
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_supported_flag_ਮੁੱਲ(ਗਿਣਤੀ xkb_enumerations_ਮੁੱਲਾਂ ਮੁੱਲਾਂ,
                        bool accept_zero, uint32_t ਮੁੱਲ)
{
    ਵਾਪਸ (accept_zero || ਮੁੱਲ) && ((uint32_t) ਮੁੱਲਾਂ & ਮੁੱਲ) == ਮੁੱਲ;
}

bool
xkb_feature_supported(ਗਿਣਤੀ xkb_feature feature, uint32_t ਮੁੱਲ)
{
    ਚੋਣ (feature) {
    ਹਾਲਤ XKB_FEATURE_ENUM_FEATURE:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_array(
            xkb_feature_ਮੁੱਲਾਂ, ARRAY_ਮਾਪ(xkb_feature_ਮੁੱਲਾਂ), ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_ਸਥਿਤੀ:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_array(
            xkb_ਸਥਿਤੀ_ਮੁੱਲਾਂ, ARRAY_ਮਾਪ(xkb_ਸਥਿਤੀ_ਮੁੱਲਾਂ), ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_CONTEXT_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_LOG_LEVEL:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_array(
            xkb_log_level_ਮੁੱਲਾਂ, ARRAY_ਮਾਪ(xkb_log_level_ਮੁੱਲਾਂ), ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_KEYSYM_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_RMLVO_BUILDER_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_KEYMAP_FORMAT_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_KEYMAP_COMPILE_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_KEYMAP_SERIALIZE_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_STATE_COMPONENT:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_STATE_COMPONENT_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_A11Y_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_A11_Y_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_KEYBOARD_CONTROL_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_STATE_ਬਹੁਲਕ:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_STATE_ਬਹੁਲਕ_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_STATE_ਮੇਲ:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_STATE_ਮੇਲ_ਮੁੱਲਾਂ, false, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_CONSUMED_ਬਹੁਲਕ:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_CONSUMED_ਬਹੁਲਕ_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_MACHINE_BUILDER_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_MACHINE_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_EVENT_TYPE:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_EVENT_TYPE_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_ਕੁੰਜੀ_DIRECTION:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_ਕੁੰਜੀ_DIRECTION_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_ਸੂਚਕ_MOTION_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_ਸੂਚਕ_MOTION_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_ਸੂਚਕ_BUTTON_STATE:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_ਸੂਚਕ_BUTTON_STATE_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_EVENTS_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_COMPOSE_FORMAT_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_COMPOSE_COMPILE_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_COMPOSE_ਸਥਿਤੀ:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_COMPOSE_ਸਥਿਤੀ_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਹਾਲਤ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ਵਾਪਸ is_supported_flag_ਮੁੱਲ(
            XKB_COMPOSE_STATE_FLAGS_ਮੁੱਲਾਂ, true, ਮੁੱਲ
        );
    ਹਾਲਤ XKB_FEATURE_ENUM_COMPOSE_FEED_ਨਤੀਜਾ:
        ਵਾਪਸ is_supported_enum_ਮੁੱਲ_ਨਕਾਬ(XKB_COMPOSE_FEED_ਨਤੀਜਾ_ਮੁੱਲਾਂ, ਮੁੱਲ);
    ਮੂਲ:
        /* Invalid feature */
        ਵਾਪਸ false;
    }
}
