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
is_supported_enum_मान_mask(गणना xkb_enumerations_मानहरू मानहरू, uint32_t मान)
{
    फर्काऊ (मान < UINT32_WIDTH && (मानहरू & (UINT32_C(1) << मान)));
}

स्थिर bool
is_supported_enum_मान_array(अचर uint32_t *मानहरू, माप_प्रकार size, uint32_t मान)
{
    लागि (माप_प्रकार v = 0; v < size; v++) {
        यदि (मानहरू[v] == मान) {
            फर्काऊ true;
        }
    }
    फर्काऊ false;
}

स्थिर अन्तःस्थ bool
is_supported_flag_मान(गणना xkb_enumerations_मानहरू मानहरू,
                        bool accept_zero, uint32_t मान)
{
    फर्काऊ (accept_zero || मान) && ((uint32_t) मानहरू & मान) == मान;
}

bool
xkb_feature_supported(गणना xkb_feature feature, uint32_t मान)
{
    विकल्प (feature) {
    पक्ष XKB_FEATURE_ENUM_FEATURE:
        फर्काऊ is_supported_enum_मान_array(
            xkb_feature_मानहरू, ARRAY_SIZE(xkb_feature_मानहरू), मान
        );
    पक्ष XKB_FEATURE_ENUM_STATUS:
        फर्काऊ is_supported_enum_मान_array(
            xkb_status_मानहरू, ARRAY_SIZE(xkb_status_मानहरू), मान
        );
    पक्ष XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_CONTEXT_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_LOG_LEVEL:
        फर्काऊ is_supported_enum_मान_array(
            xkb_log_level_मानहरू, ARRAY_SIZE(xkb_log_level_मानहरू), मान
        );
    पक्ष XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_KEYSYM_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_RMLVO_BUILDER_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        फर्काऊ is_supported_enum_मान_mask(XKB_KEYMAP_FORMAT_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_KEYMAP_COMPILE_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_KEYMAP_SERIALIZE_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_KEYMAP_साँचो_ITERATOR_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_KEYMAP_साँचो_ITERATOR_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_STATE_COMPONENT:
        फर्काऊ is_supported_flag_मान(
            XKB_STATE_COMPONENT_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        फर्काऊ is_supported_enum_मान_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_A11Y_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_A11_Y_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_KEYBOARD_CONTROL_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_STATE_MODE:
        फर्काऊ is_supported_enum_मान_mask(XKB_STATE_MODE_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_STATE_MATCH:
        फर्काऊ is_supported_flag_मान(
            XKB_STATE_MATCH_मानहरू, false, मान
        );
    पक्ष XKB_FEATURE_ENUM_CONSUMED_MODE:
        फर्काऊ is_supported_enum_मान_mask(XKB_CONSUMED_MODE_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_MACHINE_BUILDER_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_MACHINE_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_MACHINE_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_EVENT_TYPE:
        फर्काऊ is_supported_enum_मान_mask(XKB_EVENT_TYPE_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_साँचो_DIRECTION:
        फर्काऊ is_supported_enum_मान_mask(XKB_साँचो_DIRECTION_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_POINTER_MOTION_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        फर्काऊ is_supported_enum_मान_mask(XKB_POINTER_BUTTON_STATE_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_EVENTS_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_EVENTS_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        फर्काऊ is_supported_enum_मान_mask(XKB_COMPOSE_FORMAT_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_COMPOSE_COMPILE_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATUS:
        फर्काऊ is_supported_enum_मान_mask(XKB_COMPOSE_STATUS_मानहरू, मान);
    पक्ष XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        फर्काऊ is_supported_flag_मान(
            XKB_COMPOSE_STATE_FLAGS_मानहरू, true, मान
        );
    पक्ष XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        फर्काऊ is_supported_enum_मान_mask(XKB_COMPOSE_FEED_RESULT_मानहरू, मान);
    स्वभाव:
        /* Invalid feature */
        फर्काऊ false;
    }
}
