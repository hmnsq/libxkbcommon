/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "x11-priv.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) गर {    \
    यदि (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} जबसम्म (0)

स्थिर bool
update_initial_state(संरचना xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, गणना xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, रिक्त);

    यदि (!reply)
        फर्काऊ false;

    /* NOTE: Use the public API with private enum values */
    अचर संरचना xkb_state_components_update components = {
        .size = परिमाण(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (गणना xkb_keyboard_control_flags) controls,
        .controls = (गणना xkb_keyboard_control_flags) controls,
    };
    अचर संरचना xkb_synthetic_update update = {
        .size = परिमाण(update),
        .components = &components,
    };
    अचर गणना xkb_status status =
        xkb_state_update_synthetic(state, &update, रिक्त);
    यदि (status != XKB_SUCCESS)
        फर्काऊ false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    मुक्त(reply);
    फर्काऊ true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#यदि 0
// TODO: currently unused
स्थिर गणना xkb_a11y_flags
translate_state_accessibility_flags(अचर xcb_xkb_get_controls_reply_t *reply)
{
    गणना xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    यदि (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11_Y_STICKY_साँचोहरू_LATCH_TO_LOCK;
    }
    फर्काऊ flags;
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
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
}

संरचना xkb_state *
xkb_x11_state_new_from_device(संरचना xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    यदि (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        फर्काऊ रिक्त;
    }

    गणना xkb_action_controls controls = 0;
    यदि (!get_controls(keymap->ctx, conn, device_id, &controls))
        फर्काऊ रिक्त;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    संरचना xkb_state * अचर state = xkb_state_new(keymap);
    यदि (!state)
        फर्काऊ रिक्त;

    यदि (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        फर्काऊ रिक्त;
    }

    फर्काऊ state;
}
