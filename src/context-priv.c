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

ಅಕ್ಷರ *
xkb_context_getenv(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಆದರೆ (ctx->use_secure_getenv) {
        ಹಿಂತಿರುಗಿ secure_getenv(name);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ getenv(name);
    }
}

bool
xkb_context_init_includes(ರಚನೆ xkb_context *ctx)
{
    ಆದರೆ (ctx->pending_default_includes) {
        ಆದರೆ (darray_ಖಾಲಿಗಳು(ctx->failed_includes)) {
            ಆದರೆ (!xkb_context_include_ಮಾರ್ಗ_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_ಮಾರ್ಗ,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_ಮಾರ್ಗ_get_system_ಮಾರ್ಗ(ctx));
                ಹಿಂತಿರುಗಿ false;
            }
            ctx->pending_default_includes = false;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ಹಿಂತಿರುಗಿ false;
        }
    }
    ಹಿಂತಿರುಗಿ true;
}

darray_ಗಾತ್ರ_t
xkb_context_ಅಂಕೆ_failed_include_paths(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ (xkb_context_init_includes(ctx))
        ? darray_ಗಾತ್ರ(ctx->failed_includes)
        : 0;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_failed_include_ಮಾರ್ಗ_get(ರಚನೆ xkb_context *ctx,
                                    darray_ಗಾತ್ರ_t idx)
{
    ಆದರೆ (idx >= xkb_context_ಅಂಕೆ_failed_include_paths(ctx))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ darray_ಅಂಶ(ctx->failed_includes, idx);
}

darray_ಗಾತ್ರ_t
xkb_atom_ಕೋಷ್ಟಕ_ಗಾತ್ರ(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ atom_ಕೋಷ್ಟಕ_ಗಾತ್ರ(ctx->atom_ಕೋಷ್ಟಕ);
}

xkb_atom_t
xkb_atom_lookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string)
{
    ಹಿಂತಿರುಗಿ atom_intern(ctx->atom_ಕೋಷ್ಟಕ, string, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string), false);
}

xkb_atom_t
xkb_atom_intern(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    ಹಿಂತಿರುಗಿ atom_intern(ctx->atom_ಕೋಷ್ಟಕ, string, len, true);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_atom_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_atom_t atom)
{
    ಹಿಂತಿರುಗಿ atom_ಪಠ್ಯ(ctx->atom_ಕೋಷ್ಟಕ, atom);
}

ಶೂನ್ಯ
xkb_log(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_log_level level, ಪೂರ್ಣಾಂಕ verbosity,
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list ವಾದಗಳು;

    ಆದರೆ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ಹಿಂತಿರುಗಿ;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    ctx->log_fn(ctx, level, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);
}

ಅಕ್ಷರ *
xkb_context_get_ಬಫರ್(ರಚನೆ xkb_context *ctx, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಅಕ್ಷರ *rtrn;

    ಆದರೆ (ಗಾತ್ರ_2 >= ಗಾತ್ರ(ctx->ಪಠ್ಯ_ಬಫರ್))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (ಗಾತ್ರ(ctx->ಪಠ್ಯ_ಬಫರ್) - ctx->ಪಠ್ಯ_ಮುಂದಿನ <= ಗಾತ್ರ_2)
        ctx->ಪಠ್ಯ_ಮುಂದಿನ = 0;

    rtrn = &ctx->ಪಠ್ಯ_ಬಫರ್[ctx->ಪಠ್ಯ_ಮುಂದಿನ];
    ctx->ಪಠ್ಯ_ಮುಂದಿನ += ಗಾತ್ರ_2;

    ಹಿಂತಿರುಗಿ rtrn;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_get_default_ನಿಯಮಗಳು(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env = ಖಾಲಿ;

    ಆದರೆ (ctx->use_environment_ಹೆಸರುಗಳು)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ಹಿಂತಿರುಗಿ env ? env : DEFAULT_XKB_ನಿಯಮಗಳು;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_get_default_model(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env = ಖಾಲಿ;

    ಆದರೆ (ctx->use_environment_ಹೆಸರುಗಳು)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ಹಿಂತಿರುಗಿ env ? env : DEFAULT_XKB_MODEL;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_get_default_layout(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env = ಖಾಲಿ;

    ಆದರೆ (ctx->use_environment_ಹೆಸರುಗಳು)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ಹಿಂತಿರುಗಿ env ? env : DEFAULT_XKB_LAYOUT;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_get_default_variant(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ಆದರೆ (layout && ctx->use_environment_ಹೆಸರುಗಳು)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ಹಿಂತಿರುಗಿ env ? env : DEFAULT_XKB_VARIANT;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_get_default_options(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env = ಖಾಲಿ;

    ಆದರೆ (ctx->use_environment_ಹೆಸರುಗಳು)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ಹಿಂತಿರುಗಿ env ? env : DEFAULT_XKB_OPTIONS;
}

ಎಣಿಕೆ RMLVO
xkb_context_sanitize_ನಿಯಮ_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
                                ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo)
{
    ಎಣಿಕೆ RMLVO modified = 0;

    ಆದರೆ (isempty(rmlvo->ನಿಯಮಗಳು)) {
        rmlvo->ನಿಯಮಗಳು = xkb_context_get_default_ನಿಯಮಗಳು(ctx);
        modified |= RMLVO_ನಿಯಮಗಳು;
    }
    ಆದರೆ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ಆದರೆ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ variant = xkb_context_get_default_variant(ctx);
        ಆದರೆ (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
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
    ಆದರೆ (rmlvo->options == ಖಾಲಿ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ಹಿಂತಿರುಗಿ modified;
}
