/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <libxml/parser.h>

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    #অন্তর্ভাব <limits.h>
    #যদি HAVE_UNISTD_H && HAVE_DIRENT_H
        #অন্তর্ভাব <dirent.h>
        #অন্তর্ভাব <unistd.h>
    #endif
#endif

#অন্তর্ভাব "xkbcommon/xkbregistry.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-list.h"
#অন্তর্ভাব "util-mem.h"

কাঠামো rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
কাঠামো rxkb_object {
    কাঠামো rxkb_object *parent;
    uint32_t refcount;
    কাঠামো list link;
};

কাঠামো rxkb_iso639_code {
    কাঠামো rxkb_object base;
    অক্ষর *code;
};

কাঠামো rxkb_iso3166_code {
    কাঠামো rxkb_object base;
    অক্ষর *code;
};

গণনা context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

কাঠামো rxkb_context {
    কাঠামো rxkb_object base;
    গণনা context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    কাঠামো list models;         /* list of struct rxkb_models */
    কাঠামো list layouts;        /* list of struct rxkb_layouts */
    কাঠামো list option_groups;  /* list of struct rxkb_option_group */

    darray(অক্ষর *) includes;


    ATTR_PRINTF(3, 0) শূন্য (*log_fn)(কাঠামো rxkb_context *ctx,
                                     গণনা rxkb_log_level level,
                                     ধ্রুবক অক্ষর *fmt, va_list args);
    গণনা rxkb_log_level log_level;

    শূন্য *userdata;
};

কাঠামো rxkb_model {
    কাঠামো rxkb_object base;

    অক্ষর *name;
    অক্ষর *vendor;
    অক্ষর *description;
    গণনা rxkb_popularity popularity;
};

কাঠামো rxkb_layout {
    কাঠামো rxkb_object base;

    অক্ষর *name;
    অক্ষর *brief;
    অক্ষর *description;
    অক্ষর *variant;
    গণনা rxkb_popularity popularity;

    কাঠামো list iso639s;  /* list of struct rxkb_iso639_code */
    কাঠামো list iso3166s; /* list of struct rxkb_iso3166_code */
};

কাঠামো rxkb_option_group {
    কাঠামো rxkb_object base;

    bool allow_multiple;
    কাঠামো list options; /* list of struct rxkb_options */
    অক্ষর *name;
    অক্ষর *description;
    গণনা rxkb_popularity popularity;
};

কাঠামো rxkb_option {
    কাঠামো rxkb_object base;

    অক্ষর *name;
    অক্ষর *brief;
    অক্ষর *description;
    গণনা rxkb_popularity popularity;
    bool layout_specific;
};

স্থির bool
parse(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *path,
      গণনা rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
স্থির শূন্য
rxkb_log(কাঠামো rxkb_context *ctx, গণনা rxkb_log_level level,
         ধ্রুবক অক্ষর *fmt, ...)
{
    va_list args;

    যদি (ctx->log_level < level)
        ফেরত;

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
#define rxkb_log_with_code(ctx, level, বার্তা_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(বার্তা_id, fmt), ##__VA_ARGS__)
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
কাঠামো type_ * type_##_ref(কাঠামো type_ *object) { \
    rxkb_object_ref(&object->base); \
    ফেরত object; \
} \
কাঠামো type_ * type_##_unref(কাঠামো type_ *object) { \
    যদি (!object) ফেরত ফাঁকা; \
    প্রতিজ্ঞা(object->base.refcount >= 1); \
    যদি (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        মুক্ত(object); \
    } \
    ফেরত ফাঁকা;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
স্থির অন্তঃরেখ কাঠামো type_ * type_##_create(কাঠামো rxkb_object *parent) { \
    কাঠামো type_ *t = calloc(1, আকার *t); \
    যদি (t) \
        rxkb_object_init(&t->base, parent); \
    ফেরত t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(কাঠামো type_ *object) { \
    ফেরত object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ধ্রুবক অক্ষর*)

#define DECLARE_FIRST_পরবর্তী_FOR_TYPE(type_, parent_type_, parent_field_) \
কাঠামো type_ * type_##_first(কাঠামো parent_type_ *parent) { \
    কাঠামো type_ *o = ফাঁকা; \
    যদি (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    ফেরত o; \
} \
কাঠামো type_ * \
type_##_next(কাঠামো type_ *o) \
{ \
    কাঠামো parent_type_ *parent; \
    কাঠামো type_ *পরবর্তী; \
    parent = container_of(o->base.parent, কাঠামো parent_type_, base); \
    পরবর্তী = list_first_entry(&o->base.link, o, base.link); \
    যদি (list_is_last(&parent->parent_field_, &o->base.link)) \
        ফেরত ফাঁকা; \
    ফেরত পরবর্তী; \
}

স্থির শূন্য
rxkb_object_init(কাঠামো rxkb_object *object, কাঠামো rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

স্থির শূন্য *
rxkb_object_ref(কাঠামো rxkb_object *object)
{
    প্রতিজ্ঞা(object->refcount >= 1);
    ++object->refcount;
    ফেরত object;
}

স্থির শূন্য
rxkb_iso639_code_destroy(কাঠামো rxkb_iso639_code *code)
{
    মুক্ত(code->code);
}

কাঠামো rxkb_iso639_code *
rxkb_layout_get_iso639_first(কাঠামো rxkb_layout *layout)
{
    কাঠামো rxkb_iso639_code *code = ফাঁকা;

    যদি (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    ফেরত code;
}

কাঠামো rxkb_iso639_code *
rxkb_iso639_code_পরবর্তী(কাঠামো rxkb_iso639_code *code)
{
    কাঠামো rxkb_iso639_code *পরবর্তী = ফাঁকা;
    কাঠামো rxkb_layout *layout;

    layout = container_of(code->base.parent, কাঠামো rxkb_layout, base);

    যদি (list_is_last(&layout->iso639s, &code->base.link))
        ফেরত ফাঁকা;

    পরবর্তী = list_first_entry(&code->base.link, code, base.link);

    ফেরত পরবর্তী;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

স্থির শূন্য
rxkb_iso3166_code_destroy(কাঠামো rxkb_iso3166_code *code)
{
    মুক্ত(code->code);
}

কাঠামো rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(কাঠামো rxkb_layout *layout)
{
    কাঠামো rxkb_iso3166_code *code = ফাঁকা;

    যদি (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    ফেরত code;
}

কাঠামো rxkb_iso3166_code *
rxkb_iso3166_code_পরবর্তী(কাঠামো rxkb_iso3166_code *code)
{
    কাঠামো rxkb_iso3166_code *পরবর্তী = ফাঁকা;
    কাঠামো rxkb_layout *layout;

    layout = container_of(code->base.parent, কাঠামো rxkb_layout, base);

    যদি (list_is_last(&layout->iso3166s, &code->base.link))
        ফেরত ফাঁকা;

    পরবর্তী = list_first_entry(&code->base.link, code, base.link);

    ফেরত পরবর্তী;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

স্থির শূন্য
rxkb_option_destroy(কাঠামো rxkb_option *o)
{
    মুক্ত(o->name);
    মুক্ত(o->brief);
    মুক্ত(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, গণনা rxkb_popularity);
bool rxkb_option_is_layout_specific(কাঠামো rxkb_option *object) {
    ফেরত object->layout_specific;
}
DECLARE_FIRST_পরবর্তী_FOR_TYPE(rxkb_option, rxkb_option_group, options);

স্থির শূন্য
rxkb_layout_destroy(কাঠামো rxkb_layout *l)
{
    কাঠামো rxkb_iso639_code *iso639, *tmp_639;
    কাঠামো rxkb_iso3166_code *iso3166, *tmp_3166;

    মুক্ত(l->name);
    মুক্ত(l->brief);
    মুক্ত(l->description);
    মুক্ত(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, গণনা rxkb_popularity);
DECLARE_FIRST_পরবর্তী_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

স্থির শূন্য
rxkb_model_destroy(কাঠামো rxkb_model *m)
{
    মুক্ত(m->name);
    মুক্ত(m->vendor);
    মুক্ত(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, গণনা rxkb_popularity);
DECLARE_FIRST_পরবর্তী_FOR_TYPE(rxkb_model, rxkb_context, models);

স্থির শূন্য
rxkb_option_group_destroy(কাঠামো rxkb_option_group *og)
{
    কাঠামো rxkb_option *o, *otmp;

    মুক্ত(og->name);
    মুক্ত(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(কাঠামো rxkb_option_group *g)
{
    ফেরত g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, গণনা rxkb_popularity);
DECLARE_FIRST_পরবর্তী_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

স্থির শূন্য
rxkb_context_destroy(কাঠামো rxkb_context *ctx)
{
    কাঠামো rxkb_model *m, *mtmp;
    কাঠামো rxkb_layout *l, *ltmp;
    কাঠামো rxkb_option_group *og, *ogtmp;
    অক্ষর **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    প্রতিজ্ঞা(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    প্রতিজ্ঞা(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    প্রতিজ্ঞা(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        মুক্ত(*path);
    darray_free(ctx->includes);

    প্রতিজ্ঞা(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, গণনা rxkb_log_level);

স্থির অক্ষর *
rxkb_context_getenv(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *name)
{
    যদি (ctx->use_secure_getenv) {
        ফেরত secure_getenv(name);
    } অন্যথা {
        ফেরত getenv(name);
    }
}


শূন্য
rxkb_context_set_log_level(কাঠামো rxkb_context *ctx,
                           গণনা rxkb_log_level level)
{
    ctx->log_level = level;
}

স্থির ধ্রুবক অক্ষর *
log_level_to_prefix(গণনা rxkb_log_level level)
{
    নির্বাচন (level) {
    ক্ষেত্র RXKB_LOG_LEVEL_DEBUG:
        ফেরত "xkbregistry: DEBUG: ";
    ক্ষেত্র RXKB_LOG_LEVEL_INFO:
        ফেরত "xkbregistry: INFO: ";
    ক্ষেত্র RXKB_LOG_LEVEL_WARNING:
        ফেরত "xkbregistry: WARNING: ";
    ক্ষেত্র RXKB_LOG_LEVEL_ERROR:
        ফেরত "xkbregistry: ERROR: ";
    ক্ষেত্র RXKB_LOG_LEVEL_CRITICAL:
        ফেরত "xkbregistry: CRITICAL: ";
    পূর্বনির্ধারিত:
        ফেরত ফাঁকা;
    }
}

ATTR_PRINTF(3, 0) স্থির শূন্য
default_log_fn(কাঠামো rxkb_context *ctx, গণনা rxkb_log_level level,
               ধ্রুবক অক্ষর *fmt, va_list args)
{
    ধ্রুবক অক্ষর *prefix = log_level_to_prefix(level);

    যদি (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

স্থির গণনা rxkb_log_level
log_level(ধ্রুবক অক্ষর *level) {
    errno = 0;
    অক্ষর *endptr;
    ধ্রুবক long lvl = strtol(level, &endptr, 10);
    যদি (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ফেরত (গণনা rxkb_log_level)lvl;

    যদি (istreq_prefix("crit", level))
        ফেরত RXKB_LOG_LEVEL_CRITICAL;
    যদি (istreq_prefix("err", level))
        ফেরত RXKB_LOG_LEVEL_ERROR;
    যদি (istreq_prefix("warn", level))
        ফেরত RXKB_LOG_LEVEL_WARNING;
    যদি (istreq_prefix("info", level))
        ফেরত RXKB_LOG_LEVEL_INFO;
    যদি (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ফেরত RXKB_LOG_LEVEL_DEBUG;

    ফেরত RXKB_LOG_LEVEL_ERROR;
}

কাঠামো rxkb_context *
rxkb_context_new(গণনা rxkb_context_flags flags)
{
    কাঠামো rxkb_context *ctx = rxkb_context_create(ফাঁকা);
    ধ্রুবক অক্ষর *env;

    যদি (!ctx)
        ফেরত ফাঁকা;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    যদি (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    স্থির ধ্রুবক গণনা rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    যদি (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        মুক্ত(ctx);
        ফেরত ফাঁকা;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    যদি (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_মূল);
        rxkb_context_unref(ctx);
        ফেরত ফাঁকা;
    }

    ফেরত ctx;
}

শূন্য
rxkb_context_set_log_fn(কাঠামো rxkb_context *ctx,
                        শূন্য (*log_fn)(কাঠামো rxkb_context *ctx,
                                       গণনা rxkb_log_level level,
                                       ধ্রুবক অক্ষর *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *path)
{
    পূর্ণসংখ্যা err = 0;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        যাও error;
    }

    কাঠামো stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        যাও error;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        যাও error;
    }

    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        যাও error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    অক্ষর rules[PATH_MAX];
    যদি (!snprintf_safe(rules, আকার(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                আকার(rules), path, DEFAULT_XKB_RULES);
        যাও error;
    }

    অক্ষর * ধ্রুবক tmp = strdup(path);
    যদি (!tmp) {
        err = ENOMEM;
        যাও error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ফেরত true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ফেরত false;
}

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES

স্থির পূর্ণসংখ্যা
compare_str(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
    ফেরত strcmp(*(অক্ষর **)a, *(অক্ষর **) b);
}

স্থির পূর্ণসংখ্যা
add_direct_subdirectories(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *path,
                          darray_string *extensions,
                          darray_আকার_t versioned_গুনতি,
                          আকার_t versioned_path_দৈর্ঘ্য)
{
    পূর্ণসংখ্যা ret = 0;
    পূর্ণসংখ্যা err = ENOMEM;
    DIR *dir = ফাঁকা;

    /* Check extensions parent directory */
    কাঠামো stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        যাও err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        যাও err;
    }
    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        যাও err;
    }

    dir = opendir(path);
    যদি (dir == ফাঁকা) {
        err = EACCES;
        যাও err;
    }

    কাঠামো dirent *entry;
    অক্ষর path_buf[PATH_MAX] = "";
    versioned_path_দৈর্ঘ্য++; /* Additional final ‘/’ */
    যখন ((entry = readdir(dir)) != ফাঁকা) {
        ধ্রুবক অক্ষর *name = entry->d_name;

        /* Skip special entries */
        যদি (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            চালু;

        যদি (!snprintf_safe(path_buf, আকার(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            যাও err;
        }
        যদি (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            চালু;

        /* Skip if the corresponding versioned directory is already included */
        জন্য (darray_আকার_t i = 0; i < versioned_গুনতি; i++) {
            ধ্রুবক অক্ষর * ধ্রুবক পূর্ব_name = darray_item(*extensions, i)
                                         + versioned_path_দৈর্ঘ্য;
            যদি (strcmp(name, পূর্ব_name) == 0)
                যাও পরবর্তী;
        }

        অক্ষর *ext_path = strdup_safe(path_buf);
        যদি (!ext_path) {
            err = ENOMEM;
            যাও err;
        }

        darray_append(*extensions, ext_path);
পরবর্তী:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    যদি (darray_আকার(*extensions) > versioned_গুনতি) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_বস্তুসমূহ(*extensions) + versioned_গুনতি,
              darray_আকার(*extensions) - versioned_গুনতি,
              আকার(*darray_বস্তুসমূহ(*extensions)), &compare_str);
        অক্ষর **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_গুনতি) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    ফেরত ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    যদি (dir)
        closedir(dir);

    ফেরত ret;
}
#endif

bool
rxkb_context_include_path_append_default(কাঠামো rxkb_context *ctx)
{
    অক্ষর user_path[PATH_MAX];
    bool ret = false;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ফেরত false;
    }

    ধ্রুবক অক্ষর * ধ্রুবক home = rxkb_context_getenv(ctx, "HOME");
    ধ্রুবক অক্ষর * ধ্রুবক xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    যদি (xdg != ফাঁকা) {
        যদি (snprintf_safe(user_path, আকার(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } অন্যথা যদি (home != ফাঁকা) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        যদি (snprintf_safe(user_path, আকার(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    যদি (home != ফাঁকা) {
        যদি (snprintf_safe(user_path, আকার(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ধ্রুবক অক্ষর * ধ্রুবক extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != ফাঁকা) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ধ্রুবক অক্ষর *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    যদি (extensions_path == ফাঁকা)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    আকার_t versioned_path_দৈর্ঘ্য = 0;
    যদি (extensions_path != ফাঁকা) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_দৈর্ঘ্য = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    যদি (extensions_path == ফাঁকা)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    যদি (extensions_path != ফাঁকা) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_আকার(extensions),
                                         versioned_path_দৈর্ঘ্য);
    }

    অক্ষর **ext_path;
    darray_foreach(ext_path, extensions) {
        মুক্ত(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ধ্রুবক অক্ষর * ধ্রুবক মূল = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ধ্রুবক bool has_মূল = rxkb_context_include_path_append(
        ctx, ((মূল != ফাঁকা) ? মূল: DFLT_XKB_CONFIG_মূল)
    );
    ret |= has_মূল;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    যদি (!has_মূল && (মূল == ফাঁকা || মূল[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_মূল,
                 ((মূল == ফাঁকা) ? DFLT_XKB_CONFIG_মূল : মূল));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_মূল);
    }

    ফেরত ret;
}

bool
rxkb_context_parse_default_ruleset(কাঠামো rxkb_context *ctx)
{
    ফেরত rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *ruleset)
{
    অক্ষর **path;
    bool success = false;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        ফেরত false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        অক্ষর rules[PATH_MAX];

        যদি (snprintf_safe(rules, আকার(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            যদি (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        যদি (ctx->load_extra_rules_files &&
            snprintf_safe(rules, আকার(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            যদি (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ফেরত success;
}


শূন্য
rxkb_context_set_user_তথ্য(কাঠামো rxkb_context *ctx, শূন্য *userdata)
{
    ctx->userdata = userdata;
}

শূন্য *
rxkb_context_get_user_তথ্য(কাঠামো rxkb_context *ctx)
{
    ফেরত ctx->userdata;
}

স্থির অন্তঃরেখ bool
is_গ্রন্থি(xml_গ্রন্থি *গ্রন্থি, ধ্রুবক অক্ষর *name)
{
    ফেরত গ্রন্থি->type == XML_ELEMENT_গ্রন্থি &&
        xmlStrEqual(গ্রন্থি->name, (ধ্রুবক xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
স্থির অক্ষর *
extract_text(xml_গ্রন্থি *গ্রন্থি)
{
    xml_গ্রন্থি *n;

    জন্য (n = গ্রন্থি->children; n; n = n->পরবর্তী) {
        যদি (n->type == XML_TEXT_গ্রন্থি)
            ফেরত (অক্ষর *)xmlStrdup(n->content);
    }
    ফেরত ফাঁকা;
}

/* Data from “configItem” node */
কাঠামো config_item {
    অক্ষর *name;
    অক্ষর *description;
    অক্ষর *brief;
    অক্ষর *vendor;
    গণনা rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = ফাঁকা, \
    .description = ফাঁকা, \
    .brief = ফাঁকা, \
    .vendor = ফাঁকা, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

স্থির শূন্য
config_item_free(কাঠামো config_item *config) {
    মুক্ত(config->name);
    মুক্ত(config->description);
    মুক্ত(config->brief);
    মুক্ত(config->vendor);
}

স্থির bool
parse_config_item(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *parent,
                  কাঠামো config_item *config)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;
    xml_গ্রন্থি *ci = ফাঁকা;

    জন্য (ci = parent->children; ci; ci = ci->পরবর্তী) {
        যদি (is_গ্রন্থি(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ধ্রুবক xmlChar*)"popularity");
            যদি (raw_popularity) {
                যদি (xmlStrEqual(raw_popularity, (ধ্রুবক xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                অন্যথা যদি (xmlStrEqual(raw_popularity, (ধ্রুবক xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                অন্যথা
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->সারি, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ধ্রুবক xmlChar*)"layout-specific");
            যদি (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ধ্রুবক xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            জন্য (গ্রন্থি = ci->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
                যদি (is_গ্রন্থি(গ্রন্থি, "name"))
                    config->name = extract_text(গ্রন্থি);
                অন্যথা যদি (is_গ্রন্থি(গ্রন্থি, "description"))
                    config->description = extract_text(গ্রন্থি);
                অন্যথা যদি (is_গ্রন্থি(গ্রন্থি, "shortDescription"))
                    config->brief = extract_text(গ্রন্থি);
                অন্যথা যদি (is_গ্রন্থি(গ্রন্থি, "vendor"))
                    config->vendor = extract_text(গ্রন্থি);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            যদি (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->সারি);
                config_item_free(config);
                ফেরত false;
            }

            ফেরত true; /* only one configItem allowed in the dtd */
        }
    }

    ফেরত false;
}

স্থির শূন্য
parse_model(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *model,
            গণনা rxkb_popularity popularity)
{
    কাঠামো config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, model, &config)) {
        কাঠামো rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            যদি (streq(m->name, config.name)) {
                config_item_free(&config);
                ফেরত;
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

স্থির শূন্য
parse_model_list(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *model_list,
                 গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;

    জন্য (গ্রন্থি = model_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "model"))
            parse_model(ctx, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
parse_language_list(xml_গ্রন্থি *language_list, কাঠামো rxkb_layout *layout)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;
    কাঠামো rxkb_iso639_code *code;

    জন্য (গ্রন্থি = language_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "iso639Id")) {
            অক্ষর *str = extract_text(গ্রন্থি);
            কাঠামো rxkb_object *parent;

            যদি (!str || strlen(str) != 3) {
                মুক্ত(str);
                চালু;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

স্থির শূন্য
parse_country_list(xml_গ্রন্থি *country_list, কাঠামো rxkb_layout *layout)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;
    কাঠামো rxkb_iso3166_code *code;

    জন্য (গ্রন্থি = country_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "iso3166Id")) {
            অক্ষর *str = extract_text(গ্রন্থি);
            কাঠামো rxkb_object *parent;

            যদি (!str || strlen(str) != 2) {
                মুক্ত(str);
                চালু;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

স্থির শূন্য
parse_variant(কাঠামো rxkb_context *ctx, কাঠামো rxkb_layout *l,
              xml_গ্রন্থি *variant, গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *ci;
    কাঠামো config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, variant, &config)) {
        কাঠামো rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            যদি (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                বিরতি;
            }
        }

        যদি (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ফাঁকা ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            জন্য (ci = variant->children; ci; ci = ci->পরবর্তী) {
                xml_গ্রন্থি *গ্রন্থি;

                যদি (!is_গ্রন্থি(ci, "configItem"))
                    চালু;

                bool found_language_list = false;
                bool found_country_list = false;
                জন্য (গ্রন্থি = ci->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
                    যদি (is_গ্রন্থি(গ্রন্থি, "languageList")) {
                        parse_language_list(গ্রন্থি, v);
                        found_language_list = true;
                    }
                    যদি (is_গ্রন্থি(গ্রন্থি, "countryList")) {
                        parse_country_list(গ্রন্থি, v);
                        found_country_list = true;
                    }
                }
                যদি (!found_language_list) {
                    // inherit from parent layout
                    কাঠামো rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        কাঠামো rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                যদি (!found_country_list) {
                    // inherit from parent layout
                    কাঠামো rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        কাঠামো rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } অন্যথা {
            config_item_free(&config);
        }
    }
}

স্থির শূন্য
parse_variant_list(কাঠামো rxkb_context *ctx, কাঠামো rxkb_layout *l,
                   xml_গ্রন্থি *variant_list, গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;

    জন্য (গ্রন্থি = variant_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "variant"))
            parse_variant(ctx, l, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
parse_layout(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *layout,
             গণনা rxkb_popularity popularity)
{
    কাঠামো config_item config = config_item_new(popularity);
    কাঠামো rxkb_layout *l;
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;
    bool exists = false;

    যদি (!parse_config_item(ctx, layout, &config))
        ফেরত;

    list_for_each(l, &ctx->layouts, base.link) {
        যদি (streq(l->name, config.name) && l->variant == ফাঁকা) {
            exists = true;
            বিরতি;
        }
    }

    যদি (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ফাঁকা;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } অন্যথা {
        config_item_free(&config);
    }

    জন্য (গ্রন্থি = layout->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "variantList")) {
            parse_variant_list(ctx, l, গ্রন্থি, popularity);
        }
        যদি (!exists && is_গ্রন্থি(গ্রন্থি, "configItem")) {
            xml_গ্রন্থি *ll;
            জন্য (ll = গ্রন্থি->children; ll; ll = ll->পরবর্তী) {
                যদি (is_গ্রন্থি(ll, "languageList"))
                    parse_language_list(ll, l);
                যদি (is_গ্রন্থি(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

স্থির শূন্য
parse_layout_list(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *layout_list,
                  গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;

    জন্য (গ্রন্থি = layout_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "layout"))
            parse_layout(ctx, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
parse_option(কাঠামো rxkb_context *ctx, কাঠামো rxkb_option_group *group,
             xml_গ্রন্থি *option, গণনা rxkb_popularity popularity)
{
    কাঠামো config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, option, &config)) {
        কাঠামো rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            যদি (streq(o->name, config.name)) {
                config_item_free(&config);
                ফেরত;
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

স্থির শূন্য
parse_group(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *group,
            গণনা rxkb_popularity popularity)
{
    কাঠামো config_item config = config_item_new(popularity);
    কাঠামো rxkb_option_group *g;
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;
    xmlChar *multiple;
    bool exists = false;

    যদি (!parse_config_item(ctx, group, &config))
        ফেরত;

    list_for_each(g, &ctx->option_groups, base.link) {
        যদি (streq(g->name, config.name)) {
            exists = true;
            বিরতি;
        }
    }

    যদি (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ধ্রুবক xmlChar*)"allowMultipleSelection");
        যদি (multiple && xmlStrEqual(multiple, (ধ্রুবক xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } অন্যথা {
        config_item_free(&config);
    }

    জন্য (গ্রন্থি = group->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "option"))
            parse_option(ctx, g, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
parse_option_list(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *option_list,
                  গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;

    জন্য (গ্রন্থি = option_list->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "group"))
            parse_group(ctx, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
parse_rules_xml(কাঠামো rxkb_context *ctx, xml_গ্রন্থি *মূল,
                গণনা rxkb_popularity popularity)
{
    xml_গ্রন্থি *গ্রন্থি = ফাঁকা;

    জন্য (গ্রন্থি = মূল->children; গ্রন্থি; গ্রন্থি = গ্রন্থি->পরবর্তী) {
        যদি (is_গ্রন্থি(গ্রন্থি, "modelList"))
            parse_model_list(ctx, গ্রন্থি, popularity);
        অন্যথা যদি (is_গ্রন্থি(গ্রন্থি, "layoutList"))
            parse_layout_list(ctx, গ্রন্থি, popularity);
        অন্যথা যদি (is_গ্রন্থি(গ্রন্থি, "optionList"))
            parse_option_list(ctx, গ্রন্থি, popularity);
    }
}

স্থির শূন্য
ATTR_PRINTF(2, 0)
xml_error_func(শূন্য *ctx, ধ্রুবক অক্ষর *বার্তা, ...)
{
    স্থির অক্ষর buf[PATH_MAX];
    স্থির আকার_t slen = 0;
    va_list args;
    পূর্ণসংখ্যা rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, বার্তা);
    rc = vsnprintf(&buf[slen], আকার(buf) - slen, বার্তা, args);
    va_end(args);

    /* This shouldn't really happen */
    যদি (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_বাফার_আকার,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, আকার(buf));
        ফেরত;
    }

    slen += (আকার_t)rc;
    যদি (slen >= আকার(buf)) {
        /* truncated, let's flush this */
        buf[আকার(buf) - 1] = '\n';
        slen = আকার(buf);
    }

    /* We're assuming here that the last character is \n. */
    যদি (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, আকার(buf));
        slen = 0;
    }
}

#যদি HAVE_XML_CTXT_SET_ERRORHANDLER
স্থির শূন্য
xml_structured_error_func(শূন্য *user_তথ্য_2, ধ্রুবক xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_তথ্য_2);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#অন্যথা
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

স্থির bool
validate(কাঠামো rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ধ্রুবক অক্ষর dtdstr[] =
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

#যদি HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    যদি (!xmlCtxt)
        ফেরত false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_প্রবেশ_Ptr pinput =
        xml_New_প্রবেশ_From_Memory(ফাঁকা, dtdstr, ARRAY_আকার(dtdstr) - 1,
                              XML_প্রবেশ_BUF_STATIC);
    যদি (!pinput)
        যাও dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ফাঁকা, ফাঁকা);
#অন্যথা
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_প্রবেশ_বাফার_Ptr buf =
        xml_Parser_প্রবেশ_বাফার_Create_Mem(dtdstr, ARRAY_আকার(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    যদি (!buf)
        যাও dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ফাঁকা, buf, XML_CHAR_ENCODING_UTF8);
#endif

    যদি (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        যাও dtd_error;
    }

#যদি HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#অন্যথা
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    যদি (!dtdvalid) {
        success = false;
        যাও ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_তথ্য_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#যদি HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    ফেরত success;
}

স্থির bool
parse(কাঠামো rxkb_context *ctx, ধ্রুবক অক্ষর *path,
      গণনা rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ফাঁকা;
    xml_গ্রন্থি *মূল = ফাঁকা;

    যদি (!check_eaccess(path, R_OK))
        ফেরত false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    যদি (!xmlCtxt)
        ফেরত false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#অন্যথা
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#যদি HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#যদি !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, ফাঁকা, 0);
    যদি (!doc)
        যাও parse_error;

    যদি (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        যাও validate_error;
    }

    মূল = xml_Doc_Get_মূল_Element(doc);
    parse_rules_xml(ctx, মূল, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#যদি !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(ফাঁকা, ফাঁকা);
#endif
#যদি HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, ফাঁকা, ফাঁকা);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    ফেরত success;
}
