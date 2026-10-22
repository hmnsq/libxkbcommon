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
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

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
    بنية rxkb_object *والد;
    uint32_t refcount;
    بنية list link;
};

بنية rxkb_iso639_ڪوڊ {
    بنية rxkb_object بنياد;
    حرف *ڪوڊ;
};

بنية rxkb_iso3166_ڪوڊ {
    بنية rxkb_object بنياد;
    حرف *ڪوڊ;
};

تعداد context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

بنية rxkb_context {
    بنية rxkb_object بنياد;
    تعداد context_state context_state;

    bool ڀريو_extra_قاعدا_files;
    bool use_secure_getenv;

    بنية list models;         /* list of struct rxkb_models */
    بنية list layouts;        /* list of struct rxkb_layouts */
    بنية list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية rxkb_context *ctx,
                                     تعداد rxkb_log_level level,
                                     ثابت حرف *fmt, va_list دليلون);
    تعداد rxkb_log_level log_level;

    فراغ *userdata;
};

بنية rxkb_model {
    بنية rxkb_object بنياد;

    حرف *name;
    حرف *vendor;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_layout {
    بنية rxkb_object بنياد;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    تعداد rxkb_popularity popularity;

    بنية list iso639s;  /* list of struct rxkb_iso639_code */
    بنية list iso3166s; /* list of struct rxkb_iso3166_code */
};

بنية rxkb_option_گروهه {
    بنية rxkb_object بنياد;

    bool allow_گڻو;
    بنية list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_option {
    بنية rxkb_object بنياد;

    حرف *name;
    حرف *brief;
    حرف *description;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *رستو,
      تعداد rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساكن فراغ
rxkb_log(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list دليلون;

    إذا (ctx->log_level < level)
        أرجع;

    va_شروعات(دليلون, fmt);
    ctx->log_fn(ctx, level, fmt, دليلون);
    va_پڇاڙي(دليلون);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#وصف_ڪريو rxkb_log_with_ڪوڊ(ctx, level, نياپو_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_پيغام_ID(نياپو_id, fmt), ##__VA_ARGS__)
#وصف_ڪريو log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#وصف_ڪريو log_info(ctx, id, ...) \
    rxkb_log_with_ڪوڊ((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#وصف_ڪريو log_warn(ctx, id, ...) \
    rxkb_log_with_ڪوڊ((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#وصف_ڪريو log_err(ctx, id, ...) \
    rxkb_log_with_ڪوڊ((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#وصف_ڪريو log_wsgo(ctx, id, ...) \
    rxkb_log_with_ڪوڊ((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#وصف_ڪريو DECLARE_REF_UNREF_FOR_TYPE(type_) \
بنية type_ * type_##_ref(بنية type_ *object) { \
    rxkb_object_ref(&object->بنياد); \
    أرجع object; \
} \
بنية type_ * type_##_unref(بنية type_ *object) { \
    إذا (!object) أرجع عدم; \
    أكد(object->بنياد.refcount >= 1); \
    إذا (--object->بنياد.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->بنياد.link);\
        حرر(object); \
    } \
    أرجع عدم;\
}

#وصف_ڪريو DECLARE_CREATE_FOR_TYPE(type_) \
ساكن مضمن بنية type_ * type_##_create(بنية rxkb_object *والد) { \
    بنية type_ *t = صفر_يادگيري_ڏيو(1, حجم *t); \
    إذا (t) \
        rxkb_object_init(&t->بنياد, والد); \
    أرجع t; \
}

#وصف_ڪريو DECLARE_TYPED_GETTER_FOR_TYPE(type_, ميدان, rtype_) \
rtype_ type_##_get_##field_(بنية type_ *object) { \
    أرجع object->ميدان; \
}

#وصف_ڪريو DECLARE_GETTER_FOR_TYPE(type_, ميدان) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, ميدان, ثابت حرف*)

#وصف_ڪريو DECLARE_پهريون_ايندڙ_FOR_TYPE(type_, والد_type, والد_ميدان) \
بنية type_ * type_##_first(بنية والد_type *والد) { \
    بنية type_ *o = عدم; \
    إذا (!list_خالي_جايون(&والد->والد_ميدان)) \
        o = list_پهريون_داخلا(&والد->والد_ميدان, o, بنياد.link); \
    أرجع o; \
} \
بنية type_ * \
type_##_next(بنية type_ *o) \
{ \
    بنية والد_type *والد; \
    بنية type_ *ايندڙ; \
    والد = container_جو(o->بنياد.والد, بنية والد_type, بنياد); \
    ايندڙ = list_پهريون_داخلا(&o->بنياد.link, o, بنياد.link); \
    إذا (list_is_آخري(&والد->والد_ميدان, &o->بنياد.link)) \
        أرجع عدم; \
    أرجع ايندڙ; \
}

ساكن فراغ
rxkb_object_init(بنية rxkb_object *object, بنية rxkb_object *والد)
{
    object->refcount = 1;
    object->والد = والد;
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
rxkb_iso639_ڪوڊ_destroy(بنية rxkb_iso639_ڪوڊ *ڪوڊ)
{
    حرر(ڪوڊ->ڪوڊ);
}

بنية rxkb_iso639_ڪوڊ *
rxkb_layout_get_iso639_پهريون(بنية rxkb_layout *layout)
{
    بنية rxkb_iso639_ڪوڊ *ڪوڊ = عدم;

    إذا (!list_خالي_جايون(&layout->iso639s))
        ڪوڊ = list_پهريون_داخلا(&layout->iso639s, ڪوڊ, بنياد.link);

    أرجع ڪوڊ;
}

بنية rxkb_iso639_ڪوڊ *
rxkb_iso639_ڪوڊ_ايندڙ(بنية rxkb_iso639_ڪوڊ *ڪوڊ)
{
    بنية rxkb_iso639_ڪوڊ *ايندڙ = عدم;
    بنية rxkb_layout *layout;

    layout = container_جو(ڪوڊ->بنياد.والد, بنية rxkb_layout, بنياد);

    إذا (list_is_آخري(&layout->iso639s, &ڪوڊ->بنياد.link))
        أرجع عدم;

    ايندڙ = list_پهريون_داخلا(&ڪوڊ->بنياد.link, ڪوڊ, بنياد.link);

    أرجع ايندڙ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_ڪوڊ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_ڪوڊ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_ڪوڊ, ڪوڊ);

ساكن فراغ
rxkb_iso3166_ڪوڊ_destroy(بنية rxkb_iso3166_ڪوڊ *ڪوڊ)
{
    حرر(ڪوڊ->ڪوڊ);
}

بنية rxkb_iso3166_ڪوڊ *
rxkb_layout_get_iso3166_پهريون(بنية rxkb_layout *layout)
{
    بنية rxkb_iso3166_ڪوڊ *ڪوڊ = عدم;

    إذا (!list_خالي_جايون(&layout->iso3166s))
        ڪوڊ = list_پهريون_داخلا(&layout->iso3166s, ڪوڊ, بنياد.link);

    أرجع ڪوڊ;
}

بنية rxkb_iso3166_ڪوڊ *
rxkb_iso3166_ڪوڊ_ايندڙ(بنية rxkb_iso3166_ڪوڊ *ڪوڊ)
{
    بنية rxkb_iso3166_ڪوڊ *ايندڙ = عدم;
    بنية rxkb_layout *layout;

    layout = container_جو(ڪوڊ->بنياد.والد, بنية rxkb_layout, بنياد);

    إذا (list_is_آخري(&layout->iso3166s, &ڪوڊ->بنياد.link))
        أرجع عدم;

    ايندڙ = list_پهريون_داخلا(&ڪوڊ->بنياد.link, ڪوڊ, بنياد.link);

    أرجع ايندڙ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_ڪوڊ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_ڪوڊ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_ڪوڊ, ڪوڊ);

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
DECLARE_پهريون_ايندڙ_FOR_TYPE(rxkb_option, rxkb_option_گروهه, options);

ساكن فراغ
rxkb_layout_destroy(بنية rxkb_layout *l)
{
    بنية rxkb_iso639_ڪوڊ *iso639, *tmp_639;
    بنية rxkb_iso3166_ڪوڊ *iso3166, *tmp_3166;

    حرر(l->name);
    حرر(l->brief);
    حرر(l->description);
    حرر(l->variant);

    list_for_each_محفوظ(iso639, tmp_639, &l->iso639s, بنياد.link) {
        rxkb_iso639_ڪوڊ_unref(iso639);
    }
    list_for_each_محفوظ(iso3166, tmp_3166, &l->iso3166s, بنياد.link) {
        rxkb_iso3166_ڪوڊ_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, تعداد rxkb_popularity);
DECLARE_پهريون_ايندڙ_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

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
DECLARE_پهريون_ايندڙ_FOR_TYPE(rxkb_model, rxkb_context, models);

ساكن فراغ
rxkb_option_گروهه_destroy(بنية rxkb_option_گروهه *og)
{
    بنية rxkb_option *o, *otmp;

    حرر(og->name);
    حرر(og->description);

    list_for_each_محفوظ(o, otmp, &og->options, بنياد.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_گروهه_allows_گڻو(بنية rxkb_option_گروهه *g)
{
    أرجع g->allow_گڻو;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_گروهه);
DECLARE_CREATE_FOR_TYPE(rxkb_option_گروهه);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروهه, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروهه, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_گروهه, popularity, تعداد rxkb_popularity);
DECLARE_پهريون_ايندڙ_FOR_TYPE(rxkb_option_گروهه, rxkb_context, option_groups);

ساكن فراغ
rxkb_context_destroy(بنية rxkb_context *ctx)
{
    بنية rxkb_model *m, *mtmp;
    بنية rxkb_layout *l, *ltmp;
    بنية rxkb_option_گروهه *og, *ogtmp;
    حرف **رستو;

    list_for_each_محفوظ(m, mtmp, &ctx->models, بنياد.link)
        rxkb_model_unref(m);
    أكد(list_خالي_جايون(&ctx->models));

    list_for_each_محفوظ(l, ltmp, &ctx->layouts, بنياد.link)
        rxkb_layout_unref(l);
    أكد(list_خالي_جايون(&ctx->layouts));

    list_for_each_محفوظ(og, ogtmp, &ctx->option_groups, بنياد.link)
        rxkb_option_گروهه_unref(og);
    أكد(list_خالي_جايون(&ctx->option_groups));

    darray_foreach(رستو, ctx->includes)
        حرر(*رستو);
    darray_free(ctx->includes);

    أكد(darray_خالي_جايون(ctx->includes));
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
rxkb_context_سيٽ_log_level(بنية rxkb_context *ctx,
                           تعداد rxkb_log_level level)
{
    ctx->log_level = level;
}

ساكن ثابت حرف *
log_level_منزل_اڳياڙي(تعداد rxkb_log_level level)
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
               ثابت حرف *fmt, va_list دليلون)
{
    ثابت حرف *اڳياڙي = log_level_منزل_اڳياڙي(level);

    إذا (اڳياڙي)
        fprintf(stderr, "%s", اڳياڙي);
    vfprintf(stderr, fmt, دليلون);
}

ساكن تعداد rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت ڊگهو lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد rxkb_log_level)lvl;

    إذا (istreq_اڳياڙي("crit", level))
        أرجع RXKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_اڳياڙي("err", level))
        أرجع RXKB_LOG_LEVEL_ERROR;
    إذا (istreq_اڳياڙي("warn", level))
        أرجع RXKB_LOG_LEVEL_WARNING;
    إذا (istreq_اڳياڙي("info", level))
        أرجع RXKB_LOG_LEVEL_INFO;
    إذا (istreq_اڳياڙي("debug", level) || istreq_اڳياڙي("dbg", level))
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
    ctx->ڀريو_extra_قاعدا_files = flags & RXKB_CONTEXT_ڀريو_EXOTIC_قاعدا;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    إذا (env)
        rxkb_context_سيٽ_log_level(ctx, log_level(env));

    ساكن ثابت تعداد rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_ڀريو_EXOTIC_قاعدا
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    إذا (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        حرر(ctx);
        أرجع عدم;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    إذا (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_رستو_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_رستو,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_پاڙ);
        rxkb_context_unref(ctx);
        أرجع عدم;
    }

    أرجع ctx;
}

فراغ
rxkb_context_سيٽ_log_fn(بنية rxkb_context *ctx,
                        فراغ (*log_fn)(بنية rxkb_context *ctx,
                                       تعداد rxkb_log_level level,
                                       ثابت حرف *fmt, va_list دليلون))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_رستو_append(بنية rxkb_context *ctx, ثابت حرف *رستو)
{
    صحيح err = 0;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "include paths can only be appended to a new context\n");
        اذهب error;
    }

    بنية stat stat_buf;
    err = stat(رستو, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب error;
    }
    إذا (!S_ISDIR(stat_buf.st_گهڻائي)) {
        err = ENOTDIR;
        اذهب error;
    }

    إذا (!check_eaccess(رستو, R_OK | X_OK)) {
        err = EACCES;
        اذهب error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف قاعدا[PATH_MAX];
    إذا (!snprintf_محفوظ(قاعدا, حجم(قاعدا), "%s/rules/%s.xml",
                       رستو, DEFAULT_XKB_قاعدا)) {
        log_err(ctx, XKB_ERROR_INVALID_رستو,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(قاعدا), رستو, DEFAULT_XKB_قاعدا);
        اذهب error;
    }

    حرف * ثابت tmp = strdup(رستو);
    إذا (!tmp) {
        err = ENOMEM;
        اذهب error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پيغام_NO_ID, "Include path added: %s\n", tmp);

    أرجع true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پيغام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", رستو, strerror(err));
    أرجع false;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
جوڙيو_سڌو_subdirectories(بنية rxkb_context *ctx, ثابت حرف *رستو,
                          darray_string *extensions,
                          darray_سائيز_t versioned_ڳڻپ,
                          سائيز_t versioned_رستو_ڊيگهائي)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(رستو, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_گهڻائي)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(رستو, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(رستو);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *داخلا;
    حرف رستو_buf[PATH_MAX] = "";
    versioned_رستو_ڊيگهائي++; /* Additional final ‘/’ */
    طالما ((داخلا = readdir(dir)) != عدم) {
        ثابت حرف *name = داخلا->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_محفوظ(رستو_buf, حجم(رستو_buf), "%s/%s", رستو, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(رستو_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_گهڻائي))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_سائيز_t i = 0; i < versioned_ڳڻپ; i++) {
            ثابت حرف * ثابت prev_name = darray_شي(*extensions, i)
                                         + versioned_رستو_ڊيگهائي;
            إذا (strcmp(name, prev_name) == 0)
                اذهب ايندڙ;
        }

        حرف *ext_رستو = strdup_محفوظ(رستو_buf);
        إذا (!ext_رستو) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_رستو);
ايندڙ:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_سائيز(*extensions) > versioned_ڳڻپ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_شيون(*extensions) + versioned_ڳڻپ,
              darray_سائيز(*extensions) - versioned_ڳڻپ,
              حجم(*darray_شيون(*extensions)), &compare_str);
        حرف **ext_رستو;
        darray_foreach_from(ext_رستو, *extensions, versioned_ڳڻپ) {
            ret |= rxkb_context_include_رستو_append(ctx, *ext_رستو);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", رستو, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#جيڪڏهن_پورو

bool
rxkb_context_include_رستو_append_default(بنية rxkb_context *ctx)
{
    حرف user_رستو[PATH_MAX];
    bool ret = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "include paths can only be appended to a new context\n");
        أرجع false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        إذا (snprintf_محفوظ(user_رستو, حجم(user_رستو), "%s/xkb", xdg))
            ret |= rxkb_context_include_رستو_append(ctx, user_رستو);
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        إذا (snprintf_محفوظ(user_رستو, حجم(user_رستو), "%s/.config/xkb", home))
            ret |= rxkb_context_include_رستو_append(ctx, user_رستو);
    }

    إذا (home != عدم) {
        إذا (snprintf_محفوظ(user_رستو, حجم(user_رستو), "%s/.xkb", home))
            ret |= rxkb_context_include_رستو_append(ctx, user_رستو);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_رستو_append(
        ctx, ((extra != عدم) ? extra : DFLT_XKB_CONFIG_EXTRA_رستو)
    );

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_رستو =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#جيڪڏهن_وصف_ٿيل DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_رستو
    إذا (extensions_رستو == عدم)
        extensions_رستو = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_رستو;
#جيڪڏهن_پورو
    سائيز_t versioned_رستو_ڊيگهائي = 0;
    إذا (extensions_رستو != عدم) {
        /* Add direct subdirectories */
        ret |= جوڙيو_سڌو_subdirectories(ctx, extensions_رستو,
                                         &extensions, 0, 0);
        versioned_رستو_ڊيگهائي = لڙي_ڊيگهه(extensions_رستو);
    }

    /* Unversioned extensions directory */
    extensions_رستو =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#جيڪڏهن_وصف_ٿيل DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_رستو
    إذا (extensions_رستو == عدم)
        extensions_رستو = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_رستو;
#جيڪڏهن_پورو
    إذا (extensions_رستو != عدم) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جوڙيو_سڌو_subdirectories(ctx, extensions_رستو, &extensions,
                                         darray_سائيز(extensions),
                                         versioned_رستو_ڊيگهائي);
    }

    حرف **ext_رستو;
    darray_foreach(ext_رستو, extensions) {
        حرر(*ext_رستو);
    }
    darray_free(extensions);
#جيڪڏهن_پورو

    /* Canonical XKB root */
    ثابت حرف * ثابت پاڙ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_پاڙ = rxkb_context_include_رستو_append(
        ctx, ((پاڙ != عدم) ? پاڙ: DFLT_XKB_CONFIG_پاڙ)
    );
    ret |= has_پاڙ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    إذا (!has_پاڙ && (پاڙ == عدم || پاڙ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_پيغام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_پاڙ,
                 ((پاڙ == عدم) ? DFLT_XKB_CONFIG_پاڙ : پاڙ));
        ret |= rxkb_context_include_رستو_append(ctx, DFLT_XKB_LEGACY_پاڙ);
    }

    أرجع ret;
}

bool
rxkb_context_parse_default_ruleset(بنية rxkb_context *ctx)
{
    أرجع rxkb_context_parse(ctx, DEFAULT_XKB_قاعدا);
}

bool
rxkb_context_parse(بنية rxkb_context *ctx, ثابت حرف *ruleset)
{
    حرف **رستو;
    bool success = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "parse must only be called on a new context\n");
        أرجع false;
    }

    darray_foreach_ابتو(رستو, ctx->includes) {
        حرف قاعدا[PATH_MAX];

        إذا (snprintf_محفوظ(قاعدا, حجم(قاعدا), "%s/rules/%s.xml",
                           *رستو, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قاعدا);
            إذا (parse(ctx, قاعدا, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        إذا (ctx->ڀريو_extra_قاعدا_files &&
            snprintf_محفوظ(قاعدا, حجم(قاعدا), "%s/rules/%s.extras.xml",
                          *رستو, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قاعدا);
            إذا (parse(ctx, قاعدا, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    أرجع success;
}


فراغ
rxkb_context_سيٽ_user_معلومات(بنية rxkb_context *ctx, فراغ *userdata)
{
    ctx->userdata = userdata;
}

فراغ *
rxkb_context_get_user_معلومات(بنية rxkb_context *ctx)
{
    أرجع ctx->userdata;
}

ساكن مضمن bool
is_ڳنڍ(xml_ڳنڍ *ڳنڍ, ثابت حرف *name)
{
    أرجع ڳنڍ->type == XML_ELEMENT_ڳنڍ &&
        xmlStrEqual(ڳنڍ->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساكن حرف *
ڪڍو_متن(xml_ڳنڍ *ڳنڍ)
{
    xml_ڳنڍ *n;

    لكل (n = ڳنڍ->children; n; n = n->ايندڙ) {
        إذا (n->type == XML_متن_ڳنڍ)
            أرجع (حرف *)xmlStrdup(n->content);
    }
    أرجع عدم;
}

/* Data from “configItem” node */
بنية config_شي {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

#وصف_ڪريو config_شي_new(popularity_) { \
    .name = عدم, \
    .description = عدم, \
    .brief = عدم, \
    .vendor = عدم, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساكن فراغ
config_شي_free(بنية config_شي *config) {
    حرر(config->name);
    حرر(config->description);
    حرر(config->brief);
    حرر(config->vendor);
}

ساكن bool
parse_config_شي(بنية rxkb_context *ctx, xml_ڳنڍ *والد,
                  بنية config_شي *config)
{
    xml_ڳنڍ *ڳنڍ = عدم;
    xml_ڳنڍ *ci = عدم;

    لكل (ci = والد->children; ci; ci = ci->ايندڙ) {
        إذا (is_ڳنڍ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            إذا (raw_popularity) {
                إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وإلا إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وإلا
                    log_err(ctx, XKB_LOG_پيغام_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->سٽ, raw_popularity);
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
            لكل (ڳنڍ = ci->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
                إذا (is_ڳنڍ(ڳنڍ, "name"))
                    config->name = ڪڍو_متن(ڳنڍ);
                وإلا إذا (is_ڳنڍ(ڳنڍ, "description"))
                    config->description = ڪڍو_متن(ڳنڍ);
                وإلا إذا (is_ڳنڍ(ڳنڍ, "shortDescription"))
                    config->brief = ڪڍو_متن(ڳنڍ);
                وإلا إذا (is_ڳنڍ(ڳنڍ, "vendor"))
                    config->vendor = ڪڍو_متن(ڳنڍ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            إذا (!config->name || !لڙي_ڊيگهه(config->name))  {
                log_err(ctx, XKB_LOG_پيغام_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->سٽ);
                config_شي_free(config);
                أرجع false;
            }

            أرجع true; /* only one configItem allowed in the dtd */
        }
    }

    أرجع false;
}

ساكن فراغ
parse_model(بنية rxkb_context *ctx, xml_ڳنڍ *model,
            تعداد rxkb_popularity popularity)
{
    بنية config_شي config = config_شي_new(popularity);

    إذا (parse_config_شي(ctx, model, &config)) {
        بنية rxkb_model *m;

        list_for_each(m, &ctx->models, بنياد.link) {
            إذا (streq(m->name, config.name)) {
                config_شي_free(&config);
                أرجع;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->بنياد);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->بنياد.link);
    }
}

ساكن فراغ
parse_model_list(بنية rxkb_context *ctx, xml_ڳنڍ *model_list,
                 تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ڳنڍ = عدم;

    لكل (ڳنڍ = model_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "model"))
            parse_model(ctx, ڳنڍ, popularity);
    }
}

ساكن فراغ
parse_language_list(xml_ڳنڍ *language_list, بنية rxkb_layout *layout)
{
    xml_ڳنڍ *ڳنڍ = عدم;
    بنية rxkb_iso639_ڪوڊ *ڪوڊ;

    لكل (ڳنڍ = language_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "iso639Id")) {
            حرف *str = ڪڍو_متن(ڳنڍ);
            بنية rxkb_object *والد;

            إذا (!str || لڙي_ڊيگهه(str) != 3) {
                حرر(str);
                استمر;
            }

            والد = &layout->بنياد;
            ڪوڊ = rxkb_iso639_ڪوڊ_create(والد);
            ڪوڊ->ڪوڊ = str;
            list_append(&layout->iso639s, &ڪوڊ->بنياد.link);
        }
    }
}

ساكن فراغ
parse_country_list(xml_ڳنڍ *country_list, بنية rxkb_layout *layout)
{
    xml_ڳنڍ *ڳنڍ = عدم;
    بنية rxkb_iso3166_ڪوڊ *ڪوڊ;

    لكل (ڳنڍ = country_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "iso3166Id")) {
            حرف *str = ڪڍو_متن(ڳنڍ);
            بنية rxkb_object *والد;

            إذا (!str || لڙي_ڊيگهه(str) != 2) {
                حرر(str);
                استمر;
            }

            والد = &layout->بنياد;
            ڪوڊ = rxkb_iso3166_ڪوڊ_create(والد);
            ڪوڊ->ڪوڊ = str;
            list_append(&layout->iso3166s, &ڪوڊ->بنياد.link);
        }
    }
}

ساكن فراغ
parse_variant(بنية rxkb_context *ctx, بنية rxkb_layout *l,
              xml_ڳنڍ *variant, تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ci;
    بنية config_شي config = config_شي_new(popularity);

    إذا (parse_config_شي(ctx, variant, &config)) {
        بنية rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, بنياد.link) {
            إذا (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                اكسر;
            }
        }

        إذا (!exists) {
            v = rxkb_layout_create(&ctx->بنياد);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == عدم ? strdup_محفوظ(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->بنياد.link);

            لكل (ci = variant->children; ci; ci = ci->ايندڙ) {
                xml_ڳنڍ *ڳنڍ;

                إذا (!is_ڳنڍ(ci, "configItem"))
                    استمر;

                bool مليو_language_list = false;
                bool مليو_country_list = false;
                لكل (ڳنڍ = ci->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
                    إذا (is_ڳنڍ(ڳنڍ, "languageList")) {
                        parse_language_list(ڳنڍ, v);
                        مليو_language_list = true;
                    }
                    إذا (is_ڳنڍ(ڳنڍ, "countryList")) {
                        parse_country_list(ڳنڍ, v);
                        مليو_country_list = true;
                    }
                }
                إذا (!مليو_language_list) {
                    // inherit from parent layout
                    بنية rxkb_iso639_ڪوڊ* x;
                    list_for_each(x, &l->iso639s, بنياد.link) {
                        بنية rxkb_iso639_ڪوڊ* ڪوڊ = rxkb_iso639_ڪوڊ_create(&v->بنياد);
                        ڪوڊ->ڪوڊ = strdup(x->ڪوڊ);
                        list_append(&v->iso639s, &ڪوڊ->بنياد.link);
                    }
                }
                إذا (!مليو_country_list) {
                    // inherit from parent layout
                    بنية rxkb_iso3166_ڪوڊ* x;
                    list_for_each(x, &l->iso3166s, بنياد.link) {
                        بنية rxkb_iso3166_ڪوڊ* ڪوڊ = rxkb_iso3166_ڪوڊ_create(&v->بنياد);
                        ڪوڊ->ڪوڊ = strdup(x->ڪوڊ);
                        list_append(&v->iso3166s, &ڪوڊ->بنياد.link);
                    }
                }
            }
        } وإلا {
            config_شي_free(&config);
        }
    }
}

ساكن فراغ
parse_variant_list(بنية rxkb_context *ctx, بنية rxkb_layout *l,
                   xml_ڳنڍ *variant_list, تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ڳنڍ = عدم;

    لكل (ڳنڍ = variant_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "variant"))
            parse_variant(ctx, l, ڳنڍ, popularity);
    }
}

ساكن فراغ
parse_layout(بنية rxkb_context *ctx, xml_ڳنڍ *layout,
             تعداد rxkb_popularity popularity)
{
    بنية config_شي config = config_شي_new(popularity);
    بنية rxkb_layout *l;
    xml_ڳنڍ *ڳنڍ = عدم;
    bool exists = false;

    إذا (!parse_config_شي(ctx, layout, &config))
        أرجع;

    list_for_each(l, &ctx->layouts, بنياد.link) {
        إذا (streq(l->name, config.name) && l->variant == عدم) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        l = rxkb_layout_create(&ctx->بنياد);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = عدم;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->بنياد.link);
    } وإلا {
        config_شي_free(&config);
    }

    لكل (ڳنڍ = layout->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "variantList")) {
            parse_variant_list(ctx, l, ڳنڍ, popularity);
        }
        إذا (!exists && is_ڳنڍ(ڳنڍ, "configItem")) {
            xml_ڳنڍ *ll;
            لكل (ll = ڳنڍ->children; ll; ll = ll->ايندڙ) {
                إذا (is_ڳنڍ(ll, "languageList"))
                    parse_language_list(ll, l);
                إذا (is_ڳنڍ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساكن فراغ
parse_layout_list(بنية rxkb_context *ctx, xml_ڳنڍ *layout_list,
                  تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ڳنڍ = عدم;

    لكل (ڳنڍ = layout_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "layout"))
            parse_layout(ctx, ڳنڍ, popularity);
    }
}

ساكن فراغ
parse_option(بنية rxkb_context *ctx, بنية rxkb_option_گروهه *گروهه,
             xml_ڳنڍ *option, تعداد rxkb_popularity popularity)
{
    بنية config_شي config = config_شي_new(popularity);

    إذا (parse_config_شي(ctx, option, &config)) {
        بنية rxkb_option *o;

        list_for_each(o, &گروهه->options, بنياد.link) {
            إذا (streq(o->name, config.name)) {
                config_شي_free(&config);
                أرجع;
            }
        }

        o = rxkb_option_create(&گروهه->بنياد);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&گروهه->options, &o->بنياد.link);
    }
}

ساكن فراغ
parse_گروهه(بنية rxkb_context *ctx, xml_ڳنڍ *گروهه,
            تعداد rxkb_popularity popularity)
{
    بنية config_شي config = config_شي_new(popularity);
    بنية rxkb_option_گروهه *g;
    xml_ڳنڍ *ڳنڍ = عدم;
    xmlChar *گڻو;
    bool exists = false;

    إذا (!parse_config_شي(ctx, گروهه, &config))
        أرجع;

    list_for_each(g, &ctx->option_groups, بنياد.link) {
        إذا (streq(g->name, config.name)) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        g = rxkb_option_گروهه_create(&ctx->بنياد);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        گڻو = xmlGetProp(گروهه, (ثابت xmlChar*)"allowMultipleSelection");
        إذا (گڻو && xmlStrEqual(گڻو, (ثابت xmlChar*)"true"))
            g->allow_گڻو = true;
        xmlFree(گڻو);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->بنياد.link);
    } وإلا {
        config_شي_free(&config);
    }

    لكل (ڳنڍ = گروهه->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "option"))
            parse_option(ctx, g, ڳنڍ, popularity);
    }
}

ساكن فراغ
parse_option_list(بنية rxkb_context *ctx, xml_ڳنڍ *option_list,
                  تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ڳنڍ = عدم;

    لكل (ڳنڍ = option_list->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "group"))
            parse_گروهه(ctx, ڳنڍ, popularity);
    }
}

ساكن فراغ
parse_قاعدا_xml(بنية rxkb_context *ctx, xml_ڳنڍ *پاڙ,
                تعداد rxkb_popularity popularity)
{
    xml_ڳنڍ *ڳنڍ = عدم;

    لكل (ڳنڍ = پاڙ->children; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ) {
        إذا (is_ڳنڍ(ڳنڍ, "modelList"))
            parse_model_list(ctx, ڳنڍ, popularity);
        وإلا إذا (is_ڳنڍ(ڳنڍ, "layoutList"))
            parse_layout_list(ctx, ڳنڍ, popularity);
        وإلا إذا (is_ڳنڍ(ڳنڍ, "optionList"))
            parse_option_list(ctx, ڳنڍ, popularity);
    }
}

ساكن فراغ
ATTR_PRINTF(2, 0)
xml_error_func(فراغ *ctx, ثابت حرف *نياپو, ...)
{
    ساكن حرف buf[PATH_MAX];
    ساكن سائيز_t slen = 0;
    va_list دليلون;
    صحيح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_شروعات(دليلون, نياپو);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, نياپو, دليلون);
    va_پڇاڙي(دليلون);

    /* This shouldn't really happen */
    إذا (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, حجم(buf));
        أرجع;
    }

    slen += (سائيز_t)rc;
    إذا (slen >= حجم(buf)) {
        /* truncated, let's flush this */
        buf[حجم(buf) - 1] = '\n';
        slen = حجم(buf);
    }

    /* We're assuming here that the last character is \n. */
    إذا (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_پيغام_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#إذا HAVE_XML_CTXT_سيٽ_ERRORHANDLER
ساكن فراغ
xml_structured_error_func(فراغ *user_معلومات_2, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_معلومات_2);
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل XML_PARSE_NO_XXE
#وصف_ڪريو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#وصف_ڪريو _XML_OPTIONS (XML_PARSE_NONET)
#جيڪڏهن_پورو

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
    static_assert(HAVE_XML_CTXT_سيٽ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_سيٽ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_سيٽ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_داخلا_Ptr pinput =
        xml_New_داخلا_From_يادگيري(عدم, dtdstr, ARRAY_سائيز(dtdstr) - 1,
                              XML_داخلا_BUF_STATIC);
    إذا (!pinput)
        اذهب dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, عدم, عدم);
#وإلا
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_داخلا_بفر_Ptr buf =
        xml_Parser_داخلا_بفر_Create_Mem(dtdstr, ARRAY_سائيز(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    إذا (!buf)
        اذهب dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(عدم, buf, XML_CHAR_ENCODING_UTF8);
#جيڪڏهن_پورو

    إذا (!dtd) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID, "Failed to load DTD\n");
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
#جيڪڏهن_پورو
    xmlFreeDtd(dtd);

dtd_error:
#إذا HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#جيڪڏهن_پورو
    أرجع success;
}

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *رستو,
      تعداد rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = عدم;
    xml_ڳنڍ *پاڙ = عدم;

    إذا (!check_eaccess(رستو, R_OK))
        أرجع false;

    LIBXML_TEST_نسخو

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;

#جيڪڏهن_وصف_ٿيل XML_PARSE_NO_XXE
#وصف_ڪريو _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#وصف_ڪريو _XML_OPTIONS (XML_PARSE_NONET)
#جيڪڏهن_پورو
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#إذا HAVE_XML_CTXT_سيٽ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_سيٽ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#جيڪڏهن_پورو
#إذا !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_سيٽ_Generic_Error_Func(ctx, xml_error_func);
#جيڪڏهن_پورو

    doc = xmlCtxtReadFile(xmlCtxt, رستو, عدم, 0);
    إذا (!doc)
        اذهب parse_error;

    إذا (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "XML error: failed to validate document at %s\n", رستو);
        اذهب validate_error;
    }

    پاڙ = xml_Doc_Get_پاڙ_Element(doc);
    parse_قاعدا_xml(ctx, پاڙ, popularity);

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
    xml_سيٽ_Generic_Error_Func(عدم, عدم);
#جيڪڏهن_پورو
#إذا HAVE_XML_CTXT_سيٽ_ERRORHANDLER
    xml_Ctxt_سيٽ_Error_Handler(xmlCtxt, عدم, عدم);
#جيڪڏهن_پورو
    xmlFreeParserCtxt(xmlCtxt);

    أرجع success;
}
