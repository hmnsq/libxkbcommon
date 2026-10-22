/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <libxml/parser.h>

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    #अन्तर्भाव <limits.h>
    #यदि HAVE_UNISTD_H && HAVE_DIRENT_H
        #अन्तर्भाव <dirent.h>
        #अन्तर्भाव <unistd.h>
    #यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "xkbcommon/xkbregistry.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-list.h"
#अन्तर्भाव "util-mem.h"

संरचना rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
संरचना rxkb_object {
    संरचना rxkb_object *parent;
    uint32_t refcount;
    संरचना list link;
};

संरचना rxkb_iso639_संहिता {
    संरचना rxkb_object आधारः;
    वर्ण *संहिता;
};

संरचना rxkb_iso3166_संहिता {
    संरचना rxkb_object आधारः;
    वर्ण *संहिता;
};

गणना context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

संरचना rxkb_context {
    संरचना rxkb_object आधारः;
    गणना context_state context_state;

    bool आरोपय_extra_नियमाः_files;
    bool use_secure_getenv;

    संरचना list models;         /* list of struct rxkb_models */
    संरचना list layouts;        /* list of struct rxkb_layouts */
    संरचना list option_groups;  /* list of struct rxkb_option_group */

    darray(वर्ण *) includes;


    ATTR_PRINTF(3, 0) शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                     गणना rxkb_log_level level,
                                     ध्रुव वर्ण *fmt, va_list तर्काः);
    गणना rxkb_log_level log_level;

    शून्य *userdata;
};

संरचना rxkb_model {
    संरचना rxkb_object आधारः;

    वर्ण *name;
    वर्ण *vendor;
    वर्ण *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_layout {
    संरचना rxkb_object आधारः;

    वर्ण *name;
    वर्ण *brief;
    वर्ण *description;
    वर्ण *variant;
    गणना rxkb_popularity popularity;

    संरचना list iso639s;  /* list of struct rxkb_iso639_code */
    संरचना list iso3166s; /* list of struct rxkb_iso3166_code */
};

संरचना rxkb_option_गणः {
    संरचना rxkb_object आधारः;

    bool allow_गुणजः;
    संरचना list options; /* list of struct rxkb_options */
    वर्ण *name;
    वर्ण *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_option {
    संरचना rxkb_object आधारः;

    वर्ण *name;
    वर्ण *brief;
    वर्ण *description;
    गणना rxkb_popularity popularity;
    bool layout_specific;
};

स्थिर bool
parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *मार्गः,
      गणना rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
स्थिर शून्य
rxkb_log(संरचना rxkb_context *ctx, गणना rxkb_log_level level,
         ध्रुव वर्ण *fmt, ...)
{
    va_list तर्काः;

    यदि (ctx->log_level < level)
        प्रत्यागम;

    va_start(तर्काः, fmt);
    ctx->log_fn(ctx, level, fmt, तर्काः);
    va_अन्तः(तर्काः);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#परिभाषय rxkb_log_with_संहिता(ctx, level, सन्देशः_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_सन्देशः_ID(सन्देशः_id, fmt), ##__VA_ARGS__)
#परिभाषय log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#परिभाषय log_info(ctx, id, ...) \
    rxkb_log_with_संहिता((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#परिभाषय log_warn(ctx, id, ...) \
    rxkb_log_with_संहिता((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#परिभाषय log_err(ctx, id, ...) \
    rxkb_log_with_संहिता((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#परिभाषय log_wsgo(ctx, id, ...) \
    rxkb_log_with_संहिता((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#परिभाषय DECLARE_REF_UNREF_FOR_TYPE(type_) \
संरचना type_ * type_##_ref(संरचना type_ *object) { \
    rxkb_object_ref(&object->आधारः); \
    प्रत्यागम object; \
} \
संरचना type_ * type_##_unref(संरचना type_ *object) { \
    यदि (!object) प्रत्यागम रिक्त; \
    प्रतिज्ञा(object->आधारः.refcount >= 1); \
    यदि (--object->आधारः.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->आधारः.link);\
        मुक्त(object); \
    } \
    प्रत्यागम रिक्त;\
}

#परिभाषय DECLARE_CREATE_FOR_TYPE(type_) \
स्थिर अन्तःस्थ संरचना type_ * type_##_create(संरचना rxkb_object *parent) { \
    संरचना type_ *t = शून्यस्मृतिं_देहि(1, परिमाण *t); \
    यदि (t) \
        rxkb_object_init(&t->आधारः, parent); \
    प्रत्यागम t; \
}

#परिभाषय DECLARE_TYPED_GETTER_FOR_TYPE(type_, क्षेत्रम्, rtype_) \
rtype_ type_##_get_##field_(संरचना type_ *object) { \
    प्रत्यागम object->क्षेत्रम्; \
}

#परिभाषय DECLARE_GETTER_FOR_TYPE(type_, क्षेत्रम्) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, क्षेत्रम्, ध्रुव वर्ण*)

#परिभाषय DECLARE_FIRST_अग्र_FOR_TYPE(type_, parent_type_, parent_क्षेत्रम्) \
संरचना type_ * type_##_first(संरचना parent_type_ *parent) { \
    संरचना type_ *o = रिक्त; \
    यदि (!list_रिक्तानि(&parent->parent_क्षेत्रम्)) \
        o = list_first_प्रविष्टिः(&parent->parent_क्षेत्रम्, o, आधारः.link); \
    प्रत्यागम o; \
} \
संरचना type_ * \
type_##_next(संरचना type_ *o) \
{ \
    संरचना parent_type_ *parent; \
    संरचना type_ *अग्र; \
    parent = container_सम्बन्धि(o->आधारः.parent, संरचना parent_type_, आधारः); \
    अग्र = list_first_प्रविष्टिः(&o->आधारः.link, o, आधारः.link); \
    यदि (list_is_चरमः(&parent->parent_क्षेत्रम्, &o->आधारः.link)) \
        प्रत्यागम रिक्त; \
    प्रत्यागम अग्र; \
}

स्थिर शून्य
rxkb_object_init(संरचना rxkb_object *object, संरचना rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

स्थिर शून्य *
rxkb_object_ref(संरचना rxkb_object *object)
{
    प्रतिज्ञा(object->refcount >= 1);
    ++object->refcount;
    प्रत्यागम object;
}

स्थिर शून्य
rxkb_iso639_संहिता_destroy(संरचना rxkb_iso639_संहिता *संहिता)
{
    मुक्त(संहिता->संहिता);
}

संरचना rxkb_iso639_संहिता *
rxkb_layout_get_iso639_first(संरचना rxkb_layout *layout)
{
    संरचना rxkb_iso639_संहिता *संहिता = रिक्त;

    यदि (!list_रिक्तानि(&layout->iso639s))
        संहिता = list_first_प्रविष्टिः(&layout->iso639s, संहिता, आधारः.link);

    प्रत्यागम संहिता;
}

संरचना rxkb_iso639_संहिता *
rxkb_iso639_संहिता_अग्र(संरचना rxkb_iso639_संहिता *संहिता)
{
    संरचना rxkb_iso639_संहिता *अग्र = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_सम्बन्धि(संहिता->आधारः.parent, संरचना rxkb_layout, आधारः);

    यदि (list_is_चरमः(&layout->iso639s, &संहिता->आधारः.link))
        प्रत्यागम रिक्त;

    अग्र = list_first_प्रविष्टिः(&संहिता->आधारः.link, संहिता, आधारः.link);

    प्रत्यागम अग्र;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_संहिता);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_संहिता);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_संहिता, संहिता);

स्थिर शून्य
rxkb_iso3166_संहिता_destroy(संरचना rxkb_iso3166_संहिता *संहिता)
{
    मुक्त(संहिता->संहिता);
}

संरचना rxkb_iso3166_संहिता *
rxkb_layout_get_iso3166_first(संरचना rxkb_layout *layout)
{
    संरचना rxkb_iso3166_संहिता *संहिता = रिक्त;

    यदि (!list_रिक्तानि(&layout->iso3166s))
        संहिता = list_first_प्रविष्टिः(&layout->iso3166s, संहिता, आधारः.link);

    प्रत्यागम संहिता;
}

संरचना rxkb_iso3166_संहिता *
rxkb_iso3166_संहिता_अग्र(संरचना rxkb_iso3166_संहिता *संहिता)
{
    संरचना rxkb_iso3166_संहिता *अग्र = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_सम्बन्धि(संहिता->आधारः.parent, संरचना rxkb_layout, आधारः);

    यदि (list_is_चरमः(&layout->iso3166s, &संहिता->आधारः.link))
        प्रत्यागम रिक्त;

    अग्र = list_first_प्रविष्टिः(&संहिता->आधारः.link, संहिता, आधारः.link);

    प्रत्यागम अग्र;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_संहिता);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_संहिता);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_संहिता, संहिता);

स्थिर शून्य
rxkb_option_destroy(संरचना rxkb_option *o)
{
    मुक्त(o->name);
    मुक्त(o->brief);
    मुक्त(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, गणना rxkb_popularity);
bool rxkb_option_is_layout_specific(संरचना rxkb_option *object) {
    प्रत्यागम object->layout_specific;
}
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_option, rxkb_option_गणः, options);

स्थिर शून्य
rxkb_layout_destroy(संरचना rxkb_layout *l)
{
    संरचना rxkb_iso639_संहिता *iso639, *tmp_639;
    संरचना rxkb_iso3166_संहिता *iso3166, *tmp_3166;

    मुक्त(l->name);
    मुक्त(l->brief);
    मुक्त(l->description);
    मुक्त(l->variant);

    list_for_each_safe(iso639, tmp_639, &l->iso639s, आधारः.link) {
        rxkb_iso639_संहिता_unref(iso639);
    }
    list_for_each_safe(iso3166, tmp_3166, &l->iso3166s, आधारः.link) {
        rxkb_iso3166_संहिता_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, गणना rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

स्थिर शून्य
rxkb_model_destroy(संरचना rxkb_model *m)
{
    मुक्त(m->name);
    मुक्त(m->vendor);
    मुक्त(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, गणना rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_model, rxkb_context, models);

स्थिर शून्य
rxkb_option_गणः_destroy(संरचना rxkb_option_गणः *og)
{
    संरचना rxkb_option *o, *otmp;

    मुक्त(og->name);
    मुक्त(og->description);

    list_for_each_safe(o, otmp, &og->options, आधारः.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_गणः_allows_गुणजः(संरचना rxkb_option_गणः *g)
{
    प्रत्यागम g->allow_गुणजः;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_गणः);
DECLARE_CREATE_FOR_TYPE(rxkb_option_गणः);
DECLARE_GETTER_FOR_TYPE(rxkb_option_गणः, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_गणः, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_गणः, popularity, गणना rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_option_गणः, rxkb_context, option_groups);

स्थिर शून्य
rxkb_context_destroy(संरचना rxkb_context *ctx)
{
    संरचना rxkb_model *m, *mtmp;
    संरचना rxkb_layout *l, *ltmp;
    संरचना rxkb_option_गणः *og, *ogtmp;
    वर्ण **मार्गः;

    list_for_each_safe(m, mtmp, &ctx->models, आधारः.link)
        rxkb_model_unref(m);
    प्रतिज्ञा(list_रिक्तानि(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, आधारः.link)
        rxkb_layout_unref(l);
    प्रतिज्ञा(list_रिक्तानि(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, आधारः.link)
        rxkb_option_गणः_unref(og);
    प्रतिज्ञा(list_रिक्तानि(&ctx->option_groups));

    darray_foreach(मार्गः, ctx->includes)
        मुक्त(*मार्गः);
    darray_free(ctx->includes);

    प्रतिज्ञा(darray_रिक्तानि(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, गणना rxkb_log_level);

स्थिर वर्ण *
rxkb_context_getenv(संरचना rxkb_context *ctx, ध्रुव वर्ण *name)
{
    यदि (ctx->use_secure_getenv) {
        प्रत्यागम secure_getenv(name);
    } अन्यथा {
        प्रत्यागम getenv(name);
    }
}


शून्य
rxkb_context_समुच्चयः_log_level(संरचना rxkb_context *ctx,
                           गणना rxkb_log_level level)
{
    ctx->log_level = level;
}

स्थिर ध्रुव वर्ण *
log_level_गन्तव्यम्_उपसर्गः(गणना rxkb_log_level level)
{
    विकल्प (level) {
    पक्ष RXKB_LOG_LEVEL_DEBUG:
        प्रत्यागम "xkbregistry: DEBUG: ";
    पक्ष RXKB_LOG_LEVEL_INFO:
        प्रत्यागम "xkbregistry: INFO: ";
    पक्ष RXKB_LOG_LEVEL_WARNING:
        प्रत्यागम "xkbregistry: WARNING: ";
    पक्ष RXKB_LOG_LEVEL_ERROR:
        प्रत्यागम "xkbregistry: ERROR: ";
    पक्ष RXKB_LOG_LEVEL_CRITICAL:
        प्रत्यागम "xkbregistry: CRITICAL: ";
    स्वभाव:
        प्रत्यागम रिक्त;
    }
}

ATTR_PRINTF(3, 0) स्थिर शून्य
default_log_fn(संरचना rxkb_context *ctx, गणना rxkb_log_level level,
               ध्रुव वर्ण *fmt, va_list तर्काः)
{
    ध्रुव वर्ण *उपसर्गः = log_level_गन्तव्यम्_उपसर्गः(level);

    यदि (उपसर्गः)
        fprintf(stderr, "%s", उपसर्गः);
    vfprintf(stderr, fmt, तर्काः);
}

स्थिर गणना rxkb_log_level
log_level(ध्रुव वर्ण *level) {
    errno = 0;
    वर्ण *endptr;
    ध्रुव दीर्घ lvl = strtol(level, &endptr, 10);
    यदि (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        प्रत्यागम (गणना rxkb_log_level)lvl;

    यदि (istreq_उपसर्गः("crit", level))
        प्रत्यागम RXKB_LOG_LEVEL_CRITICAL;
    यदि (istreq_उपसर्गः("err", level))
        प्रत्यागम RXKB_LOG_LEVEL_ERROR;
    यदि (istreq_उपसर्गः("warn", level))
        प्रत्यागम RXKB_LOG_LEVEL_WARNING;
    यदि (istreq_उपसर्गः("info", level))
        प्रत्यागम RXKB_LOG_LEVEL_INFO;
    यदि (istreq_उपसर्गः("debug", level) || istreq_उपसर्गः("dbg", level))
        प्रत्यागम RXKB_LOG_LEVEL_DEBUG;

    प्रत्यागम RXKB_LOG_LEVEL_ERROR;
}

संरचना rxkb_context *
rxkb_context_new(गणना rxkb_context_flags flags)
{
    संरचना rxkb_context *ctx = rxkb_context_create(रिक्त);
    ध्रुव वर्ण *env;

    यदि (!ctx)
        प्रत्यागम रिक्त;

    ctx->context_state = CONTEXT_NEW;
    ctx->आरोपय_extra_नियमाः_files = flags & RXKB_CONTEXT_आरोपय_EXOTIC_नियमाः;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    यदि (env)
        rxkb_context_समुच्चयः_log_level(ctx, log_level(env));

    स्थिर ध्रुव गणना rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_आरोपय_EXOTIC_नियमाः
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    यदि (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        मुक्त(ctx);
        प्रत्यागम रिक्त;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    यदि (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_मार्गः_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_मार्गः,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_मूलम्);
        rxkb_context_unref(ctx);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम ctx;
}

शून्य
rxkb_context_समुच्चयः_log_fn(संरचना rxkb_context *ctx,
                        शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                       गणना rxkb_log_level level,
                                       ध्रुव वर्ण *fmt, va_list तर्काः))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_मार्गः_append(संरचना rxkb_context *ctx, ध्रुव वर्ण *मार्गः)
{
    पूर्णाङ्क err = 0;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "include paths can only be appended to a new context\n");
        गच्छ error;
    }

    संरचना stat stat_buf;
    err = stat(मार्गः, &stat_buf);
    यदि (err != 0) {
        err = errno;
        गच्छ error;
    }
    यदि (!S_ISDIR(stat_buf.st_बहुलकम्)) {
        err = ENOTDIR;
        गच्छ error;
    }

    यदि (!check_eaccess(मार्गः, R_OK | X_OK)) {
        err = EACCES;
        गच्छ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    वर्ण नियमाः[PATH_MAX];
    यदि (!snprintf_safe(नियमाः, परिमाण(नियमाः), "%s/rules/%s.xml",
                       मार्गः, DEFAULT_XKB_नियमाः)) {
        log_err(ctx, XKB_ERROR_INVALID_मार्गः,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                परिमाण(नियमाः), मार्गः, DEFAULT_XKB_नियमाः);
        गच्छ error;
    }

    वर्ण * ध्रुव tmp = strdup(मार्गः);
    यदि (!tmp) {
        err = ENOMEM;
        गच्छ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_सन्देशः_NO_ID, "Include path added: %s\n", tmp);

    प्रत्यागम true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_सन्देशः_NO_ID,
             "Include path failed: \"%s\" (%s)\n", मार्गः, strerror(err));
    प्रत्यागम false;
}

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES

स्थिर पूर्णाङ्क
compare_str(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम वाक्यं_तुलय(*(वर्ण **)a, *(वर्ण **) b);
}

स्थिर पूर्णाङ्क
सङ्कलय_प्रत्यक्षम्_subdirectories(संरचना rxkb_context *ctx, ध्रुव वर्ण *मार्गः,
                          darray_string *extensions,
                          darray_आकार_t versioned_गणनम्,
                          आकार_t versioned_मार्गः_दीर्घता)
{
    पूर्णाङ्क ret = 0;
    पूर्णाङ्क err = ENOMEM;
    DIR *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(मार्गः, &stat_buf);
    यदि (err != 0) {
        err = errno;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_बहुलकम्)) {
        err = ENOTDIR;
        गच्छ err;
    }
    यदि (!check_eaccess(मार्गः, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    dir = opendir(मार्गः);
    यदि (dir == रिक्त) {
        err = EACCES;
        गच्छ err;
    }

    संरचना dirent *प्रविष्टिः;
    वर्ण मार्गः_buf[PATH_MAX] = "";
    versioned_मार्गः_दीर्घता++; /* Additional final ‘/’ */
    यावत् ((प्रविष्टिः = readdir(dir)) != रिक्त) {
        ध्रुव वर्ण *name = प्रविष्टिः->d_name;

        /* Skip special entries */
        यदि (वाक्यं_तुलय(name, ".") == 0 || वाक्यं_तुलय(name, "..") == 0)
            अनुवर्त;

        यदि (!snprintf_safe(मार्गः_buf, परिमाण(मार्गः_buf), "%s/%s", मार्गः, name)) {
            err = ENOMEM;
            गच्छ err;
        }
        यदि (stat(मार्गः_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_बहुलकम्))
            अनुवर्त;

        /* Skip if the corresponding versioned directory is already included */
        प्रति (darray_आकार_t i = 0; i < versioned_गणनम्; i++) {
            ध्रुव वर्ण * ध्रुव पूर्व_name = darray_item(*extensions, i)
                                         + versioned_मार्गः_दीर्घता;
            यदि (वाक्यं_तुलय(name, पूर्व_name) == 0)
                गच्छ अग्र;
        }

        वर्ण *ext_मार्गः = strdup_safe(मार्गः_buf);
        यदि (!ext_मार्गः) {
            err = ENOMEM;
            गच्छ err;
        }

        darray_append(*extensions, ext_मार्गः);
अग्र:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    यदि (darray_आकार(*extensions) > versioned_गणनम्) {
        /* Sort the entries, so that they are appended in lexicographic order */
        शीघ्रं_क्रमय(darray_वस्तूनि(*extensions) + versioned_गणनम्,
              darray_आकार(*extensions) - versioned_गणनम्,
              परिमाण(*darray_वस्तूनि(*extensions)), &compare_str);
        वर्ण **ext_मार्गः;
        darray_foreach_from(ext_मार्गः, *extensions, versioned_गणनम्) {
            ret |= rxkb_context_include_मार्गः_append(ctx, *ext_मार्गः);
        }
    }

    प्रत्यागम ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", मार्गः, strerror(err));
    यदि (dir)
        closedir(dir);

    प्रत्यागम ret;
}
#यदि_अन्तः

bool
rxkb_context_include_मार्गः_append_default(संरचना rxkb_context *ctx)
{
    वर्ण user_मार्गः[PATH_MAX];
    bool ret = false;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "include paths can only be appended to a new context\n");
        प्रत्यागम false;
    }

    ध्रुव वर्ण * ध्रुव home = rxkb_context_getenv(ctx, "HOME");
    ध्रुव वर्ण * ध्रुव xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    यदि (xdg != रिक्त) {
        यदि (snprintf_safe(user_मार्गः, परिमाण(user_मार्गः), "%s/xkb", xdg))
            ret |= rxkb_context_include_मार्गः_append(ctx, user_मार्गः);
    } अन्यथा यदि (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        यदि (snprintf_safe(user_मार्गः, परिमाण(user_मार्गः), "%s/.config/xkb", home))
            ret |= rxkb_context_include_मार्गः_append(ctx, user_मार्गः);
    }

    यदि (home != रिक्त) {
        यदि (snprintf_safe(user_मार्गः, परिमाण(user_मार्गः), "%s/.xkb", home))
            ret |= rxkb_context_include_मार्गः_append(ctx, user_मार्गः);
    }

    ध्रुव वर्ण * ध्रुव extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_मार्गः_append(
        ctx, ((extra != रिक्त) ? extra : DFLT_XKB_CONFIG_EXTRA_मार्गः)
    );

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ध्रुव वर्ण *extensions_मार्गः =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#यदि_परिभाषितम् DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_मार्गः
    यदि (extensions_मार्गः == रिक्त)
        extensions_मार्गः = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_मार्गः;
#यदि_अन्तः
    आकार_t versioned_मार्गः_दीर्घता = 0;
    यदि (extensions_मार्गः != रिक्त) {
        /* Add direct subdirectories */
        ret |= सङ्कलय_प्रत्यक्षम्_subdirectories(ctx, extensions_मार्गः,
                                         &extensions, 0, 0);
        versioned_मार्गः_दीर्घता = वाक्यदैर्घ्यम्(extensions_मार्गः);
    }

    /* Unversioned extensions directory */
    extensions_मार्गः =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#यदि_परिभाषितम् DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_मार्गः
    यदि (extensions_मार्गः == रिक्त)
        extensions_मार्गः = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_मार्गः;
#यदि_अन्तः
    यदि (extensions_मार्गः != रिक्त) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= सङ्कलय_प्रत्यक्षम्_subdirectories(ctx, extensions_मार्गः, &extensions,
                                         darray_आकार(extensions),
                                         versioned_मार्गः_दीर्घता);
    }

    वर्ण **ext_मार्गः;
    darray_foreach(ext_मार्गः, extensions) {
        मुक्त(*ext_मार्गः);
    }
    darray_free(extensions);
#यदि_अन्तः

    /* Canonical XKB root */
    ध्रुव वर्ण * ध्रुव मूलम् = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ध्रुव bool has_मूलम् = rxkb_context_include_मार्गः_append(
        ctx, ((मूलम् != रिक्त) ? मूलम्: DFLT_XKB_CONFIG_मूलम्)
    );
    ret |= has_मूलम्;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    यदि (!has_मूलम् && (मूलम् == रिक्त || मूलम्[0] != '\0')) {
        log_warn(ctx, XKB_LOG_सन्देशः_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूलम्,
                 ((मूलम् == रिक्त) ? DFLT_XKB_CONFIG_मूलम् : मूलम्));
        ret |= rxkb_context_include_मार्गः_append(ctx, DFLT_XKB_LEGACY_मूलम्);
    }

    प्रत्यागम ret;
}

bool
rxkb_context_parse_default_ruleset(संरचना rxkb_context *ctx)
{
    प्रत्यागम rxkb_context_parse(ctx, DEFAULT_XKB_नियमाः);
}

bool
rxkb_context_parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *ruleset)
{
    वर्ण **मार्गः;
    bool success = false;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "parse must only be called on a new context\n");
        प्रत्यागम false;
    }

    darray_foreach_reverse(मार्गः, ctx->includes) {
        वर्ण नियमाः[PATH_MAX];

        यदि (snprintf_safe(नियमाः, परिमाण(नियमाः), "%s/rules/%s.xml",
                           *मार्गः, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", नियमाः);
            यदि (parse(ctx, नियमाः, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        यदि (ctx->आरोपय_extra_नियमाः_files &&
            snprintf_safe(नियमाः, परिमाण(नियमाः), "%s/rules/%s.extras.xml",
                          *मार्गः, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", नियमाः);
            यदि (parse(ctx, नियमाः, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    प्रत्यागम success;
}


शून्य
rxkb_context_समुच्चयः_user_दत्त(संरचना rxkb_context *ctx, शून्य *userdata)
{
    ctx->userdata = userdata;
}

शून्य *
rxkb_context_get_user_दत्त(संरचना rxkb_context *ctx)
{
    प्रत्यागम ctx->userdata;
}

स्थिर अन्तःस्थ bool
is_ग्रन्थिः(xml_ग्रन्थिः *ग्रन्थिः, ध्रुव वर्ण *name)
{
    प्रत्यागम ग्रन्थिः->type == XML_ELEMENT_ग्रन्थिः &&
        xmlStrEqual(ग्रन्थिः->name, (ध्रुव xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
स्थिर वर्ण *
extract_text(xml_ग्रन्थिः *ग्रन्थिः)
{
    xml_ग्रन्थिः *n;

    प्रति (n = ग्रन्थिः->children; n; n = n->अग्र) {
        यदि (n->type == XML_TEXT_ग्रन्थिः)
            प्रत्यागम (वर्ण *)xmlStrdup(n->content);
    }
    प्रत्यागम रिक्त;
}

/* Data from “configItem” node */
संरचना config_item {
    वर्ण *name;
    वर्ण *description;
    वर्ण *brief;
    वर्ण *vendor;
    गणना rxkb_popularity popularity;
    bool layout_specific;
};

#परिभाषय config_item_new(popularity_) { \
    .name = रिक्त, \
    .description = रिक्त, \
    .brief = रिक्त, \
    .vendor = रिक्त, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

स्थिर शून्य
config_item_free(संरचना config_item *config) {
    मुक्त(config->name);
    मुक्त(config->description);
    मुक्त(config->brief);
    मुक्त(config->vendor);
}

स्थिर bool
parse_config_item(संरचना rxkb_context *ctx, xml_ग्रन्थिः *parent,
                  संरचना config_item *config)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;
    xml_ग्रन्थिः *ci = रिक्त;

    प्रति (ci = parent->children; ci; ci = ci->अग्र) {
        यदि (is_ग्रन्थिः(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ध्रुव xmlChar*)"popularity");
            यदि (raw_popularity) {
                यदि (xmlStrEqual(raw_popularity, (ध्रुव xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                अन्यथा यदि (xmlStrEqual(raw_popularity, (ध्रुव xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                अन्यथा
                    log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->पङ्क्तिः, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ध्रुव xmlChar*)"layout-specific");
            यदि (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ध्रुव xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            प्रति (ग्रन्थिः = ci->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
                यदि (is_ग्रन्थिः(ग्रन्थिः, "name"))
                    config->name = extract_text(ग्रन्थिः);
                अन्यथा यदि (is_ग्रन्थिः(ग्रन्थिः, "description"))
                    config->description = extract_text(ग्रन्थिः);
                अन्यथा यदि (is_ग्रन्थिः(ग्रन्थिः, "shortDescription"))
                    config->brief = extract_text(ग्रन्थिः);
                अन्यथा यदि (is_ग्रन्थिः(ग्रन्थिः, "vendor"))
                    config->vendor = extract_text(ग्रन्थिः);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            यदि (!config->name || !वाक्यदैर्घ्यम्(config->name))  {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->पङ्क्तिः);
                config_item_free(config);
                प्रत्यागम false;
            }

            प्रत्यागम true; /* only one configItem allowed in the dtd */
        }
    }

    प्रत्यागम false;
}

स्थिर शून्य
parse_model(संरचना rxkb_context *ctx, xml_ग्रन्थिः *model,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, model, &config)) {
        संरचना rxkb_model *m;

        list_for_each(m, &ctx->models, आधारः.link) {
            यदि (streq(m->name, config.name)) {
                config_item_free(&config);
                प्रत्यागम;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->आधारः);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->आधारः.link);
    }
}

स्थिर शून्य
parse_model_list(संरचना rxkb_context *ctx, xml_ग्रन्थिः *model_list,
                 गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;

    प्रति (ग्रन्थिः = model_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "model"))
            parse_model(ctx, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
parse_language_list(xml_ग्रन्थिः *language_list, संरचना rxkb_layout *layout)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;
    संरचना rxkb_iso639_संहिता *संहिता;

    प्रति (ग्रन्थिः = language_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "iso639Id")) {
            वर्ण *str = extract_text(ग्रन्थिः);
            संरचना rxkb_object *parent;

            यदि (!str || वाक्यदैर्घ्यम्(str) != 3) {
                मुक्त(str);
                अनुवर्त;
            }

            parent = &layout->आधारः;
            संहिता = rxkb_iso639_संहिता_create(parent);
            संहिता->संहिता = str;
            list_append(&layout->iso639s, &संहिता->आधारः.link);
        }
    }
}

स्थिर शून्य
parse_country_list(xml_ग्रन्थिः *country_list, संरचना rxkb_layout *layout)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;
    संरचना rxkb_iso3166_संहिता *संहिता;

    प्रति (ग्रन्थिः = country_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "iso3166Id")) {
            वर्ण *str = extract_text(ग्रन्थिः);
            संरचना rxkb_object *parent;

            यदि (!str || वाक्यदैर्घ्यम्(str) != 2) {
                मुक्त(str);
                अनुवर्त;
            }

            parent = &layout->आधारः;
            संहिता = rxkb_iso3166_संहिता_create(parent);
            संहिता->संहिता = str;
            list_append(&layout->iso3166s, &संहिता->आधारः.link);
        }
    }
}

स्थिर शून्य
parse_variant(संरचना rxkb_context *ctx, संरचना rxkb_layout *l,
              xml_ग्रन्थिः *variant, गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ci;
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, variant, &config)) {
        संरचना rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, आधारः.link) {
            यदि (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                भङ्ग;
            }
        }

        यदि (!exists) {
            v = rxkb_layout_create(&ctx->आधारः);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == रिक्त ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->आधारः.link);

            प्रति (ci = variant->children; ci; ci = ci->अग्र) {
                xml_ग्रन्थिः *ग्रन्थिः;

                यदि (!is_ग्रन्थिः(ci, "configItem"))
                    अनुवर्त;

                bool प्राप्तम्_language_list = false;
                bool प्राप्तम्_country_list = false;
                प्रति (ग्रन्थिः = ci->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
                    यदि (is_ग्रन्थिः(ग्रन्थिः, "languageList")) {
                        parse_language_list(ग्रन्थिः, v);
                        प्राप्तम्_language_list = true;
                    }
                    यदि (is_ग्रन्थिः(ग्रन्थिः, "countryList")) {
                        parse_country_list(ग्रन्थिः, v);
                        प्राप्तम्_country_list = true;
                    }
                }
                यदि (!प्राप्तम्_language_list) {
                    // inherit from parent layout
                    संरचना rxkb_iso639_संहिता* x;
                    list_for_each(x, &l->iso639s, आधारः.link) {
                        संरचना rxkb_iso639_संहिता* संहिता = rxkb_iso639_संहिता_create(&v->आधारः);
                        संहिता->संहिता = strdup(x->संहिता);
                        list_append(&v->iso639s, &संहिता->आधारः.link);
                    }
                }
                यदि (!प्राप्तम्_country_list) {
                    // inherit from parent layout
                    संरचना rxkb_iso3166_संहिता* x;
                    list_for_each(x, &l->iso3166s, आधारः.link) {
                        संरचना rxkb_iso3166_संहिता* संहिता = rxkb_iso3166_संहिता_create(&v->आधारः);
                        संहिता->संहिता = strdup(x->संहिता);
                        list_append(&v->iso3166s, &संहिता->आधारः.link);
                    }
                }
            }
        } अन्यथा {
            config_item_free(&config);
        }
    }
}

स्थिर शून्य
parse_variant_list(संरचना rxkb_context *ctx, संरचना rxkb_layout *l,
                   xml_ग्रन्थिः *variant_list, गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;

    प्रति (ग्रन्थिः = variant_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "variant"))
            parse_variant(ctx, l, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
parse_layout(संरचना rxkb_context *ctx, xml_ग्रन्थिः *layout,
             गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_layout *l;
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;
    bool exists = false;

    यदि (!parse_config_item(ctx, layout, &config))
        प्रत्यागम;

    list_for_each(l, &ctx->layouts, आधारः.link) {
        यदि (streq(l->name, config.name) && l->variant == रिक्त) {
            exists = true;
            भङ्ग;
        }
    }

    यदि (!exists) {
        l = rxkb_layout_create(&ctx->आधारः);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = रिक्त;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->आधारः.link);
    } अन्यथा {
        config_item_free(&config);
    }

    प्रति (ग्रन्थिः = layout->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "variantList")) {
            parse_variant_list(ctx, l, ग्रन्थिः, popularity);
        }
        यदि (!exists && is_ग्रन्थिः(ग्रन्थिः, "configItem")) {
            xml_ग्रन्थिः *ll;
            प्रति (ll = ग्रन्थिः->children; ll; ll = ll->अग्र) {
                यदि (is_ग्रन्थिः(ll, "languageList"))
                    parse_language_list(ll, l);
                यदि (is_ग्रन्थिः(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

स्थिर शून्य
parse_layout_list(संरचना rxkb_context *ctx, xml_ग्रन्थिः *layout_list,
                  गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;

    प्रति (ग्रन्थिः = layout_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "layout"))
            parse_layout(ctx, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
parse_option(संरचना rxkb_context *ctx, संरचना rxkb_option_गणः *गणः,
             xml_ग्रन्थिः *option, गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, option, &config)) {
        संरचना rxkb_option *o;

        list_for_each(o, &गणः->options, आधारः.link) {
            यदि (streq(o->name, config.name)) {
                config_item_free(&config);
                प्रत्यागम;
            }
        }

        o = rxkb_option_create(&गणः->आधारः);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&गणः->options, &o->आधारः.link);
    }
}

स्थिर शून्य
parse_गणः(संरचना rxkb_context *ctx, xml_ग्रन्थिः *गणः,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_option_गणः *g;
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;
    xmlChar *गुणजः;
    bool exists = false;

    यदि (!parse_config_item(ctx, गणः, &config))
        प्रत्यागम;

    list_for_each(g, &ctx->option_groups, आधारः.link) {
        यदि (streq(g->name, config.name)) {
            exists = true;
            भङ्ग;
        }
    }

    यदि (!exists) {
        g = rxkb_option_गणः_create(&ctx->आधारः);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        गुणजः = xmlGetProp(गणः, (ध्रुव xmlChar*)"allowMultipleSelection");
        यदि (गुणजः && xmlStrEqual(गुणजः, (ध्रुव xmlChar*)"true"))
            g->allow_गुणजः = true;
        xmlFree(गुणजः);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->आधारः.link);
    } अन्यथा {
        config_item_free(&config);
    }

    प्रति (ग्रन्थिः = गणः->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "option"))
            parse_option(ctx, g, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
parse_option_list(संरचना rxkb_context *ctx, xml_ग्रन्थिः *option_list,
                  गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;

    प्रति (ग्रन्थिः = option_list->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "group"))
            parse_गणः(ctx, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
parse_नियमाः_xml(संरचना rxkb_context *ctx, xml_ग्रन्थिः *मूलम्,
                गणना rxkb_popularity popularity)
{
    xml_ग्रन्थिः *ग्रन्थिः = रिक्त;

    प्रति (ग्रन्थिः = मूलम्->children; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र) {
        यदि (is_ग्रन्थिः(ग्रन्थिः, "modelList"))
            parse_model_list(ctx, ग्रन्थिः, popularity);
        अन्यथा यदि (is_ग्रन्थिः(ग्रन्थिः, "layoutList"))
            parse_layout_list(ctx, ग्रन्थिः, popularity);
        अन्यथा यदि (is_ग्रन्थिः(ग्रन्थिः, "optionList"))
            parse_option_list(ctx, ग्रन्थिः, popularity);
    }
}

स्थिर शून्य
ATTR_PRINTF(2, 0)
xml_error_func(शून्य *ctx, ध्रुव वर्ण *सन्देशः, ...)
{
    स्थिर वर्ण buf[PATH_MAX];
    स्थिर आकार_t slen = 0;
    va_list तर्काः;
    पूर्णाङ्क rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(तर्काः, सन्देशः);
    rc = vsnprintf(&buf[slen], परिमाण(buf) - slen, सन्देशः, तर्काः);
    va_अन्तः(तर्काः);

    /* This shouldn't really happen */
    यदि (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        स्मृतिं_पूरय(buf, 0, परिमाण(buf));
        प्रत्यागम;
    }

    slen += (आकार_t)rc;
    यदि (slen >= परिमाण(buf)) {
        /* truncated, let's flush this */
        buf[परिमाण(buf) - 1] = '\n';
        slen = परिमाण(buf);
    }

    /* We're assuming here that the last character is \n. */
    यदि (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID, "%s", buf);
        स्मृतिं_पूरय(buf, 0, परिमाण(buf));
        slen = 0;
    }
}

#यदि HAVE_XML_CTXT_समुच्चयः_ERRORHANDLER
स्थिर शून्य
xml_structured_error_func(शून्य *user_दत्त_2, ध्रुव xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_दत्त_2);
}
#यदि_अन्तः

#यदि_परिभाषितम् XML_PARSE_NO_XXE
#परिभाषय _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#परिभाषय _XML_OPTIONS (XML_PARSE_NONET)
#यदि_अन्तः

स्थिर bool
validate(संरचना rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ध्रुव वर्ण dtdstr[] =
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

#यदि HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    यदि (!xmlCtxt)
        प्रत्यागम false;
    static_assert(HAVE_XML_CTXT_समुच्चयः_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_समुच्चयः_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_समुच्चयः_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_निवेशः_Ptr pinput =
        xml_New_निवेशः_From_स्मृतिः(रिक्त, dtdstr, ARRAY_आकार(dtdstr) - 1,
                              XML_निवेशः_BUF_STATIC);
    यदि (!pinput)
        गच्छ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, रिक्त, रिक्त);
#अन्यथा
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_निवेशः_बफर_Ptr buf =
        xml_Parser_निवेशः_बफर_Create_Mem(dtdstr, ARRAY_आकार(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    यदि (!buf)
        गच्छ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(रिक्त, buf, XML_CHAR_ENCODING_UTF8);
#यदि_अन्तः

    यदि (!dtd) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID, "Failed to load DTD\n");
        गच्छ dtd_error;
    }

#यदि HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#अन्यथा
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    यदि (!dtdvalid) {
        success = false;
        गच्छ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_दत्त_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#यदि_अन्तः
    xmlFreeDtd(dtd);

dtd_error:
#यदि HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#यदि_अन्तः
    प्रत्यागम success;
}

स्थिर bool
parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *मार्गः,
      गणना rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = रिक्त;
    xml_ग्रन्थिः *मूलम् = रिक्त;

    यदि (!check_eaccess(मार्गः, R_OK))
        प्रत्यागम false;

    LIBXML_TEST_संस्करणम्

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    यदि (!xmlCtxt)
        प्रत्यागम false;

#यदि_परिभाषितम् XML_PARSE_NO_XXE
#परिभाषय _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#परिभाषय _XML_OPTIONS (XML_PARSE_NONET)
#यदि_अन्तः
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#यदि HAVE_XML_CTXT_समुच्चयः_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_समुच्चयः_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#यदि_अन्तः
#यदि !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_समुच्चयः_Generic_Error_Func(ctx, xml_error_func);
#यदि_अन्तः

    doc = xmlCtxtReadFile(xmlCtxt, मार्गः, रिक्त, 0);
    यदि (!doc)
        गच्छ parse_error;

    यदि (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "XML error: failed to validate document at %s\n", मार्गः);
        गच्छ validate_error;
    }

    मूलम् = xml_Doc_Get_मूलम्_Element(doc);
    parse_नियमाः_xml(ctx, मूलम्, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#यदि !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xml_समुच्चयः_Generic_Error_Func(रिक्त, रिक्त);
#यदि_अन्तः
#यदि HAVE_XML_CTXT_समुच्चयः_ERRORHANDLER
    xml_Ctxt_समुच्चयः_Error_Handler(xmlCtxt, रिक्त, रिक्त);
#यदि_अन्तः
    xmlFreeParserCtxt(xmlCtxt);

    प्रत्यागम success;
}
