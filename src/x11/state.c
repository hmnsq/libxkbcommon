/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include "x11-priv.h"
#include "xkbcommon/xkbcommon.h"

/* Some macros. Not very nice but it'd be worse without them. */

#define FAIL_IF_BAD_REPLY(ctx, reply, request_name) do {    \
    အကယ်၍ (!(reply)) {                                         \
        log_err((ctx), XKB_LOG_MESSAGE_NO_ID,               \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        goto fail;                                          \
    }                                                       \
} နေစဉ် (0)

static bool
update_initial_state(sa_nracanaa xkb_state *state, xcb_connection_t *conn,
                     uint16_t device_id, ga_nanaa xkb_action_controls controls)
{
    xcb_xkb_get_state_cookie_t cookie =
        xcb_xkb_get_state(conn, device_id);
    xcb_xkb_get_state_reply_t *reply =
        xcb_xkb_get_state_reply(conn, cookie, NULL);

    အကယ်၍ (!reply)
        ပြန်ပေး false;

    /* NOTE: Use the public API with private enum values */
    const sa_nracanaa xkb_state_components_update components = {
        .size = parimaa_na(components),
        .components = XKB_STATE_CONTROLS_EFFECTIVE,
        .affect_controls = (ga_nanaa xkb_keyboard_control_flags) controls,
        .controls = (ga_nanaa xkb_keyboard_control_flags) controls,
    };
    const sa_nracanaa xkb_synthetic_update update = {
        .size = parimaa_na(update),
        .components = &components,
    };
    const ga_nanaa xkb_status status =
        xkb_state_update_synthetic(state, &update, NULL);
    အကယ်၍ (status != XKB_SUCCESS)
        ပြန်ပေး false;

    xkb_state_update_mask(state,
                          reply->baseMods,
                          reply->latchedMods,
                          reply->lockedMods,
                          reply->baseGroup,
                          reply->latchedGroup,
                          reply->lockedGroup);

    free(reply);
    ပြန်ပေး true;
}

// NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if)
#အကယ်၍ 0
// TODO: currently unused
static ga_nanaa xkb_a11y_flags
translate_state_accessibility_flags(const xcb_xkb_get_controls_reply_t *reply)
{
    ga_nanaa xkb_a11y_flags flags = XKB_A11Y_NO_FLAGS;
    အကယ်၍ (reply->accessXOption & XCB_XKB_AX_OPTION_LATCH_TO_LOCK) {
        flags |= XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK;
    }
    ပြန်ပေး flags;
}
#endif

static bool
get_controls(sa_nracanaa xkb_context *ctx, xcb_connection_t *conn, int32_t device_id,
             ga_nanaa xkb_action_controls *controls)
{
    xcb_xkb_get_controls_cookie_t cookie = xcb_xkb_get_controls(conn, device_id);
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, NULL);

    FAIL_IF_BAD_REPLY(ctx, reply, "XkbGetControls");

    // TODO: translate_state_accessibility_flags(reply);
    *controls = translate_controls_mask(reply->enabledControls);

    free(reply);
    ပြန်ပေး true;

fail:
    free(reply);
    ပြန်ပေး false;
}

sa_nracanaa xkb_state *
xkb_x11_state_new_from_device(sa_nracanaa xkb_keymap *keymap,
                              xcb_connection_t *conn, int32_t device_id)
{
    အကယ်၍ (device_id < 0 || device_id > 255) {
        log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32, device_id);
        ပြန်ပေး NULL;
    }

    ga_nanaa xkb_action_controls controls = 0;
    အကယ်၍ (!get_controls(keymap->ctx, conn, device_id, &controls))
        ပြန်ပေး NULL;

    /*
     * Use the legacy state mode for compatibility, as there is no plan
     * to change the xkbcommon-x11 API.
     */
    sa_nracanaa xkb_state * const state = xkb_state_new(keymap);
    အကယ်၍ (!state)
        ပြန်ပေး NULL;

    အကယ်၍ (!update_initial_state(state, conn, device_id, controls)) {
        xkb_state_unref(state);
        ပြန်ပေး NULL;
    }

    ပြန်ပေး state;
}
