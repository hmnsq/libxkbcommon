/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "x11-priv.h"
#شامل "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) انجامبده {    \
    اگر (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        برو fail;                                          \
    }                                                       \
} وقتی (0)

ایستا bool
update_initial_state(ساختار xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, شمارش xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, پوچ);

    اگر (!reply)
        بازگشت false;

    /* NOTE: Use the public API with private enum values */
    ثابت ساختار xkb_state_components_update components = {
        .size = اندازه(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (شمارش xkb_keyboard_control_flags) controls,
        .controls = (شمارش xkb_keyboard_control_flags) controls,
    };
    ثابت ساختار xkb_synthetic_update update = {
        .size = اندازه(update),
        .components = &components,
    };
    ثابت شمارش xkb_status status =
        xkb_state_update_synthetic(state, &update, پوچ);
    اگر (status != XKB_SUCCESS)
        بازگشت false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    آزاد(reply);
    بازگشت true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#اگر 0
// TODO: currently unused
ایستا شمارش xkb_a11y_flags
translate_state_accessibility_flags(ثابت xcb_xkb_get_controls_reply_t *reply)
{
    شمارش xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    اگر (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_کلیدها_LATCH_TO_LOCK;
    }
    بازگشت flags;
}
#endif

ایستا bool
get_controls(ساختار xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             شمارش xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, پوچ);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ساختار xkb_state *
xkb_x11_state_new_from_device(ساختار xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    اگر (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        بازگشت پوچ;
    }

    شمارش xkb_action_controls controls = 0;
    اگر (!get_controls(keymap->ctx, conn, device_id, &controls))
        بازگشت پوچ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ساختار xkb_state * ثابت state = xkb_state_new(keymap);
    اگر (!state)
        بازگشت پوچ;

    اگر (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        بازگشت پوچ;
    }

    بازگشت state;
}
