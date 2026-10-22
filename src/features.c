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
is_supported_enum_मानम्_mask(गणना xkb_enumerations_मानानि मानानि, uint32_t मानम्)
{
    पटिनिवत्त (मानम् < UINT32_WIDTH && (मानानि & (UINT32_C(1) << मानम्)));
}

स्थिर bool
is_supported_enum_मानम्_array(ध्रुव uint32_t *मानानि, size_t size, uint32_t मानम्)
{
    अत्थाय (size_t v = 0; v < size; v++) {
        सचे (मानानि[v] == मानम्) {
            पटिनिवत्त true;
        }
    }
    पटिनिवत्त false;
}

स्थिर अन्तःस्थ bool
is_supported_flag_मानम्(गणना xkb_enumerations_मानानि मानानि,
                        bool accept_zero, uint32_t मानम्)
{
    पटिनिवत्त (accept_zero || मानम्) && ((uint32_t) मानानि & मानम्) == मानम्;
}

bool
xkb_feature_supported(गणना xkb_feature feature, uint32_t मानम्)
{
    विकल्प (feature) {
    पक्ष XKB_FEATURE_ENUM_FEATURE:
        पटिनिवत्त is_supported_enum_मानम्_array(
            xkb_feature_मानानि, ARRAY_SIZE(xkb_feature_मानानि), मानम्
        );
    पक्ष XKB_FEATURE_ENUM_STATUS:
        पटिनिवत्त is_supported_enum_मानम्_array(
            xkb_status_मानानि, ARRAY_SIZE(xkb_status_मानानि), मानम्
        );
    पक्ष XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_CONTEXT_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_LOG_LEVEL:
        पटिनिवत्त is_supported_enum_मानम्_array(
            xkb_log_level_मानानि, ARRAY_SIZE(xkb_log_level_मानानि), मानम्
        );
    पक्ष XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_KEYSYM_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_RMLVO_BUILDER_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_KEYMAP_FORMAT_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_KEYMAP_COMPILE_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_KEYMAP_SERIALIZE_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_कुञ्चिका_ITERATOR_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_STATE_COMPONENT:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_STATE_COMPONENT_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_A11Y_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_A11_Y_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_KEYBOARD_CONTROL_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_STATE_MODE:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_STATE_MODE_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_STATE_MATCH:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_STATE_MATCH_मानानि, false, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_CONSUMED_MODE:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_CONSUMED_MODE_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_MACHINE_BUILDER_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_MACHINE_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_MACHINE_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_EVENT_TYPE:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_EVENT_TYPE_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_कुञ्चिका_DIRECTION:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_कुञ्चिका_DIRECTION_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_POINTER_MOTION_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_POINTER_BUTTON_STATE_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_EVENTS_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_EVENTS_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_COMPOSE_FORMAT_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_COMPOSE_COMPILE_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATUS:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_COMPOSE_STATUS_मानानि, मानम्);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        पटिनिवत्त is_supported_flag_मानम्(
            XKB_COMPOSE_STATE_FLAGS_मानानि, true, मानम्
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        पटिनिवत्त is_supported_enum_मानम्_mask(XKB_COMPOSE_FEED_RESULT_मानानि, मानम्);
    स्वभाव:
        /* Invalid feature */
        पटिनिवत्त false;
    }
}
