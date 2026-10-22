/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "x11-priv.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            ଗଣନା xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    ସ୍ଥିର ଅଚଳ ଗଣନା xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    ଯଦି (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ଫେରନ୍ତୁ 0;
    }

    {
        ଅଚଳ xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        ଯଦି (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ଫେରନ୍ତୁ 0;
        }

        ଯଦି (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ଫେରନ୍ତୁ 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = ଖାଲି;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        ଯଦି (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            ମୁକ୍ତ(error);
            ଫେରନ୍ତୁ 0;
        }

        ଯଦି (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            ମୁକ୍ତ(reply);
            ଫେରନ୍ତୁ 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        ମୁକ୍ତ(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ଯଦି (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    ଯଦି (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    ଯଦି (base_event_out)
        *base_event_out = base_event;
    ଯଦି (base_error_out)
        *base_error_out = base_error;

    ଫେରନ୍ତୁ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, ଖାଲି);

    ଯଦି (!reply)
        ଫେରନ୍ତୁ -1;

    device_id = reply->deviceID;
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ device_id;
}

ସଂରଚନା x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ସଂରଚନା {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

ସ୍ଥିର ସଂରଚନା x11_atom_cache *
get_cache(ସଂରଚନା xkb_context *ctx, xcb_connection_t *conn)
{
    ଯଦି (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, ଆକାର(ସଂରଚନା x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ସଂରଚନା x11_atom_cache *cache = ctx->x11_atom_cache;
    ଯଦି (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ଫେରନ୍ତୁ cache;
}

ଶୂନ୍ୟ
x11_atom_interner_init(ସଂରଚନା x11_atom_interner *interner,
                       ସଂରଚନା xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->ଅଙ୍କ_pending = 0;
    interner->ଅଙ୍କ_copies = 0;
    interner->ଅଙ୍କ_escaped = 0;
}

ଶୂନ୍ୟ
x11_atom_interner_adopt_atom(ସଂରଚନା x11_atom_interner *interner,
                             ଅଚଳ xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    ଯଦି (atom == XCB_ATOM_NONE)
        ଫେରନ୍ତୁ;

    /* Can be NULL in case the malloc failed. */
    ସଂରଚନା x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ଯଦି (cache) {
        ପାଇଁ (size_t c = 0; c < cache->len; c++) {
            ଯଦି (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ଫେରନ୍ତୁ;
            }
        }
    }

    /* Already pending? */
    ପାଇଁ (size_t i = 0; i < interner->ଅଙ୍କ_pending; i++) {
        ଯଦି (interner->pending[i].from == atom) {
            ଯଦି (interner->ଅଙ୍କ_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                ଯାଅ retry;
            }

            size_t idx = interner->ଅଙ୍କ_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ଫେରନ୍ତୁ;
        }
    }

    /* We have to send a GetAtomName request */
    ଯଦି (interner->ଅଙ୍କ_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        ପ୍ରତିଜ୍ଞା(interner->ଅଙ୍କ_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->ଅଙ୍କ_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

ଶୂନ୍ୟ
x11_atom_interner_round_trip(ସଂରଚନା x11_atom_interner *interner) {
    ସଂରଚନା xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ସଂରଚନା x11_atom_cache *cache = get_cache(ctx, conn);

    ପାଇଁ (size_t i = 0; i < interner->ଅଙ୍କ_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, ଖାଲି);
        ଯଦି (!reply) {
            interner->had_error = true;
            ଜାରି;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        ମୁକ୍ତ(reply);

        ଯଦି (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        ପାଇଁ (size_t j = 0; j < interner->ଅଙ୍କ_copies; j++) {
            ଯଦି (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    ପାଇଁ (size_t i = 0; i < interner->ଅଙ୍କ_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା length;
        ଅକ୍ଷର *name;
        ଅକ୍ଷର **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, ଖାଲି);
        *interner->escaped[i].out = ଖାଲି;
        ଯଦି (!reply) {
            interner->had_error = true;
        } ଅନ୍ୟଥା {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            ମୁକ୍ତ(reply);
            ଯଦି (*out == ଖାଲି) {
                interner->had_error = true;
            } ଅନ୍ୟଥା {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->ଅଙ୍କ_pending = 0;
    interner->ଅଙ୍କ_copies = 0;
    interner->ଅଙ୍କ_escaped = 0;
}

ଶୂନ୍ୟ
x11_atom_interner_get_escaped_atom_name(ସଂରଚନା x11_atom_interner *interner,
                                        xcb_atom_t atom, ଅକ୍ଷର **out)
{
    ଯଦି (atom == 0) {
        *out = ଖାଲି;
        ଫେରନ୍ତୁ;
    }
    size_t idx = interner->ଅଙ୍କ_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ପ୍ରତିଜ୍ଞା(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
