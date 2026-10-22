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
    නම් (ctx->use_secure_getenv) {
        ආපසු secure_getenv(name);
    } නැතහොත් {
        ආපසු getenv(name);
    }
}

bool
xkb_context_init_includes(sa_nracanaa xkb_context *ctx)
{
    නම් (ctx->pending_default_includes) {
        නම් (darray_empty(ctx->failed_includes)) {
            නම් (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ආපසු false;
            }
            ctx->pending_default_includes = false;
        } නැතහොත් {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ආපසු false;
        }
    }
    ආපසු true;
}

darray_size_t
xkb_context_num_failed_include_paths(sa_nracanaa xkb_context *ctx)
{
    ආපසු (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

const var_na *
xkb_context_failed_include_path_get(sa_nracanaa xkb_context *ctx,
                                    darray_size_t idx)
{
    නම් (idx >= xkb_context_num_failed_include_paths(ctx))
        ආපසු NULL;

    ආපසු darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(sa_nracanaa xkb_context *ctx)
{
    ආපසු atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(sa_nracanaa xkb_context *ctx, const var_na *string)
{
    ආපසු atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(sa_nracanaa xkb_context *ctx, const var_na *string, size_t len)
{
    ආපසු atom_intern(ctx->atom_table, string, len, true);
}

const var_na *
xkb_atom_text(sa_nracanaa xkb_context *ctx, xkb_atom_t atom)
{
    ආපසු atom_text(ctx->atom_table, atom);
}

void
xkb_log(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_log_level level, පූර්ණ_සංඛ්‍යා verbosity,
        const var_na *fmt, ...)
{
    va_list args;

    නම් (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ආපසු;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

var_na *
xkb_context_get_buffer(sa_nracanaa xkb_context *ctx, size_t size)
{
    var_na *rtrn;

    නම් (size >= parimaa_na(ctx->text_buffer))
        ආපසු NULL;

    නම් (parimaa_na(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    ආපසු rtrn;
}

static const var_na *
xkb_context_get_default_rules(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    නම් (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ආපසු env ? env : DEFAULT_XKB_RULES;
}

static const var_na *
xkb_context_get_default_model(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    නම් (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ආපසු env ? env : DEFAULT_XKB_MODEL;
}

static const var_na *
xkb_context_get_default_layout(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    නම් (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ආපසු env ? env : DEFAULT_XKB_LAYOUT;
}

static const var_na *
xkb_context_get_default_variant(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;
    const var_na *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    නම් (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ආපසු env ? env : DEFAULT_XKB_VARIANT;
}

static const var_na *
xkb_context_get_default_options(sa_nracanaa xkb_context *ctx)
{
    const var_na *env = NULL;

    නම් (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ආපසු env ? env : DEFAULT_XKB_OPTIONS;
}

ga_nanaa RMLVO
xkb_context_sanitize_rule_names(sa_nracanaa xkb_context *ctx,
                                sa_nracanaa xkb_rule_names *rmlvo)
{
    ga_nanaa RMLVO modified = 0;

    නම් (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    නම් (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    නම් (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        const var_na * const variant = xkb_context_get_default_variant(ctx);
        නම් (!isempty(rmlvo->variant)) {
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
    නම් (rmlvo->options == NULL) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ආපසු modified;
}
