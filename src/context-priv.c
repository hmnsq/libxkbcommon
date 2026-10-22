/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "rmlvo.h"
#അന്തര്ഭാവ "utils.h"

അക്ഷരം *
xkb_context_getenv(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *name)
{
    എങ്കിൽ (ctx->use_secure_getenv) {
        തിരികെ secure_getenv(name);
    } അല്ലെങ്കിൽ {
        തിരികെ getenv(name);
    }
}

bool
xkb_context_init_includes(ഘടന xkb_context *ctx)
{
    എങ്കിൽ (ctx->pending_default_includes) {
        എങ്കിൽ (darray_empty(ctx->failed_includes)) {
            എങ്കിൽ (!xkb_context_include_path_append_default(ctx)) {
                log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                        "Failed to add any default include path "
                        "(system path: %s)\n",
                        xkb_context_include_path_get_system_path(ctx));
                തിരികെ false;
            }
            ctx->pending_default_includes = false;
        } അല്ലെങ്കിൽ {
            /*
             * If there are failed includes then we already tried to load
             * default include paths, so avoid further attempts.
             */
            തിരികെ false;
        }
    }
    തിരികെ true;
}

darray_size_t
xkb_context_അക്കം_failed_include_paths(ഘടന xkb_context *ctx)
{
    തിരികെ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->failed_includes)
        : 0;
}

സ്ഥിരം അക്ഷരം *
xkb_context_failed_include_path_get(ഘടന xkb_context *ctx,
                                    darray_size_t idx)
{
    എങ്കിൽ (idx >= xkb_context_അക്കം_failed_include_paths(ctx))
        തിരികെ ശൂന്യ;

    തിരികെ darray_item(ctx->failed_includes, idx);
}

darray_size_t
xkb_atom_table_size(ഘടന xkb_context *ctx)
{
    തിരികെ atom_table_size(ctx->atom_table);
}

xkb_atom_t
xkb_atom_lookup(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string)
{
    തിരികെ atom_intern(ctx->atom_table, string, strlen(string), false);
}

xkb_atom_t
xkb_atom_intern(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string, size_t len)
{
    തിരികെ atom_intern(ctx->atom_table, string, len, true);
}

സ്ഥിരം അക്ഷരം *
xkb_atom_text(ഘടന xkb_context *ctx, xkb_atom_t atom)
{
    തിരികെ atom_text(ctx->atom_table, atom);
}

ശൂന്യം
xkb_log(ഘടന xkb_context *ctx, എണ്ണൽ xkb_log_level level, പൂർണ്ണസംഖ്യ verbosity,
        സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list args;

    എങ്കിൽ (ctx->log_level < level || ctx->log_verbosity < verbosity)
        തിരികെ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

അക്ഷരം *
xkb_context_get_buffer(ഘടന xkb_context *ctx, size_t size)
{
    അക്ഷരം *rtrn;

    എങ്കിൽ (size >= വലുപ്പം(ctx->text_buffer))
        തിരികെ ശൂന്യ;

    എങ്കിൽ (വലുപ്പം(ctx->text_buffer) - ctx->text_next <= size)
        ctx->text_next = 0;

    rtrn = &ctx->text_buffer[ctx->text_next];
    ctx->text_next += size;

    തിരികെ rtrn;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
xkb_context_get_default_rules(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *env = ശൂന്യ;

    എങ്കിൽ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_RULES");

    തിരികെ env ? env : DEFAULT_XKB_RULES;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
xkb_context_get_default_model(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *env = ശൂന്യ;

    എങ്കിൽ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_MODEL");

    തിരികെ env ? env : DEFAULT_XKB_MODEL;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
xkb_context_get_default_layout(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *env = ശൂന്യ;

    എങ്കിൽ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    തിരികെ env ? env : DEFAULT_XKB_LAYOUT;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
xkb_context_get_default_variant(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *env = ശൂന്യ;
    സ്ഥിരം അക്ഷരം *layout = xkb_context_getenv(ctx, "XKB_DEFAULT_LAYOUT");

    /* We don't want to inherit the variant if they haven't also set a
     * layout, since they're so closely paired. */
    എങ്കിൽ (layout && ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_VARIANT");

    തിരികെ env ? env : DEFAULT_XKB_VARIANT;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
xkb_context_get_default_options(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *env = ശൂന്യ;

    എങ്കിൽ (ctx->use_environment_names)
        env = xkb_context_getenv(ctx, "XKB_DEFAULT_OPTIONS");

    തിരികെ env ? env : DEFAULT_XKB_OPTIONS;
}

എണ്ണൽ RMLVO
xkb_context_sanitize_rule_names(ഘടന xkb_context *ctx,
                                ഘടന xkb_rule_names *rmlvo)
{
    എണ്ണൽ RMLVO modified = 0;

    എങ്കിൽ (isempty(rmlvo->rules)) {
        rmlvo->rules = xkb_context_get_default_rules(ctx);
        modified |= RMLVO_RULES;
    }
    എങ്കിൽ (isempty(rmlvo->model)) {
        rmlvo->model = xkb_context_get_default_model(ctx);
        modified |= RMLVO_MODEL;
    }
    /* Layout and variant are tied together, so don't try to use one from
     * the caller and one from the environment. */
    എങ്കിൽ (isempty(rmlvo->layout)) {
        rmlvo->layout = xkb_context_get_default_layout(ctx);
        modified |= RMLVO_LAYOUT;
        സ്ഥിരം അക്ഷരം * സ്ഥിരം variant = xkb_context_get_default_variant(ctx);
        എങ്കിൽ (!isempty(rmlvo->variant)) {
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
    എങ്കിൽ (rmlvo->options == ശൂന്യ) {
        rmlvo->options = xkb_context_get_default_options(ctx);
        modified |= RMLVO_OPTIONS;
    }

    തിരികെ modified;
}
