/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "x11-priv.h"

ਪੂਰਨ
xkb_x11_ਤਿਆਰੀ_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_ਸੰਸਕਰਣ,
                            uint16_t minor_xkb_ਸੰਸਕਰਣ,
                            ਗਿਣਤੀ xkb_x11_ਤਿਆਰੀ_xkb_extension_flags flags,
                            uint16_t *major_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ,
                            uint16_t *minor_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ,
                            uint8_t *ਅਧਾਰ_event_ਬਾਹਰਲਾ,
                            uint8_t *ਅਧਾਰ_error_ਬਾਹਰਲਾ)
{
    uint8_t ਅਧਾਰ_event, ਅਧਾਰ_error;
    uint16_t server_major, server_minor;

    ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ xkb_x11_ਤਿਆਰੀ_xkb_extension_flags
        XKB_X11_ਤਿਆਰੀ_XKB_EXTENSION_FLAGS = XKB_X11_ਤਿਆਰੀ_XKB_EXTENSION_NO_FLAGS;

    ਜੇ (flags & ~XKB_X11_ਤਿਆਰੀ_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ਵਾਪਸ 0;
    }

    {
        ਅਚਲ xcb_query_extension_ਜਵਾਬ_t *ਜਵਾਬ =
            xcb_get_extension_ਅੰਕੜੇ(conn, &xcb_xkb_id);
        ਜੇ (!ਜਵਾਬ) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ਵਾਪਸ 0;
        }

        ਜੇ (!ਜਵਾਬ->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ਵਾਪਸ 0;
        }

        ਅਧਾਰ_event = ਜਵਾਬ->ਪਹਿਲਾ_event;
        ਅਧਾਰ_error = ਜਵਾਬ->ਪਹਿਲਾ_error;
    }

    {
        xcb_generic_error_t *error = ਨਿਰਾ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_ਸੰਸਕਰਣ, minor_xkb_ਸੰਸਕਰਣ);
        xcb_xkb_use_extension_ਜਵਾਬ_t *ਜਵਾਬ =
            xcb_xkb_use_extension_ਜਵਾਬ(conn, cookie, &error);

        ਜੇ (!ਜਵਾਬ) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            ਮੁਕਤ(error);
            ਵਾਪਸ 0;
        }

        ਜੇ (!ਜਵਾਬ->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            ਮੁਕਤ(ਜਵਾਬ);
            ਵਾਪਸ 0;
        }

        server_major = ਜਵਾਬ->serverMajor;
        server_minor = ਜਵਾਬ->serverMinor;

        ਮੁਕਤ(ਜਵਾਬ);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ਜੇ (major_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ)
        *major_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ = server_major;
    ਜੇ (minor_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ)
        *minor_xkb_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ = server_minor;
    ਜੇ (ਅਧਾਰ_event_ਬਾਹਰਲਾ)
        *ਅਧਾਰ_event_ਬਾਹਰਲਾ = ਅਧਾਰ_event;
    ਜੇ (ਅਧਾਰ_error_ਬਾਹਰਲਾ)
        *ਅਧਾਰ_error_ਬਾਹਰਲਾ = ਅਧਾਰ_error;

    ਵਾਪਸ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_device_info_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    ਜੇ (!ਜਵਾਬ)
        ਵਾਪਸ -1;

    device_id = ਜਵਾਬ->deviceID;
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ device_id;
}

ਬਣਤਰ x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ਬਣਤਰ {
        xcb_atom_t from;
        xkb_atom_t ਮੰਜ਼ਿਲ;
    } cache[256];
    ਮਾਪ_t len;
};

ਸਥਿਰ ਬਣਤਰ x11_atom_cache *
get_cache(ਬਣਤਰ xkb_context *ctx, xcb_connection_t *conn)
{
    ਜੇ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ਬਣਤਰ x11_atom_cache *cache = ctx->x11_atom_cache;
    ਜੇ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ਵਾਪਸ cache;
}

ਖਾਲੀ
x11_atom_interner_init(ਬਣਤਰ x11_atom_interner *interner,
                       ਬਣਤਰ xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->ਅੰਕ_pending = 0;
    interner->ਅੰਕ_copies = 0;
    interner->ਅੰਕ_escaped = 0;
}

ਖਾਲੀ
x11_atom_interner_adopt_atom(ਬਣਤਰ x11_atom_interner *interner,
                             ਅਚਲ xcb_atom_t atom, xkb_atom_t *ਬਾਹਰਲਾ)
{
    *ਬਾਹਰਲਾ = XKB_ATOM_NONE;

    ਜੇ (atom == XCB_ATOM_NONE)
        ਵਾਪਸ;

    /* Can be NULL in case the malloc failed. */
    ਬਣਤਰ x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ਜੇ (cache) {
        ਲਈ (ਮਾਪ_t c = 0; c < cache->len; c++) {
            ਜੇ (cache->cache[c].from == atom) {
                *ਬਾਹਰਲਾ = cache->cache[c].ਮੰਜ਼ਿਲ;
                ਵਾਪਸ;
            }
        }
    }

    /* Already pending? */
    ਲਈ (ਮਾਪ_t i = 0; i < interner->ਅੰਕ_pending; i++) {
        ਜੇ (interner->pending[i].from == atom) {
            ਜੇ (interner->ਅੰਕ_copies == ARRAY_ਮਾਪ(interner->copies)) {
                x11_atom_interner_ਗੇੜਾ_trip(interner);
                ਜਾਓ retry;
            }

            ਮਾਪ_t idx = interner->ਅੰਕ_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].ਬਾਹਰਲਾ = ਬਾਹਰਲਾ;
            ਵਾਪਸ;
        }
    }

    /* We have to send a GetAtomName request */
    ਜੇ (interner->ਅੰਕ_pending == ARRAY_ਮਾਪ(interner->pending)) {
        x11_atom_interner_ਗੇੜਾ_trip(interner);
        ਪ੍ਰਤਿਜ੍ਞਾ(interner->ਅੰਕ_pending < ARRAY_ਮਾਪ(interner->pending));
    }
    ਮਾਪ_t idx = interner->ਅੰਕ_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].ਬਾਹਰਲਾ = ਬਾਹਰਲਾ;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

ਖਾਲੀ
x11_atom_interner_ਗੇੜਾ_trip(ਬਣਤਰ x11_atom_interner *interner) {
    ਬਣਤਰ xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ਬਣਤਰ x11_atom_cache *cache = get_cache(ctx, conn);

    ਲਈ (ਮਾਪ_t i = 0; i < interner->ਅੰਕ_pending; i++) {
        xcb_get_atom_name_ਜਵਾਬ_t *ਜਵਾਬ;

        ਜਵਾਬ = xcb_get_atom_name_ਜਵਾਬ(conn, interner->pending[i].cookie, ਨਿਰਾ);
        ਜੇ (!ਜਵਾਬ) {
            interner->had_error = true;
            ਜਾਰੀ;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(ਜਵਾਬ),
                                          xcb_get_atom_name_name_ਲੰਮਾਈ(ਜਵਾਬ));
        ਮੁਕਤ(ਜਵਾਬ);

        ਜੇ (cache && cache->len < ARRAY_ਮਾਪ(cache->cache)) {
            ਮਾਪ_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].ਮੰਜ਼ਿਲ = atom;
        }

        *interner->pending[i].ਬਾਹਰਲਾ = atom;

        ਲਈ (ਮਾਪ_t j = 0; j < interner->ਅੰਕ_copies; j++) {
            ਜੇ (interner->copies[j].from == x11_atom)
                *interner->copies[j].ਬਾਹਰਲਾ = atom;
        }
    }

    ਲਈ (ਮਾਪ_t i = 0; i < interner->ਅੰਕ_escaped; i++) {
        xcb_get_atom_name_ਜਵਾਬ_t *ਜਵਾਬ;
        ਪੂਰਨ ਲੰਮਾਈ;
        ਅੱਖਰ *name;
        ਅੱਖਰ **ਬਾਹਰਲਾ = interner->escaped[i].ਬਾਹਰਲਾ;

        ਜਵਾਬ = xcb_get_atom_name_ਜਵਾਬ(conn, interner->escaped[i].cookie, ਨਿਰਾ);
        *interner->escaped[i].ਬਾਹਰਲਾ = ਨਿਰਾ;
        ਜੇ (!ਜਵਾਬ) {
            interner->had_error = true;
        } ਨਹੀਂਤਾਂ {
            ਲੰਮਾਈ = xcb_get_atom_name_name_ਲੰਮਾਈ(ਜਵਾਬ);
            name = xcb_get_atom_name_name(ਜਵਾਬ);

            *ਬਾਹਰਲਾ = strndup(name, (ਮਾਪ_t)ਲੰਮਾਈ);
            ਮੁਕਤ(ਜਵਾਬ);
            ਜੇ (*ਬਾਹਰਲਾ == ਨਿਰਾ) {
                interner->had_error = true;
            } ਨਹੀਂਤਾਂ {
                XkbEscapeMapName(*ਬਾਹਰਲਾ);
            }
        }
    }

    interner->ਅੰਕ_pending = 0;
    interner->ਅੰਕ_copies = 0;
    interner->ਅੰਕ_escaped = 0;
}

ਖਾਲੀ
x11_atom_interner_get_escaped_atom_name(ਬਣਤਰ x11_atom_interner *interner,
                                        xcb_atom_t atom, ਅੱਖਰ **ਬਾਹਰਲਾ)
{
    ਜੇ (atom == 0) {
        *ਬਾਹਰਲਾ = ਨਿਰਾ;
        ਵਾਪਸ;
    }
    ਮਾਪ_t idx = interner->ਅੰਕ_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ਪ੍ਰਤਿਜ੍ਞਾ(idx < ARRAY_ਮਾਪ(interner->escaped));
    interner->escaped[idx].ਬਾਹਰਲਾ = ਬਾਹਰਲਾ;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
