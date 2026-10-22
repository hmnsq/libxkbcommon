/*
 * Copyright © 2012 Intel Corporation
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>
#समावेश <stddef.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "atom.h"
#समावेश "darray.h"
#समावेश "messages-codes.h"
#समावेश "rmlvo.h"
#समावेश "utils.h"

काष्ठा xkb_context {
    पूर्णांक refcnt;

    ATTR_PRINTF(3, 0) व्योम (*log_fn)(काष्ठा xkb_context *ctx,
                                     क्रमागत xkb_log_level level,
                                     स्थिर अक्षर *fmt, बहु_सूची args);
    क्रमागत xkb_log_level log_level;
    पूर्णांक log_verbosity;
    व्योम *user_तथ्य;

    काष्ठा xkb_rule_names names_dflt;

    darray(अक्षर *) includes;
    darray(अक्षर *) failed_includes;

    काष्ठा atom_table *atom_table;

    /* Used and allocated by xkbcommon-x11, free()d with the context. */
    व्योम *x11_atom_cache;

    /* Buffer for the *Text() functions. */
    अक्षर text_बफर[2048];
    माप_प्रकार text_अग्र;

    bool use_environment_names : 1;
    bool use_secure_getenv : 1;
    bool pending_default_includes : 1;
};

अक्षर *
xkb_context_getenv(काष्ठा xkb_context *ctx, स्थिर अक्षर *name);

darray_आकार_t
xkb_context_अंक_failed_include_paths(काष्ठा xkb_context *ctx);

bool
xkb_context_init_includes(काष्ठा xkb_context *ctx);

स्थिर अक्षर *
xkb_context_failed_include_path_get(काष्ठा xkb_context *ctx,
                                    darray_आकार_t idx);

स्थिर अक्षर *
xkb_context_include_path_get_extra_path(काष्ठा xkb_context *ctx);

स्थिर अक्षर *
xkb_context_include_path_get_unversioned_extensions_path(काष्ठा xkb_context *ctx);

स्थिर अक्षर *
xkb_context_include_path_get_versioned_extensions_path(काष्ठा xkb_context *ctx);

स्थिर अक्षर *
xkb_context_include_path_get_system_path(काष्ठा xkb_context *ctx);

XKB_EXPORT_PRIVATE darray_आकार_t
xkb_atom_table_आकार(काष्ठा xkb_context *ctx);

/*
 * Returns XKB_ATOM_NONE if @string was not previously interned,
 * otherwise returns the atom.
 */
xkb_atom_t
xkb_atom_lookup(काष्ठा xkb_context *ctx, स्थिर अक्षर *string);

XKB_EXPORT_PRIVATE xkb_atom_t
xkb_atom_intern(काष्ठा xkb_context *ctx, स्थिर अक्षर *string, माप_प्रकार len);

#घोषणा xkb_atom_intern_literal(ctx, literal) \
    xkb_atom_intern((ctx), (literal), माप(literal) - 1)

/**
 * If @string is dynamically allocated, NUL-terminated, free'd immediately
 * after being interned, and not used afterwards, use this function
 * instead of xkb_atom_intern to avoid some unnecessary allocations.
 * The caller should not use or free the passed in string afterwards.
 */
xkb_atom_t
xkb_atom_steal(काष्ठा xkb_context *ctx, अक्षर *string);

XKB_EXPORT_PRIVATE स्थिर अक्षर *
xkb_atom_text(काष्ठा xkb_context *ctx, xkb_atom_t atom);

अक्षर *
xkb_context_get_बफर(काष्ठा xkb_context *ctx, माप_प्रकार आकार);

XKB_EXPORT_PRIVATE ATTR_PRINTF(4, 5) व्योम
xkb_log(काष्ठा xkb_context *ctx, क्रमागत xkb_log_level level, पूर्णांक verbosity,
        स्थिर अक्षर *fmt, ...);

क्रमागत RMLVO
xkb_context_sanitize_rule_names(काष्ठा xkb_context *ctx,
                                काष्ठा xkb_rule_names *rmlvo);

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#घोषणा xkb_log_with_code(ctx, level, verbosity, संदेश_id, fmt, ...) \
    xkb_log(ctx, level, verbosity, PREPEND_MESSAGE_ID(संदेश_id, fmt), ##__VA_ARGS__)
#घोषणा log_dbg(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_DEBUG, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#घोषणा log_info(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_INFO, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#घोषणा log_warn(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_WARNING, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#घोषणा log_vrb(ctx, vrb, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_WARNING, (vrb), id, __VA_ARGS__)
#घोषणा log_err(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_ERROR, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)
#घोषणा log_wsgo(ctx, id, ...) \
    xkb_log_with_code((ctx), XKB_LOG_LEVEL_CRITICAL, XKB_LOG_VERBOSITY_MINIMAL, id, __VA_ARGS__)

/*
 * Variants which are prefixed by the name of the function they're
 * called from.
 * Here we must have the silly 1 variant.
 */
#घोषणा log_warn_func(ctx, id, fmt, ...) \
    log_warn(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#घोषणा log_err_func(ctx, id, fmt, ...) \
    log_err(ctx, id, "%s: " fmt, __func__, __VA_ARGS__)
#घोषणा log_err_func1(ctx, id, fmt) \
    log_err(ctx, id, "%s: " fmt, __func__)
