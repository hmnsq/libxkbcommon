/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdalign.h>
#ضمن <stddef.h>
#ضمن <stdint.h>

#ضمن "utils.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "abi-check.h"
#ضمن "keymap.h"

/** Core keyboard state components */
بنية state_components {
    /* These may be negative, because of -1 group actions. */
    int32_t پایه_گروه; /**< depressed */
    int32_t latched_گروه;
    int32_t locked_گروه;
    xkb_layout_شاخص_t گروه; /**< effective */

    xkb_mod_ماسک_t پایه_mods; /**< depressed */
    xkb_mod_ماسک_t latched_mods;
    xkb_mod_ماسک_t locked_mods;
    xkb_mod_ماسک_t mods; /**< effective */

    xkb_led_ماسک_t leds;

    تعداد xkb_action_controls controls; /**< effective */
};

بنية xkb_event {
    بنية xkb_context *ctx;
    تعداد xkb_event_type type;
    اتحاد {
        بنية {
            xkb_keycode_t keycode;
            تعداد xkb_کلید_direction direction;
        } کلید;
        بنية {
            بنية state_components components;
            تعداد xkb_state_component تغییریافته;
        } components;
        بنية xkb_event_اشاره_گر_motion اشاره_گر_motion;
        بنية xkb_event_اشاره_گر_button اشاره_گر_button;
        بنية {
            int8_t شاخص_or_جابه_جایی;
            bool is_جابه_جایی;
        } virtual_console;
    };
};

XKB_EXPORT_خصوصی فراغ
xkb_state_reset(بنية xkb_state *state);

XKB_EXPORT_خصوصی xkb_led_ماسک_t
xkb_state_serialize_leds(ثابت بنية xkb_state *state,
                         تعداد xkb_state_component type);

/******************************************************************************
 * xkb_events_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_events_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_events_config_v1 {
    uint32_t اندازه;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_events_config, اندازه, flags);
assert_no_padding(بنية xkb_events_config, flags);

/* Current version is 1 */
static_assert(حجم(بنية xkb_events_config) ==
              حجم(بنية xkb_events_config_v1), "");
assert_same_فیلد(بنية xkb_events_config, _v1, اندازه);
assert_same_فیلد(بنية xkb_events_config, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_events_config) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_event_components
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_components`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_event_components_v1 {
    uint32_t اندازه;
    uint32_t تغییریافته;
    xkb_mod_ماسک_t depressed_mods;
    xkb_mod_ماسک_t latched_mods;
    xkb_mod_ماسک_t locked_mods;
    xkb_mod_ماسک_t mods;
    xkb_layout_شاخص_t depressed_layout;
    xkb_layout_شاخص_t latched_layout;
    xkb_layout_شاخص_t locked_layout;
    xkb_layout_شاخص_t layout;
    xkb_led_ماسک_t leds;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_event_components, اندازه, تغییریافته);
assert_no_padding(بنية xkb_event_components, تغییریافته, depressed_mods);
assert_no_padding(بنية xkb_event_components, depressed_mods, latched_mods);
assert_no_padding(بنية xkb_event_components, latched_mods, locked_mods);
assert_no_padding(بنية xkb_event_components, locked_mods, mods);
assert_no_padding(بنية xkb_event_components, mods, depressed_layout);
assert_no_padding(بنية xkb_event_components, depressed_layout, latched_layout);
assert_no_padding(بنية xkb_event_components, latched_layout, locked_layout);
assert_no_padding(بنية xkb_event_components, locked_layout, layout);
assert_no_padding(بنية xkb_event_components, layout, leds);
assert_no_padding(بنية xkb_event_components, leds, controls);
assert_no_padding(بنية xkb_event_components, controls);

/* Current version is 1 */
static_assert(حجم(بنية xkb_event_components) ==
              حجم(بنية xkb_event_components_v1), "");
assert_same_فیلد(بنية xkb_event_components, _v1, اندازه);
assert_same_فیلد(بنية xkb_event_components, _v1, تغییریافته);
assert_same_فیلد(بنية xkb_event_components, _v1, depressed_mods);
assert_same_فیلد(بنية xkb_event_components, _v1, latched_mods);
assert_same_فیلد(بنية xkb_event_components, _v1, locked_mods);
assert_same_فیلد(بنية xkb_event_components, _v1, mods);
assert_same_فیلد(بنية xkb_event_components, _v1, depressed_layout);
assert_same_فیلد(بنية xkb_event_components, _v1, latched_layout);
assert_same_فیلد(بنية xkb_event_components, _v1, locked_layout);
assert_same_فیلد(بنية xkb_event_components, _v1, layout);
assert_same_فیلد(بنية xkb_event_components, _v1, leds);
assert_same_فیلد(بنية xkb_event_components, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_event_components) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_event_pointer_motion
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_motion`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_event_اشاره_گر_motion_v1 {
    uint32_t اندازه;
    uint32_t flags;
    int32_t x;
    int32_t y;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_event_اشاره_گر_motion, اندازه, flags);
assert_no_padding(بنية xkb_event_اشاره_گر_motion, flags, x);
assert_no_padding(بنية xkb_event_اشاره_گر_motion, x, y);
assert_no_padding(بنية xkb_event_اشاره_گر_motion, y);

/* Current version is 1 */
static_assert(حجم(بنية xkb_event_اشاره_گر_motion) ==
              حجم(بنية xkb_event_اشاره_گر_motion_v1), "");
assert_same_فیلد(بنية xkb_event_اشاره_گر_motion, _v1, اندازه);
assert_same_فیلد(بنية xkb_event_اشاره_گر_motion, _v1, flags);
assert_same_فیلد(بنية xkb_event_اشاره_گر_motion, _v1, x);
assert_same_فیلد(بنية xkb_event_اشاره_گر_motion, _v1, y);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_event_اشاره_گر_motion) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_event_pointer_button
 *****************************************************************************/

/**
 * Version 1 of `xkb_event_pointer_button`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_event_اشاره_گر_button_v1 {
    uint32_t اندازه;
    uint32_t button;
    uint8_t شمار;
    uint8_t state;
    uint8_t reserved0[2];
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_event_اشاره_گر_button, اندازه, button);
assert_no_padding(بنية xkb_event_اشاره_گر_button, button, شمار);
assert_no_padding(بنية xkb_event_اشاره_گر_button, شمار, state);
assert_no_padding(بنية xkb_event_اشاره_گر_button, state, reserved0);
assert_no_padding(بنية xkb_event_اشاره_گر_button, reserved0);

/* Current version is 1 */
static_assert(حجم(بنية xkb_event_اشاره_گر_button) ==
              حجم(بنية xkb_event_اشاره_گر_button_v1), "");
assert_same_فیلد(بنية xkb_event_اشاره_گر_button, _v1, اندازه);
assert_same_فیلد(بنية xkb_event_اشاره_گر_button, _v1, button);
assert_same_فیلد(بنية xkb_event_اشاره_گر_button, _v1, شمار);
assert_same_فیلد(بنية xkb_event_اشاره_گر_button, _v1, state);
assert_same_فیلد(بنية xkb_event_اشاره_گر_button, _v1, reserved0);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_event_اشاره_گر_button) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_state_components_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_state_components_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_state_components_update_v1 {
    uint32_t اندازه;
    uint32_t components;
    xkb_mod_ماسک_t affect_latched_mods;
    xkb_mod_ماسک_t latched_mods;
    xkb_mod_ماسک_t affect_locked_mods;
    xkb_mod_ماسک_t locked_mods;
    int32_t latched_layout;
    int32_t locked_layout;
    uint32_t affect_controls;
    uint32_t controls;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_state_components_update, اندازه, components);
assert_no_padding(بنية xkb_state_components_update, components, affect_latched_mods);
assert_no_padding(بنية xkb_state_components_update, affect_latched_mods, latched_mods);
assert_no_padding(بنية xkb_state_components_update, latched_mods, affect_locked_mods);
assert_no_padding(بنية xkb_state_components_update, affect_locked_mods, locked_mods);
assert_no_padding(بنية xkb_state_components_update, locked_mods, latched_layout);
assert_no_padding(بنية xkb_state_components_update, latched_layout, locked_layout);
assert_no_padding(بنية xkb_state_components_update, locked_layout, affect_controls);
assert_no_padding(بنية xkb_state_components_update, affect_controls, controls);
assert_no_padding(بنية xkb_state_components_update, controls);

/* Current version is 1 */
static_assert(حجم(بنية xkb_state_components_update) ==
              حجم(بنية xkb_state_components_update_v1), "");
assert_same_فیلد(بنية xkb_state_components_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_state_components_update, _v1, components);
assert_same_فیلد(بنية xkb_state_components_update, _v1, affect_latched_mods);
assert_same_فیلد(بنية xkb_state_components_update, _v1, latched_mods);
assert_same_فیلد(بنية xkb_state_components_update, _v1, affect_locked_mods);
assert_same_فیلد(بنية xkb_state_components_update, _v1, locked_mods);
assert_same_فیلد(بنية xkb_state_components_update, _v1, latched_layout);
assert_same_فیلد(بنية xkb_state_components_update, _v1, locked_layout);
assert_same_فیلد(بنية xkb_state_components_update, _v1, affect_controls);
assert_same_فیلد(بنية xkb_state_components_update, _v1, controls);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_state_components_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_layout_policy_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_layout_policy_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_layout_policy_update_v1 {
    uint32_t اندازه;
    uint32_t policy;
    xkb_layout_شاخص_t redirect;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_layout_policy_update, اندازه, policy);
assert_no_padding(بنية xkb_layout_policy_update, policy, redirect);
assert_no_padding(بنية xkb_layout_policy_update, redirect);

/* Current version is 1 */
static_assert(حجم(بنية xkb_layout_policy_update) ==
              حجم(بنية xkb_layout_policy_update_v1), "");
assert_same_فیلد(بنية xkb_layout_policy_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_layout_policy_update, _v1, policy);
assert_same_فیلد(بنية xkb_layout_policy_update, _v1, redirect);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_layout_policy_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_synthetic_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_synthetic_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_synthetic_update_v1 {
    uint32_t اندازه;
    uint32_t reserved0;
    ثابت بنية xkb_state_components_update_v1 *components;
    ثابت بنية xkb_layout_policy_update_v1 *layout_policy;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_synthetic_update, اندازه, reserved0);
assert_no_padding(بنية xkb_synthetic_update, reserved0, components);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_no_padding(بنية xkb_synthetic_update, components, layout_policy);
assert_no_padding(بنية xkb_synthetic_update, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Current version is 1 */
static_assert(حجم(بنية xkb_synthetic_update) ==
              حجم(بنية xkb_synthetic_update_v1), "");
assert_same_فیلد(بنية xkb_synthetic_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_synthetic_update, _v1, reserved0);
// NOLINTBEGIN(bugprone-sizeof-expression)
assert_same_فیلد(بنية xkb_synthetic_update, _v1, components);
assert_same_فیلد(بنية xkb_synthetic_update, _v1, layout_policy);
// NOLINTEND(bugprone-sizeof-expression)

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_synthetic_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_machine_builder_config
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_config`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_machine_builder_config_v1 {
    uint32_t اندازه;
    uint32_t builder_flags;
    uint32_t machine_flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_machine_builder_config, اندازه, builder_flags);
assert_no_padding(بنية xkb_machine_builder_config, builder_flags, machine_flags);
assert_no_padding(بنية xkb_machine_builder_config, machine_flags);

/* Current version is 1 */
static_assert(حجم(بنية xkb_machine_builder_config) ==
              حجم(بنية xkb_machine_builder_config_v1), "");
assert_same_فیلد(بنية xkb_machine_builder_config, _v1, اندازه);
assert_same_فیلد(بنية xkb_machine_builder_config, _v1, builder_flags);
assert_same_فیلد(بنية xkb_machine_builder_config, _v1, machine_flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_machine_builder_config) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_machine_builder_a11y_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_a11y_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_machine_builder_a11y_update_v1 {
    uint32_t اندازه;
    uint32_t affect_flags;
    uint32_t flags;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_machine_builder_a11y_update, اندازه, affect_flags);
assert_no_padding(بنية xkb_machine_builder_a11y_update, affect_flags, flags);
assert_no_padding(بنية xkb_machine_builder_a11y_update, flags);

/* Current version is 1 */
static_assert(حجم(بنية xkb_machine_builder_a11y_update) ==
              حجم(بنية xkb_machine_builder_a11y_update_v1), "");
assert_same_فیلد(بنية xkb_machine_builder_a11y_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_machine_builder_a11y_update, _v1, affect_flags);
assert_same_فیلد(بنية xkb_machine_builder_a11y_update, _v1, flags);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_machine_builder_a11y_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_machine_builder_mods_remap_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_mods_remap_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_machine_builder_mods_remap_update_v1 {
    uint32_t اندازه;
    xkb_mod_ماسک_t منبع;
    xkb_mod_ماسک_t هدف;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_machine_builder_mods_remap_update, اندازه, منبع);
assert_no_padding(بنية xkb_machine_builder_mods_remap_update, منبع, هدف);
assert_no_padding(بنية xkb_machine_builder_mods_remap_update, هدف);

/* Current version is 1 */
static_assert(
    حجم(بنية xkb_machine_builder_mods_remap_update) ==
    حجم(بنية xkb_machine_builder_mods_remap_update_v1), ""
);
assert_same_فیلد(بنية xkb_machine_builder_mods_remap_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_machine_builder_mods_remap_update, _v1, منبع);
assert_same_فیلد(بنية xkb_machine_builder_mods_remap_update, _v1, هدف);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_machine_builder_mods_remap_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * xkb_machine_builder_shortcut_override_update
 *****************************************************************************/

/**
 * Version 1 of `xkb_machine_builder_shortcut_override_update`, used for ABI check only
 *
 * @since 1.14.0
 */
بنية xkb_machine_builder_shortcut_override_update_v1 {
    uint32_t اندازه;
    xkb_layout_شاخص_t منبع;
    xkb_layout_شاخص_t هدف;
    xkb_mod_ماسک_t affect_mods;
    xkb_mod_ماسک_t mods;
};

/* Ensure there is no implicit padding */
assert_no_padding(بنية xkb_machine_builder_shortcut_override_update, اندازه, منبع);
assert_no_padding(بنية xkb_machine_builder_shortcut_override_update, منبع, هدف);
assert_no_padding(بنية xkb_machine_builder_shortcut_override_update, هدف, affect_mods);
assert_no_padding(بنية xkb_machine_builder_shortcut_override_update, affect_mods, mods);
assert_no_padding(بنية xkb_machine_builder_shortcut_override_update, mods);

/* Current version is 1 */
static_assert(حجم(بنية xkb_machine_builder_shortcut_override_update) ==
              حجم(بنية xkb_machine_builder_shortcut_override_update_v1), "");
assert_same_فیلد(بنية xkb_machine_builder_shortcut_override_update, _v1, اندازه);
assert_same_فیلد(بنية xkb_machine_builder_shortcut_override_update, _v1, منبع);
assert_same_فیلد(بنية xkb_machine_builder_shortcut_override_update, _v1, هدف);
assert_same_فیلد(بنية xkb_machine_builder_shortcut_override_update, _v1, affect_mods);
assert_same_فیلد(بنية xkb_machine_builder_shortcut_override_update, _v1, mods);

/* Ensure reasonable margin to the upper size limit */
static_assert(حجم(بنية xkb_machine_builder_shortcut_override_update) * 30 <=
              (اندازه_t)XKB_ABI_MAX_اندازه, "");

/******************************************************************************
 * Utils
 *****************************************************************************/

/** Size of the *first version* of the struct */
#تعریف_کن xkb_versioned_struct_اندازه_v1(x) _Generic(                   \
    (x),                                                            \
    ثابت بنية xkb_events_config *:                               \
        حجم(بنية xkb_events_config_v1),                        \
    بنية xkb_event_components *:                                  \
        حجم(بنية xkb_event_components_v1),                     \
    بنية xkb_event_اشاره_گر_motion *:                              \
        حجم(بنية xkb_event_اشاره_گر_motion_v1),                 \
    بنية xkb_event_اشاره_گر_button *:                              \
        حجم(بنية xkb_event_اشاره_گر_button_v1),                 \
    ثابت بنية xkb_synthetic_update *:                            \
        حجم(بنية xkb_synthetic_update_v1),                     \
    ثابت بنية xkb_state_components_update *:                     \
        حجم(بنية xkb_state_components_update_v1),              \
    ثابت بنية xkb_layout_policy_update *:                        \
        حجم(بنية xkb_layout_policy_update_v1),                 \
    ثابت بنية xkb_machine_builder_config *:                      \
        حجم(بنية xkb_machine_builder_config_v1),               \
    ثابت بنية xkb_machine_builder_a11y_update *:                 \
        حجم(بنية xkb_machine_builder_a11y_update_v1),          \
    ثابت بنية xkb_machine_builder_mods_remap_update *:           \
        حجم(بنية xkb_machine_builder_mods_remap_update_v1),    \
    ثابت بنية xkb_machine_builder_shortcut_override_update *:      \
        حجم(بنية xkb_machine_builder_shortcut_override_update_v1)\
)

/** Minimal *current* valid size of the struct */
#تعریف_کن xkb_versioned_struct_اندازه_min(x) _Generic(                  \
    (x),                                                            \
    ثابت بنية xkb_events_config *:                               \
        حجم(بنية xkb_events_config_v1),                        \
    بنية xkb_event_components *:                                  \
        حجم(بنية xkb_event_components_v1),                     \
    بنية xkb_event_اشاره_گر_motion *:                              \
        حجم(بنية xkb_event_اشاره_گر_motion_v1),                 \
    بنية xkb_event_اشاره_گر_button *:                              \
        حجم(بنية xkb_event_اشاره_گر_button_v1),                 \
    ثابت بنية xkb_synthetic_update *:                            \
        حجم(بنية xkb_synthetic_update_v1),                     \
    ثابت بنية xkb_state_components_update *:                     \
        حجم(بنية xkb_state_components_update_v1),              \
    ثابت بنية xkb_layout_policy_update *:                        \
        حجم(بنية xkb_layout_policy_update_v1),                 \
    ثابت بنية xkb_machine_builder_config *:                      \
        حجم(بنية xkb_machine_builder_config_v1),               \
    ثابت بنية xkb_machine_builder_a11y_update *:                 \
        حجم(بنية xkb_machine_builder_a11y_update_v1),          \
    ثابت بنية xkb_machine_builder_mods_remap_update *:           \
        حجم(بنية xkb_machine_builder_mods_remap_update_v1),    \
    ثابت بنية xkb_machine_builder_shortcut_override_update *:      \
        حجم(بنية xkb_machine_builder_shortcut_override_update_v1)\
)

/** Offset of the last meaningful field of the current struct */
#تعریف_کن xkb_versioned_struct_reserved_جابه_جایی(x) _Generic(        \
    (x),                                                         \
    ثابت بنية xkb_events_config *:                            \
        حجم(بنية xkb_events_config),                        \
    بنية xkb_event_components *:                               \
        حجم(بنية xkb_event_components),                     \
    بنية xkb_event_اشاره_گر_motion *:                           \
        حجم(بنية xkb_event_اشاره_گر_motion),                 \
    بنية xkb_event_اشاره_گر_button *:                           \
        offsetof(بنية xkb_event_اشاره_گر_button, reserved0),    \
    ثابت بنية xkb_synthetic_update *:                         \
        حجم(بنية xkb_synthetic_update),                     \
    ثابت بنية xkb_state_components_update *:                  \
        حجم(بنية xkb_state_components_update),              \
    ثابت بنية xkb_layout_policy_update *:                     \
        حجم(بنية xkb_layout_policy_update),                 \
    ثابت بنية xkb_machine_builder_config *:                   \
        حجم(بنية xkb_machine_builder_config),               \
    ثابت بنية xkb_machine_builder_a11y_update *:              \
        حجم(بنية xkb_machine_builder_a11y_update),          \
    ثابت بنية xkb_machine_builder_mods_remap_update *:        \
        حجم(بنية xkb_machine_builder_mods_remap_update),    \
    ثابت بنية xkb_machine_builder_shortcut_override_update *:   \
        حجم(بنية xkb_machine_builder_shortcut_override_update)\
)

#تعریف_کن xkb_check_state_abi(x) xkb_check_versioned_struct_اندازه_2( \
    xkb_versioned_struct_اندازه_v1(x),                            \
    xkb_versioned_struct_اندازه_min(x),                           \
    xkb_versioned_struct_reserved_جابه_جایی(x),                    \
    (x)                                                         \
)

/* V1 is the smallest struct version */
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_events_config *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_events_config *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((بنية xkb_event_components *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_components *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((بنية xkb_event_اشاره_گر_motion *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_اشاره_گر_motion *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((بنية xkb_event_اشاره_گر_button *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_اشاره_گر_button *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_synthetic_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_synthetic_update *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_state_components_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_state_components_update *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_layout_policy_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_layout_policy_update *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_machine_builder_config *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_config *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_machine_builder_a11y_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_a11y_update *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_machine_builder_mods_remap_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_mods_remap_update *)عدم)),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_v1(((ثابت بنية xkb_machine_builder_shortcut_override_update *)عدم)) <=
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_shortcut_override_update *)عدم)),
    ""
);

/* Minimal size is lower or equal to the current size */
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_events_config *)عدم)) <=
    حجم(ثابت بنية xkb_events_config),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_components *)عدم)) <=
    حجم(بنية xkb_event_components),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_اشاره_گر_motion *)عدم)) <=
    حجم(بنية xkb_event_اشاره_گر_motion),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((بنية xkb_event_اشاره_گر_button*)عدم)) <=
    حجم(بنية xkb_event_اشاره_گر_button),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_synthetic_update *)عدم)) <=
    حجم(ثابت بنية xkb_synthetic_update),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_state_components_update *)عدم)) <=
    حجم(ثابت بنية xkb_state_components_update),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_layout_policy_update *)عدم)) <=
    حجم(ثابت بنية xkb_layout_policy_update),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_config *)عدم)) <=
    حجم(ثابت بنية xkb_machine_builder_config),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_a11y_update *)عدم)) <=
    حجم(ثابت بنية xkb_machine_builder_a11y_update),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_mods_remap_update *)عدم)) <=
    حجم(ثابت بنية xkb_machine_builder_mods_remap_update),
    ""
);
static_assert(
    xkb_versioned_struct_اندازه_min(((ثابت بنية xkb_machine_builder_shortcut_override_update *)عدم)) <=
    حجم(ثابت بنية xkb_machine_builder_shortcut_override_update),
    ""
);

/** Reference count is not updated */
XKB_EXPORT_خصوصی بنية xkb_state *
xkb_machine_get_state(بنية xkb_machine *machine);
