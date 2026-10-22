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

अक्षर *
xkb_context_getenv(संरचना xkb_context *ctx, स्थिरांक अक्षर *name)
{
    जर (ctx->use_secure_getenv) {
        परत secure_getenv(name);
    } अन्यथा {
        परत दो_पर्या(name);
    }
}

bool
xkb_context_init_includes(संरचना xkb_context *ctx)
{
    जर (ctx->pending_default_includes) {
        जर (darray_empty(ctx->failed_includes)) {
            जर (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                परत false;
            }
            ctx->pending_default_includes = false;
        } अन्यथा {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            परत false;
        }
    }
    परत true;
}

darray_size_t
xkb_context_अंक_failed_include_paths(संरचना xkb_context *ctx)
{
    परत (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

स्थिरांक अक्षर *
xkb_context_failed_include_path_get(संरचना xkb_context *ctx,
                                    darray_size_t idx)
{
    जर (idx >= xkb_context_अंक_failed_include_paths(ctx))
        परत रिक्त;

    परत darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(संरचना xkb_context *ctx)
{
    परत atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(संरचना xkb_context *ctx, स्थिरांक अक्षर *string)
{
    परत atom_intern(ctx->atom_table, string, म_माप(string), false);
}

xkb_atom_t
xkb_atom_intern(संरचना xkb_context *ctx, स्थिरांक अक्षर *string, माप_प्रकार len)
{
    परत atom_intern(ctx->atom_table, string, len, true);
}

स्थिरांक अक्षर *
xkb_atom_text(संरचना xkb_context *ctx, xkb_atom_t atom)
{
    परत atom_text(ctx->atom_table, atom);
}

शून्य
xkb_log(संरचना xkb_context *ctx, गणना xkb_log_level level, पूर्णांक verbosity,
        स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची args;

    जर (ctx->log_level < level || ctx->log_verbosity < verbosity)
        परत;

    बहु_शुरू(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    बहु_पूर्ण(args);
}

अक्षर *
xkb_context_get_buffer(संरचना xkb_context *ctx, माप_प्रकार size)
{
    अक्षर *rtrn;

    जर (size >= आकार(ctx->text_buffer))
        परत रिक्त;

    जर (आकार(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    परत rtrn;
}

स्थिर स्थिरांक अक्षर *
xkb_context_get_default_rules(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *env = रिक्त;

    जर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    परत env ? env : DEFAULT_XKB_RULES;
}

स्थिर स्थिरांक अक्षर *
xkb_context_get_default_model(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *env = रिक्त;

    जर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    परत env ? env : DEFAULT_XKB_MODEL;
}

स्थिर स्थिरांक अक्षर *
xkb_context_get_default_layout(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *env = रिक्त;

    जर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    परत env ? env : DEFAULT_XKB_LAYOUT;
}

स्थिर स्थिरांक अक्षर *
xkb_context_get_default_variant(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *env = रिक्त;
    स्थिरांक अक्षर *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    जर (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    परत env ? env : DEFAULT_XKB_VARIANT;
}

स्थिर स्थिरांक अक्षर *
xkb_context_get_default_options(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *env = रिक्त;

    जर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    परत env ? env : DEFAULT_XKB_OPTIONS;
}

गणना RMLVO
xkb_context_sanitize_rule_names(संरचना xkb_context *ctx,
                                संरचना xkb_rule_names *rmlvo)
{
    गणना RMLVO modified = 0;

    जर (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    जर (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    जर (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        स्थिरांक अक्षर * स्थिरांक variant = xkb_context_get_default_variant(ctx);
        जर (!isempty(rmlvo->variant)) {
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
    जर (rmlvo->options == रिक्त) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    परत modified;
}
