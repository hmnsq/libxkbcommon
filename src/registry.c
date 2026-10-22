/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <libxml/parser.h>

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #അന്തര്ഭാവ <limits.h>
    #എങ്കിൽ HAVE_UNISTD_H && HAVE_DIRENT_H
        #അന്തര്ഭാവ <dirent.h>
        #അന്തര്ഭാവ <unistd.h>
    #endif
#endif

#അന്തര്ഭാവ "xkbcommon/xkbregistry.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-list.h"
#അന്തര്ഭാവ "util-mem.h"

ഘടന rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ഘടന rxkb_object {
    ഘടന rxkb_object *parent;
    uint32_t refcount;
    ഘടന list link;
};

ഘടന rxkb_iso639_code {
    ഘടന rxkb_object base;
    അക്ഷരം *code;
};

ഘടന rxkb_iso3166_code {
    ഘടന rxkb_object base;
    അക്ഷരം *code;
};

എണ്ണൽ context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ഘടന rxkb_context {
    ഘടന rxkb_object base;
    എണ്ണൽ context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    ഘടന list models;         /* list of struct rxkb_models */
    ഘടന list layouts;        /* list of struct rxkb_layouts */
    ഘടന list option_groups;  /* list of struct rxkb_option_group */

    darray(അക്ഷരം *) includes;


    ATTR_PRINTF(3, 0) ശൂന്യം (*log_fn)(ഘടന rxkb_context *ctx,
                                     എണ്ണൽ rxkb_log_level level,
                                     സ്ഥിരം അക്ഷരം *fmt, va_list args);
    എണ്ണൽ rxkb_log_level log_level;

    ശൂന്യം *userdata;
};

ഘടന rxkb_model {
    ഘടന rxkb_object base;

    അക്ഷരം *name;
    അക്ഷരം *vendor;
    അക്ഷരം *description;
    എണ്ണൽ rxkb_popularity popularity;
};

ഘടന rxkb_layout {
    ഘടന rxkb_object base;

    അക്ഷരം *name;
    അക്ഷരം *brief;
    അക്ഷരം *description;
    അക്ഷരം *variant;
    എണ്ണൽ rxkb_popularity popularity;

    ഘടന list iso639s;  /* list of struct rxkb_iso639_code */
    ഘടന list iso3166s; /* list of struct rxkb_iso3166_code */
};

ഘടന rxkb_option_group {
    ഘടന rxkb_object base;

    bool allow_multiple;
    ഘടന list options; /* list of struct rxkb_options */
    അക്ഷരം *name;
    അക്ഷരം *description;
    എണ്ണൽ rxkb_popularity popularity;
};

ഘടന rxkb_option {
    ഘടന rxkb_object base;

    അക്ഷരം *name;
    അക്ഷരം *brief;
    അക്ഷരം *description;
    എണ്ണൽ rxkb_popularity popularity;
    bool layout_specific;
};

സ്ഥിതം bool
parse(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *path,
      എണ്ണൽ rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
സ്ഥിതം ശൂന്യം
rxkb_log(ഘടന rxkb_context *ctx, എണ്ണൽ rxkb_log_level level,
         സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list args;

    എങ്കിൽ (ctx->log_level < level)
        തിരികെ;

    va_start(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    va_end(args);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define rxkb_log_with_code(ctx, level, സന്ദേശം_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(സന്ദേശം_id, fmt), ##__VA_ARGS__)
#define log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define log_info(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#define log_warn(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#define log_err(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#define log_wsgo(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#define DECLARE_REF_UNREF_FOR_TYPE(type_) \
ഘടന type_ * type_##_ref(ഘടന type_ *object) { \
    rxkb_object_ref(&object->base); \
    തിരികെ object; \
} \
ഘടന type_ * type_##_unref(ഘടന type_ *object) { \
    എങ്കിൽ (!object) തിരികെ ശൂന്യ; \
    പ്രതിജ്ഞാ(object->base.refcount >= 1); \
    എങ്കിൽ (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        സ്വതന്ത്രം(object); \
    } \
    തിരികെ ശൂന്യ;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
സ്ഥിതം വരിയിൽ ഘടന type_ * type_##_create(ഘടന rxkb_object *parent) { \
    ഘടന type_ *t = calloc(1, വലുപ്പം *t); \
    എങ്കിൽ (t) \
        rxkb_object_init(&t->base, parent); \
    തിരികെ t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(ഘടന type_ *object) { \
    തിരികെ object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, സ്ഥിരം അക്ഷരം*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
ഘടന type_ * type_##_first(ഘടന parent_type_ *parent) { \
    ഘടന type_ *o = ശൂന്യ; \
    എങ്കിൽ (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    തിരികെ o; \
} \
ഘടന type_ * \
type_##_next(ഘടന type_ *o) \
{ \
    ഘടന parent_type_ *parent; \
    ഘടന type_ *next; \
    parent = container_of(o->base.parent, ഘടന parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    എങ്കിൽ (list_is_last(&parent->parent_field_, &o->base.link)) \
        തിരികെ ശൂന്യ; \
    തിരികെ next; \
}

സ്ഥിതം ശൂന്യം
rxkb_object_init(ഘടന rxkb_object *object, ഘടന rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

സ്ഥിതം ശൂന്യം *
rxkb_object_ref(ഘടന rxkb_object *object)
{
    പ്രതിജ്ഞാ(object->refcount >= 1);
    ++object->refcount;
    തിരികെ object;
}

സ്ഥിതം ശൂന്യം
rxkb_iso639_code_destroy(ഘടന rxkb_iso639_code *code)
{
    സ്വതന്ത്രം(code->code);
}

ഘടന rxkb_iso639_code *
rxkb_layout_get_iso639_first(ഘടന rxkb_layout *layout)
{
    ഘടന rxkb_iso639_code *code = ശൂന്യ;

    എങ്കിൽ (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    തിരികെ code;
}

ഘടന rxkb_iso639_code *
rxkb_iso639_code_next(ഘടന rxkb_iso639_code *code)
{
    ഘടന rxkb_iso639_code *next = ശൂന്യ;
    ഘടന rxkb_layout *layout;

    layout = container_of(code->base.parent, ഘടന rxkb_layout, base);

    എങ്കിൽ (list_is_last(&layout->iso639s, &code->base.link))
        തിരികെ ശൂന്യ;

    next = list_first_entry(&code->base.link, code, base.link);

    തിരികെ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

സ്ഥിതം ശൂന്യം
rxkb_iso3166_code_destroy(ഘടന rxkb_iso3166_code *code)
{
    സ്വതന്ത്രം(code->code);
}

ഘടന rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(ഘടന rxkb_layout *layout)
{
    ഘടന rxkb_iso3166_code *code = ശൂന്യ;

    എങ്കിൽ (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    തിരികെ code;
}

ഘടന rxkb_iso3166_code *
rxkb_iso3166_code_next(ഘടന rxkb_iso3166_code *code)
{
    ഘടന rxkb_iso3166_code *next = ശൂന്യ;
    ഘടന rxkb_layout *layout;

    layout = container_of(code->base.parent, ഘടന rxkb_layout, base);

    എങ്കിൽ (list_is_last(&layout->iso3166s, &code->base.link))
        തിരികെ ശൂന്യ;

    next = list_first_entry(&code->base.link, code, base.link);

    തിരികെ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

സ്ഥിതം ശൂന്യം
rxkb_option_destroy(ഘടന rxkb_option *o)
{
    സ്വതന്ത്രം(o->name);
    സ്വതന്ത്രം(o->brief);
    സ്വതന്ത്രം(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, എണ്ണൽ rxkb_popularity);
bool rxkb_option_is_layout_specific(ഘടന rxkb_option *object) {
    തിരികെ object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

സ്ഥിതം ശൂന്യം
rxkb_layout_destroy(ഘടന rxkb_layout *l)
{
    ഘടന rxkb_iso639_code *iso639, *tmp_639;
    ഘടന rxkb_iso3166_code *iso3166, *tmp_3166;

    സ്വതന്ത്രം(l->name);
    സ്വതന്ത്രം(l->brief);
    സ്വതന്ത്രം(l->description);
    സ്വതന്ത്രം(l->variant);

    list_for_each_safe(iso639, tmp_639, &l->iso639s, base.link) {
        rxkb_iso639_code_unref(iso639);
    }
    list_for_each_safe(iso3166, tmp_3166, &l->iso3166s, base.link) {
        rxkb_iso3166_code_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, എണ്ണൽ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

സ്ഥിതം ശൂന്യം
rxkb_model_destroy(ഘടന rxkb_model *m)
{
    സ്വതന്ത്രം(m->name);
    സ്വതന്ത്രം(m->vendor);
    സ്വതന്ത്രം(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, എണ്ണൽ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

സ്ഥിതം ശൂന്യം
rxkb_option_group_destroy(ഘടന rxkb_option_group *og)
{
    ഘടന rxkb_option *o, *otmp;

    സ്വതന്ത്രം(og->name);
    സ്വതന്ത്രം(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(ഘടന rxkb_option_group *g)
{
    തിരികെ g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, എണ്ണൽ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

സ്ഥിതം ശൂന്യം
rxkb_context_destroy(ഘടന rxkb_context *ctx)
{
    ഘടന rxkb_model *m, *mtmp;
    ഘടന rxkb_layout *l, *ltmp;
    ഘടന rxkb_option_group *og, *ogtmp;
    അക്ഷരം **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    പ്രതിജ്ഞാ(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    പ്രതിജ്ഞാ(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    പ്രതിജ്ഞാ(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        സ്വതന്ത്രം(*path);
    darray_free(ctx->includes);

    പ്രതിജ്ഞാ(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, എണ്ണൽ rxkb_log_level);

സ്ഥിതം അക്ഷരം *
rxkb_context_getenv(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *name)
{
    എങ്കിൽ (ctx->use_secure_getenv) {
        തിരികെ secure_getenv(name);
    } അല്ലെങ്കിൽ {
        തിരികെ getenv(name);
    }
}


ശൂന്യം
rxkb_context_set_log_level(ഘടന rxkb_context *ctx,
                           എണ്ണൽ rxkb_log_level level)
{
    ctx->log_level = level;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
log_level_to_prefix(എണ്ണൽ rxkb_log_level level)
{
    തിരഞ്ഞെടുപ്പ് (level) {
    അവസ്ഥ RXKB_LOG_LEVEL_DEBUG:
        തിരികെ "xkbregistry: DEBUG: ";
    അവസ്ഥ RXKB_LOG_LEVEL_INFO:
        തിരികെ "xkbregistry: INFO: ";
    അവസ്ഥ RXKB_LOG_LEVEL_WARNING:
        തിരികെ "xkbregistry: WARNING: ";
    അവസ്ഥ RXKB_LOG_LEVEL_ERROR:
        തിരികെ "xkbregistry: ERROR: ";
    അവസ്ഥ RXKB_LOG_LEVEL_CRITICAL:
        തിരികെ "xkbregistry: CRITICAL: ";
    സ്ഥിരസ്ഥിതി:
        തിരികെ ശൂന്യ;
    }
}

ATTR_PRINTF(3, 0) സ്ഥിതം ശൂന്യം
default_log_fn(ഘടന rxkb_context *ctx, എണ്ണൽ rxkb_log_level level,
               സ്ഥിരം അക്ഷരം *fmt, va_list args)
{
    സ്ഥിരം അക്ഷരം *prefix = log_level_to_prefix(level);

    എങ്കിൽ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

സ്ഥിതം എണ്ണൽ rxkb_log_level
log_level(സ്ഥിരം അക്ഷരം *level) {
    errno = 0;
    അക്ഷരം *endptr;
    സ്ഥിരം long lvl = strtol(level, &endptr, 10);
    എങ്കിൽ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        തിരികെ (എണ്ണൽ rxkb_log_level)lvl;

    എങ്കിൽ (istreq_prefix("crit", level))
        തിരികെ RXKB_LOG_LEVEL_CRITICAL;
    എങ്കിൽ (istreq_prefix("err", level))
        തിരികെ RXKB_LOG_LEVEL_ERROR;
    എങ്കിൽ (istreq_prefix("warn", level))
        തിരികെ RXKB_LOG_LEVEL_WARNING;
    എങ്കിൽ (istreq_prefix("info", level))
        തിരികെ RXKB_LOG_LEVEL_INFO;
    എങ്കിൽ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        തിരികെ RXKB_LOG_LEVEL_DEBUG;

    തിരികെ RXKB_LOG_LEVEL_ERROR;
}

ഘടന rxkb_context *
rxkb_context_new(എണ്ണൽ rxkb_context_flags flags)
{
    ഘടന rxkb_context *ctx = rxkb_context_create(ശൂന്യ);
    സ്ഥിരം അക്ഷരം *env;

    എങ്കിൽ (!ctx)
        തിരികെ ശൂന്യ;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    എങ്കിൽ (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    സ്ഥിതം സ്ഥിരം എണ്ണൽ rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    എങ്കിൽ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        സ്വതന്ത്രം(ctx);
        തിരികെ ശൂന്യ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    എങ്കിൽ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_വേര്);
        rxkb_context_unref(ctx);
        തിരികെ ശൂന്യ;
    }

    തിരികെ ctx;
}

ശൂന്യം
rxkb_context_set_log_fn(ഘടന rxkb_context *ctx,
                        ശൂന്യം (*log_fn)(ഘടന rxkb_context *ctx,
                                       എണ്ണൽ rxkb_log_level level,
                                       സ്ഥിരം അക്ഷരം *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *path)
{
    പൂർണ്ണസംഖ്യ err = 0;

    എങ്കിൽ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        പോകുക error;
    }

    ഘടന stat stat_buf;
    err = stat(path, &stat_buf);
    എങ്കിൽ (err != 0) {
        err = errno;
        പോകുക error;
    }
    എങ്കിൽ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        പോകുക error;
    }

    എങ്കിൽ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        പോകുക error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    അക്ഷരം rules[PATH_MAX];
    എങ്കിൽ (!snprintf_safe(rules, വലുപ്പം(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                വലുപ്പം(rules), path, DEFAULT_XKB_RULES);
        പോകുക error;
    }

    അക്ഷരം * സ്ഥിരം tmp = strdup(path);
    എങ്കിൽ (!tmp) {
        err = ENOMEM;
        പോകുക error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    തിരികെ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    തിരികെ false;
}

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_str(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    തിരികെ strcmp(*(അക്ഷരം **)a, *(അക്ഷരം **) b);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
add_direct_subdirectories(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *path,
                          darray_string *extensions,
                          darray_size_t versioned_എണ്ണം,
                          size_t versioned_path_length)
{
    പൂർണ്ണസംഖ്യ ret = 0;
    പൂർണ്ണസംഖ്യ err = ENOMEM;
    DIR *dir = ശൂന്യ;

    /* Check extensions parent directory */
    ഘടന stat stat_buf;
    err = stat(path, &stat_buf);
    എങ്കിൽ (err != 0) {
        err = errno;
        പോകുക err;
    }
    എങ്കിൽ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        പോകുക err;
    }
    എങ്കിൽ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        പോകുക err;
    }

    dir = opendir(path);
    എങ്കിൽ (dir == ശൂന്യ) {
        err = EACCES;
        പോകുക err;
    }

    ഘടന dirent *entry;
    അക്ഷരം path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ആകുമ്പോൾ ((entry = readdir(dir)) != ശൂന്യ) {
        സ്ഥിരം അക്ഷരം *name = entry->d_name;

        /* Skip special entries */
        എങ്കിൽ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            തുടരുക;

        എങ്കിൽ (!snprintf_safe(path_buf, വലുപ്പം(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            പോകുക err;
        }
        എങ്കിൽ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            തുടരുക;

        /* Skip if the corresponding versioned directory is already included */
        വേണ്ടി (darray_size_t i = 0; i < versioned_എണ്ണം; i++) {
            സ്ഥിരം അക്ഷരം * സ്ഥിരം prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            എങ്കിൽ (strcmp(name, prev_name) == 0)
                പോകുക next;
        }

        അക്ഷരം *ext_path = strdup_safe(path_buf);
        എങ്കിൽ (!ext_path) {
            err = ENOMEM;
            പോകുക err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    എങ്കിൽ (darray_size(*extensions) > versioned_എണ്ണം) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ഇനങ്ങൾ(*extensions) + versioned_എണ്ണം,
              darray_size(*extensions) - versioned_എണ്ണം,
              വലുപ്പം(*darray_ഇനങ്ങൾ(*extensions)), &compare_str);
        അക്ഷരം **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_എണ്ണം) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    തിരികെ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    എങ്കിൽ (dir)
        closedir(dir);

    തിരികെ ret;
}
#endif

bool
rxkb_context_include_path_append_default(ഘടന rxkb_context *ctx)
{
    അക്ഷരം user_path[PATH_MAX];
    bool ret = false;

    എങ്കിൽ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        തിരികെ false;
    }

    സ്ഥിരം അക്ഷരം * സ്ഥിരം home = rxkb_context_getenv(ctx, "HOME");
    സ്ഥിരം അക്ഷരം * സ്ഥിരം xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    എങ്കിൽ (xdg != ശൂന്യ) {
        എങ്കിൽ (snprintf_safe(user_path, വലുപ്പം(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } അല്ലെങ്കിൽ എങ്കിൽ (home != ശൂന്യ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        എങ്കിൽ (snprintf_safe(user_path, വലുപ്പം(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    എങ്കിൽ (home != ശൂന്യ) {
        എങ്കിൽ (snprintf_safe(user_path, വലുപ്പം(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    സ്ഥിരം അക്ഷരം * സ്ഥിരം extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != ശൂന്യ) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#എങ്കിൽ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    സ്ഥിരം അക്ഷരം *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    എങ്കിൽ (extensions_path == ശൂന്യ)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    എങ്കിൽ (extensions_path != ശൂന്യ) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    എങ്കിൽ (extensions_path == ശൂന്യ)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    എങ്കിൽ (extensions_path != ശൂന്യ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    അക്ഷരം **ext_path;
    darray_foreach(ext_path, extensions) {
        സ്വതന്ത്രം(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    സ്ഥിരം അക്ഷരം * സ്ഥിരം വേര് = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    സ്ഥിരം bool has_വേര് = rxkb_context_include_path_append(
        ctx, ((വേര് != ശൂന്യ) ? വേര്: DFLT_XKB_CONFIG_വേര്)
    );
    ret |= has_വേര്;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    എങ്കിൽ (!has_വേര് && (വേര് == ശൂന്യ || വേര്[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_വേര്,
                 ((വേര് == ശൂന്യ) ? DFLT_XKB_CONFIG_വേര് : വേര്));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_വേര്);
    }

    തിരികെ ret;
}

bool
rxkb_context_parse_default_ruleset(ഘടന rxkb_context *ctx)
{
    തിരികെ rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *ruleset)
{
    അക്ഷരം **path;
    bool success = false;

    എങ്കിൽ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        തിരികെ false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        അക്ഷരം rules[PATH_MAX];

        എങ്കിൽ (snprintf_safe(rules, വലുപ്പം(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            എങ്കിൽ (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        എങ്കിൽ (ctx->load_extra_rules_files &&
            snprintf_safe(rules, വലുപ്പം(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            എങ്കിൽ (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    തിരികെ success;
}


ശൂന്യം
rxkb_context_set_user_data(ഘടന rxkb_context *ctx, ശൂന്യം *userdata)
{
    ctx->userdata = userdata;
}

ശൂന്യം *
rxkb_context_get_user_data(ഘടന rxkb_context *ctx)
{
    തിരികെ ctx->userdata;
}

സ്ഥിതം വരിയിൽ bool
is_കണ്ണി(xml_കണ്ണി *കണ്ണി, സ്ഥിരം അക്ഷരം *name)
{
    തിരികെ കണ്ണി->type == XML_ELEMENT_കണ്ണി &&
        xmlStrEqual(കണ്ണി->name, (സ്ഥിരം xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
സ്ഥിതം അക്ഷരം *
extract_text(xml_കണ്ണി *കണ്ണി)
{
    xml_കണ്ണി *n;

    വേണ്ടി (n = കണ്ണി->children; n; n = n->next) {
        എങ്കിൽ (n->type == XML_TEXT_കണ്ണി)
            തിരികെ (അക്ഷരം *)xmlStrdup(n->content);
    }
    തിരികെ ശൂന്യ;
}

/* Data from “configItem” node */
ഘടന config_item {
    അക്ഷരം *name;
    അക്ഷരം *description;
    അക്ഷരം *brief;
    അക്ഷരം *vendor;
    എണ്ണൽ rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = ശൂന്യ, \
    .description = ശൂന്യ, \
    .brief = ശൂന്യ, \
    .vendor = ശൂന്യ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

സ്ഥിതം ശൂന്യം
config_item_free(ഘടന config_item *config) {
    സ്വതന്ത്രം(config->name);
    സ്വതന്ത്രം(config->description);
    സ്വതന്ത്രം(config->brief);
    സ്വതന്ത്രം(config->vendor);
}

സ്ഥിതം bool
parse_config_item(ഘടന rxkb_context *ctx, xml_കണ്ണി *parent,
                  ഘടന config_item *config)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;
    xml_കണ്ണി *ci = ശൂന്യ;

    വേണ്ടി (ci = parent->children; ci; ci = ci->next) {
        എങ്കിൽ (is_കണ്ണി(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (സ്ഥിരം xmlChar*)"popularity");
            എങ്കിൽ (raw_popularity) {
                എങ്കിൽ (xmlStrEqual(raw_popularity, (സ്ഥിരം xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                അല്ലെങ്കിൽ എങ്കിൽ (xmlStrEqual(raw_popularity, (സ്ഥിരം xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                അല്ലെങ്കിൽ
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->വരി, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (സ്ഥിരം xmlChar*)"layout-specific");
            എങ്കിൽ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (സ്ഥിരം xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            വേണ്ടി (കണ്ണി = ci->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
                എങ്കിൽ (is_കണ്ണി(കണ്ണി, "name"))
                    config->name = extract_text(കണ്ണി);
                അല്ലെങ്കിൽ എങ്കിൽ (is_കണ്ണി(കണ്ണി, "description"))
                    config->description = extract_text(കണ്ണി);
                അല്ലെങ്കിൽ എങ്കിൽ (is_കണ്ണി(കണ്ണി, "shortDescription"))
                    config->brief = extract_text(കണ്ണി);
                അല്ലെങ്കിൽ എങ്കിൽ (is_കണ്ണി(കണ്ണി, "vendor"))
                    config->vendor = extract_text(കണ്ണി);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            എങ്കിൽ (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->വരി);
                config_item_free(config);
                തിരികെ false;
            }

            തിരികെ true; /* only one configItem allowed in the dtd */
        }
    }

    തിരികെ false;
}

സ്ഥിതം ശൂന്യം
parse_model(ഘടന rxkb_context *ctx, xml_കണ്ണി *model,
            എണ്ണൽ rxkb_popularity popularity)
{
    ഘടന config_item config = config_item_new(popularity);

    എങ്കിൽ (parse_config_item(ctx, model, &config)) {
        ഘടന rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            എങ്കിൽ (streq(m->name, config.name)) {
                config_item_free(&config);
                തിരികെ;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->base);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->base.link);
    }
}

സ്ഥിതം ശൂന്യം
parse_model_list(ഘടന rxkb_context *ctx, xml_കണ്ണി *model_list,
                 എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;

    വേണ്ടി (കണ്ണി = model_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "model"))
            parse_model(ctx, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
parse_language_list(xml_കണ്ണി *language_list, ഘടന rxkb_layout *layout)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;
    ഘടന rxkb_iso639_code *code;

    വേണ്ടി (കണ്ണി = language_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "iso639Id")) {
            അക്ഷരം *str = extract_text(കണ്ണി);
            ഘടന rxkb_object *parent;

            എങ്കിൽ (!str || strlen(str) != 3) {
                സ്വതന്ത്രം(str);
                തുടരുക;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

സ്ഥിതം ശൂന്യം
parse_country_list(xml_കണ്ണി *country_list, ഘടന rxkb_layout *layout)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;
    ഘടന rxkb_iso3166_code *code;

    വേണ്ടി (കണ്ണി = country_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "iso3166Id")) {
            അക്ഷരം *str = extract_text(കണ്ണി);
            ഘടന rxkb_object *parent;

            എങ്കിൽ (!str || strlen(str) != 2) {
                സ്വതന്ത്രം(str);
                തുടരുക;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

സ്ഥിതം ശൂന്യം
parse_variant(ഘടന rxkb_context *ctx, ഘടന rxkb_layout *l,
              xml_കണ്ണി *variant, എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *ci;
    ഘടന config_item config = config_item_new(popularity);

    എങ്കിൽ (parse_config_item(ctx, variant, &config)) {
        ഘടന rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            എങ്കിൽ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                നിർത്തുക;
            }
        }

        എങ്കിൽ (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ശൂന്യ ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            വേണ്ടി (ci = variant->children; ci; ci = ci->next) {
                xml_കണ്ണി *കണ്ണി;

                എങ്കിൽ (!is_കണ്ണി(ci, "configItem"))
                    തുടരുക;

                bool found_language_list = false;
                bool found_country_list = false;
                വേണ്ടി (കണ്ണി = ci->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
                    എങ്കിൽ (is_കണ്ണി(കണ്ണി, "languageList")) {
                        parse_language_list(കണ്ണി, v);
                        found_language_list = true;
                    }
                    എങ്കിൽ (is_കണ്ണി(കണ്ണി, "countryList")) {
                        parse_country_list(കണ്ണി, v);
                        found_country_list = true;
                    }
                }
                എങ്കിൽ (!found_language_list) {
                    // inherit from parent layout
                    ഘടന rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        ഘടന rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                എങ്കിൽ (!found_country_list) {
                    // inherit from parent layout
                    ഘടന rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        ഘടന rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } അല്ലെങ്കിൽ {
            config_item_free(&config);
        }
    }
}

സ്ഥിതം ശൂന്യം
parse_variant_list(ഘടന rxkb_context *ctx, ഘടന rxkb_layout *l,
                   xml_കണ്ണി *variant_list, എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;

    വേണ്ടി (കണ്ണി = variant_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "variant"))
            parse_variant(ctx, l, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
parse_layout(ഘടന rxkb_context *ctx, xml_കണ്ണി *layout,
             എണ്ണൽ rxkb_popularity popularity)
{
    ഘടന config_item config = config_item_new(popularity);
    ഘടന rxkb_layout *l;
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;
    bool exists = false;

    എങ്കിൽ (!parse_config_item(ctx, layout, &config))
        തിരികെ;

    list_for_each(l, &ctx->layouts, base.link) {
        എങ്കിൽ (streq(l->name, config.name) && l->variant == ശൂന്യ) {
            exists = true;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ശൂന്യ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } അല്ലെങ്കിൽ {
        config_item_free(&config);
    }

    വേണ്ടി (കണ്ണി = layout->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "variantList")) {
            parse_variant_list(ctx, l, കണ്ണി, popularity);
        }
        എങ്കിൽ (!exists && is_കണ്ണി(കണ്ണി, "configItem")) {
            xml_കണ്ണി *ll;
            വേണ്ടി (ll = കണ്ണി->children; ll; ll = ll->next) {
                എങ്കിൽ (is_കണ്ണി(ll, "languageList"))
                    parse_language_list(ll, l);
                എങ്കിൽ (is_കണ്ണി(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

സ്ഥിതം ശൂന്യം
parse_layout_list(ഘടന rxkb_context *ctx, xml_കണ്ണി *layout_list,
                  എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;

    വേണ്ടി (കണ്ണി = layout_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "layout"))
            parse_layout(ctx, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
parse_option(ഘടന rxkb_context *ctx, ഘടന rxkb_option_group *group,
             xml_കണ്ണി *option, എണ്ണൽ rxkb_popularity popularity)
{
    ഘടന config_item config = config_item_new(popularity);

    എങ്കിൽ (parse_config_item(ctx, option, &config)) {
        ഘടന rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            എങ്കിൽ (streq(o->name, config.name)) {
                config_item_free(&config);
                തിരികെ;
            }
        }

        o = rxkb_option_create(&group->base);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&group->options, &o->base.link);
    }
}

സ്ഥിതം ശൂന്യം
parse_group(ഘടന rxkb_context *ctx, xml_കണ്ണി *group,
            എണ്ണൽ rxkb_popularity popularity)
{
    ഘടന config_item config = config_item_new(popularity);
    ഘടന rxkb_option_group *g;
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;
    xmlChar *multiple;
    bool exists = false;

    എങ്കിൽ (!parse_config_item(ctx, group, &config))
        തിരികെ;

    list_for_each(g, &ctx->option_groups, base.link) {
        എങ്കിൽ (streq(g->name, config.name)) {
            exists = true;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (സ്ഥിരം xmlChar*)"allowMultipleSelection");
        എങ്കിൽ (multiple && xmlStrEqual(multiple, (സ്ഥിരം xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } അല്ലെങ്കിൽ {
        config_item_free(&config);
    }

    വേണ്ടി (കണ്ണി = group->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "option"))
            parse_option(ctx, g, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
parse_option_list(ഘടന rxkb_context *ctx, xml_കണ്ണി *option_list,
                  എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;

    വേണ്ടി (കണ്ണി = option_list->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "group"))
            parse_group(ctx, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
parse_rules_xml(ഘടന rxkb_context *ctx, xml_കണ്ണി *വേര്,
                എണ്ണൽ rxkb_popularity popularity)
{
    xml_കണ്ണി *കണ്ണി = ശൂന്യ;

    വേണ്ടി (കണ്ണി = വേര്->children; കണ്ണി; കണ്ണി = കണ്ണി->next) {
        എങ്കിൽ (is_കണ്ണി(കണ്ണി, "modelList"))
            parse_model_list(ctx, കണ്ണി, popularity);
        അല്ലെങ്കിൽ എങ്കിൽ (is_കണ്ണി(കണ്ണി, "layoutList"))
            parse_layout_list(ctx, കണ്ണി, popularity);
        അല്ലെങ്കിൽ എങ്കിൽ (is_കണ്ണി(കണ്ണി, "optionList"))
            parse_option_list(ctx, കണ്ണി, popularity);
    }
}

സ്ഥിതം ശൂന്യം
ATTR_PRINTF(2, 0)
xml_error_func(ശൂന്യം *ctx, സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
    സ്ഥിതം അക്ഷരം buf[PATH_MAX];
    സ്ഥിതം size_t slen = 0;
    va_list args;
    പൂർണ്ണസംഖ്യ rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, സന്ദേശം);
    rc = vsnprintf(&buf[slen], വലുപ്പം(buf) - slen, സന്ദേശം, args);
    va_end(args);

    /* This shouldn't really happen */
    എങ്കിൽ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, വലുപ്പം(buf));
        തിരികെ;
    }

    slen += (size_t)rc;
    എങ്കിൽ (slen >= വലുപ്പം(buf)) {
        /* truncated, let's flush this */
        buf[വലുപ്പം(buf) - 1] = '\n';
        slen = വലുപ്പം(buf);
    }

    /* We're assuming here that the last character is \n. */
    എങ്കിൽ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, വലുപ്പം(buf));
        slen = 0;
    }
}

#എങ്കിൽ HAVE_XML_CTXT_SET_ERRORHANDLER
സ്ഥിതം ശൂന്യം
xml_structured_error_func(ശൂന്യം *userData, സ്ഥിരം xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#അല്ലെങ്കിൽ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

സ്ഥിതം bool
validate(ഘടന rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    സ്ഥിരം അക്ഷരം dtdstr[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<!ELEMENT xkbConfigRegistry (modelList?, layoutList?, optionList?)>\n"
        "<!ATTLIST xkbConfigRegistry version CDATA \"1.1\">\n"
        "<!ELEMENT modelList (model*)>\n"
        "<!ELEMENT model (configItem)>\n"
        "<!ELEMENT layoutList (layout*)>\n"
        "<!ELEMENT layout (configItem,  variantList?)>\n"
        "<!ELEMENT optionList (group*)>\n"
        "<!ELEMENT variantList (variant*)>\n"
        "<!ELEMENT variant (configItem)>\n"
        "<!ELEMENT group (configItem, option*)>\n"
        "<!ATTLIST group allowMultipleSelection (true|false) \"false\">\n"
        "<!ELEMENT option (configItem)>\n"
        "<!ELEMENT configItem (name, shortDescription?, description?, vendor?, countryList?, languageList?, hwList?)>\n"
        "<!ATTLIST configItem layout-specific (true|false) \"false\">\n"
        "<!ATTLIST configItem popularity (standard|exotic) #IMPLIED>\n"
        "<!ELEMENT name (#PCDATA)>\n"
        "<!ELEMENT shortDescription (#PCDATA)>\n"
        "<!ELEMENT description (#PCDATA)>\n"
        "<!ELEMENT vendor (#PCDATA)>\n"
        "<!ELEMENT countryList (iso3166Id+)>\n"
        "<!ELEMENT iso3166Id (#PCDATA)>\n"
        "<!ELEMENT languageList (iso639Id+)>\n"
        "<!ELEMENT iso639Id (#PCDATA)>\n"
        "<!ELEMENT hwList (hwId+)>\n"
        "<!ELEMENT hwId (#PCDATA)>\n";

#എങ്കിൽ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    എങ്കിൽ (!xmlCtxt)
        തിരികെ false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_നിവേശം_Ptr pinput =
        xml_New_നിവേശം_From_Memory(ശൂന്യ, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_നിവേശം_BUF_STATIC);
    എങ്കിൽ (!pinput)
        പോകുക dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ശൂന്യ, ശൂന്യ);
#അല്ലെങ്കിൽ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_നിവേശം_Buffer_Ptr buf =
        xml_Parser_നിവേശം_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    എങ്കിൽ (!buf)
        പോകുക dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ശൂന്യ, buf, XML_CHAR_ENCODING_UTF8);
#endif

    എങ്കിൽ (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        പോകുക dtd_error;
    }

#എങ്കിൽ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#അല്ലെങ്കിൽ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    എങ്കിൽ (!dtdvalid) {
        success = false;
        പോകുക ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#എങ്കിൽ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    തിരികെ success;
}

സ്ഥിതം bool
parse(ഘടന rxkb_context *ctx, സ്ഥിരം അക്ഷരം *path,
      എണ്ണൽ rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ശൂന്യ;
    xml_കണ്ണി *വേര് = ശൂന്യ;

    എങ്കിൽ (!check_eaccess(path, R_OK))
        തിരികെ false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    എങ്കിൽ (!xmlCtxt)
        തിരികെ false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#അല്ലെങ്കിൽ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#എങ്കിൽ HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#എങ്കിൽ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, ശൂന്യ, 0);
    എങ്കിൽ (!doc)
        പോകുക parse_error;

    എങ്കിൽ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        പോകുക validate_error;
    }

    വേര് = xml_Doc_Get_വേര്_Element(doc);
    parse_rules_xml(ctx, വേര്, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#എങ്കിൽ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(ശൂന്യ, ശൂന്യ);
#endif
#എങ്കിൽ HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, ശൂന്യ, ശൂന്യ);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    തിരികെ success;
}
