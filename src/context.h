/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ರಚನೆ xkb_context {
    ಪೂರ್ಣಾಂಕ refcnt;

    ATTR_PRINTF(3, 0) ಶೂನ್ಯ (*log_fn)(ರಚನೆ xkb_context *ctx,
                                     ಎಣಿಕೆ xkb_log_level level,
                                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು);
    ಎಣಿಕೆ xkb_log_level log_level;
    ಪೂರ್ಣಾಂಕ log_verbosity;
    ಶೂನ್ಯ *user_ದತ್ತಾಂಶ;

    ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು ಹೆಸರುಗಳು_dflt;

    darray(ಅಕ್ಷರ *) includes;
    darray(ಅಕ್ಷರ *) failed_includes;

    ರಚನೆ atom_ಕೋಷ್ಟಕ *atom_ಕೋಷ್ಟಕ;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    ಶೂನ್ಯ *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    ಅಕ್ಷರ ಪಠ್ಯ_ಬಫರ್[2048];
    ಗಾತ್ರ_t ಪಠ್ಯ_ಮುಂದಿನ;

    bool use_environment_ಹೆಸರುಗಳು : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

ಅಕ್ಷರ *
xkb_context_getenv(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);

darray_ಗಾತ್ರ_t
xkb_context_ಅಂಕೆ_failed_include_paths(ರಚನೆ xkb_context *ctx);

bool
xkb_context_init_includes(ರಚನೆ xkb_context *ctx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_failed_include_ಮಾರ್ಗ_get(ರಚನೆ xkb_context *ctx,
                                    darray_ಗಾತ್ರ_t idx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_extra_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_unversioned_extensions_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_versioned_extensions_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_system_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx);

XKB_EXPORT_ಖಾಸಗಿ darray_ಗಾತ್ರ_t
xkb_atom_ಕೋಷ್ಟಕ_ಗಾತ್ರ(ರಚನೆ xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string);

XKB_EXPORT_ಖಾಸಗಿ xkb_atom_t
xkb_atom_intern(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len);

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), ಗಾತ್ರ(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(ರಚನೆ xkb_context *ctx, ಅಕ್ಷರ *string);

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_atom_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_atom_t atom);

ಅಕ್ಷರ *
xkb_context_get_ಬಫರ್(ರಚನೆ xkb_context *ctx, ಗಾತ್ರ_t ಗಾತ್ರ_2);

XKB_EXPORT_ಖಾಸಗಿ ATTR_PRINTF(4, 5) ಶೂನ್ಯ
xkb_log(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_log_level level, ಪೂರ್ಣಾಂಕ verbosity,
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...);

ಎಣಿಕೆ RMLVO
xkb_context_sanitize_ನಿಯಮ_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
                                ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು xkb_log_with_ಸಂಕೇತ(ctx, level, verbosity, ಸಂದೇಶ_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_ಸಂದೇಶ_ID(ಸಂದೇಶ_id, fmt), ##__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_dbg(ctx, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_info(ctx, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_warn(ctx, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_err(ctx, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_wsgo(ctx, id, ...) \
    xkb_log_with_ಸಂಕೇತ((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
