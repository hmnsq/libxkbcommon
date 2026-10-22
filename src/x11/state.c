/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "x11-priv.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#परिभाषय FAIL_IF_BAD_प्रत्युत्तरम्(ctx, प्रत्युत्तरम्, request_name) कुरु {    \
    यदि (!(प्रत्युत्तरम्)) {                                         \
        log_err((ctx), XKB_LOG_सन्देशः_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} यावत् (0)

स्थिर bool
update_initial_state(संरचना xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, गणना xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_state_प्रत्युत्तरम्(conn, cookie, रिक्त);

    यदि (!प्रत्युत्तरम्)
        प्रत्यागम false;

    /* NOTE: Use the public API with private enum values */
    ध्रुव संरचना xkb_state_components_update components = {
        .आकार = परिमाण(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (गणना xkb_keyboard_control_flags) controls,
        .controls = (गणना xkb_keyboard_control_flags) controls,
    };
    ध्रुव संरचना xkb_synthetic_update update = {
        .आकार = परिमाण(update),
        .components = &components,
    };
    ध्रुव गणना xkb_स्थितिः स्थितिः =
        xkb_state_update_synthetic(state, &update, रिक्त);
    यदि (स्थितिः != XKB_SUCCESS)
        प्रत्यागम false;

    xkb_state_update_आवरणम्(state,
                          प्रत्युत्तरम्->आधारः_Mods,
                          प्रत्युत्तरम्->latchedMods,
                          प्रत्युत्तरम्->lockedMods,
                          प्रत्युत्तरम्->आधारः_गणः_2,
                          प्रत्युत्तरम्->latched_गणः_2,
                          प्रत्युत्तरम्->locked_गणः_2);

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#यदि 0
// TODO: currently unused
स्थिर गणना xkb_a11y_flags
translate_state_accessibility_flags(ध्रुव xcb_xkb_get_controls_प्रत्युत्तरम्_t *प्रत्युत्तरम्)
{
    गणना xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    यदि (प्रत्युत्तरम्->accessXOption & XCB_XKB_AX_OPTION_LATCH_गन्तव्यम्_तालकम्) {
        flags |= XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_गन्तव्यम्_तालकम्;
    }
    प्रत्यागम flags;
}
#यदि_अन्तः

स्थिर bool
get_controls(संरचना xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             गणना xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_controls_प्रत्युत्तरम्(conn, cookie, रिक्त);

    FAIL_IF_BAD_प्रत्युत्तरम्(ctx, प्रत्युत्तरम्, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_आवरणम्(प्रत्युत्तरम्->enabledControls);

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

संरचना xkb_state *
xkb_x11_state_new_from_device(संरचना xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    यदि (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        प्रत्यागम रिक्त;
    }

    गणना xkb_action_controls controls = 0;
    यदि (!get_controls(keymap->ctx, conn, device_id, &controls))
        प्रत्यागम रिक्त;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    संरचना xkb_state * ध्रुव state = xkb_state_new(keymap);
    यदि (!state)
        प्रत्यागम रिक्त;

    यदि (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम state;
}
