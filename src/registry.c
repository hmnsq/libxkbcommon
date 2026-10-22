/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <stdbool.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <stdint.h>
#ضمن <libxml/parser.h>

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ضمن <limits.h>
    #إذا HAVE_UNISTD_H && HAVE_DIRENT_H
        #ضمن <dirent.h>
        #ضمن <unistd.h>
    #endif
#endif

#ضمن "xkbcommon/xkbregistry.h"
#ضمن "messages-codes.h"
#ضمن "darray.h"
#ضمن "utils.h"
#ضمن "util-list.h"
#ضمن "util-mem.h"

بنية rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
بنية rxkb_object {
    بنية rxkb_object *parent;
    uint32_t refcount;
    بنية list link;
};

بنية rxkb_iso639_code {
    بنية rxkb_object base;
    حرف *code;
};

بنية rxkb_iso3166_code {
    بنية rxkb_object base;
    حرف *code;
};

تعداد context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

بنية rxkb_context {
    بنية rxkb_object base;
    تعداد context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    بنية list models;         /* list of struct rxkb_models */
    بنية list layouts;        /* list of struct rxkb_layouts */
    بنية list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية rxkb_context *ctx,
                                     تعداد rxkb_log_level level,
                                     ثابت حرف *fmt, va_list args);
    تعداد rxkb_log_level log_level;

    فراغ *userdata;
};

بنية rxkb_model {
    بنية rxkb_object base;

    حرف *name;
    حرف *vendor;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_layout {
    بنية rxkb_object base;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    تعداد rxkb_popularity popularity;

    بنية list iso639s;  /* list of struct rxkb_iso639_code */
    بنية list iso3166s; /* list of struct rxkb_iso3166_code */
};

بنية rxkb_option_group {
    بنية rxkb_object base;

    bool allow_multiple;
    بنية list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_option {
    بنية rxkb_object base;

    حرف *name;
    حرف *brief;
    حرف *description;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *path,
      تعداد rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساكن فراغ
rxkb_log(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list args;

    إذا (ctx->log_level < level)
        أرجع;

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
#define rxkb_log_with_code(ctx, level, ڤسنن_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(ڤسنن_id, fmt), ##__VA_ARGS__)
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
بنية type_ * type_##_ref(بنية type_ *object) { \
    rxkb_object_ref(&object->base); \
    أرجع object; \
} \
بنية type_ * type_##_unref(بنية type_ *object) { \
    إذا (!object) أرجع عدم; \
    أكد(object->base.refcount >= 1); \
    إذا (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        حرر(object); \
    } \
    أرجع عدم;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
ساكن مضمن بنية type_ * type_##_create(بنية rxkb_object *parent) { \
    بنية type_ *t = calloc(1, حجم *t); \
    إذا (t) \
        rxkb_object_init(&t->base, parent); \
    أرجع t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(بنية type_ *object) { \
    أرجع object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ثابت حرف*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
بنية type_ * type_##_first(بنية parent_type_ *parent) { \
    بنية type_ *o = عدم; \
    إذا (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    أرجع o; \
} \
بنية type_ * \
type_##_next(بنية type_ *o) \
{ \
    بنية parent_type_ *parent; \
    بنية type_ *next; \
    parent = container_of(o->base.parent, بنية parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    إذا (list_is_last(&parent->parent_field_, &o->base.link)) \
        أرجع عدم; \
    أرجع next; \
}

ساكن فراغ
rxkb_object_init(بنية rxkb_object *object, بنية rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

ساكن فراغ *
rxkb_object_ref(بنية rxkb_object *object)
{
    أكد(object->refcount >= 1);
    ++object->refcount;
    أرجع object;
}

ساكن فراغ
rxkb_iso639_code_destroy(بنية rxkb_iso639_code *code)
{
    حرر(code->code);
}

بنية rxkb_iso639_code *
rxkb_layout_get_iso639_first(بنية rxkb_layout *layout)
{
    بنية rxkb_iso639_code *code = عدم;

    إذا (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    أرجع code;
}

بنية rxkb_iso639_code *
rxkb_iso639_code_next(بنية rxkb_iso639_code *code)
{
    بنية rxkb_iso639_code *next = عدم;
    بنية rxkb_layout *layout;

    layout = container_of(code->base.parent, بنية rxkb_layout, base);

    إذا (list_is_last(&layout->iso639s, &code->base.link))
        أرجع عدم;

    next = list_first_entry(&code->base.link, code, base.link);

    أرجع next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

ساكن فراغ
rxkb_iso3166_code_destroy(بنية rxkb_iso3166_code *code)
{
    حرر(code->code);
}

بنية rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(بنية rxkb_layout *layout)
{
    بنية rxkb_iso3166_code *code = عدم;

    إذا (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    أرجع code;
}

بنية rxkb_iso3166_code *
rxkb_iso3166_code_next(بنية rxkb_iso3166_code *code)
{
    بنية rxkb_iso3166_code *next = عدم;
    بنية rxkb_layout *layout;

    layout = container_of(code->base.parent, بنية rxkb_layout, base);

    إذا (list_is_last(&layout->iso3166s, &code->base.link))
        أرجع عدم;

    next = list_first_entry(&code->base.link, code, base.link);

    أرجع next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

ساكن فراغ
rxkb_option_destroy(بنية rxkb_option *o)
{
    حرر(o->name);
    حرر(o->brief);
    حرر(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, تعداد rxkb_popularity);
bool rxkb_option_is_layout_specific(بنية rxkb_option *object) {
    أرجع object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

ساكن فراغ
rxkb_layout_destroy(بنية rxkb_layout *l)
{
    بنية rxkb_iso639_code *iso639, *tmp_639;
    بنية rxkb_iso3166_code *iso3166, *tmp_3166;

    حرر(l->name);
    حرر(l->brief);
    حرر(l->description);
    حرر(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, تعداد rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ساكن فراغ
rxkb_model_destroy(بنية rxkb_model *m)
{
    حرر(m->name);
    حرر(m->vendor);
    حرر(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, تعداد rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

ساكن فراغ
rxkb_option_group_destroy(بنية rxkb_option_group *og)
{
    بنية rxkb_option *o, *otmp;

    حرر(og->name);
    حرر(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(بنية rxkb_option_group *g)
{
    أرجع g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, تعداد rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

ساكن فراغ
rxkb_context_destroy(بنية rxkb_context *ctx)
{
    بنية rxkb_model *m, *mtmp;
    بنية rxkb_layout *l, *ltmp;
    بنية rxkb_option_group *og, *ogtmp;
    حرف **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    أكد(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    أكد(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    أكد(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        حرر(*path);
    darray_free(ctx->includes);

    أكد(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, تعداد rxkb_log_level);

ساكن حرف *
rxkb_context_getenv(بنية rxkb_context *ctx, ثابت حرف *name)
{
    إذا (ctx->use_secure_getenv) {
        أرجع secure_getenv(name);
    } وإلا {
        أرجع getenv(name);
    }
}


فراغ
rxkb_context_set_log_level(بنية rxkb_context *ctx,
                           تعداد rxkb_log_level level)
{
    ctx->log_level = level;
}

ساكن ثابت حرف *
log_level_to_prefix(تعداد rxkb_log_level level)
{
    بدل (level) {
    حالة RXKB_LOG_LEVEL_DEBUG:
        أرجع "xkbregistry: DEBUG: ";
    حالة RXKB_LOG_LEVEL_INFO:
        أرجع "xkbregistry: INFO: ";
    حالة RXKB_LOG_LEVEL_WARNING:
        أرجع "xkbregistry: WARNING: ";
    حالة RXKB_LOG_LEVEL_ERROR:
        أرجع "xkbregistry: ERROR: ";
    حالة RXKB_LOG_LEVEL_CRITICAL:
        أرجع "xkbregistry: CRITICAL: ";
    افتراضي:
        أرجع عدم;
    }
}

ATTR_PRINTF(3, 0) ساكن فراغ
default_log_fn(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
               ثابت حرف *fmt, va_list args)
{
    ثابت حرف *prefix = log_level_to_prefix(level);

    إذا (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ساكن تعداد rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت long lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد rxkb_log_level)lvl;

    إذا (istreq_prefix("crit", level))
        أرجع RXKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_prefix("err", level))
        أرجع RXKB_LOG_LEVEL_ERROR;
    إذا (istreq_prefix("warn", level))
        أرجع RXKB_LOG_LEVEL_WARNING;
    إذا (istreq_prefix("info", level))
        أرجع RXKB_LOG_LEVEL_INFO;
    إذا (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        أرجع RXKB_LOG_LEVEL_DEBUG;

    أرجع RXKB_LOG_LEVEL_ERROR;
}

بنية rxkb_context *
rxkb_context_new(تعداد rxkb_context_flags flags)
{
    بنية rxkb_context *ctx = rxkb_context_create(عدم);
    ثابت حرف *env;

    إذا (!ctx)
        أرجع عدم;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    إذا (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    ساكن ثابت تعداد rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    إذا (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        حرر(ctx);
        أرجع عدم;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    إذا (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_اکر);
        rxkb_context_unref(ctx);
        أرجع عدم;
    }

    أرجع ctx;
}

فراغ
rxkb_context_set_log_fn(بنية rxkb_context *ctx,
                        فراغ (*log_fn)(بنية rxkb_context *ctx,
                                       تعداد rxkb_log_level level,
                                       ثابت حرف *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(بنية rxkb_context *ctx, ثابت حرف *path)
{
    صحيح err = 0;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        اذهب error;
    }

    بنية stat stat_buf;
    err = stat(path, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب error;
    }
    إذا (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        اذهب error;
    }

    إذا (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        اذهب error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف rules[PATH_MAX];
    إذا (!snprintf_safe(rules, حجم(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(rules), path, DEFAULT_XKB_RULES);
        اذهب error;
    }

    حرف * ثابت tmp = strdup(path);
    إذا (!tmp) {
        err = ENOMEM;
        اذهب error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    أرجع true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    أرجع false;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
add_direct_subdirectories(بنية rxkb_context *ctx, ثابت حرف *path,
                          darray_string *extensions,
                          darray_size_t versioned_کيراءن,
                          size_t versioned_path_length)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(path, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(path);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *entry;
    حرف path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    طالما ((entry = readdir(dir)) != عدم) {
        ثابت حرف *name = entry->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_safe(path_buf, حجم(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_size_t i = 0; i < versioned_کيراءن; i++) {
            ثابت حرف * ثابت prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            إذا (strcmp(name, prev_name) == 0)
                اذهب next;
        }

        حرف *ext_path = strdup_safe(path_buf);
        إذا (!ext_path) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_size(*extensions) > versioned_کيراءن) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_بارڠن(*extensions) + versioned_کيراءن,
              darray_size(*extensions) - versioned_کيراءن,
              حجم(*darray_بارڠن(*extensions)), &compare_str);
        حرف **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_کيراءن) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#endif

bool
rxkb_context_include_path_append_default(بنية rxkb_context *ctx)
{
    حرف user_path[PATH_MAX];
    bool ret = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        أرجع false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        إذا (snprintf_safe(user_path, حجم(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        إذا (snprintf_safe(user_path, حجم(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    إذا (home != عدم) {
        إذا (snprintf_safe(user_path, حجم(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != عدم) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    إذا (extensions_path == عدم)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    إذا (extensions_path != عدم) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    إذا (extensions_path == عدم)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    إذا (extensions_path != عدم) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    حرف **ext_path;
    darray_foreach(ext_path, extensions) {
        حرر(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ثابت حرف * ثابت اکر = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_اکر = rxkb_context_include_path_append(
        ctx, ((اکر != عدم) ? اکر: DFLT_XKB_CONFIG_اکر)
    );
    ret |= has_اکر;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    إذا (!has_اکر && (اکر == عدم || اکر[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_اکر,
                 ((اکر == عدم) ? DFLT_XKB_CONFIG_اکر : اکر));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_اکر);
    }

    أرجع ret;
}

bool
rxkb_context_parse_default_ruleset(بنية rxkb_context *ctx)
{
    أرجع rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(بنية rxkb_context *ctx, ثابت حرف *ruleset)
{
    حرف **path;
    bool success = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        أرجع false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        حرف rules[PATH_MAX];

        إذا (snprintf_safe(rules, حجم(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            إذا (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        إذا (ctx->load_extra_rules_files &&
            snprintf_safe(rules, حجم(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            إذا (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    أرجع success;
}


فراغ
rxkb_context_set_user_data(بنية rxkb_context *ctx, فراغ *userdata)
{
    ctx->userdata = userdata;
}

فراغ *
rxkb_context_get_user_data(بنية rxkb_context *ctx)
{
    أرجع ctx->userdata;
}

ساكن مضمن bool
is_نود(xml_نود *نود, ثابت حرف *name)
{
    أرجع نود->type == XML_ELEMENT_نود &&
        xmlStrEqual(نود->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساكن حرف *
extract_text(xml_نود *نود)
{
    xml_نود *n;

    لكل (n = نود->children; n; n = n->next) {
        إذا (n->type == XML_TEXT_نود)
            أرجع (حرف *)xmlStrdup(n->content);
    }
    أرجع عدم;
}

/* Data from “configItem” node */
بنية config_item {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = عدم, \
    .description = عدم, \
    .brief = عدم, \
    .vendor = عدم, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساكن فراغ
config_item_free(بنية config_item *config) {
    حرر(config->name);
    حرر(config->description);
    حرر(config->brief);
    حرر(config->vendor);
}

ساكن bool
parse_config_item(بنية rxkb_context *ctx, xml_نود *parent,
                  بنية config_item *config)
{
    xml_نود *نود = عدم;
    xml_نود *ci = عدم;

    لكل (ci = parent->children; ci; ci = ci->next) {
        إذا (is_نود(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            إذا (raw_popularity) {
                إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وإلا إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وإلا
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->باريس, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ثابت xmlChar*)"layout-specific");
            إذا (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ثابت xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            لكل (نود = ci->children; نود; نود = نود->next) {
                إذا (is_نود(نود, "name"))
                    config->name = extract_text(نود);
                وإلا إذا (is_نود(نود, "description"))
                    config->description = extract_text(نود);
                وإلا إذا (is_نود(نود, "shortDescription"))
                    config->brief = extract_text(نود);
                وإلا إذا (is_نود(نود, "vendor"))
                    config->vendor = extract_text(نود);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            إذا (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->باريس);
                config_item_free(config);
                أرجع false;
            }

            أرجع true; /* only one configItem allowed in the dtd */
        }
    }

    أرجع false;
}

ساكن فراغ
parse_model(بنية rxkb_context *ctx, xml_نود *model,
            تعداد rxkb_popularity popularity)
{
    بنية config_item config = config_item_new(popularity);

    إذا (parse_config_item(ctx, model, &config)) {
        بنية rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            إذا (streq(m->name, config.name)) {
                config_item_free(&config);
                أرجع;
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

ساكن فراغ
parse_model_list(بنية rxkb_context *ctx, xml_نود *model_list,
                 تعداد rxkb_popularity popularity)
{
    xml_نود *نود = عدم;

    لكل (نود = model_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "model"))
            parse_model(ctx, نود, popularity);
    }
}

ساكن فراغ
parse_language_list(xml_نود *language_list, بنية rxkb_layout *layout)
{
    xml_نود *نود = عدم;
    بنية rxkb_iso639_code *code;

    لكل (نود = language_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "iso639Id")) {
            حرف *str = extract_text(نود);
            بنية rxkb_object *parent;

            إذا (!str || strlen(str) != 3) {
                حرر(str);
                استمر;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

ساكن فراغ
parse_country_list(xml_نود *country_list, بنية rxkb_layout *layout)
{
    xml_نود *نود = عدم;
    بنية rxkb_iso3166_code *code;

    لكل (نود = country_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "iso3166Id")) {
            حرف *str = extract_text(نود);
            بنية rxkb_object *parent;

            إذا (!str || strlen(str) != 2) {
                حرر(str);
                استمر;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

ساكن فراغ
parse_variant(بنية rxkb_context *ctx, بنية rxkb_layout *l,
              xml_نود *variant, تعداد rxkb_popularity popularity)
{
    xml_نود *ci;
    بنية config_item config = config_item_new(popularity);

    إذا (parse_config_item(ctx, variant, &config)) {
        بنية rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            إذا (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                اكسر;
            }
        }

        إذا (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == عدم ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            لكل (ci = variant->children; ci; ci = ci->next) {
                xml_نود *نود;

                إذا (!is_نود(ci, "configItem"))
                    استمر;

                bool found_language_list = false;
                bool found_country_list = false;
                لكل (نود = ci->children; نود; نود = نود->next) {
                    إذا (is_نود(نود, "languageList")) {
                        parse_language_list(نود, v);
                        found_language_list = true;
                    }
                    إذا (is_نود(نود, "countryList")) {
                        parse_country_list(نود, v);
                        found_country_list = true;
                    }
                }
                إذا (!found_language_list) {
                    // inherit from parent layout
                    بنية rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        بنية rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                إذا (!found_country_list) {
                    // inherit from parent layout
                    بنية rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        بنية rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } وإلا {
            config_item_free(&config);
        }
    }
}

ساكن فراغ
parse_variant_list(بنية rxkb_context *ctx, بنية rxkb_layout *l,
                   xml_نود *variant_list, تعداد rxkb_popularity popularity)
{
    xml_نود *نود = عدم;

    لكل (نود = variant_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "variant"))
            parse_variant(ctx, l, نود, popularity);
    }
}

ساكن فراغ
parse_layout(بنية rxkb_context *ctx, xml_نود *layout,
             تعداد rxkb_popularity popularity)
{
    بنية config_item config = config_item_new(popularity);
    بنية rxkb_layout *l;
    xml_نود *نود = عدم;
    bool exists = false;

    إذا (!parse_config_item(ctx, layout, &config))
        أرجع;

    list_for_each(l, &ctx->layouts, base.link) {
        إذا (streq(l->name, config.name) && l->variant == عدم) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = عدم;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } وإلا {
        config_item_free(&config);
    }

    لكل (نود = layout->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "variantList")) {
            parse_variant_list(ctx, l, نود, popularity);
        }
        إذا (!exists && is_نود(نود, "configItem")) {
            xml_نود *ll;
            لكل (ll = نود->children; ll; ll = ll->next) {
                إذا (is_نود(ll, "languageList"))
                    parse_language_list(ll, l);
                إذا (is_نود(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساكن فراغ
parse_layout_list(بنية rxkb_context *ctx, xml_نود *layout_list,
                  تعداد rxkb_popularity popularity)
{
    xml_نود *نود = عدم;

    لكل (نود = layout_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "layout"))
            parse_layout(ctx, نود, popularity);
    }
}

ساكن فراغ
parse_option(بنية rxkb_context *ctx, بنية rxkb_option_group *group,
             xml_نود *option, تعداد rxkb_popularity popularity)
{
    بنية config_item config = config_item_new(popularity);

    إذا (parse_config_item(ctx, option, &config)) {
        بنية rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            إذا (streq(o->name, config.name)) {
                config_item_free(&config);
                أرجع;
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

ساكن فراغ
parse_group(بنية rxkb_context *ctx, xml_نود *group,
            تعداد rxkb_popularity popularity)
{
    بنية config_item config = config_item_new(popularity);
    بنية rxkb_option_group *g;
    xml_نود *نود = عدم;
    xmlChar *multiple;
    bool exists = false;

    إذا (!parse_config_item(ctx, group, &config))
        أرجع;

    list_for_each(g, &ctx->option_groups, base.link) {
        إذا (streq(g->name, config.name)) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ثابت xmlChar*)"allowMultipleSelection");
        إذا (multiple && xmlStrEqual(multiple, (ثابت xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } وإلا {
        config_item_free(&config);
    }

    لكل (نود = group->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "option"))
            parse_option(ctx, g, نود, popularity);
    }
}

ساكن فراغ
parse_option_list(بنية rxkb_context *ctx, xml_نود *option_list,
                  تعداد rxkb_popularity popularity)
{
    xml_نود *نود = عدم;

    لكل (نود = option_list->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "group"))
            parse_group(ctx, نود, popularity);
    }
}

ساكن فراغ
parse_rules_xml(بنية rxkb_context *ctx, xml_نود *اکر,
                تعداد rxkb_popularity popularity)
{
    xml_نود *نود = عدم;

    لكل (نود = اکر->children; نود; نود = نود->next) {
        إذا (is_نود(نود, "modelList"))
            parse_model_list(ctx, نود, popularity);
        وإلا إذا (is_نود(نود, "layoutList"))
            parse_layout_list(ctx, نود, popularity);
        وإلا إذا (is_نود(نود, "optionList"))
            parse_option_list(ctx, نود, popularity);
    }
}

ساكن فراغ
ATTR_PRINTF(2, 0)
xml_error_func(فراغ *ctx, ثابت حرف *ڤسنن, ...)
{
    ساكن حرف buf[PATH_MAX];
    ساكن size_t slen = 0;
    va_list args;
    صحيح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, ڤسنن);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, ڤسنن, args);
    va_end(args);

    /* This shouldn't really happen */
    إذا (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, حجم(buf));
        أرجع;
    }

    slen += (size_t)rc;
    إذا (slen >= حجم(buf)) {
        /* truncated, let's flush this */
        buf[حجم(buf) - 1] = '\n';
        slen = حجم(buf);
    }

    /* We're assuming here that the last character is \n. */
    إذا (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#إذا HAVE_XML_CTXT_SET_ERRORHANDLER
ساكن فراغ
xml_structured_error_func(فراغ *userData, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

ساكن bool
validate(بنية rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ثابت حرف dtdstr[] =
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

#إذا HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ماسوقن_Ptr pinput =
        xml_New_ماسوقن_From_Memory(عدم, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_ماسوقن_BUF_STATIC);
    إذا (!pinput)
        اذهب dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, عدم, عدم);
#وإلا
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ماسوقن_Buffer_Ptr buf =
        xml_Parser_ماسوقن_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    إذا (!buf)
        اذهب dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(عدم, buf, XML_CHAR_ENCODING_UTF8);
#endif

    إذا (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        اذهب dtd_error;
    }

#إذا HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#وإلا
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    إذا (!dtdvalid) {
        success = false;
        اذهب ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#إذا HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    أرجع success;
}

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *path,
      تعداد rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = عدم;
    xml_نود *اکر = عدم;

    إذا (!check_eaccess(path, R_OK))
        أرجع false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#إذا HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#إذا !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, عدم, 0);
    إذا (!doc)
        اذهب parse_error;

    إذا (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        اذهب validate_error;
    }

    اکر = xml_Doc_Get_اکر_Element(doc);
    parse_rules_xml(ctx, اکر, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#إذا !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(عدم, عدم);
#endif
#إذا HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, عدم, عدم);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    أرجع success;
}
