/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "x11-priv.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) करोहि {    \
    सचे (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} याव (0)

स्थिर bool
update_initial_state(संरचना xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, गणना xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, रिक्त);

    सचे (!reply)
        पटिनिवत्त false;

    /* NOTE: Use the public API with private enum values */
    ध्रुव संरचना xkb_state_components_update components = {
        .size = परिमाण(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (गणना xkb_keyboard_control_flags) controls,
        .controls = (गणना xkb_keyboard_control_flags) controls,
    };
    ध्रुव संरचना xkb_synthetic_update update = {
        .size = परिमाण(update),
        .components = &components,
    };
    ध्रुव गणना xkb_status status =
        xkb_state_update_synthetic(state, &update, रिक्त);
    सचे (status != XKB_SUCCESS)
        पटिनिवत्त false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    मुक्त(reply);
    पटिनिवत्त true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#सचे 0
// TODO: currently unused
स्थिर गणना xkb_a11y_flags
translate_state_accessibility_flags(ध्रुव xcb_xkb_get_controls_reply_t *reply)
{
    गणना xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    सचे (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_TO_LOCK;
    }
    पटिनिवत्त flags;
}
#endif

स्थिर bool
get_controls(संरचना xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             गणना xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    मुक्त(reply);
    पटिनिवत्त true;

fail:
    मुक्त(reply);
    पटिनिवत्त false;
}

संरचना xkb_state *
xkb_x11_state_new_from_device(संरचना xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    सचे (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        पटिनिवत्त रिक्त;
    }

    गणना xkb_action_controls controls = 0;
    सचे (!get_controls(keymap->ctx, conn, device_id, &controls))
        पटिनिवत्त रिक्त;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    संरचना xkb_state * ध्रुव state = xkb_state_new(keymap);
    सचे (!state)
        पटिनिवत्त रिक्त;

    सचे (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त state;
}
