/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#समावेश "config.h"

#समावेश <sys/types.h>
#समावेश <sys/stat.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "atom.h"
#समावेश "darray.h"
#समावेश "context.h"
#समावेश "rmlvo.h"
#समावेश "utils.h"

अक्षर *
xkb_context_getenv(काष्ठा xkb_context *ctx, स्थिर अक्षर *name)
{
    अगर (ctx->use_secure_getenv) {
        वापस secure_getenv(name);
    } अन्यथा {
        वापस दो_पर्या(name);
    }
}

bool
xkb_context_init_includes(काष्ठा xkb_context *ctx)
{
    अगर (ctx->pending_default_includes) {
        अगर (darray_empty(ctx->failed_includes)) {
            अगर (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                वापस false;
            }
            ctx->pending_default_includes = false;
        } अन्यथा {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            वापस false;
        }
    }
    वापस true;
}

darray_आकार_t
xkb_context_अंक_failed_include_paths(काष्ठा xkb_context *ctx)
{
    वापस (xkb_context_init_includes(ctx))
        ? darray_आकार(ctx->failed_includes)
        : 0;
}

स्थिर अक्षर *
xkb_context_failed_include_path_get(काष्ठा xkb_context *ctx,
                                    darray_आकार_t idx)
{
    अगर (idx >= xkb_context_अंक_failed_include_paths(ctx))
        वापस शून्य;

    वापस darray_item(ctx->failed_includes, idx);
}

darray_आकार_t
xkb_atom_table_आकार(काष्ठा xkb_context *ctx)
{
    वापस atom_table_आकार(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(काष्ठा xkb_context *ctx, स्थिर अक्षर *string)
{
    वापस atom_intern(ctx->atom_table, string, म_माप(string), false);
}

xkb_atom_t
xkb_atom_intern(काष्ठा xkb_context *ctx, स्थिर अक्षर *string, माप_प्रकार len)
{
    वापस atom_intern(ctx->atom_table, string, len, true);
}

स्थिर अक्षर *
xkb_atom_text(काष्ठा xkb_context *ctx, xkb_atom_t atom)
{
    वापस atom_text(ctx->atom_table, atom);
}

व्योम
xkb_log(काष्ठा xkb_context *ctx, क्रमागत xkb_log_level level, पूर्णांक verbosity,
        स्थिर अक्षर *fmt, ...)
{
    बहु_सूची args;

    अगर (ctx->log_level < level || ctx->log_verbosity < verbosity)
        वापस;

    बहु_शुरू(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    बहु_पूर्ण(args);
}

अक्षर *
xkb_context_get_बफर(काष्ठा xkb_context *ctx, माप_प्रकार आकार)
{
    अक्षर *rtrn;

    अगर (आकार >= माप(ctx->text_बफर))
        वापस शून्य;

    अगर (माप(ctx->text_बफर) - ctx->text_अग्र <= आकार)
        ctx->text_अग्र = 0;

    rtrn = &ctx->text_बफर[ctx->text_अग्र];
    ctx->text_अग्र += आकार;

    वापस rtrn;
}

अटल स्थिर अक्षर *
xkb_context_get_default_rules(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *env = शून्य;

    अगर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    वापस env ? env : DEFAULT_XKB_RULES;
}

अटल स्थिर अक्षर *
xkb_context_get_default_model(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *env = शून्य;

    अगर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    वापस env ? env : DEFAULT_XKB_MODEL;
}

अटल स्थिर अक्षर *
xkb_context_get_default_layout(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *env = शून्य;

    अगर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    वापस env ? env : DEFAULT_XKB_LAYOUT;
}

अटल स्थिर अक्षर *
xkb_context_get_default_variant(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *env = शून्य;
    स्थिर अक्षर *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    अगर (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    वापस env ? env : DEFAULT_XKB_VARIANT;
}

अटल स्थिर अक्षर *
xkb_context_get_default_options(काष्ठा xkb_context *ctx)
{
    स्थिर अक्षर *env = शून्य;

    अगर (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    वापस env ? env : DEFAULT_XKB_OPTIONS;
}

क्रमागत RMLVO
xkb_context_sanitize_rule_names(काष्ठा xkb_context *ctx,
                                काष्ठा xkb_rule_names *rmlvo)
{
    क्रमागत RMLVO modified = 0;

    अगर (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    अगर (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    अगर (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        स्थिर अक्षर * स्थिर variant = xkb_context_get_default_variant(ctx);
        अगर (!isempty(rmlvo->variant)) {
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
    अगर (rmlvo->options == शून्य) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    वापस modified;
}
