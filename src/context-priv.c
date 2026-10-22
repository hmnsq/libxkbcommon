/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "rmlvo.h"
#அந்தர்பாவ "utils.h"

எழுத்து *
xkb_context_getenv(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *name)
{
    எனில் (ctx->use_secure_getenv) {
        திரும்பு secure_getenv(name);
    } இல்லையெனில் {
        திரும்பு getenv(name);
    }
}

bool
xkb_context_init_includes(கட்டமைப்பு xkb_context *ctx)
{
    எனில் (ctx->pending_default_includes) {
        எனில் (darray_empty(ctx->failed_includes)) {
            எனில் (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                திரும்பு false;
            }
            ctx->pending_default_includes = false;
        } இல்லையெனில் {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            திரும்பு false;
        }
    }
    திரும்பு true;
}

darray_size_t
xkb_context_இலக்கம்_failed_include_paths(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

மாறிலி எழுத்து *
xkb_context_failed_include_path_get(கட்டமைப்பு xkb_context *ctx,
                                    darray_size_t idx)
{
    எனில் (idx >= xkb_context_இலக்கம்_failed_include_paths(ctx))
        திரும்பு வெற்று;

    திரும்பு darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(கட்டமைப்பு xkb_context *ctx)
{
    திரும்பு atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string)
{
    திரும்பு atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string, size_t len)
{
    திரும்பு atom_intern(ctx->atom_table, string, len, true);
}

மாறிலி எழுத்து *
xkb_atom_text(கட்டமைப்பு xkb_context *ctx, xkb_atom_t atom)
{
    திரும்பு atom_text(ctx->atom_table, atom);
}

வெறுமை
xkb_log(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_log_level level, முழு verbosity,
        மாறிலி எழுத்து *fmt, ...)
{
    va_list args;

    எனில் (ctx->log_level < level || ctx->log_verbosity < verbosity)
        திரும்பு;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

எழுத்து *
xkb_context_get_buffer(கட்டமைப்பு xkb_context *ctx, size_t size)
{
    எழுத்து *rtrn;

    எனில் (size >= அளவு(ctx->text_buffer))
        திரும்பு வெற்று;

    எனில் (அளவு(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    திரும்பு rtrn;
}

நிலையான மாறிலி எழுத்து *
xkb_context_get_default_rules(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *env = வெற்று;

    எனில் (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    திரும்பு env ? env : DEFAULT_XKB_RULES;
}

நிலையான மாறிலி எழுத்து *
xkb_context_get_default_model(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *env = வெற்று;

    எனில் (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    திரும்பு env ? env : DEFAULT_XKB_MODEL;
}

நிலையான மாறிலி எழுத்து *
xkb_context_get_default_layout(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *env = வெற்று;

    எனில் (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    திரும்பு env ? env : DEFAULT_XKB_LAYOUT;
}

நிலையான மாறிலி எழுத்து *
xkb_context_get_default_variant(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *env = வெற்று;
    மாறிலி எழுத்து *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    எனில் (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    திரும்பு env ? env : DEFAULT_XKB_VARIANT;
}

நிலையான மாறிலி எழுத்து *
xkb_context_get_default_options(கட்டமைப்பு xkb_context *ctx)
{
    மாறிலி எழுத்து *env = வெற்று;

    எனில் (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    திரும்பு env ? env : DEFAULT_XKB_OPTIONS;
}

எண்ணிக்கை RMLVO
xkb_context_sanitize_rule_names(கட்டமைப்பு xkb_context *ctx,
                                கட்டமைப்பு xkb_rule_names *rmlvo)
{
    எண்ணிக்கை RMLVO modified = 0;

    எனில் (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    எனில் (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    எனில் (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        மாறிலி எழுத்து * மாறிலி variant = xkb_context_get_default_variant(ctx);
        எனில் (!isempty(rmlvo->variant)) {
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
    எனில் (rmlvo->options == வெற்று) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    திரும்பு modified;
}
