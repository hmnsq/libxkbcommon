/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "messages-codes.h"
#ضمن "rmlvo.h"
#ضمن "utils.h"

بنية xkb_context {
    صحيح refcnt;

    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية xkb_context *ctx,
                                     تعداد xkb_log_level level,
                                     ثابت حرف *fmt, va_list دلیلاں);
    تعداد xkb_log_level log_level;
    صحيح log_verbosity;
    فراغ *user_انکڑے;

    بنية xkb_قاعدہ_ناں ناں_dflt;

    darray(حرف *) includes;
    darray(حرف *) failed_includes;

    بنية atom_جدول *atom_جدول;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    فراغ *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    حرف متن_بفر[2048];
    ناپ_t متن_اگلا;

    bool use_environment_ناں : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

حرف *
xkb_context_getenv(بنية xkb_context *ctx, ثابت حرف *name);

darray_ناپ_t
xkb_context_عدد_failed_include_paths(بنية xkb_context *ctx);

bool
xkb_context_init_includes(بنية xkb_context *ctx);

ثابت حرف *
xkb_context_failed_include_رستہ_get(بنية xkb_context *ctx,
                                    darray_ناپ_t idx);

ثابت حرف *
xkb_context_include_رستہ_get_extra_رستہ(بنية xkb_context *ctx);

ثابت حرف *
xkb_context_include_رستہ_get_unversioned_extensions_رستہ(بنية xkb_context *ctx);

ثابت حرف *
xkb_context_include_رستہ_get_versioned_extensions_رستہ(بنية xkb_context *ctx);

ثابت حرف *
xkb_context_include_رستہ_get_system_رستہ(بنية xkb_context *ctx);

XKB_EXPORT_نجی darray_ناپ_t
xkb_atom_جدول_ناپ(بنية xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(بنية xkb_context *ctx, ثابت حرف *string);

XKB_EXPORT_نجی xkb_atom_t
xkb_atom_intern(بنية xkb_context *ctx, ثابت حرف *string, ناپ_t len);

#تعریف_کرو xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), حجم(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(بنية xkb_context *ctx, حرف *string);

XKB_EXPORT_نجی ثابت حرف *
xkb_atom_متن(بنية xkb_context *ctx, xkb_atom_t atom);

حرف *
xkb_context_get_بفر(بنية xkb_context *ctx, ناپ_t ناپ);

XKB_EXPORT_نجی ATTR_PRINTF(4, 5) فراغ
xkb_log(بنية xkb_context *ctx, تعداد xkb_log_level level, صحيح verbosity,
        ثابت حرف *fmt, ...);

تعداد RMLVO
xkb_context_sanitize_قاعدہ_ناں(بنية xkb_context *ctx,
                                بنية xkb_قاعدہ_ناں *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#تعریف_کرو xkb_log_with_کوڈ(ctx, level, verbosity, پیغام_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_سنیہا_ID(پیغام_id, fmt), ##__VA_ARGS__)
#تعریف_کرو log_dbg(ctx, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_اقل, id, __VA_ARGS__)
#تعریف_کرو log_info(ctx, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_اقل, id, __VA_ARGS__)
#تعریف_کرو log_warn(ctx, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_اقل, id, __VA_ARGS__)
#تعریف_کرو log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#تعریف_کرو log_err(ctx, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_اقل, id, __VA_ARGS__)
#تعریف_کرو log_wsgo(ctx, id, ...) \
    xkb_log_with_کوڈ((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_اقل, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#تعریف_کرو log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#تعریف_کرو log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#تعریف_کرو log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
