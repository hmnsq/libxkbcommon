/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libxml/parser.h>

#ᱡᱩᱫᱤ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #include <limits.h>
    #ᱡᱩᱫᱤ HAVE_UNISTD_H && HAVE_DIRENT_H
        #include <dirent.h>
        #include <unistd.h>
    #endif
#endif

#include "xkbcommon/xkbregistry.h"
#include "messages-codes.h"
#include "darray.h"
#include "utils.h"
#include "util-list.h"
#include "util-mem.h"

sa_nracanaa rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
sa_nracanaa rxkb_object {
    sa_nracanaa rxkb_object *parent;
    uint32_t refcount;
    sa_nracanaa list link;
};

sa_nracanaa rxkb_iso639_code {
    sa_nracanaa rxkb_object base;
    var_na *code;
};

sa_nracanaa rxkb_iso3166_code {
    sa_nracanaa rxkb_object base;
    var_na *code;
};

ga_nanaa context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

sa_nracanaa rxkb_context {
    sa_nracanaa rxkb_object base;
    ga_nanaa context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    sa_nracanaa list models;         /* list of struct rxkb_models */
    sa_nracanaa list layouts;        /* list of struct rxkb_layouts */
    sa_nracanaa list option_groups;  /* list of struct rxkb_option_group */

    darray(var_na *) includes;


    ATTR_PRINTF(3, 0) void (*log_fn)(sa_nracanaa rxkb_context *ctx,
                                     ga_nanaa rxkb_log_level level,
                                     const var_na *fmt, va_list args);
    ga_nanaa rxkb_log_level log_level;

    void *userdata;
};

sa_nracanaa rxkb_model {
    sa_nracanaa rxkb_object base;

    var_na *name;
    var_na *vendor;
    var_na *description;
    ga_nanaa rxkb_popularity popularity;
};

sa_nracanaa rxkb_layout {
    sa_nracanaa rxkb_object base;

    var_na *name;
    var_na *brief;
    var_na *description;
    var_na *variant;
    ga_nanaa rxkb_popularity popularity;

    sa_nracanaa list iso639s;  /* list of struct rxkb_iso639_code */
    sa_nracanaa list iso3166s; /* list of struct rxkb_iso3166_code */
};

sa_nracanaa rxkb_option_group {
    sa_nracanaa rxkb_object base;

    bool allow_multiple;
    sa_nracanaa list options; /* list of struct rxkb_options */
    var_na *name;
    var_na *description;
    ga_nanaa rxkb_popularity popularity;
};

sa_nracanaa rxkb_option {
    sa_nracanaa rxkb_object base;

    var_na *name;
    var_na *brief;
    var_na *description;
    ga_nanaa rxkb_popularity popularity;
    bool layout_specific;
};

static bool
parse(sa_nracanaa rxkb_context *ctx, const var_na *path,
      ga_nanaa rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
static void
rxkb_log(sa_nracanaa rxkb_context *ctx, ga_nanaa rxkb_log_level level,
         const var_na *fmt, ...)
{
    va_list args;

    ᱡᱩᱫᱤ (ctx->log_level < level)
        ᱨᱩᱣᱟᱹᱲ;

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
#define rxkb_log_with_code(ctx, level, msg_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(msg_id, fmt), ##__VA_ARGS__)
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
sa_nracanaa type_ * type_##_ref(sa_nracanaa type_ *object) { \
    rxkb_object_ref(&object->base); \
    ᱨᱩᱣᱟᱹᱲ object; \
} \
sa_nracanaa type_ * type_##_unref(sa_nracanaa type_ *object) { \
    ᱡᱩᱫᱤ (!object) ᱨᱩᱣᱟᱹᱲ NULL; \
    assert(object->base.refcount >= 1); \
    ᱡᱩᱫᱤ (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        free(object); \
    } \
    ᱨᱩᱣᱟᱹᱲ NULL;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
static anta_hstha sa_nracanaa type_ * type_##_create(sa_nracanaa rxkb_object *parent) { \
    sa_nracanaa type_ *t = calloc(1, parimaa_na *t); \
    ᱡᱩᱫᱤ (t) \
        rxkb_object_init(&t->base, parent); \
    ᱨᱩᱣᱟᱹᱲ t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(sa_nracanaa type_ *object) { \
    ᱨᱩᱣᱟᱹᱲ object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, const var_na*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
sa_nracanaa type_ * type_##_first(sa_nracanaa parent_type_ *parent) { \
    sa_nracanaa type_ *o = NULL; \
    ᱡᱩᱫᱤ (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    ᱨᱩᱣᱟᱹᱲ o; \
} \
sa_nracanaa type_ * \
type_##_next(sa_nracanaa type_ *o) \
{ \
    sa_nracanaa parent_type_ *parent; \
    sa_nracanaa type_ *next; \
    parent = container_of(o->base.parent, sa_nracanaa parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    ᱡᱩᱫᱤ (list_is_last(&parent->parent_field_, &o->base.link)) \
        ᱨᱩᱣᱟᱹᱲ NULL; \
    ᱨᱩᱣᱟᱹᱲ next; \
}

static void
rxkb_object_init(sa_nracanaa rxkb_object *object, sa_nracanaa rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

static void *
rxkb_object_ref(sa_nracanaa rxkb_object *object)
{
    assert(object->refcount >= 1);
    ++object->refcount;
    ᱨᱩᱣᱟᱹᱲ object;
}

static void
rxkb_iso639_code_destroy(sa_nracanaa rxkb_iso639_code *code)
{
    free(code->code);
}

sa_nracanaa rxkb_iso639_code *
rxkb_layout_get_iso639_first(sa_nracanaa rxkb_layout *layout)
{
    sa_nracanaa rxkb_iso639_code *code = NULL;

    ᱡᱩᱫᱤ (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    ᱨᱩᱣᱟᱹᱲ code;
}

sa_nracanaa rxkb_iso639_code *
rxkb_iso639_code_next(sa_nracanaa rxkb_iso639_code *code)
{
    sa_nracanaa rxkb_iso639_code *next = NULL;
    sa_nracanaa rxkb_layout *layout;

    layout = container_of(code->base.parent, sa_nracanaa rxkb_layout, base);

    ᱡᱩᱫᱤ (list_is_last(&layout->iso639s, &code->base.link))
        ᱨᱩᱣᱟᱹᱲ NULL;

    next = list_first_entry(&code->base.link, code, base.link);

    ᱨᱩᱣᱟᱹᱲ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

static void
rxkb_iso3166_code_destroy(sa_nracanaa rxkb_iso3166_code *code)
{
    free(code->code);
}

sa_nracanaa rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(sa_nracanaa rxkb_layout *layout)
{
    sa_nracanaa rxkb_iso3166_code *code = NULL;

    ᱡᱩᱫᱤ (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    ᱨᱩᱣᱟᱹᱲ code;
}

sa_nracanaa rxkb_iso3166_code *
rxkb_iso3166_code_next(sa_nracanaa rxkb_iso3166_code *code)
{
    sa_nracanaa rxkb_iso3166_code *next = NULL;
    sa_nracanaa rxkb_layout *layout;

    layout = container_of(code->base.parent, sa_nracanaa rxkb_layout, base);

    ᱡᱩᱫᱤ (list_is_last(&layout->iso3166s, &code->base.link))
        ᱨᱩᱣᱟᱹᱲ NULL;

    next = list_first_entry(&code->base.link, code, base.link);

    ᱨᱩᱣᱟᱹᱲ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

static void
rxkb_option_destroy(sa_nracanaa rxkb_option *o)
{
    free(o->name);
    free(o->brief);
    free(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ga_nanaa rxkb_popularity);
bool rxkb_option_is_layout_specific(sa_nracanaa rxkb_option *object) {
    ᱨᱩᱣᱟᱹᱲ object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

static void
rxkb_layout_destroy(sa_nracanaa rxkb_layout *l)
{
    sa_nracanaa rxkb_iso639_code *iso639, *tmp_639;
    sa_nracanaa rxkb_iso3166_code *iso3166, *tmp_3166;

    free(l->name);
    free(l->brief);
    free(l->description);
    free(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ga_nanaa rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

static void
rxkb_model_destroy(sa_nracanaa rxkb_model *m)
{
    free(m->name);
    free(m->vendor);
    free(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ga_nanaa rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

static void
rxkb_option_group_destroy(sa_nracanaa rxkb_option_group *og)
{
    sa_nracanaa rxkb_option *o, *otmp;

    free(og->name);
    free(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(sa_nracanaa rxkb_option_group *g)
{
    ᱨᱩᱣᱟᱹᱲ g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, ga_nanaa rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

static void
rxkb_context_destroy(sa_nracanaa rxkb_context *ctx)
{
    sa_nracanaa rxkb_model *m, *mtmp;
    sa_nracanaa rxkb_layout *l, *ltmp;
    sa_nracanaa rxkb_option_group *og, *ogtmp;
    var_na **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    assert(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    assert(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    assert(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        free(*path);
    darray_free(ctx->includes);

    assert(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ga_nanaa rxkb_log_level);

static var_na *
rxkb_context_getenv(sa_nracanaa rxkb_context *ctx, const var_na *name)
{
    ᱡᱩᱫᱤ (ctx->use_secure_getenv) {
        ᱨᱩᱣᱟᱹᱲ secure_getenv(name);
    } ᱵᱟᱝ {
        ᱨᱩᱣᱟᱹᱲ getenv(name);
    }
}


void
rxkb_context_set_log_level(sa_nracanaa rxkb_context *ctx,
                           ga_nanaa rxkb_log_level level)
{
    ctx->log_level = level;
}

static const var_na *
log_level_to_prefix(ga_nanaa rxkb_log_level level)
{
    switch (level) {
    pak_sa RXKB_LOG_LEVEL_DEBUG:
        ᱨᱩᱣᱟᱹᱲ "xkbregistry: DEBUG: ";
    pak_sa RXKB_LOG_LEVEL_INFO:
        ᱨᱩᱣᱟᱹᱲ "xkbregistry: INFO: ";
    pak_sa RXKB_LOG_LEVEL_WARNING:
        ᱨᱩᱣᱟᱹᱲ "xkbregistry: WARNING: ";
    pak_sa RXKB_LOG_LEVEL_ERROR:
        ᱨᱩᱣᱟᱹᱲ "xkbregistry: ERROR: ";
    pak_sa RXKB_LOG_LEVEL_CRITICAL:
        ᱨᱩᱣᱟᱹᱲ "xkbregistry: CRITICAL: ";
    default:
        ᱨᱩᱣᱟᱹᱲ NULL;
    }
}

ATTR_PRINTF(3, 0) static void
default_log_fn(sa_nracanaa rxkb_context *ctx, ga_nanaa rxkb_log_level level,
               const var_na *fmt, va_list args)
{
    const var_na *prefix = log_level_to_prefix(level);

    ᱡᱩᱫᱤ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

static ga_nanaa rxkb_log_level
log_level(const var_na *level) {
    errno = 0;
    var_na *endptr;
    const long lvl = strtol(level, &endptr, 10);
    ᱡᱩᱫᱤ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ᱨᱩᱣᱟᱹᱲ (ga_nanaa rxkb_log_level)lvl;

    ᱡᱩᱫᱤ (istreq_prefix("crit", level))
        ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_CRITICAL;
    ᱡᱩᱫᱤ (istreq_prefix("err", level))
        ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_ERROR;
    ᱡᱩᱫᱤ (istreq_prefix("warn", level))
        ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_WARNING;
    ᱡᱩᱫᱤ (istreq_prefix("info", level))
        ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_INFO;
    ᱡᱩᱫᱤ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_DEBUG;

    ᱨᱩᱣᱟᱹᱲ RXKB_LOG_LEVEL_ERROR;
}

sa_nracanaa rxkb_context *
rxkb_context_new(ga_nanaa rxkb_context_flags flags)
{
    sa_nracanaa rxkb_context *ctx = rxkb_context_create(NULL);
    const var_na *env;

    ᱡᱩᱫᱤ (!ctx)
        ᱨᱩᱣᱟᱹᱲ NULL;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ᱡᱩᱫᱤ (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    static const ga_nanaa rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ᱡᱩᱫᱤ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        free(ctx);
        ᱨᱩᱣᱟᱹᱲ NULL;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ᱡᱩᱫᱤ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ROOT);
        rxkb_context_unref(ctx);
        ᱨᱩᱣᱟᱹᱲ NULL;
    }

    ᱨᱩᱣᱟᱹᱲ ctx;
}

void
rxkb_context_set_log_fn(sa_nracanaa rxkb_context *ctx,
                        void (*log_fn)(sa_nracanaa rxkb_context *ctx,
                                       ga_nanaa rxkb_log_level level,
                                       const var_na *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(sa_nracanaa rxkb_context *ctx, const var_na *path)
{
    puur_naa_nka err = 0;

    ᱡᱩᱫᱤ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        goto error;
    }

    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ᱡᱩᱫᱤ (err != 0) {
        err = errno;
        goto error;
    }
    ᱡᱩᱫᱤ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto error;
    }

    ᱡᱩᱫᱤ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    var_na rules[PATH_MAX];
    ᱡᱩᱫᱤ (!snprintf_safe(rules, parimaa_na(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                parimaa_na(rules), path, DEFAULT_XKB_RULES);
        goto error;
    }

    var_na * const tmp = strdup(path);
    ᱡᱩᱫᱤ (!tmp) {
        err = ENOMEM;
        goto error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ᱨᱩᱣᱟᱹᱲ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ᱨᱩᱣᱟᱹᱲ false;
}

#ᱡᱩᱫᱤ HAVE_XKB_EXTENSIONS_DIRECTORIES

static puur_naa_nka
compare_str(const void *a, const void *b)
{
    ᱨᱩᱣᱟᱹᱲ strcmp(*(var_na **)a, *(var_na **) b);
}

static puur_naa_nka
add_direct_subdirectories(sa_nracanaa rxkb_context *ctx, const var_na *path,
                          darray_string *extensions,
                          darray_size_t versioned_count,
                          size_t versioned_path_length)
{
    puur_naa_nka ret = 0;
    puur_naa_nka err = ENOMEM;
    DIR *dir = NULL;

    /* Check extensions parent directory */
    sa_nracanaa stat stat_buf;
    err = stat(path, &stat_buf);
    ᱡᱩᱫᱤ (err != 0) {
        err = errno;
        goto err;
    }
    ᱡᱩᱫᱤ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }
    ᱡᱩᱫᱤ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    dir = opendir(path);
    ᱡᱩᱫᱤ (dir == NULL) {
        err = EACCES;
        goto err;
    }

    sa_nracanaa dirent *entry;
    var_na path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ᱡᱚᱠᱷᱚᱱ ((entry = readdir(dir)) != NULL) {
        const var_na *name = entry->d_name;

        /* Skip special entries */
        ᱡᱩᱫᱤ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        ᱡᱩᱫᱤ (!snprintf_safe(path_buf, parimaa_na(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            goto err;
        }
        ᱡᱩᱫᱤ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            continue;

        /* Skip if the corresponding versioned directory is already included */
        for (darray_size_t i = 0; i < versioned_count; i++) {
            const var_na * const prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ᱡᱩᱫᱤ (strcmp(name, prev_name) == 0)
                goto next;
        }

        var_na *ext_path = strdup_safe(path_buf);
        ᱡᱩᱫᱤ (!ext_path) {
            err = ENOMEM;
            goto err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ᱡᱩᱫᱤ (darray_size(*extensions) > versioned_count) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_items(*extensions) + versioned_count,
              darray_size(*extensions) - versioned_count,
              parimaa_na(*darray_items(*extensions)), &compare_str);
        var_na **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_count) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    ᱨᱩᱣᱟᱹᱲ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ᱡᱩᱫᱤ (dir)
        closedir(dir);

    ᱨᱩᱣᱟᱹᱲ ret;
}
#endif

bool
rxkb_context_include_path_append_default(sa_nracanaa rxkb_context *ctx)
{
    var_na user_path[PATH_MAX];
    bool ret = false;

    ᱡᱩᱫᱤ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ᱨᱩᱣᱟᱹᱲ false;
    }

    const var_na * const home = rxkb_context_getenv(ctx, "HOME");
    const var_na * const xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ᱡᱩᱫᱤ (xdg != NULL) {
        ᱡᱩᱫᱤ (snprintf_safe(user_path, parimaa_na(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } ᱵᱟᱝ ᱡᱩᱫᱤ (home != NULL) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ᱡᱩᱫᱤ (snprintf_safe(user_path, parimaa_na(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ᱡᱩᱫᱤ (home != NULL) {
        ᱡᱩᱫᱤ (snprintf_safe(user_path, parimaa_na(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    const var_na * const extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != NULL) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#ᱡᱩᱫᱤ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    const var_na *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    ᱡᱩᱫᱤ (extensions_path == NULL)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    ᱡᱩᱫᱤ (extensions_path != NULL) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    ᱡᱩᱫᱤ (extensions_path == NULL)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    ᱡᱩᱫᱤ (extensions_path != NULL) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    var_na **ext_path;
    darray_foreach(ext_path, extensions) {
        free(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    const var_na * const root = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    const bool has_root = rxkb_context_include_path_append(
        ctx, ((root != NULL) ? root: DFLT_XKB_CONFIG_ROOT)
    );
    ret |= has_root;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ᱡᱩᱫᱤ (!has_root && (root == NULL || root[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ROOT,
                 ((root == NULL) ? DFLT_XKB_CONFIG_ROOT : root));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_ROOT);
    }

    ᱨᱩᱣᱟᱹᱲ ret;
}

bool
rxkb_context_parse_default_ruleset(sa_nracanaa rxkb_context *ctx)
{
    ᱨᱩᱣᱟᱹᱲ rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(sa_nracanaa rxkb_context *ctx, const var_na *ruleset)
{
    var_na **path;
    bool success = false;

    ᱡᱩᱫᱤ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        ᱨᱩᱣᱟᱹᱲ false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        var_na rules[PATH_MAX];

        ᱡᱩᱫᱤ (snprintf_safe(rules, parimaa_na(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ᱡᱩᱫᱤ (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ᱡᱩᱫᱤ (ctx->load_extra_rules_files &&
            snprintf_safe(rules, parimaa_na(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ᱡᱩᱫᱤ (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ᱨᱩᱣᱟᱹᱲ success;
}


void
rxkb_context_set_user_data(sa_nracanaa rxkb_context *ctx, void *userdata)
{
    ctx->userdata = userdata;
}

void *
rxkb_context_get_user_data(sa_nracanaa rxkb_context *ctx)
{
    ᱨᱩᱣᱟᱹᱲ ctx->userdata;
}

static anta_hstha bool
is_node(xmlNode *node, const var_na *name)
{
    ᱨᱩᱣᱟᱹᱲ node->type == XML_ELEMENT_NODE &&
        xmlStrEqual(node->name, (const xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
static var_na *
extract_text(xmlNode *node)
{
    xmlNode *n;

    for (n = node->children; n; n = n->next) {
        ᱡᱩᱫᱤ (n->type == XML_TEXT_NODE)
            ᱨᱩᱣᱟᱹᱲ (var_na *)xmlStrdup(n->content);
    }
    ᱨᱩᱣᱟᱹᱲ NULL;
}

/* Data from “configItem” node */
sa_nracanaa config_item {
    var_na *name;
    var_na *description;
    var_na *brief;
    var_na *vendor;
    ga_nanaa rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = NULL, \
    .description = NULL, \
    .brief = NULL, \
    .vendor = NULL, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

static void
config_item_free(sa_nracanaa config_item *config) {
    free(config->name);
    free(config->description);
    free(config->brief);
    free(config->vendor);
}

static bool
parse_config_item(sa_nracanaa rxkb_context *ctx, xmlNode *parent,
                  sa_nracanaa config_item *config)
{
    xmlNode *node = NULL;
    xmlNode *ci = NULL;

    for (ci = parent->children; ci; ci = ci->next) {
        ᱡᱩᱫᱤ (is_node(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (const xmlChar*)"popularity");
            ᱡᱩᱫᱤ (raw_popularity) {
                ᱡᱩᱫᱤ (xmlStrEqual(raw_popularity, (const xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ᱵᱟᱝ ᱡᱩᱫᱤ (xmlStrEqual(raw_popularity, (const xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ᱵᱟᱝ
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->line, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (const xmlChar*)"layout-specific");
            ᱡᱩᱫᱤ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (const xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            for (node = ci->children; node; node = node->next) {
                ᱡᱩᱫᱤ (is_node(node, "name"))
                    config->name = extract_text(node);
                ᱵᱟᱝ ᱡᱩᱫᱤ (is_node(node, "description"))
                    config->description = extract_text(node);
                ᱵᱟᱝ ᱡᱩᱫᱤ (is_node(node, "shortDescription"))
                    config->brief = extract_text(node);
                ᱵᱟᱝ ᱡᱩᱫᱤ (is_node(node, "vendor"))
                    config->vendor = extract_text(node);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ᱡᱩᱫᱤ (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->line);
                config_item_free(config);
                ᱨᱩᱣᱟᱹᱲ false;
            }

            ᱨᱩᱣᱟᱹᱲ true; /* only one configItem allowed in the dtd */
        }
    }

    ᱨᱩᱣᱟᱹᱲ false;
}

static void
parse_model(sa_nracanaa rxkb_context *ctx, xmlNode *model,
            ga_nanaa rxkb_popularity popularity)
{
    sa_nracanaa config_item config = config_item_new(popularity);

    ᱡᱩᱫᱤ (parse_config_item(ctx, model, &config)) {
        sa_nracanaa rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            ᱡᱩᱫᱤ (streq(m->name, config.name)) {
                config_item_free(&config);
                ᱨᱩᱣᱟᱹᱲ;
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

static void
parse_model_list(sa_nracanaa rxkb_context *ctx, xmlNode *model_list,
                 ga_nanaa rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    for (node = model_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "model"))
            parse_model(ctx, node, popularity);
    }
}

static void
parse_language_list(xmlNode *language_list, sa_nracanaa rxkb_layout *layout)
{
    xmlNode *node = NULL;
    sa_nracanaa rxkb_iso639_code *code;

    for (node = language_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "iso639Id")) {
            var_na *str = extract_text(node);
            sa_nracanaa rxkb_object *parent;

            ᱡᱩᱫᱤ (!str || strlen(str) != 3) {
                free(str);
                continue;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

static void
parse_country_list(xmlNode *country_list, sa_nracanaa rxkb_layout *layout)
{
    xmlNode *node = NULL;
    sa_nracanaa rxkb_iso3166_code *code;

    for (node = country_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "iso3166Id")) {
            var_na *str = extract_text(node);
            sa_nracanaa rxkb_object *parent;

            ᱡᱩᱫᱤ (!str || strlen(str) != 2) {
                free(str);
                continue;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

static void
parse_variant(sa_nracanaa rxkb_context *ctx, sa_nracanaa rxkb_layout *l,
              xmlNode *variant, ga_nanaa rxkb_popularity popularity)
{
    xmlNode *ci;
    sa_nracanaa config_item config = config_item_new(popularity);

    ᱡᱩᱫᱤ (parse_config_item(ctx, variant, &config)) {
        sa_nracanaa rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            ᱡᱩᱫᱤ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                bha_nga;
            }
        }

        ᱡᱩᱫᱤ (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == NULL ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            for (ci = variant->children; ci; ci = ci->next) {
                xmlNode *node;

                ᱡᱩᱫᱤ (!is_node(ci, "configItem"))
                    continue;

                bool found_language_list = false;
                bool found_country_list = false;
                for (node = ci->children; node; node = node->next) {
                    ᱡᱩᱫᱤ (is_node(node, "languageList")) {
                        parse_language_list(node, v);
                        found_language_list = true;
                    }
                    ᱡᱩᱫᱤ (is_node(node, "countryList")) {
                        parse_country_list(node, v);
                        found_country_list = true;
                    }
                }
                ᱡᱩᱫᱤ (!found_language_list) {
                    // inherit from parent layout
                    sa_nracanaa rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        sa_nracanaa rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                ᱡᱩᱫᱤ (!found_country_list) {
                    // inherit from parent layout
                    sa_nracanaa rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        sa_nracanaa rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } ᱵᱟᱝ {
            config_item_free(&config);
        }
    }
}

static void
parse_variant_list(sa_nracanaa rxkb_context *ctx, sa_nracanaa rxkb_layout *l,
                   xmlNode *variant_list, ga_nanaa rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    for (node = variant_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "variant"))
            parse_variant(ctx, l, node, popularity);
    }
}

static void
parse_layout(sa_nracanaa rxkb_context *ctx, xmlNode *layout,
             ga_nanaa rxkb_popularity popularity)
{
    sa_nracanaa config_item config = config_item_new(popularity);
    sa_nracanaa rxkb_layout *l;
    xmlNode *node = NULL;
    bool exists = false;

    ᱡᱩᱫᱤ (!parse_config_item(ctx, layout, &config))
        ᱨᱩᱣᱟᱹᱲ;

    list_for_each(l, &ctx->layouts, base.link) {
        ᱡᱩᱫᱤ (streq(l->name, config.name) && l->variant == NULL) {
            exists = true;
            bha_nga;
        }
    }

    ᱡᱩᱫᱤ (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = NULL;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } ᱵᱟᱝ {
        config_item_free(&config);
    }

    for (node = layout->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "variantList")) {
            parse_variant_list(ctx, l, node, popularity);
        }
        ᱡᱩᱫᱤ (!exists && is_node(node, "configItem")) {
            xmlNode *ll;
            for (ll = node->children; ll; ll = ll->next) {
                ᱡᱩᱫᱤ (is_node(ll, "languageList"))
                    parse_language_list(ll, l);
                ᱡᱩᱫᱤ (is_node(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

static void
parse_layout_list(sa_nracanaa rxkb_context *ctx, xmlNode *layout_list,
                  ga_nanaa rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    for (node = layout_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "layout"))
            parse_layout(ctx, node, popularity);
    }
}

static void
parse_option(sa_nracanaa rxkb_context *ctx, sa_nracanaa rxkb_option_group *group,
             xmlNode *option, ga_nanaa rxkb_popularity popularity)
{
    sa_nracanaa config_item config = config_item_new(popularity);

    ᱡᱩᱫᱤ (parse_config_item(ctx, option, &config)) {
        sa_nracanaa rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            ᱡᱩᱫᱤ (streq(o->name, config.name)) {
                config_item_free(&config);
                ᱨᱩᱣᱟᱹᱲ;
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

static void
parse_group(sa_nracanaa rxkb_context *ctx, xmlNode *group,
            ga_nanaa rxkb_popularity popularity)
{
    sa_nracanaa config_item config = config_item_new(popularity);
    sa_nracanaa rxkb_option_group *g;
    xmlNode *node = NULL;
    xmlChar *multiple;
    bool exists = false;

    ᱡᱩᱫᱤ (!parse_config_item(ctx, group, &config))
        ᱨᱩᱣᱟᱹᱲ;

    list_for_each(g, &ctx->option_groups, base.link) {
        ᱡᱩᱫᱤ (streq(g->name, config.name)) {
            exists = true;
            bha_nga;
        }
    }

    ᱡᱩᱫᱤ (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (const xmlChar*)"allowMultipleSelection");
        ᱡᱩᱫᱤ (multiple && xmlStrEqual(multiple, (const xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } ᱵᱟᱝ {
        config_item_free(&config);
    }

    for (node = group->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "option"))
            parse_option(ctx, g, node, popularity);
    }
}

static void
parse_option_list(sa_nracanaa rxkb_context *ctx, xmlNode *option_list,
                  ga_nanaa rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    for (node = option_list->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "group"))
            parse_group(ctx, node, popularity);
    }
}

static void
parse_rules_xml(sa_nracanaa rxkb_context *ctx, xmlNode *root,
                ga_nanaa rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    for (node = root->children; node; node = node->next) {
        ᱡᱩᱫᱤ (is_node(node, "modelList"))
            parse_model_list(ctx, node, popularity);
        ᱵᱟᱝ ᱡᱩᱫᱤ (is_node(node, "layoutList"))
            parse_layout_list(ctx, node, popularity);
        ᱵᱟᱝ ᱡᱩᱫᱤ (is_node(node, "optionList"))
            parse_option_list(ctx, node, popularity);
    }
}

static void
ATTR_PRINTF(2, 0)
xml_error_func(void *ctx, const var_na *msg, ...)
{
    static var_na buf[PATH_MAX];
    static size_t slen = 0;
    va_list args;
    puur_naa_nka rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, msg);
    rc = vsnprintf(&buf[slen], parimaa_na(buf) - slen, msg, args);
    va_end(args);

    /* This shouldn't really happen */
    ᱡᱩᱫᱤ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, parimaa_na(buf));
        ᱨᱩᱣᱟᱹᱲ;
    }

    slen += (size_t)rc;
    ᱡᱩᱫᱤ (slen >= parimaa_na(buf)) {
        /* truncated, let's flush this */
        buf[parimaa_na(buf) - 1] = '\n';
        slen = parimaa_na(buf);
    }

    /* We're assuming here that the last character is \n. */
    ᱡᱩᱫᱤ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, parimaa_na(buf));
        slen = 0;
    }
}

#ᱡᱩᱫᱤ HAVE_XML_CTXT_SET_ERRORHANDLER
static void
xml_structured_error_func(void *userData, const xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ᱵᱟᱝ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

static bool
validate(sa_nracanaa rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    const var_na dtdstr[] =
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

#ᱡᱩᱫᱤ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ᱡᱩᱫᱤ (!xmlCtxt)
        ᱨᱩᱣᱟᱹᱲ false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xmlParserInputPtr pinput =
        xmlNewInputFromMemory(NULL, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_INPUT_BUF_STATIC);
    ᱡᱩᱫᱤ (!pinput)
        goto dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, NULL, NULL);
#ᱵᱟᱝ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xmlParserInputBufferPtr buf =
        xmlParserInputBufferCreateMem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ᱡᱩᱫᱤ (!buf)
        goto dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(NULL, buf, XML_CHAR_ENCODING_UTF8);
#endif

    ᱡᱩᱫᱤ (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        goto dtd_error;
    }

#ᱡᱩᱫᱤ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ᱵᱟᱝ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ᱡᱩᱫᱤ (!dtdvalid) {
        success = false;
        goto ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#ᱡᱩᱫᱤ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    ᱨᱩᱣᱟᱹᱲ success;
}

static bool
parse(sa_nracanaa rxkb_context *ctx, const var_na *path,
      ga_nanaa rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = NULL;
    xmlNode *root = NULL;

    ᱡᱩᱫᱤ (!check_eaccess(path, R_OK))
        ᱨᱩᱣᱟᱹᱲ false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ᱡᱩᱫᱤ (!xmlCtxt)
        ᱨᱩᱣᱟᱹᱲ false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ᱵᱟᱝ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ᱡᱩᱫᱤ HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#ᱡᱩᱫᱤ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, NULL, 0);
    ᱡᱩᱫᱤ (!doc)
        goto parse_error;

    ᱡᱩᱫᱤ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        goto validate_error;
    }

    root = xmlDocGetRootElement(doc);
    parse_rules_xml(ctx, root, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ᱡᱩᱫᱤ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(NULL, NULL);
#endif
#ᱡᱩᱫᱤ HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, NULL, NULL);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    ᱨᱩᱣᱟᱹᱲ success;
}
