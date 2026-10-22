/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "rmlvo.h"
#અન્તર્ભાવ "utils.h"

અક્ષર *
xkb_context_getenv(સંરચના xkb_context *ctx, અચલ અક્ષર *name)
{
    જો (ctx->use_secure_getenv) {
        પાછા secure_getenv(name);
    } નહીંતર {
        પાછા getenv(name);
    }
}

bool
xkb_context_init_includes(સંરચના xkb_context *ctx)
{
    જો (ctx->pending_default_includes) {
        જો (darray_empty(ctx->failed_includes)) {
            જો (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                પાછા false;
            }
            ctx->pending_default_includes = false;
        } નહીંતર {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            પાછા false;
        }
    }
    પાછા true;
}

darray_size_t
xkb_context_અંક_failed_include_paths(સંરચના xkb_context *ctx)
{
    પાછા (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

અચલ અક્ષર *
xkb_context_failed_include_path_get(સંરચના xkb_context *ctx,
                                    darray_size_t idx)
{
    જો (idx >= xkb_context_અંક_failed_include_paths(ctx))
        પાછા ખાલી;

    પાછા darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(સંરચના xkb_context *ctx)
{
    પાછા atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(સંરચના xkb_context *ctx, અચલ અક્ષર *string)
{
    પાછા atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(સંરચના xkb_context *ctx, અચલ અક્ષર *string, size_t len)
{
    પાછા atom_intern(ctx->atom_table, string, len, true);
}

અચલ અક્ષર *
xkb_atom_text(સંરચના xkb_context *ctx, xkb_atom_t atom)
{
    પાછા atom_text(ctx->atom_table, atom);
}

શૂન્ય
xkb_log(સંરચના xkb_context *ctx, ગણના xkb_log_level level, પૂર્ણાંક verbosity,
        અચલ અક્ષર *fmt, ...)
{
    va_list args;

    જો (ctx->log_level < level || ctx->log_verbosity < verbosity)
        પાછા;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

અક્ષર *
xkb_context_get_buffer(સંરચના xkb_context *ctx, size_t size)
{
    અક્ષર *rtrn;

    જો (size >= કદ(ctx->text_buffer))
        પાછા ખાલી;

    જો (કદ(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    પાછા rtrn;
}

સ્થિર અચલ અક્ષર *
xkb_context_get_default_rules(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *env = ખાલી;

    જો (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    પાછા env ? env : DEFAULT_XKB_RULES;
}

સ્થિર અચલ અક્ષર *
xkb_context_get_default_model(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *env = ખાલી;

    જો (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    પાછા env ? env : DEFAULT_XKB_MODEL;
}

સ્થિર અચલ અક્ષર *
xkb_context_get_default_layout(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *env = ખાલી;

    જો (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    પાછા env ? env : DEFAULT_XKB_LAYOUT;
}

સ્થિર અચલ અક્ષર *
xkb_context_get_default_variant(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *env = ખાલી;
    અચલ અક્ષર *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    જો (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    પાછા env ? env : DEFAULT_XKB_VARIANT;
}

સ્થિર અચલ અક્ષર *
xkb_context_get_default_options(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *env = ખાલી;

    જો (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    પાછા env ? env : DEFAULT_XKB_OPTIONS;
}

ગણના RMLVO
xkb_context_sanitize_rule_names(સંરચના xkb_context *ctx,
                                સંરચના xkb_rule_names *rmlvo)
{
    ગણના RMLVO modified = 0;

    જો (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    જો (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    જો (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        અચલ અક્ષર * અચલ variant = xkb_context_get_default_variant(ctx);
        જો (!isempty(rmlvo->variant)) {
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
    જો (rmlvo->options == ખાલી) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    પાછા modified;
}
