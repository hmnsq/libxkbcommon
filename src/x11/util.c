/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "x11-priv.h"

ಪೂರ್ಣಾಂಕ
xkb_x11_ಸಿದ್ಧತೆ_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_ಆವೃತ್ತಿ,
                            uint16_t minor_xkb_ಆವೃತ್ತಿ,
                            ಎಣಿಕೆ xkb_x11_ಸಿದ್ಧತೆ_xkb_extension_flags flags,
                            uint16_t *major_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು,
                            uint16_t *minor_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು,
                            uint8_t *ತಳ_event_ಹೊರಹೋಗು,
                            uint8_t *ತಳ_error_ಹೊರಹೋಗು)
{
    uint8_t ತಳ_event, ತಳ_error;
    uint16_t server_major, server_minor;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_x11_ಸಿದ್ಧತೆ_xkb_extension_flags
        XKB_X11_ಸಿದ್ಧತೆ_XKB_EXTENSION_FLAGS = XKB_X11_ಸಿದ್ಧತೆ_XKB_EXTENSION_NO_FLAGS;

    ಆದರೆ (flags & ~XKB_X11_ಸಿದ್ಧತೆ_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ಹಿಂತಿರುಗಿ 0;
    }

    {
        ಸ್ಥಿರಾಂಕ xcb_query_extension_ಉತ್ತರ_t *ಉತ್ತರ =
            xcb_get_extension_ದತ್ತಾಂಶ(conn, &xcb_xkb_id);
        ಆದರೆ (!ಉತ್ತರ) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ಹಿಂತಿರುಗಿ 0;
        }

        ಆದರೆ (!ಉತ್ತರ->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ಹಿಂತಿರುಗಿ 0;
        }

        ತಳ_event = ಉತ್ತರ->ಮೊದಲ_event;
        ತಳ_error = ಉತ್ತರ->ಮೊದಲ_error;
    }

    {
        xcb_generic_error_t *error = ಖಾಲಿ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_ಆವೃತ್ತಿ, minor_xkb_ಆವೃತ್ತಿ);
        xcb_xkb_use_extension_ಉತ್ತರ_t *ಉತ್ತರ =
            xcb_xkb_use_extension_ಉತ್ತರ(conn, cookie, &error);

        ಆದರೆ (!ಉತ್ತರ) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            ಬಿಡುಗಡೆ(error);
            ಹಿಂತಿರುಗಿ 0;
        }

        ಆದರೆ (!ಉತ್ತರ->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            ಬಿಡುಗಡೆ(ಉತ್ತರ);
            ಹಿಂತಿರುಗಿ 0;
        }

        server_major = ಉತ್ತರ->serverMajor;
        server_minor = ಉತ್ತರ->serverMinor;

        ಬಿಡುಗಡೆ(ಉತ್ತರ);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ಆದರೆ (major_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು)
        *major_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು = server_major;
    ಆದರೆ (minor_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು)
        *minor_xkb_ಆವೃತ್ತಿ_ಹೊರಹೋಗು = server_minor;
    ಆದರೆ (ತಳ_event_ಹೊರಹೋಗು)
        *ತಳ_event_ಹೊರಹೋಗು = ತಳ_event;
    ಆದರೆ (ತಳ_error_ಹೊರಹೋಗು)
        *ತಳ_error_ಹೊರಹೋಗು = ತಳ_error;

    ಹಿಂತಿರುಗಿ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_device_info_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    ಆದರೆ (!ಉತ್ತರ)
        ಹಿಂತಿರುಗಿ -1;

    device_id = ಉತ್ತರ->deviceID;
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ device_id;
}

ರಚನೆ x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ರಚನೆ {
        xcb_atom_t from;
        xkb_atom_t ಗಮ್ಯ;
    } cache[256];
    ಗಾತ್ರ_t len;
};

ಸ್ಥಿರ ರಚನೆ x11_atom_cache *
get_cache(ರಚನೆ xkb_context *ctx, xcb_connection_t *conn)
{
    ಆದರೆ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ರಚನೆ x11_atom_cache *cache = ctx->x11_atom_cache;
    ಆದರೆ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ಹಿಂತಿರುಗಿ cache;
}

ಶೂನ್ಯ
x11_atom_interner_init(ರಚನೆ x11_atom_interner *interner,
                       ರಚನೆ xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->ಅಂಕೆ_pending = 0;
    interner->ಅಂಕೆ_copies = 0;
    interner->ಅಂಕೆ_escaped = 0;
}

ಶೂನ್ಯ
x11_atom_interner_adopt_atom(ರಚನೆ x11_atom_interner *interner,
                             ಸ್ಥಿರಾಂಕ xcb_atom_t atom, xkb_atom_t *ಹೊರಹೋಗು)
{
    *ಹೊರಹೋಗು = XKB_ATOM_NONE;

    ಆದರೆ (atom == XCB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ;

    /* Can be NULL in case the malloc failed. */
    ರಚನೆ x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ಆದರೆ (cache) {
        ಗಾಗಿ (ಗಾತ್ರ_t c = 0; c < cache->len; c++) {
            ಆದರೆ (cache->cache[c].from == atom) {
                *ಹೊರಹೋಗು = cache->cache[c].ಗಮ್ಯ;
                ಹಿಂತಿರುಗಿ;
            }
        }
    }

    /* Already pending? */
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < interner->ಅಂಕೆ_pending; i++) {
        ಆದರೆ (interner->pending[i].from == atom) {
            ಆದರೆ (interner->ಅಂಕೆ_copies == ARRAY_ಗಾತ್ರ(interner->copies)) {
                x11_atom_interner_ಸುತ್ತು_trip(interner);
                ಹೋಗು retry;
            }

            ಗಾತ್ರ_t idx = interner->ಅಂಕೆ_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].ಹೊರಹೋಗು = ಹೊರಹೋಗು;
            ಹಿಂತಿರುಗಿ;
        }
    }

    /* We have to send a GetAtomName request */
    ಆದರೆ (interner->ಅಂಕೆ_pending == ARRAY_ಗಾತ್ರ(interner->pending)) {
        x11_atom_interner_ಸುತ್ತು_trip(interner);
        ಪ್ರತಿಜ್ಞಾ(interner->ಅಂಕೆ_pending < ARRAY_ಗಾತ್ರ(interner->pending));
    }
    ಗಾತ್ರ_t idx = interner->ಅಂಕೆ_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].ಹೊರಹೋಗು = ಹೊರಹೋಗು;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

ಶೂನ್ಯ
x11_atom_interner_ಸುತ್ತು_trip(ರಚನೆ x11_atom_interner *interner) {
    ರಚನೆ xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ರಚನೆ x11_atom_cache *cache = get_cache(ctx, conn);

    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < interner->ಅಂಕೆ_pending; i++) {
        xcb_get_atom_name_ಉತ್ತರ_t *ಉತ್ತರ;

        ಉತ್ತರ = xcb_get_atom_name_ಉತ್ತರ(conn, interner->pending[i].cookie, ಖಾಲಿ);
        ಆದರೆ (!ಉತ್ತರ) {
            interner->had_error = true;
            ಮುಂದುವರಿ;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(ಉತ್ತರ),
                                          xcb_get_atom_name_name_ಉದ್ದಳತೆ(ಉತ್ತರ));
        ಬಿಡುಗಡೆ(ಉತ್ತರ);

        ಆದರೆ (cache && cache->len < ARRAY_ಗಾತ್ರ(cache->cache)) {
            ಗಾತ್ರ_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].ಗಮ್ಯ = atom;
        }

        *interner->pending[i].ಹೊರಹೋಗು = atom;

        ಗಾಗಿ (ಗಾತ್ರ_t j = 0; j < interner->ಅಂಕೆ_copies; j++) {
            ಆದರೆ (interner->copies[j].from == x11_atom)
                *interner->copies[j].ಹೊರಹೋಗು = atom;
        }
    }

    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < interner->ಅಂಕೆ_escaped; i++) {
        xcb_get_atom_name_ಉತ್ತರ_t *ಉತ್ತರ;
        ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ;
        ಅಕ್ಷರ *name;
        ಅಕ್ಷರ **ಹೊರಹೋಗು = interner->escaped[i].ಹೊರಹೋಗು;

        ಉತ್ತರ = xcb_get_atom_name_ಉತ್ತರ(conn, interner->escaped[i].cookie, ಖಾಲಿ);
        *interner->escaped[i].ಹೊರಹೋಗು = ಖಾಲಿ;
        ಆದರೆ (!ಉತ್ತರ) {
            interner->had_error = true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಉದ್ದಳತೆ = xcb_get_atom_name_name_ಉದ್ದಳತೆ(ಉತ್ತರ);
            name = xcb_get_atom_name_name(ಉತ್ತರ);

            *ಹೊರಹೋಗು = strndup(name, (ಗಾತ್ರ_t)ಉದ್ದಳತೆ);
            ಬಿಡುಗಡೆ(ಉತ್ತರ);
            ಆದರೆ (*ಹೊರಹೋಗು == ಖಾಲಿ) {
                interner->had_error = true;
            } ಇಲ್ಲದಿದ್ದರೆ {
                XkbEscapeMapName(*ಹೊರಹೋಗು);
            }
        }
    }

    interner->ಅಂಕೆ_pending = 0;
    interner->ಅಂಕೆ_copies = 0;
    interner->ಅಂಕೆ_escaped = 0;
}

ಶೂನ್ಯ
x11_atom_interner_get_escaped_atom_name(ರಚನೆ x11_atom_interner *interner,
                                        xcb_atom_t atom, ಅಕ್ಷರ **ಹೊರಹೋಗು)
{
    ಆದರೆ (atom == 0) {
        *ಹೊರಹೋಗು = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }
    ಗಾತ್ರ_t idx = interner->ಅಂಕೆ_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ಪ್ರತಿಜ್ಞಾ(idx < ARRAY_ಗಾತ್ರ(interner->escaped));
    interner->escaped[idx].ಹೊರಹೋಗು = ಹೊರಹೋಗು;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
