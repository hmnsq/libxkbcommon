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

#如果 HAVE_XKB_EXTENSIONS_DIRECTORIES
    #include <limits.h>
    #如果 HAVE_UNISTD_H && HAVE_DIRENT_H
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

struct rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
struct rxkb_object {
    struct rxkb_object *parent;
    uint32_t refcount;
    struct list link;
};

struct rxkb_iso639_code {
    struct rxkb_object base;
    char *code;
};

struct rxkb_iso3166_code {
    struct rxkb_object base;
    char *code;
};

enum context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

struct rxkb_context {
    struct rxkb_object base;
    enum context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    struct list models;         /* list of struct rxkb_models */
    struct list layouts;        /* list of struct rxkb_layouts */
    struct list option_groups;  /* list of struct rxkb_option_group */

    darray(char *) includes;


    ATTR_PRINTF(3, 0) void (*log_fn)(struct rxkb_context *ctx,
                                     enum rxkb_log_level level,
                                     const char *fmt, va_list args);
    enum rxkb_log_level log_level;

    void *userdata;
};

struct rxkb_model {
    struct rxkb_object base;

    char *name;
    char *vendor;
    char *description;
    enum rxkb_popularity popularity;
};

struct rxkb_layout {
    struct rxkb_object base;

    char *name;
    char *brief;
    char *description;
    char *variant;
    enum rxkb_popularity popularity;

    struct list iso639s;  /* list of struct rxkb_iso639_code */
    struct list iso3166s; /* list of struct rxkb_iso3166_code */
};

struct rxkb_option_group {
    struct rxkb_object base;

    bool allow_multiple;
    struct list options; /* list of struct rxkb_options */
    char *name;
    char *description;
    enum rxkb_popularity popularity;
};

struct rxkb_option {
    struct rxkb_object base;

    char *name;
    char *brief;
    char *description;
    enum rxkb_popularity popularity;
    bool layout_specific;
};

static bool
parse(struct rxkb_context *ctx, const char *path,
      enum rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
static void
rxkb_log(struct rxkb_context *ctx, enum rxkb_log_level level,
         const char *fmt, ...)
{
    va_list args;

    如果 (ctx->log_level < level)
        返回;

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
struct type_ * type_##_ref(struct type_ *object) { \
    rxkb_object_ref(&object->base); \
    返回 object; \
} \
struct type_ * type_##_unref(struct type_ *object) { \
    如果 (!object) 返回 NULL; \
    assert(object->base.refcount >= 1); \
    如果 (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        free(object); \
    } \
    返回 NULL;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
static inline struct type_ * type_##_create(struct rxkb_object *parent) { \
    struct type_ *t = calloc(1, sizeof *t); \
    如果 (t) \
        rxkb_object_init(&t->base, parent); \
    返回 t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(struct type_ *object) { \
    返回 object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, const char*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
struct type_ * type_##_first(struct parent_type_ *parent) { \
    struct type_ *o = NULL; \
    如果 (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    返回 o; \
} \
struct type_ * \
type_##_next(struct type_ *o) \
{ \
    struct parent_type_ *parent; \
    struct type_ *next; \
    parent = container_of(o->base.parent, struct parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    如果 (list_is_last(&parent->parent_field_, &o->base.link)) \
        返回 NULL; \
    返回 next; \
}

static void
rxkb_object_init(struct rxkb_object *object, struct rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

static void *
rxkb_object_ref(struct rxkb_object *object)
{
    assert(object->refcount >= 1);
    ++object->refcount;
    返回 object;
}

static void
rxkb_iso639_code_destroy(struct rxkb_iso639_code *code)
{
    free(code->code);
}

struct rxkb_iso639_code *
rxkb_layout_get_iso639_first(struct rxkb_layout *layout)
{
    struct rxkb_iso639_code *code = NULL;

    如果 (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    返回 code;
}

struct rxkb_iso639_code *
rxkb_iso639_code_next(struct rxkb_iso639_code *code)
{
    struct rxkb_iso639_code *next = NULL;
    struct rxkb_layout *layout;

    layout = container_of(code->base.parent, struct rxkb_layout, base);

    如果 (list_is_last(&layout->iso639s, &code->base.link))
        返回 NULL;

    next = list_first_entry(&code->base.link, code, base.link);

    返回 next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

static void
rxkb_iso3166_code_destroy(struct rxkb_iso3166_code *code)
{
    free(code->code);
}

struct rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(struct rxkb_layout *layout)
{
    struct rxkb_iso3166_code *code = NULL;

    如果 (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    返回 code;
}

struct rxkb_iso3166_code *
rxkb_iso3166_code_next(struct rxkb_iso3166_code *code)
{
    struct rxkb_iso3166_code *next = NULL;
    struct rxkb_layout *layout;

    layout = container_of(code->base.parent, struct rxkb_layout, base);

    如果 (list_is_last(&layout->iso3166s, &code->base.link))
        返回 NULL;

    next = list_first_entry(&code->base.link, code, base.link);

    返回 next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

static void
rxkb_option_destroy(struct rxkb_option *o)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, enum rxkb_popularity);
bool rxkb_option_is_layout_specific(struct rxkb_option *object) {
    返回 object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

static void
rxkb_layout_destroy(struct rxkb_layout *l)
{
    struct rxkb_iso639_code *iso639, *tmp_639;
    struct rxkb_iso3166_code *iso3166, *tmp_3166;

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, enum rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

static void
rxkb_model_destroy(struct rxkb_model *m)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, enum rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

static void
rxkb_option_group_destroy(struct rxkb_option_group *og)
{
    struct rxkb_option *o, *otmp;

    free(og->name);
    free(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(struct rxkb_option_group *g)
{
    返回 g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, enum rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

static void
rxkb_context_destroy(struct rxkb_context *ctx)
{
    struct rxkb_model *m, *mtmp;
    struct rxkb_layout *l, *ltmp;
    struct rxkb_option_group *og, *ogtmp;
    char **path;

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, enum rxkb_log_level);

static char *
rxkb_context_getenv(struct rxkb_context *ctx, const char *name)
{
    如果 (ctx->use_secure_getenv) {
        返回 secure_getenv(name);
    } 否则 {
        返回 getenv(name);
    }
}


void
rxkb_context_set_log_level(struct rxkb_context *ctx,
                           enum rxkb_log_level level)
{
    ctx->log_level = level;
}

static const char *
log_level_to_prefix(enum rxkb_log_level level)
{
    switch (level) {
    case RXKB_LOG_LEVEL_DEBUG:
        返回 "xkbregistry: DEBUG: ";
    case RXKB_LOG_LEVEL_INFO:
        返回 "xkbregistry: INFO: ";
    case RXKB_LOG_LEVEL_WARNING:
        返回 "xkbregistry: WARNING: ";
    case RXKB_LOG_LEVEL_ERROR:
        返回 "xkbregistry: ERROR: ";
    case RXKB_LOG_LEVEL_CRITICAL:
        返回 "xkbregistry: CRITICAL: ";
    default:
        返回 NULL;
    }
}

ATTR_PRINTF(3, 0) static void
default_log_fn(struct rxkb_context *ctx, enum rxkb_log_level level,
               const char *fmt, va_list args)
{
    const char *prefix = log_level_to_prefix(level);

    如果 (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

static enum rxkb_log_level
log_level(const char *level) {
    errno = 0;
    char *endptr;
    const long lvl = strtol(level, &endptr, 10);
    如果 (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        返回 (enum rxkb_log_level)lvl;

    如果 (istreq_prefix("crit", level))
        返回 RXKB_LOG_LEVEL_CRITICAL;
    如果 (istreq_prefix("err", level))
        返回 RXKB_LOG_LEVEL_ERROR;
    如果 (istreq_prefix("warn", level))
        返回 RXKB_LOG_LEVEL_WARNING;
    如果 (istreq_prefix("info", level))
        返回 RXKB_LOG_LEVEL_INFO;
    如果 (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        返回 RXKB_LOG_LEVEL_DEBUG;

    返回 RXKB_LOG_LEVEL_ERROR;
}

struct rxkb_context *
rxkb_context_new(enum rxkb_context_flags flags)
{
    struct rxkb_context *ctx = rxkb_context_create(NULL);
    const char *env;

    如果 (!ctx)
        返回 NULL;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    如果 (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    static const enum rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    如果 (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        free(ctx);
        返回 NULL;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    如果 (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ROOT);
        rxkb_context_unref(ctx);
        返回 NULL;
    }

    返回 ctx;
}

void
rxkb_context_set_log_fn(struct rxkb_context *ctx,
                        void (*log_fn)(struct rxkb_context *ctx,
                                       enum rxkb_log_level level,
                                       const char *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(struct rxkb_context *ctx, const char *path)
{
    整数 err = 0;

    如果 (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        goto error;
    }

    struct stat stat_buf;
    err = stat(path, &stat_buf);
    如果 (err != 0) {
        err = errno;
        goto error;
    }
    如果 (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto error;
    }

    如果 (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    char rules[PATH_MAX];
    如果 (!snprintf_safe(rules, sizeof(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                sizeof(rules), path, DEFAULT_XKB_RULES);
        goto error;
    }

    char * const tmp = strdup(path);
    如果 (!tmp) {
        err = ENOMEM;
        goto error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    返回 true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    返回 false;
}

#如果 HAVE_XKB_EXTENSIONS_DIRECTORIES

static 整数
compare_str(const void *a, const void *b)
{
    返回 strcmp(*(char **)a, *(char **) b);
}

static 整数
add_direct_subdirectories(struct rxkb_context *ctx, const char *path,
                          darray_string *extensions,
                          darray_size_t versioned_count,
                          size_t versioned_path_length)
{
    整数 ret = 0;
    整数 err = ENOMEM;
    DIR *dir = NULL;

    /* Check extensions parent directory */
    struct stat stat_buf;
    err = stat(path, &stat_buf);
    如果 (err != 0) {
        err = errno;
        goto err;
    }
    如果 (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        goto err;
    }
    如果 (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        goto err;
    }

    dir = opendir(path);
    如果 (dir == NULL) {
        err = EACCES;
        goto err;
    }

    struct dirent *entry;
    char path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    当 ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        /* Skip special entries */
        如果 (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        如果 (!snprintf_safe(path_buf, sizeof(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            goto err;
        }
        如果 (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            continue;

        /* Skip if the corresponding versioned directory is already included */
        为 (darray_size_t i = 0; i < versioned_count; i++) {
            const char * const prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            如果 (strcmp(name, prev_name) == 0)
                goto next;
        }

        char *ext_path = strdup_safe(path_buf);
        如果 (!ext_path) {
            err = ENOMEM;
            goto err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    如果 (darray_size(*extensions) > versioned_count) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_items(*extensions) + versioned_count,
              darray_size(*extensions) - versioned_count,
              sizeof(*darray_items(*extensions)), &compare_str);
        char **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_count) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    返回 ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    如果 (dir)
        closedir(dir);

    返回 ret;
}
#endif

bool
rxkb_context_include_path_append_default(struct rxkb_context *ctx)
{
    char user_path[PATH_MAX];
    bool ret = false;

    如果 (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        返回 false;
    }

    const char * const home = rxkb_context_getenv(ctx, "HOME");
    const char * const xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    如果 (xdg != NULL) {
        如果 (snprintf_safe(user_path, sizeof(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } 否则 如果 (home != NULL) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        如果 (snprintf_safe(user_path, sizeof(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    如果 (home != NULL) {
        如果 (snprintf_safe(user_path, sizeof(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    const char * const extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != NULL) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#如果 HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    const char *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    如果 (extensions_path == NULL)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    如果 (extensions_path != NULL) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    如果 (extensions_path == NULL)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    如果 (extensions_path != NULL) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    char **ext_path;
    darray_foreach(ext_path, extensions) {
        free(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    const char * const root = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
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
    如果 (!has_root && (root == NULL || root[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ROOT,
                 ((root == NULL) ? DFLT_XKB_CONFIG_ROOT : root));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_ROOT);
    }

    返回 ret;
}

bool
rxkb_context_parse_default_ruleset(struct rxkb_context *ctx)
{
    返回 rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(struct rxkb_context *ctx, const char *ruleset)
{
    char **path;
    bool success = false;

    如果 (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        返回 false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        char rules[PATH_MAX];

        如果 (snprintf_safe(rules, sizeof(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            如果 (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        如果 (ctx->load_extra_rules_files &&
            snprintf_safe(rules, sizeof(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            如果 (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    返回 success;
}


void
rxkb_context_set_user_data(struct rxkb_context *ctx, void *userdata)
{
    ctx->userdata = userdata;
}

void *
rxkb_context_get_user_data(struct rxkb_context *ctx)
{
    返回 ctx->userdata;
}

static inline bool
is_node(xmlNode *node, const char *name)
{
    返回 node->type == XML_ELEMENT_NODE &&
        xmlStrEqual(node->name, (const xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
static char *
extract_text(xmlNode *node)
{
    xmlNode *n;

    为 (n = node->children; n; n = n->next) {
        如果 (n->type == XML_TEXT_NODE)
            返回 (char *)xmlStrdup(n->content);
    }
    返回 NULL;
}

/* Data from “configItem” node */
struct config_item {
    char *name;
    char *description;
    char *brief;
    char *vendor;
    enum rxkb_popularity popularity;
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
config_item_free(struct config_item *config) {
    free(config->name);
    free(config->description);
    free(config->brief);
    free(config->vendor);
}

static bool
parse_config_item(struct rxkb_context *ctx, xmlNode *parent,
                  struct config_item *config)
{
    xmlNode *node = NULL;
    xmlNode *ci = NULL;

    为 (ci = parent->children; ci; ci = ci->next) {
        如果 (is_node(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (const xmlChar*)"popularity");
            如果 (raw_popularity) {
                如果 (xmlStrEqual(raw_popularity, (const xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                否则 如果 (xmlStrEqual(raw_popularity, (const xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                否则
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->line, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (const xmlChar*)"layout-specific");
            如果 (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (const xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            为 (node = ci->children; node; node = node->next) {
                如果 (is_node(node, "name"))
                    config->name = extract_text(node);
                否则 如果 (is_node(node, "description"))
                    config->description = extract_text(node);
                否则 如果 (is_node(node, "shortDescription"))
                    config->brief = extract_text(node);
                否则 如果 (is_node(node, "vendor"))
                    config->vendor = extract_text(node);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            如果 (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->line);
                config_item_free(config);
                返回 false;
            }

            返回 true; /* only one configItem allowed in the dtd */
        }
    }

    返回 false;
}

static void
parse_model(struct rxkb_context *ctx, xmlNode *model,
            enum rxkb_popularity popularity)
{
    struct config_item config = config_item_new(popularity);

    如果 (parse_config_item(ctx, model, &config)) {
        struct rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            如果 (streq(m->name, config.name)) {
                config_item_free(&config);
                返回;
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
parse_model_list(struct rxkb_context *ctx, xmlNode *model_list,
                 enum rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    为 (node = model_list->children; node; node = node->next) {
        如果 (is_node(node, "model"))
            parse_model(ctx, node, popularity);
    }
}

static void
parse_language_list(xmlNode *language_list, struct rxkb_layout *layout)
{
    xmlNode *node = NULL;
    struct rxkb_iso639_code *code;

    为 (node = language_list->children; node; node = node->next) {
        如果 (is_node(node, "iso639Id")) {
            char *str = extract_text(node);
            struct rxkb_object *parent;

            如果 (!str || strlen(str) != 3) {
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
parse_country_list(xmlNode *country_list, struct rxkb_layout *layout)
{
    xmlNode *node = NULL;
    struct rxkb_iso3166_code *code;

    为 (node = country_list->children; node; node = node->next) {
        如果 (is_node(node, "iso3166Id")) {
            char *str = extract_text(node);
            struct rxkb_object *parent;

            如果 (!str || strlen(str) != 2) {
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
parse_variant(struct rxkb_context *ctx, struct rxkb_layout *l,
              xmlNode *variant, enum rxkb_popularity popularity)
{
    xmlNode *ci;
    struct config_item config = config_item_new(popularity);

    如果 (parse_config_item(ctx, variant, &config)) {
        struct rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            如果 (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                break;
            }
        }

        如果 (!exists) {
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

            为 (ci = variant->children; ci; ci = ci->next) {
                xmlNode *node;

                如果 (!is_node(ci, "configItem"))
                    continue;

                bool found_language_list = false;
                bool found_country_list = false;
                为 (node = ci->children; node; node = node->next) {
                    如果 (is_node(node, "languageList")) {
                        parse_language_list(node, v);
                        found_language_list = true;
                    }
                    如果 (is_node(node, "countryList")) {
                        parse_country_list(node, v);
                        found_country_list = true;
                    }
                }
                如果 (!found_language_list) {
                    // inherit from parent layout
                    struct rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        struct rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                如果 (!found_country_list) {
                    // inherit from parent layout
                    struct rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        struct rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } 否则 {
            config_item_free(&config);
        }
    }
}

static void
parse_variant_list(struct rxkb_context *ctx, struct rxkb_layout *l,
                   xmlNode *variant_list, enum rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    为 (node = variant_list->children; node; node = node->next) {
        如果 (is_node(node, "variant"))
            parse_variant(ctx, l, node, popularity);
    }
}

static void
parse_layout(struct rxkb_context *ctx, xmlNode *layout,
             enum rxkb_popularity popularity)
{
    struct config_item config = config_item_new(popularity);
    struct rxkb_layout *l;
    xmlNode *node = NULL;
    bool exists = false;

    如果 (!parse_config_item(ctx, layout, &config))
        返回;

    list_for_each(l, &ctx->layouts, base.link) {
        如果 (streq(l->name, config.name) && l->variant == NULL) {
            exists = true;
            break;
        }
    }

    如果 (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = NULL;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } 否则 {
        config_item_free(&config);
    }

    为 (node = layout->children; node; node = node->next) {
        如果 (is_node(node, "variantList")) {
            parse_variant_list(ctx, l, node, popularity);
        }
        如果 (!exists && is_node(node, "configItem")) {
            xmlNode *ll;
            为 (ll = node->children; ll; ll = ll->next) {
                如果 (is_node(ll, "languageList"))
                    parse_language_list(ll, l);
                如果 (is_node(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

static void
parse_layout_list(struct rxkb_context *ctx, xmlNode *layout_list,
                  enum rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    为 (node = layout_list->children; node; node = node->next) {
        如果 (is_node(node, "layout"))
            parse_layout(ctx, node, popularity);
    }
}

static void
parse_option(struct rxkb_context *ctx, struct rxkb_option_group *group,
             xmlNode *option, enum rxkb_popularity popularity)
{
    struct config_item config = config_item_new(popularity);

    如果 (parse_config_item(ctx, option, &config)) {
        struct rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            如果 (streq(o->name, config.name)) {
                config_item_free(&config);
                返回;
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
parse_group(struct rxkb_context *ctx, xmlNode *group,
            enum rxkb_popularity popularity)
{
    struct config_item config = config_item_new(popularity);
    struct rxkb_option_group *g;
    xmlNode *node = NULL;
    xmlChar *multiple;
    bool exists = false;

    如果 (!parse_config_item(ctx, group, &config))
        返回;

    list_for_each(g, &ctx->option_groups, base.link) {
        如果 (streq(g->name, config.name)) {
            exists = true;
            break;
        }
    }

    如果 (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (const xmlChar*)"allowMultipleSelection");
        如果 (multiple && xmlStrEqual(multiple, (const xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } 否则 {
        config_item_free(&config);
    }

    为 (node = group->children; node; node = node->next) {
        如果 (is_node(node, "option"))
            parse_option(ctx, g, node, popularity);
    }
}

static void
parse_option_list(struct rxkb_context *ctx, xmlNode *option_list,
                  enum rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    为 (node = option_list->children; node; node = node->next) {
        如果 (is_node(node, "group"))
            parse_group(ctx, node, popularity);
    }
}

static void
parse_rules_xml(struct rxkb_context *ctx, xmlNode *root,
                enum rxkb_popularity popularity)
{
    xmlNode *node = NULL;

    为 (node = root->children; node; node = node->next) {
        如果 (is_node(node, "modelList"))
            parse_model_list(ctx, node, popularity);
        否则 如果 (is_node(node, "layoutList"))
            parse_layout_list(ctx, node, popularity);
        否则 如果 (is_node(node, "optionList"))
            parse_option_list(ctx, node, popularity);
    }
}

static void
ATTR_PRINTF(2, 0)
xml_error_func(void *ctx, const char *msg, ...)
{
    static char buf[PATH_MAX];
    static size_t slen = 0;
    va_list args;
    整数 rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, msg);
    rc = vsnprintf(&buf[slen], sizeof(buf) - slen, msg, args);
    va_end(args);

    /* This shouldn't really happen */
    如果 (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, sizeof(buf));
        返回;
    }

    slen += (size_t)rc;
    如果 (slen >= sizeof(buf)) {
        /* truncated, let's flush this */
        buf[sizeof(buf) - 1] = '\n';
        slen = sizeof(buf);
    }

    /* We're assuming here that the last character is \n. */
    如果 (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, sizeof(buf));
        slen = 0;
    }
}

#如果 HAVE_XML_CTXT_SET_ERRORHANDLER
static void
xml_structured_error_func(void *userData, const xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#否则
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

static bool
validate(struct rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    const char dtdstr[] =
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

#如果 HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    如果 (!xmlCtxt)
        返回 false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xmlParserInputPtr pinput =
        xmlNewInputFromMemory(NULL, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_INPUT_BUF_STATIC);
    如果 (!pinput)
        goto dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, NULL, NULL);
#否则
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xmlParserInputBufferPtr buf =
        xmlParserInputBufferCreateMem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    如果 (!buf)
        goto dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(NULL, buf, XML_CHAR_ENCODING_UTF8);
#endif

    如果 (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        goto dtd_error;
    }

#如果 HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#否则
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    如果 (!dtdvalid) {
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
#如果 HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    返回 success;
}

static bool
parse(struct rxkb_context *ctx, const char *path,
      enum rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = NULL;
    xmlNode *root = NULL;

    如果 (!check_eaccess(path, R_OK))
        返回 false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    如果 (!xmlCtxt)
        返回 false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#否则
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#如果 HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#如果 !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, NULL, 0);
    如果 (!doc)
        goto parse_error;

    如果 (!validate(ctx, doc)) {
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

#如果 !HAVE_XML_CTXT_PARSE_DTD
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
#如果 HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, NULL, NULL);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    返回 success;
}
