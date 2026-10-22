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

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES
    #अन्तर्भाव <limits.h>
    #जर HAVE_UNISTD_H && HAVE_DIRENT_H
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
    अक्षर *code;
};

संरचना rxkb_iso3166_code {
    संरचना rxkb_object base;
    अक्षर *code;
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

    darray(अक्षर *) includes;


    ATTR_PRINTF(3, 0) शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                     गणना rxkb_log_level level,
                                     स्थिरांक अक्षर *fmt, बहु_सूची args);
    गणना rxkb_log_level log_level;

    शून्य *userdata;
};

संरचना rxkb_model {
    संरचना rxkb_object base;

    अक्षर *name;
    अक्षर *vendor;
    अक्षर *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_layout {
    संरचना rxkb_object base;

    अक्षर *name;
    अक्षर *brief;
    अक्षर *description;
    अक्षर *variant;
    गणना rxkb_popularity popularity;

    संरचना list iso639s;  /* list of struct rxkb_iso639_code */
    संरचना list iso3166s; /* list of struct rxkb_iso3166_code */
};

संरचना rxkb_option_group {
    संरचना rxkb_object base;

    bool allow_multiple;
    संरचना list options; /* list of struct rxkb_options */
    अक्षर *name;
    अक्षर *description;
    गणना rxkb_popularity popularity;
};

संरचना rxkb_option {
    संरचना rxkb_object base;

    अक्षर *name;
    अक्षर *brief;
    अक्षर *description;
    गणना rxkb_popularity popularity;
    bool layout_specific;
};

स्थिर bool
parse(संरचना rxkb_context *ctx, स्थिरांक अक्षर *path,
      गणना rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
स्थिर शून्य
rxkb_log(संरचना rxkb_context *ctx, गणना rxkb_log_level level,
         स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची args;

    जर (ctx->log_level < level)
        परत;

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
    परत object; \
} \
संरचना type_ * type_##_unref(संरचना type_ *object) { \
    जर (!object) परत रिक्त; \
    प्रतिज्ञा(object->base.refcount >= 1); \
    जर (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        मुक्त(object); \
    } \
    परत रिक्त;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
स्थिर ओळीत संरचना type_ * type_##_create(संरचना rxkb_object *parent) { \
    संरचना type_ *t = सुस्मृति(1, आकार *t); \
    जर (t) \
        rxkb_object_init(&t->base, parent); \
    परत t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(संरचना type_ *object) { \
    परत object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, स्थिरांक अक्षर*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
संरचना type_ * type_##_first(संरचना parent_type_ *parent) { \
    संरचना type_ *o = रिक्त; \
    जर (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    परत o; \
} \
संरचना type_ * \
type_##_next(संरचना type_ *o) \
{ \
    संरचना parent_type_ *parent; \
    संरचना type_ *next; \
    parent = container_of(o->base.parent, संरचना parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    जर (list_is_last(&parent->parent_field_, &o->base.link)) \
        परत रिक्त; \
    परत next; \
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
    परत object;
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

    जर (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    परत code;
}

संरचना rxkb_iso639_code *
rxkb_iso639_code_next(संरचना rxkb_iso639_code *code)
{
    संरचना rxkb_iso639_code *next = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_of(code->base.parent, संरचना rxkb_layout, base);

    जर (list_is_last(&layout->iso639s, &code->base.link))
        परत रिक्त;

    next = list_first_entry(&code->base.link, code, base.link);

    परत next;
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

    जर (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    परत code;
}

संरचना rxkb_iso3166_code *
rxkb_iso3166_code_next(संरचना rxkb_iso3166_code *code)
{
    संरचना rxkb_iso3166_code *next = रिक्त;
    संरचना rxkb_layout *layout;

    layout = container_of(code->base.parent, संरचना rxkb_layout, base);

    जर (list_is_last(&layout->iso3166s, &code->base.link))
        परत रिक्त;

    next = list_first_entry(&code->base.link, code, base.link);

    परत next;
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
    परत object->layout_specific;
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
    परत g->allow_multiple;
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
    अक्षर **path;

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

स्थिर अक्षर *
rxkb_context_getenv(संरचना rxkb_context *ctx, स्थिरांक अक्षर *name)
{
    जर (ctx->use_secure_getenv) {
        परत secure_getenv(name);
    } अन्यथा {
        परत दो_पर्या(name);
    }
}


शून्य
rxkb_context_set_log_level(संरचना rxkb_context *ctx,
                           गणना rxkb_log_level level)
{
    ctx->log_level = level;
}

स्थिर स्थिरांक अक्षर *
log_level_to_prefix(गणना rxkb_log_level level)
{
    निवड (level) {
    स्थिती RXKB_LOG_LEVEL_DEBUG:
        परत "xkbregistry: DEBUG: ";
    स्थिती RXKB_LOG_LEVEL_INFO:
        परत "xkbregistry: INFO: ";
    स्थिती RXKB_LOG_LEVEL_WARNING:
        परत "xkbregistry: WARNING: ";
    स्थिती RXKB_LOG_LEVEL_ERROR:
        परत "xkbregistry: ERROR: ";
    स्थिती RXKB_LOG_LEVEL_CRITICAL:
        परत "xkbregistry: CRITICAL: ";
    पूर्वनिर्धारित:
        परत रिक्त;
    }
}

ATTR_PRINTF(3, 0) स्थिर शून्य
default_log_fn(संरचना rxkb_context *ctx, गणना rxkb_log_level level,
               स्थिरांक अक्षर *fmt, बहु_सूची args)
{
    स्थिरांक अक्षर *prefix = log_level_to_prefix(level);

    जर (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

स्थिर गणना rxkb_log_level
log_level(स्थिरांक अक्षर *level) {
    त्रुटि_सं = 0;
    अक्षर *endptr;
    स्थिरांक दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    जर (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        परत (गणना rxkb_log_level)lvl;

    जर (istreq_prefix("crit", level))
        परत RXKB_LOG_LEVEL_CRITICAL;
    जर (istreq_prefix("err", level))
        परत RXKB_LOG_LEVEL_ERROR;
    जर (istreq_prefix("warn", level))
        परत RXKB_LOG_LEVEL_WARNING;
    जर (istreq_prefix("info", level))
        परत RXKB_LOG_LEVEL_INFO;
    जर (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        परत RXKB_LOG_LEVEL_DEBUG;

    परत RXKB_LOG_LEVEL_ERROR;
}

संरचना rxkb_context *
rxkb_context_new(गणना rxkb_context_flags flags)
{
    संरचना rxkb_context *ctx = rxkb_context_create(रिक्त);
    स्थिरांक अक्षर *env;

    जर (!ctx)
        परत रिक्त;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    जर (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    स्थिर स्थिरांक गणना rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    जर (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        मुक्त(ctx);
        परत रिक्त;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    जर (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_मूळ);
        rxkb_context_unref(ctx);
        परत रिक्त;
    }

    परत ctx;
}

शून्य
rxkb_context_set_log_fn(संरचना rxkb_context *ctx,
                        शून्य (*log_fn)(संरचना rxkb_context *ctx,
                                       गणना rxkb_log_level level,
                                       स्थिरांक अक्षर *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(संरचना rxkb_context *ctx, स्थिरांक अक्षर *path)
{
    पूर्णांक err = 0;

    जर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        जा error;
    }

    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    जर (err != 0) {
        err = त्रुटि_सं;
        जा error;
    }
    जर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जा error;
    }

    जर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जा error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    अक्षर rules[PATH_MAX];
    जर (!snprintf_safe(rules, आकार(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                आकार(rules), path, DEFAULT_XKB_RULES);
        जा error;
    }

    अक्षर * स्थिरांक tmp = strdup(path);
    जर (!tmp) {
        err = ENOMEM;
        जा error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    परत true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    परत false;
}

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES

स्थिर पूर्णांक
compare_str(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    परत म_भेद(*(अक्षर **)a, *(अक्षर **) b);
}

स्थिर पूर्णांक
add_direct_subdirectories(संरचना rxkb_context *ctx, स्थिरांक अक्षर *path,
                          darray_string *extensions,
                          darray_size_t versioned_मोजणी,
                          माप_प्रकार versioned_path_length)
{
    पूर्णांक ret = 0;
    पूर्णांक err = ENOMEM;
    सूची *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    जर (err != 0) {
        err = त्रुटि_सं;
        जा err;
    }
    जर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जा err;
    }
    जर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जा err;
    }

    dir = सूची_खोलो(path);
    जर (dir == रिक्त) {
        err = EACCES;
        जा err;
    }

    संरचना dirent *entry;
    अक्षर path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    पर्यंत ((entry = सूची_पढ़ो(dir)) != रिक्त) {
        स्थिरांक अक्षर *name = entry->d_name;

        /* Skip special entries */
        जर (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            सुरूठेवा;

        जर (!snprintf_safe(path_buf, आकार(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            जा err;
        }
        जर (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            सुरूठेवा;

        /* Skip if the corresponding versioned directory is already included */
        साठी (darray_size_t i = 0; i < versioned_मोजणी; i++) {
            स्थिरांक अक्षर * स्थिरांक prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            जर (म_भेद(name, prev_name) == 0)
                जा next;
        }

        अक्षर *ext_path = strdup_safe(path_buf);
        जर (!ext_path) {
            err = ENOMEM;
            जा err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    जर (darray_size(*extensions) > versioned_मोजणी) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुसंच(*extensions) + versioned_मोजणी,
              darray_size(*extensions) - versioned_मोजणी,
              आकार(*darray_वस्तुसंच(*extensions)), &compare_str);
        अक्षर **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_मोजणी) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    परत ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    जर (dir)
        बंद_सूची(dir);

    परत ret;
}
#endif

bool
rxkb_context_include_path_append_default(संरचना rxkb_context *ctx)
{
    अक्षर user_path[PATH_MAX];
    bool ret = false;

    जर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        परत false;
    }

    स्थिरांक अक्षर * स्थिरांक home = rxkb_context_getenv(ctx, "HOME");
    स्थिरांक अक्षर * स्थिरांक xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    जर (xdg != रिक्त) {
        जर (snprintf_safe(user_path, आकार(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } अन्यथा जर (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        जर (snprintf_safe(user_path, आकार(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    जर (home != रिक्त) {
        जर (snprintf_safe(user_path, आकार(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    स्थिरांक अक्षर * स्थिरांक extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != रिक्त) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#जर HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    स्थिरांक अक्षर *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    जर (extensions_path == रिक्त)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    माप_प्रकार versioned_path_length = 0;
    जर (extensions_path != रिक्त) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    जर (extensions_path == रिक्त)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    जर (extensions_path != रिक्त) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    अक्षर **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    स्थिरांक अक्षर * स्थिरांक मूळ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    स्थिरांक bool has_मूळ = rxkb_context_include_path_append(
        ctx, ((मूळ != रिक्त) ? मूळ: DFLT_XKB_CONFIG_मूळ)
    );
    ret |= has_मूळ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    जर (!has_मूळ && (मूळ == रिक्त || मूळ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूळ,
                 ((मूळ == रिक्त) ? DFLT_XKB_CONFIG_मूळ : मूळ));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_मूळ);
    }

    परत ret;
}

bool
rxkb_context_parse_default_ruleset(संरचना rxkb_context *ctx)
{
    परत rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(संरचना rxkb_context *ctx, स्थिरांक अक्षर *ruleset)
{
    अक्षर **path;
    bool success = false;

    जर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        परत false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        अक्षर rules[PATH_MAX];

        जर (snprintf_safe(rules, आकार(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            जर (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        जर (ctx->load_extra_rules_files &&
            snprintf_safe(rules, आकार(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            जर (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    परत success;
}


शून्य
rxkb_context_set_user_data(संरचना rxkb_context *ctx, शून्य *userdata)
{
    ctx->userdata = userdata;
}

शून्य *
rxkb_context_get_user_data(संरचना rxkb_context *ctx)
{
    परत ctx->userdata;
}

स्थिर ओळीत bool
is_गाठ(xml_गाठ *गाठ, स्थिरांक अक्षर *name)
{
    परत गाठ->type == XML_ELEMENT_गाठ &&
        xmlStrEqual(गाठ->name, (स्थिरांक xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
स्थिर अक्षर *
extract_text(xml_गाठ *गाठ)
{
    xml_गाठ *n;

    साठी (n = गाठ->children; n; n = n->next) {
        जर (n->type == XML_TEXT_गाठ)
            परत (अक्षर *)xmlStrdup(n->content);
    }
    परत रिक्त;
}

/* Data from “configItem” node */
संरचना config_item {
    अक्षर *name;
    अक्षर *description;
    अक्षर *brief;
    अक्षर *vendor;
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
parse_config_item(संरचना rxkb_context *ctx, xml_गाठ *parent,
                  संरचना config_item *config)
{
    xml_गाठ *गाठ = रिक्त;
    xml_गाठ *ci = रिक्त;

    साठी (ci = parent->children; ci; ci = ci->next) {
        जर (is_गाठ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (स्थिरांक xmlChar*)"popularity");
            जर (raw_popularity) {
                जर (xmlStrEqual(raw_popularity, (स्थिरांक xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                अन्यथा जर (xmlStrEqual(raw_popularity, (स्थिरांक xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                अन्यथा
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->ओळ, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (स्थिरांक xmlChar*)"layout-specific");
            जर (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (स्थिरांक xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            साठी (गाठ = ci->children; गाठ; गाठ = गाठ->next) {
                जर (is_गाठ(गाठ, "name"))
                    config->name = extract_text(गाठ);
                अन्यथा जर (is_गाठ(गाठ, "description"))
                    config->description = extract_text(गाठ);
                अन्यथा जर (is_गाठ(गाठ, "shortDescription"))
                    config->brief = extract_text(गाठ);
                अन्यथा जर (is_गाठ(गाठ, "vendor"))
                    config->vendor = extract_text(गाठ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            जर (!config->name || !म_माप(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->ओळ);
                config_item_free(config);
                परत false;
            }

            परत true; /* only one configItem allowed in the dtd */
        }
    }

    परत false;
}

स्थिर शून्य
parse_model(संरचना rxkb_context *ctx, xml_गाठ *model,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    जर (parse_config_item(ctx, model, &config)) {
        संरचना rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            जर (streq(m->name, config.name)) {
                config_item_free(&config);
                परत;
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
parse_model_list(संरचना rxkb_context *ctx, xml_गाठ *model_list,
                 गणना rxkb_popularity popularity)
{
    xml_गाठ *गाठ = रिक्त;

    साठी (गाठ = model_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "model"))
            parse_model(ctx, गाठ, popularity);
    }
}

स्थिर शून्य
parse_language_list(xml_गाठ *language_list, संरचना rxkb_layout *layout)
{
    xml_गाठ *गाठ = रिक्त;
    संरचना rxkb_iso639_code *code;

    साठी (गाठ = language_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "iso639Id")) {
            अक्षर *str = extract_text(गाठ);
            संरचना rxkb_object *parent;

            जर (!str || म_माप(str) != 3) {
                मुक्त(str);
                सुरूठेवा;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

स्थिर शून्य
parse_country_list(xml_गाठ *country_list, संरचना rxkb_layout *layout)
{
    xml_गाठ *गाठ = रिक्त;
    संरचना rxkb_iso3166_code *code;

    साठी (गाठ = country_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "iso3166Id")) {
            अक्षर *str = extract_text(गाठ);
            संरचना rxkb_object *parent;

            जर (!str || म_माप(str) != 2) {
                मुक्त(str);
                सुरूठेवा;
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
              xml_गाठ *variant, गणना rxkb_popularity popularity)
{
    xml_गाठ *ci;
    संरचना config_item config = config_item_new(popularity);

    जर (parse_config_item(ctx, variant, &config)) {
        संरचना rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            जर (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                थांबा;
            }
        }

        जर (!exists) {
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

            साठी (ci = variant->children; ci; ci = ci->next) {
                xml_गाठ *गाठ;

                जर (!is_गाठ(ci, "configItem"))
                    सुरूठेवा;

                bool found_language_list = false;
                bool found_country_list = false;
                साठी (गाठ = ci->children; गाठ; गाठ = गाठ->next) {
                    जर (is_गाठ(गाठ, "languageList")) {
                        parse_language_list(गाठ, v);
                        found_language_list = true;
                    }
                    जर (is_गाठ(गाठ, "countryList")) {
                        parse_country_list(गाठ, v);
                        found_country_list = true;
                    }
                }
                जर (!found_language_list) {
                    // inherit from parent layout
                    संरचना rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        संरचना rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                जर (!found_country_list) {
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
                   xml_गाठ *variant_list, गणना rxkb_popularity popularity)
{
    xml_गाठ *गाठ = रिक्त;

    साठी (गाठ = variant_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "variant"))
            parse_variant(ctx, l, गाठ, popularity);
    }
}

स्थिर शून्य
parse_layout(संरचना rxkb_context *ctx, xml_गाठ *layout,
             गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_layout *l;
    xml_गाठ *गाठ = रिक्त;
    bool exists = false;

    जर (!parse_config_item(ctx, layout, &config))
        परत;

    list_for_each(l, &ctx->layouts, base.link) {
        जर (streq(l->name, config.name) && l->variant == रिक्त) {
            exists = true;
            थांबा;
        }
    }

    जर (!exists) {
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

    साठी (गाठ = layout->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "variantList")) {
            parse_variant_list(ctx, l, गाठ, popularity);
        }
        जर (!exists && is_गाठ(गाठ, "configItem")) {
            xml_गाठ *ll;
            साठी (ll = गाठ->children; ll; ll = ll->next) {
                जर (is_गाठ(ll, "languageList"))
                    parse_language_list(ll, l);
                जर (is_गाठ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

स्थिर शून्य
parse_layout_list(संरचना rxkb_context *ctx, xml_गाठ *layout_list,
                  गणना rxkb_popularity popularity)
{
    xml_गाठ *गाठ = रिक्त;

    साठी (गाठ = layout_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "layout"))
            parse_layout(ctx, गाठ, popularity);
    }
}

स्थिर शून्य
parse_option(संरचना rxkb_context *ctx, संरचना rxkb_option_group *group,
             xml_गाठ *option, गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);

    जर (parse_config_item(ctx, option, &config)) {
        संरचना rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            जर (streq(o->name, config.name)) {
                config_item_free(&config);
                परत;
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
parse_group(संरचना rxkb_context *ctx, xml_गाठ *group,
            गणना rxkb_popularity popularity)
{
    संरचना config_item config = config_item_new(popularity);
    संरचना rxkb_option_group *g;
    xml_गाठ *गाठ = रिक्त;
    xmlChar *multiple;
    bool exists = false;

    जर (!parse_config_item(ctx, group, &config))
        परत;

    list_for_each(g, &ctx->option_groups, base.link) {
        जर (streq(g->name, config.name)) {
            exists = true;
            थांबा;
        }
    }

    जर (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (स्थिरांक xmlChar*)"allowMultipleSelection");
        जर (multiple && xmlStrEqual(multiple, (स्थिरांक xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } अन्यथा {
        config_item_free(&config);
    }

    साठी (गाठ = group->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "option"))
            parse_option(ctx, g, गाठ, popularity);
    }
}

स्थिर शून्य
parse_option_list(संरचना rxkb_context *ctx, xml_गाठ *option_list,
                  गणना rxkb_popularity popularity)
{
    xml_गाठ *गाठ = रिक्त;

    साठी (गाठ = option_list->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "group"))
            parse_group(ctx, गाठ, popularity);
    }
}

स्थिर शून्य
parse_rules_xml(संरचना rxkb_context *ctx, xml_गाठ *मूळ,
                गणना rxkb_popularity popularity)
{
    xml_गाठ *गाठ = रिक्त;

    साठी (गाठ = मूळ->children; गाठ; गाठ = गाठ->next) {
        जर (is_गाठ(गाठ, "modelList"))
            parse_model_list(ctx, गाठ, popularity);
        अन्यथा जर (is_गाठ(गाठ, "layoutList"))
            parse_layout_list(ctx, गाठ, popularity);
        अन्यथा जर (is_गाठ(गाठ, "optionList"))
            parse_option_list(ctx, गाठ, popularity);
    }
}

स्थिर शून्य
ATTR_PRINTF(2, 0)
xml_error_func(शून्य *ctx, स्थिरांक अक्षर *संदेश, ...)
{
    स्थिर अक्षर buf[PATH_MAX];
    स्थिर माप_प्रकार slen = 0;
    बहु_सूची args;
    पूर्णांक rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    बहु_शुरू(args, संदेश);
    rc = vsnprintf(&buf[slen], आकार(buf) - slen, संदेश, args);
    बहु_पूर्ण(args);

    /* This shouldn't really happen */
    जर (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        स_रखो(buf, 0, आकार(buf));
        परत;
    }

    slen += (माप_प्रकार)rc;
    जर (slen >= आकार(buf)) {
        /* truncated, let's flush this */
        buf[आकार(buf) - 1] = '\n';
        slen = आकार(buf);
    }

    /* We're assuming here that the last character is \n. */
    जर (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        स_रखो(buf, 0, आकार(buf));
        slen = 0;
    }
}

#जर HAVE_XML_CTXT_SET_ERRORHANDLER
स्थिर शून्य
xml_structured_error_func(शून्य *userData, स्थिरांक xmlError * error)
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
    स्थिरांक अक्षर dtdstr[] =
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

#जर HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    जर (!xmlCtxt)
        परत false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_आदान_Ptr pinput =
        xml_New_आदान_From_Memory(रिक्त, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_आदान_BUF_STATIC);
    जर (!pinput)
        जा dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, रिक्त, रिक्त);
#अन्यथा
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_आदान_Buffer_Ptr buf =
        xml_Parser_आदान_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    जर (!buf)
        जा dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(रिक्त, buf, XML_CHAR_ENCODING_UTF8);
#endif

    जर (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        जा dtd_error;
    }

#जर HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#अन्यथा
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    जर (!dtdvalid) {
        success = false;
        जा ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#जर HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    परत success;
}

स्थिर bool
parse(संरचना rxkb_context *ctx, स्थिरांक अक्षर *path,
      गणना rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = रिक्त;
    xml_गाठ *मूळ = रिक्त;

    जर (!check_eaccess(path, R_OK))
        परत false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    जर (!xmlCtxt)
        परत false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#जर HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#जर !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, रिक्त, 0);
    जर (!doc)
        जा parse_error;

    जर (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        जा validate_error;
    }

    मूळ = xml_Doc_Get_मूळ_Element(doc);
    parse_rules_xml(ctx, मूळ, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#जर !HAVE_XML_CTXT_PARSE_DTD
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
#जर HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, रिक्त, रिक्त);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    परत success;
}
