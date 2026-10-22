/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "x11-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL_IF_BAD_ਜਵਾਬ(ctx, ਜਵਾਬ, request_name) ਕਰੋ {    \
    ਜੇ (!(ਜਵਾਬ)) {                                         \
        log_err((ctx), XKB_LOG_ਸੁਨੇਹਾ_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ਜਾਓ fail;                                          \
    }                                                       \
} ਜਦੋਂ (0)

ਸਥਿਰ bool
update_initial_state(ਬਣਤਰ xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ਗਿਣਤੀ xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_state_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    ਜੇ (!ਜਵਾਬ)
        ਵਾਪਸ false;

    /* NOTE: Use the public API with private enum values */
    ਅਚਲ ਬਣਤਰ xkb_state_components_update components = {
        .ਮਾਪ = ਆਕਾਰ(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ਗਿਣਤੀ xkb_keyboard_control_flags) controls,
        .controls = (ਗਿਣਤੀ xkb_keyboard_control_flags) controls,
    };
    ਅਚਲ ਬਣਤਰ xkb_synthetic_update update = {
        .ਮਾਪ = ਆਕਾਰ(update),
        .components = &components,
    };
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
        xkb_state_update_synthetic(state, &update, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        ਵਾਪਸ false;

    xkb_state_update_ਨਕਾਬ(state,
                          ਜਵਾਬ->ਅਧਾਰ_Mods,
                          ਜਵਾਬ->latchedMods,
                          ਜਵਾਬ->lockedMods,
                          ਜਵਾਬ->ਅਧਾਰ_ਸਮੂਹ_2,
                          ਜਵਾਬ->latched_ਸਮੂਹ_2,
                          ਜਵਾਬ->locked_ਸਮੂਹ_2);

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#ਜੇ 0
// TODO: currently unused
ਸਥਿਰ ਗਿਣਤੀ xkb_a11y_flags
translate_state_accessibility_flags(ਅਚਲ xcb_xkb_get_controls_ਜਵਾਬ_t *ਜਵਾਬ)
{
    ਗਿਣਤੀ xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    ਜੇ (ਜਵਾਬ->accessXOption & XCB_XKB_AX_OPTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ) {
        flags |= XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
    }
    ਵਾਪਸ flags;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ bool
get_controls(ਬਣਤਰ xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ਗਿਣਤੀ xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_controls_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    FAIL_IF_BAD_ਜਵਾਬ(ctx, ਜਵਾਬ, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_ਨਕਾਬ(ਜਵਾਬ->enabledControls);

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਬਣਤਰ xkb_state *
xkb_x11_state_new_from_device(ਬਣਤਰ xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    ਜੇ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਗਿਣਤੀ xkb_action_controls controls = 0;
    ਜੇ (!get_controls(keymap->ctx, conn, device_id, &controls))
        ਵਾਪਸ ਨਿਰਾ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ਬਣਤਰ xkb_state * ਅਚਲ state = xkb_state_new(keymap);
    ਜੇ (!state)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ state;
}
