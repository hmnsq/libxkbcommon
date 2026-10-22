/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "x11-priv.h"

పూర్ణాంకం
xkb_x11_అమరిక_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_సంస్కరణ,
                            uint16_t minor_xkb_సంస్కరణ,
                            గణన xkb_x11_అమరిక_xkb_extension_flags flags,
                            uint16_t *major_xkb_సంస్కరణ_బహిర్గతం,
                            uint16_t *minor_xkb_సంస్కరణ_బహిర్గతం,
                            uint8_t *పునాది_event_బహిర్గతం,
                            uint8_t *పునాది_error_బహిర్గతం)
{
    uint8_t పునాది_event, పునాది_error;
    uint16_t server_major, server_minor;

    స్థిర స్థిరరాశి గణన xkb_x11_అమరిక_xkb_extension_flags
        XKB_X11_అమరిక_XKB_EXTENSION_FLAGS = XKB_X11_అమరిక_XKB_EXTENSION_NO_FLAGS;

    ఒకవేళ (flags & ~XKB_X11_అమరిక_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        తిరిగి 0;
    }

    {
        స్థిరరాశి xcb_query_extension_జవాబు_t *జవాబు =
            xcb_get_extension_దత్తాంశం(conn, &xcb_xkb_id);
        ఒకవేళ (!జవాబు) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            తిరిగి 0;
        }

        ఒకవేళ (!జవాబు->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            తిరిగి 0;
        }

        పునాది_event = జవాబు->మొదటి_event;
        పునాది_error = జవాబు->మొదటి_error;
    }

    {
        xcb_generic_error_t *error = ఖాళీ;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_సంస్కరణ, minor_xkb_సంస్కరణ);
        xcb_xkb_use_extension_జవాబు_t *జవాబు =
            xcb_xkb_use_extension_జవాబు(conn, cookie, &error);

        ఒకవేళ (!జవాబు) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            విడుదల(error);
            తిరిగి 0;
        }

        ఒకవేళ (!జవాబు->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            విడుదల(జవాబు);
            తిరిగి 0;
        }

        server_major = జవాబు->serverMajor;
        server_minor = జవాబు->serverMinor;

        విడుదల(జవాబు);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ఒకవేళ (major_xkb_సంస్కరణ_బహిర్గతం)
        *major_xkb_సంస్కరణ_బహిర్గతం = server_major;
    ఒకవేళ (minor_xkb_సంస్కరణ_బహిర్గతం)
        *minor_xkb_సంస్కరణ_బహిర్గతం = server_minor;
    ఒకవేళ (పునాది_event_బహిర్గతం)
        *పునాది_event_బహిర్గతం = పునాది_event;
    ఒకవేళ (పునాది_error_బహిర్గతం)
        *పునాది_error_బహిర్గతం = పునాది_error;

    తిరిగి 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_జవాబు_t *జవాబు =
        xcb_xkb_get_device_info_జవాబు(conn, cookie, ఖాళీ);

    ఒకవేళ (!జవాబు)
        తిరిగి -1;

    device_id = జవాబు->deviceID;
    విడుదల(జవాబు);
    తిరిగి device_id;
}

నిర్మాణం x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    నిర్మాణం {
        xcb_atom_t from;
        xkb_atom_t గమ్యం;
    } cache[256];
    కొలత_t len;
};

స్థిర నిర్మాణం x11_atom_cache *
get_cache(నిర్మాణం xkb_context *ctx, xcb_connection_t *conn)
{
    ఒకవేళ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    నిర్మాణం x11_atom_cache *cache = ctx->x11_atom_cache;
    ఒకవేళ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    తిరిగి cache;
}

శూన్య
x11_atom_interner_init(నిర్మాణం x11_atom_interner *interner,
                       నిర్మాణం xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->అంకె_pending = 0;
    interner->అంకె_copies = 0;
    interner->అంకె_escaped = 0;
}

శూన్య
x11_atom_interner_adopt_atom(నిర్మాణం x11_atom_interner *interner,
                             స్థిరరాశి xcb_atom_t atom, xkb_atom_t *బహిర్గతం)
{
    *బహిర్గతం = XKB_ATOM_NONE;

    ఒకవేళ (atom == XCB_ATOM_NONE)
        తిరిగి;

    /* Can be NULL in case the malloc failed. */
    నిర్మాణం x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ఒకవేళ (cache) {
        కొరకు (కొలత_t c = 0; c < cache->len; c++) {
            ఒకవేళ (cache->cache[c].from == atom) {
                *బహిర్గతం = cache->cache[c].గమ్యం;
                తిరిగి;
            }
        }
    }

    /* Already pending? */
    కొరకు (కొలత_t i = 0; i < interner->అంకె_pending; i++) {
        ఒకవేళ (interner->pending[i].from == atom) {
            ఒకవేళ (interner->అంకె_copies == ARRAY_కొలత(interner->copies)) {
                x11_atom_interner_విడత_trip(interner);
                వెళ్ళు retry;
            }

            కొలత_t idx = interner->అంకె_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].బహిర్గతం = బహిర్గతం;
            తిరిగి;
        }
    }

    /* We have to send a GetAtomName request */
    ఒకవేళ (interner->అంకె_pending == ARRAY_కొలత(interner->pending)) {
        x11_atom_interner_విడత_trip(interner);
        ప్రతిజ్ఞా(interner->అంకె_pending < ARRAY_కొలత(interner->pending));
    }
    కొలత_t idx = interner->అంకె_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].బహిర్గతం = బహిర్గతం;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

శూన్య
x11_atom_interner_విడత_trip(నిర్మాణం x11_atom_interner *interner) {
    నిర్మాణం xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    నిర్మాణం x11_atom_cache *cache = get_cache(ctx, conn);

    కొరకు (కొలత_t i = 0; i < interner->అంకె_pending; i++) {
        xcb_get_atom_name_జవాబు_t *జవాబు;

        జవాబు = xcb_get_atom_name_జవాబు(conn, interner->pending[i].cookie, ఖాళీ);
        ఒకవేళ (!జవాబు) {
            interner->had_error = true;
            కొనసాగు;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(జవాబు),
                                          xcb_get_atom_name_name_నిడివి(జవాబు));
        విడుదల(జవాబు);

        ఒకవేళ (cache && cache->len < ARRAY_కొలత(cache->cache)) {
            కొలత_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].గమ్యం = atom;
        }

        *interner->pending[i].బహిర్గతం = atom;

        కొరకు (కొలత_t j = 0; j < interner->అంకె_copies; j++) {
            ఒకవేళ (interner->copies[j].from == x11_atom)
                *interner->copies[j].బహిర్గతం = atom;
        }
    }

    కొరకు (కొలత_t i = 0; i < interner->అంకె_escaped; i++) {
        xcb_get_atom_name_జవాబు_t *జవాబు;
        పూర్ణాంకం నిడివి;
        అక్షరం *name;
        అక్షరం **బహిర్గతం = interner->escaped[i].బహిర్గతం;

        జవాబు = xcb_get_atom_name_జవాబు(conn, interner->escaped[i].cookie, ఖాళీ);
        *interner->escaped[i].బహిర్గతం = ఖాళీ;
        ఒకవేళ (!జవాబు) {
            interner->had_error = true;
        } లేకపోతే {
            నిడివి = xcb_get_atom_name_name_నిడివి(జవాబు);
            name = xcb_get_atom_name_name(జవాబు);

            *బహిర్గతం = strndup(name, (కొలత_t)నిడివి);
            విడుదల(జవాబు);
            ఒకవేళ (*బహిర్గతం == ఖాళీ) {
                interner->had_error = true;
            } లేకపోతే {
                XkbEscapeMapName(*బహిర్గతం);
            }
        }
    }

    interner->అంకె_pending = 0;
    interner->అంకె_copies = 0;
    interner->అంకె_escaped = 0;
}

శూన్య
x11_atom_interner_get_escaped_atom_name(నిర్మాణం x11_atom_interner *interner,
                                        xcb_atom_t atom, అక్షరం **బహిర్గతం)
{
    ఒకవేళ (atom == 0) {
        *బహిర్గతం = ఖాళీ;
        తిరిగి;
    }
    కొలత_t idx = interner->అంకె_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    ప్రతిజ్ఞా(idx < ARRAY_కొలత(interner->escaped));
    interner->escaped[idx].బహిర్గతం = బహిర్గతం;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
