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
    #پایان_اگر
#پایان_اگر

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

بنية rxkb_iso639_کد {
    بنية rxkb_object پایه;
    حرف *کد;
};

بنية rxkb_iso3166_کد {
    بنية rxkb_object پایه;
    حرف *کد;
};

تعداد context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

بنية rxkb_context {
    بنية rxkb_object پایه;
    تعداد context_state context_state;

    bool بارگیری_extra_قواعد_files;
    bool use_secure_getenv;

    بنية list models;         /* list of struct rxkb_models */
    بنية list layouts;        /* list of struct rxkb_layouts */
    بنية list option_groups;  /* list of struct rxkb_option_group */

    darray(حرف *) includes;


    ATTR_PRINTF(3, 0) فراغ (*log_fn)(بنية rxkb_context *ctx,
                                     تعداد rxkb_log_level level,
                                     ثابت حرف *fmt, va_list آرگومانها);
    تعداد rxkb_log_level log_level;

    فراغ *userdata;
};

بنية rxkb_model {
    بنية rxkb_object پایه;

    حرف *name;
    حرف *vendor;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_layout {
    بنية rxkb_object پایه;

    حرف *name;
    حرف *brief;
    حرف *description;
    حرف *variant;
    تعداد rxkb_popularity popularity;

    بنية list iso639s;  /* list of struct rxkb_iso639_code */
    بنية list iso3166s; /* list of struct rxkb_iso3166_code */
};

بنية rxkb_option_گروه {
    بنية rxkb_object پایه;

    bool allow_مضرب;
    بنية list options; /* list of struct rxkb_options */
    حرف *name;
    حرف *description;
    تعداد rxkb_popularity popularity;
};

بنية rxkb_option {
    بنية rxkb_object پایه;

    حرف *name;
    حرف *brief;
    حرف *description;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *مسیر,
      تعداد rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ساكن فراغ
rxkb_log(بنية rxkb_context *ctx, تعداد rxkb_log_level level,
         ثابت حرف *fmt, ...)
{
    va_list آرگومانها;

    إذا (ctx->log_level < level)
        أرجع;

    va_شروع(آرگومانها, fmt);
    ctx->log_fn(ctx, level, fmt, آرگومانها);
    va_انتها(آرگومانها);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#تعریف_کن rxkb_log_with_کد(ctx, level, پیام_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_پیام_ID(پیام_id, fmt), ##__VA_ARGS__)
#تعریف_کن log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#تعریف_کن log_info(ctx, id, ...) \
    rxkb_log_with_کد((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#تعریف_کن log_warn(ctx, id, ...) \
    rxkb_log_with_کد((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#تعریف_کن log_err(ctx, id, ...) \
    rxkb_log_with_کد((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#تعریف_کن log_wsgo(ctx, id, ...) \
    rxkb_log_with_کد((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#تعریف_کن DECLARE_REF_UNREF_FOR_TYPE(type_) \
بنية type_ * type_##_ref(بنية type_ *object) { \
    rxkb_object_ref(&object->پایه); \
    أرجع object; \
} \
بنية type_ * type_##_unref(بنية type_ *object) { \
    إذا (!object) أرجع عدم; \
    أكد(object->پایه.refcount >= 1); \
    إذا (--object->پایه.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->پایه.link);\
        حرر(object); \
    } \
    أرجع عدم;\
}

#تعریف_کن DECLARE_CREATE_FOR_TYPE(type_) \
ساكن مضمن بنية type_ * type_##_create(بنية rxkb_object *والد) { \
    بنية type_ *t = حافظه_صفر_بده(1, حجم *t); \
    إذا (t) \
        rxkb_object_init(&t->پایه, والد); \
    أرجع t; \
}

#تعریف_کن DECLARE_TYPED_GETTER_FOR_TYPE(type_, فیلد, rtype_) \
rtype_ type_##_get_##field_(بنية type_ *object) { \
    أرجع object->فیلد; \
}

#تعریف_کن DECLARE_GETTER_FOR_TYPE(type_, فیلد) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, فیلد, ثابت حرف*)

#تعریف_کن DECLARE_اولی_بعدی_FOR_TYPE(type_, والد_type, والد_فیلد) \
بنية type_ * type_##_first(بنية والد_type *والد) { \
    بنية type_ *o = عدم; \
    إذا (!list_خالی(&والد->والد_فیلد)) \
        o = list_اولی_مدخل(&والد->والد_فیلد, o, پایه.link); \
    أرجع o; \
} \
بنية type_ * \
type_##_next(بنية type_ *o) \
{ \
    بنية والد_type *والد; \
    بنية type_ *بعدی; \
    والد = container_از(o->پایه.والد, بنية والد_type, پایه); \
    بعدی = list_اولی_مدخل(&o->پایه.link, o, پایه.link); \
    إذا (list_is_آخرین(&والد->والد_فیلد, &o->پایه.link)) \
        أرجع عدم; \
    أرجع بعدی; \
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
rxkb_iso639_کد_destroy(بنية rxkb_iso639_کد *کد)
{
    حرر(کد->کد);
}

بنية rxkb_iso639_کد *
rxkb_layout_get_iso639_اولی(بنية rxkb_layout *layout)
{
    بنية rxkb_iso639_کد *کد = عدم;

    إذا (!list_خالی(&layout->iso639s))
        کد = list_اولی_مدخل(&layout->iso639s, کد, پایه.link);

    أرجع کد;
}

بنية rxkb_iso639_کد *
rxkb_iso639_کد_بعدی(بنية rxkb_iso639_کد *کد)
{
    بنية rxkb_iso639_کد *بعدی = عدم;
    بنية rxkb_layout *layout;

    layout = container_از(کد->پایه.والد, بنية rxkb_layout, پایه);

    إذا (list_is_آخرین(&layout->iso639s, &کد->پایه.link))
        أرجع عدم;

    بعدی = list_اولی_مدخل(&کد->پایه.link, کد, پایه.link);

    أرجع بعدی;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_کد);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_کد);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_کد, کد);

ساكن فراغ
rxkb_iso3166_کد_destroy(بنية rxkb_iso3166_کد *کد)
{
    حرر(کد->کد);
}

بنية rxkb_iso3166_کد *
rxkb_layout_get_iso3166_اولی(بنية rxkb_layout *layout)
{
    بنية rxkb_iso3166_کد *کد = عدم;

    إذا (!list_خالی(&layout->iso3166s))
        کد = list_اولی_مدخل(&layout->iso3166s, کد, پایه.link);

    أرجع کد;
}

بنية rxkb_iso3166_کد *
rxkb_iso3166_کد_بعدی(بنية rxkb_iso3166_کد *کد)
{
    بنية rxkb_iso3166_کد *بعدی = عدم;
    بنية rxkb_layout *layout;

    layout = container_از(کد->پایه.والد, بنية rxkb_layout, پایه);

    إذا (list_is_آخرین(&layout->iso3166s, &کد->پایه.link))
        أرجع عدم;

    بعدی = list_اولی_مدخل(&کد->پایه.link, کد, پایه.link);

    أرجع بعدی;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_کد);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_کد);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_کد, کد);

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
DECLARE_اولی_بعدی_FOR_TYPE(rxkb_option, rxkb_option_گروه, options);

ساكن فراغ
rxkb_layout_destroy(بنية rxkb_layout *l)
{
    بنية rxkb_iso639_کد *iso639, *tmp_639;
    بنية rxkb_iso3166_کد *iso3166, *tmp_3166;

    حرر(l->name);
    حرر(l->brief);
    حرر(l->description);
    حرر(l->variant);

    list_for_each_امن(iso639, tmp_639, &l->iso639s, پایه.link) {
        rxkb_iso639_کد_unref(iso639);
    }
    list_for_each_امن(iso3166, tmp_3166, &l->iso3166s, پایه.link) {
        rxkb_iso3166_کد_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, تعداد rxkb_popularity);
DECLARE_اولی_بعدی_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

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
DECLARE_اولی_بعدی_FOR_TYPE(rxkb_model, rxkb_context, models);

ساكن فراغ
rxkb_option_گروه_destroy(بنية rxkb_option_گروه *og)
{
    بنية rxkb_option *o, *otmp;

    حرر(og->name);
    حرر(og->description);

    list_for_each_امن(o, otmp, &og->options, پایه.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_گروه_allows_مضرب(بنية rxkb_option_گروه *g)
{
    أرجع g->allow_مضرب;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_گروه);
DECLARE_CREATE_FOR_TYPE(rxkb_option_گروه);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروه, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_گروه, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_گروه, popularity, تعداد rxkb_popularity);
DECLARE_اولی_بعدی_FOR_TYPE(rxkb_option_گروه, rxkb_context, option_groups);

ساكن فراغ
rxkb_context_destroy(بنية rxkb_context *ctx)
{
    بنية rxkb_model *m, *mtmp;
    بنية rxkb_layout *l, *ltmp;
    بنية rxkb_option_گروه *og, *ogtmp;
    حرف **مسیر;

    list_for_each_امن(m, mtmp, &ctx->models, پایه.link)
        rxkb_model_unref(m);
    أكد(list_خالی(&ctx->models));

    list_for_each_امن(l, ltmp, &ctx->layouts, پایه.link)
        rxkb_layout_unref(l);
    أكد(list_خالی(&ctx->layouts));

    list_for_each_امن(og, ogtmp, &ctx->option_groups, پایه.link)
        rxkb_option_گروه_unref(og);
    أكد(list_خالی(&ctx->option_groups));

    darray_foreach(مسیر, ctx->includes)
        حرر(*مسیر);
    darray_free(ctx->includes);

    أكد(darray_خالی(ctx->includes));
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
rxkb_context_مجموعه_log_level(بنية rxkb_context *ctx,
                           تعداد rxkb_log_level level)
{
    ctx->log_level = level;
}

ساكن ثابت حرف *
log_level_مقصد_پیشوند(تعداد rxkb_log_level level)
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
               ثابت حرف *fmt, va_list آرگومانها)
{
    ثابت حرف *پیشوند = log_level_مقصد_پیشوند(level);

    إذا (پیشوند)
        fprintf(stderr, "%s", پیشوند);
    vfprintf(stderr, fmt, آرگومانها);
}

ساكن تعداد rxkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr;
    ثابت دراز lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد rxkb_log_level)lvl;

    إذا (istreq_پیشوند("crit", level))
        أرجع RXKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_پیشوند("err", level))
        أرجع RXKB_LOG_LEVEL_ERROR;
    إذا (istreq_پیشوند("warn", level))
        أرجع RXKB_LOG_LEVEL_WARNING;
    إذا (istreq_پیشوند("info", level))
        أرجع RXKB_LOG_LEVEL_INFO;
    إذا (istreq_پیشوند("debug", level) || istreq_پیشوند("dbg", level))
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
    ctx->بارگیری_extra_قواعد_files = flags & RXKB_CONTEXT_بارگیری_EXOTIC_قواعد;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    إذا (env)
        rxkb_context_مجموعه_log_level(ctx, log_level(env));

    ساكن ثابت تعداد rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_بارگیری_EXOTIC_قواعد
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    إذا (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        حرر(ctx);
        أرجع عدم;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    إذا (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_مسیر_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_مسیر,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ریشه);
        rxkb_context_unref(ctx);
        أرجع عدم;
    }

    أرجع ctx;
}

فراغ
rxkb_context_مجموعه_log_fn(بنية rxkb_context *ctx,
                        فراغ (*log_fn)(بنية rxkb_context *ctx,
                                       تعداد rxkb_log_level level,
                                       ثابت حرف *fmt, va_list آرگومانها))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_مسیر_append(بنية rxkb_context *ctx, ثابت حرف *مسیر)
{
    صحيح err = 0;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "include paths can only be appended to a new context\n");
        اذهب error;
    }

    بنية stat stat_buf;
    err = stat(مسیر, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب error;
    }
    إذا (!S_ISDIR(stat_buf.st_نما)) {
        err = ENOTDIR;
        اذهب error;
    }

    إذا (!check_eaccess(مسیر, R_OK | X_OK)) {
        err = EACCES;
        اذهب error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    حرف قواعد[PATH_MAX];
    إذا (!snprintf_امن(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                       مسیر, DEFAULT_XKB_قواعد)) {
        log_err(ctx, XKB_ERROR_INVALID_مسیر,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                حجم(قواعد), مسیر, DEFAULT_XKB_قواعد);
        اذهب error;
    }

    حرف * ثابت tmp = strdup(مسیر);
    إذا (!tmp) {
        err = ENOMEM;
        اذهب error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پیام_NO_ID, "Include path added: %s\n", tmp);

    أرجع true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پیام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", مسیر, strerror(err));
    أرجع false;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
جمع_کن_مستقیم_subdirectories(بنية rxkb_context *ctx, ثابت حرف *مسیر,
                          darray_string *extensions,
                          darray_اندازه_t versioned_شمار,
                          اندازه_t versioned_مسیر_درازی)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(مسیر, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_نما)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(مسیر, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(مسیر);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *مدخل;
    حرف مسیر_buf[PATH_MAX] = "";
    versioned_مسیر_درازی++; /* Additional final ‘/’ */
    طالما ((مدخل = readdir(dir)) != عدم) {
        ثابت حرف *name = مدخل->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_امن(مسیر_buf, حجم(مسیر_buf), "%s/%s", مسیر, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(مسیر_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_نما))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_اندازه_t i = 0; i < versioned_شمار; i++) {
            ثابت حرف * ثابت prev_name = darray_قلم(*extensions, i)
                                         + versioned_مسیر_درازی;
            إذا (strcmp(name, prev_name) == 0)
                اذهب بعدی;
        }

        حرف *ext_مسیر = strdup_امن(مسیر_buf);
        إذا (!ext_مسیر) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_مسیر);
بعدی:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_اندازه(*extensions) > versioned_شمار) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اقلام(*extensions) + versioned_شمار,
              darray_اندازه(*extensions) - versioned_شمار,
              حجم(*darray_اقلام(*extensions)), &compare_str);
        حرف **ext_مسیر;
        darray_foreach_from(ext_مسیر, *extensions, versioned_شمار) {
            ret |= rxkb_context_include_مسیر_append(ctx, *ext_مسیر);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", مسیر, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#پایان_اگر

bool
rxkb_context_include_مسیر_append_default(بنية rxkb_context *ctx)
{
    حرف user_مسیر[PATH_MAX];
    bool ret = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "include paths can only be appended to a new context\n");
        أرجع false;
    }

    ثابت حرف * ثابت home = rxkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        إذا (snprintf_امن(user_مسیر, حجم(user_مسیر), "%s/xkb", xdg))
            ret |= rxkb_context_include_مسیر_append(ctx, user_مسیر);
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        إذا (snprintf_امن(user_مسیر, حجم(user_مسیر), "%s/.config/xkb", home))
            ret |= rxkb_context_include_مسیر_append(ctx, user_مسیر);
    }

    إذا (home != عدم) {
        إذا (snprintf_امن(user_مسیر, حجم(user_مسیر), "%s/.xkb", home))
            ret |= rxkb_context_include_مسیر_append(ctx, user_مسیر);
    }

    ثابت حرف * ثابت extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_مسیر_append(
        ctx, ((extra != عدم) ? extra : DFLT_XKB_CONFIG_EXTRA_مسیر)
    );

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_مسیر =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#اگر_تعریف_شده DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_مسیر
    إذا (extensions_مسیر == عدم)
        extensions_مسیر = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_مسیر;
#پایان_اگر
    اندازه_t versioned_مسیر_درازی = 0;
    إذا (extensions_مسیر != عدم) {
        /* Add direct subdirectories */
        ret |= جمع_کن_مستقیم_subdirectories(ctx, extensions_مسیر,
                                         &extensions, 0, 0);
        versioned_مسیر_درازی = طول_رشته(extensions_مسیر);
    }

    /* Unversioned extensions directory */
    extensions_مسیر =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#اگر_تعریف_شده DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_مسیر
    إذا (extensions_مسیر == عدم)
        extensions_مسیر = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_مسیر;
#پایان_اگر
    إذا (extensions_مسیر != عدم) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جمع_کن_مستقیم_subdirectories(ctx, extensions_مسیر, &extensions,
                                         darray_اندازه(extensions),
                                         versioned_مسیر_درازی);
    }

    حرف **ext_مسیر;
    darray_foreach(ext_مسیر, extensions) {
        حرر(*ext_مسیر);
    }
    darray_free(extensions);
#پایان_اگر

    /* Canonical XKB root */
    ثابت حرف * ثابت ریشه = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ثابت bool has_ریشه = rxkb_context_include_مسیر_append(
        ctx, ((ریشه != عدم) ? ریشه: DFLT_XKB_CONFIG_ریشه)
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
    إذا (!has_ریشه && (ریشه == عدم || ریشه[0] != '\0')) {
        log_warn(ctx, XKB_LOG_پیام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ریشه,
                 ((ریشه == عدم) ? DFLT_XKB_CONFIG_ریشه : ریشه));
        ret |= rxkb_context_include_مسیر_append(ctx, DFLT_XKB_LEGACY_ریشه);
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
    حرف **مسیر;
    bool success = false;

    إذا (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "parse must only be called on a new context\n");
        أرجع false;
    }

    darray_foreach_معکوس(مسیر, ctx->includes) {
        حرف قواعد[PATH_MAX];

        إذا (snprintf_امن(قواعد, حجم(قواعد), "%s/rules/%s.xml",
                           *مسیر, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            إذا (parse(ctx, قواعد, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        إذا (ctx->بارگیری_extra_قواعد_files &&
            snprintf_امن(قواعد, حجم(قواعد), "%s/rules/%s.extras.xml",
                          *مسیر, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", قواعد);
            إذا (parse(ctx, قواعد, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    أرجع success;
}


فراغ
rxkb_context_مجموعه_user_داده(بنية rxkb_context *ctx, فراغ *userdata)
{
    ctx->userdata = userdata;
}

فراغ *
rxkb_context_get_user_داده(بنية rxkb_context *ctx)
{
    أرجع ctx->userdata;
}

ساكن مضمن bool
is_گره(xml_گره *گره, ثابت حرف *name)
{
    أرجع گره->type == XML_ELEMENT_گره &&
        xmlStrEqual(گره->name, (ثابت xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ساكن حرف *
استخراج_متن(xml_گره *گره)
{
    xml_گره *n;

    لكل (n = گره->children; n; n = n->بعدی) {
        إذا (n->type == XML_متن_گره)
            أرجع (حرف *)xmlStrdup(n->content);
    }
    أرجع عدم;
}

/* Data from “configItem” node */
بنية config_قلم {
    حرف *name;
    حرف *description;
    حرف *brief;
    حرف *vendor;
    تعداد rxkb_popularity popularity;
    bool layout_specific;
};

#تعریف_کن config_قلم_new(popularity_) { \
    .name = عدم, \
    .description = عدم, \
    .brief = عدم, \
    .vendor = عدم, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ساكن فراغ
config_قلم_free(بنية config_قلم *config) {
    حرر(config->name);
    حرر(config->description);
    حرر(config->brief);
    حرر(config->vendor);
}

ساكن bool
parse_config_قلم(بنية rxkb_context *ctx, xml_گره *والد,
                  بنية config_قلم *config)
{
    xml_گره *گره = عدم;
    xml_گره *ci = عدم;

    لكل (ci = والد->children; ci; ci = ci->بعدی) {
        إذا (is_گره(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ثابت xmlChar*)"popularity");
            إذا (raw_popularity) {
                إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                وإلا إذا (xmlStrEqual(raw_popularity, (ثابت xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                وإلا
                    log_err(ctx, XKB_LOG_پیام_NO_ID,
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
            لكل (گره = ci->children; گره; گره = گره->بعدی) {
                إذا (is_گره(گره, "name"))
                    config->name = استخراج_متن(گره);
                وإلا إذا (is_گره(گره, "description"))
                    config->description = استخراج_متن(گره);
                وإلا إذا (is_گره(گره, "shortDescription"))
                    config->brief = استخراج_متن(گره);
                وإلا إذا (is_گره(گره, "vendor"))
                    config->vendor = استخراج_متن(گره);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            إذا (!config->name || !طول_رشته(config->name))  {
                log_err(ctx, XKB_LOG_پیام_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->سطر);
                config_قلم_free(config);
                أرجع false;
            }

            أرجع true; /* only one configItem allowed in the dtd */
        }
    }

    أرجع false;
}

ساكن فراغ
parse_model(بنية rxkb_context *ctx, xml_گره *model,
            تعداد rxkb_popularity popularity)
{
    بنية config_قلم config = config_قلم_new(popularity);

    إذا (parse_config_قلم(ctx, model, &config)) {
        بنية rxkb_model *m;

        list_for_each(m, &ctx->models, پایه.link) {
            إذا (streq(m->name, config.name)) {
                config_قلم_free(&config);
                أرجع;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->پایه);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->پایه.link);
    }
}

ساكن فراغ
parse_model_list(بنية rxkb_context *ctx, xml_گره *model_list,
                 تعداد rxkb_popularity popularity)
{
    xml_گره *گره = عدم;

    لكل (گره = model_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "model"))
            parse_model(ctx, گره, popularity);
    }
}

ساكن فراغ
parse_language_list(xml_گره *language_list, بنية rxkb_layout *layout)
{
    xml_گره *گره = عدم;
    بنية rxkb_iso639_کد *کد;

    لكل (گره = language_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "iso639Id")) {
            حرف *str = استخراج_متن(گره);
            بنية rxkb_object *والد;

            إذا (!str || طول_رشته(str) != 3) {
                حرر(str);
                استمر;
            }

            والد = &layout->پایه;
            کد = rxkb_iso639_کد_create(والد);
            کد->کد = str;
            list_append(&layout->iso639s, &کد->پایه.link);
        }
    }
}

ساكن فراغ
parse_country_list(xml_گره *country_list, بنية rxkb_layout *layout)
{
    xml_گره *گره = عدم;
    بنية rxkb_iso3166_کد *کد;

    لكل (گره = country_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "iso3166Id")) {
            حرف *str = استخراج_متن(گره);
            بنية rxkb_object *والد;

            إذا (!str || طول_رشته(str) != 2) {
                حرر(str);
                استمر;
            }

            والد = &layout->پایه;
            کد = rxkb_iso3166_کد_create(والد);
            کد->کد = str;
            list_append(&layout->iso3166s, &کد->پایه.link);
        }
    }
}

ساكن فراغ
parse_variant(بنية rxkb_context *ctx, بنية rxkb_layout *l,
              xml_گره *variant, تعداد rxkb_popularity popularity)
{
    xml_گره *ci;
    بنية config_قلم config = config_قلم_new(popularity);

    إذا (parse_config_قلم(ctx, variant, &config)) {
        بنية rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, پایه.link) {
            إذا (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                اكسر;
            }
        }

        إذا (!exists) {
            v = rxkb_layout_create(&ctx->پایه);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == عدم ? strdup_امن(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->پایه.link);

            لكل (ci = variant->children; ci; ci = ci->بعدی) {
                xml_گره *گره;

                إذا (!is_گره(ci, "configItem"))
                    استمر;

                bool یافت_language_list = false;
                bool یافت_country_list = false;
                لكل (گره = ci->children; گره; گره = گره->بعدی) {
                    إذا (is_گره(گره, "languageList")) {
                        parse_language_list(گره, v);
                        یافت_language_list = true;
                    }
                    إذا (is_گره(گره, "countryList")) {
                        parse_country_list(گره, v);
                        یافت_country_list = true;
                    }
                }
                إذا (!یافت_language_list) {
                    // inherit from parent layout
                    بنية rxkb_iso639_کد* x;
                    list_for_each(x, &l->iso639s, پایه.link) {
                        بنية rxkb_iso639_کد* کد = rxkb_iso639_کد_create(&v->پایه);
                        کد->کد = strdup(x->کد);
                        list_append(&v->iso639s, &کد->پایه.link);
                    }
                }
                إذا (!یافت_country_list) {
                    // inherit from parent layout
                    بنية rxkb_iso3166_کد* x;
                    list_for_each(x, &l->iso3166s, پایه.link) {
                        بنية rxkb_iso3166_کد* کد = rxkb_iso3166_کد_create(&v->پایه);
                        کد->کد = strdup(x->کد);
                        list_append(&v->iso3166s, &کد->پایه.link);
                    }
                }
            }
        } وإلا {
            config_قلم_free(&config);
        }
    }
}

ساكن فراغ
parse_variant_list(بنية rxkb_context *ctx, بنية rxkb_layout *l,
                   xml_گره *variant_list, تعداد rxkb_popularity popularity)
{
    xml_گره *گره = عدم;

    لكل (گره = variant_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "variant"))
            parse_variant(ctx, l, گره, popularity);
    }
}

ساكن فراغ
parse_layout(بنية rxkb_context *ctx, xml_گره *layout,
             تعداد rxkb_popularity popularity)
{
    بنية config_قلم config = config_قلم_new(popularity);
    بنية rxkb_layout *l;
    xml_گره *گره = عدم;
    bool exists = false;

    إذا (!parse_config_قلم(ctx, layout, &config))
        أرجع;

    list_for_each(l, &ctx->layouts, پایه.link) {
        إذا (streq(l->name, config.name) && l->variant == عدم) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        l = rxkb_layout_create(&ctx->پایه);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = عدم;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->پایه.link);
    } وإلا {
        config_قلم_free(&config);
    }

    لكل (گره = layout->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "variantList")) {
            parse_variant_list(ctx, l, گره, popularity);
        }
        إذا (!exists && is_گره(گره, "configItem")) {
            xml_گره *ll;
            لكل (ll = گره->children; ll; ll = ll->بعدی) {
                إذا (is_گره(ll, "languageList"))
                    parse_language_list(ll, l);
                إذا (is_گره(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ساكن فراغ
parse_layout_list(بنية rxkb_context *ctx, xml_گره *layout_list,
                  تعداد rxkb_popularity popularity)
{
    xml_گره *گره = عدم;

    لكل (گره = layout_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "layout"))
            parse_layout(ctx, گره, popularity);
    }
}

ساكن فراغ
parse_option(بنية rxkb_context *ctx, بنية rxkb_option_گروه *گروه,
             xml_گره *option, تعداد rxkb_popularity popularity)
{
    بنية config_قلم config = config_قلم_new(popularity);

    إذا (parse_config_قلم(ctx, option, &config)) {
        بنية rxkb_option *o;

        list_for_each(o, &گروه->options, پایه.link) {
            إذا (streq(o->name, config.name)) {
                config_قلم_free(&config);
                أرجع;
            }
        }

        o = rxkb_option_create(&گروه->پایه);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&گروه->options, &o->پایه.link);
    }
}

ساكن فراغ
parse_گروه(بنية rxkb_context *ctx, xml_گره *گروه,
            تعداد rxkb_popularity popularity)
{
    بنية config_قلم config = config_قلم_new(popularity);
    بنية rxkb_option_گروه *g;
    xml_گره *گره = عدم;
    xmlChar *مضرب;
    bool exists = false;

    إذا (!parse_config_قلم(ctx, گروه, &config))
        أرجع;

    list_for_each(g, &ctx->option_groups, پایه.link) {
        إذا (streq(g->name, config.name)) {
            exists = true;
            اكسر;
        }
    }

    إذا (!exists) {
        g = rxkb_option_گروه_create(&ctx->پایه);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        مضرب = xmlGetProp(گروه, (ثابت xmlChar*)"allowMultipleSelection");
        إذا (مضرب && xmlStrEqual(مضرب, (ثابت xmlChar*)"true"))
            g->allow_مضرب = true;
        xmlFree(مضرب);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->پایه.link);
    } وإلا {
        config_قلم_free(&config);
    }

    لكل (گره = گروه->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "option"))
            parse_option(ctx, g, گره, popularity);
    }
}

ساكن فراغ
parse_option_list(بنية rxkb_context *ctx, xml_گره *option_list,
                  تعداد rxkb_popularity popularity)
{
    xml_گره *گره = عدم;

    لكل (گره = option_list->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "group"))
            parse_گروه(ctx, گره, popularity);
    }
}

ساكن فراغ
parse_قواعد_xml(بنية rxkb_context *ctx, xml_گره *ریشه,
                تعداد rxkb_popularity popularity)
{
    xml_گره *گره = عدم;

    لكل (گره = ریشه->children; گره; گره = گره->بعدی) {
        إذا (is_گره(گره, "modelList"))
            parse_model_list(ctx, گره, popularity);
        وإلا إذا (is_گره(گره, "layoutList"))
            parse_layout_list(ctx, گره, popularity);
        وإلا إذا (is_گره(گره, "optionList"))
            parse_option_list(ctx, گره, popularity);
    }
}

ساكن فراغ
ATTR_PRINTF(2, 0)
xml_error_func(فراغ *ctx, ثابت حرف *پیام, ...)
{
    ساكن حرف buf[PATH_MAX];
    ساكن اندازه_t slen = 0;
    va_list آرگومانها;
    صحيح rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_شروع(آرگومانها, پیام);
    rc = vsnprintf(&buf[slen], حجم(buf) - slen, پیام, آرگومانها);
    va_انتها(آرگومانها);

    /* This shouldn't really happen */
    إذا (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_میانگیر_اندازه,
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
        log_err(ctx, XKB_LOG_پیام_NO_ID, "%s", buf);
        memset(buf, 0, حجم(buf));
        slen = 0;
    }
}

#إذا HAVE_XML_CTXT_مجموعه_ERRORHANDLER
ساكن فراغ
xml_structured_error_func(فراغ *user_داده_2, ثابت xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_داده_2);
}
#پایان_اگر

#اگر_تعریف_شده XML_PARSE_NO_XXE
#تعریف_کن _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کن _XML_OPTIONS (XML_PARSE_NONET)
#پایان_اگر

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
    static_assert(HAVE_XML_CTXT_مجموعه_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_مجموعه_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_مجموعه_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_درون_داد_Ptr pinput =
        xml_New_درون_داد_From_حافظه(عدم, dtdstr, ARRAY_اندازه(dtdstr) - 1,
                              XML_درون_داد_BUF_STATIC);
    إذا (!pinput)
        اذهب dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, عدم, عدم);
#وإلا
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_درون_داد_میانگیر_Ptr buf =
        xml_Parser_درون_داد_میانگیر_Create_Mem(dtdstr, ARRAY_اندازه(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    إذا (!buf)
        اذهب dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(عدم, buf, XML_CHAR_ENCODING_UTF8);
#پایان_اگر

    إذا (!dtd) {
        log_err(ctx, XKB_LOG_پیام_NO_ID, "Failed to load DTD\n");
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
    dtdvalid->user_داده_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#پایان_اگر
    xmlFreeDtd(dtd);

dtd_error:
#إذا HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#پایان_اگر
    أرجع success;
}

ساكن bool
parse(بنية rxkb_context *ctx, ثابت حرف *مسیر,
      تعداد rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = عدم;
    xml_گره *ریشه = عدم;

    إذا (!check_eaccess(مسیر, R_OK))
        أرجع false;

    LIBXML_TEST_نسخه

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    إذا (!xmlCtxt)
        أرجع false;

#اگر_تعریف_شده XML_PARSE_NO_XXE
#تعریف_کن _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#وإلا
#تعریف_کن _XML_OPTIONS (XML_PARSE_NONET)
#پایان_اگر
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#إذا HAVE_XML_CTXT_مجموعه_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_مجموعه_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#پایان_اگر
#إذا !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_مجموعه_Generic_Error_Func(ctx, xml_error_func);
#پایان_اگر

    doc = xmlCtxtReadFile(xmlCtxt, مسیر, عدم, 0);
    إذا (!doc)
        اذهب parse_error;

    إذا (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "XML error: failed to validate document at %s\n", مسیر);
        اذهب validate_error;
    }

    ریشه = xml_Doc_Get_ریشه_Element(doc);
    parse_قواعد_xml(ctx, ریشه, popularity);

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
    xml_مجموعه_Generic_Error_Func(عدم, عدم);
#پایان_اگر
#إذا HAVE_XML_CTXT_مجموعه_ERRORHANDLER
    xml_Ctxt_مجموعه_Error_Handler(xmlCtxt, عدم, عدم);
#پایان_اگر
    xmlFreeParserCtxt(xmlCtxt);

    أرجع success;
}
