/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <stdint.h>

#समावेश "xkbcommon/xkbcommon-features.h"
#समावेश "features/enums.h"
#समावेश "utils.h"

अटल bool
is_supported_enum_मूल्य_mask(क्रमागत xkb_enumerations_मूल्यावली मूल्यावली, uint32_t मूल्य)
{
    वापस (मूल्य < UINT32_WIDTH && (मूल्यावली & (UINT32_C(1) << मूल्य)));
}

अटल bool
is_supported_enum_मूल्य_array(स्थिर uint32_t *मूल्यावली, माप_प्रकार आकार, uint32_t मूल्य)
{
    क्रम (माप_प्रकार v = 0; v < आकार; v++) {
        अगर (मूल्यावली[v] == मूल्य) {
            वापस true;
        }
    }
    वापस false;
}

अटल अंतरभूत bool
is_supported_flag_मूल्य(क्रमागत xkb_enumerations_मूल्यावली मूल्यावली,
                        bool accept_zero, uint32_t मूल्य)
{
    वापस (accept_zero || मूल्य) && ((uint32_t) मूल्यावली & मूल्य) == मूल्य;
}

bool
xkb_feature_supported(क्रमागत xkb_feature feature, uint32_t मूल्य)
{
    चयन (feature) {
    हाल XKB_FEATURE_ENUM_FEATURE:
        वापस is_supported_enum_मूल्य_array(
            xkb_feature_मूल्यावली, ARRAY_आकार(xkb_feature_मूल्यावली), मूल्य
        );
    हाल XKB_FEATURE_ENUM_STATUS:
        वापस is_supported_enum_मूल्य_array(
            xkb_status_मूल्यावली, ARRAY_आकार(xkb_status_मूल्यावली), मूल्य
        );
    हाल XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_CONTEXT_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_LOG_LEVEL:
        वापस is_supported_enum_मूल्य_array(
            xkb_log_level_मूल्यावली, ARRAY_आकार(xkb_log_level_मूल्यावली), मूल्य
        );
    हाल XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_KEYSYM_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_RMLVO_BUILDER_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        वापस is_supported_enum_मूल्य_mask(XKB_KEYMAP_FORMAT_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_KEYMAP_COMPILE_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_KEYMAP_SERIALIZE_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_KEYMAP_कुंजी_ITERATOR_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_KEYMAP_कुंजी_ITERATOR_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_STATE_COMPONENT:
        वापस is_supported_flag_मूल्य(
            XKB_STATE_COMPONENT_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        वापस is_supported_enum_मूल्य_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_A11Y_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_A11_Y_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_KEYBOARD_CONTROL_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_STATE_MODE:
        वापस is_supported_enum_मूल्य_mask(XKB_STATE_MODE_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_STATE_MATCH:
        वापस is_supported_flag_मूल्य(
            XKB_STATE_MATCH_मूल्यावली, false, मूल्य
        );
    हाल XKB_FEATURE_ENUM_CONSUMED_MODE:
        वापस is_supported_enum_मूल्य_mask(XKB_CONSUMED_MODE_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_MACHINE_BUILDER_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_MACHINE_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_MACHINE_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_EVENT_TYPE:
        वापस is_supported_enum_मूल्य_mask(XKB_EVENT_TYPE_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_कुंजी_DIRECTION:
        वापस is_supported_enum_मूल्य_mask(XKB_कुंजी_DIRECTION_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_POINTER_MOTION_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        वापस is_supported_enum_मूल्य_mask(XKB_POINTER_BUTTON_STATE_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_EVENTS_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_EVENTS_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        वापस is_supported_enum_मूल्य_mask(XKB_COMPOSE_FORMAT_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_COMPOSE_COMPILE_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_COMPOSE_STATUS:
        वापस is_supported_enum_मूल्य_mask(XKB_COMPOSE_STATUS_मूल्यावली, मूल्य);
    हाल XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        वापस is_supported_flag_मूल्य(
            XKB_COMPOSE_STATE_FLAGS_मूल्यावली, true, मूल्य
        );
    हाल XKB_FEATURE_ENUM_COMPOSE_FEED_परिणाम:
        वापस is_supported_enum_मूल्य_mask(XKB_COMPOSE_FEED_परिणाम_मूल्यावली, मूल्य);
    शेष:
        /* Invalid feature */
        वापस false;
    }
}
