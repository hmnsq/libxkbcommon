/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdalign.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "abi-check.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"

/** Core keyboard state components */
ರಚನೆ state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t ತಳ_ಗುಂಪು; /**< depressed */
    int32_t latched_ಗುಂಪು;
    int32_t locked_ಗುಂಪು;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು; /**< effective */

    xkb_mod_ಮುಖವಾಡ_t ತಳ_mods; /**< depressed */
    xkb_mod_ಮುಖವಾಡ_t latched_mods;
    xkb_mod_ಮುಖವಾಡ_t locked_mods;
    xkb_mod_ಮುಖವಾಡ_t mods; /**< effective */

    xkb_led_ಮುಖವಾಡ_t leds;

    ಎಣಿಕೆ xkb_action_controls controls; /**< effective */
};

ರಚನೆ xkb_event {
    ರಚನೆ xkb_context *ctx;
    ಎಣಿಕೆ xkb_event_type type;
    ಒಕ್ಕೂಟ {
        ರಚನೆ {
            xkb_keycode_t keycode;
            ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction;
        } ಕೀಲಿ;
        ರಚನೆ {
            ರಚನೆ state_components components;
            ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು;
        } components;
        ರಚನೆ xkb_event_ಸೂಚಕ_motion ಸೂಚಕ_motion;
        ರಚನೆ xkb_event_ಸೂಚಕ_button ಸೂಚಕ_button;
        ರಚನೆ {
            int8_t ಸೂಚ್ಯಂಕ_or_ಸ್ಥಾನಾಂತರ;
            bool is_ಸ್ಥಾನಾಂತರ;
        } virtual_console;
    };
};

XKB_EXPORT_ಖಾಸಗಿ ಶೂನ್ಯ
xkb_state_reset(ರಚನೆ xkb_state *state);

XKB_EXPORT_ಖಾಸಗಿ xkb_led_ಮುಖವಾಡ_t
xkb_state_serialize_leds(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state *state,
                         ಎಣಿಕೆ xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_events_config_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_events_config, ಗಾತ್ರ_2, flags);
assert_no_padding(ರಚನೆ xkb_events_config, flags);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_events_config) ==
              ಗಾತ್ರ(ರಚನೆ xkb_events_config_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_events_config, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_events_config) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_event_components_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t ಬದಲಾಯಿತು;
    xkb_mod_ಮುಖವಾಡ_t depressed_mods;
    xkb_mod_ಮುಖವಾಡ_t latched_mods;
    xkb_mod_ಮುಖವಾಡ_t locked_mods;
    xkb_mod_ಮುಖವಾಡ_t mods;
    xkb_layout_ಸೂಚ್ಯಂಕ_t depressed_layout;
    xkb_layout_ಸೂಚ್ಯಂಕ_t latched_layout;
    xkb_layout_ಸೂಚ್ಯಂಕ_t locked_layout;
    xkb_layout_ಸೂಚ್ಯಂಕ_t layout;
    xkb_led_ಮುಖವಾಡ_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_event_components, ಗಾತ್ರ_2, ಬದಲಾಯಿತು);
assert_no_padding(ರಚನೆ xkb_event_components, ಬದಲಾಯಿತು, depressed_mods);
assert_no_padding(ರಚನೆ xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(ರಚನೆ xkb_event_components, latched_mods, locked_mods);
assert_no_padding(ರಚನೆ xkb_event_components, locked_mods, mods);
assert_no_padding(ರಚನೆ xkb_event_components, mods, depressed_layout);
assert_no_padding(ರಚನೆ xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(ರಚನೆ xkb_event_components, latched_layout, locked_layout);
assert_no_padding(ರಚನೆ xkb_event_components, locked_layout, layout);
assert_no_padding(ರಚನೆ xkb_event_components, layout, leds);
assert_no_padding(ರಚನೆ xkb_event_components, leds, controls);
assert_no_padding(ರಚನೆ xkb_event_components, controls);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_components) ==
              ಗಾತ್ರ(ರಚನೆ xkb_event_components_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, ಬದಲಾಯಿತು);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, depressed_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, latched_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, locked_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, depressed_layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, latched_layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, locked_layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, leds);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_components) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_event_ಸೂಚಕ_motion_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_motion, ಗಾತ್ರ_2, flags);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_motion, flags, x);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_motion, x, y);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_motion, y);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion) ==
              ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion, _v1, flags);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion, _v1, x);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_event_ಸೂಚಕ_button_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t button;
    uint8_t ಎಣಿಕೆ_2;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_button, ಗಾತ್ರ_2, button);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_button, button, ಎಣಿಕೆ_2);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_button, ಎಣಿಕೆ_2, state);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_button, state, reserved0);
assert_no_padding(ರಚನೆ xkb_event_ಸೂಚಕ_button, reserved0);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button) ==
              ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button, _v1, button);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button, _v1, ಎಣಿಕೆ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button, _v1, state);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_state_components_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t components;
    xkb_mod_ಮುಖವಾಡ_t affect_latched_mods;
    xkb_mod_ಮುಖವಾಡ_t latched_mods;
    xkb_mod_ಮುಖವಾಡ_t affect_locked_mods;
    xkb_mod_ಮುಖವಾಡ_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_state_components_update, ಗಾತ್ರ_2, components);
assert_no_padding(ರಚನೆ xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(ರಚನೆ xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(ರಚನೆ xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(ರಚನೆ xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(ರಚನೆ xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(ರಚನೆ xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(ರಚನೆ xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(ರಚನೆ xkb_state_components_update, affect_controls, controls);
assert_no_padding(ರಚನೆ xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_state_components_update) ==
              ಗಾತ್ರ(ರಚನೆ xkb_state_components_update_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, components);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, affect_latched_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, latched_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, affect_locked_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, locked_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, latched_layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, locked_layout);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, affect_controls);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_state_components_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_layout_policy_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t policy;
    xkb_layout_ಸೂಚ್ಯಂಕ_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_layout_policy_update, ಗಾತ್ರ_2, policy);
assert_no_padding(ರಚನೆ xkb_layout_policy_update, policy, redirect);
assert_no_padding(ರಚನೆ xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update) ==
              ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_layout_policy_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_layout_policy_update, _v1, policy);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_synthetic_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t reserved0;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update_v1 *components;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_synthetic_update, ಗಾತ್ರ_2, reserved0);
assert_no_padding(ರಚನೆ xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(ರಚನೆ xkb_synthetic_update, components, layout_policy);
assert_no_padding(ರಚನೆ xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update) ==
              ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_synthetic_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_synthetic_update, _v1, components);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_machine_builder_config_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_machine_builder_config, ಗಾತ್ರ_2, builder_flags);
assert_no_padding(ರಚನೆ xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(ರಚನೆ xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config) ==
              ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_config, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_config, _v1, builder_flags);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_machine_builder_a11y_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_machine_builder_a11y_update, ಗಾತ್ರ_2, affect_flags);
assert_no_padding(ರಚನೆ xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(ರಚನೆ xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update) ==
              ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_machine_builder_mods_remap_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    xkb_mod_ಮುಖವಾಡ_t ಮೂಲ;
    xkb_mod_ಮುಖವಾಡ_t ಗುರಿ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_machine_builder_mods_remap_update, ಗಾತ್ರ_2, ಮೂಲ);
assert_no_padding(ರಚನೆ xkb_machine_builder_mods_remap_update, ಮೂಲ, ಗುರಿ);
assert_no_padding(ರಚನೆ xkb_machine_builder_mods_remap_update, ಗುರಿ);

/* Current version is 1 */
static_assert(
    ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update) ==
    ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update, _v1, ಮೂಲ);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update, _v1, ಗುರಿ);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ರಚನೆ xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t ಗಾತ್ರ_2;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಮೂಲ;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುರಿ;
    xkb_mod_ಮುಖವಾಡ_t affect_mods;
    xkb_mod_ಮುಖವಾಡ_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(ರಚನೆ xkb_machine_builder_shortcut_override_update, ಗಾತ್ರ_2, ಮೂಲ);
assert_no_padding(ರಚನೆ xkb_machine_builder_shortcut_override_update, ಮೂಲ, ಗುರಿ);
assert_no_padding(ರಚನೆ xkb_machine_builder_shortcut_override_update, ಗುರಿ, affect_mods);
assert_no_padding(ರಚನೆ xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(ರಚನೆ xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update) ==
              ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update, _v1, ಗಾತ್ರ_2);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update, _v1, ಮೂಲ);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update, _v1, ಗುರಿ);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update) * 30 <=
              (ಗಾತ್ರ_t)XKB_ABI_MAX_ಗಾತ್ರ, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_ಗಾತ್ರ_v1(x) _Generic(                   \
    (x),                                                            \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *:                               \
        ಗಾತ್ರ(ರಚನೆ xkb_events_config_v1),                        \
    ರಚನೆ xkb_event_components *:                                  \
        ಗಾತ್ರ(ರಚನೆ xkb_event_components_v1),                     \
    ರಚನೆ xkb_event_ಸೂಚಕ_motion *:                              \
        ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion_v1),                 \
    ರಚನೆ xkb_event_ಸೂಚಕ_button *:                              \
        ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button_v1),                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *:                            \
        ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update_v1),                     \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *:                     \
        ಗಾತ್ರ(ರಚನೆ xkb_state_components_update_v1),              \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *:                        \
        ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update_v1),                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *:                      \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config_v1),               \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *:                 \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update_v1),          \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *:           \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update_v1),    \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *:      \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_ಗಾತ್ರ_min(x) _Generic(                  \
    (x),                                                            \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *:                               \
        ಗಾತ್ರ(ರಚನೆ xkb_events_config_v1),                        \
    ರಚನೆ xkb_event_components *:                                  \
        ಗಾತ್ರ(ರಚನೆ xkb_event_components_v1),                     \
    ರಚನೆ xkb_event_ಸೂಚಕ_motion *:                              \
        ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion_v1),                 \
    ರಚನೆ xkb_event_ಸೂಚಕ_button *:                              \
        ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button_v1),                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *:                            \
        ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update_v1),                     \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *:                     \
        ಗಾತ್ರ(ರಚನೆ xkb_state_components_update_v1),              \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *:                        \
        ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update_v1),                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *:                      \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config_v1),               \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *:                 \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update_v1),          \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *:           \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update_v1),    \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *:      \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_versioned_struct_reserved_ಸ್ಥಾನಾಂತರ(x) _Generic(        \
    (x),                                                         \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *:                            \
        ಗಾತ್ರ(ರಚನೆ xkb_events_config),                        \
    ರಚನೆ xkb_event_components *:                               \
        ಗಾತ್ರ(ರಚನೆ xkb_event_components),                     \
    ರಚನೆ xkb_event_ಸೂಚಕ_motion *:                           \
        ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion),                 \
    ರಚನೆ xkb_event_ಸೂಚಕ_button *:                           \
        offsetof(ರಚನೆ xkb_event_ಸೂಚಕ_button, reserved0),    \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *:                         \
        ಗಾತ್ರ(ರಚನೆ xkb_synthetic_update),                     \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *:                  \
        ಗಾತ್ರ(ರಚನೆ xkb_state_components_update),              \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *:                     \
        ಗಾತ್ರ(ರಚನೆ xkb_layout_policy_update),                 \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *:                   \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_config),               \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *:              \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_a11y_update),          \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *:        \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_mods_remap_update),    \
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *:   \
        ಗಾತ್ರ(ರಚನೆ xkb_machine_builder_shortcut_override_update)\
)

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_check_state_abi(x) xkb_check_versioned_struct_ಗಾತ್ರ_2( \
    xkb_versioned_struct_ಗಾತ್ರ_v1(x),                            \
    xkb_versioned_struct_ಗಾತ್ರ_min(x),                           \
    xkb_versioned_struct_reserved_ಸ್ಥಾನಾಂತರ(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ರಚನೆ xkb_event_components *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_components *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ರಚನೆ xkb_event_ಸೂಚಕ_motion *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_ಸೂಚಕ_motion *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ರಚನೆ xkb_event_ಸೂಚಕ_button *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_ಸೂಚಕ_button *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *)ಖಾಲಿ)),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_v1(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *)ಖಾಲಿ)) <=
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *)ಖಾಲಿ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_components *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ರಚನೆ xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_ಸೂಚಕ_motion *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_motion),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ರಚನೆ xkb_event_ಸೂಚಕ_button*)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ರಚನೆ xkb_event_ಸೂಚಕ_button),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_ಗಾತ್ರ_min(((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update *)ಖಾಲಿ)) <=
    ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_ಖಾಸಗಿ ರಚನೆ xkb_state *
xkb_machine_get_state(ರಚನೆ xkb_machine *machine);
