/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdint.h>

#অন্তর্ভাব "xkbcommon/xkbcommon-features.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "utils.h"

স্থির bool
is_supported_enum_মান_mask(গণনা xkb_enumerations_মানসমূহ মানসমূহ, uint32_t মান)
{
    প্রত্যাগম (মান < UINT32_WIDTH && (মানসমূহ & (UINT32_C(1) << মান)));
}

স্থির bool
is_supported_enum_মান_array(ধ্রুব uint32_t *মানসমূহ, size_t size, uint32_t মান)
{
    প্রতি (size_t v = 0; v < size; v++) {
        যদি (মানসমূহ[v] == মান) {
            প্রত্যাগম true;
        }
    }
    প্রত্যাগম false;
}

স্থির অন্তঃস্থ bool
is_supported_flag_মান(গণনা xkb_enumerations_মানসমূহ মানসমূহ,
                        bool accept_zero, uint32_t মান)
{
    প্রত্যাগম (accept_zero || মান) && ((uint32_t) মানসমূহ & মান) == মান;
}

bool
xkb_feature_supported(গণনা xkb_feature feature, uint32_t মান)
{
    বিকল্প (feature) {
    পক্ষ XKB_FEATURE_ENUM_FEATURE:
        প্রত্যাগম is_supported_enum_মান_array(
            xkb_feature_মানসমূহ, ARRAY_SIZE(xkb_feature_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATUS:
        প্রত্যাগম is_supported_enum_মান_array(
            xkb_status_মানসমূহ, ARRAY_SIZE(xkb_status_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_CONTEXT_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_LOG_LEVEL:
        প্রত্যাগম is_supported_enum_মান_array(
            xkb_log_level_মানসমূহ, ARRAY_SIZE(xkb_log_level_মানসমূহ), মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_KEYSYM_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_RMLVO_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_KEYMAP_FORMAT_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_KEYMAP_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_KEYMAP_SERIALIZE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYMAP_চাবি_ITERATOR_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATE_COMPONENT:
        প্রত্যাগম is_supported_flag_মান(
            XKB_STATE_COMPONENT_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_A11Y_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_A11_Y_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_KEYBOARD_CONTROL_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_STATE_MODE:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_STATE_MODE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_STATE_MATCH:
        প্রত্যাগম is_supported_flag_মান(
            XKB_STATE_MATCH_মানসমূহ, false, মান
        );
    পক্ষ XKB_FEATURE_ENUM_CONSUMED_MODE:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_CONSUMED_MODE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_MACHINE_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_MACHINE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_EVENT_TYPE:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_EVENT_TYPE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_চাবি_DIRECTION:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_চাবি_DIRECTION_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_POINTER_MOTION_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_POINTER_BUTTON_STATE_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_EVENTS_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_COMPOSE_FORMAT_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_COMPOSE_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_STATUS:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_COMPOSE_STATUS_মানসমূহ, মান);
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        প্রত্যাগম is_supported_flag_মান(
            XKB_COMPOSE_STATE_FLAGS_মানসমূহ, true, মান
        );
    পক্ষ XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        প্রত্যাগম is_supported_enum_মান_mask(XKB_COMPOSE_FEED_RESULT_মানসমূহ, মান);
    স্বভাব:
        /* Invalid feature */
        প্রত্যাগম false;
    }
}
