/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "x11-priv.h"

صحیح
xkb_x11_تیاری_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_نسخہ,
                            uint16_t minor_xkb_نسخہ,
                            شمار xkb_x11_تیاری_xkb_extension_flags flags,
                            uint16_t *major_xkb_نسخہ_برآمد,
                            uint16_t *minor_xkb_نسخہ_برآمد,
                            uint8_t *بنیاد_event_برآمد,
                            uint8_t *بنیاد_error_برآمد)
{
    uint8_t بنیاد_event, بنیاد_error;
    uint16_t server_major, server_minor;

    ساکن ثابت شمار xkb_x11_تیاری_xkb_extension_flags
        XKB_X11_تیاری_XKB_EXTENSION_FLAGS = XKB_X11_تیاری_XKB_EXTENSION_NO_FLAGS;

    اگر (flags & ~XKB_X11_تیاری_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        واپس 0;
    }

    {
        ثابت xcb_query_extension_جواب_t *جواب =
            xcb_get_extension_ڈیٹا(conn, &xcb_xkb_id);
        اگر (!جواب) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            واپس 0;
        }

        اگر (!جواب->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            واپس 0;
        }

        بنیاد_event = جواب->پہلا_event;
        بنیاد_error = جواب->پہلا_error;
    }

    {
        xcb_generic_error_t *error = تہی;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_نسخہ, minor_xkb_نسخہ);
        xcb_xkb_use_extension_جواب_t *جواب =
            xcb_xkb_use_extension_جواب(conn, cookie, &error);

        اگر (!جواب) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            آزاد(error);
            واپس 0;
        }

        اگر (!جواب->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            آزاد(جواب);
            واپس 0;
        }

        server_major = جواب->serverMajor;
        server_minor = جواب->serverMinor;

        آزاد(جواب);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    اگر (major_xkb_نسخہ_برآمد)
        *major_xkb_نسخہ_برآمد = server_major;
    اگر (minor_xkb_نسخہ_برآمد)
        *minor_xkb_نسخہ_برآمد = server_minor;
    اگر (بنیاد_event_برآمد)
        *بنیاد_event_برآمد = بنیاد_event;
    اگر (بنیاد_error_برآمد)
        *بنیاد_error_برآمد = بنیاد_error;

    واپس 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_جواب_t *جواب =
        xcb_xkb_get_device_info_جواب(conn, cookie, تہی);

    اگر (!جواب)
        واپس -1;

    device_id = جواب->deviceID;
    آزاد(جواب);
    واپس device_id;
}

ساخت x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    ساخت {
        xcb_atom_t from;
        xkb_atom_t مقصد;
    } cache[256];
    سائز_t len;
};

ساکن ساخت x11_atom_cache *
get_cache(ساخت xkb_context *ctx, xcb_connection_t *conn)
{
    اگر (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = صفر_حافظہ_دو(1, حجم(ساخت x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    ساخت x11_atom_cache *cache = ctx->x11_atom_cache;
    اگر (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    واپس cache;
}

خالی
x11_atom_interner_init(ساخت x11_atom_interner *interner,
                       ساخت xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->عدد_pending = 0;
    interner->عدد_copies = 0;
    interner->عدد_escaped = 0;
}

خالی
x11_atom_interner_adopt_atom(ساخت x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *برآمد)
{
    *برآمد = XKB_ATOM_NONE;

    اگر (atom == XCB_ATOM_NONE)
        واپس;

    /* Can be NULL in case the malloc failed. */
    ساخت x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    اگر (cache) {
        برائے (سائز_t c = 0; c < cache->len; c++) {
            اگر (cache->cache[c].from == atom) {
                *برآمد = cache->cache[c].مقصد;
                واپس;
            }
        }
    }

    /* Already pending? */
    برائے (سائز_t i = 0; i < interner->عدد_pending; i++) {
        اگر (interner->pending[i].from == atom) {
            اگر (interner->عدد_copies == ARRAY_سائز(interner->copies)) {
                x11_atom_interner_باری_trip(interner);
                جاؤ retry;
            }

            سائز_t idx = interner->عدد_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].برآمد = برآمد;
            واپس;
        }
    }

    /* We have to send a GetAtomName request */
    اگر (interner->عدد_pending == ARRAY_سائز(interner->pending)) {
        x11_atom_interner_باری_trip(interner);
        أكد(interner->عدد_pending < ARRAY_سائز(interner->pending));
    }
    سائز_t idx = interner->عدد_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].برآمد = برآمد;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

خالی
x11_atom_interner_باری_trip(ساخت x11_atom_interner *interner) {
    ساخت xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    ساخت x11_atom_cache *cache = get_cache(ctx, conn);

    برائے (سائز_t i = 0; i < interner->عدد_pending; i++) {
        xcb_get_atom_name_جواب_t *جواب;

        جواب = xcb_get_atom_name_جواب(conn, interner->pending[i].cookie, تہی);
        اگر (!جواب) {
            interner->had_error = true;
            جاری;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(جواب),
                                          xcb_get_atom_name_name_لمبائی(جواب));
        آزاد(جواب);

        اگر (cache && cache->len < ARRAY_سائز(cache->cache)) {
            سائز_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].مقصد = atom;
        }

        *interner->pending[i].برآمد = atom;

        برائے (سائز_t j = 0; j < interner->عدد_copies; j++) {
            اگر (interner->copies[j].from == x11_atom)
                *interner->copies[j].برآمد = atom;
        }
    }

    برائے (سائز_t i = 0; i < interner->عدد_escaped; i++) {
        xcb_get_atom_name_جواب_t *جواب;
        صحیح لمبائی;
        حرف *name;
        حرف **برآمد = interner->escaped[i].برآمد;

        جواب = xcb_get_atom_name_جواب(conn, interner->escaped[i].cookie, تہی);
        *interner->escaped[i].برآمد = تہی;
        اگر (!جواب) {
            interner->had_error = true;
        } ورنہ {
            لمبائی = xcb_get_atom_name_name_لمبائی(جواب);
            name = xcb_get_atom_name_name(جواب);

            *برآمد = strndup(name, (سائز_t)لمبائی);
            آزاد(جواب);
            اگر (*برآمد == تہی) {
                interner->had_error = true;
            } ورنہ {
                XkbEscapeMapName(*برآمد);
            }
        }
    }

    interner->عدد_pending = 0;
    interner->عدد_copies = 0;
    interner->عدد_escaped = 0;
}

خالی
x11_atom_interner_get_escaped_atom_name(ساخت x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **برآمد)
{
    اگر (atom == 0) {
        *برآمد = تہی;
        واپس;
    }
    سائز_t idx = interner->عدد_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_سائز(interner->escaped));
    interner->escaped[idx].برآمد = برآمد;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
