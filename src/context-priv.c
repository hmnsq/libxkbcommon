/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "rmlvo.h"
#अन्तर्भाव "utils.h"

वर्ण *
xkb_context_getenv(संरचना xkb_context *ctx, ध्रुव वर्ण *name)
{
    सचे (ctx->use_secure_getenv) {
        पटिनिवत्त secure_getenv(name);
    } अञ्ञथा {
        पटिनिवत्त getenv(name);
    }
}

bool
xkb_context_init_includes(संरचना xkb_context *ctx)
{
    सचे (ctx->pending_default_includes) {
        सचे (darray_empty(ctx->failed_includes)) {
            सचे (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                पटिनिवत्त false;
            }
            ctx->pending_default_includes = false;
        } अञ्ञथा {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            पटिनिवत्त false;
        }
    }
    पटिनिवत्त true;
}

darray_size_t
xkb_context_अङ्कः_failed_include_paths(संरचना xkb_context *ctx)
{
    पटिनिवत्त (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

ध्रुव वर्ण *
xkb_context_failed_include_path_get(संरचना xkb_context *ctx,
                                    darray_size_t idx)
{
    सचे (idx >= xkb_context_अङ्कः_failed_include_paths(ctx))
        पटिनिवत्त रिक्त;

    पटिनिवत्त darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(संरचना xkb_context *ctx)
{
    पटिनिवत्त atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(संरचना xkb_context *ctx, ध्रुव वर्ण *string)
{
    पटिनिवत्त atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(संरचना xkb_context *ctx, ध्रुव वर्ण *string, size_t len)
{
    पटिनिवत्त atom_intern(ctx->atom_table, string, len, true);
}

ध्रुव वर्ण *
xkb_atom_text(संरचना xkb_context *ctx, xkb_atom_t atom)
{
    पटिनिवत्त atom_text(ctx->atom_table, atom);
}

शून्य
xkb_log(संरचना xkb_context *ctx, गणना xkb_log_level level, पूर्णाङ्क verbosity,
        ध्रुव वर्ण *fmt, ...)
{
    va_list args;

    सचे (ctx->log_level < level || ctx->log_verbosity < verbosity)
        पटिनिवत्त;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

वर्ण *
xkb_context_get_buffer(संरचना xkb_context *ctx, size_t size)
{
    वर्ण *rtrn;

    सचे (size >= परिमाण(ctx->text_buffer))
        पटिनिवत्त रिक्त;

    सचे (परिमाण(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    पटिनिवत्त rtrn;
}

स्थिर ध्रुव वर्ण *
xkb_context_get_default_rules(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *env = रिक्त;

    सचे (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    पटिनिवत्त env ? env : DEFAULT_XKB_RULES;
}

स्थिर ध्रुव वर्ण *
xkb_context_get_default_model(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *env = रिक्त;

    सचे (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    पटिनिवत्त env ? env : DEFAULT_XKB_MODEL;
}

स्थिर ध्रुव वर्ण *
xkb_context_get_default_layout(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *env = रिक्त;

    सचे (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    पटिनिवत्त env ? env : DEFAULT_XKB_LAYOUT;
}

स्थिर ध्रुव वर्ण *
xkb_context_get_default_variant(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *env = रिक्त;
    ध्रुव वर्ण *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    सचे (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    पटिनिवत्त env ? env : DEFAULT_XKB_VARIANT;
}

स्थिर ध्रुव वर्ण *
xkb_context_get_default_options(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *env = रिक्त;

    सचे (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    पटिनिवत्त env ? env : DEFAULT_XKB_OPTIONS;
}

गणना RMLVO
xkb_context_sanitize_rule_names(संरचना xkb_context *ctx,
                                संरचना xkb_rule_names *rmlvo)
{
    गणना RMLVO modified = 0;

    सचे (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    सचे (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    सचे (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ध्रुव वर्ण * ध्रुव variant = xkb_context_get_default_variant(ctx);
        सचे (!isempty(rmlvo->variant)) {
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
    सचे (rmlvo->options == रिक्त) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    पटिनिवत्त modified;
}
