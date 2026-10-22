/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"

صحيح
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            تعداد xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    ساكن ثابت تعداد xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    ܐܢ (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ܦܢܐ 0;
    }

    {
        ثابت xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        ܐܢ (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ܦܢܐ 0;
        }

        ܐܢ (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ܦܢܐ 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = عدم;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        ܐܢ (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            حرر(error);
            ܦܢܐ 0;
        }

        ܐܢ (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            حرر(reply);
            ܦܢܐ 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        حرر(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ܐܢ (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    ܐܢ (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    ܐܢ (base_event_out)
        *base_event_out = base_event;
    ܐܢ (base_error_out)
        *base_error_out = base_error;

    ܦܢܐ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, عدم);

    ܐܢ (!reply)
        ܦܢܐ -1;

    device_id = reply->deviceID;
    حرر(reply);
    ܦܢܐ device_id;
}

بنية x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    بنية {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

ساكن بنية x11_atom_cache *
get_cache(بنية xkb_context *ctx, xcb_connection_t *conn)
{
    ܐܢ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, حجم(بنية x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = ctx->x11_atom_cache;
    ܐܢ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ܦܢܐ cache;
}

فراغ
x11_atom_interner_init(بنية x11_atom_interner *interner,
                       بنية xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->num_pending = 0;
    interner->num_copies = 0;
    interner->num_escaped = 0;
}

فراغ
x11_atom_interner_adopt_atom(بنية x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    ܐܢ (atom == XCB_ATOM_NONE)
        ܦܢܐ;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ܐܢ (cache) {
        لكل (size_t c = 0; c < cache->len; c++) {
            ܐܢ (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ܦܢܐ;
            }
        }
    }

    /* Already pending? */
    لكل (size_t i = 0; i < interner->num_pending; i++) {
        ܐܢ (interner->pending[i].from == atom) {
            ܐܢ (interner->num_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                اذهب retry;
            }

            size_t idx = interner->num_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ܦܢܐ;
        }
    }

    /* We have to send a GetAtomName request */
    ܐܢ (interner->num_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        أكد(interner->num_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->num_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

فراغ
x11_atom_interner_round_trip(بنية x11_atom_interner *interner) {
    بنية xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(ctx, conn);

    لكل (size_t i = 0; i < interner->num_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, عدم);
        ܐܢ (!reply) {
            interner->had_error = true;
            استمر;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        حرر(reply);

        ܐܢ (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        لكل (size_t j = 0; j < interner->num_copies; j++) {
            ܐܢ (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    لكل (size_t i = 0; i < interner->num_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        صحيح length;
        حرف *name;
        حرف **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, عدم);
        *interner->escaped[i].out = عدم;
        ܐܢ (!reply) {
            interner->had_error = true;
        } ܐܚܪܢܐ {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            حرر(reply);
            ܐܢ (*out == عدم) {
                interner->had_error = true;
            } ܐܚܪܢܐ {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->num_pending = 0;
    interner->num_copies = 0;
    interner->num_escaped = 0;
}

فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **out)
{
    ܐܢ (atom == 0) {
        *out = عدم;
        ܦܢܐ;
    }
    size_t idx = interner->num_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
