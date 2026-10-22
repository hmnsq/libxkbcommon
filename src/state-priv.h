/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <assert.h>
#شامل <stdalign.h>
#شامل <stddef.h>
#شامل <stdint.h>

#شامل "utils.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "abi-check.h"
#شامل "keymap.h"

/** Core keyboard state components */
ساختار state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t base_group; /**< depressed */
    int32_t latched_group;
    int32_t locked_group;
    xkb_layout_index_t group; /**< effective */

    xkb_mod_mask_t base_mods; /**< depressed */
    xkb_mod_mask_t latched_mods;
    xkb_mod_mask_t locked_mods;
    xkb_mod_mask_t mods; /**< effective */

    xkb_led_mask_t leds;

    شمارش xkb_action_controls controls; /**< effective */
};

ساختار xkb_event {
    ساختار xkb_context *ctx;
    شمارش xkb_event_type type;
    اجتماع {
        ساختار {
            xkb_keycode_t keycode;
            شمارش xkb_کلید_direction direction;
        } کلید;
        ساختار {
            ساختار state_components components;
            شمارش xkb_state_component changed;
        } components;
        ساختار xkb_event_pointer_motion pointer_motion;
        ساختار xkb_event_pointer_button pointer_button;
        ساختار {
            int8_t index_or_offset;
            bool is_offset;
        } virtual_console;
    };
};

XKB_EXPORT_PRIVATE تهی
xkb_state_reset(ساختار xkb_state *state);

XKB_EXPORT_PRIVATE xkb_led_mask_t
xkb_state_serialize_leds(ثابت ساختار xkb_state *state,
                         شمارش xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_events_config_v1 {
    uint32_t size;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_events_config, size, flags);
assert_no_padding(ساختار xkb_events_config, flags);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_events_config) ==
              اندازه(ساختار xkb_events_config_v1), "");
assert_same_field(ساختار xkb_events_config, _v1, size);
assert_same_field(ساختار xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_events_config) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_event_components_v1 {
    uint32_t size;
    uint32_t changed;
    xkb_mod_mask_t depressed_mods;
    xkb_mod_mask_t latched_mods;
    xkb_mod_mask_t locked_mods;
    xkb_mod_mask_t mods;
    xkb_layout_index_t depressed_layout;
    xkb_layout_index_t latched_layout;
    xkb_layout_index_t locked_layout;
    xkb_layout_index_t layout;
    xkb_led_mask_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_event_components, size, changed);
assert_no_padding(ساختار xkb_event_components, changed, depressed_mods);
assert_no_padding(ساختار xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(ساختار xkb_event_components, latched_mods, locked_mods);
assert_no_padding(ساختار xkb_event_components, locked_mods, mods);
assert_no_padding(ساختار xkb_event_components, mods, depressed_layout);
assert_no_padding(ساختار xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(ساختار xkb_event_components, latched_layout, locked_layout);
assert_no_padding(ساختار xkb_event_components, locked_layout, layout);
assert_no_padding(ساختار xkb_event_components, layout, leds);
assert_no_padding(ساختار xkb_event_components, leds, controls);
assert_no_padding(ساختار xkb_event_components, controls);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_event_components) ==
              اندازه(ساختار xkb_event_components_v1), "");
assert_same_field(ساختار xkb_event_components, _v1, size);
assert_same_field(ساختار xkb_event_components, _v1, changed);
assert_same_field(ساختار xkb_event_components, _v1, depressed_mods);
assert_same_field(ساختار xkb_event_components, _v1, latched_mods);
assert_same_field(ساختار xkb_event_components, _v1, locked_mods);
assert_same_field(ساختار xkb_event_components, _v1, mods);
assert_same_field(ساختار xkb_event_components, _v1, depressed_layout);
assert_same_field(ساختار xkb_event_components, _v1, latched_layout);
assert_same_field(ساختار xkb_event_components, _v1, locked_layout);
assert_same_field(ساختار xkb_event_components, _v1, layout);
assert_same_field(ساختار xkb_event_components, _v1, leds);
assert_same_field(ساختار xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_event_components) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_event_pointer_motion_v1 {
    uint32_t size;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_event_pointer_motion, size, flags);
assert_no_padding(ساختار xkb_event_pointer_motion, flags, x);
assert_no_padding(ساختار xkb_event_pointer_motion, x, y);
assert_no_padding(ساختار xkb_event_pointer_motion, y);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_event_pointer_motion) ==
              اندازه(ساختار xkb_event_pointer_motion_v1), "");
assert_same_field(ساختار xkb_event_pointer_motion, _v1, size);
assert_same_field(ساختار xkb_event_pointer_motion, _v1, flags);
assert_same_field(ساختار xkb_event_pointer_motion, _v1, x);
assert_same_field(ساختار xkb_event_pointer_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_event_pointer_motion) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_event_pointer_button_v1 {
    uint32_t size;
    uint32_t button;
    uint8_t شمار;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_event_pointer_button, size, button);
assert_no_padding(ساختار xkb_event_pointer_button, button, شمار);
assert_no_padding(ساختار xkb_event_pointer_button, شمار, state);
assert_no_padding(ساختار xkb_event_pointer_button, state, reserved0);
assert_no_padding(ساختار xkb_event_pointer_button, reserved0);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_event_pointer_button) ==
              اندازه(ساختار xkb_event_pointer_button_v1), "");
assert_same_field(ساختار xkb_event_pointer_button, _v1, size);
assert_same_field(ساختار xkb_event_pointer_button, _v1, button);
assert_same_field(ساختار xkb_event_pointer_button, _v1, شمار);
assert_same_field(ساختار xkb_event_pointer_button, _v1, state);
assert_same_field(ساختار xkb_event_pointer_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_event_pointer_button) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_state_components_update_v1 {
    uint32_t size;
    uint32_t components;
    xkb_mod_mask_t affect_latched_mods;
    xkb_mod_mask_t latched_mods;
    xkb_mod_mask_t affect_locked_mods;
    xkb_mod_mask_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_state_components_update, size, components);
assert_no_padding(ساختار xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(ساختار xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(ساختار xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(ساختار xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(ساختار xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(ساختار xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(ساختار xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(ساختار xkb_state_components_update, affect_controls, controls);
assert_no_padding(ساختار xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_state_components_update) ==
              اندازه(ساختار xkb_state_components_update_v1), "");
assert_same_field(ساختار xkb_state_components_update, _v1, size);
assert_same_field(ساختار xkb_state_components_update, _v1, components);
assert_same_field(ساختار xkb_state_components_update, _v1, affect_latched_mods);
assert_same_field(ساختار xkb_state_components_update, _v1, latched_mods);
assert_same_field(ساختار xkb_state_components_update, _v1, affect_locked_mods);
assert_same_field(ساختار xkb_state_components_update, _v1, locked_mods);
assert_same_field(ساختار xkb_state_components_update, _v1, latched_layout);
assert_same_field(ساختار xkb_state_components_update, _v1, locked_layout);
assert_same_field(ساختار xkb_state_components_update, _v1, affect_controls);
assert_same_field(ساختار xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_state_components_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_layout_policy_update_v1 {
    uint32_t size;
    uint32_t policy;
    xkb_layout_index_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_layout_policy_update, size, policy);
assert_no_padding(ساختار xkb_layout_policy_update, policy, redirect);
assert_no_padding(ساختار xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_layout_policy_update) ==
              اندازه(ساختار xkb_layout_policy_update_v1), "");
assert_same_field(ساختار xkb_layout_policy_update, _v1, size);
assert_same_field(ساختار xkb_layout_policy_update, _v1, policy);
assert_same_field(ساختار xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_layout_policy_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_synthetic_update_v1 {
    uint32_t size;
    uint32_t reserved0;
    ثابت ساختار xkb_state_components_update_v1 *components;
    ثابت ساختار xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_synthetic_update, size, reserved0);
assert_no_padding(ساختار xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(ساختار xkb_synthetic_update, components, layout_policy);
assert_no_padding(ساختار xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_synthetic_update) ==
              اندازه(ساختار xkb_synthetic_update_v1), "");
assert_same_field(ساختار xkb_synthetic_update, _v1, size);
assert_same_field(ساختار xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_field(ساختار xkb_synthetic_update, _v1, components);
assert_same_field(ساختار xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_synthetic_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_machine_builder_config_v1 {
    uint32_t size;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_machine_builder_config, size, builder_flags);
assert_no_padding(ساختار xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(ساختار xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_machine_builder_config) ==
              اندازه(ساختار xkb_machine_builder_config_v1), "");
assert_same_field(ساختار xkb_machine_builder_config, _v1, size);
assert_same_field(ساختار xkb_machine_builder_config, _v1, builder_flags);
assert_same_field(ساختار xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_machine_builder_config) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_machine_builder_a11y_update_v1 {
    uint32_t size;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_machine_builder_a11y_update, size, affect_flags);
assert_no_padding(ساختار xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(ساختار xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_machine_builder_a11y_update) ==
              اندازه(ساختار xkb_machine_builder_a11y_update_v1), "");
assert_same_field(ساختار xkb_machine_builder_a11y_update, _v1, size);
assert_same_field(ساختار xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_field(ساختار xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_machine_builder_a11y_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_machine_builder_mods_remap_update_v1 {
    uint32_t size;
    xkb_mod_mask_t source;
    xkb_mod_mask_t target;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_machine_builder_mods_remap_update, size, source);
assert_no_padding(ساختار xkb_machine_builder_mods_remap_update, source, target);
assert_no_padding(ساختار xkb_machine_builder_mods_remap_update, target);

/* Current version is 1 */
static_assert(
    اندازه(ساختار xkb_machine_builder_mods_remap_update) ==
    اندازه(ساختار xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_field(ساختار xkb_machine_builder_mods_remap_update, _v1, size);
assert_same_field(ساختار xkb_machine_builder_mods_remap_update, _v1, source);
assert_same_field(ساختار xkb_machine_builder_mods_remap_update, _v1, target);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_machine_builder_mods_remap_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساختار xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t size;
    xkb_layout_index_t source;
    xkb_layout_index_t target;
    xkb_mod_mask_t affect_mods;
    xkb_mod_mask_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساختار xkb_machine_builder_shortcut_override_update, size, source);
assert_no_padding(ساختار xkb_machine_builder_shortcut_override_update, source, target);
assert_no_padding(ساختار xkb_machine_builder_shortcut_override_update, target, affect_mods);
assert_no_padding(ساختار xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(ساختار xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(اندازه(ساختار xkb_machine_builder_shortcut_override_update) ==
              اندازه(ساختار xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_field(ساختار xkb_machine_builder_shortcut_override_update, _v1, size);
assert_same_field(ساختار xkb_machine_builder_shortcut_override_update, _v1, source);
assert_same_field(ساختار xkb_machine_builder_shortcut_override_update, _v1, target);
assert_same_field(ساختار xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_field(ساختار xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(اندازه(ساختار xkb_machine_builder_shortcut_override_update) * 30 <=
              (size_t)XKB_ABI_MAX_SIZE, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#define xkb_versioned_struct_size_v1(x) _Generic(                   \
    (x),                                                            \
    ثابت ساختار xkb_events_config *:                               \
        اندازه(ساختار xkb_events_config_v1),                        \
    ساختار xkb_event_components *:                                  \
        اندازه(ساختار xkb_event_components_v1),                     \
    ساختار xkb_event_pointer_motion *:                              \
        اندازه(ساختار xkb_event_pointer_motion_v1),                 \
    ساختار xkb_event_pointer_button *:                              \
        اندازه(ساختار xkb_event_pointer_button_v1),                 \
    ثابت ساختار xkb_synthetic_update *:                            \
        اندازه(ساختار xkb_synthetic_update_v1),                     \
    ثابت ساختار xkb_state_components_update *:                     \
        اندازه(ساختار xkb_state_components_update_v1),              \
    ثابت ساختار xkb_layout_policy_update *:                        \
        اندازه(ساختار xkb_layout_policy_update_v1),                 \
    ثابت ساختار xkb_machine_builder_config *:                      \
        اندازه(ساختار xkb_machine_builder_config_v1),               \
    ثابت ساختار xkb_machine_builder_a11y_update *:                 \
        اندازه(ساختار xkb_machine_builder_a11y_update_v1),          \
    ثابت ساختار xkb_machine_builder_mods_remap_update *:           \
        اندازه(ساختار xkb_machine_builder_mods_remap_update_v1),    \
    ثابت ساختار xkb_machine_builder_shortcut_override_update *:      \
        اندازه(ساختار xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#define xkb_versioned_struct_size_min(x) _Generic(                  \
    (x),                                                            \
    ثابت ساختار xkb_events_config *:                               \
        اندازه(ساختار xkb_events_config_v1),                        \
    ساختار xkb_event_components *:                                  \
        اندازه(ساختار xkb_event_components_v1),                     \
    ساختار xkb_event_pointer_motion *:                              \
        اندازه(ساختار xkb_event_pointer_motion_v1),                 \
    ساختار xkb_event_pointer_button *:                              \
        اندازه(ساختار xkb_event_pointer_button_v1),                 \
    ثابت ساختار xkb_synthetic_update *:                            \
        اندازه(ساختار xkb_synthetic_update_v1),                     \
    ثابت ساختار xkb_state_components_update *:                     \
        اندازه(ساختار xkb_state_components_update_v1),              \
    ثابت ساختار xkb_layout_policy_update *:                        \
        اندازه(ساختار xkb_layout_policy_update_v1),                 \
    ثابت ساختار xkb_machine_builder_config *:                      \
        اندازه(ساختار xkb_machine_builder_config_v1),               \
    ثابت ساختار xkb_machine_builder_a11y_update *:                 \
        اندازه(ساختار xkb_machine_builder_a11y_update_v1),          \
    ثابت ساختار xkb_machine_builder_mods_remap_update *:           \
        اندازه(ساختار xkb_machine_builder_mods_remap_update_v1),    \
    ثابت ساختار xkb_machine_builder_shortcut_override_update *:      \
        اندازه(ساختار xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#define xkb_versioned_struct_reserved_offset(x) _Generic(        \
    (x),                                                         \
    ثابت ساختار xkb_events_config *:                            \
        اندازه(ساختار xkb_events_config),                        \
    ساختار xkb_event_components *:                               \
        اندازه(ساختار xkb_event_components),                     \
    ساختار xkb_event_pointer_motion *:                           \
        اندازه(ساختار xkb_event_pointer_motion),                 \
    ساختار xkb_event_pointer_button *:                           \
        offsetof(ساختار xkb_event_pointer_button, reserved0),    \
    ثابت ساختار xkb_synthetic_update *:                         \
        اندازه(ساختار xkb_synthetic_update),                     \
    ثابت ساختار xkb_state_components_update *:                  \
        اندازه(ساختار xkb_state_components_update),              \
    ثابت ساختار xkb_layout_policy_update *:                     \
        اندازه(ساختار xkb_layout_policy_update),                 \
    ثابت ساختار xkb_machine_builder_config *:                   \
        اندازه(ساختار xkb_machine_builder_config),               \
    ثابت ساختار xkb_machine_builder_a11y_update *:              \
        اندازه(ساختار xkb_machine_builder_a11y_update),          \
    ثابت ساختار xkb_machine_builder_mods_remap_update *:        \
        اندازه(ساختار xkb_machine_builder_mods_remap_update),    \
    ثابت ساختار xkb_machine_builder_shortcut_override_update *:   \
        اندازه(ساختار xkb_machine_builder_shortcut_override_update)\
)

#define xkb_check_state_abi(x) xkb_check_versioned_struct_size( \
    xkb_versioned_struct_size_v1(x),                            \
    xkb_versioned_struct_size_min(x),                           \
    xkb_versioned_struct_reserved_offset(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_events_config *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_events_config *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ساختار xkb_event_components *)پوچ)) <=
    xkb_versioned_struct_size_min(((ساختار xkb_event_components *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ساختار xkb_event_pointer_motion *)پوچ)) <=
    xkb_versioned_struct_size_min(((ساختار xkb_event_pointer_motion *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ساختار xkb_event_pointer_button *)پوچ)) <=
    xkb_versioned_struct_size_min(((ساختار xkb_event_pointer_button *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_synthetic_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_synthetic_update *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_state_components_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_state_components_update *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_layout_policy_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_layout_policy_update *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_machine_builder_config *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_config *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_machine_builder_a11y_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_a11y_update *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_machine_builder_mods_remap_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_mods_remap_update *)پوچ)),
    ""
);
static_assert(
    xkb_versioned_struct_size_v1(((ثابت ساختار xkb_machine_builder_shortcut_override_update *)پوچ)) <=
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_shortcut_override_update *)پوچ)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_events_config *)پوچ)) <=
    اندازه(ثابت ساختار xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ساختار xkb_event_components *)پوچ)) <=
    اندازه(ساختار xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ساختار xkb_event_pointer_motion *)پوچ)) <=
    اندازه(ساختار xkb_event_pointer_motion),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ساختار xkb_event_pointer_button*)پوچ)) <=
    اندازه(ساختار xkb_event_pointer_button),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_synthetic_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_state_components_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_layout_policy_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_config *)پوچ)) <=
    اندازه(ثابت ساختار xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_a11y_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_mods_remap_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_size_min(((ثابت ساختار xkb_machine_builder_shortcut_override_update *)پوچ)) <=
    اندازه(ثابت ساختار xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_PRIVATE ساختار xkb_state *
xkb_machine_get_state(ساختار xkb_machine *machine);
