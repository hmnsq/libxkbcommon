/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdint.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-features.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "utils.h"

நிலையான bool
is_supported_enum_மதிப்பு_mask(எண்ணிக்கை xkb_enumerations_மதிப்புகள் மதிப்புகள், uint32_t மதிப்பு)
{
    திரும்பு (மதிப்பு < UINT32_WIDTH && (மதிப்புகள் & (UINT32_C(1) << மதிப்பு)));
}

நிலையான bool
is_supported_enum_மதிப்பு_array(மாறிலி uint32_t *மதிப்புகள், size_t size, uint32_t மதிப்பு)
{
    க்கு (size_t v = 0; v < size; v++) {
        எனில் (மதிப்புகள்[v] == மதிப்பு) {
            திரும்பு true;
        }
    }
    திரும்பு false;
}

நிலையான உள்வரி bool
is_supported_flag_மதிப்பு(எண்ணிக்கை xkb_enumerations_மதிப்புகள் மதிப்புகள்,
                        bool accept_zero, uint32_t மதிப்பு)
{
    திரும்பு (accept_zero || மதிப்பு) && ((uint32_t) மதிப்புகள் & மதிப்பு) == மதிப்பு;
}

bool
xkb_feature_supported(எண்ணிக்கை xkb_feature feature, uint32_t மதிப்பு)
{
    தேர்வு (feature) {
    நிலை XKB_FEATURE_ENUM_FEATURE:
        திரும்பு is_supported_enum_மதிப்பு_array(
            xkb_feature_மதிப்புகள், ARRAY_SIZE(xkb_feature_மதிப்புகள்), மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_STATUS:
        திரும்பு is_supported_enum_மதிப்பு_array(
            xkb_status_மதிப்புகள், ARRAY_SIZE(xkb_status_மதிப்புகள்), மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_CONTEXT_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_LOG_LEVEL:
        திரும்பு is_supported_enum_மதிப்பு_array(
            xkb_log_level_மதிப்புகள், ARRAY_SIZE(xkb_log_level_மதிப்புகள்), மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_KEYSYM_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_RMLVO_BUILDER_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_KEYMAP_FORMAT_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_KEYMAP_COMPILE_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_KEYMAP_SERIALIZE_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_KEYMAP_சாவி_ITERATOR_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_KEYMAP_சாவி_ITERATOR_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_STATE_COMPONENT:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_STATE_COMPONENT_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_A11Y_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_A11_Y_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_KEYBOARD_CONTROL_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_STATE_MODE:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_STATE_MODE_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_STATE_MATCH:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_STATE_MATCH_மதிப்புகள், false, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_CONSUMED_MODE:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_CONSUMED_MODE_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_MACHINE_BUILDER_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_MACHINE_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_MACHINE_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_EVENT_TYPE:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_EVENT_TYPE_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_சாவி_DIRECTION:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_சாவி_DIRECTION_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_POINTER_MOTION_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_POINTER_BUTTON_STATE_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_EVENTS_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_EVENTS_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_COMPOSE_FORMAT_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_COMPOSE_COMPILE_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_COMPOSE_STATUS:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_COMPOSE_STATUS_மதிப்புகள், மதிப்பு);
    நிலை XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        திரும்பு is_supported_flag_மதிப்பு(
            XKB_COMPOSE_STATE_FLAGS_மதிப்புகள், true, மதிப்பு
        );
    நிலை XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        திரும்பு is_supported_enum_மதிப்பு_mask(XKB_COMPOSE_FEED_RESULT_மதிப்புகள், மதிப்பு);
    இயல்பு:
        /* Invalid feature */
        திரும்பு false;
    }
}
