/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "x11-priv.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) কুরু {    \
    যদি (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        গচ্ছ fail;                                          \
    }                                                       \
} যাবত্ (0)

স্থির bool
update_initial_state(সংরচনা xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, গণনা xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, রিক্ত);

    যদি (!reply)
        প্রত্যাগম false;

    /* NOTE: Use the public API with private enum values */
    ধ্রুব সংরচনা xkb_state_components_update components = {
        .size = পরিমাণ(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (গণনা xkb_keyboard_control_flags) controls,
        .controls = (গণনা xkb_keyboard_control_flags) controls,
    };
    ধ্রুব সংরচনা xkb_synthetic_update update = {
        .size = পরিমাণ(update),
        .components = &components,
    };
    ধ্রুব গণনা xkb_status status =
        xkb_state_update_synthetic(state, &update, রিক্ত);
    যদি (status != XKB_SUCCESS)
        প্রত্যাগম false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    মুক্ত(reply);
    প্রত্যাগম true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#যদি 0
// TODO: currently unused
স্থির গণনা xkb_a11y_flags
translate_state_accessibility_flags(ধ্রুব xcb_xkb_get_controls_reply_t *reply)
{
    গণনা xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    যদি (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_চাবিগুলি_LATCH_TO_LOCK;
    }
    প্রত্যাগম flags;
}
#endif

স্থির bool
get_controls(সংরচনা xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             গণনা xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, রিক্ত);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

সংরচনা xkb_state *
xkb_x11_state_new_from_device(সংরচনা xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    যদি (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        প্রত্যাগম রিক্ত;
    }

    গণনা xkb_action_controls controls = 0;
    যদি (!get_controls(keymap->ctx, conn, device_id, &controls))
        প্রত্যাগম রিক্ত;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    সংরচনা xkb_state * ধ্রুব state = xkb_state_new(keymap);
    যদি (!state)
        প্রত্যাগম রিক্ত;

    যদি (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম state;
}
