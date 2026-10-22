/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdint.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon-features.h"
#অন্তর্ভাৱ "features/enums.h"
#অন্তর্ভাৱ "utils.h"

স্থির bool
is_supported_enum_মান_mask(গণনা xkb_enumerations_মানসমূহ মানসমূহ, uint32_t মান)
{
    ঘূৰাই (মান < UINT32_WIDTH && (মানসমূহ & (UINT32_C(1) << মান)));
}

স্থির bool
is_supported_enum_মান_array(ধ্রুৱ uint32_t *মানসমূহ, size_t size, uint32_t মান)
{
    বাবে (size_t v = 0; v < size; v++) {
        যদি (মানসমূহ[v] == মান) {
            ঘূৰাই true;
        }
    }
    ঘূৰাই false;
}

স্থির অন্তঃস্থ bool
is_supported_flag_মান(গণনা xkb_enumerations_মানসমূহ মানসমূহ,
                        bool accept_zero, uint32_t মান)
{
    ঘূৰাই (accept_zero || মান) && ((uint32_t) মানসমূহ & মান) == মান;
}

bool
xkb_feature_supported(গণনা xkb_feature feature, uint32_t মান)
{
    ৱিকল্প (feature) {
    পক্ষ XKB_FEATURE_ENUM_FEATURE:
        ঘূৰাই is_supported_enum_মান_array(
            xkb_feature_মানসমূহ, ARRAY_SIZE(xkb_feature_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATUS:
        ঘূৰাই is_supported_enum_মান_array(
            xkb_status_মানসমূহ, ARRAY_SIZE(xkb_status_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_CONTEXT_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_LOG_LEVEL:
        ঘূৰাই is_supported_enum_মান_array(
            xkb_log_level_মানসমূহ, ARRAY_SIZE(xkb_log_level_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_KEYSYM_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_RMLVO_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ঘূৰাই is_supported_enum_মান_mask(XKB_KEYMAP_FORMAT_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_KEYMAP_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_KEYMAP_SERIALIZE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_চাবি_ITERATOR_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATE_COMPONENT:
        ঘূৰাই is_supported_flag_মান(
            XKB_STATE_COMPONENT_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        ঘূৰাই is_supported_enum_মান_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_A11Y_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_A11_Y_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_KEYBOARD_CONTROL_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATE_MODE:
        ঘূৰাই is_supported_enum_মান_mask(XKB_STATE_MODE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_STATE_MATCH:
        ঘূৰাই is_supported_flag_মান(
            XKB_STATE_MATCH_মানসমূহ, false, মান
        );
    পক্ষ XKB_FEATURE_ENUM_CONSUMED_MODE:
        ঘূৰাই is_supported_enum_মান_mask(XKB_CONSUMED_MODE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_MACHINE_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_MACHINE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_EVENT_TYPE:
        ঘূৰাই is_supported_enum_মান_mask(XKB_EVENT_TYPE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_চাবি_DIRECTION:
        ঘূৰাই is_supported_enum_মান_mask(XKB_চাবি_DIRECTION_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_POINTER_MOTION_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        ঘূৰাই is_supported_enum_মান_mask(XKB_POINTER_BUTTON_STATE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_EVENTS_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ঘূৰাই is_supported_enum_মান_mask(XKB_COMPOSE_FORMAT_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_COMPOSE_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_STATUS:
        ঘূৰাই is_supported_enum_মান_mask(XKB_COMPOSE_STATUS_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ঘূৰাই is_supported_flag_মান(
            XKB_COMPOSE_STATE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        ঘূৰাই is_supported_enum_মান_mask(XKB_COMPOSE_FEED_RESULT_মানসমূহ, মান);
    স্ৱভাৱ:
        /* Invalid feature */
        ঘূৰাই false;
    }
}
