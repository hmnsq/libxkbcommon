/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"
#ضمن "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) עשה {    \
    אם (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} כלעוד (0)

ساكن bool
update_initial_state(بنية xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, تعداد xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, عدم);

    אם (!reply)
        החזר false;

    /* NOTE: Use the public API with private enum values */
    ثابت بنية xkb_state_components_update components = {
        .size = حجم(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (تعداد xkb_keyboard_control_flags) controls,
        .controls = (تعداد xkb_keyboard_control_flags) controls,
    };
    ثابت بنية xkb_synthetic_update update = {
        .size = حجم(update),
        .components = &components,
    };
    ثابت تعداد xkb_status status =
        xkb_state_update_synthetic(state, &update, عدم);
    אם (status != XKB_SUCCESS)
        החזר false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    حرر(reply);
    החזר true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#אם 0
// TODO: currently unused
ساكن تعداد xkb_a11y_flags
translate_state_accessibility_flags(ثابت xcb_xkb_get_controls_reply_t *reply)
{
    تعداد xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    אם (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK;
    }
    החזר flags;
}
#endif

ساكن bool
get_controls(بنية xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             تعداد xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, عدم);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    حرر(reply);
    החזר true;

fail:
    حرر(reply);
    החזר false;
}

بنية xkb_state *
xkb_x11_state_new_from_device(بنية xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    אם (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        החזר عدم;
    }

    تعداد xkb_action_controls controls = 0;
    אם (!get_controls(keymap->ctx, conn, device_id, &controls))
        החזר عدم;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    بنية xkb_state * ثابت state = xkb_state_new(keymap);
    אם (!state)
        החזר عدم;

    אם (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        החזר عدم;
    }

    החזר state;
}
