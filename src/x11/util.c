/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"

صحيح
xkb_x11_تياري_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_نسخو,
                            uint16_t minor_xkb_نسخو,
                            تعداد xkb_x11_تياري_xkb_extension_flags flags,
                            uint16_t *major_xkb_نسخو_ٻاهريون,
                            uint16_t *minor_xkb_نسخو_ٻاهريون,
                            uint8_t *بنياد_event_ٻاهريون,
                            uint8_t *بنياد_error_ٻاهريون)
{
    uint8_t بنياد_event, بنياد_error;
    uint16_t server_major, server_minor;

    ساكن ثابت تعداد xkb_x11_تياري_xkb_extension_flags
        XKB_X11_تياري_XKB_EXTENSION_FLAGS = XKB_X11_تياري_XKB_EXTENSION_NO_FLAGS;

    إذا (flags & ~XKB_X11_تياري_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        أرجع 0;
    }

    {
        ثابت xcb_query_extension_جواب_t *جواب =
            xcb_get_extension_معلومات(conn, &xcb_xkb_id);
        إذا (!جواب) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            أرجع 0;
        }

        إذا (!جواب->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            أرجع 0;
        }

        بنياد_event = جواب->پهريون_event;
        بنياد_error = جواب->پهريون_error;
    }

    {
        xcb_generic_error_t *error = عدم;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_نسخو, minor_xkb_نسخو);
        xcb_xkb_use_extension_جواب_t *جواب =
            xcb_xkb_use_extension_جواب(conn, cookie, &error);

        إذا (!جواب) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            حرر(error);
            أرجع 0;
        }

        إذا (!جواب->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            حرر(جواب);
            أرجع 0;
        }

        server_major = جواب->serverMajor;
        server_minor = جواب->serverMinor;

        حرر(جواب);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    إذا (major_xkb_نسخو_ٻاهريون)
        *major_xkb_نسخو_ٻاهريون = server_major;
    إذا (minor_xkb_نسخو_ٻاهريون)
        *minor_xkb_نسخو_ٻاهريون = server_minor;
    إذا (بنياد_event_ٻاهريون)
        *بنياد_event_ٻاهريون = بنياد_event;
    إذا (بنياد_error_ٻاهريون)
        *بنياد_error_ٻاهريون = بنياد_error;

    أرجع 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_جواب_t *جواب =
        xcb_xkb_get_device_info_جواب(conn, cookie, عدم);

    إذا (!جواب)
        أرجع -1;

    device_id = جواب->deviceID;
    حرر(جواب);
    أرجع device_id;
}

بنية x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    بنية {
        xcb_atom_t from;
        xkb_atom_t منزل;
    } cache[256];
    سائيز_t len;
};

ساكن بنية x11_atom_cache *
get_cache(بنية xkb_context *ctx, xcb_connection_t *conn)
{
    إذا (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = صفر_يادگيري_ڏيو(1, حجم(بنية x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = ctx->x11_atom_cache;
    إذا (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    أرجع cache;
}

فراغ
x11_atom_interner_init(بنية x11_atom_interner *interner,
                       بنية xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->انگ_pending = 0;
    interner->انگ_copies = 0;
    interner->انگ_escaped = 0;
}

فراغ
x11_atom_interner_adopt_atom(بنية x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *ٻاهريون)
{
    *ٻاهريون = XKB_ATOM_NONE;

    إذا (atom == XCB_ATOM_NONE)
        أرجع;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    إذا (cache) {
        لكل (سائيز_t c = 0; c < cache->len; c++) {
            إذا (cache->cache[c].from == atom) {
                *ٻاهريون = cache->cache[c].منزل;
                أرجع;
            }
        }
    }

    /* Already pending? */
    لكل (سائيز_t i = 0; i < interner->انگ_pending; i++) {
        إذا (interner->pending[i].from == atom) {
            إذا (interner->انگ_copies == ARRAY_سائيز(interner->copies)) {
                x11_atom_interner_ڦيرو_trip(interner);
                اذهب retry;
            }

            سائيز_t idx = interner->انگ_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].ٻاهريون = ٻاهريون;
            أرجع;
        }
    }

    /* We have to send a GetAtomName request */
    إذا (interner->انگ_pending == ARRAY_سائيز(interner->pending)) {
        x11_atom_interner_ڦيرو_trip(interner);
        أكد(interner->انگ_pending < ARRAY_سائيز(interner->pending));
    }
    سائيز_t idx = interner->انگ_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].ٻاهريون = ٻاهريون;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

فراغ
x11_atom_interner_ڦيرو_trip(بنية x11_atom_interner *interner) {
    بنية xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(ctx, conn);

    لكل (سائيز_t i = 0; i < interner->انگ_pending; i++) {
        xcb_get_atom_name_جواب_t *جواب;

        جواب = xcb_get_atom_name_جواب(conn, interner->pending[i].cookie, عدم);
        إذا (!جواب) {
            interner->had_error = true;
            استمر;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(جواب),
                                          xcb_get_atom_name_name_ڊيگهائي(جواب));
        حرر(جواب);

        إذا (cache && cache->len < ARRAY_سائيز(cache->cache)) {
            سائيز_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].منزل = atom;
        }

        *interner->pending[i].ٻاهريون = atom;

        لكل (سائيز_t j = 0; j < interner->انگ_copies; j++) {
            إذا (interner->copies[j].from == x11_atom)
                *interner->copies[j].ٻاهريون = atom;
        }
    }

    لكل (سائيز_t i = 0; i < interner->انگ_escaped; i++) {
        xcb_get_atom_name_جواب_t *جواب;
        صحيح ڊيگهائي;
        حرف *name;
        حرف **ٻاهريون = interner->escaped[i].ٻاهريون;

        جواب = xcb_get_atom_name_جواب(conn, interner->escaped[i].cookie, عدم);
        *interner->escaped[i].ٻاهريون = عدم;
        إذا (!جواب) {
            interner->had_error = true;
        } وإلا {
            ڊيگهائي = xcb_get_atom_name_name_ڊيگهائي(جواب);
            name = xcb_get_atom_name_name(جواب);

            *ٻاهريون = strndup(name, (سائيز_t)ڊيگهائي);
            حرر(جواب);
            إذا (*ٻاهريون == عدم) {
                interner->had_error = true;
            } وإلا {
                XkbEscapeMapName(*ٻاهريون);
            }
        }
    }

    interner->انگ_pending = 0;
    interner->انگ_copies = 0;
    interner->انگ_escaped = 0;
}

فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **ٻاهريون)
{
    إذا (atom == 0) {
        *ٻاهريون = عدم;
        أرجع;
    }
    سائيز_t idx = interner->انگ_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_سائيز(interner->escaped));
    interner->escaped[idx].ٻاهريون = ٻاهريون;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
