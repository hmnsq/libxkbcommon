/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#شامل "config.h"

#شامل <sys/types.h>
#شامل <sys/stat.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "darray.h"
#شامل "context.h"
#شامل "rmlvo.h"
#شامل "utils.h"

حرف *
xkb_context_getenv(ساخت xkb_context *ctx, ثابت حرف *name)
{
    اگر (ctx->use_secure_getenv) {
        واپس secure_getenv(name);
    } ورنہ {
        واپس getenv(name);
    }
}

bool
xkb_context_init_includes(ساخت xkb_context *ctx)
{
    اگر (ctx->pending_default_includes) {
        اگر (darray_خالی_خانے(ctx->failed_includes)) {
            اگر (!xkb_context_include_راستہ_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_راستہ,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_راستہ_get_system_راستہ(ctx));
                واپس false;
            }
            ctx->pending_default_includes = false;
        } ورنہ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            واپس false;
        }
    }
    واپس true;
}

darray_سائز_t
xkb_context_عدد_failed_include_paths(ساخت xkb_context *ctx)
{
    واپس (xkb_context_init_includes(ctx))
        ? darray_سائز(ctx->failed_includes)
        : 0;
}

ثابت حرف *
xkb_context_failed_include_راستہ_get(ساخت xkb_context *ctx,
                                    darray_سائز_t idx)
{
    اگر (idx >= xkb_context_عدد_failed_include_paths(ctx))
        واپس تہی;

    واپس darray_شے(ctx->failed_includes, idx);
}

darray_سائز_t
xkb_atom_جدول_سائز(ساخت xkb_context *ctx)
{
    واپس atom_جدول_سائز(ctx->atom_جدول);
}

xkb_atom_t
xkb_atom_lookup(ساخت xkb_context *ctx, ثابت حرف *string)
{
    واپس atom_intern(ctx->atom_جدول, string, لڑی_لمبائی(string), false);
}

xkb_atom_t
xkb_atom_intern(ساخت xkb_context *ctx, ثابت حرف *string, سائز_t len)
{
    واپس atom_intern(ctx->atom_جدول, string, len, true);
}

ثابت حرف *
xkb_atom_متن(ساخت xkb_context *ctx, xkb_atom_t atom)
{
    واپس atom_متن(ctx->atom_جدول, atom);
}

خالی
xkb_log(ساخت xkb_context *ctx, شمار xkb_log_level level, صحیح verbosity,
        ثابت حرف *fmt, ...)
{
    va_list دلائل;

    اگر (ctx->log_level < level || ctx->log_verbosity < verbosity)
        واپس;

    va_آغاز(دلائل, fmt);
    ctx->log_fn(ctx, level, fmt, دلائل);
    va_انتہا(دلائل);
}

حرف *
xkb_context_get_بفر(ساخت xkb_context *ctx, سائز_t سائز)
{
    حرف *rtrn;

    اگر (سائز >= حجم(ctx->متن_بفر))
        واپس تہی;

    اگر (حجم(ctx->متن_بفر) - ctx->متن_اگلا <= سائز)
        ctx->متن_اگلا = 0;

    rtrn = &ctx->متن_بفر[ctx->متن_اگلا];
    ctx->متن_اگلا += سائز;

    واپس rtrn;
}

ساکن ثابت حرف *
xkb_context_get_default_قواعد(ساخت xkb_context *ctx)
{
    ثابت حرف *env = تہی;

    اگر (ctx->use_environment_نام)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    واپس env ? env : DEFAULT_XKB_قواعد;
}

ساکن ثابت حرف *
xkb_context_get_default_model(ساخت xkb_context *ctx)
{
    ثابت حرف *env = تہی;

    اگر (ctx->use_environment_نام)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    واپس env ? env : DEFAULT_XKB_MODEL;
}

ساکن ثابت حرف *
xkb_context_get_default_layout(ساخت xkb_context *ctx)
{
    ثابت حرف *env = تہی;

    اگر (ctx->use_environment_نام)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    واپس env ? env : DEFAULT_XKB_LAYOUT;
}

ساکن ثابت حرف *
xkb_context_get_default_variant(ساخت xkb_context *ctx)
{
    ثابت حرف *env = تہی;
    ثابت حرف *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    اگر (layout && ctx->use_environment_نام)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    واپس env ? env : DEFAULT_XKB_VARIANT;
}

ساکن ثابت حرف *
xkb_context_get_default_options(ساخت xkb_context *ctx)
{
    ثابت حرف *env = تہی;

    اگر (ctx->use_environment_نام)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    واپس env ? env : DEFAULT_XKB_OPTIONS;
}

شمار RMLVO
xkb_context_sanitize_قاعدہ_نام(ساخت xkb_context *ctx,
                                ساخت xkb_قاعدہ_نام *rmlvo)
{
    شمار RMLVO modified = 0;

    اگر (isempty(rmlvo->قواعد)) {
        rmlvo->قواعد = xkb_context_get_default_قواعد(ctx);
        modified |= RMLVO_قواعد;
    }
    اگر (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    اگر (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ثابت حرف * ثابت variant = xkb_context_get_default_variant(ctx);
        اگر (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_پیغام_NO_ID,
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
    اگر (rmlvo->options == تہی) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    واپس modified;
}
