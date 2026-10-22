/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdalign.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "keymap.h"

/** Core keyboard state components */
संरचना state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t आधारः_गणः; /**< depressed */
    int32_t latched_गणः;
    int32_t locked_गणः;
    xkb_layout_सूचकांक_t गणः; /**< effective */

    xkb_mod_आवरणम्_t आधारः_mods; /**< depressed */
    xkb_mod_आवरणम्_t latched_mods;
    xkb_mod_आवरणम्_t locked_mods;
    xkb_mod_आवरणम्_t mods; /**< effective */

    xkb_led_आवरणम्_t leds;

    गणना xkb_action_controls controls; /**< effective */
};

संरचना xkb_event {
    संरचना xkb_context *ctx;
    गणना xkb_event_type type;
    सङ्घ {
        संरचना {
            xkb_keycode_t keycode;
            गणना xkb_कुञ्चिका_direction direction;
        } कुञ्चिका;
        संरचना {
            संरचना state_components components;
            गणना xkb_state_component परिवर्तितम्;
        } components;
        संरचना xkb_event_pointer_motion pointer_motion;
        संरचना xkb_event_pointer_button pointer_button;
        संरचना {
            int8_t सूचकांक_or_विस्थापनम्;
            bool is_विस्थापनम्;
        } virtual_console;
    };
};

XKB_EXPORT_वैयक्तिकम् शून्य
xkb_state_reset(संरचना xkb_state *state);

XKB_EXPORT_वैयक्तिकम् xkb_led_आवरणम्_t
xkb_state_serialize_leds(ध्रुव संरचना xkb_state *state,
                         गणना xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_events_config_v1 {
    uint32_t आकार;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_events_config, आकार, flags);
assert_no_padding(संरचना xkb_events_config, flags);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_events_config) ==
              परिमाण(संरचना xkb_events_config_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_events_config, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_events_config) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_event_components_v1 {
    uint32_t आकार;
    uint32_t परिवर्तितम्;
    xkb_mod_आवरणम्_t depressed_mods;
    xkb_mod_आवरणम्_t latched_mods;
    xkb_mod_आवरणम्_t locked_mods;
    xkb_mod_आवरणम्_t mods;
    xkb_layout_सूचकांक_t depressed_layout;
    xkb_layout_सूचकांक_t latched_layout;
    xkb_layout_सूचकांक_t locked_layout;
    xkb_layout_सूचकांक_t layout;
    xkb_led_आवरणम्_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_event_components, आकार, परिवर्तितम्);
assert_no_padding(संरचना xkb_event_components, परिवर्तितम्, depressed_mods);
assert_no_padding(संरचना xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(संरचना xkb_event_components, latched_mods, locked_mods);
assert_no_padding(संरचना xkb_event_components, locked_mods, mods);
assert_no_padding(संरचना xkb_event_components, mods, depressed_layout);
assert_no_padding(संरचना xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(संरचना xkb_event_components, latched_layout, locked_layout);
assert_no_padding(संरचना xkb_event_components, locked_layout, layout);
assert_no_padding(संरचना xkb_event_components, layout, leds);
assert_no_padding(संरचना xkb_event_components, leds, controls);
assert_no_padding(संरचना xkb_event_components, controls);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_event_components) ==
              परिमाण(संरचना xkb_event_components_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, परिवर्तितम्);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, depressed_mods);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, latched_mods);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, locked_mods);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, mods);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, depressed_layout);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, latched_layout);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, locked_layout);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, layout);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, leds);
assert_same_क्षेत्रम्(संरचना xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_event_components) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_event_pointer_motion_v1 {
    uint32_t आकार;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_event_pointer_motion, आकार, flags);
assert_no_padding(संरचना xkb_event_pointer_motion, flags, x);
assert_no_padding(संरचना xkb_event_pointer_motion, x, y);
assert_no_padding(संरचना xkb_event_pointer_motion, y);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_event_pointer_motion) ==
              परिमाण(संरचना xkb_event_pointer_motion_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_motion, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_motion, _v1, flags);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_motion, _v1, x);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_event_pointer_motion) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_event_pointer_button_v1 {
    uint32_t आकार;
    uint32_t button;
    uint8_t गणनम्;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_event_pointer_button, आकार, button);
assert_no_padding(संरचना xkb_event_pointer_button, button, गणनम्);
assert_no_padding(संरचना xkb_event_pointer_button, गणनम्, state);
assert_no_padding(संरचना xkb_event_pointer_button, state, reserved0);
assert_no_padding(संरचना xkb_event_pointer_button, reserved0);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_event_pointer_button) ==
              परिमाण(संरचना xkb_event_pointer_button_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_button, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_button, _v1, button);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_button, _v1, गणनम्);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_button, _v1, state);
assert_same_क्षेत्रम्(संरचना xkb_event_pointer_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_event_pointer_button) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_state_components_update_v1 {
    uint32_t आकार;
    uint32_t components;
    xkb_mod_आवरणम्_t affect_latched_mods;
    xkb_mod_आवरणम्_t latched_mods;
    xkb_mod_आवरणम्_t affect_locked_mods;
    xkb_mod_आवरणम्_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_state_components_update, आकार, components);
assert_no_padding(संरचना xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(संरचना xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(संरचना xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(संरचना xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(संरचना xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(संरचना xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(संरचना xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(संरचना xkb_state_components_update, affect_controls, controls);
assert_no_padding(संरचना xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_state_components_update) ==
              परिमाण(संरचना xkb_state_components_update_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, components);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, affect_latched_mods);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, latched_mods);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, affect_locked_mods);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, locked_mods);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, latched_layout);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, locked_layout);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, affect_controls);
assert_same_क्षेत्रम्(संरचना xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_state_components_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_layout_policy_update_v1 {
    uint32_t आकार;
    uint32_t policy;
    xkb_layout_सूचकांक_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_layout_policy_update, आकार, policy);
assert_no_padding(संरचना xkb_layout_policy_update, policy, redirect);
assert_no_padding(संरचना xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_layout_policy_update) ==
              परिमाण(संरचना xkb_layout_policy_update_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_layout_policy_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_layout_policy_update, _v1, policy);
assert_same_क्षेत्रम्(संरचना xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_layout_policy_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_synthetic_update_v1 {
    uint32_t आकार;
    uint32_t reserved0;
    ध्रुव संरचना xkb_state_components_update_v1 *components;
    ध्रुव संरचना xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_synthetic_update, आकार, reserved0);
assert_no_padding(संरचना xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(संरचना xkb_synthetic_update, components, layout_policy);
assert_no_padding(संरचना xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_synthetic_update) ==
              परिमाण(संरचना xkb_synthetic_update_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_synthetic_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_क्षेत्रम्(संरचना xkb_synthetic_update, _v1, components);
assert_same_क्षेत्रम्(संरचना xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_synthetic_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_machine_builder_config_v1 {
    uint32_t आकार;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_machine_builder_config, आकार, builder_flags);
assert_no_padding(संरचना xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(संरचना xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_machine_builder_config) ==
              परिमाण(संरचना xkb_machine_builder_config_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_config, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_config, _v1, builder_flags);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_machine_builder_config) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_machine_builder_a11y_update_v1 {
    uint32_t आकार;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_machine_builder_a11y_update, आकार, affect_flags);
assert_no_padding(संरचना xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(संरचना xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_machine_builder_a11y_update) ==
              परिमाण(संरचना xkb_machine_builder_a11y_update_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_a11y_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_machine_builder_a11y_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_machine_builder_mods_remap_update_v1 {
    uint32_t आकार;
    xkb_mod_आवरणम्_t source;
    xkb_mod_आवरणम्_t target;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_machine_builder_mods_remap_update, आकार, source);
assert_no_padding(संरचना xkb_machine_builder_mods_remap_update, source, target);
assert_no_padding(संरचना xkb_machine_builder_mods_remap_update, target);

/* Current version is 1 */
static_assert(
    परिमाण(संरचना xkb_machine_builder_mods_remap_update) ==
    परिमाण(संरचना xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_mods_remap_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_mods_remap_update, _v1, source);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_mods_remap_update, _v1, target);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_machine_builder_mods_remap_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
संरचना xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t आकार;
    xkb_layout_सूचकांक_t source;
    xkb_layout_सूचकांक_t target;
    xkb_mod_आवरणम्_t affect_mods;
    xkb_mod_आवरणम्_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(संरचना xkb_machine_builder_shortcut_override_update, आकार, source);
assert_no_padding(संरचना xkb_machine_builder_shortcut_override_update, source, target);
assert_no_padding(संरचना xkb_machine_builder_shortcut_override_update, target, affect_mods);
assert_no_padding(संरचना xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(संरचना xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(परिमाण(संरचना xkb_machine_builder_shortcut_override_update) ==
              परिमाण(संरचना xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_shortcut_override_update, _v1, आकार);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_shortcut_override_update, _v1, source);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_shortcut_override_update, _v1, target);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_क्षेत्रम्(संरचना xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(परिमाण(संरचना xkb_machine_builder_shortcut_override_update) * 30 <=
              (आकार_t)XKB_ABI_MAX_आकार, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#परिभाषय xkb_versioned_struct_आकार_v1(x) _Generic(                   \
    (x),                                                            \
    ध्रुव संरचना xkb_events_config *:                               \
        परिमाण(संरचना xkb_events_config_v1),                        \
    संरचना xkb_event_components *:                                  \
        परिमाण(संरचना xkb_event_components_v1),                     \
    संरचना xkb_event_pointer_motion *:                              \
        परिमाण(संरचना xkb_event_pointer_motion_v1),                 \
    संरचना xkb_event_pointer_button *:                              \
        परिमाण(संरचना xkb_event_pointer_button_v1),                 \
    ध्रुव संरचना xkb_synthetic_update *:                            \
        परिमाण(संरचना xkb_synthetic_update_v1),                     \
    ध्रुव संरचना xkb_state_components_update *:                     \
        परिमाण(संरचना xkb_state_components_update_v1),              \
    ध्रुव संरचना xkb_layout_policy_update *:                        \
        परिमाण(संरचना xkb_layout_policy_update_v1),                 \
    ध्रुव संरचना xkb_machine_builder_config *:                      \
        परिमाण(संरचना xkb_machine_builder_config_v1),               \
    ध्रुव संरचना xkb_machine_builder_a11y_update *:                 \
        परिमाण(संरचना xkb_machine_builder_a11y_update_v1),          \
    ध्रुव संरचना xkb_machine_builder_mods_remap_update *:           \
        परिमाण(संरचना xkb_machine_builder_mods_remap_update_v1),    \
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update *:      \
        परिमाण(संरचना xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#परिभाषय xkb_versioned_struct_आकार_min(x) _Generic(                  \
    (x),                                                            \
    ध्रुव संरचना xkb_events_config *:                               \
        परिमाण(संरचना xkb_events_config_v1),                        \
    संरचना xkb_event_components *:                                  \
        परिमाण(संरचना xkb_event_components_v1),                     \
    संरचना xkb_event_pointer_motion *:                              \
        परिमाण(संरचना xkb_event_pointer_motion_v1),                 \
    संरचना xkb_event_pointer_button *:                              \
        परिमाण(संरचना xkb_event_pointer_button_v1),                 \
    ध्रुव संरचना xkb_synthetic_update *:                            \
        परिमाण(संरचना xkb_synthetic_update_v1),                     \
    ध्रुव संरचना xkb_state_components_update *:                     \
        परिमाण(संरचना xkb_state_components_update_v1),              \
    ध्रुव संरचना xkb_layout_policy_update *:                        \
        परिमाण(संरचना xkb_layout_policy_update_v1),                 \
    ध्रुव संरचना xkb_machine_builder_config *:                      \
        परिमाण(संरचना xkb_machine_builder_config_v1),               \
    ध्रुव संरचना xkb_machine_builder_a11y_update *:                 \
        परिमाण(संरचना xkb_machine_builder_a11y_update_v1),          \
    ध्रुव संरचना xkb_machine_builder_mods_remap_update *:           \
        परिमाण(संरचना xkb_machine_builder_mods_remap_update_v1),    \
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update *:      \
        परिमाण(संरचना xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#परिभाषय xkb_versioned_struct_reserved_विस्थापनम्(x) _Generic(        \
    (x),                                                         \
    ध्रुव संरचना xkb_events_config *:                            \
        परिमाण(संरचना xkb_events_config),                        \
    संरचना xkb_event_components *:                               \
        परिमाण(संरचना xkb_event_components),                     \
    संरचना xkb_event_pointer_motion *:                           \
        परिमाण(संरचना xkb_event_pointer_motion),                 \
    संरचना xkb_event_pointer_button *:                           \
        offsetof(संरचना xkb_event_pointer_button, reserved0),    \
    ध्रुव संरचना xkb_synthetic_update *:                         \
        परिमाण(संरचना xkb_synthetic_update),                     \
    ध्रुव संरचना xkb_state_components_update *:                  \
        परिमाण(संरचना xkb_state_components_update),              \
    ध्रुव संरचना xkb_layout_policy_update *:                     \
        परिमाण(संरचना xkb_layout_policy_update),                 \
    ध्रुव संरचना xkb_machine_builder_config *:                   \
        परिमाण(संरचना xkb_machine_builder_config),               \
    ध्रुव संरचना xkb_machine_builder_a11y_update *:              \
        परिमाण(संरचना xkb_machine_builder_a11y_update),          \
    ध्रुव संरचना xkb_machine_builder_mods_remap_update *:        \
        परिमाण(संरचना xkb_machine_builder_mods_remap_update),    \
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update *:   \
        परिमाण(संरचना xkb_machine_builder_shortcut_override_update)\
)

#परिभाषय xkb_check_state_abi(x) xkb_check_versioned_struct_आकार_2( \
    xkb_versioned_struct_आकार_v1(x),                            \
    xkb_versioned_struct_आकार_min(x),                           \
    xkb_versioned_struct_reserved_विस्थापनम्(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_events_config *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_events_config *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((संरचना xkb_event_components *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_components *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((संरचना xkb_event_pointer_motion *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_pointer_motion *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((संरचना xkb_event_pointer_button *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_pointer_button *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_synthetic_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_synthetic_update *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_state_components_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_state_components_update *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_layout_policy_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_layout_policy_update *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_machine_builder_config *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_config *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_machine_builder_a11y_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_a11y_update *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_machine_builder_mods_remap_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_mods_remap_update *)रिक्त)),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_v1(((ध्रुव संरचना xkb_machine_builder_shortcut_override_update *)रिक्त)) <=
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_shortcut_override_update *)रिक्त)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_events_config *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_components *)रिक्त)) <=
    परिमाण(संरचना xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_pointer_motion *)रिक्त)) <=
    परिमाण(संरचना xkb_event_pointer_motion),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((संरचना xkb_event_pointer_button*)रिक्त)) <=
    परिमाण(संरचना xkb_event_pointer_button),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_synthetic_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_state_components_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_layout_policy_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_config *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_a11y_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_mods_remap_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_आकार_min(((ध्रुव संरचना xkb_machine_builder_shortcut_override_update *)रिक्त)) <=
    परिमाण(ध्रुव संरचना xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_वैयक्तिकम् संरचना xkb_state *
xkb_machine_get_state(संरचना xkb_machine *machine);
