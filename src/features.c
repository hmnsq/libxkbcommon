/*
 * NOTE: This file has been generated automatically by “scripts/export-enums.py”.
 *       Do not edit manually!
 */

/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon-features.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "utils.h"

స్థిర bool
is_supported_enum_విలువ_ముసుగు(గణన xkb_enumerations_విలువలు విలువలు, uint32_t విలువ)
{
    తిరిగి (విలువ < UINT32_వెడల్పు && (విలువలు & (UINT32_C(1) << విలువ)));
}

స్థిర bool
is_supported_enum_విలువ_array(స్థిరరాశి uint32_t *విలువలు, కొలత_t కొలత, uint32_t విలువ)
{
    కొరకు (కొలత_t v = 0; v < కొలత; v++) {
        ఒకవేళ (విలువలు[v] == విలువ) {
            తిరిగి true;
        }
    }
    తిరిగి false;
}

స్థిర అంతఃవరుస bool
is_supported_flag_విలువ(గణన xkb_enumerations_విలువలు విలువలు,
                        bool accept_zero, uint32_t విలువ)
{
    తిరిగి (accept_zero || విలువ) && ((uint32_t) విలువలు & విలువ) == విలువ;
}

bool
xkb_feature_supported(గణన xkb_feature feature, uint32_t విలువ)
{
    ఎంపిక (feature) {
    స్థితి XKB_FEATURE_ENUM_FEATURE:
        తిరిగి is_supported_enum_విలువ_array(
            xkb_feature_విలువలు, ARRAY_కొలత(xkb_feature_విలువలు), విలువ
        );
    స్థితి XKB_FEATURE_ENUM_స్థితి:
        తిరిగి is_supported_enum_విలువ_array(
            xkb_స్థితి_విలువలు, ARRAY_కొలత(xkb_స్థితి_విలువలు), విలువ
        );
    స్థితి XKB_FEATURE_ENUM_CONTEXT_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_CONTEXT_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_LOG_LEVEL:
        తిరిగి is_supported_enum_విలువ_array(
            xkb_log_level_విలువలు, ARRAY_కొలత(xkb_log_level_విలువలు), విలువ
        );
    స్థితి XKB_FEATURE_ENUM_KEYSYM_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_KEYSYM_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_RMLVO_BUILDER_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_RMLVO_BUILDER_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_KEYMAP_FORMAT:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_KEYMAP_FORMAT_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_KEYMAP_COMPILE_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_KEYMAP_COMPILE_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_KEYMAP_SERIALIZE_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_KEYMAP_SERIALIZE_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_KEYMAP_కీలకం_ITERATOR_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_STATE_COMPONENT:
        తిరిగి is_supported_flag_విలువ(
            XKB_STATE_COMPONENT_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_A11Y_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_A11_Y_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_KEYBOARD_CONTROL_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_KEYBOARD_CONTROL_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_STATE_బహుళకం:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_STATE_బహుళకం_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_STATE_సరిపోలిక:
        తిరిగి is_supported_flag_విలువ(
            XKB_STATE_సరిపోలిక_విలువలు, false, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_CONSUMED_బహుళకం:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_CONSUMED_బహుళకం_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_MACHINE_BUILDER_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_MACHINE_BUILDER_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_MACHINE_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_MACHINE_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_EVENT_TYPE:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_EVENT_TYPE_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_కీలకం_DIRECTION:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_కీలకం_DIRECTION_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_సూచిక_MOTION_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_సూచిక_MOTION_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_సూచిక_BUTTON_STATE:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_సూచిక_BUTTON_STATE_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_EVENTS_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_EVENTS_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_COMPOSE_FORMAT:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_COMPOSE_FORMAT_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_COMPOSE_COMPILE_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_COMPOSE_COMPILE_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_COMPOSE_స్థితి:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_COMPOSE_స్థితి_విలువలు, విలువ);
    స్థితి XKB_FEATURE_ENUM_COMPOSE_STATE_FLAGS:
        తిరిగి is_supported_flag_విలువ(
            XKB_COMPOSE_STATE_FLAGS_విలువలు, true, విలువ
        );
    స్థితి XKB_FEATURE_ENUM_COMPOSE_FEED_ఫలితం:
        తిరిగి is_supported_enum_విలువ_ముసుగు(XKB_COMPOSE_FEED_ఫలితం_విలువలు, విలువ);
    అప్రమేయ:
        /* Invalid feature */
        తిరిగి false;
    }
}
