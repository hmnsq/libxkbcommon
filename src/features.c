/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon-features.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "utils.h"

સ્થિર bool
is_supported_enum_કિંમત_mask(ગણના xkb_enumerations_કિંમતો કિંમતો, uint32_t કિંમત)
{
    પાછા (કિંમત < UINT32_WIDTH && (કિંમતો & (UINT32_C(1) << કિંમત)));
}

સ્થિર bool
is_supported_enum_કિંમત_array(અચલ uint32_t *કિંમતો, size_t size, uint32_t કિંમત)
{
    માટે (size_t v = 0; v < size; v++) {
        જો (કિંમતો[v] == કિંમત) {
            પાછા true;
        }
    }
    પાછા false;
}

સ્થિર અંતઃરેખ bool
is_supported_flag_કિંમત(ગણના xkb_enumerations_કિંમતો કિંમતો,
                        bool accept_zero, uint32_t કિંમત)
{
    પાછા (accept_zero || કિંમત) && ((uint32_t) કિંમતો & કિંમત) == કિંમત;
}

bool
xkb_feature_supported(ગણના xkb_feature feature, uint32_t કિંમત)
{
    પસંદગી (feature) {
    સ્થિતિ XKB_FEATURE_ENUM_FEATURE:
        પાછા is_supported_enum_કિંમત_array(
            xkb_feature_કિંમતો, ARRAY_SIZE(xkb_feature_કિંમતો), કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_STATUS:
        પાછા is_supported_enum_કિંમત_array(
            xkb_status_કિંમતો, ARRAY_SIZE(xkb_status_કિંમતો), કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_CONTEXT_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_LOG_LEVEL:
        પાછા is_supported_enum_કિંમત_array(
            xkb_log_level_કિંમતો, ARRAY_SIZE(xkb_log_level_કિંમતો), કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_KEYSYM_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_RMLVO_BUILDER_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        પાછા is_supported_enum_કિંમત_mask(XKB_KEYMAP_FORMAT_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_KEYMAP_COMPILE_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_KEYMAP_SERIALIZE_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_KEYMAP_ચાવી_ITERATOR_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_KEYMAP_ચાવી_ITERATOR_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_STATE_COMPONENT:
        પાછા is_supported_flag_કિંમત(
            XKB_STATE_COMPONENT_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        પાછા is_supported_enum_કિંમત_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_A11Y_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_A11_Y_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_KEYBOARD_CONTROL_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_STATE_MODE:
        પાછા is_supported_enum_કિંમત_mask(XKB_STATE_MODE_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_STATE_MATCH:
        પાછા is_supported_flag_કિંમત(
            XKB_STATE_MATCH_કિંમતો, false, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_CONSUMED_MODE:
        પાછા is_supported_enum_કિંમત_mask(XKB_CONSUMED_MODE_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_MACHINE_BUILDER_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_MACHINE_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_EVENT_TYPE:
        પાછા is_supported_enum_કિંમત_mask(XKB_EVENT_TYPE_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_ચાવી_DIRECTION:
        પાછા is_supported_enum_કિંમત_mask(XKB_ચાવી_DIRECTION_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_POINTER_MOTION_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        પાછા is_supported_enum_કિંમત_mask(XKB_POINTER_BUTTON_STATE_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_EVENTS_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        પાછા is_supported_enum_કિંમત_mask(XKB_COMPOSE_FORMAT_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_COMPOSE_COMPILE_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_COMPOSE_STATUS:
        પાછા is_supported_enum_કિંમત_mask(XKB_COMPOSE_STATUS_કિંમતો, કિંમત);
    સ્થિતિ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        પાછા is_supported_flag_કિંમત(
            XKB_COMPOSE_STATE_FLAGS_કિંમતો, true, કિંમત
        );
    સ્થિતિ XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        પાછા is_supported_enum_કિંમત_mask(XKB_COMPOSE_FEED_RESULT_કિંમતો, કિંમત);
    મૂળભૂત:
        /* Invalid feature */
        પાછા false;
    }
}
