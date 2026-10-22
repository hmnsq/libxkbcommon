/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdalign.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>

#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "abi-check.h"
#అన్తర్భావ "keymap.h"

/** Core keyboard state components */
నిర్మాణం state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t పునాది_సమూహం; /**< depressed */
    int32_t latched_సమూహం;
    int32_t locked_సమూహం;
    xkb_layout_సూచీ_t సమూహం; /**< effective */

    xkb_mod_ముసుగు_t పునాది_mods; /**< depressed */
    xkb_mod_ముసుగు_t latched_mods;
    xkb_mod_ముసుగు_t locked_mods;
    xkb_mod_ముసుగు_t mods; /**< effective */

    xkb_led_ముసుగు_t leds;

    గణన xkb_action_controls controls; /**< effective */
};

నిర్మాణం xkb_event {
    నిర్మాణం xkb_context *ctx;
    గణన xkb_event_type type;
    సంఘం {
        నిర్మాణం {
            xkb_keycode_t keycode;
            గణన xkb_కీలకం_direction direction;
        } కీలకం;
        నిర్మాణం {
            నిర్మాణం state_components components;
            గణన xkb_state_component మారింది;
        } components;
        నిర్మాణం xkb_event_సూచిక_motion సూచిక_motion;
        నిర్మాణం xkb_event_సూచిక_button సూచిక_button;
        నిర్మాణం {
            int8_t సూచీ_or_స్థానభ్రంశం;
            bool is_స్థానభ్రంశం;
        } virtual_console;
    };
};

XKB_EXPORT_వ్యక్తిగత శూన్య
xkb_state_reset(నిర్మాణం xkb_state *state);

XKB_EXPORT_వ్యక్తిగత xkb_led_ముసుగు_t
xkb_state_serialize_leds(స్థిరరాశి నిర్మాణం xkb_state *state,
                         గణన xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_events_config_v1 {
    uint32_t కొలత;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_events_config, కొలత, flags);
assert_no_padding(నిర్మాణం xkb_events_config, flags);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_events_config) ==
              పరిమాణం(నిర్మాణం xkb_events_config_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_events_config, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_events_config) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_event_components_v1 {
    uint32_t కొలత;
    uint32_t మారింది;
    xkb_mod_ముసుగు_t depressed_mods;
    xkb_mod_ముసుగు_t latched_mods;
    xkb_mod_ముసుగు_t locked_mods;
    xkb_mod_ముసుగు_t mods;
    xkb_layout_సూచీ_t depressed_layout;
    xkb_layout_సూచీ_t latched_layout;
    xkb_layout_సూచీ_t locked_layout;
    xkb_layout_సూచీ_t layout;
    xkb_led_ముసుగు_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_event_components, కొలత, మారింది);
assert_no_padding(నిర్మాణం xkb_event_components, మారింది, depressed_mods);
assert_no_padding(నిర్మాణం xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(నిర్మాణం xkb_event_components, latched_mods, locked_mods);
assert_no_padding(నిర్మాణం xkb_event_components, locked_mods, mods);
assert_no_padding(నిర్మాణం xkb_event_components, mods, depressed_layout);
assert_no_padding(నిర్మాణం xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(నిర్మాణం xkb_event_components, latched_layout, locked_layout);
assert_no_padding(నిర్మాణం xkb_event_components, locked_layout, layout);
assert_no_padding(నిర్మాణం xkb_event_components, layout, leds);
assert_no_padding(నిర్మాణం xkb_event_components, leds, controls);
assert_no_padding(నిర్మాణం xkb_event_components, controls);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_event_components) ==
              పరిమాణం(నిర్మాణం xkb_event_components_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, మారింది);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, depressed_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, latched_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, locked_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, depressed_layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, latched_layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, locked_layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, leds);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_event_components) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_event_సూచిక_motion_v1 {
    uint32_t కొలత;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_event_సూచిక_motion, కొలత, flags);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_motion, flags, x);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_motion, x, y);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_motion, y);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion) ==
              పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_motion, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_motion, _v1, flags);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_motion, _v1, x);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_event_సూచిక_button_v1 {
    uint32_t కొలత;
    uint32_t button;
    uint8_t లెక్క;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_event_సూచిక_button, కొలత, button);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_button, button, లెక్క);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_button, లెక్క, state);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_button, state, reserved0);
assert_no_padding(నిర్మాణం xkb_event_సూచిక_button, reserved0);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_event_సూచిక_button) ==
              పరిమాణం(నిర్మాణం xkb_event_సూచిక_button_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_button, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_button, _v1, button);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_button, _v1, లెక్క);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_button, _v1, state);
assert_same_క్షేత్రం(నిర్మాణం xkb_event_సూచిక_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_event_సూచిక_button) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_state_components_update_v1 {
    uint32_t కొలత;
    uint32_t components;
    xkb_mod_ముసుగు_t affect_latched_mods;
    xkb_mod_ముసుగు_t latched_mods;
    xkb_mod_ముసుగు_t affect_locked_mods;
    xkb_mod_ముసుగు_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_state_components_update, కొలత, components);
assert_no_padding(నిర్మాణం xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(నిర్మాణం xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(నిర్మాణం xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(నిర్మాణం xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(నిర్మాణం xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(నిర్మాణం xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(నిర్మాణం xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(నిర్మాణం xkb_state_components_update, affect_controls, controls);
assert_no_padding(నిర్మాణం xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_state_components_update) ==
              పరిమాణం(నిర్మాణం xkb_state_components_update_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, components);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, affect_latched_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, latched_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, affect_locked_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, locked_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, latched_layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, locked_layout);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, affect_controls);
assert_same_క్షేత్రం(నిర్మాణం xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_state_components_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_layout_policy_update_v1 {
    uint32_t కొలత;
    uint32_t policy;
    xkb_layout_సూచీ_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_layout_policy_update, కొలత, policy);
assert_no_padding(నిర్మాణం xkb_layout_policy_update, policy, redirect);
assert_no_padding(నిర్మాణం xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_layout_policy_update) ==
              పరిమాణం(నిర్మాణం xkb_layout_policy_update_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_layout_policy_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_layout_policy_update, _v1, policy);
assert_same_క్షేత్రం(నిర్మాణం xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_layout_policy_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_synthetic_update_v1 {
    uint32_t కొలత;
    uint32_t reserved0;
    స్థిరరాశి నిర్మాణం xkb_state_components_update_v1 *components;
    స్థిరరాశి నిర్మాణం xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_synthetic_update, కొలత, reserved0);
assert_no_padding(నిర్మాణం xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(నిర్మాణం xkb_synthetic_update, components, layout_policy);
assert_no_padding(నిర్మాణం xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_synthetic_update) ==
              పరిమాణం(నిర్మాణం xkb_synthetic_update_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_synthetic_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_క్షేత్రం(నిర్మాణం xkb_synthetic_update, _v1, components);
assert_same_క్షేత్రం(నిర్మాణం xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_synthetic_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_machine_builder_config_v1 {
    uint32_t కొలత;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_machine_builder_config, కొలత, builder_flags);
assert_no_padding(నిర్మాణం xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(నిర్మాణం xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_config) ==
              పరిమాణం(నిర్మాణం xkb_machine_builder_config_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_config, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_config, _v1, builder_flags);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_config) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_machine_builder_a11y_update_v1 {
    uint32_t కొలత;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_machine_builder_a11y_update, కొలత, affect_flags);
assert_no_padding(నిర్మాణం xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(నిర్మాణం xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update) ==
              పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_a11y_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_machine_builder_mods_remap_update_v1 {
    uint32_t కొలత;
    xkb_mod_ముసుగు_t ఆధారం;
    xkb_mod_ముసుగు_t లక్ష్యం;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_machine_builder_mods_remap_update, కొలత, ఆధారం);
assert_no_padding(నిర్మాణం xkb_machine_builder_mods_remap_update, ఆధారం, లక్ష్యం);
assert_no_padding(నిర్మాణం xkb_machine_builder_mods_remap_update, లక్ష్యం);

/* Current version is 1 */
static_assert(
    పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update) ==
    పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_mods_remap_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_mods_remap_update, _v1, ఆధారం);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_mods_remap_update, _v1, లక్ష్యం);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
నిర్మాణం xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t కొలత;
    xkb_layout_సూచీ_t ఆధారం;
    xkb_layout_సూచీ_t లక్ష్యం;
    xkb_mod_ముసుగు_t affect_mods;
    xkb_mod_ముసుగు_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(నిర్మాణం xkb_machine_builder_shortcut_override_update, కొలత, ఆధారం);
assert_no_padding(నిర్మాణం xkb_machine_builder_shortcut_override_update, ఆధారం, లక్ష్యం);
assert_no_padding(నిర్మాణం xkb_machine_builder_shortcut_override_update, లక్ష్యం, affect_mods);
assert_no_padding(నిర్మాణం xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(నిర్మాణం xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update) ==
              పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_shortcut_override_update, _v1, కొలత);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_shortcut_override_update, _v1, ఆధారం);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_shortcut_override_update, _v1, లక్ష్యం);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_క్షేత్రం(నిర్మాణం xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update) * 30 <=
              (కొలత_t)XKB_ABI_MAX_కొలత, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#నిర్వచించు xkb_versioned_struct_కొలత_v1(x) _Generic(                   \
    (x),                                                            \
    స్థిరరాశి నిర్మాణం xkb_events_config *:                               \
        పరిమాణం(నిర్మాణం xkb_events_config_v1),                        \
    నిర్మాణం xkb_event_components *:                                  \
        పరిమాణం(నిర్మాణం xkb_event_components_v1),                     \
    నిర్మాణం xkb_event_సూచిక_motion *:                              \
        పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion_v1),                 \
    నిర్మాణం xkb_event_సూచిక_button *:                              \
        పరిమాణం(నిర్మాణం xkb_event_సూచిక_button_v1),                 \
    స్థిరరాశి నిర్మాణం xkb_synthetic_update *:                            \
        పరిమాణం(నిర్మాణం xkb_synthetic_update_v1),                     \
    స్థిరరాశి నిర్మాణం xkb_state_components_update *:                     \
        పరిమాణం(నిర్మాణం xkb_state_components_update_v1),              \
    స్థిరరాశి నిర్మాణం xkb_layout_policy_update *:                        \
        పరిమాణం(నిర్మాణం xkb_layout_policy_update_v1),                 \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_config *:                      \
        పరిమాణం(నిర్మాణం xkb_machine_builder_config_v1),               \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *:                 \
        పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update_v1),          \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *:           \
        పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update_v1),    \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *:      \
        పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#నిర్వచించు xkb_versioned_struct_కొలత_min(x) _Generic(                  \
    (x),                                                            \
    స్థిరరాశి నిర్మాణం xkb_events_config *:                               \
        పరిమాణం(నిర్మాణం xkb_events_config_v1),                        \
    నిర్మాణం xkb_event_components *:                                  \
        పరిమాణం(నిర్మాణం xkb_event_components_v1),                     \
    నిర్మాణం xkb_event_సూచిక_motion *:                              \
        పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion_v1),                 \
    నిర్మాణం xkb_event_సూచిక_button *:                              \
        పరిమాణం(నిర్మాణం xkb_event_సూచిక_button_v1),                 \
    స్థిరరాశి నిర్మాణం xkb_synthetic_update *:                            \
        పరిమాణం(నిర్మాణం xkb_synthetic_update_v1),                     \
    స్థిరరాశి నిర్మాణం xkb_state_components_update *:                     \
        పరిమాణం(నిర్మాణం xkb_state_components_update_v1),              \
    స్థిరరాశి నిర్మాణం xkb_layout_policy_update *:                        \
        పరిమాణం(నిర్మాణం xkb_layout_policy_update_v1),                 \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_config *:                      \
        పరిమాణం(నిర్మాణం xkb_machine_builder_config_v1),               \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *:                 \
        పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update_v1),          \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *:           \
        పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update_v1),    \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *:      \
        పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#నిర్వచించు xkb_versioned_struct_reserved_స్థానభ్రంశం(x) _Generic(        \
    (x),                                                         \
    స్థిరరాశి నిర్మాణం xkb_events_config *:                            \
        పరిమాణం(నిర్మాణం xkb_events_config),                        \
    నిర్మాణం xkb_event_components *:                               \
        పరిమాణం(నిర్మాణం xkb_event_components),                     \
    నిర్మాణం xkb_event_సూచిక_motion *:                           \
        పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion),                 \
    నిర్మాణం xkb_event_సూచిక_button *:                           \
        offsetof(నిర్మాణం xkb_event_సూచిక_button, reserved0),    \
    స్థిరరాశి నిర్మాణం xkb_synthetic_update *:                         \
        పరిమాణం(నిర్మాణం xkb_synthetic_update),                     \
    స్థిరరాశి నిర్మాణం xkb_state_components_update *:                  \
        పరిమాణం(నిర్మాణం xkb_state_components_update),              \
    స్థిరరాశి నిర్మాణం xkb_layout_policy_update *:                     \
        పరిమాణం(నిర్మాణం xkb_layout_policy_update),                 \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_config *:                   \
        పరిమాణం(నిర్మాణం xkb_machine_builder_config),               \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *:              \
        పరిమాణం(నిర్మాణం xkb_machine_builder_a11y_update),          \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *:        \
        పరిమాణం(నిర్మాణం xkb_machine_builder_mods_remap_update),    \
    స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *:   \
        పరిమాణం(నిర్మాణం xkb_machine_builder_shortcut_override_update)\
)

#నిర్వచించు xkb_check_state_abi(x) xkb_check_versioned_struct_కొలత_2( \
    xkb_versioned_struct_కొలత_v1(x),                            \
    xkb_versioned_struct_కొలత_min(x),                           \
    xkb_versioned_struct_reserved_స్థానభ్రంశం(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_events_config *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_events_config *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((నిర్మాణం xkb_event_components *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_components *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((నిర్మాణం xkb_event_సూచిక_motion *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_సూచిక_motion *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((నిర్మాణం xkb_event_సూచిక_button *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_సూచిక_button *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_synthetic_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_synthetic_update *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_state_components_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_state_components_update *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_layout_policy_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_layout_policy_update *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_machine_builder_config *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_config *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *)ఖాళీ)),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_v1(((స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *)ఖాళీ)) <=
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *)ఖాళీ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_events_config *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_components *)ఖాళీ)) <=
    పరిమాణం(నిర్మాణం xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_సూచిక_motion *)ఖాళీ)) <=
    పరిమాణం(నిర్మాణం xkb_event_సూచిక_motion),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((నిర్మాణం xkb_event_సూచిక_button*)ఖాళీ)) <=
    పరిమాణం(నిర్మాణం xkb_event_సూచిక_button),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_synthetic_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_state_components_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_layout_policy_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_config *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_కొలత_min(((స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update *)ఖాళీ)) <=
    పరిమాణం(స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_వ్యక్తిగత నిర్మాణం xkb_state *
xkb_machine_get_state(నిర్మాణం xkb_machine *machine);
