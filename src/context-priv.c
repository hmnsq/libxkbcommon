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

অক্ষর *
xkb_context_getenv(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *name)
{
    যদি (ctx->use_secure_getenv) {
        ফেরত secure_getenv(name);
    } অন্যথা {
        ফেরত getenv(name);
    }
}

bool
xkb_context_init_includes(কাঠামো xkb_context *ctx)
{
    যদি (ctx->pending_default_includes) {
        যদি (darray_empty(ctx->failed_includes)) {
            যদি (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ফেরত false;
            }
            ctx->pending_default_includes = false;
        } অন্যথা {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ফেরত false;
        }
    }
    ফেরত true;
}

darray_আকার_t
xkb_context_অঙ্ক_failed_include_paths(কাঠামো xkb_context *ctx)
{
    ফেরত (xkb_context_init_includes(ctx))
        ? darray_আকার(ctx->failed_includes)
        : 0;
}

ধ্রুবক অক্ষর *
xkb_context_failed_include_path_get(কাঠামো xkb_context *ctx,
                                    darray_আকার_t idx)
{
    যদি (idx >= xkb_context_অঙ্ক_failed_include_paths(ctx))
        ফেরত ফাঁকা;

    ফেরত darray_item(ctx->failed_includes, idx);
}

darray_আকার_t
xkb_atom_table_আকার(কাঠামো xkb_context *ctx)
{
    ফেরত atom_table_আকার(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *string)
{
    ফেরত atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *string, আকার_t len)
{
    ফেরত atom_intern(ctx->atom_table, string, len, true);
}

ধ্রুবক অক্ষর *
xkb_atom_text(কাঠামো xkb_context *ctx, xkb_atom_t atom)
{
    ফেরত atom_text(ctx->atom_table, atom);
}

শূন্য
xkb_log(কাঠামো xkb_context *ctx, গণনা xkb_log_level level, পূর্ণসংখ্যা verbosity,
        ধ্রুবক অক্ষর *fmt, ...)
{
    va_list args;

    যদি (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ফেরত;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

অক্ষর *
xkb_context_get_বাফার(কাঠামো xkb_context *ctx, আকার_t আকার_2)
{
    অক্ষর *rtrn;

    যদি (আকার_2 >= আকার(ctx->text_বাফার))
        ফেরত ফাঁকা;

    যদি (আকার(ctx->text_বাফার) - ctx->text_পরবর্তী <= আকার_2)
        ctx->text_পরবর্তী = 0;

    rtrn = &ctx->text_বাফার[ctx->text_পরবর্তী];
    ctx->text_পরবর্তী += আকার_2;

    ফেরত rtrn;
}

স্থির ধ্রুবক অক্ষর *
xkb_context_get_default_rules(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *env = ফাঁকা;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ফেরত env ? env : DEFAULT_XKB_RULES;
}

স্থির ধ্রুবক অক্ষর *
xkb_context_get_default_model(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *env = ফাঁকা;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ফেরত env ? env : DEFAULT_XKB_MODEL;
}

স্থির ধ্রুবক অক্ষর *
xkb_context_get_default_layout(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *env = ফাঁকা;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ফেরত env ? env : DEFAULT_XKB_LAYOUT;
}

স্থির ধ্রুবক অক্ষর *
xkb_context_get_default_variant(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *env = ফাঁকা;
    ধ্রুবক অক্ষর *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    যদি (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ফেরত env ? env : DEFAULT_XKB_VARIANT;
}

স্থির ধ্রুবক অক্ষর *
xkb_context_get_default_options(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *env = ফাঁকা;

    যদি (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ফেরত env ? env : DEFAULT_XKB_OPTIONS;
}

গণনা RMLVO
xkb_context_sanitize_rule_names(কাঠামো xkb_context *ctx,
                                কাঠামো xkb_rule_names *rmlvo)
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
        ধ্রুবক অক্ষর * ধ্রুবক variant = xkb_context_get_default_variant(ctx);
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
    যদি (rmlvo->options == ফাঁকা) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ফেরত modified;
}
