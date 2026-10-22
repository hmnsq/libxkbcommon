/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stddef.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "rmlvo.h"
#அந்தர்பாவ "utils.h"

கட்டமைப்பு xkb_context {
    முழு refcnt;

    ATTR_PRINTF(3, 0) வெறுமை (*log_fn)(கட்டமைப்பு xkb_context *ctx,
                                     எண்ணிக்கை xkb_log_level level,
                                     மாறிலி எழுத்து *fmt, va_list args);
    எண்ணிக்கை xkb_log_level log_level;
    முழு log_verbosity;
    வெறுமை *user_data;

    கட்டமைப்பு xkb_rule_names names_dflt;

    darray(எழுத்து *) includes;
    darray(எழுத்து *) failed_includes;

    கட்டமைப்பு atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    வெறுமை *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    எழுத்து text_buffer[2048];
    size_t text_next;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

எழுத்து *
xkb_context_getenv(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *name);

darray_size_t
xkb_context_இலக்கம்_failed_include_paths(கட்டமைப்பு xkb_context *ctx);

bool
xkb_context_init_includes(கட்டமைப்பு xkb_context *ctx);

மாறிலி எழுத்து *
xkb_context_failed_include_path_get(கட்டமைப்பு xkb_context *ctx,
                                    darray_size_t idx);

மாறிலி எழுத்து *
xkb_context_include_path_get_extra_path(கட்டமைப்பு xkb_context *ctx);

மாறிலி எழுத்து *
xkb_context_include_path_get_unversioned_extensions_path(கட்டமைப்பு xkb_context *ctx);

மாறிலி எழுத்து *
xkb_context_include_path_get_versioned_extensions_path(கட்டமைப்பு xkb_context *ctx);

மாறிலி எழுத்து *
xkb_context_include_path_get_system_path(கட்டமைப்பு xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_size_t
xkb_atom_table_size(கட்டமைப்பு xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string, size_t len);

#define xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), அளவு(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(கட்டமைப்பு xkb_context *ctx, எழுத்து *string);

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
xkb_atom_text(கட்டமைப்பு xkb_context *ctx, xkb_atom_t atom);

எழுத்து *
xkb_context_get_buffer(கட்டமைப்பு xkb_context *ctx, size_t size);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) வெறுமை
xkb_log(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_log_level level, முழு verbosity,
        மாறிலி எழுத்து *fmt, ...);

எண்ணிக்கை RMLVO
xkb_context_sanitize_rule_names(கட்டமைப்பு xkb_context *ctx,
                                கட்டமைப்பு xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define xkb_log_with_code(ctx, level, verbosity, செய்தி_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(செய்தி_id, fmt), ##__VA_ARGS__)
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
