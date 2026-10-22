/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "rmlvo.h"
#অন্তর্ভাব "utils.h"

বর্ণ *
xkb_context_getenv(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *name)
{
    যদি (ctx->use_secure_getenv) {
        প্রত্যাগম secure_getenv(name);
    } অন্যথা {
        প্রত্যাগম getenv(name);
    }
}

bool
xkb_context_init_includes(সংরচনা xkb_context *ctx)
{
    যদি (ctx->pending_default_includes) {
        যদি (darray_empty(ctx->failed_includes)) {
            যদি (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                প্রত্যাগম false;
            }
            ctx->pending_default_includes = false;
        } অন্যথা {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            প্রত্যাগম false;
        }
    }
    প্রত্যাগম true;
}

darray_size_t
xkb_context_অঙ্ক_failed_include_paths(সংরচনা xkb_context *ctx)
{
    প্রত্যাগম (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ধ্রুব বর্ণ *
xkb_context_failed_include_path_get(সংরচনা xkb_context *ctx,
                                    darray_size_t idx)
{
    যদি (idx >= xkb_context_অঙ্ক_failed_include_paths(ctx))
        প্রত্যাগম রিক্ত;

    প্রত্যাগম darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(সংরচনা xkb_context *ctx)
{
    প্রত্যাগম atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *string)
{
    প্রত্যাগম atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *string, size_t len)
{
    প্রত্যাগম atom_intern(ctx->atom_table, string, len, true);
}

ধ্রুব বর্ণ *
xkb_atom_text(সংরচনা xkb_context *ctx, xkb_atom_t atom)
{
    প্রত্যাগম atom_text(ctx->atom_table, atom);
}

শূন্য
xkb_log(সংরচনা xkb_context *ctx, গণনা xkb_log_level level, পূর্ণাঙ্ক verbosity,
        ধ্রুব বর্ণ *fmt, ...)
{
    va_list args;

    যদি (ctx->log_level < level || ctx->log_verbosity < verbosity)
        প্রত্যাগম;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

বর্ণ *
xkb_context_get_buffer(সংরচনা xkb_context *ctx, size_t size)
{
    বর্ণ *rtrn;

    যদি (size >= পরিমাণ(ctx->text_buffer))
        প্রত্যাগম রিক্ত;

    যদি (পরিমাণ(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    প্রত্যাগম rtrn;
}

স্থির ধ্রুব বর্ণ *
xkb_context_get_default_rules(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *env = রিক্ত;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    প্রত্যাগম env ? env : DEFAULT_XKB_RULES;
}

স্থির ধ্রুব বর্ণ *
xkb_context_get_default_model(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *env = রিক্ত;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    প্রত্যাগম env ? env : DEFAULT_XKB_MODEL;
}

স্থির ধ্রুব বর্ণ *
xkb_context_get_default_layout(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *env = রিক্ত;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    প্রত্যাগম env ? env : DEFAULT_XKB_LAYOUT;
}

স্থির ধ্রুব বর্ণ *
xkb_context_get_default_variant(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *env = রিক্ত;
    ধ্রুব বর্ণ *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    যদি (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    প্রত্যাগম env ? env : DEFAULT_XKB_VARIANT;
}

স্থির ধ্রুব বর্ণ *
xkb_context_get_default_options(সংরচনা xkb_context *ctx)
{
    ধ্রুব বর্ণ *env = রিক্ত;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    প্রত্যাগম env ? env : DEFAULT_XKB_OPTIONS;
}

গণনা RMLVO
xkb_context_sanitize_rule_names(সংরচনা xkb_context *ctx,
                                সংরচনা xkb_rule_names *rmlvo)
{
    গণনা RMLVO modified = 0;

    যদি (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    যদি (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    যদি (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ধ্রুব বর্ণ * ধ্রুব variant = xkb_context_get_default_variant(ctx);
        যদি (!isempty(rmlvo->variant)) {
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
    যদি (rmlvo->options == রিক্ত) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    প্রত্যাগম modified;
}
