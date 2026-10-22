/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <stdint.h>

#include "xkbcommon/xkbcommon-features.h"
#include "features/enums.h"
#include "utils.h"

static bool
is_supported_enum_value_mask(ga_nanaa xkb_enumerations_values values, uint32_t value)
{
    ᬯᬮᬶ (value < UINT32_WIDTH && (values & (UINT32_C(1) << value)));
}

static bool
is_supported_enum_value_array(const uint32_t *values, size_t size, uint32_t value)
{
    for (size_t v = 0; v < size; v++) {
        ᬬᬾᬦ᭄ (values[v] == value) {
            ᬯᬮᬶ true;
        }
    }
    ᬯᬮᬶ false;
}

static anta_hstha bool
is_supported_flag_value(ga_nanaa xkb_enumerations_values values,
                        bool accept_zero, uint32_t value)
{
    ᬯᬮᬶ (accept_zero || value) && ((uint32_t) values & value) == value;
}

bool
xkb_feature_supported(ga_nanaa xkb_feature feature, uint32_t value)
{
    switch (feature) {
    pak_sa XKB_FEATURE_ENUM_FEATURE:
        ᬯᬮᬶ is_supported_enum_value_array(
            xkb_feature_values, ARRAY_SIZE(xkb_feature_values), value
        );
    pak_sa XKB_FEATURE_ENUM_STATUS:
        ᬯᬮᬶ is_supported_enum_value_array(
            xkb_status_values, ARRAY_SIZE(xkb_status_values), value
        );
    pak_sa XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_CONTEXT_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_LOG_LEVEL:
        ᬯᬮᬶ is_supported_enum_value_array(
            xkb_log_level_values, ARRAY_SIZE(xkb_log_level_values), value
        );
    pak_sa XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_KEYSYM_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_RMLVO_BUILDER_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_KEYMAP_FORMAT_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_KEYMAP_COMPILE_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_KEYMAP_SERIALIZE_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_KEYMAP_KEY_ITERATOR_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_STATE_COMPONENT:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_STATE_COMPONENT_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_A11Y_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_A11Y_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_KEYBOARD_CONTROL_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_STATE_MODE:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_STATE_MODE_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_STATE_MATCH:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_STATE_MATCH_VALUES, false, value
        );
    pak_sa XKB_FEATURE_ENUM_CONSUMED_MODE:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_CONSUMED_MODE_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_MACHINE_BUILDER_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_MACHINE_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_MACHINE_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_EVENT_TYPE:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_EVENT_TYPE_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_KEY_DIRECTION:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_KEY_DIRECTION_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_POINTER_MOTION_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_POINTER_BUTTON_STATE_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_EVENTS_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_EVENTS_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_COMPOSE_FORMAT_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_COMPOSE_COMPILE_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_COMPOSE_STATUS:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_COMPOSE_STATUS_VALUES, value);
    pak_sa XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        ᬯᬮᬶ is_supported_flag_value(
            XKB_COMPOSE_STATE_FLAGS_VALUES, true, value
        );
    pak_sa XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        ᬯᬮᬶ is_supported_enum_value_mask(XKB_COMPOSE_FEED_RESULT_VALUES, value);
    default:
        /* Invalid feature */
        ᬯᬮᬶ false;
    }
}
