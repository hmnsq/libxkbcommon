/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश "x11-priv.h"

पूर्णांक
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            क्रमागत xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    अटल स्थिर क्रमागत xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    अगर (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        वापस 0;
    }

    {
        स्थिर xcb_query_extension_reply_t *reply =
            xcb_get_extension_तथ्य(conn, &xcb_xkb_id);
        अगर (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            वापस 0;
        }

        अगर (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            वापस 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = शून्य;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        अगर (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            मुक्त(error);
            वापस 0;
        }

        अगर (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            मुक्त(reply);
            वापस 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        मुक्त(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    अगर (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    अगर (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    अगर (base_event_out)
        *base_event_out = base_event;
    अगर (base_error_out)
        *base_error_out = base_error;

    वापस 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, शून्य);

    अगर (!reply)
        वापस -1;

    device_id = reply->deviceID;
    मुक्त(reply);
    वापस device_id;
}

काष्ठा x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    काष्ठा {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    माप_प्रकार len;
};

अटल काष्ठा x11_atom_cache *
get_cache(काष्ठा xkb_context *ctx, xcb_connection_t *conn)
{
    अगर (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = सुस्मृति(1, माप(काष्ठा x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    काष्ठा x11_atom_cache *cache = ctx->x11_atom_cache;
    अगर (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    वापस cache;
}

व्योम
x11_atom_interner_init(काष्ठा x11_atom_interner *interner,
                       काष्ठा xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->अंक_pending = 0;
    interner->अंक_copies = 0;
    interner->अंक_escaped = 0;
}

व्योम
x11_atom_interner_adopt_atom(काष्ठा x11_atom_interner *interner,
                             स्थिर xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    अगर (atom == XCB_ATOM_NONE)
        वापस;

    /* Can be NULL in case the malloc failed. */
    काष्ठा x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    अगर (cache) {
        क्रम (माप_प्रकार c = 0; c < cache->len; c++) {
            अगर (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                वापस;
            }
        }
    }

    /* Already pending? */
    क्रम (माप_प्रकार i = 0; i < interner->अंक_pending; i++) {
        अगर (interner->pending[i].from == atom) {
            अगर (interner->अंक_copies == ARRAY_आकार(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                जाओ retry;
            }

            माप_प्रकार idx = interner->अंक_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            वापस;
        }
    }

    /* We have to send a GetAtomName request */
    अगर (interner->अंक_pending == ARRAY_आकार(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        निश्चित(interner->अंक_pending < ARRAY_आकार(interner->pending));
    }
    माप_प्रकार idx = interner->अंक_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

व्योम
x11_atom_interner_round_trip(काष्ठा x11_atom_interner *interner) {
    काष्ठा xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    काष्ठा x11_atom_cache *cache = get_cache(ctx, conn);

    क्रम (माप_प्रकार i = 0; i < interner->अंक_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, शून्य);
        अगर (!reply) {
            interner->had_error = true;
            जारी;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_लंबाई(reply));
        मुक्त(reply);

        अगर (cache && cache->len < ARRAY_आकार(cache->cache)) {
            माप_प्रकार idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        क्रम (माप_प्रकार j = 0; j < interner->अंक_copies; j++) {
            अगर (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    क्रम (माप_प्रकार i = 0; i < interner->अंक_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        पूर्णांक लंबाई;
        अक्षर *name;
        अक्षर **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, शून्य);
        *interner->escaped[i].out = शून्य;
        अगर (!reply) {
            interner->had_error = true;
        } अन्यथा {
            लंबाई = xcb_get_atom_name_name_लंबाई(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (माप_प्रकार)लंबाई);
            मुक्त(reply);
            अगर (*out == शून्य) {
                interner->had_error = true;
            } अन्यथा {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->अंक_pending = 0;
    interner->अंक_copies = 0;
    interner->अंक_escaped = 0;
}

व्योम
x11_atom_interner_get_escaped_atom_name(काष्ठा x11_atom_interner *interner,
                                        xcb_atom_t atom, अक्षर **out)
{
    अगर (atom == 0) {
        *out = शून्य;
        वापस;
    }
    माप_प्रकार idx = interner->अंक_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    निश्चित(idx < ARRAY_आकार(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
