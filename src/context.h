/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "rmlvo.h"
#അന്തര്ഭാവ "utils.h"

ഘടന xkb_context {
    പൂർണ്ണസംഖ്യ refcnt;

    ATTR_PRINTF(3, 0) ശൂന്യം (*log_fn)(ഘടന xkb_context *ctx,
                                     എണ്ണൽ xkb_log_level level,
                                     സ്ഥിരം അക്ഷരം *fmt, va_list args);
    എണ്ണൽ xkb_log_level log_level;
    പൂർണ്ണസംഖ്യ log_verbosity;
    ശൂന്യം *user_data;

    ഘടന xkb_rule_names names_dflt;

    darray(അക്ഷരം *) includes;
    darray(അക്ഷരം *) failed_includes;

    ഘടന atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    ശൂന്യം *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    അക്ഷരം text_buffer[2048];
    size_t text_next;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

അക്ഷരം *
xkb_context_getenv(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *name);

darray_size_t
xkb_context_അക്കം_failed_include_paths(ഘടന xkb_context *ctx);

bool
xkb_context_init_includes(ഘടന xkb_context *ctx);

സ്ഥിരം അക്ഷരം *
xkb_context_failed_include_path_get(ഘടന xkb_context *ctx,
                                    darray_size_t idx);

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_extra_path(ഘടന xkb_context *ctx);

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_unversioned_extensions_path(ഘടന xkb_context *ctx);

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_versioned_extensions_path(ഘടന xkb_context *ctx);

സ്ഥിരം അക്ഷരം *
xkb_context_include_path_get_system_path(ഘടന xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_size_t
xkb_atom_table_size(ഘടന xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string, size_t len);

#define xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), വലുപ്പം(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(ഘടന xkb_context *ctx, അക്ഷരം *string);

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
xkb_atom_text(ഘടന xkb_context *ctx, xkb_atom_t atom);

അക്ഷരം *
xkb_context_get_buffer(ഘടന xkb_context *ctx, size_t size);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) ശൂന്യം
xkb_log(ഘടന xkb_context *ctx, എണ്ണൽ xkb_log_level level, പൂർണ്ണസംഖ്യ verbosity,
        സ്ഥിരം അക്ഷരം *fmt, ...);

എണ്ണൽ RMLVO
xkb_context_sanitize_rule_names(ഘടന xkb_context *ctx,
                                ഘടന xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define xkb_log_with_code(ctx, level, verbosity, സന്ദേശം_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(സന്ദേശം_id, fmt), ##__VA_ARGS__)
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
