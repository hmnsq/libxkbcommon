/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "x11-priv.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) કરો {    \
    જો (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        જાઓ fail;                                          \
    }                                                       \
} જ્યારે (0)

સ્થિર bool
update_initial_state(સંરચના xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ગણના xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, ખાલી);

    જો (!reply)
        પાછા false;

    /* NOTE: Use the public API with private enum values */
    અચલ સંરચના xkb_state_components_update components = {
        .size = કદ(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ગણના xkb_keyboard_control_flags) controls,
        .controls = (ગણના xkb_keyboard_control_flags) controls,
    };
    અચલ સંરચના xkb_synthetic_update update = {
        .size = કદ(update),
        .components = &components,
    };
    અચલ ગણના xkb_status status =
        xkb_state_update_synthetic(state, &update, ખાલી);
    જો (status != XKB_SUCCESS)
        પાછા false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    મુક્ત(reply);
    પાછા true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#જો 0
// TODO: currently unused
સ્થિર ગણના xkb_a11y_flags
translate_state_accessibility_flags(અચલ xcb_xkb_get_controls_reply_t *reply)
{
    ગણના xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    જો (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_ચાવીઓ_LATCH_TO_LOCK;
    }
    પાછા flags;
}
#endif

સ્થિર bool
get_controls(સંરચના xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ગણના xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ખાલી);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સંરચના xkb_state *
xkb_x11_state_new_from_device(સંરચના xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    જો (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        પાછા ખાલી;
    }

    ગણના xkb_action_controls controls = 0;
    જો (!get_controls(keymap->ctx, conn, device_id, &controls))
        પાછા ખાલી;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    સંરચના xkb_state * અચલ state = xkb_state_new(keymap);
    જો (!state)
        પાછા ખાલી;

    જો (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        પાછા ખાલી;
    }

    પાછા state;
}
