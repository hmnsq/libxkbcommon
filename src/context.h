/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਬਣਤਰ xkb_context {
    ਪੂਰਨ refcnt;

    ATTR_PRINTF(3, 0) ਖਾਲੀ (*log_fn)(ਬਣਤਰ xkb_context *ctx,
                                     ਗਿਣਤੀ xkb_log_level level,
                                     ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ);
    ਗਿਣਤੀ xkb_log_level log_level;
    ਪੂਰਨ log_verbosity;
    ਖਾਲੀ *user_ਅੰਕੜੇ;

    ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ ਨਾਂ_dflt;

    darray(ਅੱਖਰ *) includes;
    darray(ਅੱਖਰ *) failed_includes;

    ਬਣਤਰ atom_ਸਾਰਣੀ *atom_ਸਾਰਣੀ;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    ਖਾਲੀ *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    ਅੱਖਰ ਪਾਠ_ਬਫਰ[2048];
    ਮਾਪ_t ਪਾਠ_ਅਗਲਾ;

    bool use_environment_ਨਾਂ : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

ਅੱਖਰ *
xkb_context_getenv(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *name);

darray_ਮਾਪ_t
xkb_context_ਅੰਕ_failed_include_paths(ਬਣਤਰ xkb_context *ctx);

bool
xkb_context_init_includes(ਬਣਤਰ xkb_context *ctx);

ਅਚਲ ਅੱਖਰ *
xkb_context_failed_include_ਰਸਤਾ_get(ਬਣਤਰ xkb_context *ctx,
                                    darray_ਮਾਪ_t idx);

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_extra_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_unversioned_extensions_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_versioned_extensions_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_system_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx);

XKB_EXPORT_ਨਿੱਜੀ darray_ਮਾਪ_t
xkb_atom_ਸਾਰਣੀ_ਮਾਪ(ਬਣਤਰ xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string);

XKB_EXPORT_ਨਿੱਜੀ xkb_atom_t
xkb_atom_intern(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), ਆਕਾਰ(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(ਬਣਤਰ xkb_context *ctx, ਅੱਖਰ *string);

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
xkb_atom_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_atom_t atom);

ਅੱਖਰ *
xkb_context_get_ਬਫਰ(ਬਣਤਰ xkb_context *ctx, ਮਾਪ_t ਮਾਪ);

XKB_EXPORT_ਨਿੱਜੀ ATTR_PRINTF(4, 5) ਖਾਲੀ
xkb_log(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_log_level level, ਪੂਰਨ verbosity,
        ਅਚਲ ਅੱਖਰ *fmt, ...);

ਗਿਣਤੀ RMLVO
xkb_context_sanitize_ਨਿਯਮ_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
                                ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_log_with_ਕੋਡ(ctx, level, verbosity, ਸੁਨੇਹਾ_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_ਸੁਨੇਹਾ_ID(ਸੁਨੇਹਾ_id, fmt), ##__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_dbg(ctx, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_info(ctx, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_warn(ctx, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_err(ctx, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_wsgo(ctx, id, ...) \
    xkb_log_with_ਕੋਡ((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
