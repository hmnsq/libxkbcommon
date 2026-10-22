/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "x11-priv.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) ചെയ്യുക {    \
    എങ്കിൽ (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        പോകുക fail;                                          \
    }                                                       \
} ആകുമ്പോൾ (0)

സ്ഥിതം bool
update_initial_state(ഘടന xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, എണ്ണൽ xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, ശൂന്യ);

    എങ്കിൽ (!reply)
        തിരികെ false;

    /* NOTE: Use the public API with private enum values */
    സ്ഥിരം ഘടന xkb_state_components_update components = {
        .size = വലുപ്പം(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (എണ്ണൽ xkb_keyboard_control_flags) controls,
        .controls = (എണ്ണൽ xkb_keyboard_control_flags) controls,
    };
    സ്ഥിരം ഘടന xkb_synthetic_update update = {
        .size = വലുപ്പം(update),
        .components = &components,
    };
    സ്ഥിരം എണ്ണൽ xkb_status status =
        xkb_state_update_synthetic(state, &update, ശൂന്യ);
    എങ്കിൽ (status != XKB_SUCCESS)
        തിരികെ false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    സ്വതന്ത്രം(reply);
    തിരികെ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#എങ്കിൽ 0
// TODO: currently unused
സ്ഥിതം എണ്ണൽ xkb_a11y_flags
translate_state_accessibility_flags(സ്ഥിരം xcb_xkb_get_controls_reply_t *reply)
{
    എണ്ണൽ xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    എങ്കിൽ (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_താക്കോലുകൾ_LATCH_TO_LOCK;
    }
    തിരികെ flags;
}
#endif

സ്ഥിതം bool
get_controls(ഘടന xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             എണ്ണൽ xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ശൂന്യ);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

ഘടന xkb_state *
xkb_x11_state_new_from_device(ഘടന xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    എങ്കിൽ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        തിരികെ ശൂന്യ;
    }

    എണ്ണൽ xkb_action_controls controls = 0;
    എങ്കിൽ (!get_controls(keymap->ctx, conn, device_id, &controls))
        തിരികെ ശൂന്യ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ഘടന xkb_state * സ്ഥിരം state = xkb_state_new(keymap);
    എങ്കിൽ (!state)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        തിരികെ ശൂന്യ;
    }

    തിരികെ state;
}
