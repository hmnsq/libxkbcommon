/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <libxml/parser.h>

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES
    #અન્તર્ભાવ <limits.h>
    #જો HAVE_UNISTD_H && HAVE_DIRENT_H
        #અન્તર્ભાવ <dirent.h>
        #અન્તર્ભાવ <unistd.h>
    #endif
#endif

#અન્તર્ભાવ "xkbcommon/xkbregistry.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-list.h"
#અન્તર્ભાવ "util-mem.h"

સંરચના rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
સંરચના rxkb_object {
    સંરચના rxkb_object *parent;
    uint32_t refcount;
    સંરચના list link;
};

સંરચના rxkb_iso639_code {
    સંરચના rxkb_object base;
    અક્ષર *code;
};

સંરચના rxkb_iso3166_code {
    સંરચના rxkb_object base;
    અક્ષર *code;
};

ગણના context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

સંરચના rxkb_context {
    સંરચના rxkb_object base;
    ગણના context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    સંરચના list models;         /* list of struct rxkb_models */
    સંરચના list layouts;        /* list of struct rxkb_layouts */
    સંરચના list option_groups;  /* list of struct rxkb_option_group */

    darray(અક્ષર *) includes;


    ATTR_PRINTF(3, 0) શૂન્ય (*log_fn)(સંરચના rxkb_context *ctx,
                                     ગણના rxkb_log_level level,
                                     અચલ અક્ષર *fmt, va_list args);
    ગણના rxkb_log_level log_level;

    શૂન્ય *userdata;
};

સંરચના rxkb_model {
    સંરચના rxkb_object base;

    અક્ષર *name;
    અક્ષર *vendor;
    અક્ષર *description;
    ગણના rxkb_popularity popularity;
};

સંરચના rxkb_layout {
    સંરચના rxkb_object base;

    અક્ષર *name;
    અક્ષર *brief;
    અક્ષર *description;
    અક્ષર *variant;
    ગણના rxkb_popularity popularity;

    સંરચના list iso639s;  /* list of struct rxkb_iso639_code */
    સંરચના list iso3166s; /* list of struct rxkb_iso3166_code */
};

સંરચના rxkb_option_group {
    સંરચના rxkb_object base;

    bool allow_multiple;
    સંરચના list options; /* list of struct rxkb_options */
    અક્ષર *name;
    અક્ષર *description;
    ગણના rxkb_popularity popularity;
};

સંરચના rxkb_option {
    સંરચના rxkb_object base;

    અક્ષર *name;
    અક્ષર *brief;
    અક્ષર *description;
    ગણના rxkb_popularity popularity;
    bool layout_specific;
};

સ્થિર bool
parse(સંરચના rxkb_context *ctx, અચલ અક્ષર *path,
      ગણના rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
સ્થિર શૂન્ય
rxkb_log(સંરચના rxkb_context *ctx, ગણના rxkb_log_level level,
         અચલ અક્ષર *fmt, ...)
{
    va_list args;

    જો (ctx->log_level < level)
        પાછા;

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
#define rxkb_log_with_code(ctx, level, સંદેશ_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(સંદેશ_id, fmt), ##__VA_ARGS__)
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
સંરચના type_ * type_##_ref(સંરચના type_ *object) { \
    rxkb_object_ref(&object->base); \
    પાછા object; \
} \
સંરચના type_ * type_##_unref(સંરચના type_ *object) { \
    જો (!object) પાછા ખાલી; \
    પ્રતિજ્ઞા(object->base.refcount >= 1); \
    જો (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        મુક્ત(object); \
    } \
    પાછા ખાલી;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
સ્થિર અંતઃરેખ સંરચના type_ * type_##_create(સંરચના rxkb_object *parent) { \
    સંરચના type_ *t = calloc(1, કદ *t); \
    જો (t) \
        rxkb_object_init(&t->base, parent); \
    પાછા t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(સંરચના type_ *object) { \
    પાછા object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, અચલ અક્ષર*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
સંરચના type_ * type_##_first(સંરચના parent_type_ *parent) { \
    સંરચના type_ *o = ખાલી; \
    જો (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    પાછા o; \
} \
સંરચના type_ * \
type_##_next(સંરચના type_ *o) \
{ \
    સંરચના parent_type_ *parent; \
    સંરચના type_ *next; \
    parent = container_of(o->base.parent, સંરચના parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    જો (list_is_last(&parent->parent_field_, &o->base.link)) \
        પાછા ખાલી; \
    પાછા next; \
}

સ્થિર શૂન્ય
rxkb_object_init(સંરચના rxkb_object *object, સંરચના rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

સ્થિર શૂન્ય *
rxkb_object_ref(સંરચના rxkb_object *object)
{
    પ્રતિજ્ઞા(object->refcount >= 1);
    ++object->refcount;
    પાછા object;
}

સ્થિર શૂન્ય
rxkb_iso639_code_destroy(સંરચના rxkb_iso639_code *code)
{
    મુક્ત(code->code);
}

સંરચના rxkb_iso639_code *
rxkb_layout_get_iso639_first(સંરચના rxkb_layout *layout)
{
    સંરચના rxkb_iso639_code *code = ખાલી;

    જો (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    પાછા code;
}

સંરચના rxkb_iso639_code *
rxkb_iso639_code_next(સંરચના rxkb_iso639_code *code)
{
    સંરચના rxkb_iso639_code *next = ખાલી;
    સંરચના rxkb_layout *layout;

    layout = container_of(code->base.parent, સંરચના rxkb_layout, base);

    જો (list_is_last(&layout->iso639s, &code->base.link))
        પાછા ખાલી;

    next = list_first_entry(&code->base.link, code, base.link);

    પાછા next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

સ્થિર શૂન્ય
rxkb_iso3166_code_destroy(સંરચના rxkb_iso3166_code *code)
{
    મુક્ત(code->code);
}

સંરચના rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(સંરચના rxkb_layout *layout)
{
    સંરચના rxkb_iso3166_code *code = ખાલી;

    જો (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    પાછા code;
}

સંરચના rxkb_iso3166_code *
rxkb_iso3166_code_next(સંરચના rxkb_iso3166_code *code)
{
    સંરચના rxkb_iso3166_code *next = ખાલી;
    સંરચના rxkb_layout *layout;

    layout = container_of(code->base.parent, સંરચના rxkb_layout, base);

    જો (list_is_last(&layout->iso3166s, &code->base.link))
        પાછા ખાલી;

    next = list_first_entry(&code->base.link, code, base.link);

    પાછા next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

સ્થિર શૂન્ય
rxkb_option_destroy(સંરચના rxkb_option *o)
{
    મુક્ત(o->name);
    મુક્ત(o->brief);
    મુક્ત(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ગણના rxkb_popularity);
bool rxkb_option_is_layout_specific(સંરચના rxkb_option *object) {
    પાછા object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

સ્થિર શૂન્ય
rxkb_layout_destroy(સંરચના rxkb_layout *l)
{
    સંરચના rxkb_iso639_code *iso639, *tmp_639;
    સંરચના rxkb_iso3166_code *iso3166, *tmp_3166;

    મુક્ત(l->name);
    મુક્ત(l->brief);
    મુક્ત(l->description);
    મુક્ત(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ગણના rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

સ્થિર શૂન્ય
rxkb_model_destroy(સંરચના rxkb_model *m)
{
    મુક્ત(m->name);
    મુક્ત(m->vendor);
    મુક્ત(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ગણના rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

સ્થિર શૂન્ય
rxkb_option_group_destroy(સંરચના rxkb_option_group *og)
{
    સંરચના rxkb_option *o, *otmp;

    મુક્ત(og->name);
    મુક્ત(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(સંરચના rxkb_option_group *g)
{
    પાછા g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, ગણના rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

સ્થિર શૂન્ય
rxkb_context_destroy(સંરચના rxkb_context *ctx)
{
    સંરચના rxkb_model *m, *mtmp;
    સંરચના rxkb_layout *l, *ltmp;
    સંરચના rxkb_option_group *og, *ogtmp;
    અક્ષર **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    પ્રતિજ્ઞા(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    પ્રતિજ્ઞા(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    પ્રતિજ્ઞા(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        મુક્ત(*path);
    darray_free(ctx->includes);

    પ્રતિજ્ઞા(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ગણના rxkb_log_level);

સ્થિર અક્ષર *
rxkb_context_getenv(સંરચના rxkb_context *ctx, અચલ અક્ષર *name)
{
    જો (ctx->use_secure_getenv) {
        પાછા secure_getenv(name);
    } નહીંતર {
        પાછા getenv(name);
    }
}


શૂન્ય
rxkb_context_set_log_level(સંરચના rxkb_context *ctx,
                           ગણના rxkb_log_level level)
{
    ctx->log_level = level;
}

સ્થિર અચલ અક્ષર *
log_level_to_prefix(ગણના rxkb_log_level level)
{
    પસંદગી (level) {
    સ્થિતિ RXKB_LOG_LEVEL_DEBUG:
        પાછા "xkbregistry: DEBUG: ";
    સ્થિતિ RXKB_LOG_LEVEL_INFO:
        પાછા "xkbregistry: INFO: ";
    સ્થિતિ RXKB_LOG_LEVEL_WARNING:
        પાછા "xkbregistry: WARNING: ";
    સ્થિતિ RXKB_LOG_LEVEL_ERROR:
        પાછા "xkbregistry: ERROR: ";
    સ્થિતિ RXKB_LOG_LEVEL_CRITICAL:
        પાછા "xkbregistry: CRITICAL: ";
    મૂળભૂત:
        પાછા ખાલી;
    }
}

ATTR_PRINTF(3, 0) સ્થિર શૂન્ય
default_log_fn(સંરચના rxkb_context *ctx, ગણના rxkb_log_level level,
               અચલ અક્ષર *fmt, va_list args)
{
    અચલ અક્ષર *prefix = log_level_to_prefix(level);

    જો (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

સ્થિર ગણના rxkb_log_level
log_level(અચલ અક્ષર *level) {
    errno = 0;
    અક્ષર *endptr;
    અચલ long lvl = strtol(level, &endptr, 10);
    જો (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        પાછા (ગણના rxkb_log_level)lvl;

    જો (istreq_prefix("crit", level))
        પાછા RXKB_LOG_LEVEL_CRITICAL;
    જો (istreq_prefix("err", level))
        પાછા RXKB_LOG_LEVEL_ERROR;
    જો (istreq_prefix("warn", level))
        પાછા RXKB_LOG_LEVEL_WARNING;
    જો (istreq_prefix("info", level))
        પાછા RXKB_LOG_LEVEL_INFO;
    જો (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        પાછા RXKB_LOG_LEVEL_DEBUG;

    પાછા RXKB_LOG_LEVEL_ERROR;
}

સંરચના rxkb_context *
rxkb_context_new(ગણના rxkb_context_flags flags)
{
    સંરચના rxkb_context *ctx = rxkb_context_create(ખાલી);
    અચલ અક્ષર *env;

    જો (!ctx)
        પાછા ખાલી;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    જો (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    સ્થિર અચલ ગણના rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    જો (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        મુક્ત(ctx);
        પાછા ખાલી;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    જો (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_મૂળ);
        rxkb_context_unref(ctx);
        પાછા ખાલી;
    }

    પાછા ctx;
}

શૂન્ય
rxkb_context_set_log_fn(સંરચના rxkb_context *ctx,
                        શૂન્ય (*log_fn)(સંરચના rxkb_context *ctx,
                                       ગણના rxkb_log_level level,
                                       અચલ અક્ષર *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(સંરચના rxkb_context *ctx, અચલ અક્ષર *path)
{
    પૂર્ણાંક err = 0;

    જો (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        જાઓ error;
    }

    સંરચના stat stat_buf;
    err = stat(path, &stat_buf);
    જો (err != 0) {
        err = errno;
        જાઓ error;
    }
    જો (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        જાઓ error;
    }

    જો (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        જાઓ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    અક્ષર rules[PATH_MAX];
    જો (!snprintf_safe(rules, કદ(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                કદ(rules), path, DEFAULT_XKB_RULES);
        જાઓ error;
    }

    અક્ષર * અચલ tmp = strdup(path);
    જો (!tmp) {
        err = ENOMEM;
        જાઓ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    પાછા true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    પાછા false;
}

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES

સ્થિર પૂર્ણાંક
compare_str(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
    પાછા strcmp(*(અક્ષર **)a, *(અક્ષર **) b);
}

સ્થિર પૂર્ણાંક
add_direct_subdirectories(સંરચના rxkb_context *ctx, અચલ અક્ષર *path,
                          darray_string *extensions,
                          darray_size_t versioned_ગણતરી,
                          size_t versioned_path_length)
{
    પૂર્ણાંક ret = 0;
    પૂર્ણાંક err = ENOMEM;
    DIR *dir = ખાલી;

    /* Check extensions parent directory */
    સંરચના stat stat_buf;
    err = stat(path, &stat_buf);
    જો (err != 0) {
        err = errno;
        જાઓ err;
    }
    જો (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        જાઓ err;
    }
    જો (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        જાઓ err;
    }

    dir = opendir(path);
    જો (dir == ખાલી) {
        err = EACCES;
        જાઓ err;
    }

    સંરચના dirent *entry;
    અક્ષર path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    જ્યારે ((entry = readdir(dir)) != ખાલી) {
        અચલ અક્ષર *name = entry->d_name;

        /* Skip special entries */
        જો (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ચાલુ;

        જો (!snprintf_safe(path_buf, કદ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            જાઓ err;
        }
        જો (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ચાલુ;

        /* Skip if the corresponding versioned directory is already included */
        માટે (darray_size_t i = 0; i < versioned_ગણતરી; i++) {
            અચલ અક્ષર * અચલ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            જો (strcmp(name, prev_name) == 0)
                જાઓ next;
        }

        અક્ષર *ext_path = strdup_safe(path_buf);
        જો (!ext_path) {
            err = ENOMEM;
            જાઓ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    જો (darray_size(*extensions) > versioned_ગણતરી) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_વસ્તુઓ(*extensions) + versioned_ગણતરી,
              darray_size(*extensions) - versioned_ગણતરી,
              કદ(*darray_વસ્તુઓ(*extensions)), &compare_str);
        અક્ષર **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ગણતરી) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    પાછા ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    જો (dir)
        closedir(dir);

    પાછા ret;
}
#endif

bool
rxkb_context_include_path_append_default(સંરચના rxkb_context *ctx)
{
    અક્ષર user_path[PATH_MAX];
    bool ret = false;

    જો (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        પાછા false;
    }

    અચલ અક્ષર * અચલ home = rxkb_context_getenv(ctx, "HOME");
    અચલ અક્ષર * અચલ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    જો (xdg != ખાલી) {
        જો (snprintf_safe(user_path, કદ(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } નહીંતર જો (home != ખાલી) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        જો (snprintf_safe(user_path, કદ(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    જો (home != ખાલી) {
        જો (snprintf_safe(user_path, કદ(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    અચલ અક્ષર * અચલ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != ખાલી) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    અચલ અક્ષર *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    જો (extensions_path == ખાલી)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    જો (extensions_path != ખાલી) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    જો (extensions_path == ખાલી)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    જો (extensions_path != ખાલી) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    અક્ષર **ext_path;
    darray_foreach(ext_path, extensions) {
        મુક્ત(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    અચલ અક્ષર * અચલ મૂળ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    અચલ bool has_મૂળ = rxkb_context_include_path_append(
        ctx, ((મૂળ != ખાલી) ? મૂળ: DFLT_XKB_CONFIG_મૂળ)
    );
    ret |= has_મૂળ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    જો (!has_મૂળ && (મૂળ == ખાલી || મૂળ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_મૂળ,
                 ((મૂળ == ખાલી) ? DFLT_XKB_CONFIG_મૂળ : મૂળ));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_મૂળ);
    }

    પાછા ret;
}

bool
rxkb_context_parse_default_ruleset(સંરચના rxkb_context *ctx)
{
    પાછા rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(સંરચના rxkb_context *ctx, અચલ અક્ષર *ruleset)
{
    અક્ષર **path;
    bool success = false;

    જો (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        પાછા false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        અક્ષર rules[PATH_MAX];

        જો (snprintf_safe(rules, કદ(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            જો (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        જો (ctx->load_extra_rules_files &&
            snprintf_safe(rules, કદ(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            જો (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    પાછા success;
}


શૂન્ય
rxkb_context_set_user_data(સંરચના rxkb_context *ctx, શૂન્ય *userdata)
{
    ctx->userdata = userdata;
}

શૂન્ય *
rxkb_context_get_user_data(સંરચના rxkb_context *ctx)
{
    પાછા ctx->userdata;
}

સ્થિર અંતઃરેખ bool
is_ગાંઠ(xml_ગાંઠ *ગાંઠ, અચલ અક્ષર *name)
{
    પાછા ગાંઠ->type == XML_ELEMENT_ગાંઠ &&
        xmlStrEqual(ગાંઠ->name, (અચલ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
સ્થિર અક્ષર *
extract_text(xml_ગાંઠ *ગાંઠ)
{
    xml_ગાંઠ *n;

    માટે (n = ગાંઠ->children; n; n = n->next) {
        જો (n->type == XML_TEXT_ગાંઠ)
            પાછા (અક્ષર *)xmlStrdup(n->content);
    }
    પાછા ખાલી;
}

/* Data from “configItem” node */
સંરચના config_item {
    અક્ષર *name;
    અક્ષર *description;
    અક્ષર *brief;
    અક્ષર *vendor;
    ગણના rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = ખાલી, \
    .description = ખાલી, \
    .brief = ખાલી, \
    .vendor = ખાલી, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

સ્થિર શૂન્ય
config_item_free(સંરચના config_item *config) {
    મુક્ત(config->name);
    મુક્ત(config->description);
    મુક્ત(config->brief);
    મુક્ત(config->vendor);
}

સ્થિર bool
parse_config_item(સંરચના rxkb_context *ctx, xml_ગાંઠ *parent,
                  સંરચના config_item *config)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;
    xml_ગાંઠ *ci = ખાલી;

    માટે (ci = parent->children; ci; ci = ci->next) {
        જો (is_ગાંઠ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (અચલ xmlChar*)"popularity");
            જો (raw_popularity) {
                જો (xmlStrEqual(raw_popularity, (અચલ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                નહીંતર જો (xmlStrEqual(raw_popularity, (અચલ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                નહીંતર
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->લીટી, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (અચલ xmlChar*)"layout-specific");
            જો (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (અચલ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            માટે (ગાંઠ = ci->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
                જો (is_ગાંઠ(ગાંઠ, "name"))
                    config->name = extract_text(ગાંઠ);
                નહીંતર જો (is_ગાંઠ(ગાંઠ, "description"))
                    config->description = extract_text(ગાંઠ);
                નહીંતર જો (is_ગાંઠ(ગાંઠ, "shortDescription"))
                    config->brief = extract_text(ગાંઠ);
                નહીંતર જો (is_ગાંઠ(ગાંઠ, "vendor"))
                    config->vendor = extract_text(ગાંઠ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            જો (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->લીટી);
                config_item_free(config);
                પાછા false;
            }

            પાછા true; /* only one configItem allowed in the dtd */
        }
    }

    પાછા false;
}

સ્થિર શૂન્ય
parse_model(સંરચના rxkb_context *ctx, xml_ગાંઠ *model,
            ગણના rxkb_popularity popularity)
{
    સંરચના config_item config = config_item_new(popularity);

    જો (parse_config_item(ctx, model, &config)) {
        સંરચના rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            જો (streq(m->name, config.name)) {
                config_item_free(&config);
                પાછા;
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

સ્થિર શૂન્ય
parse_model_list(સંરચના rxkb_context *ctx, xml_ગાંઠ *model_list,
                 ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;

    માટે (ગાંઠ = model_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "model"))
            parse_model(ctx, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
parse_language_list(xml_ગાંઠ *language_list, સંરચના rxkb_layout *layout)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;
    સંરચના rxkb_iso639_code *code;

    માટે (ગાંઠ = language_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "iso639Id")) {
            અક્ષર *str = extract_text(ગાંઠ);
            સંરચના rxkb_object *parent;

            જો (!str || strlen(str) != 3) {
                મુક્ત(str);
                ચાલુ;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

સ્થિર શૂન્ય
parse_country_list(xml_ગાંઠ *country_list, સંરચના rxkb_layout *layout)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;
    સંરચના rxkb_iso3166_code *code;

    માટે (ગાંઠ = country_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "iso3166Id")) {
            અક્ષર *str = extract_text(ગાંઠ);
            સંરચના rxkb_object *parent;

            જો (!str || strlen(str) != 2) {
                મુક્ત(str);
                ચાલુ;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

સ્થિર શૂન્ય
parse_variant(સંરચના rxkb_context *ctx, સંરચના rxkb_layout *l,
              xml_ગાંઠ *variant, ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ci;
    સંરચના config_item config = config_item_new(popularity);

    જો (parse_config_item(ctx, variant, &config)) {
        સંરચના rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            જો (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                તોડો;
            }
        }

        જો (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ખાલી ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            માટે (ci = variant->children; ci; ci = ci->next) {
                xml_ગાંઠ *ગાંઠ;

                જો (!is_ગાંઠ(ci, "configItem"))
                    ચાલુ;

                bool found_language_list = false;
                bool found_country_list = false;
                માટે (ગાંઠ = ci->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
                    જો (is_ગાંઠ(ગાંઠ, "languageList")) {
                        parse_language_list(ગાંઠ, v);
                        found_language_list = true;
                    }
                    જો (is_ગાંઠ(ગાંઠ, "countryList")) {
                        parse_country_list(ગાંઠ, v);
                        found_country_list = true;
                    }
                }
                જો (!found_language_list) {
                    // inherit from parent layout
                    સંરચના rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        સંરચના rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                જો (!found_country_list) {
                    // inherit from parent layout
                    સંરચના rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        સંરચના rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } નહીંતર {
            config_item_free(&config);
        }
    }
}

સ્થિર શૂન્ય
parse_variant_list(સંરચના rxkb_context *ctx, સંરચના rxkb_layout *l,
                   xml_ગાંઠ *variant_list, ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;

    માટે (ગાંઠ = variant_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "variant"))
            parse_variant(ctx, l, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
parse_layout(સંરચના rxkb_context *ctx, xml_ગાંઠ *layout,
             ગણના rxkb_popularity popularity)
{
    સંરચના config_item config = config_item_new(popularity);
    સંરચના rxkb_layout *l;
    xml_ગાંઠ *ગાંઠ = ખાલી;
    bool exists = false;

    જો (!parse_config_item(ctx, layout, &config))
        પાછા;

    list_for_each(l, &ctx->layouts, base.link) {
        જો (streq(l->name, config.name) && l->variant == ખાલી) {
            exists = true;
            તોડો;
        }
    }

    જો (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ખાલી;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } નહીંતર {
        config_item_free(&config);
    }

    માટે (ગાંઠ = layout->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "variantList")) {
            parse_variant_list(ctx, l, ગાંઠ, popularity);
        }
        જો (!exists && is_ગાંઠ(ગાંઠ, "configItem")) {
            xml_ગાંઠ *ll;
            માટે (ll = ગાંઠ->children; ll; ll = ll->next) {
                જો (is_ગાંઠ(ll, "languageList"))
                    parse_language_list(ll, l);
                જો (is_ગાંઠ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

સ્થિર શૂન્ય
parse_layout_list(સંરચના rxkb_context *ctx, xml_ગાંઠ *layout_list,
                  ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;

    માટે (ગાંઠ = layout_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "layout"))
            parse_layout(ctx, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
parse_option(સંરચના rxkb_context *ctx, સંરચના rxkb_option_group *group,
             xml_ગાંઠ *option, ગણના rxkb_popularity popularity)
{
    સંરચના config_item config = config_item_new(popularity);

    જો (parse_config_item(ctx, option, &config)) {
        સંરચના rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            જો (streq(o->name, config.name)) {
                config_item_free(&config);
                પાછા;
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

સ્થિર શૂન્ય
parse_group(સંરચના rxkb_context *ctx, xml_ગાંઠ *group,
            ગણના rxkb_popularity popularity)
{
    સંરચના config_item config = config_item_new(popularity);
    સંરચના rxkb_option_group *g;
    xml_ગાંઠ *ગાંઠ = ખાલી;
    xmlChar *multiple;
    bool exists = false;

    જો (!parse_config_item(ctx, group, &config))
        પાછા;

    list_for_each(g, &ctx->option_groups, base.link) {
        જો (streq(g->name, config.name)) {
            exists = true;
            તોડો;
        }
    }

    જો (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (અચલ xmlChar*)"allowMultipleSelection");
        જો (multiple && xmlStrEqual(multiple, (અચલ xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } નહીંતર {
        config_item_free(&config);
    }

    માટે (ગાંઠ = group->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "option"))
            parse_option(ctx, g, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
parse_option_list(સંરચના rxkb_context *ctx, xml_ગાંઠ *option_list,
                  ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;

    માટે (ગાંઠ = option_list->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "group"))
            parse_group(ctx, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
parse_rules_xml(સંરચના rxkb_context *ctx, xml_ગાંઠ *મૂળ,
                ગણના rxkb_popularity popularity)
{
    xml_ગાંઠ *ગાંઠ = ખાલી;

    માટે (ગાંઠ = મૂળ->children; ગાંઠ; ગાંઠ = ગાંઠ->next) {
        જો (is_ગાંઠ(ગાંઠ, "modelList"))
            parse_model_list(ctx, ગાંઠ, popularity);
        નહીંતર જો (is_ગાંઠ(ગાંઠ, "layoutList"))
            parse_layout_list(ctx, ગાંઠ, popularity);
        નહીંતર જો (is_ગાંઠ(ગાંઠ, "optionList"))
            parse_option_list(ctx, ગાંઠ, popularity);
    }
}

સ્થિર શૂન્ય
ATTR_PRINTF(2, 0)
xml_error_func(શૂન્ય *ctx, અચલ અક્ષર *સંદેશ, ...)
{
    સ્થિર અક્ષર buf[PATH_MAX];
    સ્થિર size_t slen = 0;
    va_list args;
    પૂર્ણાંક rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, સંદેશ);
    rc = vsnprintf(&buf[slen], કદ(buf) - slen, સંદેશ, args);
    va_end(args);

    /* This shouldn't really happen */
    જો (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, કદ(buf));
        પાછા;
    }

    slen += (size_t)rc;
    જો (slen >= કદ(buf)) {
        /* truncated, let's flush this */
        buf[કદ(buf) - 1] = '\n';
        slen = કદ(buf);
    }

    /* We're assuming here that the last character is \n. */
    જો (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, કદ(buf));
        slen = 0;
    }
}

#જો HAVE_XML_CTXT_SET_ERRORHANDLER
સ્થિર શૂન્ય
xml_structured_error_func(શૂન્ય *userData, અચલ xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#નહીંતર
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

સ્થિર bool
validate(સંરચના rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    અચલ અક્ષર dtdstr[] =
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

#જો HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    જો (!xmlCtxt)
        પાછા false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_આગત_Ptr pinput =
        xml_New_આગત_From_Memory(ખાલી, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_આગત_BUF_STATIC);
    જો (!pinput)
        જાઓ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ખાલી, ખાલી);
#નહીંતર
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_આગત_Buffer_Ptr buf =
        xml_Parser_આગત_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    જો (!buf)
        જાઓ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ખાલી, buf, XML_CHAR_ENCODING_UTF8);
#endif

    જો (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        જાઓ dtd_error;
    }

#જો HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#નહીંતર
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    જો (!dtdvalid) {
        success = false;
        જાઓ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#જો HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    પાછા success;
}

સ્થિર bool
parse(સંરચના rxkb_context *ctx, અચલ અક્ષર *path,
      ગણના rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ખાલી;
    xml_ગાંઠ *મૂળ = ખાલી;

    જો (!check_eaccess(path, R_OK))
        પાછા false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    જો (!xmlCtxt)
        પાછા false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#નહીંતર
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#જો HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#જો !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, ખાલી, 0);
    જો (!doc)
        જાઓ parse_error;

    જો (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        જાઓ validate_error;
    }

    મૂળ = xml_Doc_Get_મૂળ_Element(doc);
    parse_rules_xml(ctx, મૂળ, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#જો !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(ખાલી, ખાલી);
#endif
#જો HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, ખાલી, ખાલી);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    પાછા success;
}
