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
    #جے_انت
#جے_انت

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
    بنية rxkb_object *ماپا;
    uint32_t refcount;
    بنية list link;
};

بنية rxkb_iso639_کوڈ {
    بنية rxkb_object ادھار;
    حرف *کوڈ;
};

بنية rxkb_iso3166_کوڈ {
    بنية rxkb_object ادھار;
    حرف *کوڈ;
};

تعداد context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

بنية rxkb_context {
    بنية rxkb_object ادھار;
    تعداد context_state context_state;

    bool لدو_extra_قاعدے_files;
    bool use_secure_getenv;

    بنية list models;         /* list of struct rxkb_models */
    بنية list layouts;        /* list of struct rxkb_layouts */
    بنية list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية rxkb_context *ctx,
                                     تعداد rxkb_log_level level,
                                     ثابت حرف *fmt, va_list دلیلاں);
    تعداد rxkb_log_level log_level;

    فراغ *userdata;
};

بنية rxkb_model {
    بنية rxkb_object ادھار;

    حرف *name;
    حرف *vendor;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_layout {
    بنية rxkb_object ادھار;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    تعداد rxkb_popularity popularity;

    بنية list iso639s;  /* list of struct rxkb_iso639_code */
    بنية list iso3166s; /* list of struct rxkb_iso3166_code */
};

بنية rxkb_option_گروہ {
    بنية rxkb_object ادھار;

    bool allow_مضرب;
    بنية list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_option {
    بنية rxkb_object ادھار;

    حرف *name;
    حرف *brief;
    حرف *description;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *رستہ,
      تعداد rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساكن فراغ
rxkb_log(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list دلیلاں;

    إذا (ctx->log_level < level)
        أرجع;

    va_شروع(دلیلاں, fmt);
    ctx->log_fn(ctx, level, fmt, دلیلاں);
    va_سرا(دلیلاں);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#تعریف_کرو rxkb_log_with_کوڈ(ctx, level, پیغام_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_سنیہا_ID(پیغام_id, fmt), ##__VA_ARGS__)
#تعریف_کرو log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#تعریف_کرو log_info(ctx, id, ...) \
    rxkb_log_with_کوڈ((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#تعریف_کرو log_warn(ctx, id, ...) \
    rxkb_log_with_کوڈ((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#تعریف_کرو log_err(ctx, id, ...) \
    rxkb_log_with_کوڈ((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#تعریف_کرو log_wsgo(ctx, id, ...) \
    rxkb_log_with_کوڈ((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#تعریف_کرو DECLARE_REF_UNREF_FOR_TYPE(type_) \
بنية type_ * type_##_ref(بنية type_ *object) { \
    rxkb_object_ref(&object->ادھار); \
    أرجع object; \
} \
بنية type_ * type_##_unref(بنية type_ *object) { \
    إذا (!object) أرجع عدم; \
    أكد(object->ادھار.refcount >= 1); \
    إذا (--object->ادھار.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->ادھار.link);\
        حرر(object); \
    } \
    أرجع عدم;\
}

#تعریف_کرو DECLARE_CREATE_FOR_TYPE(type_) \
ساكن مضمن بنية type_ * type_##_create(بنية rxkb_object *ماپا) { \
    بنية type_ *t = صفر_حافظہ_دیو(1, حجم *t); \
    إذا (t) \
        rxkb_object_init(&t->ادھار, ماپا); \
    أرجع t; \
}

#تعریف_کرو DECLARE_TYPED_GETTER_FOR_TYPE(type_, میدان, rtype_) \
rtype_ type_##_get_##field_(بنية type_ *object) { \
    أرجع object->میدان; \
}

#تعریف_کرو DECLARE_GETTER_FOR_TYPE(type_, میدان) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, میدان, ثابت حرف*)

#تعریف_کرو DECLARE_پہلا_اگلا_FOR_TYPE(type_, ماپا_type, ماپا_میدان) \
بنية type_ * type_##_first(بنية ماپا_type *ماپا) { \
    بنية type_ *o = عدم; \
    إذا (!list_خالی_تھاواں(&ماپا->ماپا_میدان)) \
        o = list_پہلا_اندراج(&ماپا->ماپا_میدان, o, ادھار.link); \
    أرجع o; \
} \
بنية type_ * \
type_##_next(بنية type_ *o) \
{ \
    بنية ماپا_type *ماپا; \
    بنية type_ *اگلا; \
    ماپا = container_دا(o->ادھار.ماپا, بنية ماپا_type, ادھار); \
    اگلا = list_پہلا_اندراج(&o->ادھار.link, o, ادھار.link); \
    إذا (list_is_آخری(&ماپا->ماپا_میدان, &o->ادھار.link)) \
        أرجع عدم; \
    أرجع اگلا; \
}

ساكن فراغ
rxkb_object_init(بنية rxkb_object *object, بنية rxkb_object *ماپا)
{
    object->refcount = 1;
    object->ماپا = ماپا;
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
rxkb_iso639_کوڈ_destroy(بنية rxkb_iso639_کوڈ *کوڈ)
{
    حرر(کوڈ->کوڈ);
}

بنية rxkb_iso639_کوڈ *
rxkb_layout_get_iso639_پہلا(بنية rxkb_layout *layout)
{
    بنية rxkb_iso639_کوڈ *کوڈ = عدم;

    إذا (!list_خالی_تھاواں(&layout->iso639s))
        کوڈ = list_پہلا_اندراج(&layout->iso639s, کوڈ, ادھار.link);

    أرجع کوڈ;
}

بنية rxkb_iso639_کوڈ *
rxkb_iso639_کوڈ_اگلا(بنية rxkb_iso639_کوڈ *کوڈ)
{
    بنية rxkb_iso639_کوڈ *اگلا = عدم;
    بنية rxkb_layout *layout;

    layout = container_دا(کوڈ->ادھار.ماپا, بنية rxkb_layout, ادھار);

    إذا (list_is_آخری(&layout->iso639s, &کوڈ->ادھار.link))
        أرجع عدم;

    اگلا = list_پہلا_اندراج(&کوڈ->ادھار.link, کوڈ, ادھار.link);

    أرجع اگلا;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_کوڈ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_کوڈ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_کوڈ, کوڈ);

ساكن فراغ
rxkb_iso3166_کوڈ_destroy(بنية rxkb_iso3166_کوڈ *کوڈ)
{
    حرر(کوڈ->کوڈ);
}

بنية rxkb_iso3166_کوڈ *
rxkb_layout_get_iso3166_پہلا(بنية rxkb_layout *layout)
{
    بنية rxkb_iso3166_کوڈ *کوڈ = عدم;

    إذا (!list_خالی_تھاواں(&layout->iso3166s))
        کوڈ = list_پہلا_اندراج(&layout->iso3166s, کوڈ, ادھار.link);

    أرجع کوڈ;
}

بنية rxkb_iso3166_کوڈ *
rxkb_iso3166_کوڈ_اگلا(بنية rxkb_iso3166_کوڈ *کوڈ)
{
    بنية rxkb_iso3166_کوڈ *اگلا = عدم;
    بنية rxkb_layout *layout;

    layout = container_دا(کوڈ->ادھار.ماپا, بنية rxkb_layout, ادھار);

    إذا (list_is_آخری(&layout->iso3166s, &کوڈ->ادھار.link))
        أرجع عدم;

    اگلا = list_پہلا_اندراج(&کوڈ->ادھار.link, کوڈ, ادھار.link);

    أرجع اگلا;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_کوڈ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_کوڈ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_کوڈ, کوڈ);

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
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_option, rxkb_option_گروہ, options);

ساكن فراغ
rxkb_layout_destroy(بنية rxkb_layout *l)
{
    بنية rxkb_iso639_کوڈ *iso639, *tmp_639;
    بنية rxkb_iso3166_کوڈ *iso3166, *tmp_3166;

    حرر(l->name);
    حرر(l->brief);
    حرر(l->description);
    حرر(l->variant);

    list_for_each_محفوظ(iso639, tmp_639, &l->iso639s, ادھار.link) {
        rxkb_iso639_کوڈ_unref(iso639);
    }
    list_for_each_محفوظ(iso3166, tmp_3166, &l->iso3166s, ادھار.link) {
        rxkb_iso3166_کوڈ_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, تعداد rxkb_popularity);
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

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
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_model, rxkb_context, models);

ساكن فراغ
rxkb_option_گروہ_destroy(بنية rxkb_option_گروہ *og)
{
    بنية rxkb_option *o, *otmp;

    حرر(og->name);
    حرر(og->description);

    list_for_each_محفوظ(o, otmp, &og->options, ادھار.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_گروہ_allows_مضرب(بنية rxkb_option_گروہ *g)
{
    أرجع g->allow_مضرب;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_گروہ);
DECLARE_CREATE_FOR_TYPE(rxkb_option_گروہ);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروہ, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروہ, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_گروہ, popularity, تعداد rxkb_popularity);
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_option_گروہ, rxkb_context, option_groups);

ساكن فراغ
rxkb_context_destroy(بنية rxkb_context *ctx)
{
    بنية rxkb_model *m, *mtmp;
    بنية rxkb_layout *l, *ltmp;
    بنية rxkb_option_گروہ *og, *ogtmp;
    حرف **رستہ;

    list_for_each_محفوظ(m, mtmp, &ctx->models, ادھار.link)
        rxkb_model_unref(m);
    أكد(list_خالی_تھاواں(&ctx->models));

    list_for_each_محفوظ(l, ltmp, &ctx->layouts, ادھار.link)
        rxkb_layout_unref(l);
    أكد(list_خالی_تھاواں(&ctx->layouts));

    list_for_each_محفوظ(og, ogtmp, &ctx->option_groups, ادھار.link)
        rxkb_option_گروہ_unref(og);
    أكد(list_خالی_تھاواں(&ctx->option_groups));

    darray_foreach(رستہ, ctx->includes)
        حرر(*رستہ);
    darray_free(ctx->includes);

    أكد(darray_خالی_تھاواں(ctx->includes));
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
rxkb_context_سیٹ_log_level(بنية rxkb_context *ctx,
                           تعداد rxkb_log_level level)
{
    ctx->log_level = level;
}

ساكن ثابت حرف *
log_level_منزل_اگیتر(تعداد rxkb_log_level level)
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
               ثابت حرف *fmt, va_list دلیلاں)
{
    ثابت حرف *اگیتر = log_level_منزل_اگیتر(level);

    إذا (اگیتر)
        fprintf(stderr, "%s", اگیتر);
    vfprintf(stderr, fmt, دلیلاں);
}

ساكن تعداد rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت لما lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد rxkb_log_level)lvl;

    إذا (istreq_اگیتر("crit", level))
        أرجع RXKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_اگیتر("err", level))
        أرجع RXKB_LOG_LEVEL_ERROR;
    إذا (istreq_اگیتر("warn", level))
        أرجع RXKB_LOG_LEVEL_WARNING;
    إذا (istreq_اگیتر("info", level))
        أرجع RXKB_LOG_LEVEL_INFO;
    إذا (istreq_اگیتر("debug", level) || istreq_اگیتر("dbg", level))
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
    ctx->لدو_extra_قاعدے_files = flags & RXKB_CONTEXT_لدو_EXOTIC_قاعدے;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    إذا (env)
        rxkb_context_سیٹ_log_level(ctx, log_level(env));

    ساكن ثابت تعداد rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_لدو_EXOTIC_قاعدے
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    إذا (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        حرر(ctx);
        أرجع عدم;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    إذا (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_رستہ_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_رستہ,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_جڑ);
        rxkb_context_unref(ctx);
        أرجع عدم;
    }

    أرجع ctx;
}

فراغ
rxkb_context_سیٹ_log_fn(بنية rxkb_context *ctx,
                        فراغ (*log_fn)(بنية rxkb_context *ctx,
                                       تعداد rxkb_log_level level,
                                       ثابت حرف *fmt, va_list دلیلاں))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_رستہ_append(بنية rxkb_context *ctx, ثابت حرف *رستہ)
{
    صحيح err = 0;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "include paths can only be appended to a new context\n");
        اذهب error;
    }

    بنية stat stat_buf;
    err = stat(رستہ, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب error;
    }
    إذا (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        اذهب error;
    }

    إذا (!check_eaccess(رستہ, R_OK | X_OK)) {
        err = EACCES;
        اذهب error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف قاعدے[PATH_MAX];
    إذا (!snprintf_محفوظ(قاعدے, حجم(قاعدے), "%s/rules/%s.xml",
                       رستہ, DEFAULT_XKB_قاعدے)) {
        log_err(ctx, XKB_ERROR_INVALID_رستہ,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(قاعدے), رستہ, DEFAULT_XKB_قاعدے);
        اذهب error;
    }

    حرف * ثابت tmp = strdup(رستہ);
    إذا (!tmp) {
        err = ENOMEM;
        اذهب error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_سنیہا_NO_ID, "Include path added: %s\n", tmp);

    أرجع true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_سنیہا_NO_ID,
             "Include path failed: \"%s\" (%s)\n", رستہ, strerror(err));
    أرجع false;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
جوڑو_سدھا_subdirectories(بنية rxkb_context *ctx, ثابت حرف *رستہ,
                          darray_string *extensions,
                          darray_ناپ_t versioned_گنتی,
                          ناپ_t versioned_رستہ_لمائی)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(رستہ, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(رستہ, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(رستہ);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *اندراج;
    حرف رستہ_buf[PATH_MAX] = "";
    versioned_رستہ_لمائی++; /* Additional final ‘/’ */
    طالما ((اندراج = readdir(dir)) != عدم) {
        ثابت حرف *name = اندراج->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_محفوظ(رستہ_buf, حجم(رستہ_buf), "%s/%s", رستہ, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(رستہ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_منوال))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_ناپ_t i = 0; i < versioned_گنتی; i++) {
            ثابت حرف * ثابت prev_name = darray_شے(*extensions, i)
                                         + versioned_رستہ_لمائی;
            إذا (strcmp(name, prev_name) == 0)
                اذهب اگلا;
        }

        حرف *ext_رستہ = strdup_محفوظ(رستہ_buf);
        إذا (!ext_رستہ) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_رستہ);
اگلا:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_ناپ(*extensions) > versioned_گنتی) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اشیاء(*extensions) + versioned_گنتی,
              darray_ناپ(*extensions) - versioned_گنتی,
              حجم(*darray_اشیاء(*extensions)), &compare_str);
        حرف **ext_رستہ;
        darray_foreach_from(ext_رستہ, *extensions, versioned_گنتی) {
            ret |= rxkb_context_include_رستہ_append(ctx, *ext_رستہ);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", رستہ, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#جے_انت

bool
rxkb_context_include_رستہ_append_default(بنية rxkb_context *ctx)
{
    حرف user_رستہ[PATH_MAX];
    bool ret = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "include paths can only be appended to a new context\n");
        أرجع false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        إذا (snprintf_محفوظ(user_رستہ, حجم(user_رستہ), "%s/xkb", xdg))
            ret |= rxkb_context_include_رستہ_append(ctx, user_رستہ);
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        إذا (snprintf_محفوظ(user_رستہ, حجم(user_رستہ), "%s/.config/xkb", home))
            ret |= rxkb_context_include_رستہ_append(ctx, user_رستہ);
    }

    إذا (home != عدم) {
        إذا (snprintf_محفوظ(user_رستہ, حجم(user_رستہ), "%s/.xkb", home))
            ret |= rxkb_context_include_رستہ_append(ctx, user_رستہ);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_رستہ_append(
        ctx, ((extra != عدم) ? extra : DFLT_XKB_CONFIG_EXTRA_رستہ)
    );

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_رستہ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#جے_تعریف_شدہ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_رستہ
    إذا (extensions_رستہ == عدم)
        extensions_رستہ = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_رستہ;
#جے_انت
    ناپ_t versioned_رستہ_لمائی = 0;
    إذا (extensions_رستہ != عدم) {
        /* Add direct subdirectories */
        ret |= جوڑو_سدھا_subdirectories(ctx, extensions_رستہ,
                                         &extensions, 0, 0);
        versioned_رستہ_لمائی = لڑی_لمبائی(extensions_رستہ);
    }

    /* Unversioned extensions directory */
    extensions_رستہ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#جے_تعریف_شدہ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_رستہ
    إذا (extensions_رستہ == عدم)
        extensions_رستہ = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_رستہ;
#جے_انت
    إذا (extensions_رستہ != عدم) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جوڑو_سدھا_subdirectories(ctx, extensions_رستہ, &extensions,
                                         darray_ناپ(extensions),
                                         versioned_رستہ_لمائی);
    }

    حرف **ext_رستہ;
    darray_foreach(ext_رستہ, extensions) {
        حرر(*ext_رستہ);
    }
    darray_free(extensions);
#جے_انت

    /* Canonical XKB root */
    ثابت حرف * ثابت جڑ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_جڑ = rxkb_context_include_رستہ_append(
        ctx, ((جڑ != عدم) ? جڑ: DFLT_XKB_CONFIG_جڑ)
    );
    ret |= has_جڑ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    إذا (!has_جڑ && (جڑ == عدم || جڑ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_سنیہا_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_جڑ,
                 ((جڑ == عدم) ? DFLT_XKB_CONFIG_جڑ : جڑ));
        ret |= rxkb_context_include_رستہ_append(ctx, DFLT_XKB_LEGACY_جڑ);
    }

    أرجع ret;
}

bool
rxkb_context_parse_default_ruleset(بنية rxkb_context *ctx)
{
    أرجع rxkb_context_parse(ctx, DEFAULT_XKB_قاعدے);
}

bool
rxkb_context_parse(بنية rxkb_context *ctx, ثابت حرف *ruleset)
{
    حرف **رستہ;
    bool success = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "parse must only be called on a new context\n");
        أرجع false;
    }

    darray_foreach_الٹ(رستہ, ctx->includes) {
        حرف قاعدے[PATH_MAX];

        إذا (snprintf_محفوظ(قاعدے, حجم(قاعدے), "%s/rules/%s.xml",
                           *رستہ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قاعدے);
            إذا (parse(ctx, قاعدے, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        إذا (ctx->لدو_extra_قاعدے_files &&
            snprintf_محفوظ(قاعدے, حجم(قاعدے), "%s/rules/%s.extras.xml",
                          *رستہ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قاعدے);
            إذا (parse(ctx, قاعدے, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    أرجع success;
}


فراغ
rxkb_context_سیٹ_user_انکڑے(بنية rxkb_context *ctx, فراغ *userdata)
{
    ctx->userdata = userdata;
}

فراغ *
rxkb_context_get_user_انکڑے(بنية rxkb_context *ctx)
{
    أرجع ctx->userdata;
}

ساكن مضمن bool
is_گرہ(xml_گرہ *گرہ, ثابت حرف *name)
{
    أرجع گرہ->type == XML_ELEMENT_گرہ &&
        xmlStrEqual(گرہ->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساكن حرف *
کڈھو_متن(xml_گرہ *گرہ)
{
    xml_گرہ *n;

    لكل (n = گرہ->children; n; n = n->اگلا) {
        إذا (n->type == XML_متن_گرہ)
            أرجع (حرف *)xmlStrdup(n->content);
    }
    أرجع عدم;
}

/* Data from “configItem” node */
بنية config_شے {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

#تعریف_کرو config_شے_new(popularity_) { \
    .name = عدم, \
    .description = عدم, \
    .brief = عدم, \
    .vendor = عدم, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساكن فراغ
config_شے_free(بنية config_شے *config) {
    حرر(config->name);
    حرر(config->description);
    حرر(config->brief);
    حرر(config->vendor);
}

ساكن bool
parse_config_شے(بنية rxkb_context *ctx, xml_گرہ *ماپا,
                  بنية config_شے *config)
{
    xml_گرہ *گرہ = عدم;
    xml_گرہ *ci = عدم;

    لكل (ci = ماپا->children; ci; ci = ci->اگلا) {
        إذا (is_گرہ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            إذا (raw_popularity) {
                إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وإلا إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وإلا
                    log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->سطر, raw_popularity);
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
            لكل (گرہ = ci->children; گرہ; گرہ = گرہ->اگلا) {
                إذا (is_گرہ(گرہ, "name"))
                    config->name = کڈھو_متن(گرہ);
                وإلا إذا (is_گرہ(گرہ, "description"))
                    config->description = کڈھو_متن(گرہ);
                وإلا إذا (is_گرہ(گرہ, "shortDescription"))
                    config->brief = کڈھو_متن(گرہ);
                وإلا إذا (is_گرہ(گرہ, "vendor"))
                    config->vendor = کڈھو_متن(گرہ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            إذا (!config->name || !لڑی_لمبائی(config->name))  {
                log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->سطر);
                config_شے_free(config);
                أرجع false;
            }

            أرجع true; /* only one configItem allowed in the dtd */
        }
    }

    أرجع false;
}

ساكن فراغ
parse_model(بنية rxkb_context *ctx, xml_گرہ *model,
            تعداد rxkb_popularity popularity)
{
    بنية config_شے config = config_شے_new(popularity);

    إذا (parse_config_شے(ctx, model, &config)) {
        بنية rxkb_model *m;

        list_for_each(m, &ctx->models, ادھار.link) {
            إذا (streq(m->name, config.name)) {
                config_شے_free(&config);
                أرجع;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->ادھار);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->ادھار.link);
    }
}

ساكن فراغ
parse_model_list(بنية rxkb_context *ctx, xml_گرہ *model_list,
                 تعداد rxkb_popularity popularity)
{
    xml_گرہ *گرہ = عدم;

    لكل (گرہ = model_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "model"))
            parse_model(ctx, گرہ, popularity);
    }
}

ساكن فراغ
parse_language_list(xml_گرہ *language_list, بنية rxkb_layout *layout)
{
    xml_گرہ *گرہ = عدم;
    بنية rxkb_iso639_کوڈ *کوڈ;

    لكل (گرہ = language_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "iso639Id")) {
            حرف *str = کڈھو_متن(گرہ);
            بنية rxkb_object *ماپا;

            إذا (!str || لڑی_لمبائی(str) != 3) {
                حرر(str);
                استمر;
            }

            ماپا = &layout->ادھار;
            کوڈ = rxkb_iso639_کوڈ_create(ماپا);
            کوڈ->کوڈ = str;
            list_append(&layout->iso639s, &کوڈ->ادھار.link);
        }
    }
}

ساكن فراغ
parse_country_list(xml_گرہ *country_list, بنية rxkb_layout *layout)
{
    xml_گرہ *گرہ = عدم;
    بنية rxkb_iso3166_کوڈ *کوڈ;

    لكل (گرہ = country_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "iso3166Id")) {
            حرف *str = کڈھو_متن(گرہ);
            بنية rxkb_object *ماپا;

            إذا (!str || لڑی_لمبائی(str) != 2) {
                حرر(str);
                استمر;
            }

            ماپا = &layout->ادھار;
            کوڈ = rxkb_iso3166_کوڈ_create(ماپا);
            کوڈ->کوڈ = str;
            list_append(&layout->iso3166s, &کوڈ->ادھار.link);
        }
    }
}

ساكن فراغ
parse_variant(بنية rxkb_context *ctx, بنية rxkb_layout *l,
              xml_گرہ *variant, تعداد rxkb_popularity popularity)
{
    xml_گرہ *ci;
    بنية config_شے config = config_شے_new(popularity);

    إذا (parse_config_شے(ctx, variant, &config)) {
        بنية rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, ادھار.link) {
            إذا (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                اكسر;
            }
        }

        إذا (!exists) {
            v = rxkb_layout_create(&ctx->ادھار);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == عدم ? strdup_محفوظ(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->ادھار.link);

            لكل (ci = variant->children; ci; ci = ci->اگلا) {
                xml_گرہ *گرہ;

                إذا (!is_گرہ(ci, "configItem"))
                    استمر;

                bool لبھیا_language_list = false;
                bool لبھیا_country_list = false;
                لكل (گرہ = ci->children; گرہ; گرہ = گرہ->اگلا) {
                    إذا (is_گرہ(گرہ, "languageList")) {
                        parse_language_list(گرہ, v);
                        لبھیا_language_list = true;
                    }
                    إذا (is_گرہ(گرہ, "countryList")) {
                        parse_country_list(گرہ, v);
                        لبھیا_country_list = true;
                    }
                }
                إذا (!لبھیا_language_list) {
                    // inherit from parent layout
                    بنية rxkb_iso639_کوڈ* x;
                    list_for_each(x, &l->iso639s, ادھار.link) {
                        بنية rxkb_iso639_کوڈ* کوڈ = rxkb_iso639_کوڈ_create(&v->ادھار);
                        کوڈ->کوڈ = strdup(x->کوڈ);
                        list_append(&v->iso639s, &کوڈ->ادھار.link);
                    }
                }
                إذا (!لبھیا_country_list) {
                    // inherit from parent layout
                    بنية rxkb_iso3166_کوڈ* x;
                    list_for_each(x, &l->iso3166s, ادھار.link) {
                        بنية rxkb_iso3166_کوڈ* کوڈ = rxkb_iso3166_کوڈ_create(&v->ادھار);
                        کوڈ->کوڈ = strdup(x->کوڈ);
                        list_append(&v->iso3166s, &کوڈ->ادھار.link);
                    }
                }
            }
        } وإلا {
            config_شے_free(&config);
        }
    }
}

ساكن فراغ
parse_variant_list(بنية rxkb_context *ctx, بنية rxkb_layout *l,
                   xml_گرہ *variant_list, تعداد rxkb_popularity popularity)
{
    xml_گرہ *گرہ = عدم;

    لكل (گرہ = variant_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "variant"))
            parse_variant(ctx, l, گرہ, popularity);
    }
}

ساكن فراغ
parse_layout(بنية rxkb_context *ctx, xml_گرہ *layout,
             تعداد rxkb_popularity popularity)
{
    بنية config_شے config = config_شے_new(popularity);
    بنية rxkb_layout *l;
    xml_گرہ *گرہ = عدم;
    bool exists = false;

    إذا (!parse_config_شے(ctx, layout, &config))
        أرجع;

    list_for_each(l, &ctx->layouts, ادھار.link) {
        إذا (streq(l->name, config.name) && l->variant == عدم) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        l = rxkb_layout_create(&ctx->ادھار);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = عدم;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->ادھار.link);
    } وإلا {
        config_شے_free(&config);
    }

    لكل (گرہ = layout->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "variantList")) {
            parse_variant_list(ctx, l, گرہ, popularity);
        }
        إذا (!exists && is_گرہ(گرہ, "configItem")) {
            xml_گرہ *ll;
            لكل (ll = گرہ->children; ll; ll = ll->اگلا) {
                إذا (is_گرہ(ll, "languageList"))
                    parse_language_list(ll, l);
                إذا (is_گرہ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساكن فراغ
parse_layout_list(بنية rxkb_context *ctx, xml_گرہ *layout_list,
                  تعداد rxkb_popularity popularity)
{
    xml_گرہ *گرہ = عدم;

    لكل (گرہ = layout_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "layout"))
            parse_layout(ctx, گرہ, popularity);
    }
}

ساكن فراغ
parse_option(بنية rxkb_context *ctx, بنية rxkb_option_گروہ *گروہ,
             xml_گرہ *option, تعداد rxkb_popularity popularity)
{
    بنية config_شے config = config_شے_new(popularity);

    إذا (parse_config_شے(ctx, option, &config)) {
        بنية rxkb_option *o;

        list_for_each(o, &گروہ->options, ادھار.link) {
            إذا (streq(o->name, config.name)) {
                config_شے_free(&config);
                أرجع;
            }
        }

        o = rxkb_option_create(&گروہ->ادھار);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&گروہ->options, &o->ادھار.link);
    }
}

ساكن فراغ
parse_گروہ(بنية rxkb_context *ctx, xml_گرہ *گروہ,
            تعداد rxkb_popularity popularity)
{
    بنية config_شے config = config_شے_new(popularity);
    بنية rxkb_option_گروہ *g;
    xml_گرہ *گرہ = عدم;
    xmlChar *مضرب;
    bool exists = false;

    إذا (!parse_config_شے(ctx, گروہ, &config))
        أرجع;

    list_for_each(g, &ctx->option_groups, ادھار.link) {
        إذا (streq(g->name, config.name)) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        g = rxkb_option_گروہ_create(&ctx->ادھار);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        مضرب = xmlGetProp(گروہ, (ثابت xmlChar*)"allowMultipleSelection");
        إذا (مضرب && xmlStrEqual(مضرب, (ثابت xmlChar*)"true"))
            g->allow_مضرب = true;
        xmlFree(مضرب);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->ادھار.link);
    } وإلا {
        config_شے_free(&config);
    }

    لكل (گرہ = گروہ->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "option"))
            parse_option(ctx, g, گرہ, popularity);
    }
}

ساكن فراغ
parse_option_list(بنية rxkb_context *ctx, xml_گرہ *option_list,
                  تعداد rxkb_popularity popularity)
{
    xml_گرہ *گرہ = عدم;

    لكل (گرہ = option_list->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "group"))
            parse_گروہ(ctx, گرہ, popularity);
    }
}

ساكن فراغ
parse_قاعدے_xml(بنية rxkb_context *ctx, xml_گرہ *جڑ,
                تعداد rxkb_popularity popularity)
{
    xml_گرہ *گرہ = عدم;

    لكل (گرہ = جڑ->children; گرہ; گرہ = گرہ->اگلا) {
        إذا (is_گرہ(گرہ, "modelList"))
            parse_model_list(ctx, گرہ, popularity);
        وإلا إذا (is_گرہ(گرہ, "layoutList"))
            parse_layout_list(ctx, گرہ, popularity);
        وإلا إذا (is_گرہ(گرہ, "optionList"))
            parse_option_list(ctx, گرہ, popularity);
    }
}

ساكن فراغ
ATTR_PRINTF(2, 0)
xml_error_func(فراغ *ctx, ثابت حرف *پیغام, ...)
{
    ساكن حرف buf[PATH_MAX];
    ساكن ناپ_t slen = 0;
    va_list دلیلاں;
    صحيح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_شروع(دلیلاں, پیغام);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, پیغام, دلیلاں);
    va_سرا(دلیلاں);

    /* This shouldn't really happen */
    إذا (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_ناپ,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, حجم(buf));
        أرجع;
    }

    slen += (ناپ_t)rc;
    إذا (slen >= حجم(buf)) {
        /* truncated, let's flush this */
        buf[حجم(buf) - 1] = '\n';
        slen = حجم(buf);
    }

    /* We're assuming here that the last character is \n. */
    إذا (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#إذا HAVE_XML_CTXT_سیٹ_ERRORHANDLER
ساكن فراغ
xml_structured_error_func(فراغ *user_انکڑے_2, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_انکڑے_2);
}
#جے_انت

#جے_تعریف_شدہ XML_PARSE_NO_XXE
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET)
#جے_انت

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
    static_assert(HAVE_XML_CTXT_سیٹ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_سیٹ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_داخلہ_Ptr pinput =
        xml_New_داخلہ_From_حافظہ(عدم, dtdstr, ARRAY_ناپ(dtdstr) - 1,
                              XML_داخلہ_BUF_STATIC);
    إذا (!pinput)
        اذهب dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, عدم, عدم);
#وإلا
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_داخلہ_بفر_Ptr buf =
        xml_Parser_داخلہ_بفر_Create_Mem(dtdstr, ARRAY_ناپ(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    إذا (!buf)
        اذهب dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(عدم, buf, XML_CHAR_ENCODING_UTF8);
#جے_انت

    إذا (!dtd) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID, "Failed to load DTD\n");
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
    dtdvalid->user_انکڑے_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#جے_انت
    xmlFreeDtd(dtd);

dtd_error:
#إذا HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#جے_انت
    أرجع success;
}

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *رستہ,
      تعداد rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = عدم;
    xml_گرہ *جڑ = عدم;

    إذا (!check_eaccess(رستہ, R_OK))
        أرجع false;

    LIBXML_TEST_نسخہ

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;

#جے_تعریف_شدہ XML_PARSE_NO_XXE
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET)
#جے_انت
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#إذا HAVE_XML_CTXT_سیٹ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#جے_انت
#إذا !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_سیٹ_Generic_Error_Func(ctx, xml_error_func);
#جے_انت

    doc = xmlCtxtReadFile(xmlCtxt, رستہ, عدم, 0);
    إذا (!doc)
        اذهب parse_error;

    إذا (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "XML error: failed to validate document at %s\n", رستہ);
        اذهب validate_error;
    }

    جڑ = xml_Doc_Get_جڑ_Element(doc);
    parse_قاعدے_xml(ctx, جڑ, popularity);

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
    xml_سیٹ_Generic_Error_Func(عدم, عدم);
#جے_انت
#إذا HAVE_XML_CTXT_سیٹ_ERRORHANDLER
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, عدم, عدم);
#جے_انت
    xmlFreeParserCtxt(xmlCtxt);

    أرجع success;
}
