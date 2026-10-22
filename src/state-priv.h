/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdalign.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abi-check.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"

/** Core keyboard state components */
ਬਣਤਰ state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t ਅਧਾਰ_ਸਮੂਹ; /**< depressed */
    int32_t latched_ਸਮੂਹ;
    int32_t locked_ਸਮੂਹ;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ; /**< effective */

    xkb_mod_ਨਕਾਬ_t ਅਧਾਰ_mods; /**< depressed */
    xkb_mod_ਨਕਾਬ_t latched_mods;
    xkb_mod_ਨਕਾਬ_t locked_mods;
    xkb_mod_ਨਕਾਬ_t mods; /**< effective */

    xkb_led_ਨਕਾਬ_t leds;

    ਗਿਣਤੀ xkb_action_controls controls; /**< effective */
};

ਬਣਤਰ xkb_event {
    ਬਣਤਰ xkb_context *ctx;
    ਗਿਣਤੀ xkb_event_type type;
    ਸੰਘ {
        ਬਣਤਰ {
            xkb_keycode_t keycode;
            ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction;
        } ਕੁੰਜੀ;
        ਬਣਤਰ {
            ਬਣਤਰ state_components components;
            ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ;
        } components;
        ਬਣਤਰ xkb_event_ਸੂਚਕ_motion ਸੂਚਕ_motion;
        ਬਣਤਰ xkb_event_ਸੂਚਕ_button ਸੂਚਕ_button;
        ਬਣਤਰ {
            int8_t ਸੂਚਕਾਂਕ_or_ਵਿੱਥ;
            bool is_ਵਿੱਥ;
        } virtual_console;
    };
};

XKB_EXPORT_ਨਿੱਜੀ ਖਾਲੀ
xkb_state_reset(ਬਣਤਰ xkb_state *state);

XKB_EXPORT_ਨਿੱਜੀ xkb_led_ਨਕਾਬ_t
xkb_state_serialize_leds(ਅਚਲ ਬਣਤਰ xkb_state *state,
                         ਗਿਣਤੀ xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_events_config_v1 {
    uint32_t ਮਾਪ;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_events_config, ਮਾਪ, flags);
assert_no_padding(ਬਣਤਰ xkb_events_config, flags);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_events_config) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_events_config_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_events_config, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_events_config) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_event_components_v1 {
    uint32_t ਮਾਪ;
    uint32_t ਬਦਲਿਆ;
    xkb_mod_ਨਕਾਬ_t depressed_mods;
    xkb_mod_ਨਕਾਬ_t latched_mods;
    xkb_mod_ਨਕਾਬ_t locked_mods;
    xkb_mod_ਨਕਾਬ_t mods;
    xkb_layout_ਸੂਚਕਾਂਕ_t depressed_layout;
    xkb_layout_ਸੂਚਕਾਂਕ_t latched_layout;
    xkb_layout_ਸੂਚਕਾਂਕ_t locked_layout;
    xkb_layout_ਸੂਚਕਾਂਕ_t layout;
    xkb_led_ਨਕਾਬ_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_event_components, ਮਾਪ, ਬਦਲਿਆ);
assert_no_padding(ਬਣਤਰ xkb_event_components, ਬਦਲਿਆ, depressed_mods);
assert_no_padding(ਬਣਤਰ xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(ਬਣਤਰ xkb_event_components, latched_mods, locked_mods);
assert_no_padding(ਬਣਤਰ xkb_event_components, locked_mods, mods);
assert_no_padding(ਬਣਤਰ xkb_event_components, mods, depressed_layout);
assert_no_padding(ਬਣਤਰ xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(ਬਣਤਰ xkb_event_components, latched_layout, locked_layout);
assert_no_padding(ਬਣਤਰ xkb_event_components, locked_layout, layout);
assert_no_padding(ਬਣਤਰ xkb_event_components, layout, leds);
assert_no_padding(ਬਣਤਰ xkb_event_components, leds, controls);
assert_no_padding(ਬਣਤਰ xkb_event_components, controls);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_components) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_event_components_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, ਬਦਲਿਆ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, depressed_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, latched_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, locked_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, depressed_layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, latched_layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, locked_layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, leds);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_components) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_event_ਸੂਚਕ_motion_v1 {
    uint32_t ਮਾਪ;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, ਮਾਪ, flags);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, flags, x);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, x, y);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, y);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, _v1, flags);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, _v1, x);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_event_ਸੂਚਕ_button_v1 {
    uint32_t ਮਾਪ;
    uint32_t button;
    uint8_t ਗਿਣਤੀ_2;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, ਮਾਪ, button);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, button, ਗਿਣਤੀ_2);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, ਗਿਣਤੀ_2, state);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, state, reserved0);
assert_no_padding(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, reserved0);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, _v1, button);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, _v1, ਗਿਣਤੀ_2);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, _v1, state);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_state_components_update_v1 {
    uint32_t ਮਾਪ;
    uint32_t components;
    xkb_mod_ਨਕਾਬ_t affect_latched_mods;
    xkb_mod_ਨਕਾਬ_t latched_mods;
    xkb_mod_ਨਕਾਬ_t affect_locked_mods;
    xkb_mod_ਨਕਾਬ_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_state_components_update, ਮਾਪ, components);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, affect_controls, controls);
assert_no_padding(ਬਣਤਰ xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, components);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, affect_latched_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, latched_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, affect_locked_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, locked_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, latched_layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, locked_layout);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, affect_controls);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_layout_policy_update_v1 {
    uint32_t ਮਾਪ;
    uint32_t policy;
    xkb_layout_ਸੂਚਕਾਂਕ_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_layout_policy_update, ਮਾਪ, policy);
assert_no_padding(ਬਣਤਰ xkb_layout_policy_update, policy, redirect);
assert_no_padding(ਬਣਤਰ xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_layout_policy_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_layout_policy_update, _v1, policy);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_synthetic_update_v1 {
    uint32_t ਮਾਪ;
    uint32_t reserved0;
    ਅਚਲ ਬਣਤਰ xkb_state_components_update_v1 *components;
    ਅਚਲ ਬਣਤਰ xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_synthetic_update, ਮਾਪ, reserved0);
assert_no_padding(ਬਣਤਰ xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(ਬਣਤਰ xkb_synthetic_update, components, layout_policy);
assert_no_padding(ਬਣਤਰ xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_synthetic_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_synthetic_update, _v1, components);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_machine_builder_config_v1 {
    uint32_t ਮਾਪ;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_machine_builder_config, ਮਾਪ, builder_flags);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_config, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_config, _v1, builder_flags);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_machine_builder_a11y_update_v1 {
    uint32_t ਮਾਪ;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_machine_builder_a11y_update, ਮਾਪ, affect_flags);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_a11y_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_machine_builder_mods_remap_update_v1 {
    uint32_t ਮਾਪ;
    xkb_mod_ਨਕਾਬ_t ਸਰੋਤ;
    xkb_mod_ਨਕਾਬ_t ਨਿਸ਼ਾਨਾ;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_machine_builder_mods_remap_update, ਮਾਪ, ਸਰੋਤ);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_mods_remap_update, ਸਰੋਤ, ਨਿਸ਼ਾਨਾ);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_mods_remap_update, ਨਿਸ਼ਾਨਾ);

/* Current version is 1 */
static_assert(
    ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update) ==
    ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update, _v1, ਸਰੋਤ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update, _v1, ਨਿਸ਼ਾਨਾ);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ਬਣਤਰ xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t ਮਾਪ;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਰੋਤ;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਨਿਸ਼ਾਨਾ;
    xkb_mod_ਨਕਾਬ_t affect_mods;
    xkb_mod_ਨਕਾਬ_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, ਮਾਪ, ਸਰੋਤ);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, ਸਰੋਤ, ਨਿਸ਼ਾਨਾ);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, ਨਿਸ਼ਾਨਾ, affect_mods);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update) ==
              ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, _v1, ਮਾਪ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, _v1, ਸਰੋਤ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, _v1, ਨਿਸ਼ਾਨਾ);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_ਖੇਤਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update) * 30 <=
              (ਮਾਪ_t)XKB_ABI_MAX_ਮਾਪ, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_ਮਾਪ_v1(x) _Generic(                   \
    (x),                                                            \
    ਅਚਲ ਬਣਤਰ xkb_events_config *:                               \
        ਆਕਾਰ(ਬਣਤਰ xkb_events_config_v1),                        \
    ਬਣਤਰ xkb_event_components *:                                  \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_components_v1),                     \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *:                              \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion_v1),                 \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_button *:                              \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button_v1),                 \
    ਅਚਲ ਬਣਤਰ xkb_synthetic_update *:                            \
        ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update_v1),                     \
    ਅਚਲ ਬਣਤਰ xkb_state_components_update *:                     \
        ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update_v1),              \
    ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *:                        \
        ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update_v1),                 \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *:                      \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config_v1),               \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *:                 \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update_v1),          \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *:           \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update_v1),    \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *:      \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_ਮਾਪ_min(x) _Generic(                  \
    (x),                                                            \
    ਅਚਲ ਬਣਤਰ xkb_events_config *:                               \
        ਆਕਾਰ(ਬਣਤਰ xkb_events_config_v1),                        \
    ਬਣਤਰ xkb_event_components *:                                  \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_components_v1),                     \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *:                              \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion_v1),                 \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_button *:                              \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button_v1),                 \
    ਅਚਲ ਬਣਤਰ xkb_synthetic_update *:                            \
        ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update_v1),                     \
    ਅਚਲ ਬਣਤਰ xkb_state_components_update *:                     \
        ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update_v1),              \
    ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *:                        \
        ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update_v1),                 \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *:                      \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config_v1),               \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *:                 \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update_v1),          \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *:           \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update_v1),    \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *:      \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_versioned_struct_reserved_ਵਿੱਥ(x) _Generic(        \
    (x),                                                         \
    ਅਚਲ ਬਣਤਰ xkb_events_config *:                            \
        ਆਕਾਰ(ਬਣਤਰ xkb_events_config),                        \
    ਬਣਤਰ xkb_event_components *:                               \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_components),                     \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *:                           \
        ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion),                 \
    ਬਣਤਰ xkb_event_ਸੂਚਕ_button *:                           \
        offsetof(ਬਣਤਰ xkb_event_ਸੂਚਕ_button, reserved0),    \
    ਅਚਲ ਬਣਤਰ xkb_synthetic_update *:                         \
        ਆਕਾਰ(ਬਣਤਰ xkb_synthetic_update),                     \
    ਅਚਲ ਬਣਤਰ xkb_state_components_update *:                  \
        ਆਕਾਰ(ਬਣਤਰ xkb_state_components_update),              \
    ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *:                     \
        ਆਕਾਰ(ਬਣਤਰ xkb_layout_policy_update),                 \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *:                   \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_config),               \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *:              \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_a11y_update),          \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *:        \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_mods_remap_update),    \
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *:   \
        ਆਕਾਰ(ਬਣਤਰ xkb_machine_builder_shortcut_override_update)\
)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_check_state_abi(x) xkb_check_versioned_struct_ਮਾਪ_2( \
    xkb_versioned_struct_ਮਾਪ_v1(x),                            \
    xkb_versioned_struct_ਮਾਪ_min(x),                           \
    xkb_versioned_struct_reserved_ਵਿੱਥ(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_events_config *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_events_config *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਬਣਤਰ xkb_event_components *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_components *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਬਣਤਰ xkb_event_ਸੂਚਕ_button *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_ਸੂਚਕ_button *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_synthetic_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_synthetic_update *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_state_components_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_state_components_update *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *)ਨਿਰਾ)),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_v1(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *)ਨਿਰਾ)) <=
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *)ਨਿਰਾ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_events_config *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_components *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਬਣਤਰ xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_ਸੂਚਕ_motion *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_motion),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਬਣਤਰ xkb_event_ਸੂਚਕ_button*)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਬਣਤਰ xkb_event_ਸੂਚਕ_button),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_synthetic_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_state_components_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_config *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_ਮਾਪ_min(((ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update *)ਨਿਰਾ)) <=
    ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_ਨਿੱਜੀ ਬਣਤਰ xkb_state *
xkb_machine_get_state(ਬਣਤਰ xkb_machine *machine);
