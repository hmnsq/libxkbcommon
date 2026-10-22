/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-features.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ସ୍ଥିର bool
is_supported_enum_ମୂଲ୍ୟ_mask(ଗଣନା xkb_enumerations_ମୂଲ୍ୟଗୁଡ଼ିକ ମୂଲ୍ୟଗୁଡ଼ିକ, uint32_t ମୂଲ୍ୟ)
{
    ଫେରନ୍ତୁ (ମୂଲ୍ୟ < UINT32_WIDTH && (ମୂଲ୍ୟଗୁଡ଼ିକ & (UINT32_C(1) << ମୂଲ୍ୟ)));
}

ସ୍ଥିର bool
is_supported_enum_ମୂଲ୍ୟ_array(ଅଚଳ uint32_t *ମୂଲ୍ୟଗୁଡ଼ିକ, size_t size, uint32_t ମୂଲ୍ୟ)
{
    ପାଇଁ (size_t v = 0; v < size; v++) {
        ଯଦି (ମୂଲ୍ୟଗୁଡ଼ିକ[v] == ମୂଲ୍ୟ) {
            ଫେରନ୍ତୁ true;
        }
    }
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_supported_flag_ମୂଲ୍ୟ(ଗଣନା xkb_enumerations_ମୂଲ୍ୟଗୁଡ଼ିକ ମୂଲ୍ୟଗୁଡ଼ିକ,
                        bool accept_zero, uint32_t ମୂଲ୍ୟ)
{
    ଫେରନ୍ତୁ (accept_zero || ମୂଲ୍ୟ) && ((uint32_t) ମୂଲ୍ୟଗୁଡ଼ିକ & ମୂଲ୍ୟ) == ମୂଲ୍ୟ;
}

bool
xkb_feature_supported(ଗଣନା xkb_feature feature, uint32_t ମୂଲ୍ୟ)
{
    ବାଛ (feature) {
    ସ୍ଥିତି XKB_FEATURE_ENUM_FEATURE:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_array(
            xkb_feature_ମୂଲ୍ୟଗୁଡ଼ିକ, ARRAY_SIZE(xkb_feature_ମୂଲ୍ୟଗୁଡ଼ିକ), ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_STATUS:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_array(
            xkb_status_ମୂଲ୍ୟଗୁଡ଼ିକ, ARRAY_SIZE(xkb_status_ମୂଲ୍ୟଗୁଡ଼ିକ), ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_CONTEXT_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_LOG_LEVEL:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_array(
            xkb_log_level_ମୂଲ୍ୟଗୁଡ଼ିକ, ARRAY_SIZE(xkb_log_level_ମୂଲ୍ୟଗୁଡ଼ିକ), ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_KEYSYM_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_RMLVO_BUILDER_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_KEYMAP_FORMAT_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_KEYMAP_COMPILE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_KEYMAP_SERIALIZE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYMAP_ଚାବି_ITERATOR_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_KEYMAP_ଚାବି_ITERATOR_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_STATE_COMPONENT:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_STATE_COMPONENT_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_A11Y_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_A11_Y_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_KEYBOARD_CONTROL_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_STATE_MODE:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_STATE_MODE_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_STATE_MATCH:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_STATE_MATCH_ମୂଲ୍ୟଗୁଡ଼ିକ, false, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_CONSUMED_MODE:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_CONSUMED_MODE_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_MACHINE_BUILDER_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_MACHINE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_EVENT_TYPE:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_EVENT_TYPE_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_ଚାବି_DIRECTION:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_ଚାବି_DIRECTION_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_POINTER_MOTION_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_POINTER_BUTTON_STATE_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_EVENTS_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_COMPOSE_FORMAT_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_COMPOSE_COMPILE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_COMPOSE_STATUS:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_COMPOSE_STATUS_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ସ୍ଥିତି XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ଫେରନ୍ତୁ is_supported_flag_ମୂଲ୍ୟ(
            XKB_COMPOSE_STATE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ, true, ମୂଲ୍ୟ
        );
    ସ୍ଥିତି XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        ଫେରନ୍ତୁ is_supported_enum_ମୂଲ୍ୟ_mask(XKB_COMPOSE_FEED_RESULT_ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟ);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        /* Invalid feature */
        ଫେରନ୍ତୁ false;
    }
}
