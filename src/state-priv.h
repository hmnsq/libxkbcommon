/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

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
ساخت state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t بنیاد_گروہ; /**< depressed */
    int32_t latched_گروہ;
    int32_t locked_گروہ;
    xkb_layout_اشاریہ_t گروہ; /**< effective */

    xkb_mod_نقاب_t بنیاد_mods; /**< depressed */
    xkb_mod_نقاب_t latched_mods;
    xkb_mod_نقاب_t locked_mods;
    xkb_mod_نقاب_t mods; /**< effective */

    xkb_led_نقاب_t leds;

    شمار xkb_action_controls controls; /**< effective */
};

ساخت xkb_event {
    ساخت xkb_context *ctx;
    شمار xkb_event_type type;
    اتحاد {
        ساخت {
            xkb_keycode_t keycode;
            شمار xkb_کلید_direction direction;
        } کلید;
        ساخت {
            ساخت state_components components;
            شمار xkb_state_component بدلا;
        } components;
        ساخت xkb_event_اشارہ_گر_motion اشارہ_گر_motion;
        ساخت xkb_event_اشارہ_گر_button اشارہ_گر_button;
        ساخت {
            int8_t اشاریہ_or_انحراف;
            bool is_انحراف;
        } virtual_console;
    };
};

XKB_EXPORT_نجی خالی
xkb_state_reset(ساخت xkb_state *state);

XKB_EXPORT_نجی xkb_led_نقاب_t
xkb_state_serialize_leds(ثابت ساخت xkb_state *state,
                         شمار xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_events_config_v1 {
    uint32_t سائز;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_events_config, سائز, flags);
assert_no_padding(ساخت xkb_events_config, flags);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_events_config) ==
              حجم(ساخت xkb_events_config_v1), "");
assert_same_میدان(ساخت xkb_events_config, _v1, سائز);
assert_same_میدان(ساخت xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_events_config) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_event_components_v1 {
    uint32_t سائز;
    uint32_t بدلا;
    xkb_mod_نقاب_t depressed_mods;
    xkb_mod_نقاب_t latched_mods;
    xkb_mod_نقاب_t locked_mods;
    xkb_mod_نقاب_t mods;
    xkb_layout_اشاریہ_t depressed_layout;
    xkb_layout_اشاریہ_t latched_layout;
    xkb_layout_اشاریہ_t locked_layout;
    xkb_layout_اشاریہ_t layout;
    xkb_led_نقاب_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_event_components, سائز, بدلا);
assert_no_padding(ساخت xkb_event_components, بدلا, depressed_mods);
assert_no_padding(ساخت xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(ساخت xkb_event_components, latched_mods, locked_mods);
assert_no_padding(ساخت xkb_event_components, locked_mods, mods);
assert_no_padding(ساخت xkb_event_components, mods, depressed_layout);
assert_no_padding(ساخت xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(ساخت xkb_event_components, latched_layout, locked_layout);
assert_no_padding(ساخت xkb_event_components, locked_layout, layout);
assert_no_padding(ساخت xkb_event_components, layout, leds);
assert_no_padding(ساخت xkb_event_components, leds, controls);
assert_no_padding(ساخت xkb_event_components, controls);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_event_components) ==
              حجم(ساخت xkb_event_components_v1), "");
assert_same_میدان(ساخت xkb_event_components, _v1, سائز);
assert_same_میدان(ساخت xkb_event_components, _v1, بدلا);
assert_same_میدان(ساخت xkb_event_components, _v1, depressed_mods);
assert_same_میدان(ساخت xkb_event_components, _v1, latched_mods);
assert_same_میدان(ساخت xkb_event_components, _v1, locked_mods);
assert_same_میدان(ساخت xkb_event_components, _v1, mods);
assert_same_میدان(ساخت xkb_event_components, _v1, depressed_layout);
assert_same_میدان(ساخت xkb_event_components, _v1, latched_layout);
assert_same_میدان(ساخت xkb_event_components, _v1, locked_layout);
assert_same_میدان(ساخت xkb_event_components, _v1, layout);
assert_same_میدان(ساخت xkb_event_components, _v1, leds);
assert_same_میدان(ساخت xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_event_components) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_event_اشارہ_گر_motion_v1 {
    uint32_t سائز;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_event_اشارہ_گر_motion, سائز, flags);
assert_no_padding(ساخت xkb_event_اشارہ_گر_motion, flags, x);
assert_no_padding(ساخت xkb_event_اشارہ_گر_motion, x, y);
assert_no_padding(ساخت xkb_event_اشارہ_گر_motion, y);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_event_اشارہ_گر_motion) ==
              حجم(ساخت xkb_event_اشارہ_گر_motion_v1), "");
assert_same_میدان(ساخت xkb_event_اشارہ_گر_motion, _v1, سائز);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_motion, _v1, flags);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_motion, _v1, x);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_event_اشارہ_گر_motion) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_event_اشارہ_گر_button_v1 {
    uint32_t سائز;
    uint32_t button;
    uint8_t گنتی;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_event_اشارہ_گر_button, سائز, button);
assert_no_padding(ساخت xkb_event_اشارہ_گر_button, button, گنتی);
assert_no_padding(ساخت xkb_event_اشارہ_گر_button, گنتی, state);
assert_no_padding(ساخت xkb_event_اشارہ_گر_button, state, reserved0);
assert_no_padding(ساخت xkb_event_اشارہ_گر_button, reserved0);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_event_اشارہ_گر_button) ==
              حجم(ساخت xkb_event_اشارہ_گر_button_v1), "");
assert_same_میدان(ساخت xkb_event_اشارہ_گر_button, _v1, سائز);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_button, _v1, button);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_button, _v1, گنتی);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_button, _v1, state);
assert_same_میدان(ساخت xkb_event_اشارہ_گر_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_event_اشارہ_گر_button) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_state_components_update_v1 {
    uint32_t سائز;
    uint32_t components;
    xkb_mod_نقاب_t affect_latched_mods;
    xkb_mod_نقاب_t latched_mods;
    xkb_mod_نقاب_t affect_locked_mods;
    xkb_mod_نقاب_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_state_components_update, سائز, components);
assert_no_padding(ساخت xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(ساخت xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(ساخت xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(ساخت xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(ساخت xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(ساخت xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(ساخت xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(ساخت xkb_state_components_update, affect_controls, controls);
assert_no_padding(ساخت xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_state_components_update) ==
              حجم(ساخت xkb_state_components_update_v1), "");
assert_same_میدان(ساخت xkb_state_components_update, _v1, سائز);
assert_same_میدان(ساخت xkb_state_components_update, _v1, components);
assert_same_میدان(ساخت xkb_state_components_update, _v1, affect_latched_mods);
assert_same_میدان(ساخت xkb_state_components_update, _v1, latched_mods);
assert_same_میدان(ساخت xkb_state_components_update, _v1, affect_locked_mods);
assert_same_میدان(ساخت xkb_state_components_update, _v1, locked_mods);
assert_same_میدان(ساخت xkb_state_components_update, _v1, latched_layout);
assert_same_میدان(ساخت xkb_state_components_update, _v1, locked_layout);
assert_same_میدان(ساخت xkb_state_components_update, _v1, affect_controls);
assert_same_میدان(ساخت xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_state_components_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_layout_policy_update_v1 {
    uint32_t سائز;
    uint32_t policy;
    xkb_layout_اشاریہ_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_layout_policy_update, سائز, policy);
assert_no_padding(ساخت xkb_layout_policy_update, policy, redirect);
assert_no_padding(ساخت xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_layout_policy_update) ==
              حجم(ساخت xkb_layout_policy_update_v1), "");
assert_same_میدان(ساخت xkb_layout_policy_update, _v1, سائز);
assert_same_میدان(ساخت xkb_layout_policy_update, _v1, policy);
assert_same_میدان(ساخت xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_layout_policy_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_synthetic_update_v1 {
    uint32_t سائز;
    uint32_t reserved0;
    ثابت ساخت xkb_state_components_update_v1 *components;
    ثابت ساخت xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_synthetic_update, سائز, reserved0);
assert_no_padding(ساخت xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(ساخت xkb_synthetic_update, components, layout_policy);
assert_no_padding(ساخت xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(حجم(ساخت xkb_synthetic_update) ==
              حجم(ساخت xkb_synthetic_update_v1), "");
assert_same_میدان(ساخت xkb_synthetic_update, _v1, سائز);
assert_same_میدان(ساخت xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_میدان(ساخت xkb_synthetic_update, _v1, components);
assert_same_میدان(ساخت xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_synthetic_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_machine_builder_config_v1 {
    uint32_t سائز;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_machine_builder_config, سائز, builder_flags);
assert_no_padding(ساخت xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(ساخت xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_machine_builder_config) ==
              حجم(ساخت xkb_machine_builder_config_v1), "");
assert_same_میدان(ساخت xkb_machine_builder_config, _v1, سائز);
assert_same_میدان(ساخت xkb_machine_builder_config, _v1, builder_flags);
assert_same_میدان(ساخت xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_machine_builder_config) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_machine_builder_a11y_update_v1 {
    uint32_t سائز;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_machine_builder_a11y_update, سائز, affect_flags);
assert_no_padding(ساخت xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(ساخت xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_machine_builder_a11y_update) ==
              حجم(ساخت xkb_machine_builder_a11y_update_v1), "");
assert_same_میدان(ساخت xkb_machine_builder_a11y_update, _v1, سائز);
assert_same_میدان(ساخت xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_میدان(ساخت xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_machine_builder_a11y_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_machine_builder_mods_remap_update_v1 {
    uint32_t سائز;
    xkb_mod_نقاب_t ماخذ;
    xkb_mod_نقاب_t ہدف;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_machine_builder_mods_remap_update, سائز, ماخذ);
assert_no_padding(ساخت xkb_machine_builder_mods_remap_update, ماخذ, ہدف);
assert_no_padding(ساخت xkb_machine_builder_mods_remap_update, ہدف);

/* Current version is 1 */
static_assert(
    حجم(ساخت xkb_machine_builder_mods_remap_update) ==
    حجم(ساخت xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_میدان(ساخت xkb_machine_builder_mods_remap_update, _v1, سائز);
assert_same_میدان(ساخت xkb_machine_builder_mods_remap_update, _v1, ماخذ);
assert_same_میدان(ساخت xkb_machine_builder_mods_remap_update, _v1, ہدف);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_machine_builder_mods_remap_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
ساخت xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t سائز;
    xkb_layout_اشاریہ_t ماخذ;
    xkb_layout_اشاریہ_t ہدف;
    xkb_mod_نقاب_t affect_mods;
    xkb_mod_نقاب_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(ساخت xkb_machine_builder_shortcut_override_update, سائز, ماخذ);
assert_no_padding(ساخت xkb_machine_builder_shortcut_override_update, ماخذ, ہدف);
assert_no_padding(ساخت xkb_machine_builder_shortcut_override_update, ہدف, affect_mods);
assert_no_padding(ساخت xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(ساخت xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(حجم(ساخت xkb_machine_builder_shortcut_override_update) ==
              حجم(ساخت xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_میدان(ساخت xkb_machine_builder_shortcut_override_update, _v1, سائز);
assert_same_میدان(ساخت xkb_machine_builder_shortcut_override_update, _v1, ماخذ);
assert_same_میدان(ساخت xkb_machine_builder_shortcut_override_update, _v1, ہدف);
assert_same_میدان(ساخت xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_میدان(ساخت xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(ساخت xkb_machine_builder_shortcut_override_update) * 30 <=
              (سائز_t)XKB_ABI_MAX_سائز, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#تعریف_کرو xkb_versioned_struct_سائز_v1(x) _Generic(                   \
    (x),                                                            \
    ثابت ساخت xkb_events_config *:                               \
        حجم(ساخت xkb_events_config_v1),                        \
    ساخت xkb_event_components *:                                  \
        حجم(ساخت xkb_event_components_v1),                     \
    ساخت xkb_event_اشارہ_گر_motion *:                              \
        حجم(ساخت xkb_event_اشارہ_گر_motion_v1),                 \
    ساخت xkb_event_اشارہ_گر_button *:                              \
        حجم(ساخت xkb_event_اشارہ_گر_button_v1),                 \
    ثابت ساخت xkb_synthetic_update *:                            \
        حجم(ساخت xkb_synthetic_update_v1),                     \
    ثابت ساخت xkb_state_components_update *:                     \
        حجم(ساخت xkb_state_components_update_v1),              \
    ثابت ساخت xkb_layout_policy_update *:                        \
        حجم(ساخت xkb_layout_policy_update_v1),                 \
    ثابت ساخت xkb_machine_builder_config *:                      \
        حجم(ساخت xkb_machine_builder_config_v1),               \
    ثابت ساخت xkb_machine_builder_a11y_update *:                 \
        حجم(ساخت xkb_machine_builder_a11y_update_v1),          \
    ثابت ساخت xkb_machine_builder_mods_remap_update *:           \
        حجم(ساخت xkb_machine_builder_mods_remap_update_v1),    \
    ثابت ساخت xkb_machine_builder_shortcut_override_update *:      \
        حجم(ساخت xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#تعریف_کرو xkb_versioned_struct_سائز_min(x) _Generic(                  \
    (x),                                                            \
    ثابت ساخت xkb_events_config *:                               \
        حجم(ساخت xkb_events_config_v1),                        \
    ساخت xkb_event_components *:                                  \
        حجم(ساخت xkb_event_components_v1),                     \
    ساخت xkb_event_اشارہ_گر_motion *:                              \
        حجم(ساخت xkb_event_اشارہ_گر_motion_v1),                 \
    ساخت xkb_event_اشارہ_گر_button *:                              \
        حجم(ساخت xkb_event_اشارہ_گر_button_v1),                 \
    ثابت ساخت xkb_synthetic_update *:                            \
        حجم(ساخت xkb_synthetic_update_v1),                     \
    ثابت ساخت xkb_state_components_update *:                     \
        حجم(ساخت xkb_state_components_update_v1),              \
    ثابت ساخت xkb_layout_policy_update *:                        \
        حجم(ساخت xkb_layout_policy_update_v1),                 \
    ثابت ساخت xkb_machine_builder_config *:                      \
        حجم(ساخت xkb_machine_builder_config_v1),               \
    ثابت ساخت xkb_machine_builder_a11y_update *:                 \
        حجم(ساخت xkb_machine_builder_a11y_update_v1),          \
    ثابت ساخت xkb_machine_builder_mods_remap_update *:           \
        حجم(ساخت xkb_machine_builder_mods_remap_update_v1),    \
    ثابت ساخت xkb_machine_builder_shortcut_override_update *:      \
        حجم(ساخت xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#تعریف_کرو xkb_versioned_struct_reserved_انحراف(x) _Generic(        \
    (x),                                                         \
    ثابت ساخت xkb_events_config *:                            \
        حجم(ساخت xkb_events_config),                        \
    ساخت xkb_event_components *:                               \
        حجم(ساخت xkb_event_components),                     \
    ساخت xkb_event_اشارہ_گر_motion *:                           \
        حجم(ساخت xkb_event_اشارہ_گر_motion),                 \
    ساخت xkb_event_اشارہ_گر_button *:                           \
        offsetof(ساخت xkb_event_اشارہ_گر_button, reserved0),    \
    ثابت ساخت xkb_synthetic_update *:                         \
        حجم(ساخت xkb_synthetic_update),                     \
    ثابت ساخت xkb_state_components_update *:                  \
        حجم(ساخت xkb_state_components_update),              \
    ثابت ساخت xkb_layout_policy_update *:                     \
        حجم(ساخت xkb_layout_policy_update),                 \
    ثابت ساخت xkb_machine_builder_config *:                   \
        حجم(ساخت xkb_machine_builder_config),               \
    ثابت ساخت xkb_machine_builder_a11y_update *:              \
        حجم(ساخت xkb_machine_builder_a11y_update),          \
    ثابت ساخت xkb_machine_builder_mods_remap_update *:        \
        حجم(ساخت xkb_machine_builder_mods_remap_update),    \
    ثابت ساخت xkb_machine_builder_shortcut_override_update *:   \
        حجم(ساخت xkb_machine_builder_shortcut_override_update)\
)

#تعریف_کرو xkb_check_state_abi(x) xkb_check_versioned_struct_سائز_2( \
    xkb_versioned_struct_سائز_v1(x),                            \
    xkb_versioned_struct_سائز_min(x),                           \
    xkb_versioned_struct_reserved_انحراف(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_events_config *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_events_config *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ساخت xkb_event_components *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_components *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ساخت xkb_event_اشارہ_گر_motion *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_اشارہ_گر_motion *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ساخت xkb_event_اشارہ_گر_button *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_اشارہ_گر_button *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_synthetic_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_synthetic_update *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_state_components_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_state_components_update *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_layout_policy_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_layout_policy_update *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_machine_builder_config *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_config *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_machine_builder_a11y_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_a11y_update *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_machine_builder_mods_remap_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_mods_remap_update *)تہی)),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_v1(((ثابت ساخت xkb_machine_builder_shortcut_override_update *)تہی)) <=
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_shortcut_override_update *)تہی)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_events_config *)تہی)) <=
    حجم(ثابت ساخت xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_components *)تہی)) <=
    حجم(ساخت xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_اشارہ_گر_motion *)تہی)) <=
    حجم(ساخت xkb_event_اشارہ_گر_motion),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ساخت xkb_event_اشارہ_گر_button*)تہی)) <=
    حجم(ساخت xkb_event_اشارہ_گر_button),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_synthetic_update *)تہی)) <=
    حجم(ثابت ساخت xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_state_components_update *)تہی)) <=
    حجم(ثابت ساخت xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_layout_policy_update *)تہی)) <=
    حجم(ثابت ساخت xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_config *)تہی)) <=
    حجم(ثابت ساخت xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_a11y_update *)تہی)) <=
    حجم(ثابت ساخت xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_mods_remap_update *)تہی)) <=
    حجم(ثابت ساخت xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_سائز_min(((ثابت ساخت xkb_machine_builder_shortcut_override_update *)تہی)) <=
    حجم(ثابت ساخت xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_نجی ساخت xkb_state *
xkb_machine_get_state(ساخت xkb_machine *machine);
