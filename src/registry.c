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
    #د_که_پای
#د_که_پای

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
    بنية rxkb_object *پلار;
    uint32_t refcount;
    بنية list link;
};

بنية rxkb_iso639_کوډ {
    بنية rxkb_object بنسټ;
    حرف *کوډ;
};

بنية rxkb_iso3166_کوډ {
    بنية rxkb_object بنسټ;
    حرف *کوډ;
};

تعداد context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

بنية rxkb_context {
    بنية rxkb_object بنسټ;
    تعداد context_state context_state;

    bool بارول_extra_قواعد_files;
    bool use_secure_getenv;

    بنية list models;         /* list of struct rxkb_models */
    بنية list layouts;        /* list of struct rxkb_layouts */
    بنية list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية rxkb_context *ctx,
                                     تعداد rxkb_log_level level,
                                     ثابت حرف *fmt, va_list دلیلونه);
    تعداد rxkb_log_level log_level;

    فراغ *userdata;
};

بنية rxkb_model {
    بنية rxkb_object بنسټ;

    حرف *name;
    حرف *vendor;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_layout {
    بنية rxkb_object بنسټ;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    تعداد rxkb_popularity popularity;

    بنية list iso639s;  /* list of struct rxkb_iso639_code */
    بنية list iso3166s; /* list of struct rxkb_iso3166_code */
};

بنية rxkb_option_ډله {
    بنية rxkb_object بنسټ;

    bool allow_مضرب;
    بنية list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_option {
    بنية rxkb_object بنسټ;

    حرف *name;
    حرف *brief;
    حرف *description;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *لار,
      تعداد rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساكن فراغ
rxkb_log(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list دلیلونه;

    إذا (ctx->log_level < level)
        أرجع;

    va_پیل(دلیلونه, fmt);
    ctx->log_fn(ctx, level, fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#تعریف_کړه rxkb_log_with_کوډ(ctx, level, پیغام_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_پیغام_ID(پیغام_id, fmt), ##__VA_ARGS__)
#تعریف_کړه log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#تعریف_کړه log_info(ctx, id, ...) \
    rxkb_log_with_کوډ((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#تعریف_کړه log_warn(ctx, id, ...) \
    rxkb_log_with_کوډ((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#تعریف_کړه log_err(ctx, id, ...) \
    rxkb_log_with_کوډ((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#تعریف_کړه log_wsgo(ctx, id, ...) \
    rxkb_log_with_کوډ((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#تعریف_کړه DECLARE_REF_UNREF_FOR_TYPE(type_) \
بنية type_ * type_##_ref(بنية type_ *object) { \
    rxkb_object_ref(&object->بنسټ); \
    أرجع object; \
} \
بنية type_ * type_##_unref(بنية type_ *object) { \
    إذا (!object) أرجع عدم; \
    أكد(object->بنسټ.refcount >= 1); \
    إذا (--object->بنسټ.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->بنسټ.link);\
        حرر(object); \
    } \
    أرجع عدم;\
}

#تعریف_کړه DECLARE_CREATE_FOR_TYPE(type_) \
ساكن مضمن بنية type_ * type_##_create(بنية rxkb_object *پلار) { \
    بنية type_ *t = حافظه_صفر_ورکړه(1, حجم *t); \
    إذا (t) \
        rxkb_object_init(&t->بنسټ, پلار); \
    أرجع t; \
}

#تعریف_کړه DECLARE_TYPED_GETTER_FOR_TYPE(type_, ډګر, rtype_) \
rtype_ type_##_get_##field_(بنية type_ *object) { \
    أرجع object->ډګر; \
}

#تعریف_کړه DECLARE_GETTER_FOR_TYPE(type_, ډګر) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, ډګر, ثابت حرف*)

#تعریف_کړه DECLARE_لومړی_بل_FOR_TYPE(type_, پلار_type, پلار_ډګر) \
بنية type_ * type_##_first(بنية پلار_type *پلار) { \
    بنية type_ *o = عدم; \
    إذا (!list_تشه(&پلار->پلار_ډګر)) \
        o = list_لومړی_ثبت(&پلار->پلار_ډګر, o, بنسټ.link); \
    أرجع o; \
} \
بنية type_ * \
type_##_next(بنية type_ *o) \
{ \
    بنية پلار_type *پلار; \
    بنية type_ *بل; \
    پلار = container_د(o->بنسټ.پلار, بنية پلار_type, بنسټ); \
    بل = list_لومړی_ثبت(&o->بنسټ.link, o, بنسټ.link); \
    إذا (list_is_وروستی(&پلار->پلار_ډګر, &o->بنسټ.link)) \
        أرجع عدم; \
    أرجع بل; \
}

ساكن فراغ
rxkb_object_init(بنية rxkb_object *object, بنية rxkb_object *پلار)
{
    object->refcount = 1;
    object->پلار = پلار;
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
rxkb_iso639_کوډ_destroy(بنية rxkb_iso639_کوډ *کوډ)
{
    حرر(کوډ->کوډ);
}

بنية rxkb_iso639_کوډ *
rxkb_layout_get_iso639_لومړی(بنية rxkb_layout *layout)
{
    بنية rxkb_iso639_کوډ *کوډ = عدم;

    إذا (!list_تشه(&layout->iso639s))
        کوډ = list_لومړی_ثبت(&layout->iso639s, کوډ, بنسټ.link);

    أرجع کوډ;
}

بنية rxkb_iso639_کوډ *
rxkb_iso639_کوډ_بل(بنية rxkb_iso639_کوډ *کوډ)
{
    بنية rxkb_iso639_کوډ *بل = عدم;
    بنية rxkb_layout *layout;

    layout = container_د(کوډ->بنسټ.پلار, بنية rxkb_layout, بنسټ);

    إذا (list_is_وروستی(&layout->iso639s, &کوډ->بنسټ.link))
        أرجع عدم;

    بل = list_لومړی_ثبت(&کوډ->بنسټ.link, کوډ, بنسټ.link);

    أرجع بل;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_کوډ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_کوډ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_کوډ, کوډ);

ساكن فراغ
rxkb_iso3166_کوډ_destroy(بنية rxkb_iso3166_کوډ *کوډ)
{
    حرر(کوډ->کوډ);
}

بنية rxkb_iso3166_کوډ *
rxkb_layout_get_iso3166_لومړی(بنية rxkb_layout *layout)
{
    بنية rxkb_iso3166_کوډ *کوډ = عدم;

    إذا (!list_تشه(&layout->iso3166s))
        کوډ = list_لومړی_ثبت(&layout->iso3166s, کوډ, بنسټ.link);

    أرجع کوډ;
}

بنية rxkb_iso3166_کوډ *
rxkb_iso3166_کوډ_بل(بنية rxkb_iso3166_کوډ *کوډ)
{
    بنية rxkb_iso3166_کوډ *بل = عدم;
    بنية rxkb_layout *layout;

    layout = container_د(کوډ->بنسټ.پلار, بنية rxkb_layout, بنسټ);

    إذا (list_is_وروستی(&layout->iso3166s, &کوډ->بنسټ.link))
        أرجع عدم;

    بل = list_لومړی_ثبت(&کوډ->بنسټ.link, کوډ, بنسټ.link);

    أرجع بل;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_کوډ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_کوډ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_کوډ, کوډ);

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
DECLARE_لومړی_بل_FOR_TYPE(rxkb_option, rxkb_option_ډله, options);

ساكن فراغ
rxkb_layout_destroy(بنية rxkb_layout *l)
{
    بنية rxkb_iso639_کوډ *iso639, *tmp_639;
    بنية rxkb_iso3166_کوډ *iso3166, *tmp_3166;

    حرر(l->name);
    حرر(l->brief);
    حرر(l->description);
    حرر(l->variant);

    list_for_each_خوندي(iso639, tmp_639, &l->iso639s, بنسټ.link) {
        rxkb_iso639_کوډ_unref(iso639);
    }
    list_for_each_خوندي(iso3166, tmp_3166, &l->iso3166s, بنسټ.link) {
        rxkb_iso3166_کوډ_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, تعداد rxkb_popularity);
DECLARE_لومړی_بل_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

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
DECLARE_لومړی_بل_FOR_TYPE(rxkb_model, rxkb_context, models);

ساكن فراغ
rxkb_option_ډله_destroy(بنية rxkb_option_ډله *og)
{
    بنية rxkb_option *o, *otmp;

    حرر(og->name);
    حرر(og->description);

    list_for_each_خوندي(o, otmp, &og->options, بنسټ.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_ډله_allows_مضرب(بنية rxkb_option_ډله *g)
{
    أرجع g->allow_مضرب;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_ډله);
DECLARE_CREATE_FOR_TYPE(rxkb_option_ډله);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ډله, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ډله, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_ډله, popularity, تعداد rxkb_popularity);
DECLARE_لومړی_بل_FOR_TYPE(rxkb_option_ډله, rxkb_context, option_groups);

ساكن فراغ
rxkb_context_destroy(بنية rxkb_context *ctx)
{
    بنية rxkb_model *m, *mtmp;
    بنية rxkb_layout *l, *ltmp;
    بنية rxkb_option_ډله *og, *ogtmp;
    حرف **لار;

    list_for_each_خوندي(m, mtmp, &ctx->models, بنسټ.link)
        rxkb_model_unref(m);
    أكد(list_تشه(&ctx->models));

    list_for_each_خوندي(l, ltmp, &ctx->layouts, بنسټ.link)
        rxkb_layout_unref(l);
    أكد(list_تشه(&ctx->layouts));

    list_for_each_خوندي(og, ogtmp, &ctx->option_groups, بنسټ.link)
        rxkb_option_ډله_unref(og);
    أكد(list_تشه(&ctx->option_groups));

    darray_foreach(لار, ctx->includes)
        حرر(*لار);
    darray_free(ctx->includes);

    أكد(darray_تشه(ctx->includes));
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
rxkb_context_سېټ_log_level(بنية rxkb_context *ctx,
                           تعداد rxkb_log_level level)
{
    ctx->log_level = level;
}

ساكن ثابت حرف *
log_level_منزل_مختاړی(تعداد rxkb_log_level level)
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
               ثابت حرف *fmt, va_list دلیلونه)
{
    ثابت حرف *مختاړی = log_level_منزل_مختاړی(level);

    إذا (مختاړی)
        fprintf(stderr, "%s", مختاړی);
    vfprintf(stderr, fmt, دلیلونه);
}

ساكن تعداد rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت اوږد lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد rxkb_log_level)lvl;

    إذا (istreq_مختاړی("crit", level))
        أرجع RXKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_مختاړی("err", level))
        أرجع RXKB_LOG_LEVEL_ERROR;
    إذا (istreq_مختاړی("warn", level))
        أرجع RXKB_LOG_LEVEL_WARNING;
    إذا (istreq_مختاړی("info", level))
        أرجع RXKB_LOG_LEVEL_INFO;
    إذا (istreq_مختاړی("debug", level) || istreq_مختاړی("dbg", level))
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
    ctx->بارول_extra_قواعد_files = flags & RXKB_CONTEXT_بارول_EXOTIC_قواعد;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    إذا (env)
        rxkb_context_سېټ_log_level(ctx, log_level(env));

    ساكن ثابت تعداد rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_بارول_EXOTIC_قواعد
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    إذا (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        حرر(ctx);
        أرجع عدم;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    إذا (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_لار_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_لار,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ریښه);
        rxkb_context_unref(ctx);
        أرجع عدم;
    }

    أرجع ctx;
}

فراغ
rxkb_context_سېټ_log_fn(بنية rxkb_context *ctx,
                        فراغ (*log_fn)(بنية rxkb_context *ctx,
                                       تعداد rxkb_log_level level,
                                       ثابت حرف *fmt, va_list دلیلونه))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_لار_append(بنية rxkb_context *ctx, ثابت حرف *لار)
{
    صحيح err = 0;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "include paths can only be appended to a new context\n");
        اذهب error;
    }

    بنية stat stat_buf;
    err = stat(لار, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب error;
    }
    إذا (!S_ISDIR(stat_buf.st_ډېرپېښ)) {
        err = ENOTDIR;
        اذهب error;
    }

    إذا (!check_eaccess(لار, R_OK | X_OK)) {
        err = EACCES;
        اذهب error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف قواعد[PATH_MAX];
    إذا (!snprintf_خوندي(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                       لار, DEFAULT_XKB_قواعد)) {
        log_err(ctx, XKB_ERROR_INVALID_لار,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(قواعد), لار, DEFAULT_XKB_قواعد);
        اذهب error;
    }

    حرف * ثابت tmp = strdup(لار);
    إذا (!tmp) {
        err = ENOMEM;
        اذهب error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پیغام_NO_ID, "Include path added: %s\n", tmp);

    أرجع true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پیغام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", لار, strerror(err));
    أرجع false;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
جمع_کړه_مستقیم_subdirectories(بنية rxkb_context *ctx, ثابت حرف *لار,
                          darray_string *extensions,
                          darray_اندازه_t versioned_شمېر,
                          اندازه_t versioned_لار_اوږدتیا)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(لار, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_ډېرپېښ)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(لار, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(لار);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *ثبت;
    حرف لار_buf[PATH_MAX] = "";
    versioned_لار_اوږدتیا++; /* Additional final ‘/’ */
    طالما ((ثبت = readdir(dir)) != عدم) {
        ثابت حرف *name = ثبت->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_خوندي(لار_buf, حجم(لار_buf), "%s/%s", لار, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(لار_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_ډېرپېښ))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_اندازه_t i = 0; i < versioned_شمېر; i++) {
            ثابت حرف * ثابت prev_name = darray_توکی(*extensions, i)
                                         + versioned_لار_اوږدتیا;
            إذا (strcmp(name, prev_name) == 0)
                اذهب بل;
        }

        حرف *ext_لار = strdup_خوندي(لار_buf);
        إذا (!ext_لار) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_لار);
بل:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_اندازه(*extensions) > versioned_شمېر) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_توکي(*extensions) + versioned_شمېر,
              darray_اندازه(*extensions) - versioned_شمېر,
              حجم(*darray_توکي(*extensions)), &compare_str);
        حرف **ext_لار;
        darray_foreach_from(ext_لار, *extensions, versioned_شمېر) {
            ret |= rxkb_context_include_لار_append(ctx, *ext_لار);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", لار, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#د_که_پای

bool
rxkb_context_include_لار_append_default(بنية rxkb_context *ctx)
{
    حرف user_لار[PATH_MAX];
    bool ret = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "include paths can only be appended to a new context\n");
        أرجع false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        إذا (snprintf_خوندي(user_لار, حجم(user_لار), "%s/xkb", xdg))
            ret |= rxkb_context_include_لار_append(ctx, user_لار);
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        إذا (snprintf_خوندي(user_لار, حجم(user_لار), "%s/.config/xkb", home))
            ret |= rxkb_context_include_لار_append(ctx, user_لار);
    }

    إذا (home != عدم) {
        إذا (snprintf_خوندي(user_لار, حجم(user_لار), "%s/.xkb", home))
            ret |= rxkb_context_include_لار_append(ctx, user_لار);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_لار_append(
        ctx, ((extra != عدم) ? extra : DFLT_XKB_CONFIG_EXTRA_لار)
    );

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_لار =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#که_تعریف_شوی DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_لار
    إذا (extensions_لار == عدم)
        extensions_لار = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_لار;
#د_که_پای
    اندازه_t versioned_لار_اوږدتیا = 0;
    إذا (extensions_لار != عدم) {
        /* Add direct subdirectories */
        ret |= جمع_کړه_مستقیم_subdirectories(ctx, extensions_لار,
                                         &extensions, 0, 0);
        versioned_لار_اوږدتیا = مزي_اوږدوالی(extensions_لار);
    }

    /* Unversioned extensions directory */
    extensions_لار =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#که_تعریف_شوی DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_لار
    إذا (extensions_لار == عدم)
        extensions_لار = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_لار;
#د_که_پای
    إذا (extensions_لار != عدم) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جمع_کړه_مستقیم_subdirectories(ctx, extensions_لار, &extensions,
                                         darray_اندازه(extensions),
                                         versioned_لار_اوږدتیا);
    }

    حرف **ext_لار;
    darray_foreach(ext_لار, extensions) {
        حرر(*ext_لار);
    }
    darray_free(extensions);
#د_که_پای

    /* Canonical XKB root */
    ثابت حرف * ثابت ریښه = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_ریښه = rxkb_context_include_لار_append(
        ctx, ((ریښه != عدم) ? ریښه: DFLT_XKB_CONFIG_ریښه)
    );
    ret |= has_ریښه;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    إذا (!has_ریښه && (ریښه == عدم || ریښه[0] != '\0')) {
        log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ریښه,
                 ((ریښه == عدم) ? DFLT_XKB_CONFIG_ریښه : ریښه));
        ret |= rxkb_context_include_لار_append(ctx, DFLT_XKB_LEGACY_ریښه);
    }

    أرجع ret;
}

bool
rxkb_context_parse_default_ruleset(بنية rxkb_context *ctx)
{
    أرجع rxkb_context_parse(ctx, DEFAULT_XKB_قواعد);
}

bool
rxkb_context_parse(بنية rxkb_context *ctx, ثابت حرف *ruleset)
{
    حرف **لار;
    bool success = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "parse must only be called on a new context\n");
        أرجع false;
    }

    darray_foreach_سرچپه(لار, ctx->includes) {
        حرف قواعد[PATH_MAX];

        إذا (snprintf_خوندي(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                           *لار, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            إذا (parse(ctx, قواعد, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        إذا (ctx->بارول_extra_قواعد_files &&
            snprintf_خوندي(قواعد, حجم(قواعد), "%s/rules/%s.extras.xml",
                          *لار, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            إذا (parse(ctx, قواعد, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    أرجع success;
}


فراغ
rxkb_context_سېټ_user_معلومات(بنية rxkb_context *ctx, فراغ *userdata)
{
    ctx->userdata = userdata;
}

فراغ *
rxkb_context_get_user_معلومات(بنية rxkb_context *ctx)
{
    أرجع ctx->userdata;
}

ساكن مضمن bool
is_غوټه(xml_غوټه *غوټه, ثابت حرف *name)
{
    أرجع غوټه->type == XML_ELEMENT_غوټه &&
        xmlStrEqual(غوټه->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساكن حرف *
ایستل_متن(xml_غوټه *غوټه)
{
    xml_غوټه *n;

    لكل (n = غوټه->children; n; n = n->بل) {
        إذا (n->type == XML_متن_غوټه)
            أرجع (حرف *)xmlStrdup(n->content);
    }
    أرجع عدم;
}

/* Data from “configItem” node */
بنية config_توکی {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

#تعریف_کړه config_توکی_new(popularity_) { \
    .name = عدم, \
    .description = عدم, \
    .brief = عدم, \
    .vendor = عدم, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساكن فراغ
config_توکی_free(بنية config_توکی *config) {
    حرر(config->name);
    حرر(config->description);
    حرر(config->brief);
    حرر(config->vendor);
}

ساكن bool
parse_config_توکی(بنية rxkb_context *ctx, xml_غوټه *پلار,
                  بنية config_توکی *config)
{
    xml_غوټه *غوټه = عدم;
    xml_غوټه *ci = عدم;

    لكل (ci = پلار->children; ci; ci = ci->بل) {
        إذا (is_غوټه(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            إذا (raw_popularity) {
                إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وإلا إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وإلا
                    log_err(ctx, XKB_LOG_پیغام_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->کرښه, raw_popularity);
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
            لكل (غوټه = ci->children; غوټه; غوټه = غوټه->بل) {
                إذا (is_غوټه(غوټه, "name"))
                    config->name = ایستل_متن(غوټه);
                وإلا إذا (is_غوټه(غوټه, "description"))
                    config->description = ایستل_متن(غوټه);
                وإلا إذا (is_غوټه(غوټه, "shortDescription"))
                    config->brief = ایستل_متن(غوټه);
                وإلا إذا (is_غوټه(غوټه, "vendor"))
                    config->vendor = ایستل_متن(غوټه);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            إذا (!config->name || !مزي_اوږدوالی(config->name))  {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->کرښه);
                config_توکی_free(config);
                أرجع false;
            }

            أرجع true; /* only one configItem allowed in the dtd */
        }
    }

    أرجع false;
}

ساكن فراغ
parse_model(بنية rxkb_context *ctx, xml_غوټه *model,
            تعداد rxkb_popularity popularity)
{
    بنية config_توکی config = config_توکی_new(popularity);

    إذا (parse_config_توکی(ctx, model, &config)) {
        بنية rxkb_model *m;

        list_for_each(m, &ctx->models, بنسټ.link) {
            إذا (streq(m->name, config.name)) {
                config_توکی_free(&config);
                أرجع;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->بنسټ);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->بنسټ.link);
    }
}

ساكن فراغ
parse_model_list(بنية rxkb_context *ctx, xml_غوټه *model_list,
                 تعداد rxkb_popularity popularity)
{
    xml_غوټه *غوټه = عدم;

    لكل (غوټه = model_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "model"))
            parse_model(ctx, غوټه, popularity);
    }
}

ساكن فراغ
parse_language_list(xml_غوټه *language_list, بنية rxkb_layout *layout)
{
    xml_غوټه *غوټه = عدم;
    بنية rxkb_iso639_کوډ *کوډ;

    لكل (غوټه = language_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "iso639Id")) {
            حرف *str = ایستل_متن(غوټه);
            بنية rxkb_object *پلار;

            إذا (!str || مزي_اوږدوالی(str) != 3) {
                حرر(str);
                استمر;
            }

            پلار = &layout->بنسټ;
            کوډ = rxkb_iso639_کوډ_create(پلار);
            کوډ->کوډ = str;
            list_append(&layout->iso639s, &کوډ->بنسټ.link);
        }
    }
}

ساكن فراغ
parse_country_list(xml_غوټه *country_list, بنية rxkb_layout *layout)
{
    xml_غوټه *غوټه = عدم;
    بنية rxkb_iso3166_کوډ *کوډ;

    لكل (غوټه = country_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "iso3166Id")) {
            حرف *str = ایستل_متن(غوټه);
            بنية rxkb_object *پلار;

            إذا (!str || مزي_اوږدوالی(str) != 2) {
                حرر(str);
                استمر;
            }

            پلار = &layout->بنسټ;
            کوډ = rxkb_iso3166_کوډ_create(پلار);
            کوډ->کوډ = str;
            list_append(&layout->iso3166s, &کوډ->بنسټ.link);
        }
    }
}

ساكن فراغ
parse_variant(بنية rxkb_context *ctx, بنية rxkb_layout *l,
              xml_غوټه *variant, تعداد rxkb_popularity popularity)
{
    xml_غوټه *ci;
    بنية config_توکی config = config_توکی_new(popularity);

    إذا (parse_config_توکی(ctx, variant, &config)) {
        بنية rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, بنسټ.link) {
            إذا (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                اكسر;
            }
        }

        إذا (!exists) {
            v = rxkb_layout_create(&ctx->بنسټ);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == عدم ? strdup_خوندي(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->بنسټ.link);

            لكل (ci = variant->children; ci; ci = ci->بل) {
                xml_غوټه *غوټه;

                إذا (!is_غوټه(ci, "configItem"))
                    استمر;

                bool وموندل_language_list = false;
                bool وموندل_country_list = false;
                لكل (غوټه = ci->children; غوټه; غوټه = غوټه->بل) {
                    إذا (is_غوټه(غوټه, "languageList")) {
                        parse_language_list(غوټه, v);
                        وموندل_language_list = true;
                    }
                    إذا (is_غوټه(غوټه, "countryList")) {
                        parse_country_list(غوټه, v);
                        وموندل_country_list = true;
                    }
                }
                إذا (!وموندل_language_list) {
                    // inherit from parent layout
                    بنية rxkb_iso639_کوډ* x;
                    list_for_each(x, &l->iso639s, بنسټ.link) {
                        بنية rxkb_iso639_کوډ* کوډ = rxkb_iso639_کوډ_create(&v->بنسټ);
                        کوډ->کوډ = strdup(x->کوډ);
                        list_append(&v->iso639s, &کوډ->بنسټ.link);
                    }
                }
                إذا (!وموندل_country_list) {
                    // inherit from parent layout
                    بنية rxkb_iso3166_کوډ* x;
                    list_for_each(x, &l->iso3166s, بنسټ.link) {
                        بنية rxkb_iso3166_کوډ* کوډ = rxkb_iso3166_کوډ_create(&v->بنسټ);
                        کوډ->کوډ = strdup(x->کوډ);
                        list_append(&v->iso3166s, &کوډ->بنسټ.link);
                    }
                }
            }
        } وإلا {
            config_توکی_free(&config);
        }
    }
}

ساكن فراغ
parse_variant_list(بنية rxkb_context *ctx, بنية rxkb_layout *l,
                   xml_غوټه *variant_list, تعداد rxkb_popularity popularity)
{
    xml_غوټه *غوټه = عدم;

    لكل (غوټه = variant_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "variant"))
            parse_variant(ctx, l, غوټه, popularity);
    }
}

ساكن فراغ
parse_layout(بنية rxkb_context *ctx, xml_غوټه *layout,
             تعداد rxkb_popularity popularity)
{
    بنية config_توکی config = config_توکی_new(popularity);
    بنية rxkb_layout *l;
    xml_غوټه *غوټه = عدم;
    bool exists = false;

    إذا (!parse_config_توکی(ctx, layout, &config))
        أرجع;

    list_for_each(l, &ctx->layouts, بنسټ.link) {
        إذا (streq(l->name, config.name) && l->variant == عدم) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        l = rxkb_layout_create(&ctx->بنسټ);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = عدم;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->بنسټ.link);
    } وإلا {
        config_توکی_free(&config);
    }

    لكل (غوټه = layout->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "variantList")) {
            parse_variant_list(ctx, l, غوټه, popularity);
        }
        إذا (!exists && is_غوټه(غوټه, "configItem")) {
            xml_غوټه *ll;
            لكل (ll = غوټه->children; ll; ll = ll->بل) {
                إذا (is_غوټه(ll, "languageList"))
                    parse_language_list(ll, l);
                إذا (is_غوټه(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساكن فراغ
parse_layout_list(بنية rxkb_context *ctx, xml_غوټه *layout_list,
                  تعداد rxkb_popularity popularity)
{
    xml_غوټه *غوټه = عدم;

    لكل (غوټه = layout_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "layout"))
            parse_layout(ctx, غوټه, popularity);
    }
}

ساكن فراغ
parse_option(بنية rxkb_context *ctx, بنية rxkb_option_ډله *ډله,
             xml_غوټه *option, تعداد rxkb_popularity popularity)
{
    بنية config_توکی config = config_توکی_new(popularity);

    إذا (parse_config_توکی(ctx, option, &config)) {
        بنية rxkb_option *o;

        list_for_each(o, &ډله->options, بنسټ.link) {
            إذا (streq(o->name, config.name)) {
                config_توکی_free(&config);
                أرجع;
            }
        }

        o = rxkb_option_create(&ډله->بنسټ);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&ډله->options, &o->بنسټ.link);
    }
}

ساكن فراغ
parse_ډله(بنية rxkb_context *ctx, xml_غوټه *ډله,
            تعداد rxkb_popularity popularity)
{
    بنية config_توکی config = config_توکی_new(popularity);
    بنية rxkb_option_ډله *g;
    xml_غوټه *غوټه = عدم;
    xmlChar *مضرب;
    bool exists = false;

    إذا (!parse_config_توکی(ctx, ډله, &config))
        أرجع;

    list_for_each(g, &ctx->option_groups, بنسټ.link) {
        إذا (streq(g->name, config.name)) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        g = rxkb_option_ډله_create(&ctx->بنسټ);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        مضرب = xmlGetProp(ډله, (ثابت xmlChar*)"allowMultipleSelection");
        إذا (مضرب && xmlStrEqual(مضرب, (ثابت xmlChar*)"true"))
            g->allow_مضرب = true;
        xmlFree(مضرب);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->بنسټ.link);
    } وإلا {
        config_توکی_free(&config);
    }

    لكل (غوټه = ډله->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "option"))
            parse_option(ctx, g, غوټه, popularity);
    }
}

ساكن فراغ
parse_option_list(بنية rxkb_context *ctx, xml_غوټه *option_list,
                  تعداد rxkb_popularity popularity)
{
    xml_غوټه *غوټه = عدم;

    لكل (غوټه = option_list->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "group"))
            parse_ډله(ctx, غوټه, popularity);
    }
}

ساكن فراغ
parse_قواعد_xml(بنية rxkb_context *ctx, xml_غوټه *ریښه,
                تعداد rxkb_popularity popularity)
{
    xml_غوټه *غوټه = عدم;

    لكل (غوټه = ریښه->children; غوټه; غوټه = غوټه->بل) {
        إذا (is_غوټه(غوټه, "modelList"))
            parse_model_list(ctx, غوټه, popularity);
        وإلا إذا (is_غوټه(غوټه, "layoutList"))
            parse_layout_list(ctx, غوټه, popularity);
        وإلا إذا (is_غوټه(غوټه, "optionList"))
            parse_option_list(ctx, غوټه, popularity);
    }
}

ساكن فراغ
ATTR_PRINTF(2, 0)
xml_error_func(فراغ *ctx, ثابت حرف *پیغام, ...)
{
    ساكن حرف buf[PATH_MAX];
    ساكن اندازه_t slen = 0;
    va_list دلیلونه;
    صحيح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_پیل(دلیلونه, پیغام);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, پیغام, دلیلونه);
    va_پای_ټکی(دلیلونه);

    /* This shouldn't really happen */
    إذا (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_اندازه,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, حجم(buf));
        أرجع;
    }

    slen += (اندازه_t)rc;
    إذا (slen >= حجم(buf)) {
        /* truncated, let's flush this */
        buf[حجم(buf) - 1] = '\n';
        slen = حجم(buf);
    }

    /* We're assuming here that the last character is \n. */
    إذا (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_پیغام_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#إذا HAVE_XML_CTXT_سېټ_ERRORHANDLER
ساكن فراغ
xml_structured_error_func(فراغ *user_معلومات_2, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_معلومات_2);
}
#د_که_پای

#که_تعریف_شوی XML_PARSE_NO_XXE
#تعریف_کړه _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کړه _XML_OPTIONS (XML_PARSE_NONET)
#د_که_پای

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
    static_assert(HAVE_XML_CTXT_سېټ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_سېټ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_سېټ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ننوت_Ptr pinput =
        xml_New_ننوت_From_حافظه(عدم, dtdstr, ARRAY_اندازه(dtdstr) - 1,
                              XML_ننوت_BUF_STATIC);
    إذا (!pinput)
        اذهب dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, عدم, عدم);
#وإلا
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ننوت_بفر_Ptr buf =
        xml_Parser_ننوت_بفر_Create_Mem(dtdstr, ARRAY_اندازه(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    إذا (!buf)
        اذهب dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(عدم, buf, XML_CHAR_ENCODING_UTF8);
#د_که_پای

    إذا (!dtd) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID, "Failed to load DTD\n");
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
    dtdvalid->user_معلومات_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#د_که_پای
    xmlFreeDtd(dtd);

dtd_error:
#إذا HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#د_که_پای
    أرجع success;
}

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *لار,
      تعداد rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = عدم;
    xml_غوټه *ریښه = عدم;

    إذا (!check_eaccess(لار, R_OK))
        أرجع false;

    LIBXML_TEST_نسخه

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;

#که_تعریف_شوی XML_PARSE_NO_XXE
#تعریف_کړه _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کړه _XML_OPTIONS (XML_PARSE_NONET)
#د_که_پای
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#إذا HAVE_XML_CTXT_سېټ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_سېټ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#د_که_پای
#إذا !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_سېټ_Generic_Error_Func(ctx, xml_error_func);
#د_که_پای

    doc = xmlCtxtReadFile(xmlCtxt, لار, عدم, 0);
    إذا (!doc)
        اذهب parse_error;

    إذا (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "XML error: failed to validate document at %s\n", لار);
        اذهب validate_error;
    }

    ریښه = xml_Doc_Get_ریښه_Element(doc);
    parse_قواعد_xml(ctx, ریښه, popularity);

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
    xml_سېټ_Generic_Error_Func(عدم, عدم);
#د_که_پای
#إذا HAVE_XML_CTXT_سېټ_ERRORHANDLER
    xml_Ctxt_سېټ_Error_Handler(xmlCtxt, عدم, عدم);
#د_که_پای
    xmlFreeParserCtxt(xmlCtxt);

    أرجع success;
}
