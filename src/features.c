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
is_supported_enum_ಮೌಲ್ಯ_mask(ಗಣನಾ xkb_enumerations_ಮೌಲ್ಯಗಳು ಮೌಲ್ಯಗಳು, uint32_t ಮೌಲ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ (ಮೌಲ್ಯ < UINT32_WIDTH && (ಮೌಲ್ಯಗಳು & (UINT32_C(1) << ಮೌಲ್ಯ)));
}

ಸ್ಥಿರ bool
is_supported_enum_ಮೌಲ್ಯ_array(ಧ್ರುವ uint32_t *ಮೌಲ್ಯಗಳು, size_t size, uint32_t ಮೌಲ್ಯ)
{
    ಪ್ರತಿ (size_t v = 0; v < size; v++) {
        ಯದಿ (ಮೌಲ್ಯಗಳು[v] == ಮೌಲ್ಯ) {
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_supported_flag_ಮೌಲ್ಯ(ಗಣನಾ xkb_enumerations_ಮೌಲ್ಯಗಳು ಮೌಲ್ಯಗಳು,
                        bool accept_zero, uint32_t ಮೌಲ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ (accept_zero || ಮೌಲ್ಯ) && ((uint32_t) ಮೌಲ್ಯಗಳು & ಮೌಲ್ಯ) == ಮೌಲ್ಯ;
}

bool
xkb_feature_supported(ಗಣನಾ xkb_feature feature, uint32_t ಮೌಲ್ಯ)
{
    ವಿಕಲ್ಪ (feature) {
    ಪಕ್ಷ XKB_FEATURE_ENUM_FEATURE:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_feature_ಮೌಲ್ಯಗಳು, ARRAY_SIZE(xkb_feature_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_STATUS:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_status_ಮೌಲ್ಯಗಳು, ARRAY_SIZE(xkb_status_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_CONTEXT_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_LOG_LEVEL:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_array(
            xkb_log_level_ಮೌಲ್ಯಗಳು, ARRAY_SIZE(xkb_log_level_ಮೌಲ್ಯಗಳು), ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYSYM_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_RMLVO_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_KEYMAP_FORMAT_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_SERIALIZE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_STATE_COMPONENT:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_STATE_COMPONENT_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_A11Y_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_A11_Y_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_KEYBOARD_CONTROL_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_STATE_MODE:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_STATE_MODE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_STATE_MATCH:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_STATE_MATCH_ಮೌಲ್ಯಗಳು, false, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_CONSUMED_MODE:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_CONSUMED_MODE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_MACHINE_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_MACHINE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_EVENT_TYPE:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_EVENT_TYPE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_ಕೀಲಿ_DIRECTION:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_ಕೀಲಿ_DIRECTION_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_POINTER_MOTION_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_POINTER_BUTTON_STATE_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_EVENTS_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_COMPOSE_FORMAT_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_COMPOSE_COMPILE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_COMPOSE_STATUS:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_COMPOSE_STATUS_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಪಕ್ಷ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ಪ್ರತ್ಯಾಗಮ is_supported_flag_ಮೌಲ್ಯ(
            XKB_COMPOSE_STATE_FLAGS_ಮೌಲ್ಯಗಳು, true, ಮೌಲ್ಯ
        );
    ಪಕ್ಷ XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        ಪ್ರತ್ಯಾಗಮ is_supported_enum_ಮೌಲ್ಯ_mask(XKB_COMPOSE_FEED_RESULT_ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯ);
    ಸ್ವಭಾವ:
        /* Invalid feature */
        ಪ್ರತ್ಯಾಗಮ false;
    }
}
