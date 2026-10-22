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
is_supported_enum_मूल्य_mask(गणना xkb_enumerations_मूल्ये मूल्ये, uint32_t मूल्य)
{
    परत (मूल्य < UINT32_WIDTH && (मूल्ये & (UINT32_C(1) << मूल्य)));
}

स्थिर bool
is_supported_enum_मूल्य_array(स्थिरांक uint32_t *मूल्ये, माप_प्रकार size, uint32_t मूल्य)
{
    साठी (माप_प्रकार v = 0; v < size; v++) {
        जर (मूल्ये[v] == मूल्य) {
            परत true;
        }
    }
    परत false;
}

स्थिर ओळीत bool
is_supported_flag_मूल्य(गणना xkb_enumerations_मूल्ये मूल्ये,
                        bool accept_zero, uint32_t मूल्य)
{
    परत (accept_zero || मूल्य) && ((uint32_t) मूल्ये & मूल्य) == मूल्य;
}

bool
xkb_feature_supported(गणना xkb_feature feature, uint32_t मूल्य)
{
    निवड (feature) {
    स्थिती XKB_FEATURE_ENUM_FEATURE:
        परत is_supported_enum_मूल्य_array(
            xkb_feature_मूल्ये, ARRAY_SIZE(xkb_feature_मूल्ये), मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_STATUS:
        परत is_supported_enum_मूल्य_array(
            xkb_status_मूल्ये, ARRAY_SIZE(xkb_status_मूल्ये), मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_CONTEXT_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_LOG_LEVEL:
        परत is_supported_enum_मूल्य_array(
            xkb_log_level_मूल्ये, ARRAY_SIZE(xkb_log_level_मूल्ये), मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_KEYSYM_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_RMLVO_BUILDER_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        परत is_supported_enum_मूल्य_mask(XKB_KEYMAP_FORMAT_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_KEYMAP_COMPILE_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_KEYMAP_SERIALIZE_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_KEYMAP_किल्ली_ITERATOR_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_KEYMAP_किल्ली_ITERATOR_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_STATE_COMPONENT:
        परत is_supported_flag_मूल्य(
            XKB_STATE_COMPONENT_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        परत is_supported_enum_मूल्य_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_A11Y_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_A11_Y_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_KEYBOARD_CONTROL_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_STATE_MODE:
        परत is_supported_enum_मूल्य_mask(XKB_STATE_MODE_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_STATE_MATCH:
        परत is_supported_flag_मूल्य(
            XKB_STATE_MATCH_मूल्ये, false, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_CONSUMED_MODE:
        परत is_supported_enum_मूल्य_mask(XKB_CONSUMED_MODE_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_MACHINE_BUILDER_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_MACHINE_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_MACHINE_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_EVENT_TYPE:
        परत is_supported_enum_मूल्य_mask(XKB_EVENT_TYPE_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_किल्ली_DIRECTION:
        परत is_supported_enum_मूल्य_mask(XKB_किल्ली_DIRECTION_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_POINTER_MOTION_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        परत is_supported_enum_मूल्य_mask(XKB_POINTER_BUTTON_STATE_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_EVENTS_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_EVENTS_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        परत is_supported_enum_मूल्य_mask(XKB_COMPOSE_FORMAT_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_COMPOSE_COMPILE_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_COMPOSE_STATUS:
        परत is_supported_enum_मूल्य_mask(XKB_COMPOSE_STATUS_मूल्ये, मूल्य);
    स्थिती XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        परत is_supported_flag_मूल्य(
            XKB_COMPOSE_STATE_FLAGS_मूल्ये, true, मूल्य
        );
    स्थिती XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        परत is_supported_enum_मूल्य_mask(XKB_COMPOSE_FEED_RESULT_मूल्ये, मूल्य);
    पूर्वनिर्धारित:
        /* Invalid feature */
        परत false;
    }
}
