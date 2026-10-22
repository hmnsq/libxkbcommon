/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "x11-priv.h"

पूर्णाङ्क
xkb_x11_सज्जता_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_संस्करणम्,
                            uint16_t minor_xkb_संस्करणम्,
                            गणना xkb_x11_सज्जता_xkb_extension_flags flags,
                            uint16_t *major_xkb_संस्करणम्_बहिर्गमनम्,
                            uint16_t *minor_xkb_संस्करणम्_बहिर्गमनम्,
                            uint8_t *आधारः_event_बहिर्गमनम्,
                            uint8_t *आधारः_error_बहिर्गमनम्)
{
    uint8_t आधारः_event, आधारः_error;
    uint16_t server_major, server_minor;

    स्थिर ध्रुव गणना xkb_x11_सज्जता_xkb_extension_flags
        XKB_X11_सज्जता_XKB_EXTENSION_FLAGS = XKB_X11_सज्जता_XKB_EXTENSION_NO_FLAGS;

    यदि (flags & ~XKB_X11_सज्जता_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        प्रत्यागम 0;
    }

    {
        ध्रुव xcb_query_extension_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
            xcb_get_extension_दत्त(conn, &xcb_xkb_id);
        यदि (!प्रत्युत्तरम्) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            प्रत्यागम 0;
        }

        यदि (!प्रत्युत्तरम्->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            प्रत्यागम 0;
        }

        आधारः_event = प्रत्युत्तरम्->first_event;
        आधारः_error = प्रत्युत्तरम्->first_error;
    }

    {
        xcb_generic_error_t *error = रिक्त;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_संस्करणम्, minor_xkb_संस्करणम्);
        xcb_xkb_use_extension_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
            xcb_xkb_use_extension_प्रत्युत्तरम्(conn, cookie, &error);

        यदि (!प्रत्युत्तरम्) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            मुक्त(error);
            प्रत्यागम 0;
        }

        यदि (!प्रत्युत्तरम्->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            मुक्त(प्रत्युत्तरम्);
            प्रत्यागम 0;
        }

        server_major = प्रत्युत्तरम्->serverMajor;
        server_minor = प्रत्युत्तरम्->serverMinor;

        मुक्त(प्रत्युत्तरम्);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    यदि (major_xkb_संस्करणम्_बहिर्गमनम्)
        *major_xkb_संस्करणम्_बहिर्गमनम् = server_major;
    यदि (minor_xkb_संस्करणम्_बहिर्गमनम्)
        *minor_xkb_संस्करणम्_बहिर्गमनम् = server_minor;
    यदि (आधारः_event_बहिर्गमनम्)
        *आधारः_event_बहिर्गमनम् = आधारः_event;
    यदि (आधारः_error_बहिर्गमनम्)
        *आधारः_error_बहिर्गमनम् = आधारः_error;

    प्रत्यागम 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_device_info_प्रत्युत्तरम्(conn, cookie, रिक्त);

    यदि (!प्रत्युत्तरम्)
        प्रत्यागम -1;

    device_id = प्रत्युत्तरम्->deviceID;
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम device_id;
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
        xkb_atom_t गन्तव्यम्;
    } cache[256];
    आकार_t len;
};

स्थिर संरचना x11_atom_cache *
get_cache(संरचना xkb_context *ctx, xcb_connection_t *conn)
{
    यदि (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = शून्यस्मृतिं_देहि(1, परिमाण(संरचना x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = ctx->x11_atom_cache;
    यदि (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    प्रत्यागम cache;
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
                             ध्रुव xcb_atom_t atom, xkb_atom_t *बहिर्गमनम्)
{
    *बहिर्गमनम् = XKB_ATOM_NONE;

    यदि (atom == XCB_ATOM_NONE)
        प्रत्यागम;

    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    यदि (cache) {
        प्रति (आकार_t c = 0; c < cache->len; c++) {
            यदि (cache->cache[c].from == atom) {
                *बहिर्गमनम् = cache->cache[c].गन्तव्यम्;
                प्रत्यागम;
            }
        }
    }

    /* Already pending? */
    प्रति (आकार_t i = 0; i < interner->अङ्कः_pending; i++) {
        यदि (interner->pending[i].from == atom) {
            यदि (interner->अङ्कः_copies == ARRAY_आकार(interner->copies)) {
                x11_atom_interner_आवर्तनम्_trip(interner);
                गच्छ retry;
            }

            आकार_t idx = interner->अङ्कः_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].बहिर्गमनम् = बहिर्गमनम्;
            प्रत्यागम;
        }
    }

    /* We have to send a GetAtomName request */
    यदि (interner->अङ्कः_pending == ARRAY_आकार(interner->pending)) {
        x11_atom_interner_आवर्तनम्_trip(interner);
        प्रतिज्ञा(interner->अङ्कः_pending < ARRAY_आकार(interner->pending));
    }
    आकार_t idx = interner->अङ्कः_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].बहिर्गमनम् = बहिर्गमनम्;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

शून्य
x11_atom_interner_आवर्तनम्_trip(संरचना x11_atom_interner *interner) {
    संरचना xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    संरचना x11_atom_cache *cache = get_cache(ctx, conn);

    प्रति (आकार_t i = 0; i < interner->अङ्कः_pending; i++) {
        xcb_get_atom_name_प्रत्युत्तरम्_t *प्रत्युत्तरम्;

        प्रत्युत्तरम् = xcb_get_atom_name_प्रत्युत्तरम्(conn, interner->pending[i].cookie, रिक्त);
        यदि (!प्रत्युत्तरम्) {
            interner->had_error = true;
            अनुवर्त;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(प्रत्युत्तरम्),
                                          xcb_get_atom_name_name_दीर्घता(प्रत्युत्तरम्));
        मुक्त(प्रत्युत्तरम्);

        यदि (cache && cache->len < ARRAY_आकार(cache->cache)) {
            आकार_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].गन्तव्यम् = atom;
        }

        *interner->pending[i].बहिर्गमनम् = atom;

        प्रति (आकार_t j = 0; j < interner->अङ्कः_copies; j++) {
            यदि (interner->copies[j].from == x11_atom)
                *interner->copies[j].बहिर्गमनम् = atom;
        }
    }

    प्रति (आकार_t i = 0; i < interner->अङ्कः_escaped; i++) {
        xcb_get_atom_name_प्रत्युत्तरम्_t *प्रत्युत्तरम्;
        पूर्णाङ्क दीर्घता;
        वर्ण *name;
        वर्ण **बहिर्गमनम् = interner->escaped[i].बहिर्गमनम्;

        प्रत्युत्तरम् = xcb_get_atom_name_प्रत्युत्तरम्(conn, interner->escaped[i].cookie, रिक्त);
        *interner->escaped[i].बहिर्गमनम् = रिक्त;
        यदि (!प्रत्युत्तरम्) {
            interner->had_error = true;
        } अन्यथा {
            दीर्घता = xcb_get_atom_name_name_दीर्घता(प्रत्युत्तरम्);
            name = xcb_get_atom_name_name(प्रत्युत्तरम्);

            *बहिर्गमनम् = strndup(name, (आकार_t)दीर्घता);
            मुक्त(प्रत्युत्तरम्);
            यदि (*बहिर्गमनम् == रिक्त) {
                interner->had_error = true;
            } अन्यथा {
                XkbEscapeMapName(*बहिर्गमनम्);
            }
        }
    }

    interner->अङ्कः_pending = 0;
    interner->अङ्कः_copies = 0;
    interner->अङ्कः_escaped = 0;
}

शून्य
x11_atom_interner_get_escaped_atom_name(संरचना x11_atom_interner *interner,
                                        xcb_atom_t atom, वर्ण **बहिर्गमनम्)
{
    यदि (atom == 0) {
        *बहिर्गमनम् = रिक्त;
        प्रत्यागम;
    }
    आकार_t idx = interner->अङ्कः_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    प्रतिज्ञा(idx < ARRAY_आकार(interner->escaped));
    interner->escaped[idx].बहिर्गमनम् = बहिर्गमनम्;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
