/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <sys/stat.h>
#समावेश <fcntl.h>
#समावेश <stdbool.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <stdint.h>
#समावेश <libxml/parser.h>

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES
    #समावेश <limits.h>
    #अगर HAVE_UNISTD_H && HAVE_DIRENT_H
        #समावेश <dirent.h>
        #समावेश <unistd.h>
    #पूर्ण_अगर
#पूर्ण_अगर

#समावेश "xkbcommon/xkbregistry.h"
#समावेश "messages-codes.h"
#समावेश "darray.h"
#समावेश "utils.h"
#समावेश "util-list.h"
#समावेश "util-mem.h"

काष्ठा rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
काष्ठा rxkb_object {
    काष्ठा rxkb_object *parent;
    uint32_t refcount;
    काष्ठा list link;
};

काष्ठा rxkb_iso639_code {
    काष्ठा rxkb_object base;
    अक्षर *code;
};

काष्ठा rxkb_iso3166_code {
    काष्ठा rxkb_object base;
    अक्षर *code;
};

क्रमागत context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

काष्ठा rxkb_context {
    काष्ठा rxkb_object base;
    क्रमागत context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    काष्ठा list models;         /* list of struct rxkb_models */
    काष्ठा list layouts;        /* list of struct rxkb_layouts */
    काष्ठा list option_groups;  /* list of struct rxkb_option_group */

    darray(अक्षर *) includes;


    ATTR_PRINTF(3, 0) व्योम (*log_fn)(काष्ठा rxkb_context *ctx,
                                     क्रमागत rxkb_log_level level,
                                     स्थिर अक्षर *fmt, बहु_सूची args);
    क्रमागत rxkb_log_level log_level;

    व्योम *userdata;
};

काष्ठा rxkb_model {
    काष्ठा rxkb_object base;

    अक्षर *name;
    अक्षर *vendor;
    अक्षर *description;
    क्रमागत rxkb_popularity popularity;
};

काष्ठा rxkb_layout {
    काष्ठा rxkb_object base;

    अक्षर *name;
    अक्षर *brief;
    अक्षर *description;
    अक्षर *variant;
    क्रमागत rxkb_popularity popularity;

    काष्ठा list iso639s;  /* list of struct rxkb_iso639_code */
    काष्ठा list iso3166s; /* list of struct rxkb_iso3166_code */
};

काष्ठा rxkb_option_group {
    काष्ठा rxkb_object base;

    bool allow_multiple;
    काष्ठा list options; /* list of struct rxkb_options */
    अक्षर *name;
    अक्षर *description;
    क्रमागत rxkb_popularity popularity;
};

काष्ठा rxkb_option {
    काष्ठा rxkb_object base;

    अक्षर *name;
    अक्षर *brief;
    अक्षर *description;
    क्रमागत rxkb_popularity popularity;
    bool layout_specific;
};

अटल bool
parse(काष्ठा rxkb_context *ctx, स्थिर अक्षर *path,
      क्रमागत rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
अटल व्योम
rxkb_log(काष्ठा rxkb_context *ctx, क्रमागत rxkb_log_level level,
         स्थिर अक्षर *fmt, ...)
{
    बहु_सूची args;

    अगर (ctx->log_level < level)
        वापस;

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
#घोषणा rxkb_log_with_code(ctx, level, संदेश_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(संदेश_id, fmt), ##__VA_ARGS__)
#घोषणा log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#घोषणा log_info(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#घोषणा log_warn(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#घोषणा log_err(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#घोषणा log_wsgo(ctx, id, ...) \
    rxkb_log_with_code((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#घोषणा DECLARE_REF_UNREF_FOR_TYPE(type_) \
काष्ठा type_ * type_##_ref(काष्ठा type_ *object) { \
    rxkb_object_ref(&object->base); \
    वापस object; \
} \
काष्ठा type_ * type_##_unref(काष्ठा type_ *object) { \
    अगर (!object) वापस शून्य; \
    निश्चित(object->base.refcount >= 1); \
    अगर (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        मुक्त(object); \
    } \
    वापस शून्य;\
}

#घोषणा DECLARE_CREATE_FOR_TYPE(type_) \
अटल अंतरभूत काष्ठा type_ * type_##_create(काष्ठा rxkb_object *parent) { \
    काष्ठा type_ *t = सुस्मृति(1, माप *t); \
    अगर (t) \
        rxkb_object_init(&t->base, parent); \
    वापस t; \
}

#घोषणा DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(काष्ठा type_ *object) { \
    वापस object->field_; \
}

#घोषणा DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, स्थिर अक्षर*)

#घोषणा DECLARE_FIRST_अग्र_FOR_TYPE(type_, parent_type_, parent_field_) \
काष्ठा type_ * type_##_first(काष्ठा parent_type_ *parent) { \
    काष्ठा type_ *o = शून्य; \
    अगर (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    वापस o; \
} \
काष्ठा type_ * \
type_##_next(काष्ठा type_ *o) \
{ \
    काष्ठा parent_type_ *parent; \
    काष्ठा type_ *अग्र; \
    parent = container_of(o->base.parent, काष्ठा parent_type_, base); \
    अग्र = list_first_entry(&o->base.link, o, base.link); \
    अगर (list_is_last(&parent->parent_field_, &o->base.link)) \
        वापस शून्य; \
    वापस अग्र; \
}

अटल व्योम
rxkb_object_init(काष्ठा rxkb_object *object, काष्ठा rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

अटल व्योम *
rxkb_object_ref(काष्ठा rxkb_object *object)
{
    निश्चित(object->refcount >= 1);
    ++object->refcount;
    वापस object;
}

अटल व्योम
rxkb_iso639_code_destroy(काष्ठा rxkb_iso639_code *code)
{
    मुक्त(code->code);
}

काष्ठा rxkb_iso639_code *
rxkb_layout_get_iso639_first(काष्ठा rxkb_layout *layout)
{
    काष्ठा rxkb_iso639_code *code = शून्य;

    अगर (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    वापस code;
}

काष्ठा rxkb_iso639_code *
rxkb_iso639_code_अग्र(काष्ठा rxkb_iso639_code *code)
{
    काष्ठा rxkb_iso639_code *अग्र = शून्य;
    काष्ठा rxkb_layout *layout;

    layout = container_of(code->base.parent, काष्ठा rxkb_layout, base);

    अगर (list_is_last(&layout->iso639s, &code->base.link))
        वापस शून्य;

    अग्र = list_first_entry(&code->base.link, code, base.link);

    वापस अग्र;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

अटल व्योम
rxkb_iso3166_code_destroy(काष्ठा rxkb_iso3166_code *code)
{
    मुक्त(code->code);
}

काष्ठा rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(काष्ठा rxkb_layout *layout)
{
    काष्ठा rxkb_iso3166_code *code = शून्य;

    अगर (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    वापस code;
}

काष्ठा rxkb_iso3166_code *
rxkb_iso3166_code_अग्र(काष्ठा rxkb_iso3166_code *code)
{
    काष्ठा rxkb_iso3166_code *अग्र = शून्य;
    काष्ठा rxkb_layout *layout;

    layout = container_of(code->base.parent, काष्ठा rxkb_layout, base);

    अगर (list_is_last(&layout->iso3166s, &code->base.link))
        वापस शून्य;

    अग्र = list_first_entry(&code->base.link, code, base.link);

    वापस अग्र;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

अटल व्योम
rxkb_option_destroy(काष्ठा rxkb_option *o)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, क्रमागत rxkb_popularity);
bool rxkb_option_is_layout_specific(काष्ठा rxkb_option *object) {
    वापस object->layout_specific;
}
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_option, rxkb_option_group, options);

अटल व्योम
rxkb_layout_destroy(काष्ठा rxkb_layout *l)
{
    काष्ठा rxkb_iso639_code *iso639, *tmp_639;
    काष्ठा rxkb_iso3166_code *iso3166, *tmp_3166;

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, क्रमागत rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

अटल व्योम
rxkb_model_destroy(काष्ठा rxkb_model *m)
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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, क्रमागत rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_model, rxkb_context, models);

अटल व्योम
rxkb_option_group_destroy(काष्ठा rxkb_option_group *og)
{
    काष्ठा rxkb_option *o, *otmp;

    मुक्त(og->name);
    मुक्त(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(काष्ठा rxkb_option_group *g)
{
    वापस g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, क्रमागत rxkb_popularity);
DECLARE_FIRST_अग्र_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

अटल व्योम
rxkb_context_destroy(काष्ठा rxkb_context *ctx)
{
    काष्ठा rxkb_model *m, *mtmp;
    काष्ठा rxkb_layout *l, *ltmp;
    काष्ठा rxkb_option_group *og, *ogtmp;
    अक्षर **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    निश्चित(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    निश्चित(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    निश्चित(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        मुक्त(*path);
    darray_free(ctx->includes);

    निश्चित(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, क्रमागत rxkb_log_level);

अटल अक्षर *
rxkb_context_getenv(काष्ठा rxkb_context *ctx, स्थिर अक्षर *name)
{
    अगर (ctx->use_secure_getenv) {
        वापस secure_getenv(name);
    } अन्यथा {
        वापस दो_पर्या(name);
    }
}


व्योम
rxkb_context_set_log_level(काष्ठा rxkb_context *ctx,
                           क्रमागत rxkb_log_level level)
{
    ctx->log_level = level;
}

अटल स्थिर अक्षर *
log_level_to_prefix(क्रमागत rxkb_log_level level)
{
    चयन (level) {
    हाल RXKB_LOG_LEVEL_DEBUG:
        वापस "xkbregistry: DEBUG: ";
    हाल RXKB_LOG_LEVEL_INFO:
        वापस "xkbregistry: INFO: ";
    हाल RXKB_LOG_LEVEL_WARNING:
        वापस "xkbregistry: WARNING: ";
    हाल RXKB_LOG_LEVEL_ERROR:
        वापस "xkbregistry: ERROR: ";
    हाल RXKB_LOG_LEVEL_CRITICAL:
        वापस "xkbregistry: CRITICAL: ";
    शेष:
        वापस शून्य;
    }
}

ATTR_PRINTF(3, 0) अटल व्योम
default_log_fn(काष्ठा rxkb_context *ctx, क्रमागत rxkb_log_level level,
               स्थिर अक्षर *fmt, बहु_सूची args)
{
    स्थिर अक्षर *prefix = log_level_to_prefix(level);

    अगर (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

अटल क्रमागत rxkb_log_level
log_level(स्थिर अक्षर *level) {
    त्रुटि_सं = 0;
    अक्षर *endptr;
    स्थिर दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    अगर (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        वापस (क्रमागत rxkb_log_level)lvl;

    अगर (istreq_prefix("crit", level))
        वापस RXKB_LOG_LEVEL_CRITICAL;
    अगर (istreq_prefix("err", level))
        वापस RXKB_LOG_LEVEL_ERROR;
    अगर (istreq_prefix("warn", level))
        वापस RXKB_LOG_LEVEL_WARNING;
    अगर (istreq_prefix("info", level))
        वापस RXKB_LOG_LEVEL_INFO;
    अगर (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        वापस RXKB_LOG_LEVEL_DEBUG;

    वापस RXKB_LOG_LEVEL_ERROR;
}

काष्ठा rxkb_context *
rxkb_context_new(क्रमागत rxkb_context_flags flags)
{
    काष्ठा rxkb_context *ctx = rxkb_context_create(शून्य);
    स्थिर अक्षर *env;

    अगर (!ctx)
        वापस शून्य;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    अगर (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    अटल स्थिर क्रमागत rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    अगर (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        मुक्त(ctx);
        वापस शून्य;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    अगर (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_मूल);
        rxkb_context_unref(ctx);
        वापस शून्य;
    }

    वापस ctx;
}

व्योम
rxkb_context_set_log_fn(काष्ठा rxkb_context *ctx,
                        व्योम (*log_fn)(काष्ठा rxkb_context *ctx,
                                       क्रमागत rxkb_log_level level,
                                       स्थिर अक्षर *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(काष्ठा rxkb_context *ctx, स्थिर अक्षर *path)
{
    पूर्णांक err = 0;

    अगर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        जाओ error;
    }

    काष्ठा stat stat_buf;
    err = stat(path, &stat_buf);
    अगर (err != 0) {
        err = त्रुटि_सं;
        जाओ error;
    }
    अगर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जाओ error;
    }

    अगर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जाओ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    अक्षर rules[PATH_MAX];
    अगर (!snprintf_safe(rules, माप(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                माप(rules), path, DEFAULT_XKB_RULES);
        जाओ error;
    }

    अक्षर * स्थिर tmp = strdup(path);
    अगर (!tmp) {
        err = ENOMEM;
        जाओ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    वापस true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    वापस false;
}

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES

अटल पूर्णांक
compare_str(स्थिर व्योम *a, स्थिर व्योम *b)
{
    वापस म_भेद(*(अक्षर **)a, *(अक्षर **) b);
}

अटल पूर्णांक
add_direct_subdirectories(काष्ठा rxkb_context *ctx, स्थिर अक्षर *path,
                          darray_string *extensions,
                          darray_आकार_t versioned_गिनती,
                          माप_प्रकार versioned_path_लंबाई)
{
    पूर्णांक ret = 0;
    पूर्णांक err = ENOMEM;
    सूची *dir = शून्य;

    /* Check extensions parent directory */
    काष्ठा stat stat_buf;
    err = stat(path, &stat_buf);
    अगर (err != 0) {
        err = त्रुटि_सं;
        जाओ err;
    }
    अगर (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        जाओ err;
    }
    अगर (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        जाओ err;
    }

    dir = सूची_खोलो(path);
    अगर (dir == शून्य) {
        err = EACCES;
        जाओ err;
    }

    काष्ठा dirent *entry;
    अक्षर path_buf[PATH_MAX] = "";
    versioned_path_लंबाई++; /* Additional final ‘/’ */
    जबतक ((entry = सूची_पढ़ो(dir)) != शून्य) {
        स्थिर अक्षर *name = entry->d_name;

        /* Skip special entries */
        अगर (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            जारी;

        अगर (!snprintf_safe(path_buf, माप(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            जाओ err;
        }
        अगर (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            जारी;

        /* Skip if the corresponding versioned directory is already included */
        क्रम (darray_आकार_t i = 0; i < versioned_गिनती; i++) {
            स्थिर अक्षर * स्थिर पूर्व_name = darray_item(*extensions, i)
                                         + versioned_path_लंबाई;
            अगर (म_भेद(name, पूर्व_name) == 0)
                जाओ अग्र;
        }

        अक्षर *ext_path = strdup_safe(path_buf);
        अगर (!ext_path) {
            err = ENOMEM;
            जाओ err;
        }

        darray_append(*extensions, ext_path);
अग्र:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    अगर (darray_आकार(*extensions) > versioned_गिनती) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुएँ(*extensions) + versioned_गिनती,
              darray_आकार(*extensions) - versioned_गिनती,
              माप(*darray_वस्तुएँ(*extensions)), &compare_str);
        अक्षर **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_गिनती) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    वापस ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    अगर (dir)
        बंद_सूची(dir);

    वापस ret;
}
#पूर्ण_अगर

bool
rxkb_context_include_path_append_default(काष्ठा rxkb_context *ctx)
{
    अक्षर user_path[PATH_MAX];
    bool ret = false;

    अगर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        वापस false;
    }

    स्थिर अक्षर * स्थिर home = rxkb_context_getenv(ctx, "HOME");
    स्थिर अक्षर * स्थिर xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    अगर (xdg != शून्य) {
        अगर (snprintf_safe(user_path, माप(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } अन्यथा अगर (home != शून्य) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        अगर (snprintf_safe(user_path, माप(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    अगर (home != शून्य) {
        अगर (snprintf_safe(user_path, माप(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    स्थिर अक्षर * स्थिर extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != शून्य) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#अगर HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    स्थिर अक्षर *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#अगर_घोषित DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    अगर (extensions_path == शून्य)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#पूर्ण_अगर
    माप_प्रकार versioned_path_लंबाई = 0;
    अगर (extensions_path != शून्य) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_लंबाई = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#अगर_घोषित DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    अगर (extensions_path == शून्य)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#पूर्ण_अगर
    अगर (extensions_path != शून्य) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_आकार(extensions),
                                         versioned_path_लंबाई);
    }

    अक्षर **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#पूर्ण_अगर

    /* Canonical XKB root */
    स्थिर अक्षर * स्थिर मूल = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    स्थिर bool has_मूल = rxkb_context_include_path_append(
        ctx, ((मूल != शून्य) ? मूल: DFLT_XKB_CONFIG_मूल)
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
    अगर (!has_मूल && (मूल == शून्य || मूल[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूल,
                 ((मूल == शून्य) ? DFLT_XKB_CONFIG_मूल : मूल));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_मूल);
    }

    वापस ret;
}

bool
rxkb_context_parse_default_ruleset(काष्ठा rxkb_context *ctx)
{
    वापस rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(काष्ठा rxkb_context *ctx, स्थिर अक्षर *ruleset)
{
    अक्षर **path;
    bool success = false;

    अगर (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        वापस false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        अक्षर rules[PATH_MAX];

        अगर (snprintf_safe(rules, माप(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            अगर (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        अगर (ctx->load_extra_rules_files &&
            snprintf_safe(rules, माप(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            अगर (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    वापस success;
}


व्योम
rxkb_context_set_user_तथ्य(काष्ठा rxkb_context *ctx, व्योम *userdata)
{
    ctx->userdata = userdata;
}

व्योम *
rxkb_context_get_user_तथ्य(काष्ठा rxkb_context *ctx)
{
    वापस ctx->userdata;
}

अटल अंतरभूत bool
is_गाँठ(xml_गाँठ *गाँठ, स्थिर अक्षर *name)
{
    वापस गाँठ->type == XML_ELEMENT_गाँठ &&
        xmlStrEqual(गाँठ->name, (स्थिर xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
अटल अक्षर *
extract_text(xml_गाँठ *गाँठ)
{
    xml_गाँठ *n;

    क्रम (n = गाँठ->children; n; n = n->अग्र) {
        अगर (n->type == XML_TEXT_गाँठ)
            वापस (अक्षर *)xmlStrdup(n->content);
    }
    वापस शून्य;
}

/* Data from “configItem” node */
काष्ठा config_item {
    अक्षर *name;
    अक्षर *description;
    अक्षर *brief;
    अक्षर *vendor;
    क्रमागत rxkb_popularity popularity;
    bool layout_specific;
};

#घोषणा config_item_new(popularity_) { \
    .name = शून्य, \
    .description = शून्य, \
    .brief = शून्य, \
    .vendor = शून्य, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

अटल व्योम
config_item_free(काष्ठा config_item *config) {
    मुक्त(config->name);
    मुक्त(config->description);
    मुक्त(config->brief);
    मुक्त(config->vendor);
}

अटल bool
parse_config_item(काष्ठा rxkb_context *ctx, xml_गाँठ *parent,
                  काष्ठा config_item *config)
{
    xml_गाँठ *गाँठ = शून्य;
    xml_गाँठ *ci = शून्य;

    क्रम (ci = parent->children; ci; ci = ci->अग्र) {
        अगर (is_गाँठ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (स्थिर xmlChar*)"popularity");
            अगर (raw_popularity) {
                अगर (xmlStrEqual(raw_popularity, (स्थिर xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                अन्यथा अगर (xmlStrEqual(raw_popularity, (स्थिर xmlChar*)"exotic"))
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
                xmlGetProp(ci, (स्थिर xmlChar*)"layout-specific");
            अगर (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (स्थिर xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            क्रम (गाँठ = ci->children; गाँठ; गाँठ = गाँठ->अग्र) {
                अगर (is_गाँठ(गाँठ, "name"))
                    config->name = extract_text(गाँठ);
                अन्यथा अगर (is_गाँठ(गाँठ, "description"))
                    config->description = extract_text(गाँठ);
                अन्यथा अगर (is_गाँठ(गाँठ, "shortDescription"))
                    config->brief = extract_text(गाँठ);
                अन्यथा अगर (is_गाँठ(गाँठ, "vendor"))
                    config->vendor = extract_text(गाँठ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            अगर (!config->name || !म_माप(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->पंक्ति);
                config_item_free(config);
                वापस false;
            }

            वापस true; /* only one configItem allowed in the dtd */
        }
    }

    वापस false;
}

अटल व्योम
parse_model(काष्ठा rxkb_context *ctx, xml_गाँठ *model,
            क्रमागत rxkb_popularity popularity)
{
    काष्ठा config_item config = config_item_new(popularity);

    अगर (parse_config_item(ctx, model, &config)) {
        काष्ठा rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            अगर (streq(m->name, config.name)) {
                config_item_free(&config);
                वापस;
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

अटल व्योम
parse_model_list(काष्ठा rxkb_context *ctx, xml_गाँठ *model_list,
                 क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = शून्य;

    क्रम (गाँठ = model_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "model"))
            parse_model(ctx, गाँठ, popularity);
    }
}

अटल व्योम
parse_language_list(xml_गाँठ *language_list, काष्ठा rxkb_layout *layout)
{
    xml_गाँठ *गाँठ = शून्य;
    काष्ठा rxkb_iso639_code *code;

    क्रम (गाँठ = language_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "iso639Id")) {
            अक्षर *str = extract_text(गाँठ);
            काष्ठा rxkb_object *parent;

            अगर (!str || म_माप(str) != 3) {
                मुक्त(str);
                जारी;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

अटल व्योम
parse_country_list(xml_गाँठ *country_list, काष्ठा rxkb_layout *layout)
{
    xml_गाँठ *गाँठ = शून्य;
    काष्ठा rxkb_iso3166_code *code;

    क्रम (गाँठ = country_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "iso3166Id")) {
            अक्षर *str = extract_text(गाँठ);
            काष्ठा rxkb_object *parent;

            अगर (!str || म_माप(str) != 2) {
                मुक्त(str);
                जारी;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

अटल व्योम
parse_variant(काष्ठा rxkb_context *ctx, काष्ठा rxkb_layout *l,
              xml_गाँठ *variant, क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *ci;
    काष्ठा config_item config = config_item_new(popularity);

    अगर (parse_config_item(ctx, variant, &config)) {
        काष्ठा rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            अगर (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                अवरोध;
            }
        }

        अगर (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == शून्य ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            क्रम (ci = variant->children; ci; ci = ci->अग्र) {
                xml_गाँठ *गाँठ;

                अगर (!is_गाँठ(ci, "configItem"))
                    जारी;

                bool found_language_list = false;
                bool found_country_list = false;
                क्रम (गाँठ = ci->children; गाँठ; गाँठ = गाँठ->अग्र) {
                    अगर (is_गाँठ(गाँठ, "languageList")) {
                        parse_language_list(गाँठ, v);
                        found_language_list = true;
                    }
                    अगर (is_गाँठ(गाँठ, "countryList")) {
                        parse_country_list(गाँठ, v);
                        found_country_list = true;
                    }
                }
                अगर (!found_language_list) {
                    // inherit from parent layout
                    काष्ठा rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        काष्ठा rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                अगर (!found_country_list) {
                    // inherit from parent layout
                    काष्ठा rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        काष्ठा rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
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

अटल व्योम
parse_variant_list(काष्ठा rxkb_context *ctx, काष्ठा rxkb_layout *l,
                   xml_गाँठ *variant_list, क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = शून्य;

    क्रम (गाँठ = variant_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "variant"))
            parse_variant(ctx, l, गाँठ, popularity);
    }
}

अटल व्योम
parse_layout(काष्ठा rxkb_context *ctx, xml_गाँठ *layout,
             क्रमागत rxkb_popularity popularity)
{
    काष्ठा config_item config = config_item_new(popularity);
    काष्ठा rxkb_layout *l;
    xml_गाँठ *गाँठ = शून्य;
    bool exists = false;

    अगर (!parse_config_item(ctx, layout, &config))
        वापस;

    list_for_each(l, &ctx->layouts, base.link) {
        अगर (streq(l->name, config.name) && l->variant == शून्य) {
            exists = true;
            अवरोध;
        }
    }

    अगर (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = शून्य;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } अन्यथा {
        config_item_free(&config);
    }

    क्रम (गाँठ = layout->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "variantList")) {
            parse_variant_list(ctx, l, गाँठ, popularity);
        }
        अगर (!exists && is_गाँठ(गाँठ, "configItem")) {
            xml_गाँठ *ll;
            क्रम (ll = गाँठ->children; ll; ll = ll->अग्र) {
                अगर (is_गाँठ(ll, "languageList"))
                    parse_language_list(ll, l);
                अगर (is_गाँठ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

अटल व्योम
parse_layout_list(काष्ठा rxkb_context *ctx, xml_गाँठ *layout_list,
                  क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = शून्य;

    क्रम (गाँठ = layout_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "layout"))
            parse_layout(ctx, गाँठ, popularity);
    }
}

अटल व्योम
parse_option(काष्ठा rxkb_context *ctx, काष्ठा rxkb_option_group *group,
             xml_गाँठ *option, क्रमागत rxkb_popularity popularity)
{
    काष्ठा config_item config = config_item_new(popularity);

    अगर (parse_config_item(ctx, option, &config)) {
        काष्ठा rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            अगर (streq(o->name, config.name)) {
                config_item_free(&config);
                वापस;
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

अटल व्योम
parse_group(काष्ठा rxkb_context *ctx, xml_गाँठ *group,
            क्रमागत rxkb_popularity popularity)
{
    काष्ठा config_item config = config_item_new(popularity);
    काष्ठा rxkb_option_group *g;
    xml_गाँठ *गाँठ = शून्य;
    xmlChar *multiple;
    bool exists = false;

    अगर (!parse_config_item(ctx, group, &config))
        वापस;

    list_for_each(g, &ctx->option_groups, base.link) {
        अगर (streq(g->name, config.name)) {
            exists = true;
            अवरोध;
        }
    }

    अगर (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (स्थिर xmlChar*)"allowMultipleSelection");
        अगर (multiple && xmlStrEqual(multiple, (स्थिर xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } अन्यथा {
        config_item_free(&config);
    }

    क्रम (गाँठ = group->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "option"))
            parse_option(ctx, g, गाँठ, popularity);
    }
}

अटल व्योम
parse_option_list(काष्ठा rxkb_context *ctx, xml_गाँठ *option_list,
                  क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = शून्य;

    क्रम (गाँठ = option_list->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "group"))
            parse_group(ctx, गाँठ, popularity);
    }
}

अटल व्योम
parse_rules_xml(काष्ठा rxkb_context *ctx, xml_गाँठ *मूल,
                क्रमागत rxkb_popularity popularity)
{
    xml_गाँठ *गाँठ = शून्य;

    क्रम (गाँठ = मूल->children; गाँठ; गाँठ = गाँठ->अग्र) {
        अगर (is_गाँठ(गाँठ, "modelList"))
            parse_model_list(ctx, गाँठ, popularity);
        अन्यथा अगर (is_गाँठ(गाँठ, "layoutList"))
            parse_layout_list(ctx, गाँठ, popularity);
        अन्यथा अगर (is_गाँठ(गाँठ, "optionList"))
            parse_option_list(ctx, गाँठ, popularity);
    }
}

अटल व्योम
ATTR_PRINTF(2, 0)
xml_error_func(व्योम *ctx, स्थिर अक्षर *संदेश, ...)
{
    अटल अक्षर buf[PATH_MAX];
    अटल माप_प्रकार slen = 0;
    बहु_सूची args;
    पूर्णांक rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    बहु_शुरू(args, संदेश);
    rc = vsnprintf(&buf[slen], माप(buf) - slen, संदेश, args);
    बहु_पूर्ण(args);

    /* This shouldn't really happen */
    अगर (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        स_रखो(buf, 0, माप(buf));
        वापस;
    }

    slen += (माप_प्रकार)rc;
    अगर (slen >= माप(buf)) {
        /* truncated, let's flush this */
        buf[माप(buf) - 1] = '\n';
        slen = माप(buf);
    }

    /* We're assuming here that the last character is \n. */
    अगर (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        स_रखो(buf, 0, माप(buf));
        slen = 0;
    }
}

#अगर HAVE_XML_CTXT_SET_ERRORHANDLER
अटल व्योम
xml_structured_error_func(व्योम *user_तथ्य_2, स्थिर xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_तथ्य_2);
}
#पूर्ण_अगर

#अगर_घोषित XML_PARSE_NO_XXE
#घोषणा _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#घोषणा _XML_OPTIONS (XML_PARSE_NONET)
#पूर्ण_अगर

अटल bool
validate(काष्ठा rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    स्थिर अक्षर dtdstr[] =
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

#अगर HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    अगर (!xmlCtxt)
        वापस false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_निवेश_Ptr pinput =
        xml_New_निवेश_From_Memory(शून्य, dtdstr, ARRAY_आकार(dtdstr) - 1,
                              XML_निवेश_BUF_STATIC);
    अगर (!pinput)
        जाओ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, शून्य, शून्य);
#अन्यथा
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_निवेश_बफर_Ptr buf =
        xml_Parser_निवेश_बफर_Create_Mem(dtdstr, ARRAY_आकार(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    अगर (!buf)
        जाओ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(शून्य, buf, XML_CHAR_ENCODING_UTF8);
#पूर्ण_अगर

    अगर (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        जाओ dtd_error;
    }

#अगर HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#अन्यथा
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    अगर (!dtdvalid) {
        success = false;
        जाओ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_तथ्य_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#पूर्ण_अगर
    xmlFreeDtd(dtd);

dtd_error:
#अगर HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#पूर्ण_अगर
    वापस success;
}

अटल bool
parse(काष्ठा rxkb_context *ctx, स्थिर अक्षर *path,
      क्रमागत rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = शून्य;
    xml_गाँठ *मूल = शून्य;

    अगर (!check_eaccess(path, R_OK))
        वापस false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    अगर (!xmlCtxt)
        वापस false;

#अगर_घोषित XML_PARSE_NO_XXE
#घोषणा _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#अन्यथा
#घोषणा _XML_OPTIONS (XML_PARSE_NONET)
#पूर्ण_अगर
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#अगर HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#पूर्ण_अगर
#अगर !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#पूर्ण_अगर

    doc = xmlCtxtReadFile(xmlCtxt, path, शून्य, 0);
    अगर (!doc)
        जाओ parse_error;

    अगर (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        जाओ validate_error;
    }

    मूल = xml_Doc_Get_मूल_Element(doc);
    parse_rules_xml(ctx, मूल, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#अगर !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(शून्य, शून्य);
#पूर्ण_अगर
#अगर HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, शून्य, शून्य);
#पूर्ण_अगर
    xmlFreeParserCtxt(xmlCtxt);

    वापस success;
}
