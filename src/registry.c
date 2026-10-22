/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <stdbool.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <stdint.h>
#شامل <libxml/parser.h>

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    #شامل <limits.h>
    #اگر HAVE_UNISTD_H && HAVE_DIRENT_H
        #شامل <dirent.h>
        #شامل <unistd.h>
    #endif
#endif

#شامل "xkbcommon/xkbregistry.h"
#شامل "messages-codes.h"
#شامل "darray.h"
#شامل "utils.h"
#شامل "util-list.h"
#شامل "util-mem.h"

ساختار rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ساختار rxkb_object {
    ساختار rxkb_object *parent;
    uint32_t refcount;
    ساختار list link;
};

ساختار rxkb_iso639_code {
    ساختار rxkb_object base;
    نویسه *code;
};

ساختار rxkb_iso3166_code {
    ساختار rxkb_object base;
    نویسه *code;
};

شمارش context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ساختار rxkb_context {
    ساختار rxkb_object base;
    شمارش context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    ساختار list models;         /* list of struct rxkb_models */
    ساختار list layouts;        /* list of struct rxkb_layouts */
    ساختار list option_groups;  /* list of struct rxkb_option_group */

    darray(نویسه *) includes;


    ATTR_PRINTF(3, 0) تهی (*log_fn)(ساختار rxkb_context *ctx,
                                     شمارش rxkb_log_level level,
                                     ثابت نویسه *fmt, va_list args);
    شمارش rxkb_log_level log_level;

    تهی *userdata;
};

ساختار rxkb_model {
    ساختار rxkb_object base;

    نویسه *name;
    نویسه *vendor;
    نویسه *description;
    شمارش rxkb_popularity popularity;
};

ساختار rxkb_layout {
    ساختار rxkb_object base;

    نویسه *name;
    نویسه *brief;
    نویسه *description;
    نویسه *variant;
    شمارش rxkb_popularity popularity;

    ساختار list iso639s;  /* list of struct rxkb_iso639_code */
    ساختار list iso3166s; /* list of struct rxkb_iso3166_code */
};

ساختار rxkb_option_group {
    ساختار rxkb_object base;

    bool allow_multiple;
    ساختار list options; /* list of struct rxkb_options */
    نویسه *name;
    نویسه *description;
    شمارش rxkb_popularity popularity;
};

ساختار rxkb_option {
    ساختار rxkb_object base;

    نویسه *name;
    نویسه *brief;
    نویسه *description;
    شمارش rxkb_popularity popularity;
    bool layout_specific;
};

ایستا bool
parse(ساختار rxkb_context *ctx, ثابت نویسه *path,
      شمارش rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ایستا تهی
rxkb_log(ساختار rxkb_context *ctx, شمارش rxkb_log_level level,
         ثابت نویسه *fmt, ...)
{
    va_list args;

    اگر (ctx->log_level < level)
        بازگشت;

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
#define rxkb_log_with_code(ctx, level, پیام_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(پیام_id, fmt), ##__VA_ARGS__)
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
ساختار type_ * type_##_ref(ساختار type_ *object) { \
    rxkb_object_ref(&object->base); \
    بازگشت object; \
} \
ساختار type_ * type_##_unref(ساختار type_ *object) { \
    اگر (!object) بازگشت پوچ; \
    أكد(object->base.refcount >= 1); \
    اگر (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        آزاد(object); \
    } \
    بازگشت پوچ;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
ایستا درونخطی ساختار type_ * type_##_create(ساختار rxkb_object *parent) { \
    ساختار type_ *t = calloc(1, اندازه *t); \
    اگر (t) \
        rxkb_object_init(&t->base, parent); \
    بازگشت t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(ساختار type_ *object) { \
    بازگشت object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ثابت نویسه*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
ساختار type_ * type_##_first(ساختار parent_type_ *parent) { \
    ساختار type_ *o = پوچ; \
    اگر (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    بازگشت o; \
} \
ساختار type_ * \
type_##_next(ساختار type_ *o) \
{ \
    ساختار parent_type_ *parent; \
    ساختار type_ *next; \
    parent = container_of(o->base.parent, ساختار parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    اگر (list_is_last(&parent->parent_field_, &o->base.link)) \
        بازگشت پوچ; \
    بازگشت next; \
}

ایستا تهی
rxkb_object_init(ساختار rxkb_object *object, ساختار rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

ایستا تهی *
rxkb_object_ref(ساختار rxkb_object *object)
{
    أكد(object->refcount >= 1);
    ++object->refcount;
    بازگشت object;
}

ایستا تهی
rxkb_iso639_code_destroy(ساختار rxkb_iso639_code *code)
{
    آزاد(code->code);
}

ساختار rxkb_iso639_code *
rxkb_layout_get_iso639_first(ساختار rxkb_layout *layout)
{
    ساختار rxkb_iso639_code *code = پوچ;

    اگر (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    بازگشت code;
}

ساختار rxkb_iso639_code *
rxkb_iso639_code_next(ساختار rxkb_iso639_code *code)
{
    ساختار rxkb_iso639_code *next = پوچ;
    ساختار rxkb_layout *layout;

    layout = container_of(code->base.parent, ساختار rxkb_layout, base);

    اگر (list_is_last(&layout->iso639s, &code->base.link))
        بازگشت پوچ;

    next = list_first_entry(&code->base.link, code, base.link);

    بازگشت next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

ایستا تهی
rxkb_iso3166_code_destroy(ساختار rxkb_iso3166_code *code)
{
    آزاد(code->code);
}

ساختار rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(ساختار rxkb_layout *layout)
{
    ساختار rxkb_iso3166_code *code = پوچ;

    اگر (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    بازگشت code;
}

ساختار rxkb_iso3166_code *
rxkb_iso3166_code_next(ساختار rxkb_iso3166_code *code)
{
    ساختار rxkb_iso3166_code *next = پوچ;
    ساختار rxkb_layout *layout;

    layout = container_of(code->base.parent, ساختار rxkb_layout, base);

    اگر (list_is_last(&layout->iso3166s, &code->base.link))
        بازگشت پوچ;

    next = list_first_entry(&code->base.link, code, base.link);

    بازگشت next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

ایستا تهی
rxkb_option_destroy(ساختار rxkb_option *o)
{
    آزاد(o->name);
    آزاد(o->brief);
    آزاد(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, شمارش rxkb_popularity);
bool rxkb_option_is_layout_specific(ساختار rxkb_option *object) {
    بازگشت object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

ایستا تهی
rxkb_layout_destroy(ساختار rxkb_layout *l)
{
    ساختار rxkb_iso639_code *iso639, *tmp_639;
    ساختار rxkb_iso3166_code *iso3166, *tmp_3166;

    آزاد(l->name);
    آزاد(l->brief);
    آزاد(l->description);
    آزاد(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, شمارش rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ایستا تهی
rxkb_model_destroy(ساختار rxkb_model *m)
{
    آزاد(m->name);
    آزاد(m->vendor);
    آزاد(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, شمارش rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

ایستا تهی
rxkb_option_group_destroy(ساختار rxkb_option_group *og)
{
    ساختار rxkb_option *o, *otmp;

    آزاد(og->name);
    آزاد(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(ساختار rxkb_option_group *g)
{
    بازگشت g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, شمارش rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

ایستا تهی
rxkb_context_destroy(ساختار rxkb_context *ctx)
{
    ساختار rxkb_model *m, *mtmp;
    ساختار rxkb_layout *l, *ltmp;
    ساختار rxkb_option_group *og, *ogtmp;
    نویسه **path;

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
        آزاد(*path);
    darray_free(ctx->includes);

    أكد(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, شمارش rxkb_log_level);

ایستا نویسه *
rxkb_context_getenv(ساختار rxkb_context *ctx, ثابت نویسه *name)
{
    اگر (ctx->use_secure_getenv) {
        بازگشت secure_getenv(name);
    } وگرنه {
        بازگشت getenv(name);
    }
}


تهی
rxkb_context_set_log_level(ساختار rxkb_context *ctx,
                           شمارش rxkb_log_level level)
{
    ctx->log_level = level;
}

ایستا ثابت نویسه *
log_level_to_prefix(شمارش rxkb_log_level level)
{
    انتخاب (level) {
    حالت RXKB_LOG_LEVEL_DEBUG:
        بازگشت "xkbregistry: DEBUG: ";
    حالت RXKB_LOG_LEVEL_INFO:
        بازگشت "xkbregistry: INFO: ";
    حالت RXKB_LOG_LEVEL_WARNING:
        بازگشت "xkbregistry: WARNING: ";
    حالت RXKB_LOG_LEVEL_ERROR:
        بازگشت "xkbregistry: ERROR: ";
    حالت RXKB_LOG_LEVEL_CRITICAL:
        بازگشت "xkbregistry: CRITICAL: ";
    پیشفرض:
        بازگشت پوچ;
    }
}

ATTR_PRINTF(3, 0) ایستا تهی
default_log_fn(ساختار rxkb_context *ctx, شمارش rxkb_log_level level,
               ثابت نویسه *fmt, va_list args)
{
    ثابت نویسه *prefix = log_level_to_prefix(level);

    اگر (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ایستا شمارش rxkb_log_level
log_level(ثابت نویسه *level) {
    errno = 0;
    نویسه *endptr;
    ثابت long lvl = strtol(level, &endptr, 10);
    اگر (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        بازگشت (شمارش rxkb_log_level)lvl;

    اگر (istreq_prefix("crit", level))
        بازگشت RXKB_LOG_LEVEL_CRITICAL;
    اگر (istreq_prefix("err", level))
        بازگشت RXKB_LOG_LEVEL_ERROR;
    اگر (istreq_prefix("warn", level))
        بازگشت RXKB_LOG_LEVEL_WARNING;
    اگر (istreq_prefix("info", level))
        بازگشت RXKB_LOG_LEVEL_INFO;
    اگر (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        بازگشت RXKB_LOG_LEVEL_DEBUG;

    بازگشت RXKB_LOG_LEVEL_ERROR;
}

ساختار rxkb_context *
rxkb_context_new(شمارش rxkb_context_flags flags)
{
    ساختار rxkb_context *ctx = rxkb_context_create(پوچ);
    ثابت نویسه *env;

    اگر (!ctx)
        بازگشت پوچ;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    اگر (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    ایستا ثابت شمارش rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    اگر (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        آزاد(ctx);
        بازگشت پوچ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    اگر (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ریشه);
        rxkb_context_unref(ctx);
        بازگشت پوچ;
    }

    بازگشت ctx;
}

تهی
rxkb_context_set_log_fn(ساختار rxkb_context *ctx,
                        تهی (*log_fn)(ساختار rxkb_context *ctx,
                                       شمارش rxkb_log_level level,
                                       ثابت نویسه *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(ساختار rxkb_context *ctx, ثابت نویسه *path)
{
    صحیح err = 0;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        برو error;
    }

    ساختار stat stat_buf;
    err = stat(path, &stat_buf);
    اگر (err != 0) {
        err = errno;
        برو error;
    }
    اگر (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        برو error;
    }

    اگر (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        برو error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    نویسه rules[PATH_MAX];
    اگر (!snprintf_safe(rules, اندازه(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                اندازه(rules), path, DEFAULT_XKB_RULES);
        برو error;
    }

    نویسه * ثابت tmp = strdup(path);
    اگر (!tmp) {
        err = ENOMEM;
        برو error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    بازگشت true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    بازگشت false;
}

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES

ایستا صحیح
compare_str(ثابت تهی *a, ثابت تهی *b)
{
    بازگشت strcmp(*(نویسه **)a, *(نویسه **) b);
}

ایستا صحیح
add_direct_subdirectories(ساختار rxkb_context *ctx, ثابت نویسه *path,
                          darray_string *extensions,
                          darray_size_t versioned_شمار,
                          size_t versioned_path_length)
{
    صحیح ret = 0;
    صحیح err = ENOMEM;
    DIR *dir = پوچ;

    /* Check extensions parent directory */
    ساختار stat stat_buf;
    err = stat(path, &stat_buf);
    اگر (err != 0) {
        err = errno;
        برو err;
    }
    اگر (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        برو err;
    }
    اگر (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        برو err;
    }

    dir = opendir(path);
    اگر (dir == پوچ) {
        err = EACCES;
        برو err;
    }

    ساختار dirent *entry;
    نویسه path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    وقتی ((entry = readdir(dir)) != پوچ) {
        ثابت نویسه *name = entry->d_name;

        /* Skip special entries */
        اگر (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ادامه;

        اگر (!snprintf_safe(path_buf, اندازه(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            برو err;
        }
        اگر (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ادامه;

        /* Skip if the corresponding versioned directory is already included */
        برای (darray_size_t i = 0; i < versioned_شمار; i++) {
            ثابت نویسه * ثابت prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            اگر (strcmp(name, prev_name) == 0)
                برو next;
        }

        نویسه *ext_path = strdup_safe(path_buf);
        اگر (!ext_path) {
            err = ENOMEM;
            برو err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    اگر (darray_size(*extensions) > versioned_شمار) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اقلام(*extensions) + versioned_شمار,
              darray_size(*extensions) - versioned_شمار,
              اندازه(*darray_اقلام(*extensions)), &compare_str);
        نویسه **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_شمار) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    بازگشت ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    اگر (dir)
        closedir(dir);

    بازگشت ret;
}
#endif

bool
rxkb_context_include_path_append_default(ساختار rxkb_context *ctx)
{
    نویسه user_path[PATH_MAX];
    bool ret = false;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        بازگشت false;
    }

    ثابت نویسه * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت نویسه * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    اگر (xdg != پوچ) {
        اگر (snprintf_safe(user_path, اندازه(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } وگرنه اگر (home != پوچ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        اگر (snprintf_safe(user_path, اندازه(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    اگر (home != پوچ) {
        اگر (snprintf_safe(user_path, اندازه(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ثابت نویسه * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != پوچ) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت نویسه *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    اگر (extensions_path == پوچ)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    اگر (extensions_path != پوچ) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    اگر (extensions_path == پوچ)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    اگر (extensions_path != پوچ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    نویسه **ext_path;
    darray_foreach(ext_path, extensions) {
        آزاد(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ثابت نویسه * ثابت ریشه = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_ریشه = rxkb_context_include_path_append(
        ctx, ((ریشه != پوچ) ? ریشه: DFLT_XKB_CONFIG_ریشه)
    );
    ret |= has_ریشه;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    اگر (!has_ریشه && (ریشه == پوچ || ریشه[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ریشه,
                 ((ریشه == پوچ) ? DFLT_XKB_CONFIG_ریشه : ریشه));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_ریشه);
    }

    بازگشت ret;
}

bool
rxkb_context_parse_default_ruleset(ساختار rxkb_context *ctx)
{
    بازگشت rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(ساختار rxkb_context *ctx, ثابت نویسه *ruleset)
{
    نویسه **path;
    bool success = false;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        بازگشت false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        نویسه rules[PATH_MAX];

        اگر (snprintf_safe(rules, اندازه(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            اگر (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        اگر (ctx->load_extra_rules_files &&
            snprintf_safe(rules, اندازه(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            اگر (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    بازگشت success;
}


تهی
rxkb_context_set_user_data(ساختار rxkb_context *ctx, تهی *userdata)
{
    ctx->userdata = userdata;
}

تهی *
rxkb_context_get_user_data(ساختار rxkb_context *ctx)
{
    بازگشت ctx->userdata;
}

ایستا درونخطی bool
is_گره(xml_گره *گره, ثابت نویسه *name)
{
    بازگشت گره->type == XML_ELEMENT_گره &&
        xmlStrEqual(گره->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ایستا نویسه *
extract_text(xml_گره *گره)
{
    xml_گره *n;

    برای (n = گره->children; n; n = n->next) {
        اگر (n->type == XML_TEXT_گره)
            بازگشت (نویسه *)xmlStrdup(n->content);
    }
    بازگشت پوچ;
}

/* Data from “configItem” node */
ساختار config_item {
    نویسه *name;
    نویسه *description;
    نویسه *brief;
    نویسه *vendor;
    شمارش rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = پوچ, \
    .description = پوچ, \
    .brief = پوچ, \
    .vendor = پوچ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ایستا تهی
config_item_free(ساختار config_item *config) {
    آزاد(config->name);
    آزاد(config->description);
    آزاد(config->brief);
    آزاد(config->vendor);
}

ایستا bool
parse_config_item(ساختار rxkb_context *ctx, xml_گره *parent,
                  ساختار config_item *config)
{
    xml_گره *گره = پوچ;
    xml_گره *ci = پوچ;

    برای (ci = parent->children; ci; ci = ci->next) {
        اگر (is_گره(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            اگر (raw_popularity) {
                اگر (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وگرنه اگر (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وگرنه
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->سطر, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ثابت xmlChar*)"layout-specific");
            اگر (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ثابت xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            برای (گره = ci->children; گره; گره = گره->next) {
                اگر (is_گره(گره, "name"))
                    config->name = extract_text(گره);
                وگرنه اگر (is_گره(گره, "description"))
                    config->description = extract_text(گره);
                وگرنه اگر (is_گره(گره, "shortDescription"))
                    config->brief = extract_text(گره);
                وگرنه اگر (is_گره(گره, "vendor"))
                    config->vendor = extract_text(گره);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            اگر (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->سطر);
                config_item_free(config);
                بازگشت false;
            }

            بازگشت true; /* only one configItem allowed in the dtd */
        }
    }

    بازگشت false;
}

ایستا تهی
parse_model(ساختار rxkb_context *ctx, xml_گره *model,
            شمارش rxkb_popularity popularity)
{
    ساختار config_item config = config_item_new(popularity);

    اگر (parse_config_item(ctx, model, &config)) {
        ساختار rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            اگر (streq(m->name, config.name)) {
                config_item_free(&config);
                بازگشت;
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

ایستا تهی
parse_model_list(ساختار rxkb_context *ctx, xml_گره *model_list,
                 شمارش rxkb_popularity popularity)
{
    xml_گره *گره = پوچ;

    برای (گره = model_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "model"))
            parse_model(ctx, گره, popularity);
    }
}

ایستا تهی
parse_language_list(xml_گره *language_list, ساختار rxkb_layout *layout)
{
    xml_گره *گره = پوچ;
    ساختار rxkb_iso639_code *code;

    برای (گره = language_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "iso639Id")) {
            نویسه *str = extract_text(گره);
            ساختار rxkb_object *parent;

            اگر (!str || strlen(str) != 3) {
                آزاد(str);
                ادامه;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

ایستا تهی
parse_country_list(xml_گره *country_list, ساختار rxkb_layout *layout)
{
    xml_گره *گره = پوچ;
    ساختار rxkb_iso3166_code *code;

    برای (گره = country_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "iso3166Id")) {
            نویسه *str = extract_text(گره);
            ساختار rxkb_object *parent;

            اگر (!str || strlen(str) != 2) {
                آزاد(str);
                ادامه;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

ایستا تهی
parse_variant(ساختار rxkb_context *ctx, ساختار rxkb_layout *l,
              xml_گره *variant, شمارش rxkb_popularity popularity)
{
    xml_گره *ci;
    ساختار config_item config = config_item_new(popularity);

    اگر (parse_config_item(ctx, variant, &config)) {
        ساختار rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            اگر (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                بشکن;
            }
        }

        اگر (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == پوچ ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            برای (ci = variant->children; ci; ci = ci->next) {
                xml_گره *گره;

                اگر (!is_گره(ci, "configItem"))
                    ادامه;

                bool found_language_list = false;
                bool found_country_list = false;
                برای (گره = ci->children; گره; گره = گره->next) {
                    اگر (is_گره(گره, "languageList")) {
                        parse_language_list(گره, v);
                        found_language_list = true;
                    }
                    اگر (is_گره(گره, "countryList")) {
                        parse_country_list(گره, v);
                        found_country_list = true;
                    }
                }
                اگر (!found_language_list) {
                    // inherit from parent layout
                    ساختار rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        ساختار rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                اگر (!found_country_list) {
                    // inherit from parent layout
                    ساختار rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        ساختار rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } وگرنه {
            config_item_free(&config);
        }
    }
}

ایستا تهی
parse_variant_list(ساختار rxkb_context *ctx, ساختار rxkb_layout *l,
                   xml_گره *variant_list, شمارش rxkb_popularity popularity)
{
    xml_گره *گره = پوچ;

    برای (گره = variant_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "variant"))
            parse_variant(ctx, l, گره, popularity);
    }
}

ایستا تهی
parse_layout(ساختار rxkb_context *ctx, xml_گره *layout,
             شمارش rxkb_popularity popularity)
{
    ساختار config_item config = config_item_new(popularity);
    ساختار rxkb_layout *l;
    xml_گره *گره = پوچ;
    bool exists = false;

    اگر (!parse_config_item(ctx, layout, &config))
        بازگشت;

    list_for_each(l, &ctx->layouts, base.link) {
        اگر (streq(l->name, config.name) && l->variant == پوچ) {
            exists = true;
            بشکن;
        }
    }

    اگر (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = پوچ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } وگرنه {
        config_item_free(&config);
    }

    برای (گره = layout->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "variantList")) {
            parse_variant_list(ctx, l, گره, popularity);
        }
        اگر (!exists && is_گره(گره, "configItem")) {
            xml_گره *ll;
            برای (ll = گره->children; ll; ll = ll->next) {
                اگر (is_گره(ll, "languageList"))
                    parse_language_list(ll, l);
                اگر (is_گره(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ایستا تهی
parse_layout_list(ساختار rxkb_context *ctx, xml_گره *layout_list,
                  شمارش rxkb_popularity popularity)
{
    xml_گره *گره = پوچ;

    برای (گره = layout_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "layout"))
            parse_layout(ctx, گره, popularity);
    }
}

ایستا تهی
parse_option(ساختار rxkb_context *ctx, ساختار rxkb_option_group *group,
             xml_گره *option, شمارش rxkb_popularity popularity)
{
    ساختار config_item config = config_item_new(popularity);

    اگر (parse_config_item(ctx, option, &config)) {
        ساختار rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            اگر (streq(o->name, config.name)) {
                config_item_free(&config);
                بازگشت;
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

ایستا تهی
parse_group(ساختار rxkb_context *ctx, xml_گره *group,
            شمارش rxkb_popularity popularity)
{
    ساختار config_item config = config_item_new(popularity);
    ساختار rxkb_option_group *g;
    xml_گره *گره = پوچ;
    xmlChar *multiple;
    bool exists = false;

    اگر (!parse_config_item(ctx, group, &config))
        بازگشت;

    list_for_each(g, &ctx->option_groups, base.link) {
        اگر (streq(g->name, config.name)) {
            exists = true;
            بشکن;
        }
    }

    اگر (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ثابت xmlChar*)"allowMultipleSelection");
        اگر (multiple && xmlStrEqual(multiple, (ثابت xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } وگرنه {
        config_item_free(&config);
    }

    برای (گره = group->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "option"))
            parse_option(ctx, g, گره, popularity);
    }
}

ایستا تهی
parse_option_list(ساختار rxkb_context *ctx, xml_گره *option_list,
                  شمارش rxkb_popularity popularity)
{
    xml_گره *گره = پوچ;

    برای (گره = option_list->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "group"))
            parse_group(ctx, گره, popularity);
    }
}

ایستا تهی
parse_rules_xml(ساختار rxkb_context *ctx, xml_گره *ریشه,
                شمارش rxkb_popularity popularity)
{
    xml_گره *گره = پوچ;

    برای (گره = ریشه->children; گره; گره = گره->next) {
        اگر (is_گره(گره, "modelList"))
            parse_model_list(ctx, گره, popularity);
        وگرنه اگر (is_گره(گره, "layoutList"))
            parse_layout_list(ctx, گره, popularity);
        وگرنه اگر (is_گره(گره, "optionList"))
            parse_option_list(ctx, گره, popularity);
    }
}

ایستا تهی
ATTR_PRINTF(2, 0)
xml_error_func(تهی *ctx, ثابت نویسه *پیام, ...)
{
    ایستا نویسه buf[PATH_MAX];
    ایستا size_t slen = 0;
    va_list args;
    صحیح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, پیام);
    rc = vsnprintf(&buf[slen], اندازه(buf) - slen, پیام, args);
    va_end(args);

    /* This shouldn't really happen */
    اگر (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, اندازه(buf));
        بازگشت;
    }

    slen += (size_t)rc;
    اگر (slen >= اندازه(buf)) {
        /* truncated, let's flush this */
        buf[اندازه(buf) - 1] = '\n';
        slen = اندازه(buf);
    }

    /* We're assuming here that the last character is \n. */
    اگر (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, اندازه(buf));
        slen = 0;
    }
}

#اگر HAVE_XML_CTXT_SET_ERRORHANDLER
ایستا تهی
xml_structured_error_func(تهی *userData, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وگرنه
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

ایستا bool
validate(ساختار rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ثابت نویسه dtdstr[] =
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

#اگر HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    اگر (!xmlCtxt)
        بازگشت false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_درون_داد_Ptr pinput =
        xml_New_درون_داد_From_Memory(پوچ, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_درون_داد_BUF_STATIC);
    اگر (!pinput)
        برو dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, پوچ, پوچ);
#وگرنه
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_درون_داد_Buffer_Ptr buf =
        xml_Parser_درون_داد_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    اگر (!buf)
        برو dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(پوچ, buf, XML_CHAR_ENCODING_UTF8);
#endif

    اگر (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        برو dtd_error;
    }

#اگر HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#وگرنه
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    اگر (!dtdvalid) {
        success = false;
        برو ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#اگر HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    بازگشت success;
}

ایستا bool
parse(ساختار rxkb_context *ctx, ثابت نویسه *path,
      شمارش rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = پوچ;
    xml_گره *ریشه = پوچ;

    اگر (!check_eaccess(path, R_OK))
        بازگشت false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    اگر (!xmlCtxt)
        بازگشت false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وگرنه
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#اگر HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#اگر !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, پوچ, 0);
    اگر (!doc)
        برو parse_error;

    اگر (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        برو validate_error;
    }

    ریشه = xml_Doc_Get_ریشه_Element(doc);
    parse_rules_xml(ctx, ریشه, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#اگر !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(پوچ, پوچ);
#endif
#اگر HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, پوچ, پوچ);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    بازگشت success;
}
