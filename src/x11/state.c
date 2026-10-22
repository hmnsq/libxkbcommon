/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "x11-priv.h"
#شامل "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#تعریف_کرو FAIL_IF_BAD_جواب(ctx, جواب, request_name) کرو {    \
    اگر (!(جواب)) {                                         \
        log_err((ctx), XKB_LOG_پیغام_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        جاؤ fail;                                          \
    }                                                       \
} جب (0)

ساکن bool
update_initial_state(ساخت xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, شمار xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_جواب_t *جواب =
        xcb_xkb_get_state_جواب(conn, cookie, تہی);

    اگر (!جواب)
        واپس false;

    /* NOTE: Use the public API with private enum values */
    ثابت ساخت xkb_state_components_update components = {
        .سائز = حجم(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (شمار xkb_keyboard_control_flags) controls,
        .controls = (شمار xkb_keyboard_control_flags) controls,
    };
    ثابت ساخت xkb_synthetic_update update = {
        .سائز = حجم(update),
        .components = &components,
    };
    ثابت شمار xkb_حالت حالت_2 =
        xkb_state_update_synthetic(state, &update, تہی);
    اگر (حالت_2 != XKB_SUCCESS)
        واپس false;

    xkb_state_update_نقاب(state,
                          جواب->بنیاد_Mods,
                          جواب->latchedMods,
                          جواب->lockedMods,
                          جواب->بنیاد_گروہ_2,
                          جواب->latched_گروہ_2,
                          جواب->locked_گروہ_2);

    آزاد(جواب);
    واپس true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#اگر 0
// TODO: currently unused
ساکن شمار xkb_a11y_flags
translate_state_accessibility_flags(ثابت xcb_xkb_get_controls_جواب_t *جواب)
{
    شمار xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    اگر (جواب->accessXOption & XCB_XKB_AX_OPTION_LATCH_مقصد_تالا) {
        flags |= XKB_A11_Y_STICKY_کلیدیں_LATCH_مقصد_تالا;
    }
    واپس flags;
}
#اگر_ختم

ساکن bool
get_controls(ساخت xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             شمار xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_جواب_t *جواب =
        xcb_xkb_get_controls_جواب(conn, cookie, تہی);

    FAIL_IF_BAD_جواب(ctx, جواب, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_نقاب(جواب->enabledControls);

    آزاد(جواب);
    واپس true;

fail:
    آزاد(جواب);
    واپس false;
}

ساخت xkb_state *
xkb_x11_state_new_from_device(ساخت xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    اگر (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        واپس تہی;
    }

    شمار xkb_action_controls controls = 0;
    اگر (!get_controls(keymap->ctx, conn, device_id, &controls))
        واپس تہی;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ساخت xkb_state * ثابت state = xkb_state_new(keymap);
    اگر (!state)
        واپس تہی;

    اگر (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        واپس تہی;
    }

    واپس state;
}
