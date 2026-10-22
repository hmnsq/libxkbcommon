/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "x11-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#ವ್ಯಾಖ್ಯಾನಿಸು FAIL_IF_BAD_ಉತ್ತರ(ctx, ಉತ್ತರ, request_name) ಮಾಡು {    \
    ಆದರೆ (!(ಉತ್ತರ)) {                                         \
        log_err((ctx), XKB_LOG_ಸಂದೇಶ_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ಹೋಗು fail;                                          \
    }                                                       \
} ಆಗ (0)

ಸ್ಥಿರ bool
update_initial_state(ರಚನೆ xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ಎಣಿಕೆ xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_state_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    ಆದರೆ (!ಉತ್ತರ)
        ಹಿಂತಿರುಗಿ false;

    /* NOTE: Use the public API with private enum values */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update components = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ಎಣಿಕೆ xkb_keyboard_control_flags) controls,
        .controls = (ಎಣಿಕೆ xkb_keyboard_control_flags) controls,
    };
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update update = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(update),
        .components = &components,
    };
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
        xkb_state_update_synthetic(state, &update, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        ಹಿಂತಿರುಗಿ false;

    xkb_state_update_ಮುಖವಾಡ(state,
                          ಉತ್ತರ->ತಳ_Mods,
                          ಉತ್ತರ->latchedMods,
                          ಉತ್ತರ->lockedMods,
                          ಉತ್ತರ->ತಳ_ಗುಂಪು_2,
                          ಉತ್ತರ->latched_ಗುಂಪು_2,
                          ಉತ್ತರ->locked_ಗುಂಪು_2);

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#ಆದರೆ 0
// TODO: currently unused
ಸ್ಥಿರ ಎಣಿಕೆ xkb_a11y_flags
translate_state_accessibility_flags(ಸ್ಥಿರಾಂಕ xcb_xkb_get_controls_ಉತ್ತರ_t *ಉತ್ತರ)
{
    ಎಣಿಕೆ xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    ಆದರೆ (ಉತ್ತರ->accessXOption & XCB_XKB_AX_OPTION_LATCH_ಗಮ್ಯ_ಬೀಗ) {
        flags |= XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_ಗಮ್ಯ_ಬೀಗ;
    }
    ಹಿಂತಿರುಗಿ flags;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ bool
get_controls(ರಚನೆ xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ಎಣಿಕೆ xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_controls_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    FAIL_IF_BAD_ಉತ್ತರ(ctx, ಉತ್ತರ, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_ಮುಖವಾಡ(ಉತ್ತರ->enabledControls);

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ರಚನೆ xkb_state *
xkb_x11_state_new_from_device(ರಚನೆ xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    ಆದರೆ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಎಣಿಕೆ xkb_action_controls controls = 0;
    ಆದರೆ (!get_controls(keymap->ctx, conn, device_id, &controls))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ರಚನೆ xkb_state * ಸ್ಥಿರಾಂಕ state = xkb_state_new(keymap);
    ಆದರೆ (!state)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ state;
}
