/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "x11-priv.h"

പൂർണ്ണസംഖ്യ
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            എണ്ണൽ xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    സ്ഥിതം സ്ഥിരം എണ്ണൽ xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    എങ്കിൽ (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        തിരികെ 0;
    }

    {
        സ്ഥിരം xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        എങ്കിൽ (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            തിരികെ 0;
        }

        എങ്കിൽ (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            തിരികെ 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = ശൂന്യ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        എങ്കിൽ (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            സ്വതന്ത്രം(error);
            തിരികെ 0;
        }

        എങ്കിൽ (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            സ്വതന്ത്രം(reply);
            തിരികെ 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        സ്വതന്ത്രം(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    എങ്കിൽ (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    എങ്കിൽ (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    എങ്കിൽ (base_event_out)
        *base_event_out = base_event;
    എങ്കിൽ (base_error_out)
        *base_error_out = base_error;

    തിരികെ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, ശൂന്യ);

    എങ്കിൽ (!reply)
        തിരികെ -1;

    device_id = reply->deviceID;
    സ്വതന്ത്രം(reply);
    തിരികെ device_id;
}

ഘടന x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ഘടന {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

സ്ഥിതം ഘടന x11_atom_cache *
get_cache(ഘടന xkb_context *ctx, xcb_connection_t *conn)
{
    എങ്കിൽ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, വലുപ്പം(ഘടന x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ഘടന x11_atom_cache *cache = ctx->x11_atom_cache;
    എങ്കിൽ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    തിരികെ cache;
}

ശൂന്യം
x11_atom_interner_init(ഘടന x11_atom_interner *interner,
                       ഘടന xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->അക്കം_pending = 0;
    interner->അക്കം_copies = 0;
    interner->അക്കം_escaped = 0;
}

ശൂന്യം
x11_atom_interner_adopt_atom(ഘടന x11_atom_interner *interner,
                             സ്ഥിരം xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    എങ്കിൽ (atom == XCB_ATOM_NONE)
        തിരികെ;

    /* Can be NULL in case the malloc failed. */
    ഘടന x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    എങ്കിൽ (cache) {
        വേണ്ടി (size_t c = 0; c < cache->len; c++) {
            എങ്കിൽ (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                തിരികെ;
            }
        }
    }

    /* Already pending? */
    വേണ്ടി (size_t i = 0; i < interner->അക്കം_pending; i++) {
        എങ്കിൽ (interner->pending[i].from == atom) {
            എങ്കിൽ (interner->അക്കം_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                പോകുക retry;
            }

            size_t idx = interner->അക്കം_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            തിരികെ;
        }
    }

    /* We have to send a GetAtomName request */
    എങ്കിൽ (interner->അക്കം_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        പ്രതിജ്ഞാ(interner->അക്കം_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->അക്കം_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

ശൂന്യം
x11_atom_interner_round_trip(ഘടന x11_atom_interner *interner) {
    ഘടന xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ഘടന x11_atom_cache *cache = get_cache(ctx, conn);

    വേണ്ടി (size_t i = 0; i < interner->അക്കം_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, ശൂന്യ);
        എങ്കിൽ (!reply) {
            interner->had_error = true;
            തുടരുക;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        സ്വതന്ത്രം(reply);

        എങ്കിൽ (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        വേണ്ടി (size_t j = 0; j < interner->അക്കം_copies; j++) {
            എങ്കിൽ (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    വേണ്ടി (size_t i = 0; i < interner->അക്കം_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        പൂർണ്ണസംഖ്യ length;
        അക്ഷരം *name;
        അക്ഷരം **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, ശൂന്യ);
        *interner->escaped[i].out = ശൂന്യ;
        എങ്കിൽ (!reply) {
            interner->had_error = true;
        } അല്ലെങ്കിൽ {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            സ്വതന്ത്രം(reply);
            എങ്കിൽ (*out == ശൂന്യ) {
                interner->had_error = true;
            } അല്ലെങ്കിൽ {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->അക്കം_pending = 0;
    interner->അക്കം_copies = 0;
    interner->അക്കം_escaped = 0;
}

ശൂന്യം
x11_atom_interner_get_escaped_atom_name(ഘടന x11_atom_interner *interner,
                                        xcb_atom_t atom, അക്ഷരം **out)
{
    എങ്കിൽ (atom == 0) {
        *out = ശൂന്യ;
        തിരികെ;
    }
    size_t idx = interner->അക്കം_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    പ്രതിജ്ഞാ(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
