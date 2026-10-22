/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "x11-priv.h"

முழு
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            எண்ணிக்கை xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    நிலையான மாறிலி எண்ணிக்கை xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    எனில் (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        திரும்பு 0;
    }

    {
        மாறிலி xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        எனில் (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            திரும்பு 0;
        }

        எனில் (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            திரும்பு 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = வெற்று;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        எனில் (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            விடுவி(error);
            திரும்பு 0;
        }

        எனில் (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            விடுவி(reply);
            திரும்பு 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        விடுவி(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    எனில் (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    எனில் (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    எனில் (base_event_out)
        *base_event_out = base_event;
    எனில் (base_error_out)
        *base_error_out = base_error;

    திரும்பு 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, வெற்று);

    எனில் (!reply)
        திரும்பு -1;

    device_id = reply->deviceID;
    விடுவி(reply);
    திரும்பு device_id;
}

கட்டமைப்பு x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    கட்டமைப்பு {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

நிலையான கட்டமைப்பு x11_atom_cache *
get_cache(கட்டமைப்பு xkb_context *ctx, xcb_connection_t *conn)
{
    எனில் (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, அளவு(கட்டமைப்பு x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    கட்டமைப்பு x11_atom_cache *cache = ctx->x11_atom_cache;
    எனில் (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    திரும்பு cache;
}

வெறுமை
x11_atom_interner_init(கட்டமைப்பு x11_atom_interner *interner,
                       கட்டமைப்பு xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->இலக்கம்_pending = 0;
    interner->இலக்கம்_copies = 0;
    interner->இலக்கம்_escaped = 0;
}

வெறுமை
x11_atom_interner_adopt_atom(கட்டமைப்பு x11_atom_interner *interner,
                             மாறிலி xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    எனில் (atom == XCB_ATOM_NONE)
        திரும்பு;

    /* Can be NULL in case the malloc failed. */
    கட்டமைப்பு x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    எனில் (cache) {
        க்கு (size_t c = 0; c < cache->len; c++) {
            எனில் (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                திரும்பு;
            }
        }
    }

    /* Already pending? */
    க்கு (size_t i = 0; i < interner->இலக்கம்_pending; i++) {
        எனில் (interner->pending[i].from == atom) {
            எனில் (interner->இலக்கம்_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                செல் retry;
            }

            size_t idx = interner->இலக்கம்_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            திரும்பு;
        }
    }

    /* We have to send a GetAtomName request */
    எனில் (interner->இலக்கம்_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        ப்ரதிஜ்ஞா(interner->இலக்கம்_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->இலக்கம்_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

வெறுமை
x11_atom_interner_round_trip(கட்டமைப்பு x11_atom_interner *interner) {
    கட்டமைப்பு xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    கட்டமைப்பு x11_atom_cache *cache = get_cache(ctx, conn);

    க்கு (size_t i = 0; i < interner->இலக்கம்_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, வெற்று);
        எனில் (!reply) {
            interner->had_error = true;
            தொடர்;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        விடுவி(reply);

        எனில் (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        க்கு (size_t j = 0; j < interner->இலக்கம்_copies; j++) {
            எனில் (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    க்கு (size_t i = 0; i < interner->இலக்கம்_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        முழு length;
        எழுத்து *name;
        எழுத்து **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, வெற்று);
        *interner->escaped[i].out = வெற்று;
        எனில் (!reply) {
            interner->had_error = true;
        } இல்லையெனில் {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            விடுவி(reply);
            எனில் (*out == வெற்று) {
                interner->had_error = true;
            } இல்லையெனில் {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->இலக்கம்_pending = 0;
    interner->இலக்கம்_copies = 0;
    interner->இலக்கம்_escaped = 0;
}

வெறுமை
x11_atom_interner_get_escaped_atom_name(கட்டமைப்பு x11_atom_interner *interner,
                                        xcb_atom_t atom, எழுத்து **out)
{
    எனில் (atom == 0) {
        *out = வெற்று;
        திரும்பு;
    }
    size_t idx = interner->இலக்கம்_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ப்ரதிஜ்ஞா(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
