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
    #اگر_ختم
#اگر_ختم

#شامل "xkbcommon/xkbregistry.h"
#شامل "messages-codes.h"
#شامل "darray.h"
#شامل "utils.h"
#شامل "util-list.h"
#شامل "util-mem.h"

ساخت rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ساخت rxkb_object {
    ساخت rxkb_object *والد;
    uint32_t refcount;
    ساخت list link;
};

ساخت rxkb_iso639_رمز {
    ساخت rxkb_object بنیاد;
    حرف *رمز;
};

ساخت rxkb_iso3166_رمز {
    ساخت rxkb_object بنیاد;
    حرف *رمز;
};

شمار context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ساخت rxkb_context {
    ساخت rxkb_object بنیاد;
    شمار context_state context_state;

    bool بھرو_extra_قواعد_files;
    bool use_secure_getenv;

    ساخت list models;         /* list of struct rxkb_models */
    ساخت list layouts;        /* list of struct rxkb_layouts */
    ساخت list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) خالی (*log_fn)(ساخت rxkb_context *ctx,
                                     شمار rxkb_log_level level,
                                     ثابت حرف *fmt, va_list دلائل);
    شمار rxkb_log_level log_level;

    خالی *userdata;
};

ساخت rxkb_model {
    ساخت rxkb_object بنیاد;

    حرف *name;
    حرف *vendor;
    حرف *description;
    شمار rxkb_popularity popularity;
};

ساخت rxkb_layout {
    ساخت rxkb_object بنیاد;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    شمار rxkb_popularity popularity;

    ساخت list iso639s;  /* list of struct rxkb_iso639_code */
    ساخت list iso3166s; /* list of struct rxkb_iso3166_code */
};

ساخت rxkb_option_گروہ {
    ساخت rxkb_object بنیاد;

    bool allow_مضرب;
    ساخت list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    شمار rxkb_popularity popularity;
};

ساخت rxkb_option {
    ساخت rxkb_object بنیاد;

    حرف *name;
    حرف *brief;
    حرف *description;
    شمار rxkb_popularity popularity;
    bool layout_specific;
};

ساکن bool
parse(ساخت rxkb_context *ctx, ثابت حرف *راستہ,
      شمار rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساکن خالی
rxkb_log(ساخت rxkb_context *ctx, شمار rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list دلائل;

    اگر (ctx->log_level < level)
        واپس;

    va_آغاز(دلائل, fmt);
    ctx->log_fn(ctx, level, fmt, دلائل);
    va_انتہا(دلائل);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#تعریف_کرو rxkb_log_with_رمز(ctx, level, پیغام_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_پیغام_ID(پیغام_id, fmt), ##__VA_ARGS__)
#تعریف_کرو log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#تعریف_کرو log_info(ctx, id, ...) \
    rxkb_log_with_رمز((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#تعریف_کرو log_warn(ctx, id, ...) \
    rxkb_log_with_رمز((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#تعریف_کرو log_err(ctx, id, ...) \
    rxkb_log_with_رمز((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#تعریف_کرو log_wsgo(ctx, id, ...) \
    rxkb_log_with_رمز((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#تعریف_کرو DECLARE_REF_UNREF_FOR_TYPE(type_) \
ساخت type_ * type_##_ref(ساخت type_ *object) { \
    rxkb_object_ref(&object->بنیاد); \
    واپس object; \
} \
ساخت type_ * type_##_unref(ساخت type_ *object) { \
    اگر (!object) واپس تہی; \
    أكد(object->بنیاد.refcount >= 1); \
    اگر (--object->بنیاد.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->بنیاد.link);\
        آزاد(object); \
    } \
    واپس تہی;\
}

#تعریف_کرو DECLARE_CREATE_FOR_TYPE(type_) \
ساکن اندرونی ساخت type_ * type_##_create(ساخت rxkb_object *والد) { \
    ساخت type_ *t = صفر_حافظہ_دو(1, حجم *t); \
    اگر (t) \
        rxkb_object_init(&t->بنیاد, والد); \
    واپس t; \
}

#تعریف_کرو DECLARE_TYPED_GETTER_FOR_TYPE(type_, میدان, rtype_) \
rtype_ type_##_get_##field_(ساخت type_ *object) { \
    واپس object->میدان; \
}

#تعریف_کرو DECLARE_GETTER_FOR_TYPE(type_, میدان) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, میدان, ثابت حرف*)

#تعریف_کرو DECLARE_پہلا_اگلا_FOR_TYPE(type_, والد_type, والد_میدان) \
ساخت type_ * type_##_first(ساخت والد_type *والد) { \
    ساخت type_ *o = تہی; \
    اگر (!list_خالی_خانے(&والد->والد_میدان)) \
        o = list_پہلا_اندراج(&والد->والد_میدان, o, بنیاد.link); \
    واپس o; \
} \
ساخت type_ * \
type_##_next(ساخت type_ *o) \
{ \
    ساخت والد_type *والد; \
    ساخت type_ *اگلا; \
    والد = container_کا(o->بنیاد.والد, ساخت والد_type, بنیاد); \
    اگلا = list_پہلا_اندراج(&o->بنیاد.link, o, بنیاد.link); \
    اگر (list_is_آخری(&والد->والد_میدان, &o->بنیاد.link)) \
        واپس تہی; \
    واپس اگلا; \
}

ساکن خالی
rxkb_object_init(ساخت rxkb_object *object, ساخت rxkb_object *والد)
{
    object->refcount = 1;
    object->والد = والد;
    list_init(&object->link);
}

ساکن خالی *
rxkb_object_ref(ساخت rxkb_object *object)
{
    أكد(object->refcount >= 1);
    ++object->refcount;
    واپس object;
}

ساکن خالی
rxkb_iso639_رمز_destroy(ساخت rxkb_iso639_رمز *رمز)
{
    آزاد(رمز->رمز);
}

ساخت rxkb_iso639_رمز *
rxkb_layout_get_iso639_پہلا(ساخت rxkb_layout *layout)
{
    ساخت rxkb_iso639_رمز *رمز = تہی;

    اگر (!list_خالی_خانے(&layout->iso639s))
        رمز = list_پہلا_اندراج(&layout->iso639s, رمز, بنیاد.link);

    واپس رمز;
}

ساخت rxkb_iso639_رمز *
rxkb_iso639_رمز_اگلا(ساخت rxkb_iso639_رمز *رمز)
{
    ساخت rxkb_iso639_رمز *اگلا = تہی;
    ساخت rxkb_layout *layout;

    layout = container_کا(رمز->بنیاد.والد, ساخت rxkb_layout, بنیاد);

    اگر (list_is_آخری(&layout->iso639s, &رمز->بنیاد.link))
        واپس تہی;

    اگلا = list_پہلا_اندراج(&رمز->بنیاد.link, رمز, بنیاد.link);

    واپس اگلا;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_رمز);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_رمز);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_رمز, رمز);

ساکن خالی
rxkb_iso3166_رمز_destroy(ساخت rxkb_iso3166_رمز *رمز)
{
    آزاد(رمز->رمز);
}

ساخت rxkb_iso3166_رمز *
rxkb_layout_get_iso3166_پہلا(ساخت rxkb_layout *layout)
{
    ساخت rxkb_iso3166_رمز *رمز = تہی;

    اگر (!list_خالی_خانے(&layout->iso3166s))
        رمز = list_پہلا_اندراج(&layout->iso3166s, رمز, بنیاد.link);

    واپس رمز;
}

ساخت rxkb_iso3166_رمز *
rxkb_iso3166_رمز_اگلا(ساخت rxkb_iso3166_رمز *رمز)
{
    ساخت rxkb_iso3166_رمز *اگلا = تہی;
    ساخت rxkb_layout *layout;

    layout = container_کا(رمز->بنیاد.والد, ساخت rxkb_layout, بنیاد);

    اگر (list_is_آخری(&layout->iso3166s, &رمز->بنیاد.link))
        واپس تہی;

    اگلا = list_پہلا_اندراج(&رمز->بنیاد.link, رمز, بنیاد.link);

    واپس اگلا;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_رمز);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_رمز);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_رمز, رمز);

ساکن خالی
rxkb_option_destroy(ساخت rxkb_option *o)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, شمار rxkb_popularity);
bool rxkb_option_is_layout_specific(ساخت rxkb_option *object) {
    واپس object->layout_specific;
}
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_option, rxkb_option_گروہ, options);

ساکن خالی
rxkb_layout_destroy(ساخت rxkb_layout *l)
{
    ساخت rxkb_iso639_رمز *iso639, *tmp_639;
    ساخت rxkb_iso3166_رمز *iso3166, *tmp_3166;

    آزاد(l->name);
    آزاد(l->brief);
    آزاد(l->description);
    آزاد(l->variant);

    list_for_each_محفوظ(iso639, tmp_639, &l->iso639s, بنیاد.link) {
        rxkb_iso639_رمز_unref(iso639);
    }
    list_for_each_محفوظ(iso3166, tmp_3166, &l->iso3166s, بنیاد.link) {
        rxkb_iso3166_رمز_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, شمار rxkb_popularity);
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ساکن خالی
rxkb_model_destroy(ساخت rxkb_model *m)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, شمار rxkb_popularity);
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_model, rxkb_context, models);

ساکن خالی
rxkb_option_گروہ_destroy(ساخت rxkb_option_گروہ *og)
{
    ساخت rxkb_option *o, *otmp;

    آزاد(og->name);
    آزاد(og->description);

    list_for_each_محفوظ(o, otmp, &og->options, بنیاد.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_گروہ_allows_مضرب(ساخت rxkb_option_گروہ *g)
{
    واپس g->allow_مضرب;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_گروہ);
DECLARE_CREATE_FOR_TYPE(rxkb_option_گروہ);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروہ, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروہ, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_گروہ, popularity, شمار rxkb_popularity);
DECLARE_پہلا_اگلا_FOR_TYPE(rxkb_option_گروہ, rxkb_context, option_groups);

ساکن خالی
rxkb_context_destroy(ساخت rxkb_context *ctx)
{
    ساخت rxkb_model *m, *mtmp;
    ساخت rxkb_layout *l, *ltmp;
    ساخت rxkb_option_گروہ *og, *ogtmp;
    حرف **راستہ;

    list_for_each_محفوظ(m, mtmp, &ctx->models, بنیاد.link)
        rxkb_model_unref(m);
    أكد(list_خالی_خانے(&ctx->models));

    list_for_each_محفوظ(l, ltmp, &ctx->layouts, بنیاد.link)
        rxkb_layout_unref(l);
    أكد(list_خالی_خانے(&ctx->layouts));

    list_for_each_محفوظ(og, ogtmp, &ctx->option_groups, بنیاد.link)
        rxkb_option_گروہ_unref(og);
    أكد(list_خالی_خانے(&ctx->option_groups));

    darray_foreach(راستہ, ctx->includes)
        آزاد(*راستہ);
    darray_free(ctx->includes);

    أكد(darray_خالی_خانے(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, شمار rxkb_log_level);

ساکن حرف *
rxkb_context_getenv(ساخت rxkb_context *ctx, ثابت حرف *name)
{
    اگر (ctx->use_secure_getenv) {
        واپس secure_getenv(name);
    } ورنہ {
        واپس getenv(name);
    }
}


خالی
rxkb_context_سیٹ_log_level(ساخت rxkb_context *ctx,
                           شمار rxkb_log_level level)
{
    ctx->log_level = level;
}

ساکن ثابت حرف *
log_level_مقصد_سابقہ(شمار rxkb_log_level level)
{
    انتخاب (level) {
    حالت RXKB_LOG_LEVEL_DEBUG:
        واپس "xkbregistry: DEBUG: ";
    حالت RXKB_LOG_LEVEL_INFO:
        واپس "xkbregistry: INFO: ";
    حالت RXKB_LOG_LEVEL_WARNING:
        واپس "xkbregistry: WARNING: ";
    حالت RXKB_LOG_LEVEL_ERROR:
        واپس "xkbregistry: ERROR: ";
    حالت RXKB_LOG_LEVEL_CRITICAL:
        واپس "xkbregistry: CRITICAL: ";
    طےشدہ:
        واپس تہی;
    }
}

ATTR_PRINTF(3, 0) ساکن خالی
default_log_fn(ساخت rxkb_context *ctx, شمار rxkb_log_level level,
               ثابت حرف *fmt, va_list دلائل)
{
    ثابت حرف *سابقہ = log_level_مقصد_سابقہ(level);

    اگر (سابقہ)
        fprintf(stderr, "%s", سابقہ);
    vfprintf(stderr, fmt, دلائل);
}

ساکن شمار rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت طویل lvl = strtol(level, &endptr, 10);
    اگر (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        واپس (شمار rxkb_log_level)lvl;

    اگر (istreq_سابقہ("crit", level))
        واپس RXKB_LOG_LEVEL_CRITICAL;
    اگر (istreq_سابقہ("err", level))
        واپس RXKB_LOG_LEVEL_ERROR;
    اگر (istreq_سابقہ("warn", level))
        واپس RXKB_LOG_LEVEL_WARNING;
    اگر (istreq_سابقہ("info", level))
        واپس RXKB_LOG_LEVEL_INFO;
    اگر (istreq_سابقہ("debug", level) || istreq_سابقہ("dbg", level))
        واپس RXKB_LOG_LEVEL_DEBUG;

    واپس RXKB_LOG_LEVEL_ERROR;
}

ساخت rxkb_context *
rxkb_context_new(شمار rxkb_context_flags flags)
{
    ساخت rxkb_context *ctx = rxkb_context_create(تہی);
    ثابت حرف *env;

    اگر (!ctx)
        واپس تہی;

    ctx->context_state = CONTEXT_NEW;
    ctx->بھرو_extra_قواعد_files = flags & RXKB_CONTEXT_بھرو_EXOTIC_قواعد;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    اگر (env)
        rxkb_context_سیٹ_log_level(ctx, log_level(env));

    ساکن ثابت شمار rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_بھرو_EXOTIC_قواعد
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    اگر (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        آزاد(ctx);
        واپس تہی;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    اگر (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_راستہ_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_راستہ,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_جڑ);
        rxkb_context_unref(ctx);
        واپس تہی;
    }

    واپس ctx;
}

خالی
rxkb_context_سیٹ_log_fn(ساخت rxkb_context *ctx,
                        خالی (*log_fn)(ساخت rxkb_context *ctx,
                                       شمار rxkb_log_level level,
                                       ثابت حرف *fmt, va_list دلائل))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_راستہ_append(ساخت rxkb_context *ctx, ثابت حرف *راستہ)
{
    صحیح err = 0;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "include paths can only be appended to a new context\n");
        جاؤ error;
    }

    ساخت stat stat_buf;
    err = stat(راستہ, &stat_buf);
    اگر (err != 0) {
        err = errno;
        جاؤ error;
    }
    اگر (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        جاؤ error;
    }

    اگر (!check_eaccess(راستہ, R_OK | X_OK)) {
        err = EACCES;
        جاؤ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف قواعد[PATH_MAX];
    اگر (!snprintf_محفوظ(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                       راستہ, DEFAULT_XKB_قواعد)) {
        log_err(ctx, XKB_ERROR_INVALID_راستہ,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(قواعد), راستہ, DEFAULT_XKB_قواعد);
        جاؤ error;
    }

    حرف * ثابت tmp = strdup(راستہ);
    اگر (!tmp) {
        err = ENOMEM;
        جاؤ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پیغام_NO_ID, "Include path added: %s\n", tmp);

    واپس true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پیغام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", راستہ, strerror(err));
    واپس false;
}

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES

ساکن صحیح
compare_str(ثابت خالی *a, ثابت خالی *b)
{
    واپس strcmp(*(حرف **)a, *(حرف **) b);
}

ساکن صحیح
جوڑو_براہ_راست_subdirectories(ساخت rxkb_context *ctx, ثابت حرف *راستہ,
                          darray_string *extensions,
                          darray_سائز_t versioned_گنتی,
                          سائز_t versioned_راستہ_لمبائی)
{
    صحیح ret = 0;
    صحیح err = ENOMEM;
    DIR *dir = تہی;

    /* Check extensions parent directory */
    ساخت stat stat_buf;
    err = stat(راستہ, &stat_buf);
    اگر (err != 0) {
        err = errno;
        جاؤ err;
    }
    اگر (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        جاؤ err;
    }
    اگر (!check_eaccess(راستہ, R_OK | X_OK)) {
        err = EACCES;
        جاؤ err;
    }

    dir = opendir(راستہ);
    اگر (dir == تہی) {
        err = EACCES;
        جاؤ err;
    }

    ساخت dirent *اندراج_2;
    حرف راستہ_buf[PATH_MAX] = "";
    versioned_راستہ_لمبائی++; /* Additional final ‘/’ */
    جب ((اندراج_2 = readdir(dir)) != تہی) {
        ثابت حرف *name = اندراج_2->d_name;

        /* Skip special entries */
        اگر (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            جاری;

        اگر (!snprintf_محفوظ(راستہ_buf, حجم(راستہ_buf), "%s/%s", راستہ, name)) {
            err = ENOMEM;
            جاؤ err;
        }
        اگر (stat(راستہ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_منوال))
            جاری;

        /* Skip if the corresponding versioned directory is already included */
        برائے (darray_سائز_t i = 0; i < versioned_گنتی; i++) {
            ثابت حرف * ثابت پچھلا_name = darray_شے(*extensions, i)
                                         + versioned_راستہ_لمبائی;
            اگر (strcmp(name, پچھلا_name) == 0)
                جاؤ اگلا;
        }

        حرف *ext_راستہ = strdup_محفوظ(راستہ_buf);
        اگر (!ext_راستہ) {
            err = ENOMEM;
            جاؤ err;
        }

        darray_append(*extensions, ext_راستہ);
اگلا:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    اگر (darray_سائز(*extensions) > versioned_گنتی) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اشیاء(*extensions) + versioned_گنتی,
              darray_سائز(*extensions) - versioned_گنتی,
              حجم(*darray_اشیاء(*extensions)), &compare_str);
        حرف **ext_راستہ;
        darray_foreach_from(ext_راستہ, *extensions, versioned_گنتی) {
            ret |= rxkb_context_include_راستہ_append(ctx, *ext_راستہ);
        }
    }

    واپس ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", راستہ, strerror(err));
    اگر (dir)
        closedir(dir);

    واپس ret;
}
#اگر_ختم

bool
rxkb_context_include_راستہ_append_default(ساخت rxkb_context *ctx)
{
    حرف user_راستہ[PATH_MAX];
    bool ret = false;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "include paths can only be appended to a new context\n");
        واپس false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    اگر (xdg != تہی) {
        اگر (snprintf_محفوظ(user_راستہ, حجم(user_راستہ), "%s/xkb", xdg))
            ret |= rxkb_context_include_راستہ_append(ctx, user_راستہ);
    } ورنہ اگر (home != تہی) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        اگر (snprintf_محفوظ(user_راستہ, حجم(user_راستہ), "%s/.config/xkb", home))
            ret |= rxkb_context_include_راستہ_append(ctx, user_راستہ);
    }

    اگر (home != تہی) {
        اگر (snprintf_محفوظ(user_راستہ, حجم(user_راستہ), "%s/.xkb", home))
            ret |= rxkb_context_include_راستہ_append(ctx, user_راستہ);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_راستہ_append(
        ctx, ((extra != تہی) ? extra : DFLT_XKB_CONFIG_EXTRA_راستہ)
    );

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_راستہ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#اگر_تعریف_شدہ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_راستہ
    اگر (extensions_راستہ == تہی)
        extensions_راستہ = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_راستہ;
#اگر_ختم
    سائز_t versioned_راستہ_لمبائی = 0;
    اگر (extensions_راستہ != تہی) {
        /* Add direct subdirectories */
        ret |= جوڑو_براہ_راست_subdirectories(ctx, extensions_راستہ,
                                         &extensions, 0, 0);
        versioned_راستہ_لمبائی = لڑی_لمبائی(extensions_راستہ);
    }

    /* Unversioned extensions directory */
    extensions_راستہ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#اگر_تعریف_شدہ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_راستہ
    اگر (extensions_راستہ == تہی)
        extensions_راستہ = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_راستہ;
#اگر_ختم
    اگر (extensions_راستہ != تہی) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جوڑو_براہ_راست_subdirectories(ctx, extensions_راستہ, &extensions,
                                         darray_سائز(extensions),
                                         versioned_راستہ_لمبائی);
    }

    حرف **ext_راستہ;
    darray_foreach(ext_راستہ, extensions) {
        آزاد(*ext_راستہ);
    }
    darray_free(extensions);
#اگر_ختم

    /* Canonical XKB root */
    ثابت حرف * ثابت جڑ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_جڑ = rxkb_context_include_راستہ_append(
        ctx, ((جڑ != تہی) ? جڑ: DFLT_XKB_CONFIG_جڑ)
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
    اگر (!has_جڑ && (جڑ == تہی || جڑ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_جڑ,
                 ((جڑ == تہی) ? DFLT_XKB_CONFIG_جڑ : جڑ));
        ret |= rxkb_context_include_راستہ_append(ctx, DFLT_XKB_LEGACY_جڑ);
    }

    واپس ret;
}

bool
rxkb_context_parse_default_ruleset(ساخت rxkb_context *ctx)
{
    واپس rxkb_context_parse(ctx, DEFAULT_XKB_قواعد);
}

bool
rxkb_context_parse(ساخت rxkb_context *ctx, ثابت حرف *ruleset)
{
    حرف **راستہ;
    bool success = false;

    اگر (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "parse must only be called on a new context\n");
        واپس false;
    }

    darray_foreach_الٹ(راستہ, ctx->includes) {
        حرف قواعد[PATH_MAX];

        اگر (snprintf_محفوظ(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                           *راستہ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            اگر (parse(ctx, قواعد, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        اگر (ctx->بھرو_extra_قواعد_files &&
            snprintf_محفوظ(قواعد, حجم(قواعد), "%s/rules/%s.extras.xml",
                          *راستہ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            اگر (parse(ctx, قواعد, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    واپس success;
}


خالی
rxkb_context_سیٹ_user_ڈیٹا(ساخت rxkb_context *ctx, خالی *userdata)
{
    ctx->userdata = userdata;
}

خالی *
rxkb_context_get_user_ڈیٹا(ساخت rxkb_context *ctx)
{
    واپس ctx->userdata;
}

ساکن اندرونی bool
is_گرہ(xml_گرہ *گرہ, ثابت حرف *name)
{
    واپس گرہ->type == XML_ELEMENT_گرہ &&
        xmlStrEqual(گرہ->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساکن حرف *
نکالو_متن(xml_گرہ *گرہ)
{
    xml_گرہ *n;

    برائے (n = گرہ->children; n; n = n->اگلا) {
        اگر (n->type == XML_متن_گرہ)
            واپس (حرف *)xmlStrdup(n->content);
    }
    واپس تہی;
}

/* Data from “configItem” node */
ساخت config_شے {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    شمار rxkb_popularity popularity;
    bool layout_specific;
};

#تعریف_کرو config_شے_new(popularity_) { \
    .name = تہی, \
    .description = تہی, \
    .brief = تہی, \
    .vendor = تہی, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساکن خالی
config_شے_free(ساخت config_شے *config) {
    آزاد(config->name);
    آزاد(config->description);
    آزاد(config->brief);
    آزاد(config->vendor);
}

ساکن bool
parse_config_شے(ساخت rxkb_context *ctx, xml_گرہ *والد,
                  ساخت config_شے *config)
{
    xml_گرہ *گرہ = تہی;
    xml_گرہ *ci = تہی;

    برائے (ci = والد->children; ci; ci = ci->اگلا) {
        اگر (is_گرہ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            اگر (raw_popularity) {
                اگر (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ورنہ اگر (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ورنہ
                    log_err(ctx, XKB_LOG_پیغام_NO_ID,
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
            برائے (گرہ = ci->children; گرہ; گرہ = گرہ->اگلا) {
                اگر (is_گرہ(گرہ, "name"))
                    config->name = نکالو_متن(گرہ);
                ورنہ اگر (is_گرہ(گرہ, "description"))
                    config->description = نکالو_متن(گرہ);
                ورنہ اگر (is_گرہ(گرہ, "shortDescription"))
                    config->brief = نکالو_متن(گرہ);
                ورنہ اگر (is_گرہ(گرہ, "vendor"))
                    config->vendor = نکالو_متن(گرہ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            اگر (!config->name || !لڑی_لمبائی(config->name))  {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->سطر);
                config_شے_free(config);
                واپس false;
            }

            واپس true; /* only one configItem allowed in the dtd */
        }
    }

    واپس false;
}

ساکن خالی
parse_model(ساخت rxkb_context *ctx, xml_گرہ *model,
            شمار rxkb_popularity popularity)
{
    ساخت config_شے config = config_شے_new(popularity);

    اگر (parse_config_شے(ctx, model, &config)) {
        ساخت rxkb_model *m;

        list_for_each(m, &ctx->models, بنیاد.link) {
            اگر (streq(m->name, config.name)) {
                config_شے_free(&config);
                واپس;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->بنیاد);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->بنیاد.link);
    }
}

ساکن خالی
parse_model_list(ساخت rxkb_context *ctx, xml_گرہ *model_list,
                 شمار rxkb_popularity popularity)
{
    xml_گرہ *گرہ = تہی;

    برائے (گرہ = model_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "model"))
            parse_model(ctx, گرہ, popularity);
    }
}

ساکن خالی
parse_language_list(xml_گرہ *language_list, ساخت rxkb_layout *layout)
{
    xml_گرہ *گرہ = تہی;
    ساخت rxkb_iso639_رمز *رمز;

    برائے (گرہ = language_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "iso639Id")) {
            حرف *str = نکالو_متن(گرہ);
            ساخت rxkb_object *والد;

            اگر (!str || لڑی_لمبائی(str) != 3) {
                آزاد(str);
                جاری;
            }

            والد = &layout->بنیاد;
            رمز = rxkb_iso639_رمز_create(والد);
            رمز->رمز = str;
            list_append(&layout->iso639s, &رمز->بنیاد.link);
        }
    }
}

ساکن خالی
parse_country_list(xml_گرہ *country_list, ساخت rxkb_layout *layout)
{
    xml_گرہ *گرہ = تہی;
    ساخت rxkb_iso3166_رمز *رمز;

    برائے (گرہ = country_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "iso3166Id")) {
            حرف *str = نکالو_متن(گرہ);
            ساخت rxkb_object *والد;

            اگر (!str || لڑی_لمبائی(str) != 2) {
                آزاد(str);
                جاری;
            }

            والد = &layout->بنیاد;
            رمز = rxkb_iso3166_رمز_create(والد);
            رمز->رمز = str;
            list_append(&layout->iso3166s, &رمز->بنیاد.link);
        }
    }
}

ساکن خالی
parse_variant(ساخت rxkb_context *ctx, ساخت rxkb_layout *l,
              xml_گرہ *variant, شمار rxkb_popularity popularity)
{
    xml_گرہ *ci;
    ساخت config_شے config = config_شے_new(popularity);

    اگر (parse_config_شے(ctx, variant, &config)) {
        ساخت rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, بنیاد.link) {
            اگر (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                توڑو;
            }
        }

        اگر (!exists) {
            v = rxkb_layout_create(&ctx->بنیاد);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == تہی ? strdup_محفوظ(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->بنیاد.link);

            برائے (ci = variant->children; ci; ci = ci->اگلا) {
                xml_گرہ *گرہ;

                اگر (!is_گرہ(ci, "configItem"))
                    جاری;

                bool ملا_language_list = false;
                bool ملا_country_list = false;
                برائے (گرہ = ci->children; گرہ; گرہ = گرہ->اگلا) {
                    اگر (is_گرہ(گرہ, "languageList")) {
                        parse_language_list(گرہ, v);
                        ملا_language_list = true;
                    }
                    اگر (is_گرہ(گرہ, "countryList")) {
                        parse_country_list(گرہ, v);
                        ملا_country_list = true;
                    }
                }
                اگر (!ملا_language_list) {
                    // inherit from parent layout
                    ساخت rxkb_iso639_رمز* x;
                    list_for_each(x, &l->iso639s, بنیاد.link) {
                        ساخت rxkb_iso639_رمز* رمز = rxkb_iso639_رمز_create(&v->بنیاد);
                        رمز->رمز = strdup(x->رمز);
                        list_append(&v->iso639s, &رمز->بنیاد.link);
                    }
                }
                اگر (!ملا_country_list) {
                    // inherit from parent layout
                    ساخت rxkb_iso3166_رمز* x;
                    list_for_each(x, &l->iso3166s, بنیاد.link) {
                        ساخت rxkb_iso3166_رمز* رمز = rxkb_iso3166_رمز_create(&v->بنیاد);
                        رمز->رمز = strdup(x->رمز);
                        list_append(&v->iso3166s, &رمز->بنیاد.link);
                    }
                }
            }
        } ورنہ {
            config_شے_free(&config);
        }
    }
}

ساکن خالی
parse_variant_list(ساخت rxkb_context *ctx, ساخت rxkb_layout *l,
                   xml_گرہ *variant_list, شمار rxkb_popularity popularity)
{
    xml_گرہ *گرہ = تہی;

    برائے (گرہ = variant_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "variant"))
            parse_variant(ctx, l, گرہ, popularity);
    }
}

ساکن خالی
parse_layout(ساخت rxkb_context *ctx, xml_گرہ *layout,
             شمار rxkb_popularity popularity)
{
    ساخت config_شے config = config_شے_new(popularity);
    ساخت rxkb_layout *l;
    xml_گرہ *گرہ = تہی;
    bool exists = false;

    اگر (!parse_config_شے(ctx, layout, &config))
        واپس;

    list_for_each(l, &ctx->layouts, بنیاد.link) {
        اگر (streq(l->name, config.name) && l->variant == تہی) {
            exists = true;
            توڑو;
        }
    }

    اگر (!exists) {
        l = rxkb_layout_create(&ctx->بنیاد);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = تہی;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->بنیاد.link);
    } ورنہ {
        config_شے_free(&config);
    }

    برائے (گرہ = layout->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "variantList")) {
            parse_variant_list(ctx, l, گرہ, popularity);
        }
        اگر (!exists && is_گرہ(گرہ, "configItem")) {
            xml_گرہ *ll;
            برائے (ll = گرہ->children; ll; ll = ll->اگلا) {
                اگر (is_گرہ(ll, "languageList"))
                    parse_language_list(ll, l);
                اگر (is_گرہ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساکن خالی
parse_layout_list(ساخت rxkb_context *ctx, xml_گرہ *layout_list,
                  شمار rxkb_popularity popularity)
{
    xml_گرہ *گرہ = تہی;

    برائے (گرہ = layout_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "layout"))
            parse_layout(ctx, گرہ, popularity);
    }
}

ساکن خالی
parse_option(ساخت rxkb_context *ctx, ساخت rxkb_option_گروہ *گروہ,
             xml_گرہ *option, شمار rxkb_popularity popularity)
{
    ساخت config_شے config = config_شے_new(popularity);

    اگر (parse_config_شے(ctx, option, &config)) {
        ساخت rxkb_option *o;

        list_for_each(o, &گروہ->options, بنیاد.link) {
            اگر (streq(o->name, config.name)) {
                config_شے_free(&config);
                واپس;
            }
        }

        o = rxkb_option_create(&گروہ->بنیاد);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&گروہ->options, &o->بنیاد.link);
    }
}

ساکن خالی
parse_گروہ(ساخت rxkb_context *ctx, xml_گرہ *گروہ,
            شمار rxkb_popularity popularity)
{
    ساخت config_شے config = config_شے_new(popularity);
    ساخت rxkb_option_گروہ *g;
    xml_گرہ *گرہ = تہی;
    xmlChar *مضرب;
    bool exists = false;

    اگر (!parse_config_شے(ctx, گروہ, &config))
        واپس;

    list_for_each(g, &ctx->option_groups, بنیاد.link) {
        اگر (streq(g->name, config.name)) {
            exists = true;
            توڑو;
        }
    }

    اگر (!exists) {
        g = rxkb_option_گروہ_create(&ctx->بنیاد);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        مضرب = xmlGetProp(گروہ, (ثابت xmlChar*)"allowMultipleSelection");
        اگر (مضرب && xmlStrEqual(مضرب, (ثابت xmlChar*)"true"))
            g->allow_مضرب = true;
        xmlFree(مضرب);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->بنیاد.link);
    } ورنہ {
        config_شے_free(&config);
    }

    برائے (گرہ = گروہ->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "option"))
            parse_option(ctx, g, گرہ, popularity);
    }
}

ساکن خالی
parse_option_list(ساخت rxkb_context *ctx, xml_گرہ *option_list,
                  شمار rxkb_popularity popularity)
{
    xml_گرہ *گرہ = تہی;

    برائے (گرہ = option_list->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "group"))
            parse_گروہ(ctx, گرہ, popularity);
    }
}

ساکن خالی
parse_قواعد_xml(ساخت rxkb_context *ctx, xml_گرہ *جڑ,
                شمار rxkb_popularity popularity)
{
    xml_گرہ *گرہ = تہی;

    برائے (گرہ = جڑ->children; گرہ; گرہ = گرہ->اگلا) {
        اگر (is_گرہ(گرہ, "modelList"))
            parse_model_list(ctx, گرہ, popularity);
        ورنہ اگر (is_گرہ(گرہ, "layoutList"))
            parse_layout_list(ctx, گرہ, popularity);
        ورنہ اگر (is_گرہ(گرہ, "optionList"))
            parse_option_list(ctx, گرہ, popularity);
    }
}

ساکن خالی
ATTR_PRINTF(2, 0)
xml_error_func(خالی *ctx, ثابت حرف *پیغام, ...)
{
    ساکن حرف buf[PATH_MAX];
    ساکن سائز_t slen = 0;
    va_list دلائل;
    صحیح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_آغاز(دلائل, پیغام);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, پیغام, دلائل);
    va_انتہا(دلائل);

    /* This shouldn't really happen */
    اگر (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, حجم(buf));
        واپس;
    }

    slen += (سائز_t)rc;
    اگر (slen >= حجم(buf)) {
        /* truncated, let's flush this */
        buf[حجم(buf) - 1] = '\n';
        slen = حجم(buf);
    }

    /* We're assuming here that the last character is \n. */
    اگر (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_پیغام_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#اگر HAVE_XML_CTXT_سیٹ_ERRORHANDLER
ساکن خالی
xml_structured_error_func(خالی *user_ڈیٹا_2, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_ڈیٹا_2);
}
#اگر_ختم

#اگر_تعریف_شدہ XML_PARSE_NO_XXE
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ورنہ
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET)
#اگر_ختم

ساکن bool
validate(ساخت rxkb_context *ctx, xmlDoc *doc)
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

#اگر HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    اگر (!xmlCtxt)
        واپس false;
    static_assert(HAVE_XML_CTXT_سیٹ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_سیٹ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_داخلہ_Ptr pinput =
        xml_New_داخلہ_From_حافظہ(تہی, dtdstr, ARRAY_سائز(dtdstr) - 1,
                              XML_داخلہ_BUF_STATIC);
    اگر (!pinput)
        جاؤ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, تہی, تہی);
#ورنہ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_داخلہ_بفر_Ptr buf =
        xml_Parser_داخلہ_بفر_Create_Mem(dtdstr, ARRAY_سائز(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    اگر (!buf)
        جاؤ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(تہی, buf, XML_CHAR_ENCODING_UTF8);
#اگر_ختم

    اگر (!dtd) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID, "Failed to load DTD\n");
        جاؤ dtd_error;
    }

#اگر HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ورنہ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    اگر (!dtdvalid) {
        success = false;
        جاؤ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_ڈیٹا_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#اگر_ختم
    xmlFreeDtd(dtd);

dtd_error:
#اگر HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#اگر_ختم
    واپس success;
}

ساکن bool
parse(ساخت rxkb_context *ctx, ثابت حرف *راستہ,
      شمار rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = تہی;
    xml_گرہ *جڑ = تہی;

    اگر (!check_eaccess(راستہ, R_OK))
        واپس false;

    LIBXML_TEST_نسخہ

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    اگر (!xmlCtxt)
        واپس false;

#اگر_تعریف_شدہ XML_PARSE_NO_XXE
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ورنہ
#تعریف_کرو _XML_OPTIONS (XML_PARSE_NONET)
#اگر_ختم
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#اگر HAVE_XML_CTXT_سیٹ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#اگر_ختم
#اگر !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_سیٹ_Generic_Error_Func(ctx, xml_error_func);
#اگر_ختم

    doc = xmlCtxtReadFile(xmlCtxt, راستہ, تہی, 0);
    اگر (!doc)
        جاؤ parse_error;

    اگر (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "XML error: failed to validate document at %s\n", راستہ);
        جاؤ validate_error;
    }

    جڑ = xml_Doc_Get_جڑ_Element(doc);
    parse_قواعد_xml(ctx, جڑ, popularity);

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
    xml_سیٹ_Generic_Error_Func(تہی, تہی);
#اگر_ختم
#اگر HAVE_XML_CTXT_سیٹ_ERRORHANDLER
    xml_Ctxt_سیٹ_Error_Handler(xmlCtxt, تہی, تہی);
#اگر_ختم
    xmlFreeParserCtxt(xmlCtxt);

    واپس success;
}
