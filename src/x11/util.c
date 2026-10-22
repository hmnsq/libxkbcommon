/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "x11-priv.h"

पूर्णाङ्क
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            गणना xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    स्थिर ध्रुव गणना xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    सचे (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        पटिनिवत्त 0;
    }

    {
        ध्रुव xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        सचे (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            पटिनिवत्त 0;
        }

        सचे (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            पटिनिवत्त 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = रिक्त;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        सचे (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            मुक्त(error);
            पटिनिवत्त 0;
        }

        सचे (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            मुक्त(reply);
            पटिनिवत्त 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        मुक्त(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    सचे (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    सचे (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    सचे (base_event_out)
        *base_event_out = base_event;
    सचे (base_error_out)
        *base_error_out = base_error;

    पटिनिवत्त 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, रिक्त);

    सचे (!reply)
        पटिनिवत्त -1;

    device_id = reply->deviceID;
    मुक्त(reply);
    पटिनिवत्त device_id;
}

संरचना x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    संरचना {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

स्थिर संरचना x11_atom_cache *
get_cache(संरचना xkb_context *ctx, xcb_connection_t *conn)
{
    सचे (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, परिमाण(संरचना x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = ctx->x11_atom_cache;
    सचे (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    पटिनिवत्त cache;
}

शून्य
x11_atom_interner_init(संरचना x11_atom_interner *interner,
                       संरचना xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->अङ्कः_pending = 0;
    interner->अङ्कः_copies = 0;
    interner->अङ्कः_escaped = 0;
}

शून्य
x11_atom_interner_adopt_atom(संरचना x11_atom_interner *interner,
                             ध्रुव xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    सचे (atom == XCB_ATOM_NONE)
        पटिनिवत्त;

    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    सचे (cache) {
        अत्थाय (size_t c = 0; c < cache->len; c++) {
            सचे (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                पटिनिवत्त;
            }
        }
    }

    /* Already pending? */
    अत्थाय (size_t i = 0; i < interner->अङ्कः_pending; i++) {
        सचे (interner->pending[i].from == atom) {
            सचे (interner->अङ्कः_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                गच्छ retry;
            }

            size_t idx = interner->अङ्कः_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            पटिनिवत्त;
        }
    }

    /* We have to send a GetAtomName request */
    सचे (interner->अङ्कः_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        प्रतिज्ञा(interner->अङ्कः_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->अङ्कः_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

शून्य
x11_atom_interner_round_trip(संरचना x11_atom_interner *interner) {
    संरचना xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = get_cache(ctx, conn);

    अत्थाय (size_t i = 0; i < interner->अङ्कः_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, रिक्त);
        सचे (!reply) {
            interner->had_error = true;
            अनुवर्त;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        मुक्त(reply);

        सचे (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        अत्थाय (size_t j = 0; j < interner->अङ्कः_copies; j++) {
            सचे (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    अत्थाय (size_t i = 0; i < interner->अङ्कः_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        पूर्णाङ्क length;
        वर्ण *name;
        वर्ण **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, रिक्त);
        *interner->escaped[i].out = रिक्त;
        सचे (!reply) {
            interner->had_error = true;
        } अञ्ञथा {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            मुक्त(reply);
            सचे (*out == रिक्त) {
                interner->had_error = true;
            } अञ्ञथा {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->अङ्कः_pending = 0;
    interner->अङ्कः_copies = 0;
    interner->अङ्कः_escaped = 0;
}

शून्य
x11_atom_interner_get_escaped_atom_name(संरचना x11_atom_interner *interner,
                                        xcb_atom_t atom, वर्ण **out)
{
    सचे (atom == 0) {
        *out = रिक्त;
        पटिनिवत्त;
    }
    size_t idx = interner->अङ्कः_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    प्रतिज्ञा(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
