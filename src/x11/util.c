/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "x11-priv.h"

পূর্ণাঙ্ক
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            গণনা xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    স্থির ধ্রুৱ গণনা xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    যদি (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ঘূৰাই 0;
    }

    {
        ধ্রুৱ xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        যদি (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ঘূৰাই 0;
        }

        যদি (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ঘূৰাই 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = রিক্ত;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        যদি (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            মুক্ত(error);
            ঘূৰাই 0;
        }

        যদি (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            মুক্ত(reply);
            ঘূৰাই 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        মুক্ত(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    যদি (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    যদি (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    যদি (base_event_out)
        *base_event_out = base_event;
    যদি (base_error_out)
        *base_error_out = base_error;

    ঘূৰাই 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, রিক্ত);

    যদি (!reply)
        ঘূৰাই -1;

    device_id = reply->deviceID;
    মুক্ত(reply);
    ঘূৰাই device_id;
}

সংরচনা x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    সংরচনা {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

স্থির সংরচনা x11_atom_cache *
get_cache(সংরচনা xkb_context *ctx, xcb_connection_t *conn)
{
    যদি (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, পরিমাণ(সংরচনা x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    সংরচনা x11_atom_cache *cache = ctx->x11_atom_cache;
    যদি (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ঘূৰাই cache;
}

শূন্য
x11_atom_interner_init(সংরচনা x11_atom_interner *interner,
                       সংরচনা xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->অঙ্ক_pending = 0;
    interner->অঙ্ক_copies = 0;
    interner->অঙ্ক_escaped = 0;
}

শূন্য
x11_atom_interner_adopt_atom(সংরচনা x11_atom_interner *interner,
                             ধ্রুৱ xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    যদি (atom == XCB_ATOM_NONE)
        ঘূৰাই;

    /* Can be NULL in case the malloc failed. */
    সংরচনা x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    যদি (cache) {
        বাবে (size_t c = 0; c < cache->len; c++) {
            যদি (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ঘূৰাই;
            }
        }
    }

    /* Already pending? */
    বাবে (size_t i = 0; i < interner->অঙ্ক_pending; i++) {
        যদি (interner->pending[i].from == atom) {
            যদি (interner->অঙ্ক_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                গচ্ছ retry;
            }

            size_t idx = interner->অঙ্ক_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ঘূৰাই;
        }
    }

    /* We have to send a GetAtomName request */
    যদি (interner->অঙ্ক_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        প্রতিজ্ঞা(interner->অঙ্ক_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->অঙ্ক_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

শূন্য
x11_atom_interner_round_trip(সংরচনা x11_atom_interner *interner) {
    সংরচনা xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    সংরচনা x11_atom_cache *cache = get_cache(ctx, conn);

    বাবে (size_t i = 0; i < interner->অঙ্ক_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, রিক্ত);
        যদি (!reply) {
            interner->had_error = true;
            অনুৱর্ত;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        মুক্ত(reply);

        যদি (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        বাবে (size_t j = 0; j < interner->অঙ্ক_copies; j++) {
            যদি (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    বাবে (size_t i = 0; i < interner->অঙ্ক_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        পূর্ণাঙ্ক length;
        ৱর্ণ *name;
        ৱর্ণ **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, রিক্ত);
        *interner->escaped[i].out = রিক্ত;
        যদি (!reply) {
            interner->had_error = true;
        } অন্যথা {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            মুক্ত(reply);
            যদি (*out == রিক্ত) {
                interner->had_error = true;
            } অন্যথা {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->অঙ্ক_pending = 0;
    interner->অঙ্ক_copies = 0;
    interner->অঙ্ক_escaped = 0;
}

শূন্য
x11_atom_interner_get_escaped_atom_name(সংরচনা x11_atom_interner *interner,
                                        xcb_atom_t atom, ৱর্ণ **out)
{
    যদি (atom == 0) {
        *out = রিক্ত;
        ঘূৰাই;
    }
    size_t idx = interner->অঙ্ক_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    প্রতিজ্ঞা(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
