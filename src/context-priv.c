/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "rmlvo.h"
#అన్తర్భావ "utils.h"

అక్షరం *
xkb_context_getenv(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *name)
{
    ఒకవేళ (ctx->use_secure_getenv) {
        తిరిగి secure_getenv(name);
    } లేకపోతే {
        తిరిగి getenv(name);
    }
}

bool
xkb_context_init_includes(నిర్మాణం xkb_context *ctx)
{
    ఒకవేళ (ctx->pending_default_includes) {
        ఒకవేళ (darray_ఖాళీలు(ctx->failed_includes)) {
            ఒకవేళ (!xkb_context_include_మార్గం_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_మార్గం,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_మార్గం_get_system_మార్గం(ctx));
                తిరిగి false;
            }
            ctx->pending_default_includes = false;
        } లేకపోతే {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            తిరిగి false;
        }
    }
    తిరిగి true;
}

darray_కొలత_t
xkb_context_అంకె_failed_include_paths(నిర్మాణం xkb_context *ctx)
{
    తిరిగి (xkb_context_init_includes(ctx))
        ? darray_కొలత(ctx->failed_includes)
        : 0;
}

స్థిరరాశి అక్షరం *
xkb_context_failed_include_మార్గం_get(నిర్మాణం xkb_context *ctx,
                                    darray_కొలత_t idx)
{
    ఒకవేళ (idx >= xkb_context_అంకె_failed_include_paths(ctx))
        తిరిగి ఖాళీ;

    తిరిగి darray_అంశం(ctx->failed_includes, idx);
}

darray_కొలత_t
xkb_atom_పట్టిక_కొలత(నిర్మాణం xkb_context *ctx)
{
    తిరిగి atom_పట్టిక_కొలత(ctx->atom_పట్టిక);
}

xkb_atom_t
xkb_atom_lookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string)
{
    తిరిగి atom_intern(ctx->atom_పట్టిక, string, అక్షరశ్రేణి_పొడవు(string), false);
}

xkb_atom_t
xkb_atom_intern(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    తిరిగి atom_intern(ctx->atom_పట్టిక, string, len, true);
}

స్థిరరాశి అక్షరం *
xkb_atom_పాఠ్యం(నిర్మాణం xkb_context *ctx, xkb_atom_t atom)
{
    తిరిగి atom_పాఠ్యం(ctx->atom_పట్టిక, atom);
}

శూన్య
xkb_log(నిర్మాణం xkb_context *ctx, గణన xkb_log_level level, పూర్ణాంకం verbosity,
        స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list వాదనలు;

    ఒకవేళ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        తిరిగి;

    va_ప్రారంభం(వాదనలు, fmt);
    ctx->log_fn(ctx, level, fmt, వాదనలు);
    va_చివర(వాదనలు);
}

అక్షరం *
xkb_context_get_బఫరు(నిర్మాణం xkb_context *ctx, కొలత_t కొలత)
{
    అక్షరం *rtrn;

    ఒకవేళ (కొలత >= పరిమాణం(ctx->పాఠ్యం_బఫరు))
        తిరిగి ఖాళీ;

    ఒకవేళ (పరిమాణం(ctx->పాఠ్యం_బఫరు) - ctx->పాఠ్యం_తదుపరి <= కొలత)
        ctx->పాఠ్యం_తదుపరి = 0;

    rtrn = &ctx->పాఠ్యం_బఫరు[ctx->పాఠ్యం_తదుపరి];
    ctx->పాఠ్యం_తదుపరి += కొలత;

    తిరిగి rtrn;
}

స్థిర స్థిరరాశి అక్షరం *
xkb_context_get_default_నియమాలు(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *env = ఖాళీ;

    ఒకవేళ (ctx->use_environment_పేర్లు)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    తిరిగి env ? env : DEFAULT_XKB_నియమాలు;
}

స్థిర స్థిరరాశి అక్షరం *
xkb_context_get_default_model(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *env = ఖాళీ;

    ఒకవేళ (ctx->use_environment_పేర్లు)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    తిరిగి env ? env : DEFAULT_XKB_MODEL;
}

స్థిర స్థిరరాశి అక్షరం *
xkb_context_get_default_layout(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *env = ఖాళీ;

    ఒకవేళ (ctx->use_environment_పేర్లు)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    తిరిగి env ? env : DEFAULT_XKB_LAYOUT;
}

స్థిర స్థిరరాశి అక్షరం *
xkb_context_get_default_variant(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *env = ఖాళీ;
    స్థిరరాశి అక్షరం *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ఒకవేళ (layout && ctx->use_environment_పేర్లు)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    తిరిగి env ? env : DEFAULT_XKB_VARIANT;
}

స్థిర స్థిరరాశి అక్షరం *
xkb_context_get_default_options(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *env = ఖాళీ;

    ఒకవేళ (ctx->use_environment_పేర్లు)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    తిరిగి env ? env : DEFAULT_XKB_OPTIONS;
}

గణన RMLVO
xkb_context_sanitize_నియమం_పేర్లు(నిర్మాణం xkb_context *ctx,
                                నిర్మాణం xkb_నియమం_పేర్లు *rmlvo)
{
    గణన RMLVO modified = 0;

    ఒకవేళ (isempty(rmlvo->నియమాలు)) {
        rmlvo->నియమాలు = xkb_context_get_default_నియమాలు(ctx);
        modified |= RMLVO_నియమాలు;
    }
    ఒకవేళ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ఒకవేళ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        స్థిరరాశి అక్షరం * స్థిరరాశి variant = xkb_context_get_default_variant(ctx);
        ఒకవేళ (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
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
    ఒకవేళ (rmlvo->options == ఖాళీ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    తిరిగి modified;
}
