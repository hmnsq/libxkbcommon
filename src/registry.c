/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <libxml/parser.h>

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    #অন্তর্ভাৱ <limits.h>
    #যদি HAVE_UNISTD_H && HAVE_DIRENT_H
        #অন্তর্ভাৱ <dirent.h>
        #অন্তর্ভাৱ <unistd.h>
    #endif
#endif

#অন্তর্ভাৱ "xkbcommon/xkbregistry.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "util-list.h"
#অন্তর্ভাৱ "util-mem.h"

সংরচনা rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
সংরচনা rxkb_object {
    সংরচনা rxkb_object *parent;
    uint32_t refcount;
    সংরচনা list link;
};

সংরচনা rxkb_iso639_code {
    সংরচনা rxkb_object base;
    ৱর্ণ *code;
};

সংরচনা rxkb_iso3166_code {
    সংরচনা rxkb_object base;
    ৱর্ণ *code;
};

গণনা context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

সংরচনা rxkb_context {
    সংরচনা rxkb_object base;
    গণনা context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    সংরচনা list models;         /* list of struct rxkb_models */
    সংরচনা list layouts;        /* list of struct rxkb_layouts */
    সংরচনা list option_groups;  /* list of struct rxkb_option_group */

    darray(ৱর্ণ *) includes;


    ATTR_PRINTF(3, 0) শূন্য (*log_fn)(সংরচনা rxkb_context *ctx,
                                     গণনা rxkb_log_level level,
                                     ধ্রুৱ ৱর্ণ *fmt, va_list args);
    গণনা rxkb_log_level log_level;

    শূন্য *userdata;
};

সংরচনা rxkb_model {
    সংরচনা rxkb_object base;

    ৱর্ণ *name;
    ৱর্ণ *vendor;
    ৱর্ণ *description;
    গণনা rxkb_popularity popularity;
};

সংরচনা rxkb_layout {
    সংরচনা rxkb_object base;

    ৱর্ণ *name;
    ৱর্ণ *brief;
    ৱর্ণ *description;
    ৱর্ণ *variant;
    গণনা rxkb_popularity popularity;

    সংরচনা list iso639s;  /* list of struct rxkb_iso639_code */
    সংরচনা list iso3166s; /* list of struct rxkb_iso3166_code */
};

সংরচনা rxkb_option_group {
    সংরচনা rxkb_object base;

    bool allow_multiple;
    সংরচনা list options; /* list of struct rxkb_options */
    ৱর্ণ *name;
    ৱর্ণ *description;
    গণনা rxkb_popularity popularity;
};

সংরচনা rxkb_option {
    সংরচনা rxkb_object base;

    ৱর্ণ *name;
    ৱর্ণ *brief;
    ৱর্ণ *description;
    গণনা rxkb_popularity popularity;
    bool layout_specific;
};

স্থির bool
parse(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *path,
      গণনা rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
স্থির শূন্য
rxkb_log(সংরচনা rxkb_context *ctx, গণনা rxkb_log_level level,
         ধ্রুৱ ৱর্ণ *fmt, ...)
{
    va_list args;

    যদি (ctx->log_level < level)
        ঘূৰাই;

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
#define rxkb_log_with_code(ctx, level, বাৰ্তা_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(বাৰ্তা_id, fmt), ##__VA_ARGS__)
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
সংরচনা type_ * type_##_ref(সংরচনা type_ *object) { \
    rxkb_object_ref(&object->base); \
    ঘূৰাই object; \
} \
সংরচনা type_ * type_##_unref(সংরচনা type_ *object) { \
    যদি (!object) ঘূৰাই রিক্ত; \
    প্রতিজ্ঞা(object->base.refcount >= 1); \
    যদি (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        মুক্ত(object); \
    } \
    ঘূৰাই রিক্ত;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
স্থির অন্তঃস্থ সংরচনা type_ * type_##_create(সংরচনা rxkb_object *parent) { \
    সংরচনা type_ *t = calloc(1, পরিমাণ *t); \
    যদি (t) \
        rxkb_object_init(&t->base, parent); \
    ঘূৰাই t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(সংরচনা type_ *object) { \
    ঘূৰাই object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ধ্রুৱ ৱর্ণ*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
সংরচনা type_ * type_##_first(সংরচনা parent_type_ *parent) { \
    সংরচনা type_ *o = রিক্ত; \
    যদি (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    ঘূৰাই o; \
} \
সংরচনা type_ * \
type_##_next(সংরচনা type_ *o) \
{ \
    সংরচনা parent_type_ *parent; \
    সংরচনা type_ *next; \
    parent = container_of(o->base.parent, সংরচনা parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    যদি (list_is_last(&parent->parent_field_, &o->base.link)) \
        ঘূৰাই রিক্ত; \
    ঘূৰাই next; \
}

স্থির শূন্য
rxkb_object_init(সংরচনা rxkb_object *object, সংরচনা rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

স্থির শূন্য *
rxkb_object_ref(সংরচনা rxkb_object *object)
{
    প্রতিজ্ঞা(object->refcount >= 1);
    ++object->refcount;
    ঘূৰাই object;
}

স্থির শূন্য
rxkb_iso639_code_destroy(সংরচনা rxkb_iso639_code *code)
{
    মুক্ত(code->code);
}

সংরচনা rxkb_iso639_code *
rxkb_layout_get_iso639_first(সংরচনা rxkb_layout *layout)
{
    সংরচনা rxkb_iso639_code *code = রিক্ত;

    যদি (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    ঘূৰাই code;
}

সংরচনা rxkb_iso639_code *
rxkb_iso639_code_next(সংরচনা rxkb_iso639_code *code)
{
    সংরচনা rxkb_iso639_code *next = রিক্ত;
    সংরচনা rxkb_layout *layout;

    layout = container_of(code->base.parent, সংরচনা rxkb_layout, base);

    যদি (list_is_last(&layout->iso639s, &code->base.link))
        ঘূৰাই রিক্ত;

    next = list_first_entry(&code->base.link, code, base.link);

    ঘূৰাই next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

স্থির শূন্য
rxkb_iso3166_code_destroy(সংরচনা rxkb_iso3166_code *code)
{
    মুক্ত(code->code);
}

সংরচনা rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(সংরচনা rxkb_layout *layout)
{
    সংরচনা rxkb_iso3166_code *code = রিক্ত;

    যদি (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    ঘূৰাই code;
}

সংরচনা rxkb_iso3166_code *
rxkb_iso3166_code_next(সংরচনা rxkb_iso3166_code *code)
{
    সংরচনা rxkb_iso3166_code *next = রিক্ত;
    সংরচনা rxkb_layout *layout;

    layout = container_of(code->base.parent, সংরচনা rxkb_layout, base);

    যদি (list_is_last(&layout->iso3166s, &code->base.link))
        ঘূৰাই রিক্ত;

    next = list_first_entry(&code->base.link, code, base.link);

    ঘূৰাই next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

স্থির শূন্য
rxkb_option_destroy(সংরচনা rxkb_option *o)
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
bool rxkb_option_is_layout_specific(সংরচনা rxkb_option *object) {
    ঘূৰাই object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

স্থির শূন্য
rxkb_layout_destroy(সংরচনা rxkb_layout *l)
{
    সংরচনা rxkb_iso639_code *iso639, *tmp_639;
    সংরচনা rxkb_iso3166_code *iso3166, *tmp_3166;

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
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

স্থির শূন্য
rxkb_model_destroy(সংরচনা rxkb_model *m)
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
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

স্থির শূন্য
rxkb_option_group_destroy(সংরচনা rxkb_option_group *og)
{
    সংরচনা rxkb_option *o, *otmp;

    মুক্ত(og->name);
    মুক্ত(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(সংরচনা rxkb_option_group *g)
{
    ঘূৰাই g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, গণনা rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

স্থির শূন্য
rxkb_context_destroy(সংরচনা rxkb_context *ctx)
{
    সংরচনা rxkb_model *m, *mtmp;
    সংরচনা rxkb_layout *l, *ltmp;
    সংরচনা rxkb_option_group *og, *ogtmp;
    ৱর্ণ **path;

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

স্থির ৱর্ণ *
rxkb_context_getenv(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *name)
{
    যদি (ctx->use_secure_getenv) {
        ঘূৰাই secure_getenv(name);
    } অন্যথা {
        ঘূৰাই getenv(name);
    }
}


শূন্য
rxkb_context_set_log_level(সংরচনা rxkb_context *ctx,
                           গণনা rxkb_log_level level)
{
    ctx->log_level = level;
}

স্থির ধ্রুৱ ৱর্ণ *
log_level_to_prefix(গণনা rxkb_log_level level)
{
    ৱিকল্প (level) {
    পক্ষ RXKB_LOG_LEVEL_DEBUG:
        ঘূৰাই "xkbregistry: DEBUG: ";
    পক্ষ RXKB_LOG_LEVEL_INFO:
        ঘূৰাই "xkbregistry: INFO: ";
    পক্ষ RXKB_LOG_LEVEL_WARNING:
        ঘূৰাই "xkbregistry: WARNING: ";
    পক্ষ RXKB_LOG_LEVEL_ERROR:
        ঘূৰাই "xkbregistry: ERROR: ";
    পক্ষ RXKB_LOG_LEVEL_CRITICAL:
        ঘূৰাই "xkbregistry: CRITICAL: ";
    স্ৱভাৱ:
        ঘূৰাই রিক্ত;
    }
}

ATTR_PRINTF(3, 0) স্থির শূন্য
default_log_fn(সংরচনা rxkb_context *ctx, গণনা rxkb_log_level level,
               ধ্রুৱ ৱর্ণ *fmt, va_list args)
{
    ধ্রুৱ ৱর্ণ *prefix = log_level_to_prefix(level);

    যদি (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

স্থির গণনা rxkb_log_level
log_level(ধ্রুৱ ৱর্ণ *level) {
    errno = 0;
    ৱর্ণ *endptr;
    ধ্রুৱ long lvl = strtol(level, &endptr, 10);
    যদি (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ঘূৰাই (গণনা rxkb_log_level)lvl;

    যদি (istreq_prefix("crit", level))
        ঘূৰাই RXKB_LOG_LEVEL_CRITICAL;
    যদি (istreq_prefix("err", level))
        ঘূৰাই RXKB_LOG_LEVEL_ERROR;
    যদি (istreq_prefix("warn", level))
        ঘূৰাই RXKB_LOG_LEVEL_WARNING;
    যদি (istreq_prefix("info", level))
        ঘূৰাই RXKB_LOG_LEVEL_INFO;
    যদি (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ঘূৰাই RXKB_LOG_LEVEL_DEBUG;

    ঘূৰাই RXKB_LOG_LEVEL_ERROR;
}

সংরচনা rxkb_context *
rxkb_context_new(গণনা rxkb_context_flags flags)
{
    সংরচনা rxkb_context *ctx = rxkb_context_create(রিক্ত);
    ধ্রুৱ ৱর্ণ *env;

    যদি (!ctx)
        ঘূৰাই রিক্ত;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    যদি (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    স্থির ধ্রুৱ গণনা rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    যদি (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        মুক্ত(ctx);
        ঘূৰাই রিক্ত;
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
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই ctx;
}

শূন্য
rxkb_context_set_log_fn(সংরচনা rxkb_context *ctx,
                        শূন্য (*log_fn)(সংরচনা rxkb_context *ctx,
                                       গণনা rxkb_log_level level,
                                       ধ্রুৱ ৱর্ণ *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *path)
{
    পূর্ণাঙ্ক err = 0;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        গচ্ছ error;
    }

    সংরচনা stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        গচ্ছ error;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        গচ্ছ error;
    }

    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        গচ্ছ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    ৱর্ণ rules[PATH_MAX];
    যদি (!snprintf_safe(rules, পরিমাণ(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                পরিমাণ(rules), path, DEFAULT_XKB_RULES);
        গচ্ছ error;
    }

    ৱর্ণ * ধ্রুৱ tmp = strdup(path);
    যদি (!tmp) {
        err = ENOMEM;
        গচ্ছ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ঘূৰাই true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ঘূৰাই false;
}

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES

স্থির পূর্ণাঙ্ক
compare_str(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ঘূৰাই strcmp(*(ৱর্ণ **)a, *(ৱর্ণ **) b);
}

স্থির পূর্ণাঙ্ক
add_direct_subdirectories(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *path,
                          darray_string *extensions,
                          darray_size_t versioned_গুনতি,
                          size_t versioned_path_length)
{
    পূর্ণাঙ্ক ret = 0;
    পূর্ণাঙ্ক err = ENOMEM;
    DIR *dir = রিক্ত;

    /* Check extensions parent directory */
    সংরচনা stat stat_buf;
    err = stat(path, &stat_buf);
    যদি (err != 0) {
        err = errno;
        গচ্ছ err;
    }
    যদি (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        গচ্ছ err;
    }
    যদি (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        গচ্ছ err;
    }

    dir = opendir(path);
    যদি (dir == রিক্ত) {
        err = EACCES;
        গচ্ছ err;
    }

    সংরচনা dirent *entry;
    ৱর্ণ path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    যেতিয়ালৈ ((entry = readdir(dir)) != রিক্ত) {
        ধ্রুৱ ৱর্ণ *name = entry->d_name;

        /* Skip special entries */
        যদি (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            অনুৱর্ত;

        যদি (!snprintf_safe(path_buf, পরিমাণ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            গচ্ছ err;
        }
        যদি (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            অনুৱর্ত;

        /* Skip if the corresponding versioned directory is already included */
        বাবে (darray_size_t i = 0; i < versioned_গুনতি; i++) {
            ধ্রুৱ ৱর্ণ * ধ্রুৱ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            যদি (strcmp(name, prev_name) == 0)
                গচ্ছ next;
        }

        ৱর্ণ *ext_path = strdup_safe(path_buf);
        যদি (!ext_path) {
            err = ENOMEM;
            গচ্ছ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    যদি (darray_size(*extensions) > versioned_গুনতি) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_বস্তুসমূহ(*extensions) + versioned_গুনতি,
              darray_size(*extensions) - versioned_গুনতি,
              পরিমাণ(*darray_বস্তুসমূহ(*extensions)), &compare_str);
        ৱর্ণ **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_গুনতি) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    ঘূৰাই ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    যদি (dir)
        closedir(dir);

    ঘূৰাই ret;
}
#endif

bool
rxkb_context_include_path_append_default(সংরচনা rxkb_context *ctx)
{
    ৱর্ণ user_path[PATH_MAX];
    bool ret = false;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ঘূৰাই false;
    }

    ধ্রুৱ ৱর্ণ * ধ্রুৱ home = rxkb_context_getenv(ctx, "HOME");
    ধ্রুৱ ৱর্ণ * ধ্রুৱ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    যদি (xdg != রিক্ত) {
        যদি (snprintf_safe(user_path, পরিমাণ(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } অন্যথা যদি (home != রিক্ত) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        যদি (snprintf_safe(user_path, পরিমাণ(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    যদি (home != রিক্ত) {
        যদি (snprintf_safe(user_path, পরিমাণ(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ধ্রুৱ ৱর্ণ * ধ্রুৱ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != রিক্ত) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#যদি HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ধ্রুৱ ৱর্ণ *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    যদি (extensions_path == রিক্ত)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    যদি (extensions_path != রিক্ত) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    যদি (extensions_path == রিক্ত)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    যদি (extensions_path != রিক্ত) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ৱর্ণ **ext_path;
    darray_foreach(ext_path, extensions) {
        মুক্ত(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ধ্রুৱ ৱর্ণ * ধ্রুৱ মূল = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ধ্রুৱ bool has_মূল = rxkb_context_include_path_append(
        ctx, ((মূল != রিক্ত) ? মূল: DFLT_XKB_CONFIG_মূল)
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
    যদি (!has_মূল && (মূল == রিক্ত || মূল[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_মূল,
                 ((মূল == রিক্ত) ? DFLT_XKB_CONFIG_মূল : মূল));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_মূল);
    }

    ঘূৰাই ret;
}

bool
rxkb_context_parse_default_ruleset(সংরচনা rxkb_context *ctx)
{
    ঘূৰাই rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *ruleset)
{
    ৱর্ণ **path;
    bool success = false;

    যদি (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        ঘূৰাই false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        ৱর্ণ rules[PATH_MAX];

        যদি (snprintf_safe(rules, পরিমাণ(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            যদি (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        যদি (ctx->load_extra_rules_files &&
            snprintf_safe(rules, পরিমাণ(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            যদি (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ঘূৰাই success;
}


শূন্য
rxkb_context_set_user_data(সংরচনা rxkb_context *ctx, শূন্য *userdata)
{
    ctx->userdata = userdata;
}

শূন্য *
rxkb_context_get_user_data(সংরচনা rxkb_context *ctx)
{
    ঘূৰাই ctx->userdata;
}

স্থির অন্তঃস্থ bool
is_গ্ৰন্থি(xml_গ্ৰন্থি *গ্ৰন্থি, ধ্রুৱ ৱর্ণ *name)
{
    ঘূৰাই গ্ৰন্থি->type == XML_ELEMENT_গ্ৰন্থি &&
        xmlStrEqual(গ্ৰন্থি->name, (ধ্রুৱ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
স্থির ৱর্ণ *
extract_text(xml_গ্ৰন্থি *গ্ৰন্থি)
{
    xml_গ্ৰন্থি *n;

    বাবে (n = গ্ৰন্থি->children; n; n = n->next) {
        যদি (n->type == XML_TEXT_গ্ৰন্থি)
            ঘূৰাই (ৱর্ণ *)xmlStrdup(n->content);
    }
    ঘূৰাই রিক্ত;
}

/* Data from “configItem” node */
সংরচনা config_item {
    ৱর্ণ *name;
    ৱর্ণ *description;
    ৱর্ণ *brief;
    ৱর্ণ *vendor;
    গণনা rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = রিক্ত, \
    .description = রিক্ত, \
    .brief = রিক্ত, \
    .vendor = রিক্ত, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

স্থির শূন্য
config_item_free(সংরচনা config_item *config) {
    মুক্ত(config->name);
    মুক্ত(config->description);
    মুক্ত(config->brief);
    মুক্ত(config->vendor);
}

স্থির bool
parse_config_item(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *parent,
                  সংরচনা config_item *config)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
    xml_গ্ৰন্থি *ci = রিক্ত;

    বাবে (ci = parent->children; ci; ci = ci->next) {
        যদি (is_গ্ৰন্থি(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ধ্রুৱ xmlChar*)"popularity");
            যদি (raw_popularity) {
                যদি (xmlStrEqual(raw_popularity, (ধ্রুৱ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                অন্যথা যদি (xmlStrEqual(raw_popularity, (ধ্রুৱ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                অন্যথা
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->সাৰি, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ধ্রুৱ xmlChar*)"layout-specific");
            যদি (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ধ্রুৱ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            বাবে (গ্ৰন্থি = ci->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
                যদি (is_গ্ৰন্থি(গ্ৰন্থি, "name"))
                    config->name = extract_text(গ্ৰন্থি);
                অন্যথা যদি (is_গ্ৰন্থি(গ্ৰন্থি, "description"))
                    config->description = extract_text(গ্ৰন্থি);
                অন্যথা যদি (is_গ্ৰন্থি(গ্ৰন্থি, "shortDescription"))
                    config->brief = extract_text(গ্ৰন্থি);
                অন্যথা যদি (is_গ্ৰন্থি(গ্ৰন্থি, "vendor"))
                    config->vendor = extract_text(গ্ৰন্থি);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            যদি (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->সাৰি);
                config_item_free(config);
                ঘূৰাই false;
            }

            ঘূৰাই true; /* only one configItem allowed in the dtd */
        }
    }

    ঘূৰাই false;
}

স্থির শূন্য
parse_model(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *model,
            গণনা rxkb_popularity popularity)
{
    সংরচনা config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, model, &config)) {
        সংরচনা rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            যদি (streq(m->name, config.name)) {
                config_item_free(&config);
                ঘূৰাই;
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
parse_model_list(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *model_list,
                 গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    বাবে (গ্ৰন্থি = model_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "model"))
            parse_model(ctx, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
parse_language_list(xml_গ্ৰন্থি *language_list, সংরচনা rxkb_layout *layout)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
    সংরচনা rxkb_iso639_code *code;

    বাবে (গ্ৰন্থি = language_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "iso639Id")) {
            ৱর্ণ *str = extract_text(গ্ৰন্থি);
            সংরচনা rxkb_object *parent;

            যদি (!str || strlen(str) != 3) {
                মুক্ত(str);
                অনুৱর্ত;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

স্থির শূন্য
parse_country_list(xml_গ্ৰন্থি *country_list, সংরচনা rxkb_layout *layout)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
    সংরচনা rxkb_iso3166_code *code;

    বাবে (গ্ৰন্থি = country_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "iso3166Id")) {
            ৱর্ণ *str = extract_text(গ্ৰন্থি);
            সংরচনা rxkb_object *parent;

            যদি (!str || strlen(str) != 2) {
                মুক্ত(str);
                অনুৱর্ত;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

স্থির শূন্য
parse_variant(সংরচনা rxkb_context *ctx, সংরচনা rxkb_layout *l,
              xml_গ্ৰন্থি *variant, গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *ci;
    সংরচনা config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, variant, &config)) {
        সংরচনা rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            যদি (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ভঙ্গ;
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
            v->brief = config.brief == রিক্ত ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            বাবে (ci = variant->children; ci; ci = ci->next) {
                xml_গ্ৰন্থি *গ্ৰন্থি;

                যদি (!is_গ্ৰন্থি(ci, "configItem"))
                    অনুৱর্ত;

                bool found_language_list = false;
                bool found_country_list = false;
                বাবে (গ্ৰন্থি = ci->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
                    যদি (is_গ্ৰন্থি(গ্ৰন্থি, "languageList")) {
                        parse_language_list(গ্ৰন্থি, v);
                        found_language_list = true;
                    }
                    যদি (is_গ্ৰন্থি(গ্ৰন্থি, "countryList")) {
                        parse_country_list(গ্ৰন্থি, v);
                        found_country_list = true;
                    }
                }
                যদি (!found_language_list) {
                    // inherit from parent layout
                    সংরচনা rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        সংরচনা rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                যদি (!found_country_list) {
                    // inherit from parent layout
                    সংরচনা rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        সংরচনা rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
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
parse_variant_list(সংরচনা rxkb_context *ctx, সংরচনা rxkb_layout *l,
                   xml_গ্ৰন্থি *variant_list, গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    বাবে (গ্ৰন্থি = variant_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "variant"))
            parse_variant(ctx, l, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
parse_layout(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *layout,
             গণনা rxkb_popularity popularity)
{
    সংরচনা config_item config = config_item_new(popularity);
    সংরচনা rxkb_layout *l;
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
    bool exists = false;

    যদি (!parse_config_item(ctx, layout, &config))
        ঘূৰাই;

    list_for_each(l, &ctx->layouts, base.link) {
        যদি (streq(l->name, config.name) && l->variant == রিক্ত) {
            exists = true;
            ভঙ্গ;
        }
    }

    যদি (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = রিক্ত;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } অন্যথা {
        config_item_free(&config);
    }

    বাবে (গ্ৰন্থি = layout->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "variantList")) {
            parse_variant_list(ctx, l, গ্ৰন্থি, popularity);
        }
        যদি (!exists && is_গ্ৰন্থি(গ্ৰন্থি, "configItem")) {
            xml_গ্ৰন্থি *ll;
            বাবে (ll = গ্ৰন্থি->children; ll; ll = ll->next) {
                যদি (is_গ্ৰন্থি(ll, "languageList"))
                    parse_language_list(ll, l);
                যদি (is_গ্ৰন্থি(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

স্থির শূন্য
parse_layout_list(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *layout_list,
                  গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    বাবে (গ্ৰন্থি = layout_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "layout"))
            parse_layout(ctx, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
parse_option(সংরচনা rxkb_context *ctx, সংরচনা rxkb_option_group *group,
             xml_গ্ৰন্থি *option, গণনা rxkb_popularity popularity)
{
    সংরচনা config_item config = config_item_new(popularity);

    যদি (parse_config_item(ctx, option, &config)) {
        সংরচনা rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            যদি (streq(o->name, config.name)) {
                config_item_free(&config);
                ঘূৰাই;
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
parse_group(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *group,
            গণনা rxkb_popularity popularity)
{
    সংরচনা config_item config = config_item_new(popularity);
    সংরচনা rxkb_option_group *g;
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;
    xmlChar *multiple;
    bool exists = false;

    যদি (!parse_config_item(ctx, group, &config))
        ঘূৰাই;

    list_for_each(g, &ctx->option_groups, base.link) {
        যদি (streq(g->name, config.name)) {
            exists = true;
            ভঙ্গ;
        }
    }

    যদি (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ধ্রুৱ xmlChar*)"allowMultipleSelection");
        যদি (multiple && xmlStrEqual(multiple, (ধ্রুৱ xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } অন্যথা {
        config_item_free(&config);
    }

    বাবে (গ্ৰন্থি = group->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "option"))
            parse_option(ctx, g, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
parse_option_list(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *option_list,
                  গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    বাবে (গ্ৰন্থি = option_list->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "group"))
            parse_group(ctx, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
parse_rules_xml(সংরচনা rxkb_context *ctx, xml_গ্ৰন্থি *মূল,
                গণনা rxkb_popularity popularity)
{
    xml_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    বাবে (গ্ৰন্থি = মূল->children; গ্ৰন্থি; গ্ৰন্থি = গ্ৰন্থি->next) {
        যদি (is_গ্ৰন্থি(গ্ৰন্থি, "modelList"))
            parse_model_list(ctx, গ্ৰন্থি, popularity);
        অন্যথা যদি (is_গ্ৰন্থি(গ্ৰন্থি, "layoutList"))
            parse_layout_list(ctx, গ্ৰন্থি, popularity);
        অন্যথা যদি (is_গ্ৰন্থি(গ্ৰন্থি, "optionList"))
            parse_option_list(ctx, গ্ৰন্থি, popularity);
    }
}

স্থির শূন্য
ATTR_PRINTF(2, 0)
xml_error_func(শূন্য *ctx, ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
    স্থির ৱর্ণ buf[PATH_MAX];
    স্থির size_t slen = 0;
    va_list args;
    পূর্ণাঙ্ক rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, বাৰ্তা);
    rc = vsnprintf(&buf[slen], পরিমাণ(buf) - slen, বাৰ্তা, args);
    va_end(args);

    /* This shouldn't really happen */
    যদি (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, পরিমাণ(buf));
        ঘূৰাই;
    }

    slen += (size_t)rc;
    যদি (slen >= পরিমাণ(buf)) {
        /* truncated, let's flush this */
        buf[পরিমাণ(buf) - 1] = '\n';
        slen = পরিমাণ(buf);
    }

    /* We're assuming here that the last character is \n. */
    যদি (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, পরিমাণ(buf));
        slen = 0;
    }
}

#যদি HAVE_XML_CTXT_SET_ERRORHANDLER
স্থির শূন্য
xml_structured_error_func(শূন্য *userData, ধ্রুৱ xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#অন্যথা
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

স্থির bool
validate(সংরচনা rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ধ্রুৱ ৱর্ণ dtdstr[] =
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
        ঘূৰাই false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_প্ৰবেশ_Ptr pinput =
        xml_New_প্ৰবেশ_From_Memory(রিক্ত, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_প্ৰবেশ_BUF_STATIC);
    যদি (!pinput)
        গচ্ছ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, রিক্ত, রিক্ত);
#অন্যথা
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_প্ৰবেশ_Buffer_Ptr buf =
        xml_Parser_প্ৰবেশ_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    যদি (!buf)
        গচ্ছ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(রিক্ত, buf, XML_CHAR_ENCODING_UTF8);
#endif

    যদি (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        গচ্ছ dtd_error;
    }

#যদি HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#অন্যথা
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    যদি (!dtdvalid) {
        success = false;
        গচ্ছ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#যদি HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    ঘূৰাই success;
}

স্থির bool
parse(সংরচনা rxkb_context *ctx, ধ্রুৱ ৱর্ণ *path,
      গণনা rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = রিক্ত;
    xml_গ্ৰন্থি *মূল = রিক্ত;

    যদি (!check_eaccess(path, R_OK))
        ঘূৰাই false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    যদি (!xmlCtxt)
        ঘূৰাই false;

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

    doc = xmlCtxtReadFile(xmlCtxt, path, রিক্ত, 0);
    যদি (!doc)
        গচ্ছ parse_error;

    যদি (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        গচ্ছ validate_error;
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
    xmlSetGenericErrorFunc(রিক্ত, রিক্ত);
#endif
#যদি HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, রিক্ত, রিক্ত);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    ঘূৰাই success;
}
