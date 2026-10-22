/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "x11-priv.h"

પૂર્ણાંક
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            ગણના xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    સ્થિર અચલ ગણના xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    જો (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        પાછા 0;
    }

    {
        અચલ xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        જો (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            પાછા 0;
        }

        જો (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            પાછા 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = ખાલી;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        જો (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            મુક્ત(error);
            પાછા 0;
        }

        જો (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            મુક્ત(reply);
            પાછા 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        મુક્ત(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    જો (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    જો (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    જો (base_event_out)
        *base_event_out = base_event;
    જો (base_error_out)
        *base_error_out = base_error;

    પાછા 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, ખાલી);

    જો (!reply)
        પાછા -1;

    device_id = reply->deviceID;
    મુક્ત(reply);
    પાછા device_id;
}

સંરચના x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    સંરચના {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

સ્થિર સંરચના x11_atom_cache *
get_cache(સંરચના xkb_context *ctx, xcb_connection_t *conn)
{
    જો (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, કદ(સંરચના x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    સંરચના x11_atom_cache *cache = ctx->x11_atom_cache;
    જો (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    પાછા cache;
}

શૂન્ય
x11_atom_interner_init(સંરચના x11_atom_interner *interner,
                       સંરચના xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->અંક_pending = 0;
    interner->અંક_copies = 0;
    interner->અંક_escaped = 0;
}

શૂન્ય
x11_atom_interner_adopt_atom(સંરચના x11_atom_interner *interner,
                             અચલ xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    જો (atom == XCB_ATOM_NONE)
        પાછા;

    /* Can be NULL in case the malloc failed. */
    સંરચના x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    જો (cache) {
        માટે (size_t c = 0; c < cache->len; c++) {
            જો (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                પાછા;
            }
        }
    }

    /* Already pending? */
    માટે (size_t i = 0; i < interner->અંક_pending; i++) {
        જો (interner->pending[i].from == atom) {
            જો (interner->અંક_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                જાઓ retry;
            }

            size_t idx = interner->અંક_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            પાછા;
        }
    }

    /* We have to send a GetAtomName request */
    જો (interner->અંક_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        પ્રતિજ્ઞા(interner->અંક_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->અંક_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

શૂન્ય
x11_atom_interner_round_trip(સંરચના x11_atom_interner *interner) {
    સંરચના xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    સંરચના x11_atom_cache *cache = get_cache(ctx, conn);

    માટે (size_t i = 0; i < interner->અંક_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, ખાલી);
        જો (!reply) {
            interner->had_error = true;
            ચાલુ;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        મુક્ત(reply);

        જો (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        માટે (size_t j = 0; j < interner->અંક_copies; j++) {
            જો (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    માટે (size_t i = 0; i < interner->અંક_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        પૂર્ણાંક length;
        અક્ષર *name;
        અક્ષર **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, ખાલી);
        *interner->escaped[i].out = ખાલી;
        જો (!reply) {
            interner->had_error = true;
        } નહીંતર {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            મુક્ત(reply);
            જો (*out == ખાલી) {
                interner->had_error = true;
            } નહીંતર {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->અંક_pending = 0;
    interner->અંક_copies = 0;
    interner->અંક_escaped = 0;
}

શૂન્ય
x11_atom_interner_get_escaped_atom_name(સંરચના x11_atom_interner *interner,
                                        xcb_atom_t atom, અક્ષર **out)
{
    જો (atom == 0) {
        *out = ખાલી;
        પાછા;
    }
    size_t idx = interner->અંક_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    પ્રતિજ્ઞા(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
