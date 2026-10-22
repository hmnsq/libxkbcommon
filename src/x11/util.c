/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "x11-priv.h"

পূর্ণসংখ্যা
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

    স্থির ধ্রুবক গণনা xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    যদি (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ফেরত 0;
    }

    {
        ধ্রুবক xcb_query_extension_reply_t *reply =
            xcb_get_extension_তথ্য(conn, &xcb_xkb_id);
        যদি (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ফেরত 0;
        }

        যদি (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ফেরত 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = ফাঁকা;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        যদি (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            মুক্ত(error);
            ফেরত 0;
        }

        যদি (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            মুক্ত(reply);
            ফেরত 0;
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

    ফেরত 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, ফাঁকা);

    যদি (!reply)
        ফেরত -1;

    device_id = reply->deviceID;
    মুক্ত(reply);
    ফেরত device_id;
}

কাঠামো x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    কাঠামো {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    আকার_t len;
};

স্থির কাঠামো x11_atom_cache *
get_cache(কাঠামো xkb_context *ctx, xcb_connection_t *conn)
{
    যদি (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, আকার(কাঠামো x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    কাঠামো x11_atom_cache *cache = ctx->x11_atom_cache;
    যদি (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ফেরত cache;
}

শূন্য
x11_atom_interner_init(কাঠামো x11_atom_interner *interner,
                       কাঠামো xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->অঙ্ক_pending = 0;
    interner->অঙ্ক_copies = 0;
    interner->অঙ্ক_escaped = 0;
}

শূন্য
x11_atom_interner_adopt_atom(কাঠামো x11_atom_interner *interner,
                             ধ্রুবক xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    যদি (atom == XCB_ATOM_NONE)
        ফেরত;

    /* Can be NULL in case the malloc failed. */
    কাঠামো x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    যদি (cache) {
        জন্য (আকার_t c = 0; c < cache->len; c++) {
            যদি (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ফেরত;
            }
        }
    }

    /* Already pending? */
    জন্য (আকার_t i = 0; i < interner->অঙ্ক_pending; i++) {
        যদি (interner->pending[i].from == atom) {
            যদি (interner->অঙ্ক_copies == ARRAY_আকার(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                যাও retry;
            }

            আকার_t idx = interner->অঙ্ক_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ফেরত;
        }
    }

    /* We have to send a GetAtomName request */
    যদি (interner->অঙ্ক_pending == ARRAY_আকার(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        প্রতিজ্ঞা(interner->অঙ্ক_pending < ARRAY_আকার(interner->pending));
    }
    আকার_t idx = interner->অঙ্ক_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

শূন্য
x11_atom_interner_round_trip(কাঠামো x11_atom_interner *interner) {
    কাঠামো xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    কাঠামো x11_atom_cache *cache = get_cache(ctx, conn);

    জন্য (আকার_t i = 0; i < interner->অঙ্ক_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, ফাঁকা);
        যদি (!reply) {
            interner->had_error = true;
            চালু;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_দৈর্ঘ্য(reply));
        মুক্ত(reply);

        যদি (cache && cache->len < ARRAY_আকার(cache->cache)) {
            আকার_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        জন্য (আকার_t j = 0; j < interner->অঙ্ক_copies; j++) {
            যদি (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    জন্য (আকার_t i = 0; i < interner->অঙ্ক_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        পূর্ণসংখ্যা দৈর্ঘ্য;
        অক্ষর *name;
        অক্ষর **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, ফাঁকা);
        *interner->escaped[i].out = ফাঁকা;
        যদি (!reply) {
            interner->had_error = true;
        } অন্যথা {
            দৈর্ঘ্য = xcb_get_atom_name_name_দৈর্ঘ্য(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (আকার_t)দৈর্ঘ্য);
            মুক্ত(reply);
            যদি (*out == ফাঁকা) {
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
x11_atom_interner_get_escaped_atom_name(কাঠামো x11_atom_interner *interner,
                                        xcb_atom_t atom, অক্ষর **out)
{
    যদি (atom == 0) {
        *out = ফাঁকা;
        ফেরত;
    }
    আকার_t idx = interner->অঙ্ক_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    প্রতিজ্ঞা(idx < ARRAY_আকার(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
