/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "xkbcommon/xkbcommon.h"
#include "atom.h"
#include "darray.h"
#include "context.h"
#include "rmlvo.h"
#include "utils.h"

char *
xkb_context_getenv(struct xkb_context *ctx, const char *name)
{
    nếu (ctx->use_secure_getenv) {
        trả_về secure_getenv(name);
    } không_thì {
        trả_về getenv(name);
    }
}

bool
xkb_context_init_includes(struct xkb_context *ctx)
{
    nếu (ctx->pending_default_includes) {
        nếu (darray_empty(ctx->failed_includes)) {
            nếu (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                trả_về false;
            }
            ctx->pending_default_includes = false;
        } không_thì {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            trả_về false;
        }
    }
    trả_về true;
}

darray_size_t
xkb_context_num_failed_include_paths(struct xkb_context *ctx)
{
    trả_về (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

const char *
xkb_context_failed_include_path_get(struct xkb_context *ctx,
                                    darray_size_t idx)
{
    nếu (idx >= xkb_context_num_failed_include_paths(ctx))
        trả_về NULL;

    trả_về darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(struct xkb_context *ctx)
{
    trả_về atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(struct xkb_context *ctx, const char *string)
{
    trả_về atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(struct xkb_context *ctx, const char *string, size_t len)
{
    trả_về atom_intern(ctx->atom_table, string, len, true);
}

const char *
xkb_atom_text(struct xkb_context *ctx, xkb_atom_t atom)
{
    trả_về atom_text(ctx->atom_table, atom);
}

void
xkb_log(struct xkb_context *ctx, enum xkb_log_level level, số_nguyên verbosity,
        const char *fmt, ...)
{
    va_list args;

    nếu (ctx->log_level < level || ctx->log_verbosity < verbosity)
        trả_về;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

char *
xkb_context_get_buffer(struct xkb_context *ctx, size_t size)
{
    char *rtrn;

    nếu (size >= sizeof(ctx->text_buffer))
        trả_về NULL;

    nếu (sizeof(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    trả_về rtrn;
}

static const char *
xkb_context_get_default_rules(struct xkb_context *ctx)
{
    const char *env = NULL;

    nếu (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    trả_về env ? env : DEFAULT_XKB_RULES;
}

static const char *
xkb_context_get_default_model(struct xkb_context *ctx)
{
    const char *env = NULL;

    nếu (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    trả_về env ? env : DEFAULT_XKB_MODEL;
}

static const char *
xkb_context_get_default_layout(struct xkb_context *ctx)
{
    const char *env = NULL;

    nếu (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    trả_về env ? env : DEFAULT_XKB_LAYOUT;
}

static const char *
xkb_context_get_default_variant(struct xkb_context *ctx)
{
    const char *env = NULL;
    const char *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    nếu (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    trả_về env ? env : DEFAULT_XKB_VARIANT;
}

static const char *
xkb_context_get_default_options(struct xkb_context *ctx)
{
    const char *env = NULL;

    nếu (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    trả_về env ? env : DEFAULT_XKB_OPTIONS;
}

enum RMLVO
xkb_context_sanitize_rule_names(struct xkb_context *ctx,
                                struct xkb_rule_names *rmlvo)
{
    enum RMLVO modified = 0;

    nếu (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    nếu (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    nếu (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        const char * const variant = xkb_context_get_default_variant(ctx);
        nếu (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Layout not provided, but variant set to \"%s\": "
                     "ignoring variant and using defaults for both: "
                     "layout=\"%s\", variant=\"%s\".\n",
                     rmlvo->variant,
                     rmlvo->layout,
                     variant ? variant : "");
        }
        rmlvo->variant = variant;
        modified |= RMLVO_VARIANT;
    }
    /* Options can be empty, so respect that if passed in. */
    nếu (rmlvo->options == NULL) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    trả_về modified;
}
