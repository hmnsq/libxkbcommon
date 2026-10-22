/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "rmlvo.h"
#અન્તર્ભાવ "utils.h"

સંરચના xkb_context {
    પૂર્ણાંક refcnt;

    ATTR_PRINTF(3, 0) શૂન્ય (*log_fn)(સંરચના xkb_context *ctx,
                                     ગણના xkb_log_level level,
                                     અચલ અક્ષર *fmt, va_list args);
    ગણના xkb_log_level log_level;
    પૂર્ણાંક log_verbosity;
    શૂન્ય *user_data;

    સંરચના xkb_rule_names names_dflt;

    darray(અક્ષર *) includes;
    darray(અક્ષર *) failed_includes;

    સંરચના atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    શૂન્ય *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    અક્ષર text_buffer[2048];
    size_t text_next;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

અક્ષર *
xkb_context_getenv(સંરચના xkb_context *ctx, અચલ અક્ષર *name);

darray_size_t
xkb_context_અંક_failed_include_paths(સંરચના xkb_context *ctx);

bool
xkb_context_init_includes(સંરચના xkb_context *ctx);

અચલ અક્ષર *
xkb_context_failed_include_path_get(સંરચના xkb_context *ctx,
                                    darray_size_t idx);

અચલ અક્ષર *
xkb_context_include_path_get_extra_path(સંરચના xkb_context *ctx);

અચલ અક્ષર *
xkb_context_include_path_get_unversioned_extensions_path(સંરચના xkb_context *ctx);

અચલ અક્ષર *
xkb_context_include_path_get_versioned_extensions_path(સંરચના xkb_context *ctx);

અચલ અક્ષર *
xkb_context_include_path_get_system_path(સંરચના xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_size_t
xkb_atom_table_size(સંરચના xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(સંરચના xkb_context *ctx, અચલ અક્ષર *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(સંરચના xkb_context *ctx, અચલ અક્ષર *string, size_t len);

#define xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), કદ(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(સંરચના xkb_context *ctx, અક્ષર *string);

XKB_EXPORT_PRIVATE અચલ અક્ષર *
xkb_atom_text(સંરચના xkb_context *ctx, xkb_atom_t atom);

અક્ષર *
xkb_context_get_buffer(સંરચના xkb_context *ctx, size_t size);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) શૂન્ય
xkb_log(સંરચના xkb_context *ctx, ગણના xkb_log_level level, પૂર્ણાંક verbosity,
        અચલ અક્ષર *fmt, ...);

ગણના RMLVO
xkb_context_sanitize_rule_names(સંરચના xkb_context *ctx,
                                સંરચના xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define xkb_log_with_code(ctx, level, verbosity, સંદેશ_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(સંદેશ_id, fmt), ##__VA_ARGS__)
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
