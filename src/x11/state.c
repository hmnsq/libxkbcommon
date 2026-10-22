/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"
#ضمن "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#تعریف_کن FAIL_IF_BAD_پاسخ(ctx, پاسخ, request_name) افعل {    \
    إذا (!(پاسخ)) {                                         \
        log_err((ctx), XKB_LOG_پیام_NO_ID,               \
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
    xcb_xkb_get_state_پاسخ_t *پاسخ =
        xcb_xkb_get_state_پاسخ(conn, cookie, عدم);

    إذا (!پاسخ)
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
    ثابت تعداد xkb_وضعیت وضعیت =
        xkb_state_update_synthetic(state, &update, عدم);
    إذا (وضعیت != XKB_SUCCESS)
        أرجع false;

    xkb_state_update_ماسک(state,
                          پاسخ->پایه_Mods,
                          پاسخ->latchedMods,
                          پاسخ->lockedMods,
                          پاسخ->پایه_گروه_2,
                          پاسخ->latched_گروه_2,
                          پاسخ->locked_گروه_2);

    حرر(پاسخ);
    أرجع true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#إذا 0
// TODO: currently unused
ساكن تعداد xkb_a11y_flags
translate_state_accessibility_flags(ثابت xcb_xkb_get_controls_پاسخ_t *پاسخ)
{
    تعداد xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    إذا (پاسخ->accessXOption & XCB_XKB_AX_OPTION_LATCH_مقصد_قفل) {
        flags |= XKB_A11_Y_STICKY_کلیدها_LATCH_مقصد_قفل;
    }
    أرجع flags;
}
#پایان_اگر

ساكن bool
get_controls(بنية xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             تعداد xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_پاسخ_t *پاسخ =
        xcb_xkb_get_controls_پاسخ(conn, cookie, عدم);

    FAIL_IF_BAD_پاسخ(ctx, پاسخ, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_ماسک(پاسخ->enabledControls);

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

بنية xkb_state *
xkb_x11_state_new_from_device(بنية xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    إذا (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_پیام_NO_ID,
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
