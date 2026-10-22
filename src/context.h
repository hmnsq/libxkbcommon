/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stddef.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "rmlvo.h"
#অন্তর্ভাৱ "utils.h"

সংরচনা xkb_context {
    পূর্ণাঙ্ক refcnt;

    ATTR_PRINTF(3, 0) শূন্য (*log_fn)(সংরচনা xkb_context *ctx,
                                     গণনা xkb_log_level level,
                                     ধ্রুৱ ৱর্ণ *fmt, va_list args);
    গণনা xkb_log_level log_level;
    পূর্ণাঙ্ক log_verbosity;
    শূন্য *user_data;

    সংরচনা xkb_rule_names names_dflt;

    darray(ৱর্ণ *) includes;
    darray(ৱর্ণ *) failed_includes;

    সংরচনা atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    শূন্য *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    ৱর্ণ text_buffer[2048];
    size_t text_next;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

ৱর্ণ *
xkb_context_getenv(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *name);

darray_size_t
xkb_context_অঙ্ক_failed_include_paths(সংরচনা xkb_context *ctx);

bool
xkb_context_init_includes(সংরচনা xkb_context *ctx);

ধ্রুৱ ৱর্ণ *
xkb_context_failed_include_path_get(সংরচনা xkb_context *ctx,
                                    darray_size_t idx);

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_extra_path(সংরচনা xkb_context *ctx);

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_unversioned_extensions_path(সংরচনা xkb_context *ctx);

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_versioned_extensions_path(সংরচনা xkb_context *ctx);

ধ্রুৱ ৱর্ণ *
xkb_context_include_path_get_system_path(সংরচনা xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_size_t
xkb_atom_table_size(সংরচনা xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *string, size_t len);

#define xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), পরিমাণ(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(সংরচনা xkb_context *ctx, ৱর্ণ *string);

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
xkb_atom_text(সংরচনা xkb_context *ctx, xkb_atom_t atom);

ৱর্ণ *
xkb_context_get_buffer(সংরচনা xkb_context *ctx, size_t size);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) শূন্য
xkb_log(সংরচনা xkb_context *ctx, গণনা xkb_log_level level, পূর্ণাঙ্ক verbosity,
        ধ্রুৱ ৱর্ণ *fmt, ...);

গণনা RMLVO
xkb_context_sanitize_rule_names(সংরচনা xkb_context *ctx,
                                সংরচনা xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define xkb_log_with_code(ctx, level, verbosity, বাৰ্তা_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(বাৰ্তা_id, fmt), ##__VA_ARGS__)
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
