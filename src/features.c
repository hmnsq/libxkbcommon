/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon-features.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "utils.h"

स्थिर bool
is_supported_enum_मूल्य_mask(गणना xkb_enumerations_मूल्यावली मूल्यावली, uint32_t मूल्य)
{
    प्रत्यागम (मूल्य < UINT32_WIDTH && (मूल्यावली & (UINT32_C(1) << मूल्य)));
}

स्थिर bool
is_supported_enum_मूल्य_array(ध्रुव uint32_t *मूल्यावली, size_t size, uint32_t मूल्य)
{
    प्रति (size_t v = 0; v < size; v++) {
        यदि (मूल्यावली[v] == मूल्य) {
            प्रत्यागम true;
        }
    }
    प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
is_supported_flag_मूल्य(गणना xkb_enumerations_मूल्यावली मूल्यावली,
                        bool accept_zero, uint32_t मूल्य)
{
    प्रत्यागम (accept_zero || मूल्य) && ((uint32_t) मूल्यावली & मूल्य) == मूल्य;
}

bool
xkb_feature_supported(गणना xkb_feature feature, uint32_t मूल्य)
{
    विकल्प (feature) {
    पक्ष XKB_FEATURE_ENUM_FEATURE:
        प्रत्यागम is_supported_enum_मूल्य_array(
            xkb_feature_मूल्यावली, ARRAY_SIZE(xkb_feature_मूल्यावली), मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_STATUS:
        प्रत्यागम is_supported_enum_मूल्य_array(
            xkb_status_मूल्यावली, ARRAY_SIZE(xkb_status_मूल्यावली), मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_CONTEXT_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_LOG_LEVEL:
        प्रत्यागम is_supported_enum_मूल्य_array(
            xkb_log_level_मूल्यावली, ARRAY_SIZE(xkb_log_level_मूल्यावली), मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_KEYSYM_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_RMLVO_BUILDER_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_KEYMAP_FORMAT_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_KEYMAP_COMPILE_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_KEYMAP_SERIALIZE_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_कुंजी_ITERATOR_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_KEYMAP_कुंजी_ITERATOR_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_STATE_COMPONENT:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_STATE_COMPONENT_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_A11Y_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_A11_Y_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_KEYBOARD_CONTROL_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_STATE_MODE:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_STATE_MODE_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_STATE_MATCH:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_STATE_MATCH_मूल्यावली, false, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_CONSUMED_MODE:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_CONSUMED_MODE_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_MACHINE_BUILDER_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_MACHINE_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_MACHINE_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_EVENT_TYPE:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_EVENT_TYPE_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_कुंजी_DIRECTION:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_कुंजी_DIRECTION_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_POINTER_MOTION_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_POINTER_BUTTON_STATE_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_EVENTS_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_EVENTS_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_COMPOSE_FORMAT_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_COMPOSE_COMPILE_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATUS:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_COMPOSE_STATUS_मूल्यावली, मूल्य);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        प्रत्यागम is_supported_flag_मूल्य(
            XKB_COMPOSE_STATE_FLAGS_मूल्यावली, true, मूल्य
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        प्रत्यागम is_supported_enum_मूल्य_mask(XKB_COMPOSE_FEED_RESULT_मूल्यावली, मूल्य);
    स्वभाव:
        /* Invalid feature */
        प्रत्यागम false;
    }
}
