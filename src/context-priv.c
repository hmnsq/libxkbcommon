/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#شامل "config.h"

#شامل <sys/types.h>
#شامل <sys/stat.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "darray.h"
#شامل "context.h"
#شامل "rmlvo.h"
#شامل "utils.h"

نویسه *
xkb_context_getenv(ساختار xkb_context *ctx, ثابت نویسه *name)
{
    اگر (ctx->use_secure_getenv) {
        بازگشت secure_getenv(name);
    } وگرنه {
        بازگشت getenv(name);
    }
}

bool
xkb_context_init_includes(ساختار xkb_context *ctx)
{
    اگر (ctx->pending_default_includes) {
        اگر (darray_empty(ctx->failed_includes)) {
            اگر (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                بازگشت false;
            }
            ctx->pending_default_includes = false;
        } وگرنه {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            بازگشت false;
        }
    }
    بازگشت true;
}

darray_size_t
xkb_context_رقم_failed_include_paths(ساختار xkb_context *ctx)
{
    بازگشت (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ثابت نویسه *
xkb_context_failed_include_path_get(ساختار xkb_context *ctx,
                                    darray_size_t idx)
{
    اگر (idx >= xkb_context_رقم_failed_include_paths(ctx))
        بازگشت پوچ;

    بازگشت darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(ساختار xkb_context *ctx)
{
    بازگشت atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(ساختار xkb_context *ctx, ثابت نویسه *string)
{
    بازگشت atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(ساختار xkb_context *ctx, ثابت نویسه *string, size_t len)
{
    بازگشت atom_intern(ctx->atom_table, string, len, true);
}

ثابت نویسه *
xkb_atom_text(ساختار xkb_context *ctx, xkb_atom_t atom)
{
    بازگشت atom_text(ctx->atom_table, atom);
}

تهی
xkb_log(ساختار xkb_context *ctx, شمارش xkb_log_level level, صحیح verbosity,
        ثابت نویسه *fmt, ...)
{
    va_list args;

    اگر (ctx->log_level < level || ctx->log_verbosity < verbosity)
        بازگشت;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

نویسه *
xkb_context_get_buffer(ساختار xkb_context *ctx, size_t size)
{
    نویسه *rtrn;

    اگر (size >= اندازه(ctx->text_buffer))
        بازگشت پوچ;

    اگر (اندازه(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    بازگشت rtrn;
}

ایستا ثابت نویسه *
xkb_context_get_default_rules(ساختار xkb_context *ctx)
{
    ثابت نویسه *env = پوچ;

    اگر (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    بازگشت env ? env : DEFAULT_XKB_RULES;
}

ایستا ثابت نویسه *
xkb_context_get_default_model(ساختار xkb_context *ctx)
{
    ثابت نویسه *env = پوچ;

    اگر (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    بازگشت env ? env : DEFAULT_XKB_MODEL;
}

ایستا ثابت نویسه *
xkb_context_get_default_layout(ساختار xkb_context *ctx)
{
    ثابت نویسه *env = پوچ;

    اگر (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    بازگشت env ? env : DEFAULT_XKB_LAYOUT;
}

ایستا ثابت نویسه *
xkb_context_get_default_variant(ساختار xkb_context *ctx)
{
    ثابت نویسه *env = پوچ;
    ثابت نویسه *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    اگر (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    بازگشت env ? env : DEFAULT_XKB_VARIANT;
}

ایستا ثابت نویسه *
xkb_context_get_default_options(ساختار xkb_context *ctx)
{
    ثابت نویسه *env = پوچ;

    اگر (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    بازگشت env ? env : DEFAULT_XKB_OPTIONS;
}

شمارش RMLVO
xkb_context_sanitize_rule_names(ساختار xkb_context *ctx,
                                ساختار xkb_rule_names *rmlvo)
{
    شمارش RMLVO modified = 0;

    اگر (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    اگر (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    اگر (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ثابت نویسه * ثابت variant = xkb_context_get_default_variant(ctx);
        اگر (!isempty(rmlvo->variant)) {
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
    اگر (rmlvo->options == پوچ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    بازگشت modified;
}
