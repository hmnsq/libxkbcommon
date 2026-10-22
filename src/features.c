/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-features.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "utils.h"

സ്ഥിതം bool
is_supported_enum_മൂല്യം_mask(എണ്ണൽ xkb_enumerations_മൂല്യങ്ങൾ മൂല്യങ്ങൾ, uint32_t മൂല്യം)
{
    തിരികെ (മൂല്യം < UINT32_WIDTH && (മൂല്യങ്ങൾ & (UINT32_C(1) << മൂല്യം)));
}

സ്ഥിതം bool
is_supported_enum_മൂല്യം_array(സ്ഥിരം uint32_t *മൂല്യങ്ങൾ, size_t size, uint32_t മൂല്യം)
{
    വേണ്ടി (size_t v = 0; v < size; v++) {
        എങ്കിൽ (മൂല്യങ്ങൾ[v] == മൂല്യം) {
            തിരികെ true;
        }
    }
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
is_supported_flag_മൂല്യം(എണ്ണൽ xkb_enumerations_മൂല്യങ്ങൾ മൂല്യങ്ങൾ,
                        bool accept_zero, uint32_t മൂല്യം)
{
    തിരികെ (accept_zero || മൂല്യം) && ((uint32_t) മൂല്യങ്ങൾ & മൂല്യം) == മൂല്യം;
}

bool
xkb_feature_supported(എണ്ണൽ xkb_feature feature, uint32_t മൂല്യം)
{
    തിരഞ്ഞെടുപ്പ് (feature) {
    അവസ്ഥ XKB_FEATURE_ENUM_FEATURE:
        തിരികെ is_supported_enum_മൂല്യം_array(
            xkb_feature_മൂല്യങ്ങൾ, ARRAY_SIZE(xkb_feature_മൂല്യങ്ങൾ), മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_STATUS:
        തിരികെ is_supported_enum_മൂല്യം_array(
            xkb_status_മൂല്യങ്ങൾ, ARRAY_SIZE(xkb_status_മൂല്യങ്ങൾ), മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_CONTEXT_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_LOG_LEVEL:
        തിരികെ is_supported_enum_മൂല്യം_array(
            xkb_log_level_മൂല്യങ്ങൾ, ARRAY_SIZE(xkb_log_level_മൂല്യങ്ങൾ), മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_KEYSYM_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_RMLVO_BUILDER_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_KEYMAP_FORMAT_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_KEYMAP_COMPILE_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_KEYMAP_SERIALIZE_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_KEYMAP_താക്കോൽ_ITERATOR_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_STATE_COMPONENT:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_STATE_COMPONENT_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_LAYOUT_OUT_OF_RANGE_POLICY_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_A11Y_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_A11_Y_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_KEYBOARD_CONTROL_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_STATE_MODE:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_STATE_MODE_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_STATE_MATCH:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_STATE_MATCH_മൂല്യങ്ങൾ, false, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_CONSUMED_MODE:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_CONSUMED_MODE_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_MACHINE_BUILDER_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_MACHINE_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_MACHINE_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_EVENT_TYPE:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_EVENT_TYPE_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_താക്കോൽ_DIRECTION:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_താക്കോൽ_DIRECTION_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_POINTER_MOTION_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_POINTER_MOTION_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_POINTER_BUTTON_STATE:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_POINTER_BUTTON_STATE_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_EVENTS_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_EVENTS_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_COMPOSE_FORMAT_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_COMPOSE_COMPILE_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_COMPOSE_STATUS:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_COMPOSE_STATUS_മൂല്യങ്ങൾ, മൂല്യം);
    അവസ്ഥ XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        തിരികെ is_supported_flag_മൂല്യം(
            XKB_COMPOSE_STATE_FLAGS_മൂല്യങ്ങൾ, true, മൂല്യം
        );
    അവസ്ഥ XKB_FEATURE_ENUM_COMPOSE_FEED_RESULT:
        തിരികെ is_supported_enum_മൂല്യം_mask(XKB_COMPOSE_FEED_RESULT_മൂല്യങ്ങൾ, മൂല്യം);
    സ്ഥിരസ്ഥിതി:
        /* Invalid feature */
        തിരികെ false;
    }
}
