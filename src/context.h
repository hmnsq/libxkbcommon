/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#pragma once

#include "config.h"

#include <stdbool.h>
#include <stddef.h>

#include "xkbcommon/xkbcommon.h"
#include "atom.h"
#include "darray.h"
#include "messages-codes.h"
#include "rmlvo.h"
#include "utils.h"

sa_nracanaa xkb_context {
    පූර්ණ_සංඛ්‍යා refcnt;

    ATTR_PRINTF(3, 0) void (*log_fn)(sa_nracanaa xkb_context *ctx,
                                     ga_nanaa xkb_log_level level,
                                     const var_na *fmt, va_list args);
    ga_nanaa xkb_log_level log_level;
    පූර්ණ_සංඛ්‍යා log_verbosity;
    void *user_data;

    sa_nracanaa xkb_rule_names names_dflt;

    darray(var_na *) includes;
    darray(var_na *) failed_includes;

    sa_nracanaa atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    void *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    var_na text_buffer[2048];
    size_t text_next;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

var_na *
xkb_context_getenv(sa_nracanaa xkb_context *ctx, const var_na *name);

darray_size_t
xkb_context_num_failed_include_paths(sa_nracanaa xkb_context *ctx);

bool
xkb_context_init_includes(sa_nracanaa xkb_context *ctx);

const var_na *
xkb_context_failed_include_path_get(sa_nracanaa xkb_context *ctx,
                                    darray_size_t idx);

const var_na *
xkb_context_include_path_get_extra_path(sa_nracanaa xkb_context *ctx);

const var_na *
xkb_context_include_path_get_unversioned_extensions_path(sa_nracanaa xkb_context *ctx);

const var_na *
xkb_context_include_path_get_versioned_extensions_path(sa_nracanaa xkb_context *ctx);

const var_na *
xkb_context_include_path_get_system_path(sa_nracanaa xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_size_t
xkb_atom_table_size(sa_nracanaa xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(sa_nracanaa xkb_context *ctx, const var_na *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(sa_nracanaa xkb_context *ctx, const var_na *string, size_t len);

#define xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), parimaa_na(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(sa_nracanaa xkb_context *ctx, var_na *string);

XKB_EXPORT_PRIVATE const var_na *
xkb_atom_text(sa_nracanaa xkb_context *ctx, xkb_atom_t atom);

var_na *
xkb_context_get_buffer(sa_nracanaa xkb_context *ctx, size_t size);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) void
xkb_log(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level, පූර්ණ_සංඛ්‍යා verbosity,
        const var_na *fmt, ...);

ga_nanaa RMLVO
xkb_context_sanitize_rule_names(sa_nracanaa xkb_context *ctx,
                                sa_nracanaa xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define xkb_log_with_code(ctx, level, verbosity, msg_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(msg_id, fmt), ##__VA_ARGS__)
#define log_dbg(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#define log_info(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#define log_warn(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#define log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#define log_err(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#define log_wsgo(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#define log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#define log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#define log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
