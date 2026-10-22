/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "x11-priv.h"

ಪೂರ್ಣಾಙ್ಕ
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            ಗಣನಾ xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    ಯದಿ (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    {
        ಧ್ರುವ xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        ಯದಿ (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        ಯದಿ (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = ರಿಕ್ತ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        ಯದಿ (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            ಮುಕ್ತ(error);
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        ಯದಿ (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            ಮುಕ್ತ(reply);
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        ಮುಕ್ತ(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ಯದಿ (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    ಯದಿ (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    ಯದಿ (base_event_out)
        *base_event_out = base_event;
    ಯದಿ (base_error_out)
        *base_error_out = base_error;

    ಪ್ರತ್ಯಾಗಮ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, ರಿಕ್ತ);

    ಯದಿ (!reply)
        ಪ್ರತ್ಯಾಗಮ -1;

    device_id = reply->deviceID;
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ device_id;
}

ಸಂರಚನಾ x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ಸಂರಚನಾ {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

ಸ್ಥಿರ ಸಂರಚನಾ x11_atom_cache *
get_cache(ಸಂರಚನಾ xkb_context *ctx, xcb_connection_t *conn)
{
    ಯದಿ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ಸಂರಚನಾ x11_atom_cache *cache = ctx->x11_atom_cache;
    ಯದಿ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ಪ್ರತ್ಯಾಗಮ cache;
}

ಶೂನ್ಯ
x11_atom_interner_init(ಸಂರಚನಾ x11_atom_interner *interner,
                       ಸಂರಚನಾ xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->ಅಂಕೆ_pending = 0;
    interner->ಅಂಕೆ_copies = 0;
    interner->ಅಂಕೆ_escaped = 0;
}

ಶೂನ್ಯ
x11_atom_interner_adopt_atom(ಸಂರಚನಾ x11_atom_interner *interner,
                             ಧ್ರುವ xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    ಯದಿ (atom == XCB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ;

    /* Can be NULL in case the malloc failed. */
    ಸಂರಚನಾ x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ಯದಿ (cache) {
        ಪ್ರತಿ (size_t c = 0; c < cache->len; c++) {
            ಯದಿ (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
    }

    /* Already pending? */
    ಪ್ರತಿ (size_t i = 0; i < interner->ಅಂಕೆ_pending; i++) {
        ಯದಿ (interner->pending[i].from == atom) {
            ಯದಿ (interner->ಅಂಕೆ_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                ಗಚ್ಛ retry;
            }

            size_t idx = interner->ಅಂಕೆ_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    /* We have to send a GetAtomName request */
    ಯದಿ (interner->ಅಂಕೆ_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        ಪ್ರತಿಜ್ಞಾ(interner->ಅಂಕೆ_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->ಅಂಕೆ_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

ಶೂನ್ಯ
x11_atom_interner_round_trip(ಸಂರಚನಾ x11_atom_interner *interner) {
    ಸಂರಚನಾ xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ಸಂರಚನಾ x11_atom_cache *cache = get_cache(ctx, conn);

    ಪ್ರತಿ (size_t i = 0; i < interner->ಅಂಕೆ_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, ರಿಕ್ತ);
        ಯದಿ (!reply) {
            interner->had_error = true;
            ಅನುವರ್ತ;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        ಮುಕ್ತ(reply);

        ಯದಿ (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        ಪ್ರತಿ (size_t j = 0; j < interner->ಅಂಕೆ_copies; j++) {
            ಯದಿ (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    ಪ್ರತಿ (size_t i = 0; i < interner->ಅಂಕೆ_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        ಪೂರ್ಣಾಙ್ಕ length;
        ವರ್ಣ *name;
        ವರ್ಣ **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, ರಿಕ್ತ);
        *interner->escaped[i].out = ರಿಕ್ತ;
        ಯದಿ (!reply) {
            interner->had_error = true;
        } ಅನ್ಯಥಾ {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            ಮುಕ್ತ(reply);
            ಯದಿ (*out == ರಿಕ್ತ) {
                interner->had_error = true;
            } ಅನ್ಯಥಾ {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->ಅಂಕೆ_pending = 0;
    interner->ಅಂಕೆ_copies = 0;
    interner->ಅಂಕೆ_escaped = 0;
}

ಶೂನ್ಯ
x11_atom_interner_get_escaped_atom_name(ಸಂರಚನಾ x11_atom_interner *interner,
                                        xcb_atom_t atom, ವರ್ಣ **out)
{
    ಯದಿ (atom == 0) {
        *out = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }
    size_t idx = interner->ಅಂಕೆ_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ಪ್ರತಿಜ್ಞಾ(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
