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

var_na *
xkb_context_getenv(sa_nracanaa xkb_context *ctx, const var_na *name)
{
    ᰀᰧ (ctx->use_secure_getenv) {
        ᰀᰧᰌ secure_getenv(name);
    } ᰀᰧᰀ {
        ᰀᰧᰌ getenv(name);
    }
}

bool
xkb_context_init_includes(sa_nracanaa xkb_context *ctx)
{
    ᰀᰧ (ctx->pending_default_includes) {
        ᰀᰧ (darray_empty(ctx->failed_includes)) {
            ᰀᰧ (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ᰀᰧᰌ false;
            }
            ctx->pending_default_includes = false;
        } ᰀᰧᰀ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ᰀᰧᰌ false;
        }
    }
    ᰀᰧᰌ true;
}

darray_size_t
xkb_context_num_failed_include_paths(sa_nracanaa xkb_context *ctx)
{
    ᰀᰧᰌ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

const var_na *
xkb_context_failed_include_path_get(sa_nracanaa xkb_context *ctx,
                                    darray_size_t idx)
{
    ᰀᰧ (idx >= xkb_context_num_failed_include_paths(ctx))
        ᰀᰧᰌ NULL;

    ᰀᰧᰌ darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(sa_nracanaa xkb_context *ctx)
{
    ᰀᰧᰌ atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(sa_nracanaa xkb_context *ctx, const var_na *string)
{
    ᰀᰧᰌ atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(sa_nracanaa xkb_context *ctx, const var_na *string, size_t len)
{
    ᰀᰧᰌ atom_intern(ctx->atom_table, string, len, true);
}

const var_na *
xkb_atom_text(sa_nracanaa xkb_context *ctx, xkb_atom_t atom)
{
    ᰀᰧᰌ atom_text(ctx->atom_table, atom);
}

void
xkb_log(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level, puur_naa_nka verbosity,
        const var_na *fmt, ...)
{
    va_list args;

    ᰀᰧ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ᰀᰧᰌ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

var_na *
xkb_context_get_buffer(sa_nracanaa xkb_context *ctx, size_t size)
{
    var_na *rtrn;

    ᰀᰧ (size >= parimaa_na(ctx->text_buffer))
        ᰀᰧᰌ NULL;

    ᰀᰧ (parimaa_na(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    ᰀᰧᰌ rtrn;
}

static const var_na *
xkb_context_get_default_rules(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    ᰀᰧ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ᰀᰧᰌ env ? env : DEFAULT_XKB_RULES;
}

static const var_na *
xkb_context_get_default_model(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    ᰀᰧ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ᰀᰧᰌ env ? env : DEFAULT_XKB_MODEL;
}

static const var_na *
xkb_context_get_default_layout(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    ᰀᰧ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ᰀᰧᰌ env ? env : DEFAULT_XKB_LAYOUT;
}

static const var_na *
xkb_context_get_default_variant(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;
    const var_na *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ᰀᰧ (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ᰀᰧᰌ env ? env : DEFAULT_XKB_VARIANT;
}

static const var_na *
xkb_context_get_default_options(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    ᰀᰧ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ᰀᰧᰌ env ? env : DEFAULT_XKB_OPTIONS;
}

ga_nanaa RMLVO
xkb_context_sanitize_rule_names(sa_nracanaa xkb_context *ctx,
                                sa_nracanaa xkb_rule_names *rmlvo)
{
    ga_nanaa RMLVO modified = 0;

    ᰀᰧ (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    ᰀᰧ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ᰀᰧ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        const var_na * const variant = xkb_context_get_default_variant(ctx);
        ᰀᰧ (!isempty(rmlvo->variant)) {
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
    ᰀᰧ (rmlvo->options == NULL) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ᰀᰧᰌ modified;
}
