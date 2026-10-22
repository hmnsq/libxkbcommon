/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "x11-priv.h"

صحيح
xkb_x11_تیاری_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_نسخہ,
                            uint16_t minor_xkb_نسخہ,
                            تعداد xkb_x11_تیاری_xkb_extension_flags flags,
                            uint16_t *major_xkb_نسخہ_باہرلا,
                            uint16_t *minor_xkb_نسخہ_باہرلا,
                            uint8_t *ادھار_event_باہرلا,
                            uint8_t *ادھار_error_باہرلا)
{
    uint8_t ادھار_event, ادھار_error;
    uint16_t server_major, server_minor;

    ساكن ثابت تعداد xkb_x11_تیاری_xkb_extension_flags
        XKB_X11_تیاری_XKB_EXTENSION_FLAGS = XKB_X11_تیاری_XKB_EXTENSION_NO_FLAGS;

    إذا (flags & ~XKB_X11_تیاری_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        أرجع 0;
    }

    {
        ثابت xcb_query_extension_جواب_t *جواب =
            xcb_get_extension_انکڑے(conn, &xcb_xkb_id);
        إذا (!جواب) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            أرجع 0;
        }

        إذا (!جواب->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            أرجع 0;
        }

        ادھار_event = جواب->پہلا_event;
        ادھار_error = جواب->پہلا_error;
    }

    {
        xcb_generic_error_t *error = عدم;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_نسخہ, minor_xkb_نسخہ);
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

    إذا (major_xkb_نسخہ_باہرلا)
        *major_xkb_نسخہ_باہرلا = server_major;
    إذا (minor_xkb_نسخہ_باہرلا)
        *minor_xkb_نسخہ_باہرلا = server_minor;
    إذا (ادھار_event_باہرلا)
        *ادھار_event_باہرلا = ادھار_event;
    إذا (ادھار_error_باہرلا)
        *ادھار_error_باہرلا = ادھار_error;

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
    ناپ_t len;
};

ساكن بنية x11_atom_cache *
get_cache(بنية xkb_context *ctx, xcb_connection_t *conn)
{
    إذا (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = صفر_حافظہ_دیو(1, حجم(بنية x11_atom_cache));
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
                             ثابت xcb_atom_t atom, xkb_atom_t *باہرلا)
{
    *باہرلا = XKB_ATOM_NONE;

    إذا (atom == XCB_ATOM_NONE)
        أرجع;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    إذا (cache) {
        لكل (ناپ_t c = 0; c < cache->len; c++) {
            إذا (cache->cache[c].from == atom) {
                *باہرلا = cache->cache[c].منزل;
                أرجع;
            }
        }
    }

    /* Already pending? */
    لكل (ناپ_t i = 0; i < interner->عدد_pending; i++) {
        إذا (interner->pending[i].from == atom) {
            إذا (interner->عدد_copies == ARRAY_ناپ(interner->copies)) {
                x11_atom_interner_گیڑا_trip(interner);
                اذهب retry;
            }

            ناپ_t idx = interner->عدد_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].باہرلا = باہرلا;
            أرجع;
        }
    }

    /* We have to send a GetAtomName request */
    إذا (interner->عدد_pending == ARRAY_ناپ(interner->pending)) {
        x11_atom_interner_گیڑا_trip(interner);
        أكد(interner->عدد_pending < ARRAY_ناپ(interner->pending));
    }
    ناپ_t idx = interner->عدد_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].باہرلا = باہرلا;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

فراغ
x11_atom_interner_گیڑا_trip(بنية x11_atom_interner *interner) {
    بنية xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    بنية x11_atom_cache *cache = get_cache(ctx, conn);

    لكل (ناپ_t i = 0; i < interner->عدد_pending; i++) {
        xcb_get_atom_name_جواب_t *جواب;

        جواب = xcb_get_atom_name_جواب(conn, interner->pending[i].cookie, عدم);
        إذا (!جواب) {
            interner->had_error = true;
            استمر;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(جواب),
                                          xcb_get_atom_name_name_لمائی(جواب));
        حرر(جواب);

        إذا (cache && cache->len < ARRAY_ناپ(cache->cache)) {
            ناپ_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].منزل = atom;
        }

        *interner->pending[i].باہرلا = atom;

        لكل (ناپ_t j = 0; j < interner->عدد_copies; j++) {
            إذا (interner->copies[j].from == x11_atom)
                *interner->copies[j].باہرلا = atom;
        }
    }

    لكل (ناپ_t i = 0; i < interner->عدد_escaped; i++) {
        xcb_get_atom_name_جواب_t *جواب;
        صحيح لمائی;
        حرف *name;
        حرف **باہرلا = interner->escaped[i].باہرلا;

        جواب = xcb_get_atom_name_جواب(conn, interner->escaped[i].cookie, عدم);
        *interner->escaped[i].باہرلا = عدم;
        إذا (!جواب) {
            interner->had_error = true;
        } وإلا {
            لمائی = xcb_get_atom_name_name_لمائی(جواب);
            name = xcb_get_atom_name_name(جواب);

            *باہرلا = strndup(name, (ناپ_t)لمائی);
            حرر(جواب);
            إذا (*باہرلا == عدم) {
                interner->had_error = true;
            } وإلا {
                XkbEscapeMapName(*باہرلا);
            }
        }
    }

    interner->عدد_pending = 0;
    interner->عدد_copies = 0;
    interner->عدد_escaped = 0;
}

فراغ
x11_atom_interner_get_escaped_atom_name(بنية x11_atom_interner *interner,
                                        xcb_atom_t atom, حرف **باہرلا)
{
    إذا (atom == 0) {
        *باہرلا = عدم;
        أرجع;
    }
    ناپ_t idx = interner->عدد_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    أكد(idx < ARRAY_ناپ(interner->escaped));
    interner->escaped[idx].باہرلا = باہرلا;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
