/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"

صحيح
xkb_x11_تیاری_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_نسخه,
                            uint16_t minor_xkb_نسخه,
                            تعداد xkb_x11_تیاری_xkb_extension_flags flags,
                            uint16_t *major_xkb_نسخه_بهر,
                            uint16_t *minor_xkb_نسخه_بهر,
                            uint8_t *بنسټ_event_بهر,
                            uint8_t *بنسټ_error_بهر)
{
    uint8_t بنسټ_event, بنسټ_error;
    uint16_t server_major, server_minor;

    ساكن ثابت تعداد xkb_x11_تیاری_xkb_extension_flags
        XKB_X11_تیاری_XKB_EXTENSION_FLAGS = XKB_X11_تیاری_XKB_EXTENSION_NO_FLAGS;

    إذا (flags & ~XKB_X11_تیاری_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        أرجع 0;
    }

    {
        ثابت xcb_query_extension_ځواب_t *ځواب =
            xcb_get_extension_معلومات(conn, &xcb_xkb_id);
        إذا (!ځواب) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            أرجع 0;
        }

        إذا (!ځواب->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            أرجع 0;
        }

        بنسټ_event = ځواب->لومړی_event;
        بنسټ_error = ځواب->لومړی_error;
    }

    {
        xcb_generic_error_t *error = عدم;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_نسخه, minor_xkb_نسخه);
        xcb_xkb_use_extension_ځواب_t *ځواب =
            xcb_xkb_use_extension_ځواب(conn, cookie, &error);

        إذا (!ځواب) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            حرر(error);
            أرجع 0;
        }

        إذا (!ځواب->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            حرر(ځواب);
            أرجع 0;
        }

        server_major = ځواب->serverMajor;
        server_minor = ځواب->serverMinor;

        حرر(ځواب);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    إذا (major_xkb_نسخه_بهر)
        *major_xkb_نسخه_بهر = server_major;
    إذا (minor_xkb_نسخه_بهر)
        *minor_xkb_نسخه_بهر = server_minor;
    إذا (بنسټ_event_بهر)
        *بنسټ_event_بهر = بنسټ_event;
    إذا (بنسټ_error_بهر)
        *بنسټ_error_بهر = بنسټ_error;

    أرجع 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_ځواب_t *ځواب =
        xcb_xkb_get_device_info_ځواب(conn, cookie, عدم);

    إذا (!ځواب)
        أرجع -1;

    device_id = ځواب->deviceID;
    حرر(ځواب);
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
    اندازه_t len;
};

ساكن بنية x11_atom_cache *
get_cache(بنية xkb_context *ctx, xcb_connection_t *conn)
{
    إذا (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = حافظه_صفر_ورکړه(1, حجم(بنية x11_atom_cache));
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
    interner->عدد_pending = 0;
    interner->عدد_copies = 0;
    interner->عدد_escaped = 0;
}

فراغ
x11_atom_interner_adopt_atom(بنية x11_atom_interner *interner,
                             ثابت xcb_atom_t atom, xkb_atom_t *بهر)
{
    *بهر = XKB_ATOM_NONE;

    إذا (atom == XCB_ATOM_NONE)
        أرجع;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    إذا (cache) {
        لكل (اندازه_t c = 0; c < cache->len; c++) {
            إذا (cache->cache[c].from == atom) {
                *بهر = cache->cache[c].منزل;
                أرجع;
            }
        }
    }

    /* Already pending? */
    لكل (اندازه_t i = 0; i < interner->عدد_pending; i++) {
        إذا (interner->pending[i].from == atom) {
            إذا (interner->عدد_copies == ARRAY_اندازه(interner->copies)) {
                x11_atom_interner_پړاو_trip(interner);
                اذهب retry;
            }

            اندازه_t idx = interner->عدد_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].بهر = بهر;
            أرجع;
        }
    }

    /* We have to send a GetAtomName request */
    إذا (interner->عدد_pending == ARRAY_اندازه(interner->pending)) {
        x11_atom_interner_پړاو_trip(interner);
        أكد(interner->عدد_pending < ARRAY_اندازه(interner->pending));
    }
    اندازه_t idx = interner->عدد_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].بهر = بهر;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

فراغ
x11_atom_interner_پړاو_trip(بنية x11_atom_interner *interner) {
    بنية xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(ctx, conn);

    لكل (اندازه_t i = 0; i < interner->عدد_pending; i++) {
        xcb_get_atom_name_ځواب_t *ځواب;

        ځواب = xcb_get_atom_name_ځواب(conn, interner->pending[i].cookie, عدم);
        إذا (!ځواب) {
            interner->had_error = true;
            استمر;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(ځواب),
                                          xcb_get_atom_name_name_اوږدتیا(ځواب));
        حرر(ځواب);

        إذا (cache && cache->len < ARRAY_اندازه(cache->cache)) {
            اندازه_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].منزل = atom;
        }

        *interner->pending[i].بهر = atom;

        لكل (اندازه_t j = 0; j < interner->عدد_copies; j++) {
            إذا (interner->copies[j].from == x11_atom)
                *interner->copies[j].بهر = atom;
        }
    }

    لكل (اندازه_t i = 0; i < interner->عدد_escaped; i++) {
        xcb_get_atom_name_ځواب_t *ځواب;
        صحيح اوږدتیا;
        حرف *name;
        حرف **بهر = interner->escaped[i].بهر;

        ځواب = xcb_get_atom_name_ځواب(conn, interner->escaped[i].cookie, عدم);
        *interner->escaped[i].بهر = عدم;
        إذا (!ځواب) {
            interner->had_error = true;
        } وإلا {
            اوږدتیا = xcb_get_atom_name_name_اوږدتیا(ځواب);
            name = xcb_get_atom_name_name(ځواب);

            *بهر = strndup(name, (اندازه_t)اوږدتیا);
            حرر(ځواب);
            إذا (*بهر == عدم) {
                interner->had_error = true;
            } وإلا {
                XkbEscapeMapName(*بهر);
            }
        }
    }

    interner->عدد_pending = 0;
    interner->عدد_copies = 0;
    interner->عدد_escaped = 0;
}

فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **بهر)
{
    إذا (atom == 0) {
        *بهر = عدم;
        أرجع;
    }
    اندازه_t idx = interner->عدد_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_اندازه(interner->escaped));
    interner->escaped[idx].بهر = بهر;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
