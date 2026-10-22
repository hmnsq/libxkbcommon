/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ವರ್ಣ *
xkb_context_getenv(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *name)
{
    ಯದಿ (ctx->use_secure_getenv) {
        ಪ್ರತ್ಯಾಗಮ secure_getenv(name);
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ getenv(name);
    }
}

bool
xkb_context_init_includes(ಸಂರಚನಾ xkb_context *ctx)
{
    ಯದಿ (ctx->pending_default_includes) {
        ಯದಿ (darray_empty(ctx->failed_includes)) {
            ಯದಿ (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                ಪ್ರತ್ಯಾಗಮ false;
            }
            ctx->pending_default_includes = false;
        } ಅನ್ಯಥಾ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

darray_size_t
xkb_context_ಅಂಕೆ_failed_include_paths(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ಧ್ರುವ ವರ್ಣ *
xkb_context_failed_include_path_get(ಸಂರಚನಾ xkb_context *ctx,
                                    darray_size_t idx)
{
    ಯದಿ (idx >= xkb_context_ಅಂಕೆ_failed_include_paths(ctx))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *string)
{
    ಪ್ರತ್ಯಾಗಮ atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *string, size_t len)
{
    ಪ್ರತ್ಯಾಗಮ atom_intern(ctx->atom_table, string, len, true);
}

ಧ್ರುವ ವರ್ಣ *
xkb_atom_text(ಸಂರಚನಾ xkb_context *ctx, xkb_atom_t atom)
{
    ಪ್ರತ್ಯಾಗಮ atom_text(ctx->atom_table, atom);
}

ಶೂನ್ಯ
xkb_log(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_log_level level, ಪೂರ್ಣಾಙ್ಕ verbosity,
        ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list args;

    ಯದಿ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ಪ್ರತ್ಯಾಗಮ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

ವರ್ಣ *
xkb_context_get_buffer(ಸಂರಚನಾ xkb_context *ctx, size_t size)
{
    ವರ್ಣ *rtrn;

    ಯದಿ (size >= ಪರಿಮಾಣ(ctx->text_buffer))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (ಪರಿಮಾಣ(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    ಪ್ರತ್ಯಾಗಮ rtrn;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
xkb_context_get_default_rules(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *env = ರಿಕ್ತ;

    ಯದಿ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ಪ್ರತ್ಯಾಗಮ env ? env : DEFAULT_XKB_RULES;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
xkb_context_get_default_model(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *env = ರಿಕ್ತ;

    ಯದಿ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ಪ್ರತ್ಯಾಗಮ env ? env : DEFAULT_XKB_MODEL;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
xkb_context_get_default_layout(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *env = ರಿಕ್ತ;

    ಯದಿ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ಪ್ರತ್ಯಾಗಮ env ? env : DEFAULT_XKB_LAYOUT;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
xkb_context_get_default_variant(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *env = ರಿಕ್ತ;
    ಧ್ರುವ ವರ್ಣ *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ಯದಿ (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ಪ್ರತ್ಯಾಗಮ env ? env : DEFAULT_XKB_VARIANT;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
xkb_context_get_default_options(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *env = ರಿಕ್ತ;

    ಯದಿ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ಪ್ರತ್ಯಾಗಮ env ? env : DEFAULT_XKB_OPTIONS;
}

ಗಣನಾ RMLVO
xkb_context_sanitize_rule_names(ಸಂರಚನಾ xkb_context *ctx,
                                ಸಂರಚನಾ xkb_rule_names *rmlvo)
{
    ಗಣನಾ RMLVO modified = 0;

    ಯದಿ (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    ಯದಿ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ಯದಿ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ variant = xkb_context_get_default_variant(ctx);
        ಯದಿ (!isempty(rmlvo->variant)) {
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
    ಯದಿ (rmlvo->options == ರಿಕ್ತ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ಪ್ರತ್ಯಾಗಮ modified;
}
