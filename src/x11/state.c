/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"
#ضمن "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#تعریف_کړه FAIL_IF_BAD_ځواب(ctx, ځواب, request_name) افعل {    \
    إذا (!(ځواب)) {                                         \
        log_err((ctx), XKB_LOG_پیغام_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

ساكن bool
update_initial_state(بنية xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, تعداد xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_ځواب_t *ځواب =
        xcb_xkb_get_state_ځواب(conn, cookie, عدم);

    إذا (!ځواب)
        أرجع false;

    /* NOTE: Use the public API with private enum values */
    ثابت بنية xkb_state_components_update components = {
        .اندازه = حجم(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (تعداد xkb_keyboard_control_flags) controls,
        .controls = (تعداد xkb_keyboard_control_flags) controls,
    };
    ثابت بنية xkb_synthetic_update update = {
        .اندازه = حجم(update),
        .components = &components,
    };
    ثابت تعداد xkb_حالت حالت =
        xkb_state_update_synthetic(state, &update, عدم);
    إذا (حالت != XKB_SUCCESS)
        أرجع false;

    xkb_state_update_ماسک(state,
                          ځواب->بنسټ_Mods,
                          ځواب->latchedMods,
                          ځواب->lockedMods,
                          ځواب->بنسټ_ډله_2,
                          ځواب->latched_ډله_2,
                          ځواب->locked_ډله_2);

    حرر(ځواب);
    أرجع true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#إذا 0
// TODO: currently unused
ساكن تعداد xkb_a11y_flags
translate_state_accessibility_flags(ثابت xcb_xkb_get_controls_ځواب_t *ځواب)
{
    تعداد xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    إذا (ځواب->accessXOption & XCB_XKB_AX_OPTION_LATCH_منزل_قلف) {
        flags |= XKB_A11_Y_STICKY_کلیګانې_LATCH_منزل_قلف;
    }
    أرجع flags;
}
#د_که_پای

ساكن bool
get_controls(بنية xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             تعداد xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_ځواب_t *ځواب =
        xcb_xkb_get_controls_ځواب(conn, cookie, عدم);

    FAIL_IF_BAD_ځواب(ctx, ځواب, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_ماسک(ځواب->enabledControls);

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

بنية xkb_state *
xkb_x11_state_new_from_device(بنية xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    إذا (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        أرجع عدم;
    }

    تعداد xkb_action_controls controls = 0;
    إذا (!get_controls(keymap->ctx, conn, device_id, &controls))
        أرجع عدم;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    بنية xkb_state * ثابت state = xkb_state_new(keymap);
    إذا (!state)
        أرجع عدم;

    إذا (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        أرجع عدم;
    }

    أرجع state;
}
