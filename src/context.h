/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "rmlvo.h"
#अन्तर्भाव "utils.h"

संरचना xkb_context {
    पूर्णाङ्क refcnt;

    ATTR_PRINTF(3, 0) शून्य (*log_fn)(संरचना xkb_context *ctx,
                                     गणना xkb_log_level level,
                                     ध्रुव वर्ण *fmt, va_list तर्काः);
    गणना xkb_log_level log_level;
    पूर्णाङ्क log_verbosity;
    शून्य *user_दत्त;

    संरचना xkb_नियमः_नामानि नामानि_dflt;

    darray(वर्ण *) includes;
    darray(वर्ण *) failed_includes;

    संरचना atom_सारणी *atom_सारणी;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    शून्य *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    वर्ण text_बफर[2048];
    आकार_t text_अग्र;

    bool use_environment_नामानि : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

वर्ण *
xkb_context_getenv(संरचना xkb_context *ctx, ध्रुव वर्ण *name);

darray_आकार_t
xkb_context_अङ्कः_failed_include_paths(संरचना xkb_context *ctx);

bool
xkb_context_init_includes(संरचना xkb_context *ctx);

ध्रुव वर्ण *
xkb_context_failed_include_मार्गः_get(संरचना xkb_context *ctx,
                                    darray_आकार_t idx);

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_extra_मार्गः(संरचना xkb_context *ctx);

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_unversioned_extensions_मार्गः(संरचना xkb_context *ctx);

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_versioned_extensions_मार्गः(संरचना xkb_context *ctx);

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_system_मार्गः(संरचना xkb_context *ctx);

XKB_EXPORT_वैयक्तिकम् darray_आकार_t
xkb_atom_सारणी_आकार(संरचना xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(संरचना xkb_context *ctx, ध्रुव वर्ण *string);

XKB_EXPORT_वैयक्तिकम् xkb_atom_t
xkb_atom_intern(संरचना xkb_context *ctx, ध्रुव वर्ण *string, आकार_t len);

#परिभाषय xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), परिमाण(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(संरचना xkb_context *ctx, वर्ण *string);

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
xkb_atom_text(संरचना xkb_context *ctx, xkb_atom_t atom);

वर्ण *
xkb_context_get_बफर(संरचना xkb_context *ctx, आकार_t आकार);

XKB_EXPORT_वैयक्तिकम् ATTR_PRINTF(4, 5) शून्य
xkb_log(संरचना xkb_context *ctx, गणना xkb_log_level level, पूर्णाङ्क verbosity,
        ध्रुव वर्ण *fmt, ...);

गणना RMLVO
xkb_context_sanitize_नियमः_नामानि(संरचना xkb_context *ctx,
                                संरचना xkb_नियमः_नामानि *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#परिभाषय xkb_log_with_संहिता(ctx, level, verbosity, सन्देशः_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_सन्देशः_ID(सन्देशः_id, fmt), ##__VA_ARGS__)
#परिभाषय log_dbg(ctx, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, __VA_ARGS__)
#परिभाषय log_info(ctx, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, __VA_ARGS__)
#परिभाषय log_warn(ctx, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, __VA_ARGS__)
#परिभाषय log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#परिभाषय log_err(ctx, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, __VA_ARGS__)
#परिभाषय log_wsgo(ctx, id, ...) \
    xkb_log_with_संहिता((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#परिभाषय log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#परिभाषय log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#परिभाषय log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
