/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "rmlvo.h"
#అన్తర్భావ "utils.h"

నిర్మాణం xkb_context {
    పూర్ణాంకం refcnt;

    ATTR_PRINTF(3, 0) శూన్య (*log_fn)(నిర్మాణం xkb_context *ctx,
                                     గణన xkb_log_level level,
                                     స్థిరరాశి అక్షరం *fmt, va_list వాదనలు);
    గణన xkb_log_level log_level;
    పూర్ణాంకం log_verbosity;
    శూన్య *user_దత్తాంశం;

    నిర్మాణం xkb_నియమం_పేర్లు పేర్లు_dflt;

    darray(అక్షరం *) includes;
    darray(అక్షరం *) failed_includes;

    నిర్మాణం atom_పట్టిక *atom_పట్టిక;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    శూన్య *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    అక్షరం పాఠ్యం_బఫరు[2048];
    కొలత_t పాఠ్యం_తదుపరి;

    bool use_environment_పేర్లు : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

అక్షరం *
xkb_context_getenv(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *name);

darray_కొలత_t
xkb_context_అంకె_failed_include_paths(నిర్మాణం xkb_context *ctx);

bool
xkb_context_init_includes(నిర్మాణం xkb_context *ctx);

స్థిరరాశి అక్షరం *
xkb_context_failed_include_మార్గం_get(నిర్మాణం xkb_context *ctx,
                                    darray_కొలత_t idx);

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_extra_మార్గం(నిర్మాణం xkb_context *ctx);

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_unversioned_extensions_మార్గం(నిర్మాణం xkb_context *ctx);

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_versioned_extensions_మార్గం(నిర్మాణం xkb_context *ctx);

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_system_మార్గం(నిర్మాణం xkb_context *ctx);

XKB_EXPORT_వ్యక్తిగత darray_కొలత_t
xkb_atom_పట్టిక_కొలత(నిర్మాణం xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string);

XKB_EXPORT_వ్యక్తిగత xkb_atom_t
xkb_atom_intern(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string, కొలత_t len);

#నిర్వచించు xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), పరిమాణం(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(నిర్మాణం xkb_context *ctx, అక్షరం *string);

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
xkb_atom_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_atom_t atom);

అక్షరం *
xkb_context_get_బఫరు(నిర్మాణం xkb_context *ctx, కొలత_t కొలత);

XKB_EXPORT_వ్యక్తిగత ATTR_PRINTF(4, 5) శూన్య
xkb_log(నిర్మాణం xkb_context *ctx, గణన xkb_log_level level, పూర్ణాంకం verbosity,
        స్థిరరాశి అక్షరం *fmt, ...);

గణన RMLVO
xkb_context_sanitize_నియమం_పేర్లు(నిర్మాణం xkb_context *ctx,
                                నిర్మాణం xkb_నియమం_పేర్లు *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#నిర్వచించు xkb_log_with_సంకేతం(ctx, level, verbosity, సందేశం_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_సందేశం_ID(సందేశం_id, fmt), ##__VA_ARGS__)
#నిర్వచించు log_dbg(ctx, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_కనీస, id, __VA_ARGS__)
#నిర్వచించు log_info(ctx, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_కనీస, id, __VA_ARGS__)
#నిర్వచించు log_warn(ctx, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_కనీస, id, __VA_ARGS__)
#నిర్వచించు log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#నిర్వచించు log_err(ctx, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_కనీస, id, __VA_ARGS__)
#నిర్వచించు log_wsgo(ctx, id, ...) \
    xkb_log_with_సంకేతం((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_కనీస, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#నిర్వచించు log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#నిర్వచించు log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#నిర్వచించు log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
