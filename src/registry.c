/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <libxml/parser.h>

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ಅನ್ತರ್ಭಾವ <limits.h>
    #ಯದಿ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ಅನ್ತರ್ಭಾವ <dirent.h>
        #ಅನ್ತರ್ಭಾವ <unistd.h>
    #endif
#endif

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbregistry.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-list.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"

ಸಂರಚನಾ rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ಸಂರಚನಾ rxkb_object {
    ಸಂರಚನಾ rxkb_object *parent;
    uint32_t refcount;
    ಸಂರಚನಾ list link;
};

ಸಂರಚನಾ rxkb_iso639_code {
    ಸಂರಚನಾ rxkb_object base;
    ವರ್ಣ *code;
};

ಸಂರಚನಾ rxkb_iso3166_code {
    ಸಂರಚನಾ rxkb_object base;
    ವರ್ಣ *code;
};

ಗಣನಾ context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ಸಂರಚನಾ rxkb_context {
    ಸಂರಚನಾ rxkb_object base;
    ಗಣನಾ context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    ಸಂರಚನಾ list models;         /* list of struct rxkb_models */
    ಸಂರಚನಾ list layouts;        /* list of struct rxkb_layouts */
    ಸಂರಚನಾ list option_groups;  /* list of struct rxkb_option_group */

    darray(ವರ್ಣ *) includes;


    ATTR_PRINTF(3, 0) ಶೂನ್ಯ (*log_fn)(ಸಂರಚನಾ rxkb_context *ctx,
                                     ಗಣನಾ rxkb_log_level level,
                                     ಧ್ರುವ ವರ್ಣ *fmt, va_list args);
    ಗಣನಾ rxkb_log_level log_level;

    ಶೂನ್ಯ *userdata;
};

ಸಂರಚನಾ rxkb_model {
    ಸಂರಚನಾ rxkb_object base;

    ವರ್ಣ *name;
    ವರ್ಣ *vendor;
    ವರ್ಣ *description;
    ಗಣನಾ rxkb_popularity popularity;
};

ಸಂರಚನಾ rxkb_layout {
    ಸಂರಚನಾ rxkb_object base;

    ವರ್ಣ *name;
    ವರ್ಣ *brief;
    ವರ್ಣ *description;
    ವರ್ಣ *variant;
    ಗಣನಾ rxkb_popularity popularity;

    ಸಂರಚನಾ list iso639s;  /* list of struct rxkb_iso639_code */
    ಸಂರಚನಾ list iso3166s; /* list of struct rxkb_iso3166_code */
};

ಸಂರಚನಾ rxkb_option_group {
    ಸಂರಚನಾ rxkb_object base;

    bool allow_multiple;
    ಸಂರಚನಾ list options; /* list of struct rxkb_options */
    ವರ್ಣ *name;
    ವರ್ಣ *description;
    ಗಣನಾ rxkb_popularity popularity;
};

ಸಂರಚನಾ rxkb_option {
    ಸಂರಚನಾ rxkb_object base;

    ವರ್ಣ *name;
    ವರ್ಣ *brief;
    ವರ್ಣ *description;
    ಗಣನಾ rxkb_popularity popularity;
    bool layout_specific;
};

ಸ್ಥಿರ bool
parse(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path,
      ಗಣನಾ rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_log(ಸಂರಚನಾ rxkb_context *ctx, ಗಣನಾ rxkb_log_level level,
         ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list args;

    ಯದಿ (ctx->log_level < level)
        ಪ್ರತ್ಯಾಗಮ;

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
#define rxkb_log_with_code(ctx, level, ಸಂದೇಶ_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(ಸಂದೇಶ_id, fmt), ##__VA_ARGS__)
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
ಸಂರಚನಾ type_ * type_##_ref(ಸಂರಚನಾ type_ *object) { \
    rxkb_object_ref(&object->base); \
    ಪ್ರತ್ಯಾಗಮ object; \
} \
ಸಂರಚನಾ type_ * type_##_unref(ಸಂರಚನಾ type_ *object) { \
    ಯದಿ (!object) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; \
    ಪ್ರತಿಜ್ಞಾ(object->base.refcount >= 1); \
    ಯದಿ (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        ಮುಕ್ತ(object); \
    } \
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ type_ * type_##_create(ಸಂರಚನಾ rxkb_object *parent) { \
    ಸಂರಚನಾ type_ *t = calloc(1, ಪರಿಮಾಣ *t); \
    ಯದಿ (t) \
        rxkb_object_init(&t->base, parent); \
    ಪ್ರತ್ಯಾಗಮ t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(ಸಂರಚನಾ type_ *object) { \
    ಪ್ರತ್ಯಾಗಮ object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ಧ್ರುವ ವರ್ಣ*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
ಸಂರಚನಾ type_ * type_##_first(ಸಂರಚನಾ parent_type_ *parent) { \
    ಸಂರಚನಾ type_ *o = ರಿಕ್ತ; \
    ಯದಿ (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    ಪ್ರತ್ಯಾಗಮ o; \
} \
ಸಂರಚನಾ type_ * \
type_##_next(ಸಂರಚನಾ type_ *o) \
{ \
    ಸಂರಚನಾ parent_type_ *parent; \
    ಸಂರಚನಾ type_ *next; \
    parent = container_of(o->base.parent, ಸಂರಚನಾ parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    ಯದಿ (list_is_last(&parent->parent_field_, &o->base.link)) \
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; \
    ಪ್ರತ್ಯಾಗಮ next; \
}

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_object_init(ಸಂರಚನಾ rxkb_object *object, ಸಂರಚನಾ rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

ಸ್ಥಿರ ಶೂನ್ಯ *
rxkb_object_ref(ಸಂರಚನಾ rxkb_object *object)
{
    ಪ್ರತಿಜ್ಞಾ(object->refcount >= 1);
    ++object->refcount;
    ಪ್ರತ್ಯಾಗಮ object;
}

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_iso639_code_destroy(ಸಂರಚನಾ rxkb_iso639_code *code)
{
    ಮುಕ್ತ(code->code);
}

ಸಂರಚನಾ rxkb_iso639_code *
rxkb_layout_get_iso639_first(ಸಂರಚನಾ rxkb_layout *layout)
{
    ಸಂರಚನಾ rxkb_iso639_code *code = ರಿಕ್ತ;

    ಯದಿ (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    ಪ್ರತ್ಯಾಗಮ code;
}

ಸಂರಚನಾ rxkb_iso639_code *
rxkb_iso639_code_next(ಸಂರಚನಾ rxkb_iso639_code *code)
{
    ಸಂರಚನಾ rxkb_iso639_code *next = ರಿಕ್ತ;
    ಸಂರಚನಾ rxkb_layout *layout;

    layout = container_of(code->base.parent, ಸಂರಚನಾ rxkb_layout, base);

    ಯದಿ (list_is_last(&layout->iso639s, &code->base.link))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    next = list_first_entry(&code->base.link, code, base.link);

    ಪ್ರತ್ಯಾಗಮ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_iso3166_code_destroy(ಸಂರಚನಾ rxkb_iso3166_code *code)
{
    ಮುಕ್ತ(code->code);
}

ಸಂರಚನಾ rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(ಸಂರಚನಾ rxkb_layout *layout)
{
    ಸಂರಚನಾ rxkb_iso3166_code *code = ರಿಕ್ತ;

    ಯದಿ (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    ಪ್ರತ್ಯಾಗಮ code;
}

ಸಂರಚನಾ rxkb_iso3166_code *
rxkb_iso3166_code_next(ಸಂರಚನಾ rxkb_iso3166_code *code)
{
    ಸಂರಚನಾ rxkb_iso3166_code *next = ರಿಕ್ತ;
    ಸಂರಚನಾ rxkb_layout *layout;

    layout = container_of(code->base.parent, ಸಂರಚನಾ rxkb_layout, base);

    ಯದಿ (list_is_last(&layout->iso3166s, &code->base.link))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    next = list_first_entry(&code->base.link, code, base.link);

    ಪ್ರತ್ಯಾಗಮ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_option_destroy(ಸಂರಚನಾ rxkb_option *o)
{
    ಮುಕ್ತ(o->name);
    ಮುಕ್ತ(o->brief);
    ಮುಕ್ತ(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ಗಣನಾ rxkb_popularity);
bool rxkb_option_is_layout_specific(ಸಂರಚನಾ rxkb_option *object) {
    ಪ್ರತ್ಯಾಗಮ object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_layout_destroy(ಸಂರಚನಾ rxkb_layout *l)
{
    ಸಂರಚನಾ rxkb_iso639_code *iso639, *tmp_639;
    ಸಂರಚನಾ rxkb_iso3166_code *iso3166, *tmp_3166;

    ಮುಕ್ತ(l->name);
    ಮುಕ್ತ(l->brief);
    ಮುಕ್ತ(l->description);
    ಮುಕ್ತ(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ಗಣನಾ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_model_destroy(ಸಂರಚನಾ rxkb_model *m)
{
    ಮುಕ್ತ(m->name);
    ಮುಕ್ತ(m->vendor);
    ಮುಕ್ತ(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ಗಣನಾ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_option_group_destroy(ಸಂರಚನಾ rxkb_option_group *og)
{
    ಸಂರಚನಾ rxkb_option *o, *otmp;

    ಮುಕ್ತ(og->name);
    ಮುಕ್ತ(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(ಸಂರಚನಾ rxkb_option_group *g)
{
    ಪ್ರತ್ಯಾಗಮ g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, ಗಣನಾ rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_context_destroy(ಸಂರಚನಾ rxkb_context *ctx)
{
    ಸಂರಚನಾ rxkb_model *m, *mtmp;
    ಸಂರಚನಾ rxkb_layout *l, *ltmp;
    ಸಂರಚನಾ rxkb_option_group *og, *ogtmp;
    ವರ್ಣ **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    ಪ್ರತಿಜ್ಞಾ(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    ಪ್ರತಿಜ್ಞಾ(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    ಪ್ರತಿಜ್ಞಾ(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        ಮುಕ್ತ(*path);
    darray_free(ctx->includes);

    ಪ್ರತಿಜ್ಞಾ(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ಗಣನಾ rxkb_log_level);

ಸ್ಥಿರ ವರ್ಣ *
rxkb_context_getenv(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *name)
{
    ಯದಿ (ctx->use_secure_getenv) {
        ಪ್ರತ್ಯಾಗಮ secure_getenv(name);
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ getenv(name);
    }
}


ಶೂನ್ಯ
rxkb_context_set_log_level(ಸಂರಚನಾ rxkb_context *ctx,
                           ಗಣನಾ rxkb_log_level level)
{
    ctx->log_level = level;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
log_level_to_prefix(ಗಣನಾ rxkb_log_level level)
{
    ವಿಕಲ್ಪ (level) {
    ಪಕ್ಷ RXKB_LOG_LEVEL_DEBUG:
        ಪ್ರತ್ಯಾಗಮ "xkbregistry: DEBUG: ";
    ಪಕ್ಷ RXKB_LOG_LEVEL_INFO:
        ಪ್ರತ್ಯಾಗಮ "xkbregistry: INFO: ";
    ಪಕ್ಷ RXKB_LOG_LEVEL_WARNING:
        ಪ್ರತ್ಯಾಗಮ "xkbregistry: WARNING: ";
    ಪಕ್ಷ RXKB_LOG_LEVEL_ERROR:
        ಪ್ರತ್ಯಾಗಮ "xkbregistry: ERROR: ";
    ಪಕ್ಷ RXKB_LOG_LEVEL_CRITICAL:
        ಪ್ರತ್ಯಾಗಮ "xkbregistry: CRITICAL: ";
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
}

ATTR_PRINTF(3, 0) ಸ್ಥಿರ ಶೂನ್ಯ
default_log_fn(ಸಂರಚನಾ rxkb_context *ctx, ಗಣನಾ rxkb_log_level level,
               ಧ್ರುವ ವರ್ಣ *fmt, va_list args)
{
    ಧ್ರುವ ವರ್ಣ *prefix = log_level_to_prefix(level);

    ಯದಿ (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ಸ್ಥಿರ ಗಣನಾ rxkb_log_level
log_level(ಧ್ರುವ ವರ್ಣ *level) {
    errno = 0;
    ವರ್ಣ *endptr;
    ಧ್ರುವ long lvl = strtol(level, &endptr, 10);
    ಯದಿ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ಪ್ರತ್ಯಾಗಮ (ಗಣನಾ rxkb_log_level)lvl;

    ಯದಿ (istreq_prefix("crit", level))
        ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_CRITICAL;
    ಯದಿ (istreq_prefix("err", level))
        ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_ERROR;
    ಯದಿ (istreq_prefix("warn", level))
        ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_WARNING;
    ಯದಿ (istreq_prefix("info", level))
        ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_INFO;
    ಯದಿ (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_DEBUG;

    ಪ್ರತ್ಯಾಗಮ RXKB_LOG_LEVEL_ERROR;
}

ಸಂರಚನಾ rxkb_context *
rxkb_context_new(ಗಣನಾ rxkb_context_flags flags)
{
    ಸಂರಚನಾ rxkb_context *ctx = rxkb_context_create(ರಿಕ್ತ);
    ಧ್ರುವ ವರ್ಣ *env;

    ಯದಿ (!ctx)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ಯದಿ (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ಯದಿ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        ಮುಕ್ತ(ctx);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ಯದಿ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ಬೇರು);
        rxkb_context_unref(ctx);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ ctx;
}

ಶೂನ್ಯ
rxkb_context_set_log_fn(ಸಂರಚನಾ rxkb_context *ctx,
                        ಶೂನ್ಯ (*log_fn)(ಸಂರಚನಾ rxkb_context *ctx,
                                       ಗಣನಾ rxkb_log_level level,
                                       ಧ್ರುವ ವರ್ಣ *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path)
{
    ಪೂರ್ಣಾಙ್ಕ err = 0;

    ಯದಿ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ಗಚ್ಛ error;
    }

    ಸಂರಚನಾ stat stat_buf;
    err = stat(path, &stat_buf);
    ಯದಿ (err != 0) {
        err = errno;
        ಗಚ್ಛ error;
    }
    ಯದಿ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ಗಚ್ಛ error;
    }

    ಯದಿ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ಗಚ್ಛ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    ವರ್ಣ rules[PATH_MAX];
    ಯದಿ (!snprintf_safe(rules, ಪರಿಮಾಣ(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                ಪರಿಮಾಣ(rules), path, DEFAULT_XKB_RULES);
        ಗಚ್ಛ error;
    }

    ವರ್ಣ * ಧ್ರುವ tmp = strdup(path);
    ಯದಿ (!tmp) {
        err = ENOMEM;
        ಗಚ್ಛ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ಪ್ರತ್ಯಾಗಮ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ಪ್ರತ್ಯಾಗಮ false;
}

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_str(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಪ್ರತ್ಯಾಗಮ strcmp(*(ವರ್ಣ **)a, *(ವರ್ಣ **) b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
add_direct_subdirectories(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path,
                          darray_string *extensions,
                          darray_size_t versioned_ಎಣಿಕೆ,
                          size_t versioned_path_length)
{
    ಪೂರ್ಣಾಙ್ಕ ret = 0;
    ಪೂರ್ಣಾಙ್ಕ err = ENOMEM;
    DIR *dir = ರಿಕ್ತ;

    /* Check extensions parent directory */
    ಸಂರಚನಾ stat stat_buf;
    err = stat(path, &stat_buf);
    ಯದಿ (err != 0) {
        err = errno;
        ಗಚ್ಛ err;
    }
    ಯದಿ (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ಗಚ್ಛ err;
    }
    ಯದಿ (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ಗಚ್ಛ err;
    }

    dir = opendir(path);
    ಯದಿ (dir == ರಿಕ್ತ) {
        err = EACCES;
        ಗಚ್ಛ err;
    }

    ಸಂರಚನಾ dirent *entry;
    ವರ್ಣ path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ಯಾವತ್ ((entry = readdir(dir)) != ರಿಕ್ತ) {
        ಧ್ರುವ ವರ್ಣ *name = entry->d_name;

        /* Skip special entries */
        ಯದಿ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ಅನುವರ್ತ;

        ಯದಿ (!snprintf_safe(path_buf, ಪರಿಮಾಣ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            ಗಚ್ಛ err;
        }
        ಯದಿ (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ಅನುವರ್ತ;

        /* Skip if the corresponding versioned directory is already included */
        ಪ್ರತಿ (darray_size_t i = 0; i < versioned_ಎಣಿಕೆ; i++) {
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ಯದಿ (strcmp(name, prev_name) == 0)
                ಗಚ್ಛ next;
        }

        ವರ್ಣ *ext_path = strdup_safe(path_buf);
        ಯದಿ (!ext_path) {
            err = ENOMEM;
            ಗಚ್ಛ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ಯದಿ (darray_size(*extensions) > versioned_ಎಣಿಕೆ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ವಸ್ತುಗಳು(*extensions) + versioned_ಎಣಿಕೆ,
              darray_size(*extensions) - versioned_ಎಣಿಕೆ,
              ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(*extensions)), &compare_str);
        ವರ್ಣ **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ಎಣಿಕೆ) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    ಪ್ರತ್ಯಾಗಮ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ಯದಿ (dir)
        closedir(dir);

    ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

bool
rxkb_context_include_path_append_default(ಸಂರಚನಾ rxkb_context *ctx)
{
    ವರ್ಣ user_path[PATH_MAX];
    bool ret = false;

    ಯದಿ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ home = rxkb_context_getenv(ctx, "HOME");
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ಯದಿ (xdg != ರಿಕ್ತ) {
        ಯದಿ (snprintf_safe(user_path, ಪರಿಮಾಣ(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } ಅನ್ಯಥಾ ಯದಿ (home != ರಿಕ್ತ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ಯದಿ (snprintf_safe(user_path, ಪರಿಮಾಣ(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ಯದಿ (home != ರಿಕ್ತ) {
        ಯದಿ (snprintf_safe(user_path, ಪರಿಮಾಣ(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != ರಿಕ್ತ) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#ಯದಿ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ಧ್ರುವ ವರ್ಣ *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    ಯದಿ (extensions_path == ರಿಕ್ತ)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    ಯದಿ (extensions_path != ರಿಕ್ತ) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    ಯದಿ (extensions_path == ರಿಕ್ತ)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    ಯದಿ (extensions_path != ರಿಕ್ತ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ವರ್ಣ **ext_path;
    darray_foreach(ext_path, extensions) {
        ಮುಕ್ತ(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ಬೇರು = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಧ್ರುವ bool has_ಬೇರು = rxkb_context_include_path_append(
        ctx, ((ಬೇರು != ರಿಕ್ತ) ? ಬೇರು: DFLT_XKB_CONFIG_ಬೇರು)
    );
    ret |= has_ಬೇರು;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ಯದಿ (!has_ಬೇರು && (ಬೇರು == ರಿಕ್ತ || ಬೇರು[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ಬೇರು,
                 ((ಬೇರು == ರಿಕ್ತ) ? DFLT_XKB_CONFIG_ಬೇರು : ಬೇರು));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_ಬೇರು);
    }

    ಪ್ರತ್ಯಾಗಮ ret;
}

bool
rxkb_context_parse_default_ruleset(ಸಂರಚನಾ rxkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *ruleset)
{
    ವರ್ಣ **path;
    bool success = false;

    ಯದಿ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        ವರ್ಣ rules[PATH_MAX];

        ಯದಿ (snprintf_safe(rules, ಪರಿಮಾಣ(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ಯದಿ (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ಯದಿ (ctx->load_extra_rules_files &&
            snprintf_safe(rules, ಪರಿಮಾಣ(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ಯದಿ (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ಪ್ರತ್ಯಾಗಮ success;
}


ಶೂನ್ಯ
rxkb_context_set_user_data(ಸಂರಚನಾ rxkb_context *ctx, ಶೂನ್ಯ *userdata)
{
    ctx->userdata = userdata;
}

ಶೂನ್ಯ *
rxkb_context_get_user_data(ಸಂರಚನಾ rxkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ ctx->userdata;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_ಗಂಟು(xml_ಗಂಟು *ಗಂಟು, ಧ್ರುವ ವರ್ಣ *name)
{
    ಪ್ರತ್ಯಾಗಮ ಗಂಟು->type == XML_ELEMENT_ಗಂಟು &&
        xmlStrEqual(ಗಂಟು->name, (ಧ್ರುವ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ಸ್ಥಿರ ವರ್ಣ *
extract_text(xml_ಗಂಟು *ಗಂಟು)
{
    xml_ಗಂಟು *n;

    ಪ್ರತಿ (n = ಗಂಟು->children; n; n = n->next) {
        ಯದಿ (n->type == XML_TEXT_ಗಂಟು)
            ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *)xmlStrdup(n->content);
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* Data from “configItem” node */
ಸಂರಚನಾ config_item {
    ವರ್ಣ *name;
    ವರ್ಣ *description;
    ವರ್ಣ *brief;
    ವರ್ಣ *vendor;
    ಗಣನಾ rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = ರಿಕ್ತ, \
    .description = ರಿಕ್ತ, \
    .brief = ರಿಕ್ತ, \
    .vendor = ರಿಕ್ತ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ಸ್ಥಿರ ಶೂನ್ಯ
config_item_free(ಸಂರಚನಾ config_item *config) {
    ಮುಕ್ತ(config->name);
    ಮುಕ್ತ(config->description);
    ಮುಕ್ತ(config->brief);
    ಮುಕ್ತ(config->vendor);
}

ಸ್ಥಿರ bool
parse_config_item(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *parent,
                  ಸಂರಚನಾ config_item *config)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
    xml_ಗಂಟು *ci = ರಿಕ್ತ;

    ಪ್ರತಿ (ci = parent->children; ci; ci = ci->next) {
        ಯದಿ (is_ಗಂಟು(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ಧ್ರುವ xmlChar*)"popularity");
            ಯದಿ (raw_popularity) {
                ಯದಿ (xmlStrEqual(raw_popularity, (ಧ್ರುವ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ಅನ್ಯಥಾ ಯದಿ (xmlStrEqual(raw_popularity, (ಧ್ರುವ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ಅನ್ಯಥಾ
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->ಸಾಲು, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ಧ್ರುವ xmlChar*)"layout-specific");
            ಯದಿ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ಧ್ರುವ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            ಪ್ರತಿ (ಗಂಟು = ci->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
                ಯದಿ (is_ಗಂಟು(ಗಂಟು, "name"))
                    config->name = extract_text(ಗಂಟು);
                ಅನ್ಯಥಾ ಯದಿ (is_ಗಂಟು(ಗಂಟು, "description"))
                    config->description = extract_text(ಗಂಟು);
                ಅನ್ಯಥಾ ಯದಿ (is_ಗಂಟು(ಗಂಟು, "shortDescription"))
                    config->brief = extract_text(ಗಂಟು);
                ಅನ್ಯಥಾ ಯದಿ (is_ಗಂಟು(ಗಂಟು, "vendor"))
                    config->vendor = extract_text(ಗಂಟು);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ಯದಿ (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->ಸಾಲು);
                config_item_free(config);
                ಪ್ರತ್ಯಾಗಮ false;
            }

            ಪ್ರತ್ಯಾಗಮ true; /* only one configItem allowed in the dtd */
        }
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_model(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *model,
            ಗಣನಾ rxkb_popularity popularity)
{
    ಸಂರಚನಾ config_item config = config_item_new(popularity);

    ಯದಿ (parse_config_item(ctx, model, &config)) {
        ಸಂರಚನಾ rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            ಯದಿ (streq(m->name, config.name)) {
                config_item_free(&config);
                ಪ್ರತ್ಯಾಗಮ;
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

ಸ್ಥಿರ ಶೂನ್ಯ
parse_model_list(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *model_list,
                 ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    ಪ್ರತಿ (ಗಂಟು = model_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "model"))
            parse_model(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_language_list(xml_ಗಂಟು *language_list, ಸಂರಚನಾ rxkb_layout *layout)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
    ಸಂರಚನಾ rxkb_iso639_code *code;

    ಪ್ರತಿ (ಗಂಟು = language_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "iso639Id")) {
            ವರ್ಣ *str = extract_text(ಗಂಟು);
            ಸಂರಚನಾ rxkb_object *parent;

            ಯದಿ (!str || strlen(str) != 3) {
                ಮುಕ್ತ(str);
                ಅನುವರ್ತ;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_country_list(xml_ಗಂಟು *country_list, ಸಂರಚನಾ rxkb_layout *layout)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
    ಸಂರಚನಾ rxkb_iso3166_code *code;

    ಪ್ರತಿ (ಗಂಟು = country_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "iso3166Id")) {
            ವರ್ಣ *str = extract_text(ಗಂಟು);
            ಸಂರಚನಾ rxkb_object *parent;

            ಯದಿ (!str || strlen(str) != 2) {
                ಮುಕ್ತ(str);
                ಅನುವರ್ತ;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_variant(ಸಂರಚನಾ rxkb_context *ctx, ಸಂರಚನಾ rxkb_layout *l,
              xml_ಗಂಟು *variant, ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ci;
    ಸಂರಚನಾ config_item config = config_item_new(popularity);

    ಯದಿ (parse_config_item(ctx, variant, &config)) {
        ಸಂರಚನಾ rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            ಯದಿ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ಭಙ್ಗ;
            }
        }

        ಯದಿ (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ರಿಕ್ತ ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            ಪ್ರತಿ (ci = variant->children; ci; ci = ci->next) {
                xml_ಗಂಟು *ಗಂಟು;

                ಯದಿ (!is_ಗಂಟು(ci, "configItem"))
                    ಅನುವರ್ತ;

                bool found_language_list = false;
                bool found_country_list = false;
                ಪ್ರತಿ (ಗಂಟು = ci->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
                    ಯದಿ (is_ಗಂಟು(ಗಂಟು, "languageList")) {
                        parse_language_list(ಗಂಟು, v);
                        found_language_list = true;
                    }
                    ಯದಿ (is_ಗಂಟು(ಗಂಟು, "countryList")) {
                        parse_country_list(ಗಂಟು, v);
                        found_country_list = true;
                    }
                }
                ಯದಿ (!found_language_list) {
                    // inherit from parent layout
                    ಸಂರಚನಾ rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        ಸಂರಚನಾ rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                ಯದಿ (!found_country_list) {
                    // inherit from parent layout
                    ಸಂರಚನಾ rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        ಸಂರಚನಾ rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } ಅನ್ಯಥಾ {
            config_item_free(&config);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_variant_list(ಸಂರಚನಾ rxkb_context *ctx, ಸಂರಚನಾ rxkb_layout *l,
                   xml_ಗಂಟು *variant_list, ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    ಪ್ರತಿ (ಗಂಟು = variant_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "variant"))
            parse_variant(ctx, l, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_layout(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *layout,
             ಗಣನಾ rxkb_popularity popularity)
{
    ಸಂರಚನಾ config_item config = config_item_new(popularity);
    ಸಂರಚನಾ rxkb_layout *l;
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
    bool exists = false;

    ಯದಿ (!parse_config_item(ctx, layout, &config))
        ಪ್ರತ್ಯಾಗಮ;

    list_for_each(l, &ctx->layouts, base.link) {
        ಯದಿ (streq(l->name, config.name) && l->variant == ರಿಕ್ತ) {
            exists = true;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ರಿಕ್ತ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } ಅನ್ಯಥಾ {
        config_item_free(&config);
    }

    ಪ್ರತಿ (ಗಂಟು = layout->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "variantList")) {
            parse_variant_list(ctx, l, ಗಂಟು, popularity);
        }
        ಯದಿ (!exists && is_ಗಂಟು(ಗಂಟು, "configItem")) {
            xml_ಗಂಟು *ll;
            ಪ್ರತಿ (ll = ಗಂಟು->children; ll; ll = ll->next) {
                ಯದಿ (is_ಗಂಟು(ll, "languageList"))
                    parse_language_list(ll, l);
                ಯದಿ (is_ಗಂಟು(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_layout_list(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *layout_list,
                  ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    ಪ್ರತಿ (ಗಂಟು = layout_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "layout"))
            parse_layout(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_option(ಸಂರಚನಾ rxkb_context *ctx, ಸಂರಚನಾ rxkb_option_group *group,
             xml_ಗಂಟು *option, ಗಣನಾ rxkb_popularity popularity)
{
    ಸಂರಚನಾ config_item config = config_item_new(popularity);

    ಯದಿ (parse_config_item(ctx, option, &config)) {
        ಸಂರಚನಾ rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            ಯದಿ (streq(o->name, config.name)) {
                config_item_free(&config);
                ಪ್ರತ್ಯಾಗಮ;
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

ಸ್ಥಿರ ಶೂನ್ಯ
parse_group(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *group,
            ಗಣನಾ rxkb_popularity popularity)
{
    ಸಂರಚನಾ config_item config = config_item_new(popularity);
    ಸಂರಚನಾ rxkb_option_group *g;
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
    xmlChar *multiple;
    bool exists = false;

    ಯದಿ (!parse_config_item(ctx, group, &config))
        ಪ್ರತ್ಯಾಗಮ;

    list_for_each(g, &ctx->option_groups, base.link) {
        ಯದಿ (streq(g->name, config.name)) {
            exists = true;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ಧ್ರುವ xmlChar*)"allowMultipleSelection");
        ಯದಿ (multiple && xmlStrEqual(multiple, (ಧ್ರುವ xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } ಅನ್ಯಥಾ {
        config_item_free(&config);
    }

    ಪ್ರತಿ (ಗಂಟು = group->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "option"))
            parse_option(ctx, g, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_option_list(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *option_list,
                  ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    ಪ್ರತಿ (ಗಂಟು = option_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "group"))
            parse_group(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_rules_xml(ಸಂರಚನಾ rxkb_context *ctx, xml_ಗಂಟು *ಬೇರು,
                ಗಣನಾ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    ಪ್ರತಿ (ಗಂಟು = ಬೇರು->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->next) {
        ಯದಿ (is_ಗಂಟು(ಗಂಟು, "modelList"))
            parse_model_list(ctx, ಗಂಟು, popularity);
        ಅನ್ಯಥಾ ಯದಿ (is_ಗಂಟು(ಗಂಟು, "layoutList"))
            parse_layout_list(ctx, ಗಂಟು, popularity);
        ಅನ್ಯಥಾ ಯದಿ (is_ಗಂಟು(ಗಂಟು, "optionList"))
            parse_option_list(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ATTR_PRINTF(2, 0)
xml_error_func(ಶೂನ್ಯ *ctx, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
    ಸ್ಥಿರ ವರ್ಣ buf[PATH_MAX];
    ಸ್ಥಿರ size_t slen = 0;
    va_list args;
    ಪೂರ್ಣಾಙ್ಕ rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, ಸಂದೇಶ);
    rc = vsnprintf(&buf[slen], ಪರಿಮಾಣ(buf) - slen, ಸಂದೇಶ, args);
    va_end(args);

    /* This shouldn't really happen */
    ಯದಿ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, ಪರಿಮಾಣ(buf));
        ಪ್ರತ್ಯಾಗಮ;
    }

    slen += (size_t)rc;
    ಯದಿ (slen >= ಪರಿಮಾಣ(buf)) {
        /* truncated, let's flush this */
        buf[ಪರಿಮಾಣ(buf) - 1] = '\n';
        slen = ಪರಿಮಾಣ(buf);
    }

    /* We're assuming here that the last character is \n. */
    ಯದಿ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, ಪರಿಮಾಣ(buf));
        slen = 0;
    }
}

#ಯದಿ HAVE_XML_CTXT_SET_ERRORHANDLER
ಸ್ಥಿರ ಶೂನ್ಯ
xml_structured_error_func(ಶೂನ್ಯ *userData, ಧ್ರುವ xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ಅನ್ಯಥಾ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

ಸ್ಥಿರ bool
validate(ಸಂರಚನಾ rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ಧ್ರುವ ವರ್ಣ dtdstr[] =
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

#ಯದಿ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ಯದಿ (!xmlCtxt)
        ಪ್ರತ್ಯಾಗಮ false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ಒಳಹರಿವು_Ptr pinput =
        xml_New_ಒಳಹರಿವು_From_Memory(ರಿಕ್ತ, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_ಒಳಹರಿವು_BUF_STATIC);
    ಯದಿ (!pinput)
        ಗಚ್ಛ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ರಿಕ್ತ, ರಿಕ್ತ);
#ಅನ್ಯಥಾ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ಒಳಹರಿವು_Buffer_Ptr buf =
        xml_Parser_ಒಳಹರಿವು_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ಯದಿ (!buf)
        ಗಚ್ಛ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ರಿಕ್ತ, buf, XML_CHAR_ENCODING_UTF8);
#endif

    ಯದಿ (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        ಗಚ್ಛ dtd_error;
    }

#ಯದಿ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ಅನ್ಯಥಾ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ಯದಿ (!dtdvalid) {
        success = false;
        ಗಚ್ಛ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#ಯದಿ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    ಪ್ರತ್ಯಾಗಮ success;
}

ಸ್ಥಿರ bool
parse(ಸಂರಚನಾ rxkb_context *ctx, ಧ್ರುವ ವರ್ಣ *path,
      ಗಣನಾ rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ರಿಕ್ತ;
    xml_ಗಂಟು *ಬೇರು = ರಿಕ್ತ;

    ಯದಿ (!check_eaccess(path, R_OK))
        ಪ್ರತ್ಯಾಗಮ false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ಯದಿ (!xmlCtxt)
        ಪ್ರತ್ಯಾಗಮ false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ಅನ್ಯಥಾ
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ಯದಿ HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#ಯದಿ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, ರಿಕ್ತ, 0);
    ಯದಿ (!doc)
        ಗಚ್ಛ parse_error;

    ಯದಿ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        ಗಚ್ಛ validate_error;
    }

    ಬೇರು = xml_Doc_Get_ಬೇರು_Element(doc);
    parse_rules_xml(ctx, ಬೇರು, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ಯದಿ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(ರಿಕ್ತ, ರಿಕ್ತ);
#endif
#ಯದಿ HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, ರಿಕ್ತ, ರಿಕ್ತ);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    ಪ್ರತ್ಯಾಗಮ success;
}
