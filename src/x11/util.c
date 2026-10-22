/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "x11-priv.h"

صحیح
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            شمارش xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    ایستا ثابت شمارش xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    اگر (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        بازگشت 0;
    }

    {
        ثابت xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        اگر (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            بازگشت 0;
        }

        اگر (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            بازگشت 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = پوچ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        اگر (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            آزاد(error);
            بازگشت 0;
        }

        اگر (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            آزاد(reply);
            بازگشت 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        آزاد(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    اگر (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    اگر (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    اگر (base_event_out)
        *base_event_out = base_event;
    اگر (base_error_out)
        *base_error_out = base_error;

    بازگشت 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, پوچ);

    اگر (!reply)
        بازگشت -1;

    device_id = reply->deviceID;
    آزاد(reply);
    بازگشت device_id;
}

ساختار x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ساختار {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

ایستا ساختار x11_atom_cache *
get_cache(ساختار xkb_context *ctx, xcb_connection_t *conn)
{
    اگر (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, اندازه(ساختار x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ساختار x11_atom_cache *cache = ctx->x11_atom_cache;
    اگر (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    بازگشت cache;
}

تهی
x11_atom_interner_init(ساختار x11_atom_interner *interner,
                       ساختار xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->رقم_pending = 0;
    interner->رقم_copies = 0;
    interner->رقم_escaped = 0;
}

تهی
x11_atom_interner_adopt_atom(ساختار x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    اگر (atom == XCB_ATOM_NONE)
        بازگشت;

    /* Can be NULL in case the malloc failed. */
    ساختار x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    اگر (cache) {
        برای (size_t c = 0; c < cache->len; c++) {
            اگر (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                بازگشت;
            }
        }
    }

    /* Already pending? */
    برای (size_t i = 0; i < interner->رقم_pending; i++) {
        اگر (interner->pending[i].from == atom) {
            اگر (interner->رقم_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                برو retry;
            }

            size_t idx = interner->رقم_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            بازگشت;
        }
    }

    /* We have to send a GetAtomName request */
    اگر (interner->رقم_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        أكد(interner->رقم_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->رقم_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

تهی
x11_atom_interner_round_trip(ساختار x11_atom_interner *interner) {
    ساختار xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ساختار x11_atom_cache *cache = get_cache(ctx, conn);

    برای (size_t i = 0; i < interner->رقم_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, پوچ);
        اگر (!reply) {
            interner->had_error = true;
            ادامه;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        آزاد(reply);

        اگر (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        برای (size_t j = 0; j < interner->رقم_copies; j++) {
            اگر (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    برای (size_t i = 0; i < interner->رقم_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        صحیح length;
        نویسه *name;
        نویسه **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, پوچ);
        *interner->escaped[i].out = پوچ;
        اگر (!reply) {
            interner->had_error = true;
        } وگرنه {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            آزاد(reply);
            اگر (*out == پوچ) {
                interner->had_error = true;
            } وگرنه {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->رقم_pending = 0;
    interner->رقم_copies = 0;
    interner->رقم_escaped = 0;
}

تهی
x11_atom_interner_get_escaped_atom_name(ساختار x11_atom_interner *interner,
                                        xcb_atom_t atom, نویسه **out)
{
    اگر (atom == 0) {
        *out = پوچ;
        بازگشت;
    }
    size_t idx = interner->رقم_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
