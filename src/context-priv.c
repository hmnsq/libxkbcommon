/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਅੱਖਰ *
xkb_context_getenv(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *name)
{
    ਜੇ (ctx->use_secure_getenv) {
        ਵਾਪਸ secure_getenv(name);
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ getenv(name);
    }
}

bool
xkb_context_init_includes(ਬਣਤਰ xkb_context *ctx)
{
    ਜੇ (ctx->pending_default_includes) {
        ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(ctx->failed_includes)) {
            ਜੇ (!xkb_context_include_ਰਸਤਾ_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_ਰਸਤਾ,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_ਰਸਤਾ_get_system_ਰਸਤਾ(ctx));
                ਵਾਪਸ false;
            }
            ctx->pending_default_includes = false;
        } ਨਹੀਂਤਾਂ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            ਵਾਪਸ false;
        }
    }
    ਵਾਪਸ true;
}

darray_ਮਾਪ_t
xkb_context_ਅੰਕ_failed_include_paths(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ (xkb_context_init_includes(ctx))
        ? darray_ਮਾਪ(ctx->failed_includes)
        : 0;
}

ਅਚਲ ਅੱਖਰ *
xkb_context_failed_include_ਰਸਤਾ_get(ਬਣਤਰ xkb_context *ctx,
                                    darray_ਮਾਪ_t idx)
{
    ਜੇ (idx >= xkb_context_ਅੰਕ_failed_include_paths(ctx))
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ darray_ਵਸਤੂ(ctx->failed_includes, idx);
}

darray_ਮਾਪ_t
xkb_atom_ਸਾਰਣੀ_ਮਾਪ(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ atom_ਸਾਰਣੀ_ਮਾਪ(ctx->atom_ਸਾਰਣੀ);
}

xkb_atom_t
xkb_atom_lookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string)
{
    ਵਾਪਸ atom_intern(ctx->atom_ਸਾਰਣੀ, string, ਲੜੀ_ਲੰਬਾਈ(string), false);
}

xkb_atom_t
xkb_atom_intern(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    ਵਾਪਸ atom_intern(ctx->atom_ਸਾਰਣੀ, string, len, true);
}

ਅਚਲ ਅੱਖਰ *
xkb_atom_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_atom_t atom)
{
    ਵਾਪਸ atom_ਪਾਠ(ctx->atom_ਸਾਰਣੀ, atom);
}

ਖਾਲੀ
xkb_log(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_log_level level, ਪੂਰਨ verbosity,
        ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list ਦਲੀਲਾਂ;

    ਜੇ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        ਵਾਪਸ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    ctx->log_fn(ctx, level, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);
}

ਅੱਖਰ *
xkb_context_get_ਬਫਰ(ਬਣਤਰ xkb_context *ctx, ਮਾਪ_t ਮਾਪ)
{
    ਅੱਖਰ *rtrn;

    ਜੇ (ਮਾਪ >= ਆਕਾਰ(ctx->ਪਾਠ_ਬਫਰ))
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (ਆਕਾਰ(ctx->ਪਾਠ_ਬਫਰ) - ctx->ਪਾਠ_ਅਗਲਾ <= ਮਾਪ)
        ctx->ਪਾਠ_ਅਗਲਾ = 0;

    rtrn = &ctx->ਪਾਠ_ਬਫਰ[ctx->ਪਾਠ_ਅਗਲਾ];
    ctx->ਪਾਠ_ਅਗਲਾ += ਮਾਪ;

    ਵਾਪਸ rtrn;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
xkb_context_get_default_ਨਿਯਮਾਂ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *env = ਨਿਰਾ;

    ਜੇ (ctx->use_environment_ਨਾਂ)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    ਵਾਪਸ env ? env : DEFAULT_XKB_ਨਿਯਮਾਂ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
xkb_context_get_default_model(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *env = ਨਿਰਾ;

    ਜੇ (ctx->use_environment_ਨਾਂ)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    ਵਾਪਸ env ? env : DEFAULT_XKB_MODEL;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
xkb_context_get_default_layout(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *env = ਨਿਰਾ;

    ਜੇ (ctx->use_environment_ਨਾਂ)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    ਵਾਪਸ env ? env : DEFAULT_XKB_LAYOUT;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
xkb_context_get_default_variant(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *env = ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    ਜੇ (layout && ctx->use_environment_ਨਾਂ)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    ਵਾਪਸ env ? env : DEFAULT_XKB_VARIANT;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
xkb_context_get_default_options(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *env = ਨਿਰਾ;

    ਜੇ (ctx->use_environment_ਨਾਂ)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    ਵਾਪਸ env ? env : DEFAULT_XKB_OPTIONS;
}

ਗਿਣਤੀ RMLVO
xkb_context_sanitize_ਨਿਯਮ_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
                                ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo)
{
    ਗਿਣਤੀ RMLVO modified = 0;

    ਜੇ (isempty(rmlvo->ਨਿਯਮਾਂ)) {
        rmlvo->ਨਿਯਮਾਂ = xkb_context_get_default_ਨਿਯਮਾਂ(ctx);
        modified |= RMLVO_ਨਿਯਮਾਂ;
    }
    ਜੇ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    ਜੇ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ਅਚਲ ਅੱਖਰ * ਅਚਲ variant = xkb_context_get_default_variant(ctx);
        ਜੇ (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
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
    ਜੇ (rmlvo->options == ਨਿਰਾ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    ਵਾਪਸ modified;
}
