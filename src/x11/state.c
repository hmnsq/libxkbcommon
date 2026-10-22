/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "x11-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) ಕುರು {    \
    ಯದಿ (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ಗಚ್ಛ fail;                                          \
    }                                                       \
} ಯಾವತ್ (0)

ಸ್ಥಿರ bool
update_initial_state(ಸಂರಚನಾ xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ಗಣನಾ xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, ರಿಕ್ತ);

    ಯದಿ (!reply)
        ಪ್ರತ್ಯಾಗಮ false;

    /* NOTE: Use the public API with private enum values */
    ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update components = {
        .size = ಪರಿಮಾಣ(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ಗಣನಾ xkb_keyboard_control_flags) controls,
        .controls = (ಗಣನಾ xkb_keyboard_control_flags) controls,
    };
    ಧ್ರುವ ಸಂರಚನಾ xkb_synthetic_update update = {
        .size = ಪರಿಮಾಣ(update),
        .components = &components,
    };
    ಧ್ರುವ ಗಣನಾ xkb_status status =
        xkb_state_update_synthetic(state, &update, ರಿಕ್ತ);
    ಯದಿ (status != XKB_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#ಯದಿ 0
// TODO: currently unused
ಸ್ಥಿರ ಗಣನಾ xkb_a11y_flags
translate_state_accessibility_flags(ಧ್ರುವ xcb_xkb_get_controls_reply_t *reply)
{
    ಗಣನಾ xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    ಯದಿ (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_TO_LOCK;
    }
    ಪ್ರತ್ಯಾಗಮ flags;
}
#endif

ಸ್ಥಿರ bool
get_controls(ಸಂರಚನಾ xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ಗಣನಾ xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ರಿಕ್ತ);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸಂರಚನಾ xkb_state *
xkb_x11_state_new_from_device(ಸಂರಚನಾ xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    ಯದಿ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಗಣನಾ xkb_action_controls controls = 0;
    ಯದಿ (!get_controls(keymap->ctx, conn, device_id, &controls))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ಸಂರಚನಾ xkb_state * ಧ್ರುವ state = xkb_state_new(keymap);
    ಯದಿ (!state)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ state;
}
