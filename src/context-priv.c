/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ضمن "config.h"

#ضمن <sys/types.h>
#ضمن <sys/stat.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "context.h"
#ضمن "rmlvo.h"
#ضمن "utils.h"

حرف *
xkb_context_getenv(بنية xkb_context *ctx, ثابت حرف *name)
{
    ܐܢ (ctx->use_secure_getenv) {
        ܦܢܐ secure_getenv(name);
    } ܐܚܪܢܐ {
        ܦܢܐ getenv(name);
    }
}

bool
xkb_context_init_includes(بنية xkb_context *ctx)
{
    ܐܢ (ctx->pending_default_includes) {
        ܐܢ (darray_empty(ctx->failed_includes)) {
            ܐܢ (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ܦܢܐ false;
            }
            ctx->pending_default_includes = false;
        } ܐܚܪܢܐ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ܦܢܐ false;
        }
    }
    ܦܢܐ true;
}

darray_size_t
xkb_context_num_failed_include_paths(بنية xkb_context *ctx)
{
    ܦܢܐ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ثابت حرف *
xkb_context_failed_include_path_get(بنية xkb_context *ctx,
                                    darray_size_t idx)
{
    ܐܢ (idx >= xkb_context_num_failed_include_paths(ctx))
        ܦܢܐ عدم;

    ܦܢܐ darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(بنية xkb_context *ctx)
{
    ܦܢܐ atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(بنية xkb_context *ctx, ثابت حرف *string)
{
    ܦܢܐ atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(بنية xkb_context *ctx, ثابت حرف *string, size_t len)
{
    ܦܢܐ atom_intern(ctx->atom_table, string, len, true);
}

ثابت حرف *
xkb_atom_text(بنية xkb_context *ctx, xkb_atom_t atom)
{
    ܦܢܐ atom_text(ctx->atom_table, atom);
}

فراغ
xkb_log(بنية xkb_context *ctx, تعداد xkb_log_level level, صحيح verbosity,
        ثابت حرف *fmt, ...)
{
    va_list args;

    ܐܢ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ܦܢܐ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

حرف *
xkb_context_get_buffer(بنية xkb_context *ctx, size_t size)
{
    حرف *rtrn;

    ܐܢ (size >= حجم(ctx->text_buffer))
        ܦܢܐ عدم;

    ܐܢ (حجم(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    ܦܢܐ rtrn;
}

ساكن ثابت حرف *
xkb_context_get_default_rules(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    ܐܢ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ܦܢܐ env ? env : DEFAULT_XKB_RULES;
}

ساكن ثابت حرف *
xkb_context_get_default_model(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    ܐܢ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ܦܢܐ env ? env : DEFAULT_XKB_MODEL;
}

ساكن ثابت حرف *
xkb_context_get_default_layout(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    ܐܢ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ܦܢܐ env ? env : DEFAULT_XKB_LAYOUT;
}

ساكن ثابت حرف *
xkb_context_get_default_variant(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;
    ثابت حرف *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ܐܢ (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ܦܢܐ env ? env : DEFAULT_XKB_VARIANT;
}

ساكن ثابت حرف *
xkb_context_get_default_options(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    ܐܢ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ܦܢܐ env ? env : DEFAULT_XKB_OPTIONS;
}

تعداد RMLVO
xkb_context_sanitize_rule_names(بنية xkb_context *ctx,
                                بنية xkb_rule_names *rmlvo)
{
    تعداد RMLVO modified = 0;

    ܐܢ (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    ܐܢ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ܐܢ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ثابت حرف * ثابت variant = xkb_context_get_default_variant(ctx);
        ܐܢ (!isempty(rmlvo->variant)) {
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
    ܐܢ (rmlvo->options == عدم) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ܦܢܐ modified;
}
