/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "x11-priv.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) କର {    \
    ଯଦି (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ଯାଅ fail;                                          \
    }                                                       \
} ଯେତେବେଳେ (0)

ସ୍ଥିର bool
update_initial_state(ସଂରଚନା xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ଗଣନା xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, ଖାଲି);

    ଯଦି (!reply)
        ଫେରନ୍ତୁ false;

    /* NOTE: Use the public API with private enum values */
    ଅଚଳ ସଂରଚନା xkb_state_components_update components = {
        .size = ଆକାର(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ଗଣନା xkb_keyboard_control_flags) controls,
        .controls = (ଗଣନା xkb_keyboard_control_flags) controls,
    };
    ଅଚଳ ସଂରଚନା xkb_synthetic_update update = {
        .size = ଆକାର(update),
        .components = &components,
    };
    ଅଚଳ ଗଣନା xkb_status status =
        xkb_state_update_synthetic(state, &update, ଖାଲି);
    ଯଦି (status != XKB_SUCCESS)
        ଫେରନ୍ତୁ false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#ଯଦି 0
// TODO: currently unused
ସ୍ଥିର ଗଣନା xkb_a11y_flags
translate_state_accessibility_flags(ଅଚଳ xcb_xkb_get_controls_reply_t *reply)
{
    ଗଣନା xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    ଯଦି (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ_LATCH_TO_LOCK;
    }
    ଫେରନ୍ତୁ flags;
}
#endif

ସ୍ଥିର bool
get_controls(ସଂରଚନା xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ଗଣନା xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ଖାଲି);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସଂରଚନା xkb_state *
xkb_x11_state_new_from_device(ସଂରଚନା xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    ଯଦି (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଗଣନା xkb_action_controls controls = 0;
    ଯଦି (!get_controls(keymap->ctx, conn, device_id, &controls))
        ଫେରନ୍ତୁ ଖାଲି;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    ସଂରଚନା xkb_state * ଅଚଳ state = xkb_state_new(keymap);
    ଯଦି (!state)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ state;
}
