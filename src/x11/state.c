/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "x11-priv.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) செய் {    \
    எனில் (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        செல் fail;                                          \
    }                                                       \
} போது (0)

நிலையான bool
update_initial_state(கட்டமைப்பு xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, எண்ணிக்கை xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, வெற்று);

    எனில் (!reply)
        திரும்பு false;

    /* NOTE: Use the public API with private enum values */
    மாறிலி கட்டமைப்பு xkb_state_components_update components = {
        .size = அளவு(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (எண்ணிக்கை xkb_keyboard_control_flags) controls,
        .controls = (எண்ணிக்கை xkb_keyboard_control_flags) controls,
    };
    மாறிலி கட்டமைப்பு xkb_synthetic_update update = {
        .size = அளவு(update),
        .components = &components,
    };
    மாறிலி எண்ணிக்கை xkb_status status =
        xkb_state_update_synthetic(state, &update, வெற்று);
    எனில் (status != XKB_SUCCESS)
        திரும்பு false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    விடுவி(reply);
    திரும்பு true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#எனில் 0
// TODO: currently unused
நிலையான எண்ணிக்கை xkb_a11y_flags
translate_state_accessibility_flags(மாறிலி xcb_xkb_get_controls_reply_t *reply)
{
    எண்ணிக்கை xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    எனில் (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_சாவிகள்_LATCH_TO_LOCK;
    }
    திரும்பு flags;
}
#endif

நிலையான bool
get_controls(கட்டமைப்பு xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             எண்ணிக்கை xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, வெற்று);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

கட்டமைப்பு xkb_state *
xkb_x11_state_new_from_device(கட்டமைப்பு xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    எனில் (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        திரும்பு வெற்று;
    }

    எண்ணிக்கை xkb_action_controls controls = 0;
    எனில் (!get_controls(keymap->ctx, conn, device_id, &controls))
        திரும்பு வெற்று;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    கட்டமைப்பு xkb_state * மாறிலி state = xkb_state_new(keymap);
    எனில் (!state)
        திரும்பு வெற்று;

    எனில் (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        திரும்பு வெற்று;
    }

    திரும்பு state;
}
