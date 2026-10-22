/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"

صحيح
xkb_x11_راهاندازی_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_نسخه,
                            uint16_t minor_xkb_نسخه,
                            تعداد xkb_x11_راهاندازی_xkb_extension_flags flags,
                            uint16_t *major_xkb_نسخه_بیرون,
                            uint16_t *minor_xkb_نسخه_بیرون,
                            uint8_t *پایه_event_بیرون,
                            uint8_t *پایه_error_بیرون)
{
    uint8_t پایه_event, پایه_error;
    uint16_t server_major, server_minor;

    ساكن ثابت تعداد xkb_x11_راهاندازی_xkb_extension_flags
        XKB_X11_راهاندازی_XKB_EXTENSION_FLAGS = XKB_X11_راهاندازی_XKB_EXTENSION_NO_FLAGS;

    إذا (flags & ~XKB_X11_راهاندازی_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        أرجع 0;
    }

    {
        ثابت xcb_query_extension_پاسخ_t *پاسخ =
            xcb_get_extension_داده(conn, &xcb_xkb_id);
        إذا (!پاسخ) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            أرجع 0;
        }

        إذا (!پاسخ->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            أرجع 0;
        }

        پایه_event = پاسخ->اولی_event;
        پایه_error = پاسخ->اولی_error;
    }

    {
        xcb_generic_error_t *error = عدم;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_نسخه, minor_xkb_نسخه);
        xcb_xkb_use_extension_پاسخ_t *پاسخ =
            xcb_xkb_use_extension_پاسخ(conn, cookie, &error);

        إذا (!پاسخ) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            حرر(error);
            أرجع 0;
        }

        إذا (!پاسخ->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            حرر(پاسخ);
            أرجع 0;
        }

        server_major = پاسخ->serverMajor;
        server_minor = پاسخ->serverMinor;

        حرر(پاسخ);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    إذا (major_xkb_نسخه_بیرون)
        *major_xkb_نسخه_بیرون = server_major;
    إذا (minor_xkb_نسخه_بیرون)
        *minor_xkb_نسخه_بیرون = server_minor;
    إذا (پایه_event_بیرون)
        *پایه_event_بیرون = پایه_event;
    إذا (پایه_error_بیرون)
        *پایه_error_بیرون = پایه_error;

    أرجع 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_پاسخ_t *پاسخ =
        xcb_xkb_get_device_info_پاسخ(conn, cookie, عدم);

    إذا (!پاسخ)
        أرجع -1;

    device_id = پاسخ->deviceID;
    حرر(پاسخ);
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
        xkb_atom_t مقصد;
    } cache[256];
    اندازه_t len;
};

ساكن بنية x11_atom_cache *
get_cache(بنية xkb_context *ctx, xcb_connection_t *conn)
{
    إذا (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = حافظه_صفر_بده(1, حجم(بنية x11_atom_cache));
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
    interner->رقم_pending = 0;
    interner->رقم_copies = 0;
    interner->رقم_escaped = 0;
}

فراغ
x11_atom_interner_adopt_atom(بنية x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *بیرون)
{
    *بیرون = XKB_ATOM_NONE;

    إذا (atom == XCB_ATOM_NONE)
        أرجع;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    إذا (cache) {
        لكل (اندازه_t c = 0; c < cache->len; c++) {
            إذا (cache->cache[c].from == atom) {
                *بیرون = cache->cache[c].مقصد;
                أرجع;
            }
        }
    }

    /* Already pending? */
    لكل (اندازه_t i = 0; i < interner->رقم_pending; i++) {
        إذا (interner->pending[i].from == atom) {
            إذا (interner->رقم_copies == ARRAY_اندازه(interner->copies)) {
                x11_atom_interner_دور_trip(interner);
                اذهب retry;
            }

            اندازه_t idx = interner->رقم_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].بیرون = بیرون;
            أرجع;
        }
    }

    /* We have to send a GetAtomName request */
    إذا (interner->رقم_pending == ARRAY_اندازه(interner->pending)) {
        x11_atom_interner_دور_trip(interner);
        أكد(interner->رقم_pending < ARRAY_اندازه(interner->pending));
    }
    اندازه_t idx = interner->رقم_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].بیرون = بیرون;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

فراغ
x11_atom_interner_دور_trip(بنية x11_atom_interner *interner) {
    بنية xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(ctx, conn);

    لكل (اندازه_t i = 0; i < interner->رقم_pending; i++) {
        xcb_get_atom_name_پاسخ_t *پاسخ;

        پاسخ = xcb_get_atom_name_پاسخ(conn, interner->pending[i].cookie, عدم);
        إذا (!پاسخ) {
            interner->had_error = true;
            استمر;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(پاسخ),
                                          xcb_get_atom_name_name_درازی(پاسخ));
        حرر(پاسخ);

        إذا (cache && cache->len < ARRAY_اندازه(cache->cache)) {
            اندازه_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].مقصد = atom;
        }

        *interner->pending[i].بیرون = atom;

        لكل (اندازه_t j = 0; j < interner->رقم_copies; j++) {
            إذا (interner->copies[j].from == x11_atom)
                *interner->copies[j].بیرون = atom;
        }
    }

    لكل (اندازه_t i = 0; i < interner->رقم_escaped; i++) {
        xcb_get_atom_name_پاسخ_t *پاسخ;
        صحيح درازی;
        حرف *name;
        حرف **بیرون = interner->escaped[i].بیرون;

        پاسخ = xcb_get_atom_name_پاسخ(conn, interner->escaped[i].cookie, عدم);
        *interner->escaped[i].بیرون = عدم;
        إذا (!پاسخ) {
            interner->had_error = true;
        } وإلا {
            درازی = xcb_get_atom_name_name_درازی(پاسخ);
            name = xcb_get_atom_name_name(پاسخ);

            *بیرون = strndup(name, (اندازه_t)درازی);
            حرر(پاسخ);
            إذا (*بیرون == عدم) {
                interner->had_error = true;
            } وإلا {
                XkbEscapeMapName(*بیرون);
            }
        }
    }

    interner->رقم_pending = 0;
    interner->رقم_copies = 0;
    interner->رقم_escaped = 0;
}

فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **بیرون)
{
    إذا (atom == 0) {
        *بیرون = عدم;
        أرجع;
    }
    اندازه_t idx = interner->رقم_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_اندازه(interner->escaped));
    interner->escaped[idx].بیرون = بیرون;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
