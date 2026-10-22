/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <libxml/parser.h>

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ଅନ୍ତର୍ଭାଵ <limits.h>
    #ଯଦି HAVE_UNISTD_H && HAVE_DIRENT_H
        #ଅନ୍ତର୍ଭାଵ <dirent.h>
        #ଅନ୍ତର୍ଭାଵ <unistd.h>
    #endif
#endif

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbregistry.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-list.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"

ସଂରଚନା rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ସଂରଚନା rxkb_object {
    ସଂରଚନା rxkb_object *parent;
    uint32_t refcount;
    ସଂରଚନା list link;
};

ସଂରଚନା rxkb_iso639_code {
    ସଂରଚନା rxkb_object base;
    ଅକ୍ଷର *code;
};

ସଂରଚନା rxkb_iso3166_code {
    ସଂରଚନା rxkb_object base;
    ଅକ୍ଷର *code;
};

ଗଣନା context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ସଂରଚନା rxkb_context {
    ସଂରଚନା rxkb_object base;
    ଗଣନା context_state context_state;

    bool load_extra_rules_files;
    bool use_secure_getenv;

    ସଂରଚନା list models;         /* list of struct rxkb_models */
    ସଂରଚନା list layouts;        /* list of struct rxkb_layouts */
    ସଂରଚନା list option_groups;  /* list of struct rxkb_option_group */

    darray(ଅକ୍ଷର *) includes;


    ATTR_PRINTF(3, 0) ଶୂନ୍ୟ (*log_fn)(ସଂରଚନା rxkb_context *ctx,
                                     ଗଣନା rxkb_log_level level,
                                     ଅଚଳ ଅକ୍ଷର *fmt, va_list args);
    ଗଣନା rxkb_log_level log_level;

    ଶୂନ୍ୟ *userdata;
};

ସଂରଚନା rxkb_model {
    ସଂରଚନା rxkb_object base;

    ଅକ୍ଷର *name;
    ଅକ୍ଷର *vendor;
    ଅକ୍ଷର *description;
    ଗଣନା rxkb_popularity popularity;
};

ସଂରଚନା rxkb_layout {
    ସଂରଚନା rxkb_object base;

    ଅକ୍ଷର *name;
    ଅକ୍ଷର *brief;
    ଅକ୍ଷର *description;
    ଅକ୍ଷର *variant;
    ଗଣନା rxkb_popularity popularity;

    ସଂରଚନା list iso639s;  /* list of struct rxkb_iso639_code */
    ସଂରଚନା list iso3166s; /* list of struct rxkb_iso3166_code */
};

ସଂରଚନା rxkb_option_group {
    ସଂରଚନା rxkb_object base;

    bool allow_multiple;
    ସଂରଚନା list options; /* list of struct rxkb_options */
    ଅକ୍ଷର *name;
    ଅକ୍ଷର *description;
    ଗଣନା rxkb_popularity popularity;
};

ସଂରଚନା rxkb_option {
    ସଂରଚନା rxkb_object base;

    ଅକ୍ଷର *name;
    ଅକ୍ଷର *brief;
    ଅକ୍ଷର *description;
    ଗଣନା rxkb_popularity popularity;
    bool layout_specific;
};

ସ୍ଥିର bool
parse(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path,
      ଗଣନା rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ସ୍ଥିର ଶୂନ୍ୟ
rxkb_log(ସଂରଚନା rxkb_context *ctx, ଗଣନା rxkb_log_level level,
         ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list args;

    ଯଦି (ctx->log_level < level)
        ଫେରନ୍ତୁ;

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
#define rxkb_log_with_code(ctx, level, ସନ୍ଦେଶ_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_MESSAGE_ID(ସନ୍ଦେଶ_id, fmt), ##__VA_ARGS__)
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
ସଂରଚନା type_ * type_##_ref(ସଂରଚନା type_ *object) { \
    rxkb_object_ref(&object->base); \
    ଫେରନ୍ତୁ object; \
} \
ସଂରଚନା type_ * type_##_unref(ସଂରଚନା type_ *object) { \
    ଯଦି (!object) ଫେରନ୍ତୁ ଖାଲି; \
    ପ୍ରତିଜ୍ଞା(object->base.refcount >= 1); \
    ଯଦି (--object->base.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->base.link);\
        ମୁକ୍ତ(object); \
    } \
    ଫେରନ୍ତୁ ଖାଲି;\
}

#define DECLARE_CREATE_FOR_TYPE(type_) \
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା type_ * type_##_create(ସଂରଚନା rxkb_object *parent) { \
    ସଂରଚନା type_ *t = calloc(1, ଆକାର *t); \
    ଯଦି (t) \
        rxkb_object_init(&t->base, parent); \
    ଫେରନ୍ତୁ t; \
}

#define DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, rtype_) \
rtype_ type_##_get_##field_(ସଂରଚନା type_ *object) { \
    ଫେରନ୍ତୁ object->field_; \
}

#define DECLARE_GETTER_FOR_TYPE(type_, field_) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, field_, ଅଚଳ ଅକ୍ଷର*)

#define DECLARE_FIRST_NEXT_FOR_TYPE(type_, parent_type_, parent_field_) \
ସଂରଚନା type_ * type_##_first(ସଂରଚନା parent_type_ *parent) { \
    ସଂରଚନା type_ *o = ଖାଲି; \
    ଯଦି (!list_empty(&parent->parent_field_)) \
        o = list_first_entry(&parent->parent_field_, o, base.link); \
    ଫେରନ୍ତୁ o; \
} \
ସଂରଚନା type_ * \
type_##_next(ସଂରଚନା type_ *o) \
{ \
    ସଂରଚନା parent_type_ *parent; \
    ସଂରଚନା type_ *next; \
    parent = container_of(o->base.parent, ସଂରଚନା parent_type_, base); \
    next = list_first_entry(&o->base.link, o, base.link); \
    ଯଦି (list_is_last(&parent->parent_field_, &o->base.link)) \
        ଫେରନ୍ତୁ ଖାଲି; \
    ଫେରନ୍ତୁ next; \
}

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_object_init(ସଂରଚନା rxkb_object *object, ସଂରଚନା rxkb_object *parent)
{
    object->refcount = 1;
    object->parent = parent;
    list_init(&object->link);
}

ସ୍ଥିର ଶୂନ୍ୟ *
rxkb_object_ref(ସଂରଚନା rxkb_object *object)
{
    ପ୍ରତିଜ୍ଞା(object->refcount >= 1);
    ++object->refcount;
    ଫେରନ୍ତୁ object;
}

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_iso639_code_destroy(ସଂରଚନା rxkb_iso639_code *code)
{
    ମୁକ୍ତ(code->code);
}

ସଂରଚନା rxkb_iso639_code *
rxkb_layout_get_iso639_first(ସଂରଚନା rxkb_layout *layout)
{
    ସଂରଚନା rxkb_iso639_code *code = ଖାଲି;

    ଯଦି (!list_empty(&layout->iso639s))
        code = list_first_entry(&layout->iso639s, code, base.link);

    ଫେରନ୍ତୁ code;
}

ସଂରଚନା rxkb_iso639_code *
rxkb_iso639_code_next(ସଂରଚନା rxkb_iso639_code *code)
{
    ସଂରଚନା rxkb_iso639_code *next = ଖାଲି;
    ସଂରଚନା rxkb_layout *layout;

    layout = container_of(code->base.parent, ସଂରଚନା rxkb_layout, base);

    ଯଦି (list_is_last(&layout->iso639s, &code->base.link))
        ଫେରନ୍ତୁ ଖାଲି;

    next = list_first_entry(&code->base.link, code, base.link);

    ଫେରନ୍ତୁ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_code, code);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_iso3166_code_destroy(ସଂରଚନା rxkb_iso3166_code *code)
{
    ମୁକ୍ତ(code->code);
}

ସଂରଚନା rxkb_iso3166_code *
rxkb_layout_get_iso3166_first(ସଂରଚନା rxkb_layout *layout)
{
    ସଂରଚନା rxkb_iso3166_code *code = ଖାଲି;

    ଯଦି (!list_empty(&layout->iso3166s))
        code = list_first_entry(&layout->iso3166s, code, base.link);

    ଫେରନ୍ତୁ code;
}

ସଂରଚନା rxkb_iso3166_code *
rxkb_iso3166_code_next(ସଂରଚନା rxkb_iso3166_code *code)
{
    ସଂରଚନା rxkb_iso3166_code *next = ଖାଲି;
    ସଂରଚନା rxkb_layout *layout;

    layout = container_of(code->base.parent, ସଂରଚନା rxkb_layout, base);

    ଯଦି (list_is_last(&layout->iso3166s, &code->base.link))
        ଫେରନ୍ତୁ ଖାଲି;

    next = list_first_entry(&code->base.link, code, base.link);

    ଫେରନ୍ତୁ next;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_code);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_code);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_code, code);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_option_destroy(ସଂରଚନା rxkb_option *o)
{
    ମୁକ୍ତ(o->name);
    ମୁକ୍ତ(o->brief);
    ମୁକ୍ତ(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ଗଣନା rxkb_popularity);
bool rxkb_option_is_layout_specific(ସଂରଚନା rxkb_option *object) {
    ଫେରନ୍ତୁ object->layout_specific;
}
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option, rxkb_option_group, options);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_layout_destroy(ସଂରଚନା rxkb_layout *l)
{
    ସଂରଚନା rxkb_iso639_code *iso639, *tmp_639;
    ସଂରଚନା rxkb_iso3166_code *iso3166, *tmp_3166;

    ମୁକ୍ତ(l->name);
    ମୁକ୍ତ(l->brief);
    ମୁକ୍ତ(l->description);
    ମୁକ୍ତ(l->variant);

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
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ଗଣନା rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_model_destroy(ସଂରଚନା rxkb_model *m)
{
    ମୁକ୍ତ(m->name);
    ମୁକ୍ତ(m->vendor);
    ମୁକ୍ତ(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ଗଣନା rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_model, rxkb_context, models);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_option_group_destroy(ସଂରଚନା rxkb_option_group *og)
{
    ସଂରଚନା rxkb_option *o, *otmp;

    ମୁକ୍ତ(og->name);
    ମୁକ୍ତ(og->description);

    list_for_each_safe(o, otmp, &og->options, base.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_group_allows_multiple(ସଂରଚନା rxkb_option_group *g)
{
    ଫେରନ୍ତୁ g->allow_multiple;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_group);
DECLARE_CREATE_FOR_TYPE(rxkb_option_group);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_group, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_group, popularity, ଗଣନା rxkb_popularity);
DECLARE_FIRST_NEXT_FOR_TYPE(rxkb_option_group, rxkb_context, option_groups);

ସ୍ଥିର ଶୂନ୍ୟ
rxkb_context_destroy(ସଂରଚନା rxkb_context *ctx)
{
    ସଂରଚନା rxkb_model *m, *mtmp;
    ସଂରଚନା rxkb_layout *l, *ltmp;
    ସଂରଚନା rxkb_option_group *og, *ogtmp;
    ଅକ୍ଷର **path;

    list_for_each_safe(m, mtmp, &ctx->models, base.link)
        rxkb_model_unref(m);
    ପ୍ରତିଜ୍ଞା(list_empty(&ctx->models));

    list_for_each_safe(l, ltmp, &ctx->layouts, base.link)
        rxkb_layout_unref(l);
    ପ୍ରତିଜ୍ଞା(list_empty(&ctx->layouts));

    list_for_each_safe(og, ogtmp, &ctx->option_groups, base.link)
        rxkb_option_group_unref(og);
    ପ୍ରତିଜ୍ଞା(list_empty(&ctx->option_groups));

    darray_foreach(path, ctx->includes)
        ମୁକ୍ତ(*path);
    darray_free(ctx->includes);

    ପ୍ରତିଜ୍ଞା(darray_empty(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ଗଣନା rxkb_log_level);

ସ୍ଥିର ଅକ୍ଷର *
rxkb_context_getenv(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *name)
{
    ଯଦି (ctx->use_secure_getenv) {
        ଫେରନ୍ତୁ secure_getenv(name);
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ getenv(name);
    }
}


ଶୂନ୍ୟ
rxkb_context_set_log_level(ସଂରଚନା rxkb_context *ctx,
                           ଗଣନା rxkb_log_level level)
{
    ctx->log_level = level;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
log_level_to_prefix(ଗଣନା rxkb_log_level level)
{
    ବାଛ (level) {
    ସ୍ଥିତି RXKB_LOG_LEVEL_DEBUG:
        ଫେରନ୍ତୁ "xkbregistry: DEBUG: ";
    ସ୍ଥିତି RXKB_LOG_LEVEL_INFO:
        ଫେରନ୍ତୁ "xkbregistry: INFO: ";
    ସ୍ଥିତି RXKB_LOG_LEVEL_WARNING:
        ଫେରନ୍ତୁ "xkbregistry: WARNING: ";
    ସ୍ଥିତି RXKB_LOG_LEVEL_ERROR:
        ଫେରନ୍ତୁ "xkbregistry: ERROR: ";
    ସ୍ଥିତି RXKB_LOG_LEVEL_CRITICAL:
        ଫେରନ୍ତୁ "xkbregistry: CRITICAL: ";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ ଖାଲି;
    }
}

ATTR_PRINTF(3, 0) ସ୍ଥିର ଶୂନ୍ୟ
default_log_fn(ସଂରଚନା rxkb_context *ctx, ଗଣନା rxkb_log_level level,
               ଅଚଳ ଅକ୍ଷର *fmt, va_list args)
{
    ଅଚଳ ଅକ୍ଷର *prefix = log_level_to_prefix(level);

    ଯଦି (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ସ୍ଥିର ଗଣନା rxkb_log_level
log_level(ଅଚଳ ଅକ୍ଷର *level) {
    errno = 0;
    ଅକ୍ଷର *endptr;
    ଅଚଳ long lvl = strtol(level, &endptr, 10);
    ଯଦି (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ଫେରନ୍ତୁ (ଗଣନା rxkb_log_level)lvl;

    ଯଦି (istreq_prefix("crit", level))
        ଫେରନ୍ତୁ RXKB_LOG_LEVEL_CRITICAL;
    ଯଦି (istreq_prefix("err", level))
        ଫେରନ୍ତୁ RXKB_LOG_LEVEL_ERROR;
    ଯଦି (istreq_prefix("warn", level))
        ଫେରନ୍ତୁ RXKB_LOG_LEVEL_WARNING;
    ଯଦି (istreq_prefix("info", level))
        ଫେରନ୍ତୁ RXKB_LOG_LEVEL_INFO;
    ଯଦି (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        ଫେରନ୍ତୁ RXKB_LOG_LEVEL_DEBUG;

    ଫେରନ୍ତୁ RXKB_LOG_LEVEL_ERROR;
}

ସଂରଚନା rxkb_context *
rxkb_context_new(ଗଣନା rxkb_context_flags flags)
{
    ସଂରଚନା rxkb_context *ctx = rxkb_context_create(ଖାଲି);
    ଅଚଳ ଅକ୍ଷର *env;

    ଯଦି (!ctx)
        ଫେରନ୍ତୁ ଖାଲି;

    ctx->context_state = CONTEXT_NEW;
    ctx->load_extra_rules_files = flags & RXKB_CONTEXT_LOAD_EXOTIC_RULES;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ଯଦି (env)
        rxkb_context_set_log_level(ctx, log_level(env));

    ସ୍ଥିର ଅଚଳ ଗଣନା rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_LOAD_EXOTIC_RULES
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ଯଦି (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        ମୁକ୍ତ(ctx);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ଯଦି (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_path_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_PATH,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ମୂଳ);
        rxkb_context_unref(ctx);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ
rxkb_context_set_log_fn(ସଂରଚନା rxkb_context *ctx,
                        ଶୂନ୍ୟ (*log_fn)(ସଂରଚନା rxkb_context *ctx,
                                       ଗଣନା rxkb_log_level level,
                                       ଅଚଳ ଅକ୍ଷର *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_path_append(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

    ଯଦି (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ଯାଅ error;
    }

    ସଂରଚନା stat stat_buf;
    err = stat(path, &stat_buf);
    ଯଦି (err != 0) {
        err = errno;
        ଯାଅ error;
    }
    ଯଦି (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ଯାଅ error;
    }

    ଯଦି (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ଯାଅ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    ଅକ୍ଷର rules[PATH_MAX];
    ଯଦି (!snprintf_safe(rules, ଆକାର(rules), "%s/rules/%s.xml",
                       path, DEFAULT_XKB_RULES)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                ଆକାର(rules), path, DEFAULT_XKB_RULES);
        ଯାଅ error;
    }

    ଅକ୍ଷର * ଅଚଳ tmp = strdup(path);
    ଯଦି (!tmp) {
        err = ENOMEM;
        ଯାଅ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    ଫେରନ୍ତୁ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    ଫେରନ୍ତୁ false;
}

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_str(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଫେରନ୍ତୁ strcmp(*(ଅକ୍ଷର **)a, *(ଅକ୍ଷର **) b);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
add_direct_subdirectories(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path,
                          darray_string *extensions,
                          darray_size_t versioned_ଗଣନା,
                          size_t versioned_path_length)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = ENOMEM;
    DIR *dir = ଖାଲି;

    /* Check extensions parent directory */
    ସଂରଚନା stat stat_buf;
    err = stat(path, &stat_buf);
    ଯଦି (err != 0) {
        err = errno;
        ଯାଅ err;
    }
    ଯଦି (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        ଯାଅ err;
    }
    ଯଦି (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        ଯାଅ err;
    }

    dir = opendir(path);
    ଯଦି (dir == ଖାଲି) {
        err = EACCES;
        ଯାଅ err;
    }

    ସଂରଚନା dirent *entry;
    ଅକ୍ଷର path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    ଯେତେବେଳେ ((entry = readdir(dir)) != ଖାଲି) {
        ଅଚଳ ଅକ୍ଷର *name = entry->d_name;

        /* Skip special entries */
        ଯଦି (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ଜାରି;

        ଯଦି (!snprintf_safe(path_buf, ଆକାର(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            ଯାଅ err;
        }
        ଯଦି (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ଜାରି;

        /* Skip if the corresponding versioned directory is already included */
        ପାଇଁ (darray_size_t i = 0; i < versioned_ଗଣନା; i++) {
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            ଯଦି (strcmp(name, prev_name) == 0)
                ଯାଅ next;
        }

        ଅକ୍ଷର *ext_path = strdup_safe(path_buf);
        ଯଦି (!ext_path) {
            err = ENOMEM;
            ଯାଅ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ଯଦି (darray_size(*extensions) > versioned_ଗଣନା) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(*extensions) + versioned_ଗଣନା,
              darray_size(*extensions) - versioned_ଗଣନା,
              ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(*extensions)), &compare_str);
        ଅକ୍ଷର **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ଗଣନା) {
            ret |= rxkb_context_include_path_append(ctx, *ext_path);
        }
    }

    ଫେରନ୍ତୁ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    ଯଦି (dir)
        closedir(dir);

    ଫେରନ୍ତୁ ret;
}
#endif

bool
rxkb_context_include_path_append_default(ସଂରଚନା rxkb_context *ctx)
{
    ଅକ୍ଷର user_path[PATH_MAX];
    bool ret = false;

    ଯଦି (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "include paths can only be appended to a new context\n");
        ଫେରନ୍ତୁ false;
    }

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ home = rxkb_context_getenv(ctx, "HOME");
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ଯଦି (xdg != ଖାଲି) {
        ଯଦି (snprintf_safe(user_path, ଆକାର(user_path), "%s/xkb", xdg))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    } ଅନ୍ୟଥା ଯଦି (home != ଖାଲି) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ଯଦି (snprintf_safe(user_path, ଆକାର(user_path), "%s/.config/xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ଯଦି (home != ଖାଲି) {
        ଯଦି (snprintf_safe(user_path, ଆକାର(user_path), "%s/.xkb", home))
            ret |= rxkb_context_include_path_append(ctx, user_path);
    }

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_path_append(
        ctx, ((extra != ଖାଲି) ? extra : DFLT_XKB_CONFIG_EXTRA_PATH)
    );

#ଯଦି HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ଅଚଳ ଅକ୍ଷର *extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
    ଯଦି (extensions_path == ଖାଲି)
        extensions_path = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#endif
    size_t versioned_path_length = 0;
    ଯଦି (extensions_path != ଖାଲି) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
    ଯଦି (extensions_path == ଖାଲି)
        extensions_path = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#endif
    ଯଦି (extensions_path != ଖାଲି) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    ଅକ୍ଷର **ext_path;
    darray_foreach(ext_path, extensions) {
        ମୁକ୍ତ(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ ମୂଳ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ଅଚଳ bool has_ମୂଳ = rxkb_context_include_path_append(
        ctx, ((ମୂଳ != ଖାଲି) ? ମୂଳ: DFLT_XKB_CONFIG_ମୂଳ)
    );
    ret |= has_ମୂଳ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ଯଦି (!has_ମୂଳ && (ମୂଳ == ଖାଲି || ମୂଳ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ମୂଳ,
                 ((ମୂଳ == ଖାଲି) ? DFLT_XKB_CONFIG_ମୂଳ : ମୂଳ));
        ret |= rxkb_context_include_path_append(ctx, DFLT_XKB_LEGACY_ମୂଳ);
    }

    ଫେରନ୍ତୁ ret;
}

bool
rxkb_context_parse_default_ruleset(ସଂରଚନା rxkb_context *ctx)
{
    ଫେରନ୍ତୁ rxkb_context_parse(ctx, DEFAULT_XKB_RULES);
}

bool
rxkb_context_parse(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *ruleset)
{
    ଅକ୍ଷର **path;
    bool success = false;

    ଯଦି (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "parse must only be called on a new context\n");
        ଫେରନ୍ତୁ false;
    }

    darray_foreach_reverse(path, ctx->includes) {
        ଅକ୍ଷର rules[PATH_MAX];

        ଯଦି (snprintf_safe(rules, ଆକାର(rules), "%s/rules/%s.xml",
                           *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ଯଦି (parse(ctx, rules, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ଯଦି (ctx->load_extra_rules_files &&
            snprintf_safe(rules, ଆକାର(rules), "%s/rules/%s.extras.xml",
                          *path, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", rules);
            ଯଦି (parse(ctx, rules, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ଫେରନ୍ତୁ success;
}


ଶୂନ୍ୟ
rxkb_context_set_user_data(ସଂରଚନା rxkb_context *ctx, ଶୂନ୍ୟ *userdata)
{
    ctx->userdata = userdata;
}

ଶୂନ୍ୟ *
rxkb_context_get_user_data(ସଂରଚନା rxkb_context *ctx)
{
    ଫେରନ୍ତୁ ctx->userdata;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_ଗଣ୍ଠି(xml_ଗଣ୍ଠି *ଗଣ୍ଠି, ଅଚଳ ଅକ୍ଷର *name)
{
    ଫେରନ୍ତୁ ଗଣ୍ଠି->type == XML_ELEMENT_ଗଣ୍ଠି &&
        xmlStrEqual(ଗଣ୍ଠି->name, (ଅଚଳ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ସ୍ଥିର ଅକ୍ଷର *
extract_text(xml_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
    xml_ଗଣ୍ଠି *n;

    ପାଇଁ (n = ଗଣ୍ଠି->children; n; n = n->next) {
        ଯଦି (n->type == XML_TEXT_ଗଣ୍ଠି)
            ଫେରନ୍ତୁ (ଅକ୍ଷର *)xmlStrdup(n->content);
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

/* Data from “configItem” node */
ସଂରଚନା config_item {
    ଅକ୍ଷର *name;
    ଅକ୍ଷର *description;
    ଅକ୍ଷର *brief;
    ଅକ୍ଷର *vendor;
    ଗଣନା rxkb_popularity popularity;
    bool layout_specific;
};

#define config_item_new(popularity_) { \
    .name = ଖାଲି, \
    .description = ଖାଲି, \
    .brief = ଖାଲି, \
    .vendor = ଖାଲି, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ସ୍ଥିର ଶୂନ୍ୟ
config_item_free(ସଂରଚନା config_item *config) {
    ମୁକ୍ତ(config->name);
    ମୁକ୍ତ(config->description);
    ମୁକ୍ତ(config->brief);
    ମୁକ୍ତ(config->vendor);
}

ସ୍ଥିର bool
parse_config_item(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *parent,
                  ସଂରଚନା config_item *config)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
    xml_ଗଣ୍ଠି *ci = ଖାଲି;

    ପାଇଁ (ci = parent->children; ci; ci = ci->next) {
        ଯଦି (is_ଗଣ୍ଠି(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ଅଚଳ xmlChar*)"popularity");
            ଯଦି (raw_popularity) {
                ଯଦି (xmlStrEqual(raw_popularity, (ଅଚଳ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ଅନ୍ୟଥା ଯଦି (xmlStrEqual(raw_popularity, (ଅଚଳ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ଅନ୍ୟଥା
                    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->ଧାଡ଼ି, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ଅଚଳ xmlChar*)"layout-specific");
            ଯଦି (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ଅଚଳ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            ପାଇଁ (ଗଣ୍ଠି = ci->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
                ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "name"))
                    config->name = extract_text(ଗଣ୍ଠି);
                ଅନ୍ୟଥା ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "description"))
                    config->description = extract_text(ଗଣ୍ଠି);
                ଅନ୍ୟଥା ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "shortDescription"))
                    config->brief = extract_text(ଗଣ୍ଠି);
                ଅନ୍ୟଥା ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "vendor"))
                    config->vendor = extract_text(ଗଣ୍ଠି);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ଯଦି (!config->name || !strlen(config->name))  {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->ଧାଡ଼ି);
                config_item_free(config);
                ଫେରନ୍ତୁ false;
            }

            ଫେରନ୍ତୁ true; /* only one configItem allowed in the dtd */
        }
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_model(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *model,
            ଗଣନା rxkb_popularity popularity)
{
    ସଂରଚନା config_item config = config_item_new(popularity);

    ଯଦି (parse_config_item(ctx, model, &config)) {
        ସଂରଚନା rxkb_model *m;

        list_for_each(m, &ctx->models, base.link) {
            ଯଦି (streq(m->name, config.name)) {
                config_item_free(&config);
                ଫେରନ୍ତୁ;
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

ସ୍ଥିର ଶୂନ୍ୟ
parse_model_list(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *model_list,
                 ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    ପାଇଁ (ଗଣ୍ଠି = model_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "model"))
            parse_model(ctx, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_language_list(xml_ଗଣ୍ଠି *language_list, ସଂରଚନା rxkb_layout *layout)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
    ସଂରଚନା rxkb_iso639_code *code;

    ପାଇଁ (ଗଣ୍ଠି = language_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "iso639Id")) {
            ଅକ୍ଷର *str = extract_text(ଗଣ୍ଠି);
            ସଂରଚନା rxkb_object *parent;

            ଯଦି (!str || strlen(str) != 3) {
                ମୁକ୍ତ(str);
                ଜାରି;
            }

            parent = &layout->base;
            code = rxkb_iso639_code_create(parent);
            code->code = str;
            list_append(&layout->iso639s, &code->base.link);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_country_list(xml_ଗଣ୍ଠି *country_list, ସଂରଚନା rxkb_layout *layout)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
    ସଂରଚନା rxkb_iso3166_code *code;

    ପାଇଁ (ଗଣ୍ଠି = country_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "iso3166Id")) {
            ଅକ୍ଷର *str = extract_text(ଗଣ୍ଠି);
            ସଂରଚନା rxkb_object *parent;

            ଯଦି (!str || strlen(str) != 2) {
                ମୁକ୍ତ(str);
                ଜାରି;
            }

            parent = &layout->base;
            code = rxkb_iso3166_code_create(parent);
            code->code = str;
            list_append(&layout->iso3166s, &code->base.link);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_variant(ସଂରଚନା rxkb_context *ctx, ସଂରଚନା rxkb_layout *l,
              xml_ଗଣ୍ଠି *variant, ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ci;
    ସଂରଚନା config_item config = config_item_new(popularity);

    ଯଦି (parse_config_item(ctx, variant, &config)) {
        ସଂରଚନା rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, base.link) {
            ଯଦି (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ଭାଙ୍ଗ;
            }
        }

        ଯଦି (!exists) {
            v = rxkb_layout_create(&ctx->base);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ଖାଲି ? strdup_safe(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->base.link);

            ପାଇଁ (ci = variant->children; ci; ci = ci->next) {
                xml_ଗଣ୍ଠି *ଗଣ୍ଠି;

                ଯଦି (!is_ଗଣ୍ଠି(ci, "configItem"))
                    ଜାରି;

                bool found_language_list = false;
                bool found_country_list = false;
                ପାଇଁ (ଗଣ୍ଠି = ci->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
                    ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "languageList")) {
                        parse_language_list(ଗଣ୍ଠି, v);
                        found_language_list = true;
                    }
                    ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "countryList")) {
                        parse_country_list(ଗଣ୍ଠି, v);
                        found_country_list = true;
                    }
                }
                ଯଦି (!found_language_list) {
                    // inherit from parent layout
                    ସଂରଚନା rxkb_iso639_code* x;
                    list_for_each(x, &l->iso639s, base.link) {
                        ସଂରଚନା rxkb_iso639_code* code = rxkb_iso639_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso639s, &code->base.link);
                    }
                }
                ଯଦି (!found_country_list) {
                    // inherit from parent layout
                    ସଂରଚନା rxkb_iso3166_code* x;
                    list_for_each(x, &l->iso3166s, base.link) {
                        ସଂରଚନା rxkb_iso3166_code* code = rxkb_iso3166_code_create(&v->base);
                        code->code = strdup(x->code);
                        list_append(&v->iso3166s, &code->base.link);
                    }
                }
            }
        } ଅନ୍ୟଥା {
            config_item_free(&config);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_variant_list(ସଂରଚନା rxkb_context *ctx, ସଂରଚନା rxkb_layout *l,
                   xml_ଗଣ୍ଠି *variant_list, ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    ପାଇଁ (ଗଣ୍ଠି = variant_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "variant"))
            parse_variant(ctx, l, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_layout(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *layout,
             ଗଣନା rxkb_popularity popularity)
{
    ସଂରଚନା config_item config = config_item_new(popularity);
    ସଂରଚନା rxkb_layout *l;
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
    bool exists = false;

    ଯଦି (!parse_config_item(ctx, layout, &config))
        ଫେରନ୍ତୁ;

    list_for_each(l, &ctx->layouts, base.link) {
        ଯଦି (streq(l->name, config.name) && l->variant == ଖାଲି) {
            exists = true;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (!exists) {
        l = rxkb_layout_create(&ctx->base);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ଖାଲି;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->base.link);
    } ଅନ୍ୟଥା {
        config_item_free(&config);
    }

    ପାଇଁ (ଗଣ୍ଠି = layout->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "variantList")) {
            parse_variant_list(ctx, l, ଗଣ୍ଠି, popularity);
        }
        ଯଦି (!exists && is_ଗଣ୍ଠି(ଗଣ୍ଠି, "configItem")) {
            xml_ଗଣ୍ଠି *ll;
            ପାଇଁ (ll = ଗଣ୍ଠି->children; ll; ll = ll->next) {
                ଯଦି (is_ଗଣ୍ଠି(ll, "languageList"))
                    parse_language_list(ll, l);
                ଯଦି (is_ଗଣ୍ଠି(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_layout_list(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *layout_list,
                  ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    ପାଇଁ (ଗଣ୍ଠି = layout_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "layout"))
            parse_layout(ctx, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_option(ସଂରଚନା rxkb_context *ctx, ସଂରଚନା rxkb_option_group *group,
             xml_ଗଣ୍ଠି *option, ଗଣନା rxkb_popularity popularity)
{
    ସଂରଚନା config_item config = config_item_new(popularity);

    ଯଦି (parse_config_item(ctx, option, &config)) {
        ସଂରଚନା rxkb_option *o;

        list_for_each(o, &group->options, base.link) {
            ଯଦି (streq(o->name, config.name)) {
                config_item_free(&config);
                ଫେରନ୍ତୁ;
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

ସ୍ଥିର ଶୂନ୍ୟ
parse_group(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *group,
            ଗଣନା rxkb_popularity popularity)
{
    ସଂରଚନା config_item config = config_item_new(popularity);
    ସଂରଚନା rxkb_option_group *g;
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
    xmlChar *multiple;
    bool exists = false;

    ଯଦି (!parse_config_item(ctx, group, &config))
        ଫେରନ୍ତୁ;

    list_for_each(g, &ctx->option_groups, base.link) {
        ଯଦି (streq(g->name, config.name)) {
            exists = true;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (!exists) {
        g = rxkb_option_group_create(&ctx->base);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        multiple = xmlGetProp(group, (ଅଚଳ xmlChar*)"allowMultipleSelection");
        ଯଦି (multiple && xmlStrEqual(multiple, (ଅଚଳ xmlChar*)"true"))
            g->allow_multiple = true;
        xmlFree(multiple);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->base.link);
    } ଅନ୍ୟଥା {
        config_item_free(&config);
    }

    ପାଇଁ (ଗଣ୍ଠି = group->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "option"))
            parse_option(ctx, g, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_option_list(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *option_list,
                  ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    ପାଇଁ (ଗଣ୍ଠି = option_list->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "group"))
            parse_group(ctx, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_rules_xml(ସଂରଚନା rxkb_context *ctx, xml_ଗଣ୍ଠି *ମୂଳ,
                ଗଣନା rxkb_popularity popularity)
{
    xml_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    ପାଇଁ (ଗଣ୍ଠି = ମୂଳ->children; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next) {
        ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "modelList"))
            parse_model_list(ctx, ଗଣ୍ଠି, popularity);
        ଅନ୍ୟଥା ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "layoutList"))
            parse_layout_list(ctx, ଗଣ୍ଠି, popularity);
        ଅନ୍ୟଥା ଯଦି (is_ଗଣ୍ଠି(ଗଣ୍ଠି, "optionList"))
            parse_option_list(ctx, ଗଣ୍ଠି, popularity);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
ATTR_PRINTF(2, 0)
xml_error_func(ଶୂନ୍ୟ *ctx, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
    ସ୍ଥିର ଅକ୍ଷର buf[PATH_MAX];
    ସ୍ଥିର size_t slen = 0;
    va_list args;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_start(args, ସନ୍ଦେଶ);
    rc = vsnprintf(&buf[slen], ଆକାର(buf) - slen, ସନ୍ଦେଶ, args);
    va_end(args);

    /* This shouldn't really happen */
    ଯଦି (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, ଆକାର(buf));
        ଫେରନ୍ତୁ;
    }

    slen += (size_t)rc;
    ଯଦି (slen >= ଆକାର(buf)) {
        /* truncated, let's flush this */
        buf[ଆକାର(buf) - 1] = '\n';
        slen = ଆକାର(buf);
    }

    /* We're assuming here that the last character is \n. */
    ଯଦି (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "%s", buf);
        memset(buf, 0, ଆକାର(buf));
        slen = 0;
    }
}

#ଯଦି HAVE_XML_CTXT_SET_ERRORHANDLER
ସ୍ଥିର ଶୂନ୍ୟ
xml_structured_error_func(ଶୂନ୍ୟ *userData, ଅଚଳ xmlError * error)
{
    xmlFormatError(error, xml_error_func, userData);
}
#endif

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ଅନ୍ୟଥା
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif

ସ୍ଥିର bool
validate(ସଂରଚନା rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ଅଚଳ ଅକ୍ଷର dtdstr[] =
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

#ଯଦି HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ଯଦି (!xmlCtxt)
        ଫେରନ୍ତୁ false;
    static_assert(HAVE_XML_CTXT_SET_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
    xmlCtxtSetOptions(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ନିବେଶ_Ptr pinput =
        xml_New_ନିବେଶ_From_Memory(ଖାଲି, dtdstr, ARRAY_SIZE(dtdstr) - 1,
                              XML_ନିବେଶ_BUF_STATIC);
    ଯଦି (!pinput)
        ଯାଅ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ଖାଲି, ଖାଲି);
#ଅନ୍ୟଥା
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ନିବେଶ_Buffer_Ptr buf =
        xml_Parser_ନିବେଶ_Buffer_Create_Mem(dtdstr, ARRAY_SIZE(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ଯଦି (!buf)
        ଯାଅ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ଖାଲି, buf, XML_CHAR_ENCODING_UTF8);
#endif

    ଯଦି (!dtd) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID, "Failed to load DTD\n");
        ଯାଅ dtd_error;
    }

#ଯଦି HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ଅନ୍ୟଥା
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ଯଦି (!dtdvalid) {
        success = false;
        ଯାଅ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->userData = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#endif
    xmlFreeDtd(dtd);

dtd_error:
#ଯଦି HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#endif
    ଫେରନ୍ତୁ success;
}

ସ୍ଥିର bool
parse(ସଂରଚନା rxkb_context *ctx, ଅଚଳ ଅକ୍ଷର *path,
      ଗଣନା rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ଖାଲି;
    xml_ଗଣ୍ଠି *ମୂଳ = ଖାଲି;

    ଯଦି (!check_eaccess(path, R_OK))
        ଫେରନ୍ତୁ false;

    LIBXML_TEST_VERSION

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ଯଦି (!xmlCtxt)
        ଫେରନ୍ତୁ false;

#ifdef XML_PARSE_NO_XXE
#define _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ଅନ୍ୟଥା
#define _XML_OPTIONS (XML_PARSE_NONET)
#endif
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ଯଦି HAVE_XML_CTXT_SET_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xmlCtxtSetErrorHandler(xmlCtxt, xml_structured_error_func, ctx);
#endif
#ଯଦି !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xmlSetGenericErrorFunc(ctx, xml_error_func);
#endif

    doc = xmlCtxtReadFile(xmlCtxt, path, ଖାଲି, 0);
    ଯଦି (!doc)
        ଯାଅ parse_error;

    ଯଦି (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "XML error: failed to validate document at %s\n", path);
        ଯାଅ validate_error;
    }

    ମୂଳ = xml_Doc_Get_ମୂଳ_Element(doc);
    parse_rules_xml(ctx, ମୂଳ, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ଯଦି !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xmlSetGenericErrorFunc(ଖାଲି, ଖାଲି);
#endif
#ଯଦି HAVE_XML_CTXT_SET_ERRORHANDLER
    xmlCtxtSetErrorHandler(xmlCtxt, ଖାଲି, ଖାଲି);
#endif
    xmlFreeParserCtxt(xmlCtxt);

    ଫେରନ୍ତୁ success;
}
