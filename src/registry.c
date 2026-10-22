/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <libxml/parser.h>

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES
    #அந்தர்பாவ <limits.h>
    #எனில் HAVE_UNISTD_H && HAVE_DIRENT_H
        #அந்தர்பாவ <dirent.h>
        #அந்தர்பாவ <unistd.h>
    #endif
#endif

#அந்தர்பாவ "xkbcommon/xkbregistry.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-list.h"
#அந்தர்பாவ "util-mem.h"

கட்டமைப்பு rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
கட்டமைப்பு rxkb_object {
    கட்டமைப்பு rxkb_object *parent;
    uint32_t refcount;
    கட்டமைப்பு list link;
};

கட்டமைப்பு rxkb_iso639_code {
    கட்டமைப்பு rxkb_object base;
    எழுத்து *code;
};

கட்டமைப்பு rxkb_iso3166_code {
    கட்டமைப்பு rxkb_object base;
    எழுத்து *code;
};

எண்ணிக்கை context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

கட்டமைப்பு rxkb_context {
    கட்டமைப்பு rxkb_object base;
    எண்ணிக்கை context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    கட்டமைப்பு list models;         /* list of struct rxkb_models */
    கட்டமைப்பு list layouts;        /* list of struct rxkb_layouts */
    கட்டமைப்பு list option_groups;  /* list of struct rxkb_option_group */

    darray(எழுத்து *) includes;


    ATTR_PRINTF(3, 0) வெறுமை (*log_fn)(கட்டமைப்பு rxkb_context *ctx,
                                     எண்ணிக்கை rxkb_log_level level,
                                     மாறிலி எழுத்து *fmt, va_list args);
    எண்ணிக்கை rxkb_log_level log_level;

    வெறுமை *userdata;
};

கட்டமைப்பு rxkb_model {
    கட்டமைப்பு rxkb_object base;

    எழுத்து *name;
    எழுத்து *vendor;
    எழுத்து *description;
    எண்ணிக்கை rxkb_popularity popularity;
};

கட்டமைப்பு rxkb_layout {
    கட்டமைப்பு rxkb_object base;

    எழுத்து *name;
    எழுத்து *brief;
    எழுத்து *description;
    எழுத்து *variant;
    எண்ணிக்கை rxkb_popularity popularity;

    கட்டமைப்பு list iso639s;  /* list of struct rxkb_iso639_code */
    கட்டமைப்பு list iso3166s; /* list of struct rxkb_iso3166_code */
};

கட்டமைப்பு rxkb_option_group {
    கட்டமைப்பு rxkb_object base;

    bool allow_multiple;
    கட்டமைப்பு list options; /* list of struct rxkb_options */
    எழுத்து *name;
    எழுத்து *description;
    எண்ணிக்கை rxkb_popularity popularity;
};

கட்டமைப்பு rxkb_option {
    கட்டமைப்பு rxkb_object base;

    எழுத்து *name;
    எழுத்து *brief;
    எழுத்து *description;
    எண்ணிக்கை rxkb_popularity popularity;
    bool layout_specific;
};

நிலையான bool
parse(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *path,
      எண்ணிக்கை rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
நிலையான வெறுமை
rxkb_log(கட்டமைப்பு rxkb_context *ctx, எண்ணிக்கை rxkb_log_level level,
         மாறிலி எழுத்து *fmt, ...)
{
    va_list args;

    எனில் (ctx->log_level < level)
        திரும்பு;

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
#define rxkb_log_with_code(ctx, level, செய்தி_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(செய்தி_id, fmt), ##__VA_ARGS__)
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
கட்டமைப்பு type_ * type_##_ref(கட்டமைப்பு type_ *object) { \
    rxkb_object_ref(&object->base); \
    திரும்பு object; \
} \
கட்டமைப்பு type_ * type_##_unref(கட்டமைப்பு type_ *object) { \
    எனில் (!object) திரும்பு வெற்று; \
    ப்ரதிஜ்ஞா(object->base.refcount >= 1); \
    எனில் (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        விடுவி(object); \
    } \
    திரும்பு வெற்று;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
நிலையான உள்வரி கட்டமைப்பு type_ * type_##_create(கட்டமைப்பு rxkb_object *parent) { \
    கட்டமைப்பு type_ *t = calloc(1, அளவு *t); \
    எனில் (t) \
        rxkb_object_init(&t->base, parent); \
    திரும்பு t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(கட்டமைப்பு type_ *object) { \
    திரும்பு object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, மாறிலி எழுத்து*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
கட்டமைப்பு type_ * type_##_first(கட்டமைப்பு parent_type_ *parent) { \
    கட்டமைப்பு type_ *o = வெற்று; \
    எனில் (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    திரும்பு o; \
} \
கட்டமைப்பு type_ * \
type_##_next(கட்டமைப்பு type_ *o) \
{ \
    கட்டமைப்பு parent_type_ *parent; \
    கட்டமைப்பு type_ *next; \
    parent = container_of(o->base.parent, கட்டமைப்பு parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    எனில் (list_is_last(&parent->parent_field_, &o->base.link)) \
        திரும்பு வெற்று; \
    திரும்பு next; \
}

நிலையான வெறுமை
rxkb_object_init(கட்டமைப்பு rxkb_object *object, கட்டமைப்பு rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

நிலையான வெறுமை *
rxkb_object_ref(கட்டமைப்பு rxkb_object *object)
{
    ப்ரதிஜ்ஞா(object->refcount >= 1);
    ++object->refcount;
    திரும்பு object;
}

நிலையான வெறுமை
rxkb_iso639_code_destroy(கட்டமைப்பு rxkb_iso639_code *code)
{
    விடுவி(code->code);
}

கட்டமைப்பு rxkb_iso639_code *
rxkb_layout_get_iso639_first(கட்டமைப்பு rxkb_layout *layout)
{
    கட்டமைப்பு rxkb_iso639_code *code = வெற்று;

    எனில் (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    திரும்பு code;
}

கட்டமைப்பு rxkb_iso639_code *
rxkb_iso639_code_next(கட்டமைப்பு rxkb_iso639_code *code)
{
    கட்டமைப்பு rxkb_iso639_code *next = வெற்று;
    கட்டமைப்பு rxkb_layout *layout;

    layout = container_of(code->base.parent, கட்டமைப்பு rxkb_layout, base);

    எனில் (list_is_last(&layout->iso639s, &code->base.link))
        திரும்பு வெற்று;

    next = list_first_entry(&code->base.link, code, base.link);

    திரும்பு next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

நிலையான வெறுமை
rxkb_iso3166_code_destroy(கட்டமைப்பு rxkb_iso3166_code *code)
{
    விடுவி(code->code);
}

கட்டமைப்பு rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(கட்டமைப்பு rxkb_layout *layout)
{
    கட்டமைப்பு rxkb_iso3166_code *code = வெற்று;

    எனில் (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    திரும்பு code;
}

கட்டமைப்பு rxkb_iso3166_code *
rxkb_iso3166_code_next(கட்டமைப்பு rxkb_iso3166_code *code)
{
    கட்டமைப்பு rxkb_iso3166_code *next = வெற்று;
    கட்டமைப்பு rxkb_layout *layout;

    layout = container_of(code->base.parent, கட்டமைப்பு rxkb_layout, base);

    எனில் (list_is_last(&layout->iso3166s, &code->base.link))
        திரும்பு வெற்று;

    next = list_first_entry(&code->base.link, code, base.link);

    திரும்பு next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

நிலையான வெறுமை
rxkb_option_destroy(கட்டமைப்பு rxkb_option *o)
{
    விடுவி(o->name);
    விடுவி(o->brief);
    விடுவி(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, எண்ணிக்கை rxkb_popularity);
bool rxkb_option_is_layout_specific(கட்டமைப்பு rxkb_option *object) {
    திரும்பு object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

நிலையான வெறுமை
rxkb_layout_destroy(கட்டமைப்பு rxkb_layout *l)
{
    கட்டமைப்பு rxkb_iso639_code *iso639, *tmp_639;
    கட்டமைப்பு rxkb_iso3166_code *iso3166, *tmp_3166;

    விடுவி(l->name);
    விடுவி(l->brief);
    விடுவி(l->description);
    விடுவி(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, எண்ணிக்கை rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

நிலையான வெறுமை
rxkb_model_destroy(கட்டமைப்பு rxkb_model *m)
{
    விடுவி(m->name);
    விடுவி(m->vendor);
    விடுவி(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, எண்ணிக்கை rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

நிலையான வெறுமை
rxkb_option_group_destroy(கட்டமைப்பு rxkb_option_group *og)
{
    கட்டமைப்பு rxkb_option *o, *otmp;

    விடுவி(og->name);
    விடுவி(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(கட்டமைப்பு rxkb_option_group *g)
{
    திரும்பு g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, எண்ணிக்கை rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

நிலையான வெறுமை
rxkb_context_destroy(கட்டமைப்பு rxkb_context *ctx)
{
    கட்டமைப்பு rxkb_model *m, *mtmp;
    கட்டமைப்பு rxkb_layout *l, *ltmp;
    கட்டமைப்பு rxkb_option_group *og, *ogtmp;
    எழுத்து **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    ப்ரதிஜ்ஞா(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    ப்ரதிஜ்ஞா(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    ப்ரதிஜ்ஞா(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        விடுவி(*path);
    darray_free(ctx->includes);

    ப்ரதிஜ்ஞா(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, எண்ணிக்கை rxkb_log_level);

நிலையான எழுத்து *
rxkb_context_getenv(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *name)
{
    எனில் (ctx->use_secure_getenv) {
        திரும்பு secure_getenv(name);
    } இல்லையெனில் {
        திரும்பு getenv(name);
    }
}


வெறுமை
rxkb_context_set_log_level(கட்டமைப்பு rxkb_context *ctx,
                           எண்ணிக்கை rxkb_log_level level)
{
    ctx->log_level = level;
}

நிலையான மாறிலி எழுத்து *
log_level_to_prefix(எண்ணிக்கை rxkb_log_level level)
{
    தேர்வு (level) {
    நிலை RXKB_LOG_LEVEL_DEBUG:
        திரும்பு "xkbregistry: DEBUG: ";
    நிலை RXKB_LOG_LEVEL_INFO:
        திரும்பு "xkbregistry: INFO: ";
    நிலை RXKB_LOG_LEVEL_WARNING:
        திரும்பு "xkbregistry: WARNING: ";
    நிலை RXKB_LOG_LEVEL_ERROR:
        திரும்பு "xkbregistry: ERROR: ";
    நிலை RXKB_LOG_LEVEL_CRITICAL:
        திரும்பு "xkbregistry: CRITICAL: ";
    இயல்பு:
        திரும்பு வெற்று;
    }
}

ATTR_PRINTF(3, 0) நிலையான வெறுமை
default_log_fn(கட்டமைப்பு rxkb_context *ctx, எண்ணிக்கை rxkb_log_level level,
               மாறிலி எழுத்து *fmt, va_list args)
{
    மாறிலி எழுத்து *prefix = log_level_to_prefix(level);

    எனில் (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

நிலையான எண்ணிக்கை rxkb_log_level
log_level(மாறிலி எழுத்து *level) {
    errno = 0;
    எழுத்து *endptr;
    மாறிலி long lvl = strtol(level, &endptr, 10);
    எனில் (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        திரும்பு (எண்ணிக்கை rxkb_log_level)lvl;

    எனில் (istreq_prefix("crit", level))
        திரும்பு RXKB_LOG_LEVEL_CRITICAL;
    எனில் (istreq_prefix("err", level))
        திரும்பு RXKB_LOG_LEVEL_ERROR;
    எனில் (istreq_prefix("warn", level))
        திரும்பு RXKB_LOG_LEVEL_WARNING;
    எனில் (istreq_prefix("info", level))
        திரும்பு RXKB_LOG_LEVEL_INFO;
    எனில் (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        திரும்பு RXKB_LOG_LEVEL_DEBUG;

    திரும்பு RXKB_LOG_LEVEL_ERROR;
}

கட்டமைப்பு rxkb_context *
rxkb_context_new(எண்ணிக்கை rxkb_context_flags flags)
{
    கட்டமைப்பு rxkb_context *ctx = rxkb_context_create(வெற்று);
    மாறிலி எழுத்து *env;

    எனில் (!ctx)
        திரும்பு வெற்று;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    எனில் (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    நிலையான மாறிலி எண்ணிக்கை rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    எனில் (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        விடுவி(ctx);
        திரும்பு வெற்று;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    எனில் (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_வேர்);
        rxkb_context_unref(ctx);
        திரும்பு வெற்று;
    }

    திரும்பு ctx;
}

வெறுமை
rxkb_context_set_log_fn(கட்டமைப்பு rxkb_context *ctx,
                        வெறுமை (*log_fn)(கட்டமைப்பு rxkb_context *ctx,
                                       எண்ணிக்கை rxkb_log_level level,
                                       மாறிலி எழுத்து *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *path)
{
    முழு err = 0;

    எனில் (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        செல் error;
    }

    கட்டமைப்பு stat stat_buf;
    err = stat(path, &stat_buf);
    எனில் (err != 0) {
        err = errno;
        செல் error;
    }
    எனில் (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        செல் error;
    }

    எனில் (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        செல் error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    எழுத்து rules[PATH_MAX];
    எனில் (!snprintf_safe(rules, அளவு(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                அளவு(rules), path, DEFAULT_XKB_RULES);
        செல் error;
    }

    எழுத்து * மாறிலி tmp = strdup(path);
    எனில் (!tmp) {
        err = ENOMEM;
        செல் error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    திரும்பு true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    திரும்பு false;
}

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES

நிலையான முழு
compare_str(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    திரும்பு strcmp(*(எழுத்து **)a, *(எழுத்து **) b);
}

நிலையான முழு
add_direct_subdirectories(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *path,
                          darray_string *extensions,
                          darray_size_t versioned_மொத்தம்,
                          size_t versioned_path_length)
{
    முழு ret = 0;
    முழு err = ENOMEM;
    DIR *dir = வெற்று;

    /* Check extensions parent directory */
    கட்டமைப்பு stat stat_buf;
    err = stat(path, &stat_buf);
    எனில் (err != 0) {
        err = errno;
        செல் err;
    }
    எனில் (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        செல் err;
    }
    எனில் (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        செல் err;
    }

    dir = opendir(path);
    எனில் (dir == வெற்று) {
        err = EACCES;
        செல் err;
    }

    கட்டமைப்பு dirent *entry;
    எழுத்து path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    போது ((entry = readdir(dir)) != வெற்று) {
        மாறிலி எழுத்து *name = entry->d_name;

        /* Skip special entries */
        எனில் (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            தொடர்;

        எனில் (!snprintf_safe(path_buf, அளவு(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            செல் err;
        }
        எனில் (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            தொடர்;

        /* Skip if the corresponding versioned directory is already included */
        க்கு (darray_size_t i = 0; i < versioned_மொத்தம்; i++) {
            மாறிலி எழுத்து * மாறிலி prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            எனில் (strcmp(name, prev_name) == 0)
                செல் next;
        }

        எழுத்து *ext_path = strdup_safe(path_buf);
        எனில் (!ext_path) {
            err = ENOMEM;
            செல் err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    எனில் (darray_size(*extensions) > versioned_மொத்தம்) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_பொருட்கள்(*extensions) + versioned_மொத்தம்,
              darray_size(*extensions) - versioned_மொத்தம்,
              அளவு(*darray_பொருட்கள்(*extensions)), &compare_str);
        எழுத்து **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_மொத்தம்) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    திரும்பு ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    எனில் (dir)
        closedir(dir);

    திரும்பு ret;
}
#endif

bool
rxkb_context_include_path_append_default(கட்டமைப்பு rxkb_context *ctx)
{
    எழுத்து user_path[PATH_MAX];
    bool ret = false;

    எனில் (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        திரும்பு false;
    }

    மாறிலி எழுத்து * மாறிலி home = rxkb_context_getenv(ctx, "HOME");
    மாறிலி எழுத்து * மாறிலி xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    எனில் (xdg != வெற்று) {
        எனில் (snprintf_safe(user_path, அளவு(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } இல்லையெனில் எனில் (home != வெற்று) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        எனில் (snprintf_safe(user_path, அளவு(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    எனில் (home != வெற்று) {
        எனில் (snprintf_safe(user_path, அளவு(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    மாறிலி எழுத்து * மாறிலி extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != வெற்று) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#எனில் HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    மாறிலி எழுத்து *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    எனில் (extensions_path == வெற்று)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    எனில் (extensions_path != வெற்று) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    எனில் (extensions_path == வெற்று)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    எனில் (extensions_path != வெற்று) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    எழுத்து **ext_path;
    darray_foreach(ext_path, extensions) {
        விடுவி(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    மாறிலி எழுத்து * மாறிலி வேர் = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    மாறிலி bool has_வேர் = rxkb_context_include_path_append(
        ctx, ((வேர் != வெற்று) ? வேர்: DFLT_XKB_CONFIG_வேர்)
    );
    ret |= has_வேர்;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    எனில் (!has_வேர் && (வேர் == வெற்று || வேர்[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_வேர்,
                 ((வேர் == வெற்று) ? DFLT_XKB_CONFIG_வேர் : வேர்));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_வேர்);
    }

    திரும்பு ret;
}

bool
rxkb_context_parse_default_ruleset(கட்டமைப்பு rxkb_context *ctx)
{
    திரும்பு rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *ruleset)
{
    எழுத்து **path;
    bool success = false;

    எனில் (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        திரும்பு false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        எழுத்து rules[PATH_MAX];

        எனில் (snprintf_safe(rules, அளவு(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            எனில் (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        எனில் (ctx->load_extra_rules_files &&
            snprintf_safe(rules, அளவு(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            எனில் (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    திரும்பு success;
}


வெறுமை
rxkb_context_set_user_data(கட்டமைப்பு rxkb_context *ctx, வெறுமை *userdata)
{
    ctx->userdata = userdata;
}

வெறுமை *
rxkb_context_get_user_data(கட்டமைப்பு rxkb_context *ctx)
{
    திரும்பு ctx->userdata;
}

நிலையான உள்வரி bool
is_முனை(xml_முனை *முனை, மாறிலி எழுத்து *name)
{
    திரும்பு முனை->type == XML_ELEMENT_முனை &&
        xmlStrEqual(முனை->name, (மாறிலி xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
நிலையான எழுத்து *
extract_text(xml_முனை *முனை)
{
    xml_முனை *n;

    க்கு (n = முனை->children; n; n = n->next) {
        எனில் (n->type == XML_TEXT_முனை)
            திரும்பு (எழுத்து *)xmlStrdup(n->content);
    }
    திரும்பு வெற்று;
}

/* Data from “configItem” node */
கட்டமைப்பு config_item {
    எழுத்து *name;
    எழுத்து *description;
    எழுத்து *brief;
    எழுத்து *vendor;
    எண்ணிக்கை rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = வெற்று, \
    .description = வெற்று, \
    .brief = வெற்று, \
    .vendor = வெற்று, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

நிலையான வெறுமை
config_item_free(கட்டமைப்பு config_item *config) {
    விடுவி(config->name);
    விடுவி(config->description);
    விடுவி(config->brief);
    விடுவி(config->vendor);
}

நிலையான bool
parse_config_item(கட்டமைப்பு rxkb_context *ctx, xml_முனை *parent,
                  கட்டமைப்பு config_item *config)
{
    xml_முனை *முனை = வெற்று;
    xml_முனை *ci = வெற்று;

    க்கு (ci = parent->children; ci; ci = ci->next) {
        எனில் (is_முனை(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (மாறிலி xmlChar*)"popularity");
            எனில் (raw_popularity) {
                எனில் (xmlStrEqual(raw_popularity, (மாறிலி xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                இல்லையெனில் எனில் (xmlStrEqual(raw_popularity, (மாறிலி xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                இல்லையெனில்
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->வரி, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (மாறிலி xmlChar*)"layout-specific");
            எனில் (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (மாறிலி xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            க்கு (முனை = ci->children; முனை; முனை = முனை->next) {
                எனில் (is_முனை(முனை, "name"))
                    config->name = extract_text(முனை);
                இல்லையெனில் எனில் (is_முனை(முனை, "description"))
                    config->description = extract_text(முனை);
                இல்லையெனில் எனில் (is_முனை(முனை, "shortDescription"))
                    config->brief = extract_text(முனை);
                இல்லையெனில் எனில் (is_முனை(முனை, "vendor"))
                    config->vendor = extract_text(முனை);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            எனில் (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->வரி);
                config_item_free(config);
                திரும்பு false;
            }

            திரும்பு true; /* only one configItem allowed in the dtd */
        }
    }

    திரும்பு false;
}

நிலையான வெறுமை
parse_model(கட்டமைப்பு rxkb_context *ctx, xml_முனை *model,
            எண்ணிக்கை rxkb_popularity popularity)
{
    கட்டமைப்பு config_item config = config_item_new(popularity);

    எனில் (parse_config_item(ctx, model, &config)) {
        கட்டமைப்பு rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            எனில் (streq(m->name, config.name)) {
                config_item_free(&config);
                திரும்பு;
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

நிலையான வெறுமை
parse_model_list(கட்டமைப்பு rxkb_context *ctx, xml_முனை *model_list,
                 எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *முனை = வெற்று;

    க்கு (முனை = model_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "model"))
            parse_model(ctx, முனை, popularity);
    }
}

நிலையான வெறுமை
parse_language_list(xml_முனை *language_list, கட்டமைப்பு rxkb_layout *layout)
{
    xml_முனை *முனை = வெற்று;
    கட்டமைப்பு rxkb_iso639_code *code;

    க்கு (முனை = language_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "iso639Id")) {
            எழுத்து *str = extract_text(முனை);
            கட்டமைப்பு rxkb_object *parent;

            எனில் (!str || strlen(str) != 3) {
                விடுவி(str);
                தொடர்;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

நிலையான வெறுமை
parse_country_list(xml_முனை *country_list, கட்டமைப்பு rxkb_layout *layout)
{
    xml_முனை *முனை = வெற்று;
    கட்டமைப்பு rxkb_iso3166_code *code;

    க்கு (முனை = country_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "iso3166Id")) {
            எழுத்து *str = extract_text(முனை);
            கட்டமைப்பு rxkb_object *parent;

            எனில் (!str || strlen(str) != 2) {
                விடுவி(str);
                தொடர்;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

நிலையான வெறுமை
parse_variant(கட்டமைப்பு rxkb_context *ctx, கட்டமைப்பு rxkb_layout *l,
              xml_முனை *variant, எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *ci;
    கட்டமைப்பு config_item config = config_item_new(popularity);

    எனில் (parse_config_item(ctx, variant, &config)) {
        கட்டமைப்பு rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            எனில் (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                நிறுத்து;
            }
        }

        எனில் (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == வெற்று ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            க்கு (ci = variant->children; ci; ci = ci->next) {
                xml_முனை *முனை;

                எனில் (!is_முனை(ci, "configItem"))
                    தொடர்;

                bool found_language_list = false;
                bool found_country_list = false;
                க்கு (முனை = ci->children; முனை; முனை = முனை->next) {
                    எனில் (is_முனை(முனை, "languageList")) {
                        parse_language_list(முனை, v);
                        found_language_list = true;
                    }
                    எனில் (is_முனை(முனை, "countryList")) {
                        parse_country_list(முனை, v);
                        found_country_list = true;
                    }
                }
                எனில் (!found_language_list) {
                    // inherit from parent layout
                    கட்டமைப்பு rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        கட்டமைப்பு rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                எனில் (!found_country_list) {
                    // inherit from parent layout
                    கட்டமைப்பு rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        கட்டமைப்பு rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } இல்லையெனில் {
            config_item_free(&config);
        }
    }
}

நிலையான வெறுமை
parse_variant_list(கட்டமைப்பு rxkb_context *ctx, கட்டமைப்பு rxkb_layout *l,
                   xml_முனை *variant_list, எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *முனை = வெற்று;

    க்கு (முனை = variant_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "variant"))
            parse_variant(ctx, l, முனை, popularity);
    }
}

நிலையான வெறுமை
parse_layout(கட்டமைப்பு rxkb_context *ctx, xml_முனை *layout,
             எண்ணிக்கை rxkb_popularity popularity)
{
    கட்டமைப்பு config_item config = config_item_new(popularity);
    கட்டமைப்பு rxkb_layout *l;
    xml_முனை *முனை = வெற்று;
    bool exists = false;

    எனில் (!parse_config_item(ctx, layout, &config))
        திரும்பு;

    list_for_each(l, &ctx->layouts, base.link) {
        எனில் (streq(l->name, config.name) && l->variant == வெற்று) {
            exists = true;
            நிறுத்து;
        }
    }

    எனில் (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = வெற்று;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } இல்லையெனில் {
        config_item_free(&config);
    }

    க்கு (முனை = layout->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "variantList")) {
            parse_variant_list(ctx, l, முனை, popularity);
        }
        எனில் (!exists && is_முனை(முனை, "configItem")) {
            xml_முனை *ll;
            க்கு (ll = முனை->children; ll; ll = ll->next) {
                எனில் (is_முனை(ll, "languageList"))
                    parse_language_list(ll, l);
                எனில் (is_முனை(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

நிலையான வெறுமை
parse_layout_list(கட்டமைப்பு rxkb_context *ctx, xml_முனை *layout_list,
                  எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *முனை = வெற்று;

    க்கு (முனை = layout_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "layout"))
            parse_layout(ctx, முனை, popularity);
    }
}

நிலையான வெறுமை
parse_option(கட்டமைப்பு rxkb_context *ctx, கட்டமைப்பு rxkb_option_group *group,
             xml_முனை *option, எண்ணிக்கை rxkb_popularity popularity)
{
    கட்டமைப்பு config_item config = config_item_new(popularity);

    எனில் (parse_config_item(ctx, option, &config)) {
        கட்டமைப்பு rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            எனில் (streq(o->name, config.name)) {
                config_item_free(&config);
                திரும்பு;
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

நிலையான வெறுமை
parse_group(கட்டமைப்பு rxkb_context *ctx, xml_முனை *group,
            எண்ணிக்கை rxkb_popularity popularity)
{
    கட்டமைப்பு config_item config = config_item_new(popularity);
    கட்டமைப்பு rxkb_option_group *g;
    xml_முனை *முனை = வெற்று;
    xmlChar *multiple;
    bool exists = false;

    எனில் (!parse_config_item(ctx, group, &config))
        திரும்பு;

    list_for_each(g, &ctx->option_groups, base.link) {
        எனில் (streq(g->name, config.name)) {
            exists = true;
            நிறுத்து;
        }
    }

    எனில் (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (மாறிலி xmlChar*)"allowMultipleSelection");
        எனில் (multiple && xmlStrEqual(multiple, (மாறிலி xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } இல்லையெனில் {
        config_item_free(&config);
    }

    க்கு (முனை = group->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "option"))
            parse_option(ctx, g, முனை, popularity);
    }
}

நிலையான வெறுமை
parse_option_list(கட்டமைப்பு rxkb_context *ctx, xml_முனை *option_list,
                  எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *முனை = வெற்று;

    க்கு (முனை = option_list->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "group"))
            parse_group(ctx, முனை, popularity);
    }
}

நிலையான வெறுமை
parse_rules_xml(கட்டமைப்பு rxkb_context *ctx, xml_முனை *வேர்,
                எண்ணிக்கை rxkb_popularity popularity)
{
    xml_முனை *முனை = வெற்று;

    க்கு (முனை = வேர்->children; முனை; முனை = முனை->next) {
        எனில் (is_முனை(முனை, "modelList"))
            parse_model_list(ctx, முனை, popularity);
        இல்லையெனில் எனில் (is_முனை(முனை, "layoutList"))
            parse_layout_list(ctx, முனை, popularity);
        இல்லையெனில் எனில் (is_முனை(முனை, "optionList"))
            parse_option_list(ctx, முனை, popularity);
    }
}

நிலையான வெறுமை
ATTR_PRINTF(2, 0)
xml_error_func(வெறுமை *ctx, மாறிலி எழுத்து *செய்தி, ...)
{
    நிலையான எழுத்து buf[PATH_MAX];
    நிலையான size_t slen = 0;
    va_list args;
    முழு rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, செய்தி);
    rc = vsnprintf(&buf[slen], அளவு(buf) - slen, செய்தி, args);
    va_end(args);

    /* This shouldn't really happen */
    எனில் (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, அளவு(buf));
        திரும்பு;
    }

    slen += (size_t)rc;
    எனில் (slen >= அளவு(buf)) {
        /* truncated, let's flush this */
        buf[அளவு(buf) - 1] = '\n';
        slen = அளவு(buf);
    }

    /* We're assuming here that the last character is \n. */
    எனில் (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, அளவு(buf));
        slen = 0;
    }
}

#எனில் HAVE_XML_CTXT_SET_ERRORHANDLER
நிலையான வெறுமை
xml_structured_error_func(வெறுமை *userData, மாறிலி xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#இல்லையெனில்
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

நிலையான bool
validate(கட்டமைப்பு rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    மாறிலி எழுத்து dtdstr[] =
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

#எனில் HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    எனில் (!xmlCtxt)
        திரும்பு false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_உள்ளீடு_Ptr pinput =
        xml_New_உள்ளீடு_From_Memory(வெற்று, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_உள்ளீடு_BUF_STATIC);
    எனில் (!pinput)
        செல் dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, வெற்று, வெற்று);
#இல்லையெனில்
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_உள்ளீடு_Buffer_Ptr buf =
        xml_Parser_உள்ளீடு_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    எனில் (!buf)
        செல் dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(வெற்று, buf, XML_CHAR_ENCODING_UTF8);
#endif

    எனில் (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        செல் dtd_error;
    }

#எனில் HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#இல்லையெனில்
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    எனில் (!dtdvalid) {
        success = false;
        செல் ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#எனில் HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    திரும்பு success;
}

நிலையான bool
parse(கட்டமைப்பு rxkb_context *ctx, மாறிலி எழுத்து *path,
      எண்ணிக்கை rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = வெற்று;
    xml_முனை *வேர் = வெற்று;

    எனில் (!check_eaccess(path, R_OK))
        திரும்பு false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    எனில் (!xmlCtxt)
        திரும்பு false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#இல்லையெனில்
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#எனில் HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#எனில் !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, வெற்று, 0);
    எனில் (!doc)
        செல் parse_error;

    எனில் (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        செல் validate_error;
    }

    வேர் = xml_Doc_Get_வேர்_Element(doc);
    parse_rules_xml(ctx, வேர், popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#எனில் !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(வெற்று, வெற்று);
#endif
#எனில் HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, வெற்று, வெற்று);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    திரும்பு success;
}
