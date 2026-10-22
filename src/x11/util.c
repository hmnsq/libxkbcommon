/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include "x11-priv.h"

ចំនួនគត់
xkb_x11_setup_xkb_extension(xcb_connection_t *conn,
                            uint16_t major_xkb_version,
                            uint16_t minor_xkb_version,
                            ga_nanaa xkb_x11_setup_xkb_extension_flags flags,
                            uint16_t *major_xkb_version_out,
                            uint16_t *minor_xkb_version_out,
                            uint8_t *base_event_out,
                            uint8_t *base_error_out)
{
    uint8_t base_event, base_error;
    uint16_t server_major, server_minor;

    static const ga_nanaa xkb_x11_setup_xkb_extension_flags
        XKB_X11_SETUP_XKB_EXTENSION_FLAGS = XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS;

    ប្រសិនបើ (flags & ~XKB_X11_SETUP_XKB_EXTENSION_FLAGS) {
        /* log_err_func(ctx, "unrecognized flags: %#x\n", flags); */
        ត្រឡប់ 0;
    }

    {
        const xcb_query_extension_reply_t *reply =
            xcb_get_extension_data(conn, &xcb_xkb_id);
        ប្រសិនបើ (!reply) {
            /* log_err_func(ctx, "failed to query for XKB extension\n"); */
            ត្រឡប់ 0;
        }

        ប្រសិនបើ (!reply->present) {
            /* log_err_func(ctx, "failed to start using XKB extension: not available in server\n"); */
            ត្រឡប់ 0;
        }

        base_event = reply->first_event;
        base_error = reply->first_error;
    }

    {
        xcb_generic_error_t *error = NULL;
        xcb_xkb_use_extension_cookie_t cookie =
            xcb_xkb_use_extension(conn, major_xkb_version, minor_xkb_version);
        xcb_xkb_use_extension_reply_t *reply =
            xcb_xkb_use_extension_reply(conn, cookie, &error);

        ប្រសិនបើ (!reply) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: error code %d\n", */
            /*              error ? error->error_code : -1); */
            free(error);
            ត្រឡប់ 0;
        }

        ប្រសិនបើ (!reply->supported) {
            /* log_err_func(ctx, */
            /*              "failed to start using XKB extension: server doesn't support version %d.%d\n", */
            /*              major_xkb_version, minor_xkb_version); */
            free(reply);
            ត្រឡប់ 0;
        }

        server_major = reply->serverMajor;
        server_minor = reply->serverMinor;

        free(reply);
    }

    /*
    * The XkbUseExtension() in libX11 has a *bunch* of legacy stuff, but
    * it doesn't seem like any of it is useful to us.
    */

    ប្រសិនបើ (major_xkb_version_out)
        *major_xkb_version_out = server_major;
    ប្រសិនបើ (minor_xkb_version_out)
        *minor_xkb_version_out = server_minor;
    ប្រសិនបើ (base_event_out)
        *base_event_out = base_event;
    ប្រសិនបើ (base_error_out)
        *base_error_out = base_error;

    ត្រឡប់ 1;
}

int32_t
xkb_x11_get_core_keyboard_device_id(xcb_connection_t *conn)
{
    int32_t device_id;
    xcb_xkb_get_device_info_cookie_t cookie =
        xcb_xkb_get_device_info(conn, XCB_XKB_ID_USE_CORE_KBD,
                                0, 0, 0, 0, 0, 0);
    xcb_xkb_get_device_info_reply_t *reply =
        xcb_xkb_get_device_info_reply(conn, cookie, NULL);

    ប្រសិនបើ (!reply)
        ត្រឡប់ -1;

    device_id = reply->deviceID;
    free(reply);
    ត្រឡប់ device_id;
}

sa_nracanaa x11_atom_cache {
    /*
     * Invalidate the cache based on the XCB connection.
     * X11 atoms are actually not per connection or client, but per X server
     * session. But better be safe just in case we survive an X server restart.
     */
    xcb_connection_t *conn;
    sa_nracanaa {
        xcb_atom_t from;
        xkb_atom_t to;
    } cache[256];
    size_t len;
};

static sa_nracanaa x11_atom_cache *
get_cache(sa_nracanaa xkb_context *ctx, xcb_connection_t *conn)
{
    ប្រសិនបើ (!ctx->x11_atom_cache) {
        ctx->x11_atom_cache = calloc(1, parimaa_na(sa_nracanaa x11_atom_cache));
    }
    /* Can be NULL in case the malloc failed. */
    sa_nracanaa x11_atom_cache *cache = ctx->x11_atom_cache;
    ប្រសិនបើ (cache && cache->conn != conn) {
        cache->conn = conn;
        cache->len = 0;
    }
    ត្រឡប់ cache;
}

void
x11_atom_interner_init(sa_nracanaa x11_atom_interner *interner,
                       sa_nracanaa xkb_context *ctx, xcb_connection_t *conn)
{
    interner->had_error = false;
    interner->ctx = ctx;
    interner->conn = conn;
    interner->num_pending = 0;
    interner->num_copies = 0;
    interner->num_escaped = 0;
}

void
x11_atom_interner_adopt_atom(sa_nracanaa x11_atom_interner *interner,
                             const xcb_atom_t atom, xkb_atom_t *out)
{
    *out = XKB_ATOM_NONE;

    ប្រសិនបើ (atom == XCB_ATOM_NONE)
        ត្រឡប់;

    /* Can be NULL in case the malloc failed. */
    sa_nracanaa x11_atom_cache *cache = get_cache(interner->ctx, interner->conn);

retry:

    /* Already in the cache? */
    ប្រសិនបើ (cache) {
        សម្រាប់ (size_t c = 0; c < cache->len; c++) {
            ប្រសិនបើ (cache->cache[c].from == atom) {
                *out = cache->cache[c].to;
                ត្រឡប់;
            }
        }
    }

    /* Already pending? */
    សម្រាប់ (size_t i = 0; i < interner->num_pending; i++) {
        ប្រសិនបើ (interner->pending[i].from == atom) {
            ប្រសិនបើ (interner->num_copies == ARRAY_SIZE(interner->copies)) {
                x11_atom_interner_round_trip(interner);
                goto retry;
            }

            size_t idx = interner->num_copies++;
            interner->copies[idx].from = atom;
            interner->copies[idx].out = out;
            ត្រឡប់;
        }
    }

    /* We have to send a GetAtomName request */
    ប្រសិនបើ (interner->num_pending == ARRAY_SIZE(interner->pending)) {
        x11_atom_interner_round_trip(interner);
        assert(interner->num_pending < ARRAY_SIZE(interner->pending));
    }
    size_t idx = interner->num_pending++;
    interner->pending[idx].from = atom;
    interner->pending[idx].out = out;
    interner->pending[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}

void
x11_atom_interner_round_trip(sa_nracanaa x11_atom_interner *interner) {
    sa_nracanaa xkb_context *ctx = interner->ctx;
    xcb_connection_t *conn = interner->conn;

    /* Can be NULL in case the malloc failed. */
    sa_nracanaa x11_atom_cache *cache = get_cache(ctx, conn);

    សម្រាប់ (size_t i = 0; i < interner->num_pending; i++) {
        xcb_get_atom_name_reply_t *reply;

        reply = xcb_get_atom_name_reply(conn, interner->pending[i].cookie, NULL);
        ប្រសិនបើ (!reply) {
            interner->had_error = true;
            continue;
        }
        xcb_atom_t x11_atom = interner->pending[i].from;
        xkb_atom_t atom = xkb_atom_intern(ctx,
                                          xcb_get_atom_name_name(reply),
                                          xcb_get_atom_name_name_length(reply));
        free(reply);

        ប្រសិនបើ (cache && cache->len < ARRAY_SIZE(cache->cache)) {
            size_t idx = cache->len++;
            cache->cache[idx].from = x11_atom;
            cache->cache[idx].to = atom;
        }

        *interner->pending[i].out = atom;

        សម្រាប់ (size_t j = 0; j < interner->num_copies; j++) {
            ប្រសិនបើ (interner->copies[j].from == x11_atom)
                *interner->copies[j].out = atom;
        }
    }

    សម្រាប់ (size_t i = 0; i < interner->num_escaped; i++) {
        xcb_get_atom_name_reply_t *reply;
        ចំនួនគត់ length;
        var_na *name;
        var_na **out = interner->escaped[i].out;

        reply = xcb_get_atom_name_reply(conn, interner->escaped[i].cookie, NULL);
        *interner->escaped[i].out = NULL;
        ប្រសិនបើ (!reply) {
            interner->had_error = true;
        } បើមិនដូច្នេះ {
            length = xcb_get_atom_name_name_length(reply);
            name = xcb_get_atom_name_name(reply);

            *out = strndup(name, (size_t)length);
            free(reply);
            ប្រសិនបើ (*out == NULL) {
                interner->had_error = true;
            } បើមិនដូច្នេះ {
                XkbEscapeMapName(*out);
            }
        }
    }

    interner->num_pending = 0;
    interner->num_copies = 0;
    interner->num_escaped = 0;
}

void
x11_atom_interner_get_escaped_atom_name(sa_nracanaa x11_atom_interner *interner,
                                        xcb_atom_t atom, var_na **out)
{
    ប្រសិនបើ (atom == 0) {
        *out = NULL;
        ត្រឡប់;
    }
    size_t idx = interner->num_escaped++;
    /* There can only be a fixed number of calls to this function "in-flight",
     * thus we assert this number. Increase the array size if this assert fails.
     */
    assert(idx < ARRAY_SIZE(interner->escaped));
    interner->escaped[idx].out = out;
    interner->escaped[idx].cookie = xcb_get_atom_name(interner->conn, atom);
}
