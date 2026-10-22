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
is_supported_enum_हालागा_mask(enum xkb_enumerations_मगा_हालागा मगा_हालागा, uint32_t हालागा)
{
    return (हालागा < UINT32_WIDTH && (मगा_हालागा & (UINT32_C(1) << हालागा)));
}

static bool
is_supported_enum_हालागा_array(const uint32_t *मगा_हालागा, size_t size, uint32_t हालागा)
{
    for (size_t v = 0; v < size; v++) {
        if (मगा_हालागा[v] == हालागा) {
            return true;
        }
    }
    return false;
}

static inline bool
is_supported_flag_हालागा(enum xkb_enumerations_मगा_हालागा मगा_हालागा,
                        bool accept_zero, uint32_t हालागा)
{
    return (accept_zero || हालागा) && ((uint32_t) मगा_हालागा & हालागा) == हालागा;
}

bool
xkb_feature_supported(enum xkb_feature feature, uint32_t हालागा)
{
    switch (feature) {
    case XKB_FEATURE_ENUM_FEATURE:
        return is_supported_enum_हालागा_array(
            xkb_feature_मगा_हालागा, ARRAY_SIZE(xkb_feature_मगा_हालागा), हालागा
        );
    case XKB_FEATURE_ENUM_STATUS:
        return is_supported_enum_हालागा_array(
            xkb_status_मगा_हालागा, ARRAY_SIZE(xkb_status_मगा_हालागा), हालागा
        );
    case XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        return is_supported_flag_हालागा(
            XKB_CONTEXT_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_LOG_LEVEL:
        return is_supported_enum_हालागा_array(
            xkb_log_level_मगा_हालागा, ARRAY_SIZE(xkb_log_level_मगा_हालागा), हालागा
        );
    case XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        return is_supported_flag_हालागा(
            XKB_KEYSYM_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        return is_supported_flag_हालागा(
            XKB_RMLVO_BUILDER_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        return is_supported_enum_हालागा_mask(XKB_KEYMAP_FORMAT_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        return is_supported_flag_हालागा(
            XKB_KEYMAP_COMPILE_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        return is_supported_flag_हालागा(
            XKB_KEYMAP_SERIALIZE_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_KEYMAP_सुसि_ITERATOR_FLAGS:
        return is_supported_flag_हालागा(
            XKB_KEYMAP_सुसि_ITERATOR_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_STATE_COMPONENT:
        return is_supported_flag_हालागा(
            XKB_STATE_COMPONENT_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        return is_supported_enum_हालागा_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_A11Y_FLAGS:
        return is_supported_flag_हालागा(
            XKB_A11_Y_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        return is_supported_flag_हालागा(
            XKB_KEYBOARD_CONTROL_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_STATE_MODE:
        return is_supported_enum_हालागा_mask(XKB_STATE_MODE_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_STATE_MATCH:
        return is_supported_flag_हालागा(
            XKB_STATE_MATCH_मगा_हालागा, false, हालागा
        );
    case XKB_FEATURE_ENUM_CONSUMED_MODE:
        return is_supported_enum_हालागा_mask(XKB_CONSUMED_MODE_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        return is_supported_flag_हालागा(
            XKB_MACHINE_BUILDER_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_MACHINE_FLAGS:
        return is_supported_flag_हालागा(
            XKB_MACHINE_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_EVENT_TYPE:
        return is_supported_enum_हालागा_mask(XKB_EVENT_TYPE_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_सुसि_DIRECTION:
        return is_supported_enum_हालागा_mask(XKB_सुसि_DIRECTION_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        return is_supported_flag_हालागा(
            XKB_POINTER_MOTION_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        return is_supported_enum_हालागा_mask(XKB_POINTER_BUTTON_STATE_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_EVENTS_FLAGS:
        return is_supported_flag_हालागा(
            XKB_EVENTS_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        return is_supported_enum_हालागा_mask(XKB_COMPOSE_FORMAT_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        return is_supported_flag_हालागा(
            XKB_COMPOSE_COMPILE_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_COMPOSE_STATUS:
        return is_supported_enum_हालागा_mask(XKB_COMPOSE_STATUS_मगा_हालागा, हालागा);
    case XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        return is_supported_flag_हालागा(
            XKB_COMPOSE_STATE_FLAGS_मगा_हालागा, true, हालागा
        );
    case XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        return is_supported_enum_हालागा_mask(XKB_COMPOSE_FEED_RESULT_मगा_हालागा, हालागा);
    default:
        /* Invalid feature */
        return false;
    }
}
