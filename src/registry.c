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
    #endif
#endif

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

संरचना rxkb_iso639_code {
    संरचना rxkb_object base;
    वर्ण *code;
};

संरचना rxkb_iso3166_code {
    संरचना rxkb_object base;
    वर्ण *code;
};

गणना context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

संरचना rxkb_context {
    संरचना rxkb_object base;
    गणना context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    संरचना list models;         /* list of struct rxkb_models */
    संरचना list layouts;        /* list of struct rxkb_layouts */
    संरचना list option_groups;  /* list of struct rxkb_option_group */

    darray(वर्ण *) includes;


    ATTR_PRINTF(3, 0) शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                     गणना rxkb_log_level level,
                                     ध्रुव वर्ण *fmt, बहु_सूची args);
    गणना rxkb_log_level log_level;

    शून्य *userdata;
};

संरचना rxkb_model {
    संरचना rxkb_object base;

    वर्ण *name;
    वर्ण *vendor;
    वर्ण *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_layout {
    संरचना rxkb_object base;

    वर्ण *name;
    वर्ण *brief;
    वर्ण *description;
    वर्ण *variant;
    गणना rxkb_popularity popularity;

    संरचना list iso639s;  /* list of struct rxkb_iso639_code */
    संरचना list iso3166s; /* list of struct rxkb_iso3166_code */
};

संरचना rxkb_option_group {
    संरचना rxkb_object base;

    bool allow_multiple;
    संरचना list options; /* list of struct rxkb_options */
    वर्ण *name;
    वर्ण *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_option {
    संरचना rxkb_object base;

    वर्ण *name;
    वर्ण *brief;
    वर्ण *description;
    गणना rxkb_popularity popularity;
    bool layout_specific;
};

स्थिर bool
parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *path,
      गणना rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
स्थिर शून्य
rxkb_log(संरचना rxkb_context *ctx, गणना rxkb_log_level level,
         ध्रुव वर्ण *fmt, ...)
{
    बहु_सूची args;

    यदि (ctx->log_level < level)
        प्रत्यागम;

    बहु_शुरू(args, fmt);
    ctx->log_fn(ctx, level, fmt, args);
    बहु_पूर्ण(args);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#define rxkb_log_with_code(ctx, level, संदेश_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(संदेश_id, fmt), ##__VA_ARGS__)
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
संरचना type_ * type_##_ref(संरचना type_ *object) { \
    rxkb_object_ref(&object->base); \
    प्रत्यागम object; \
} \
संरचना type_ * type_##_unref(संरचना type_ *object) { \
    यदि (!object) प्रत्यागम रिक्त; \
    प्रतिज्ञा(object->base.refcount >= 1); \
    यदि (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        मुक्त(object); \
    } \
    प्रत्यागम रिक्त;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
स्थिर अन्तःस्थ संरचना type_ * type_##_create(संरचना rxkb_object *parent) { \
    संरचना type_ *t = सुस्मृति(1, परिमाण *t); \
    यदि (t) \
        rxkb_object_init(&t->base, parent); \
    प्रत्यागम t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(संरचना type_ *object) { \
    प्रत्यागम object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ध्रुव वर्ण*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
संरचना type_ * type_##_first(संरचना parent_type_ *parent) { \
    संरचना type_ *o = रिक्त; \
    यदि (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    प्रत्यागम o; \
} \
संरचना type_ * \
type_##_next(संरचना type_ *o) \
{ \
    संरचना parent_type_ *parent; \
    संरचना type_ *next; \
    parent = container_of(o->base.parent, संरचना parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    यदि (list_is_last(&parent->parent_field_, &o->base.link)) \
        प्रत्यागम रिक्त; \
    प्रत्यागम next; \
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
rxkb_iso639_code_destroy(संरचना rxkb_iso639_code *code)
{
    मुक्त(code->code);
}

संरचना rxkb_iso639_code *
rxkb_layout_get_iso639_first(संरचना rxkb_layout *layout)
{
    संरचना rxkb_iso639_code *code = रिक्त;

    यदि (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    प्रत्यागम code;
}

संरचना rxkb_iso639_code *
rxkb_iso639_code_next(संरचना rxkb_iso639_code *code)
{
    संरचना rxkb_iso639_code *next = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_of(code->base.parent, संरचना rxkb_layout, base);

    यदि (list_is_last(&layout->iso639s, &code->base.link))
        प्रत्यागम रिक्त;

    next = list_first_entry(&code->base.link, code, base.link);

    प्रत्यागम next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

स्थिर शून्य
rxkb_iso3166_code_destroy(संरचना rxkb_iso3166_code *code)
{
    मुक्त(code->code);
}

संरचना rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(संरचना rxkb_layout *layout)
{
    संरचना rxkb_iso3166_code *code = रिक्त;

    यदि (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    प्रत्यागम code;
}

संरचना rxkb_iso3166_code *
rxkb_iso3166_code_next(संरचना rxkb_iso3166_code *code)
{
    संरचना rxkb_iso3166_code *next = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_of(code->base.parent, संरचना rxkb_layout, base);

    यदि (list_is_last(&layout->iso3166s, &code->base.link))
        प्रत्यागम रिक्त;

    next = list_first_entry(&code->base.link, code, base.link);

    प्रत्यागम next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

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
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

स्थिर शून्य
rxkb_layout_destroy(संरचना rxkb_layout *l)
{
    संरचना rxkb_iso639_code *iso639, *tmp_639;
    संरचना rxkb_iso3166_code *iso3166, *tmp_3166;

    मुक्त(l->name);
    मुक्त(l->brief);
    मुक्त(l->description);
    मुक्त(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, गणना rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

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
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

स्थिर शून्य
rxkb_option_group_destroy(संरचना rxkb_option_group *og)
{
    संरचना rxkb_option *o, *otmp;

    मुक्त(og->name);
    मुक्त(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(संरचना rxkb_option_group *g)
{
    प्रत्यागम g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, गणना rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

स्थिर शून्य
rxkb_context_destroy(संरचना rxkb_context *ctx)
{
    संरचना rxkb_model *m, *mtmp;
    संरचना rxkb_layout *l, *ltmp;
    संरचना rxkb_option_group *og, *ogtmp;
    वर्ण **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    प्रतिज्ञा(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    प्रतिज्ञा(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    प्रतिज्ञा(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        मुक्त(*path);
    darray_free(ctx->includes);

    प्रतिज्ञा(darray_empty(ctx->includes));
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
        प्रत्यागम दो_पर्या(name);
    }
}


शून्य
rxkb_context_set_log_level(संरचना rxkb_context *ctx,
                           गणना rxkb_log_level level)
{
    ctx->log_level = level;
}

स्थिर ध्रुव वर्ण *
log_level_to_prefix(गणना rxkb_log_level level)
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
               ध्रुव वर्ण *fmt, बहु_सूची args)
{
    ध्रुव वर्ण *prefix = log_level_to_prefix(level);

    यदि (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

स्थिर गणना rxkb_log_level
log_level(ध्रुव वर्ण *level) {
    त्रुटि_सं = 0;
    वर्ण *endptr;
    ध्रुव दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    यदि (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        प्रत्यागम (गणना rxkb_log_level)lvl;

    यदि (istreq_prefix("crit", level))
        प्रत्यागम RXKB_LOG_LEVEL_CRITICAL;
    यदि (istreq_prefix("err", level))
        प्रत्यागम RXKB_LOG_LEVEL_ERROR;
    यदि (istreq_prefix("warn", level))
        प्रत्यागम RXKB_LOG_LEVEL_WARNING;
    यदि (istreq_prefix("info", level))
        प्रत्यागम RXKB_LOG_LEVEL_INFO;
    यदि (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
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
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    यदि (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    स्थिर ध्रुव गणना rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    यदि (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        मुक्त(ctx);
        प्रत्यागम रिक्त;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    यदि (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_मूल);
        rxkb_context_unref(ctx);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम ctx;
}

शून्य
rxkb_context_set_log_fn(संरचना rxkb_context *ctx,
                        शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                       गणना rxkb_log_level level,
                                       ध्रुव वर्ण *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(संरचना rxkb_context *ctx, ध्रुव वर्ण *path)
{
    पूर्णाङ्क err = 0;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        गच्छ error;
    }

    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    यदि (err != 0) {
        err = त्रुटि_सं;
        गच्छ error;
    }
    यदि (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        गच्छ error;
    }

    यदि (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        गच्छ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    वर्ण rules[PATH_MAX];
    यदि (!snprintf_safe(rules, परिमाण(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                परिमाण(rules), path, DEFAULT_XKB_RULES);
        गच्छ error;
    }

    वर्ण * ध्रुव tmp = strdup(path);
    यदि (!tmp) {
        err = ENOMEM;
        गच्छ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    प्रत्यागम true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    प्रत्यागम false;
}

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES

स्थिर पूर्णाङ्क
compare_str(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम म_भेद(*(वर्ण **)a, *(वर्ण **) b);
}

स्थिर पूर्णाङ्क
add_direct_subdirectories(संरचना rxkb_context *ctx, ध्रुव वर्ण *path,
                          darray_string *extensions,
                          darray_size_t versioned_गिनती,
                          माप_प्रकार versioned_path_length)
{
    पूर्णाङ्क ret = 0;
    पूर्णाङ्क err = ENOMEM;
    सूची *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    यदि (err != 0) {
        err = त्रुटि_सं;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        गच्छ err;
    }
    यदि (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    dir = सूची_खोलो(path);
    यदि (dir == रिक्त) {
        err = EACCES;
        गच्छ err;
    }

    संरचना dirent *entry;
    वर्ण path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    यावत् ((entry = सूची_पढ़ो(dir)) != रिक्त) {
        ध्रुव वर्ण *name = entry->d_name;

        /* Skip special entries */
        यदि (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            अनुवर्त;

        यदि (!snprintf_safe(path_buf, परिमाण(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            गच्छ err;
        }
        यदि (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            अनुवर्त;

        /* Skip if the corresponding versioned directory is already included */
        प्रति (darray_size_t i = 0; i < versioned_गिनती; i++) {
            ध्रुव वर्ण * ध्रुव prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            यदि (म_भेद(name, prev_name) == 0)
                गच्छ next;
        }

        वर्ण *ext_path = strdup_safe(path_buf);
        यदि (!ext_path) {
            err = ENOMEM;
            गच्छ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    यदि (darray_size(*extensions) > versioned_गिनती) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुएँ(*extensions) + versioned_गिनती,
              darray_size(*extensions) - versioned_गिनती,
              परिमाण(*darray_वस्तुएँ(*extensions)), &compare_str);
        वर्ण **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_गिनती) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    प्रत्यागम ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    यदि (dir)
        बंद_सूची(dir);

    प्रत्यागम ret;
}
#endif

bool
rxkb_context_include_path_append_default(संरचना rxkb_context *ctx)
{
    वर्ण user_path[PATH_MAX];
    bool ret = false;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        प्रत्यागम false;
    }

    ध्रुव वर्ण * ध्रुव home = rxkb_context_getenv(ctx, "HOME");
    ध्रुव वर्ण * ध्रुव xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    यदि (xdg != रिक्त) {
        यदि (snprintf_safe(user_path, परिमाण(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } अन्यथा यदि (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        यदि (snprintf_safe(user_path, परिमाण(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    यदि (home != रिक्त) {
        यदि (snprintf_safe(user_path, परिमाण(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ध्रुव वर्ण * ध्रुव extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != रिक्त) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ध्रुव वर्ण *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    यदि (extensions_path == रिक्त)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    माप_प्रकार versioned_path_length = 0;
    यदि (extensions_path != रिक्त) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    यदि (extensions_path == रिक्त)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    यदि (extensions_path != रिक्त) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    वर्ण **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ध्रुव वर्ण * ध्रुव मूल = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ध्रुव bool has_मूल = rxkb_context_include_path_append(
        ctx, ((मूल != रिक्त) ? मूल: DFLT_XKB_CONFIG_मूल)
    );
    ret |= has_मूल;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    यदि (!has_मूल && (मूल == रिक्त || मूल[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूल,
                 ((मूल == रिक्त) ? DFLT_XKB_CONFIG_मूल : मूल));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_मूल);
    }

    प्रत्यागम ret;
}

bool
rxkb_context_parse_default_ruleset(संरचना rxkb_context *ctx)
{
    प्रत्यागम rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *ruleset)
{
    वर्ण **path;
    bool success = false;

    यदि (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        प्रत्यागम false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        वर्ण rules[PATH_MAX];

        यदि (snprintf_safe(rules, परिमाण(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            यदि (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        यदि (ctx->load_extra_rules_files &&
            snprintf_safe(rules, परिमाण(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            यदि (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    प्रत्यागम success;
}


शून्य
rxkb_context_set_user_data(संरचना rxkb_context *ctx, शून्य *userdata)
{
    ctx->userdata = userdata;
}

शून्य *
rxkb_context_get_user_data(संरचना rxkb_context *ctx)
{
    प्रत्यागम ctx->userdata;
}

स्थिर अन्तःस्थ bool
is_गाँठ(xml_गाँठ *गाँठ, ध्रुव वर्ण *name)
{
    प्रत्यागम गाँठ->type == XML_ELEMENT_गाँठ &&
        xmlStrEqual(गाँठ->name, (ध्रुव xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
स्थिर वर्ण *
extract_text(xml_गाँठ *गाँठ)
{
    xml_गाँठ *n;

    प्रति (n = गाँठ->children; n; n = n->next) {
        यदि (n->type == XML_TEXT_गाँठ)
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

#define config_item_new(popularity_) { \
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
parse_config_item(संरचना rxkb_context *ctx, xml_गाँठ *parent,
                  संरचना config_item *config)
{
    xml_गाँठ *गाँठ = रिक्त;
    xml_गाँठ *ci = रिक्त;

    प्रति (ci = parent->children; ci; ci = ci->next) {
        यदि (is_गाँठ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ध्रुव xmlChar*)"popularity");
            यदि (raw_popularity) {
                यदि (xmlStrEqual(raw_popularity, (ध्रुव xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                अन्यथा यदि (xmlStrEqual(raw_popularity, (ध्रुव xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                अन्यथा
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->पंक्ति, raw_popularity);
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
            प्रति (गाँठ = ci->children; गाँठ; गाँठ = गाँठ->next) {
                यदि (is_गाँठ(गाँठ, "name"))
                    config->name = extract_text(गाँठ);
                अन्यथा यदि (is_गाँठ(गाँठ, "description"))
                    config->description = extract_text(गाँठ);
                अन्यथा यदि (is_गाँठ(गाँठ, "shortDescription"))
                    config->brief = extract_text(गाँठ);
                अन्यथा यदि (is_गाँठ(गाँठ, "vendor"))
                    config->vendor = extract_text(गाँठ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            यदि (!config->name || !म_माप(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->पंक्ति);
                config_item_free(config);
                प्रत्यागम false;
            }

            प्रत्यागम true; /* only one configItem allowed in the dtd */
        }
    }

    प्रत्यागम false;
}

स्थिर शून्य
parse_model(संरचना rxkb_context *ctx, xml_गाँठ *model,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, model, &config)) {
        संरचना rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            यदि (streq(m->name, config.name)) {
                config_item_free(&config);
                प्रत्यागम;
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

स्थिर शून्य
parse_model_list(संरचना rxkb_context *ctx, xml_गाँठ *model_list,
                 गणना rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = रिक्त;

    प्रति (गाँठ = model_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "model"))
            parse_model(ctx, गाँठ, popularity);
    }
}

स्थिर शून्य
parse_language_list(xml_गाँठ *language_list, संरचना rxkb_layout *layout)
{
    xml_गाँठ *गाँठ = रिक्त;
    संरचना rxkb_iso639_code *code;

    प्रति (गाँठ = language_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "iso639Id")) {
            वर्ण *str = extract_text(गाँठ);
            संरचना rxkb_object *parent;

            यदि (!str || म_माप(str) != 3) {
                मुक्त(str);
                अनुवर्त;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

स्थिर शून्य
parse_country_list(xml_गाँठ *country_list, संरचना rxkb_layout *layout)
{
    xml_गाँठ *गाँठ = रिक्त;
    संरचना rxkb_iso3166_code *code;

    प्रति (गाँठ = country_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "iso3166Id")) {
            वर्ण *str = extract_text(गाँठ);
            संरचना rxkb_object *parent;

            यदि (!str || म_माप(str) != 2) {
                मुक्त(str);
                अनुवर्त;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

स्थिर शून्य
parse_variant(संरचना rxkb_context *ctx, संरचना rxkb_layout *l,
              xml_गाँठ *variant, गणना rxkb_popularity popularity)
{
    xml_गाँठ *ci;
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, variant, &config)) {
        संरचना rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            यदि (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                भङ्ग;
            }
        }

        यदि (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == रिक्त ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            प्रति (ci = variant->children; ci; ci = ci->next) {
                xml_गाँठ *गाँठ;

                यदि (!is_गाँठ(ci, "configItem"))
                    अनुवर्त;

                bool found_language_list = false;
                bool found_country_list = false;
                प्रति (गाँठ = ci->children; गाँठ; गाँठ = गाँठ->next) {
                    यदि (is_गाँठ(गाँठ, "languageList")) {
                        parse_language_list(गाँठ, v);
                        found_language_list = true;
                    }
                    यदि (is_गाँठ(गाँठ, "countryList")) {
                        parse_country_list(गाँठ, v);
                        found_country_list = true;
                    }
                }
                यदि (!found_language_list) {
                    // inherit from parent layout
                    संरचना rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        संरचना rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                यदि (!found_country_list) {
                    // inherit from parent layout
                    संरचना rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        संरचना rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
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
                   xml_गाँठ *variant_list, गणना rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = रिक्त;

    प्रति (गाँठ = variant_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "variant"))
            parse_variant(ctx, l, गाँठ, popularity);
    }
}

स्थिर शून्य
parse_layout(संरचना rxkb_context *ctx, xml_गाँठ *layout,
             गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_layout *l;
    xml_गाँठ *गाँठ = रिक्त;
    bool exists = false;

    यदि (!parse_config_item(ctx, layout, &config))
        प्रत्यागम;

    list_for_each(l, &ctx->layouts, base.link) {
        यदि (streq(l->name, config.name) && l->variant == रिक्त) {
            exists = true;
            भङ्ग;
        }
    }

    यदि (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = रिक्त;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } अन्यथा {
        config_item_free(&config);
    }

    प्रति (गाँठ = layout->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "variantList")) {
            parse_variant_list(ctx, l, गाँठ, popularity);
        }
        यदि (!exists && is_गाँठ(गाँठ, "configItem")) {
            xml_गाँठ *ll;
            प्रति (ll = गाँठ->children; ll; ll = ll->next) {
                यदि (is_गाँठ(ll, "languageList"))
                    parse_language_list(ll, l);
                यदि (is_गाँठ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

स्थिर शून्य
parse_layout_list(संरचना rxkb_context *ctx, xml_गाँठ *layout_list,
                  गणना rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = रिक्त;

    प्रति (गाँठ = layout_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "layout"))
            parse_layout(ctx, गाँठ, popularity);
    }
}

स्थिर शून्य
parse_option(संरचना rxkb_context *ctx, संरचना rxkb_option_group *group,
             xml_गाँठ *option, गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    यदि (parse_config_item(ctx, option, &config)) {
        संरचना rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            यदि (streq(o->name, config.name)) {
                config_item_free(&config);
                प्रत्यागम;
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

स्थिर शून्य
parse_group(संरचना rxkb_context *ctx, xml_गाँठ *group,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_option_group *g;
    xml_गाँठ *गाँठ = रिक्त;
    xmlChar *multiple;
    bool exists = false;

    यदि (!parse_config_item(ctx, group, &config))
        प्रत्यागम;

    list_for_each(g, &ctx->option_groups, base.link) {
        यदि (streq(g->name, config.name)) {
            exists = true;
            भङ्ग;
        }
    }

    यदि (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ध्रुव xmlChar*)"allowMultipleSelection");
        यदि (multiple && xmlStrEqual(multiple, (ध्रुव xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } अन्यथा {
        config_item_free(&config);
    }

    प्रति (गाँठ = group->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "option"))
            parse_option(ctx, g, गाँठ, popularity);
    }
}

स्थिर शून्य
parse_option_list(संरचना rxkb_context *ctx, xml_गाँठ *option_list,
                  गणना rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = रिक्त;

    प्रति (गाँठ = option_list->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "group"))
            parse_group(ctx, गाँठ, popularity);
    }
}

स्थिर शून्य
parse_rules_xml(संरचना rxkb_context *ctx, xml_गाँठ *मूल,
                गणना rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = रिक्त;

    प्रति (गाँठ = मूल->children; गाँठ; गाँठ = गाँठ->next) {
        यदि (is_गाँठ(गाँठ, "modelList"))
            parse_model_list(ctx, गाँठ, popularity);
        अन्यथा यदि (is_गाँठ(गाँठ, "layoutList"))
            parse_layout_list(ctx, गाँठ, popularity);
        अन्यथा यदि (is_गाँठ(गाँठ, "optionList"))
            parse_option_list(ctx, गाँठ, popularity);
    }
}

स्थिर शून्य
ATTR_PRINTF(2, 0)
xml_error_func(शून्य *ctx, ध्रुव वर्ण *संदेश, ...)
{
    स्थिर वर्ण buf[PATH_MAX];
    स्थिर माप_प्रकार slen = 0;
    बहु_सूची args;
    पूर्णाङ्क rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    बहु_शुरू(args, संदेश);
    rc = vsnprintf(&buf[slen], परिमाण(buf) - slen, संदेश, args);
    बहु_पूर्ण(args);

    /* This shouldn't really happen */
    यदि (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        स_रखो(buf, 0, परिमाण(buf));
        प्रत्यागम;
    }

    slen += (माप_प्रकार)rc;
    यदि (slen >= परिमाण(buf)) {
        /* truncated, let's flush this */
        buf[परिमाण(buf) - 1] = '\n';
        slen = परिमाण(buf);
    }

    /* We're assuming here that the last character is \n. */
    यदि (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        स_रखो(buf, 0, परिमाण(buf));
        slen = 0;
    }
}

#यदि HAVE_XML_CTXT_SET_ERRORHANDLER
स्थिर शून्य
xml_structured_error_func(शून्य *userData, ध्रुव xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

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
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_निवेश_Ptr pinput =
        xml_New_निवेश_From_Memory(रिक्त, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_निवेश_BUF_STATIC);
    यदि (!pinput)
        गच्छ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, रिक्त, रिक्त);
#अन्यथा
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_निवेश_Buffer_Ptr buf =
        xml_Parser_निवेश_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    यदि (!buf)
        गच्छ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(रिक्त, buf, XML_CHAR_ENCODING_UTF8);
#endif

    यदि (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
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
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#यदि HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    प्रत्यागम success;
}

स्थिर bool
parse(संरचना rxkb_context *ctx, ध्रुव वर्ण *path,
      गणना rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = रिक्त;
    xml_गाँठ *मूल = रिक्त;

    यदि (!check_eaccess(path, R_OK))
        प्रत्यागम false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    यदि (!xmlCtxt)
        प्रत्यागम false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#यदि HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#यदि !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, रिक्त, 0);
    यदि (!doc)
        गच्छ parse_error;

    यदि (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        गच्छ validate_error;
    }

    मूल = xml_Doc_Get_मूल_Element(doc);
    parse_rules_xml(ctx, मूल, popularity);

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
    xmlSetGenericErrorFunc(रिक्त, रिक्त);
#endif
#यदि HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, रिक्त, रिक्त);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    प्रत्यागम success;
}
