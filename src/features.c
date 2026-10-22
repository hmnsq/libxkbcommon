/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-features.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಸ್ಥಿರ bool
is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(ಎಣಿಕೆ xkb_enumerations_ಮೌಲ್ಯಗಳು ಮೌಲ್ಯಗಳು, uint32_t ಮೌಲ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ಮೌಲ್ಯ < UINT32_ಅಗಲ && (ಮೌಲ್ಯಗಳು & (UINT32_C(1) << ಮೌಲ್ಯ)));
}

ಸ್ಥಿರ bool
is_supported_enum_ಮೌಲ್ಯ_array(ಸ್ಥಿರಾಂಕ uint32_t *ಮೌಲ್ಯಗಳು, ಗಾತ್ರ_t ಗಾತ್ರ_2, uint32_t ಮೌಲ್ಯ)
{
    ಗಾಗಿ (ಗಾತ್ರ_t v = 0; v < ಗಾತ್ರ_2; v++) {
        ಆದರೆ (ಮೌಲ್ಯಗಳು[v] == ಮೌಲ್ಯ) {
            ಹಿಂತಿರುಗಿ true;
        }
    }
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_supported_flag_ಮೌಲ್ಯ(ಎಣಿಕೆ xkb_enumerations_ಮೌಲ್ಯಗಳು ಮೌಲ್ಯಗಳು,
                        bool accept_zero, uint32_t ಮೌಲ್ಯ)
{
    ಹಿಂತಿರುಗಿ (accept_zero || ಮೌಲ್ಯ) && ((uint32_t) ಮೌಲ್ಯಗಳು & ಮೌಲ್ಯ) == ಮೌಲ್ಯ;
}

bool
xkb_feature_supported(ಎಣಿಕೆ xkb_feature feature, uint32_t ಮೌಲ್ಯ)
{
    ಆಯ್ಕೆ (feature) {
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_FEATURE:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_feature_ಮೌಲ್ಯಗಳು, ARRAY_ಗಾತ್ರ(xkb_feature_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_ಸ್ಥಿತಿ:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು, ARRAY_ಗಾತ್ರ(xkb_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_CONTEXT_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_LOG_LEVEL:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_log_level_ಮೌಲ್ಯಗಳು, ARRAY_ಗಾತ್ರ(xkb_log_level_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYSYM_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_RMLVO_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_KEYMAP_FORMAT_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_SERIALIZE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_STATE_COMPONENT:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_STATE_COMPONENT_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_A11Y_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_A11_Y_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYBOARD_CONTROL_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_STATE_ಬಹುಲಕ:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_STATE_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_STATE_ಹೊಂದಿಕೆ:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_STATE_ಹೊಂದಿಕೆ_ಮೌಲ್ಯಗಳು, false, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_CONSUMED_ಬಹುಲಕ:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_CONSUMED_ಬಹುಲಕ_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_MACHINE_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_MACHINE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_EVENT_TYPE:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_EVENT_TYPE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_ಕೀಲಿ_DIRECTION:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_ಕೀಲಿ_DIRECTION_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_ಸೂಚಕ_MOTION_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_ಸೂಚಕ_MOTION_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_ಸೂಚಕ_BUTTON_STATE:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_ಸೂಚಕ_BUTTON_STATE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_EVENTS_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_COMPOSE_FORMAT_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_COMPOSE_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_COMPOSE_ಸ್ಥಿತಿ:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_COMPOSE_ಸ್ಥಿತಿ_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ಹಿಂತಿರುಗಿ is_supported_flag_ಮೌಲ್ಯ(
            XKB_COMPOSE_STATE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಸ್ಥಿತಿ XKB_FEATURE_ENUM_COMPOSE_FEED_ಫಲಿತಾಂಶ:
        ಹಿಂತಿರುಗಿ is_supported_enum_ಮೌಲ್ಯ_ಮುಖವಾಡ(XKB_COMPOSE_FEED_ಫಲಿತಾಂಶ_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪೂರ್ವನಿಯೋಜಿತ:
        /* Invalid feature */
        ಹಿಂತಿರುಗಿ false;
    }
}
