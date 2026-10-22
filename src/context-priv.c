/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ضمن "config.h"

#ضمن <sys/types.h>
#ضمن <sys/stat.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "context.h"
#ضمن "rmlvo.h"
#ضمن "utils.h"

حرف *
xkb_context_getenv(بنية xkb_context *ctx, ثابت حرف *name)
{
    إذا (ctx->use_secure_getenv) {
        أرجع secure_getenv(name);
    } وإلا {
        أرجع getenv(name);
    }
}

bool
xkb_context_init_includes(بنية xkb_context *ctx)
{
    إذا (ctx->pending_default_includes) {
        إذا (darray_خالی(ctx->failed_includes)) {
            إذا (!xkb_context_include_مسیر_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_مسیر,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_مسیر_get_system_مسیر(ctx));
                أرجع false;
            }
            ctx->pending_default_includes = false;
        } وإلا {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            أرجع false;
        }
    }
    أرجع true;
}

darray_اندازه_t
xkb_context_رقم_failed_include_paths(بنية xkb_context *ctx)
{
    أرجع (xkb_context_init_includes(ctx))
        ? darray_اندازه(ctx->failed_includes)
        : 0;
}

ثابت حرف *
xkb_context_failed_include_مسیر_get(بنية xkb_context *ctx,
                                    darray_اندازه_t idx)
{
    إذا (idx >= xkb_context_رقم_failed_include_paths(ctx))
        أرجع عدم;

    أرجع darray_قلم(ctx->failed_includes, idx);
}

darray_اندازه_t
xkb_atom_جدول_اندازه(بنية xkb_context *ctx)
{
    أرجع atom_جدول_اندازه(ctx->atom_جدول);
}

xkb_atom_t
xkb_atom_lookup(بنية xkb_context *ctx, ثابت حرف *string)
{
    أرجع atom_intern(ctx->atom_جدول, string, طول_رشته(string), false);
}

xkb_atom_t
xkb_atom_intern(بنية xkb_context *ctx, ثابت حرف *string, اندازه_t len)
{
    أرجع atom_intern(ctx->atom_جدول, string, len, true);
}

ثابت حرف *
xkb_atom_متن(بنية xkb_context *ctx, xkb_atom_t atom)
{
    أرجع atom_متن(ctx->atom_جدول, atom);
}

فراغ
xkb_log(بنية xkb_context *ctx, تعداد xkb_log_level level, صحيح verbosity,
        ثابت حرف *fmt, ...)
{
    va_list آرگومانها;

    إذا (ctx->log_level < level || ctx->log_verbosity < verbosity)
        أرجع;

    va_شروع(آرگومانها, fmt);
    ctx->log_fn(ctx, level, fmt, آرگومانها);
    va_انتها(آرگومانها);
}

حرف *
xkb_context_get_میانگیر(بنية xkb_context *ctx, اندازه_t اندازه)
{
    حرف *rtrn;

    إذا (اندازه >= حجم(ctx->متن_میانگیر))
        أرجع عدم;

    إذا (حجم(ctx->متن_میانگیر) - ctx->متن_بعدی <= اندازه)
        ctx->متن_بعدی = 0;

    rtrn = &ctx->متن_میانگیر[ctx->متن_بعدی];
    ctx->متن_بعدی += اندازه;

    أرجع rtrn;
}

ساكن ثابت حرف *
xkb_context_get_default_قواعد(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    إذا (ctx->use_environment_نامها)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    أرجع env ? env : DEFAULT_XKB_قواعد;
}

ساكن ثابت حرف *
xkb_context_get_default_model(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    إذا (ctx->use_environment_نامها)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    أرجع env ? env : DEFAULT_XKB_MODEL;
}

ساكن ثابت حرف *
xkb_context_get_default_layout(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    إذا (ctx->use_environment_نامها)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    أرجع env ? env : DEFAULT_XKB_LAYOUT;
}

ساكن ثابت حرف *
xkb_context_get_default_variant(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;
    ثابت حرف *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    إذا (layout && ctx->use_environment_نامها)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    أرجع env ? env : DEFAULT_XKB_VARIANT;
}

ساكن ثابت حرف *
xkb_context_get_default_options(بنية xkb_context *ctx)
{
    ثابت حرف *env = عدم;

    إذا (ctx->use_environment_نامها)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    أرجع env ? env : DEFAULT_XKB_OPTIONS;
}

تعداد RMLVO
xkb_context_sanitize_قاعده_نامها(بنية xkb_context *ctx,
                                بنية xkb_قاعده_نامها *rmlvo)
{
    تعداد RMLVO modified = 0;

    إذا (isempty(rmlvo->قواعد)) {
        rmlvo->قواعد = xkb_context_get_default_قواعد(ctx);
        modified |= RMLVO_قواعد;
    }
    إذا (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    إذا (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        ثابت حرف * ثابت variant = xkb_context_get_default_variant(ctx);
        إذا (!isempty(rmlvo->variant)) {
            log_warn(ctx, XKB_LOG_پیام_NO_ID,
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
    إذا (rmlvo->options == عدم) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    أرجع modified;
}
