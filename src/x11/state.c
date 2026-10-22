/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "x11-priv.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#నిర్వచించు FAIL_IF_BAD_జవాబు(ctx, జవాబు, request_name) చేయి {    \
    ఒకవేళ (!(జవాబు)) {                                         \
        log_err((ctx), XKB_LOG_సందేశం_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        వెళ్ళు fail;                                          \
    }                                                       \
} అయితే (0)

స్థిర bool
update_initial_state(నిర్మాణం xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, గణన xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_జవాబు_t *జవాబు =
        xcb_xkb_get_state_జవాబు(conn, cookie, ఖాళీ);

    ఒకవేళ (!జవాబు)
        తిరిగి false;

    /* NOTE: Use the public API with private enum values */
    స్థిరరాశి నిర్మాణం xkb_state_components_update components = {
        .కొలత = పరిమాణం(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (గణన xkb_keyboard_control_flags) controls,
        .controls = (గణన xkb_keyboard_control_flags) controls,
    };
    స్థిరరాశి నిర్మాణం xkb_synthetic_update update = {
        .కొలత = పరిమాణం(update),
        .components = &components,
    };
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
        xkb_state_update_synthetic(state, &update, ఖాళీ);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        తిరిగి false;

    xkb_state_update_ముసుగు(state,
                          జవాబు->పునాది_Mods,
                          జవాబు->latchedMods,
                          జవాబు->lockedMods,
                          జవాబు->పునాది_సమూహం_2,
                          జవాబు->latched_సమూహం_2,
                          జవాబు->locked_సమూహం_2);

    విడుదల(జవాబు);
    తిరిగి true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#ఒకవేళ 0
// TODO: currently unused
స్థిర గణన xkb_a11y_flags
translate_state_accessibility_flags(స్థిరరాశి xcb_xkb_get_controls_జవాబు_t *జవాబు)
{
    గణన xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    ఒకవేళ (జవాబు->accessXOption & XCB_XKB_AX_OPTION_LATCH_గమ్యం_తాళం) {
        flags |= XKB_A11_Y_STICKY_కీలకాలు_LATCH_గమ్యం_తాళం;
    }
    తిరిగి flags;
}
#అయితే_ముగింపు

స్థిర bool
get_controls(నిర్మాణం xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             గణన xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_జవాబు_t *జవాబు =
        xcb_xkb_get_controls_జవాబు(conn, cookie, ఖాళీ);

    FAIL_IF_BAD_జవాబు(ctx, జవాబు, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_ముసుగు(జవాబు->enabledControls);

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

నిర్మాణం xkb_state *
xkb_x11_state_new_from_device(నిర్మాణం xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    ఒకవేళ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        తిరిగి ఖాళీ;
    }

    గణన xkb_action_controls controls = 0;
    ఒకవేళ (!get_controls(keymap->ctx, conn, device_id, &controls))
        తిరిగి ఖాళీ;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    నిర్మాణం xkb_state * స్థిరరాశి state = xkb_state_new(keymap);
    ఒకవేళ (!state)
        తిరిగి ఖాళీ;

    ఒకవేళ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        తిరిగి ఖాళీ;
    }

    తిరిగి state;
}
