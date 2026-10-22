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
    ফেরত (মান < UINT32_WIDTH && (মানসমূহ & (UINT32_C(1) << মান)));
}

স্থির bool
is_supported_enum_মান_array(ধ্রুবক uint32_t *মানসমূহ, আকার_t আকার_2, uint32_t মান)
{
    জন্য (আকার_t v = 0; v < আকার_2; v++) {
        যদি (মানসমূহ[v] == মান) {
            ফেরত true;
        }
    }
    ফেরত false;
}

স্থির অন্তঃরেখ bool
is_supported_flag_মান(গণনা xkb_enumerations_মানসমূহ মানসমূহ,
                        bool accept_zero, uint32_t মান)
{
    ফেরত (accept_zero || মান) && ((uint32_t) মানসমূহ & মান) == মান;
}

bool
xkb_feature_supported(গণনা xkb_feature feature, uint32_t মান)
{
    নির্বাচন (feature) {
    ক্ষেত্র XKB_FEATURE_ENUM_FEATURE:
        ফেরত is_supported_enum_মান_array(
            xkb_feature_মানসমূহ, ARRAY_আকার(xkb_feature_মানসমূহ), মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_STATUS:
        ফেরত is_supported_enum_মান_array(
            xkb_status_মানসমূহ, ARRAY_আকার(xkb_status_মানসমূহ), মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_CONTEXT_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_LOG_LEVEL:
        ফেরত is_supported_enum_মান_array(
            xkb_log_level_মানসমূহ, ARRAY_আকার(xkb_log_level_মানসমূহ), মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_KEYSYM_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_RMLVO_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ফেরত is_supported_enum_মান_mask(XKB_KEYMAP_FORMAT_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_KEYMAP_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_KEYMAP_SERIALIZE_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_KEYMAP_চাবি_ITERATOR_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_STATE_COMPONENT:
        ফেরত is_supported_flag_মান(
            XKB_STATE_COMPONENT_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        ফেরত is_supported_enum_মান_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_A11Y_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_A11_Y_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_KEYBOARD_CONTROL_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_STATE_MODE:
        ফেরত is_supported_enum_মান_mask(XKB_STATE_MODE_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_STATE_MATCH:
        ফেরত is_supported_flag_মান(
            XKB_STATE_MATCH_মানসমূহ, false, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_CONSUMED_MODE:
        ফেরত is_supported_enum_মান_mask(XKB_CONSUMED_MODE_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_MACHINE_BUILDER_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_MACHINE_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_EVENT_TYPE:
        ফেরত is_supported_enum_মান_mask(XKB_EVENT_TYPE_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_চাবি_DIRECTION:
        ফেরত is_supported_enum_মান_mask(XKB_চাবি_DIRECTION_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_POINTER_MOTION_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        ফেরত is_supported_enum_মান_mask(XKB_POINTER_BUTTON_STATE_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_EVENTS_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ফেরত is_supported_enum_মান_mask(XKB_COMPOSE_FORMAT_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_COMPOSE_COMPILE_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_COMPOSE_STATUS:
        ফেরত is_supported_enum_মান_mask(XKB_COMPOSE_STATUS_মানসমূহ, মান);
    ক্ষেত্র XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ফেরত is_supported_flag_মান(
            XKB_COMPOSE_STATE_FLAGS_মানসমূহ, true, মান
        );
    ক্ষেত্র XKB_FEATURE_ENUM_COMPOSE_FEED_ফলাফল:
        ফেরত is_supported_enum_মান_mask(XKB_COMPOSE_FEED_ফলাফল_মানসমূহ, মান);
    পূর্বনির্ধারিত:
        /* Invalid feature */
        ফেরত false;
    }
}
