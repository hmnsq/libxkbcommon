/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "rmlvo.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ଅକ୍ଷର *
xkb_context_getenv(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *name)
{
    ଯଦି (ctx->use_secure_getenv) {
        ଫେରନ୍ତୁ secure_getenv(name);
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ getenv(name);
    }
}

bool
xkb_context_init_includes(ସଂରଚନା xkb_context *ctx)
{
    ଯଦି (ctx->pending_default_includes) {
        ଯଦି (darray_empty(ctx->failed_includes)) {
            ଯଦି (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ଫେରନ୍ତୁ false;
            }
            ctx->pending_default_includes = false;
        } ଅନ୍ୟଥା {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ଫେରନ୍ତୁ false;
        }
    }
    ଫେରନ୍ତୁ true;
}

darray_size_t
xkb_context_ଅଙ୍କ_failed_include_paths(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ଅଚଳ ଅକ୍ଷର *
xkb_context_failed_include_path_get(ସଂରଚନା xkb_context *ctx,
                                    darray_size_t idx)
{
    ଯଦି (idx >= xkb_context_ଅଙ୍କ_failed_include_paths(ctx))
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(ସଂରଚନା xkb_context *ctx)
{
    ଫେରନ୍ତୁ atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *string)
{
    ଫେରନ୍ତୁ atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *string, size_t len)
{
    ଫେରନ୍ତୁ atom_intern(ctx->atom_table, string, len, true);
}

ଅଚଳ ଅକ୍ଷର *
xkb_atom_text(ସଂରଚନା xkb_context *ctx, xkb_atom_t atom)
{
    ଫେରନ୍ତୁ atom_text(ctx->atom_table, atom);
}

ଶୂନ୍ୟ
xkb_log(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_log_level level, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity,
        ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list args;

    ଯଦି (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ଫେରନ୍ତୁ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

ଅକ୍ଷର *
xkb_context_get_buffer(ସଂରଚନା xkb_context *ctx, size_t size)
{
    ଅକ୍ଷର *rtrn;

    ଯଦି (size >= ଆକାର(ctx->text_buffer))
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (ଆକାର(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    ଫେରନ୍ତୁ rtrn;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
xkb_context_get_default_rules(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *env = ଖାଲି;

    ଯଦି (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ଫେରନ୍ତୁ env ? env : DEFAULT_XKB_RULES;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
xkb_context_get_default_model(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *env = ଖାଲି;

    ଯଦି (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ଫେରନ୍ତୁ env ? env : DEFAULT_XKB_MODEL;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
xkb_context_get_default_layout(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *env = ଖାଲି;

    ଯଦି (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ଫେରନ୍ତୁ env ? env : DEFAULT_XKB_LAYOUT;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
xkb_context_get_default_variant(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *env = ଖାଲି;
    ଅଚଳ ଅକ୍ଷର *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ଯଦି (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ଫେରନ୍ତୁ env ? env : DEFAULT_XKB_VARIANT;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
xkb_context_get_default_options(ସଂରଚନା xkb_context *ctx)
{
    ଅଚଳ ଅକ୍ଷର *env = ଖାଲି;

    ଯଦି (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ଫେରନ୍ତୁ env ? env : DEFAULT_XKB_OPTIONS;
}

ଗଣନା RMLVO
xkb_context_sanitize_rule_names(ସଂରଚନା xkb_context *ctx,
                                ସଂରଚନା xkb_rule_names *rmlvo)
{
    ଗଣନା RMLVO modified = 0;

    ଯଦି (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    ଯଦି (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ଯଦି (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ variant = xkb_context_get_default_variant(ctx);
        ଯଦି (!isempty(rmlvo->variant)) {
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
    ଯଦି (rmlvo->options == ଖାଲି) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ଫେରନ୍ତୁ modified;
}
