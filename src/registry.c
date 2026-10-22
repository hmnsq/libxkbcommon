/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libxml/parser.h>

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
    #ਜੇ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
        #ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbregistry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"

ਬਣਤਰ rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ਬਣਤਰ rxkb_object {
    ਬਣਤਰ rxkb_object *ਮਾਪਾ;
    uint32_t refcount;
    ਬਣਤਰ list link;
};

ਬਣਤਰ rxkb_iso639_ਕੋਡ {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;
    ਅੱਖਰ *ਕੋਡ;
};

ਬਣਤਰ rxkb_iso3166_ਕੋਡ {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;
    ਅੱਖਰ *ਕੋਡ;
};

ਗਿਣਤੀ context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ਬਣਤਰ rxkb_context {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;
    ਗਿਣਤੀ context_state context_state;

    bool ਲੱਦੋ_extra_ਨਿਯਮਾਂ_files;
    bool use_secure_getenv;

    ਬਣਤਰ list models;         /* list of struct rxkb_models */
    ਬਣਤਰ list layouts;        /* list of struct rxkb_layouts */
    ਬਣਤਰ list option_groups;  /* list of struct rxkb_option_group */

    darray(ਅੱਖਰ *) includes;


    ATTR_PRINTF(3, 0) ਖਾਲੀ (*log_fn)(ਬਣਤਰ rxkb_context *ctx,
                                     ਗਿਣਤੀ rxkb_log_level level,
                                     ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ);
    ਗਿਣਤੀ rxkb_log_level log_level;

    ਖਾਲੀ *userdata;
};

ਬਣਤਰ rxkb_model {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;

    ਅੱਖਰ *name;
    ਅੱਖਰ *vendor;
    ਅੱਖਰ *description;
    ਗਿਣਤੀ rxkb_popularity popularity;
};

ਬਣਤਰ rxkb_layout {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;

    ਅੱਖਰ *name;
    ਅੱਖਰ *brief;
    ਅੱਖਰ *description;
    ਅੱਖਰ *variant;
    ਗਿਣਤੀ rxkb_popularity popularity;

    ਬਣਤਰ list iso639s;  /* list of struct rxkb_iso639_code */
    ਬਣਤਰ list iso3166s; /* list of struct rxkb_iso3166_code */
};

ਬਣਤਰ rxkb_option_ਸਮੂਹ {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;

    bool allow_ਗੁਣਜ;
    ਬਣਤਰ list options; /* list of struct rxkb_options */
    ਅੱਖਰ *name;
    ਅੱਖਰ *description;
    ਗਿਣਤੀ rxkb_popularity popularity;
};

ਬਣਤਰ rxkb_option {
    ਬਣਤਰ rxkb_object ਅਧਾਰ;

    ਅੱਖਰ *name;
    ਅੱਖਰ *brief;
    ਅੱਖਰ *description;
    ਗਿਣਤੀ rxkb_popularity popularity;
    bool layout_specific;
};

ਸਥਿਰ bool
parse(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
      ਗਿਣਤੀ rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ਸਥਿਰ ਖਾਲੀ
rxkb_log(ਬਣਤਰ rxkb_context *ctx, ਗਿਣਤੀ rxkb_log_level level,
         ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list ਦਲੀਲਾਂ;

    ਜੇ (ctx->log_level < level)
        ਵਾਪਸ;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    ctx->log_fn(ctx, level, fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rxkb_log_with_ਕੋਡ(ctx, level, ਸੁਨੇਹਾ_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_ਸੁਨੇਹਾ_ID(ਸੁਨੇਹਾ_id, fmt), ##__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_info(ctx, id, ...) \
    rxkb_log_with_ਕੋਡ((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_warn(ctx, id, ...) \
    rxkb_log_with_ਕੋਡ((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_err(ctx, id, ...) \
    rxkb_log_with_ਕੋਡ((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ log_wsgo(ctx, id, ...) \
    rxkb_log_with_ਕੋਡ((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_REF_UNREF_FOR_TYPE(type_) \
ਬਣਤਰ type_ * type_##_ref(ਬਣਤਰ type_ *object) { \
    rxkb_object_ref(&object->ਅਧਾਰ); \
    ਵਾਪਸ object; \
} \
ਬਣਤਰ type_ * type_##_unref(ਬਣਤਰ type_ *object) { \
    ਜੇ (!object) ਵਾਪਸ ਨਿਰਾ; \
    ਪ੍ਰਤਿਜ੍ਞਾ(object->ਅਧਾਰ.refcount >= 1); \
    ਜੇ (--object->ਅਧਾਰ.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->ਅਧਾਰ.link);\
        ਮੁਕਤ(object); \
    } \
    ਵਾਪਸ ਨਿਰਾ;\
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_CREATE_FOR_TYPE(type_) \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ type_ * type_##_create(ਬਣਤਰ rxkb_object *ਮਾਪਾ) { \
    ਬਣਤਰ type_ *t = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ *t); \
    ਜੇ (t) \
        rxkb_object_init(&t->ਅਧਾਰ, ਮਾਪਾ); \
    ਵਾਪਸ t; \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_TYPED_GETTER_FOR_TYPE(type_, ਖੇਤਰ, rtype_) \
rtype_ type_##_get_##field_(ਬਣਤਰ type_ *object) { \
    ਵਾਪਸ object->ਖੇਤਰ; \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_GETTER_FOR_TYPE(type_, ਖੇਤਰ) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, ਖੇਤਰ, ਅਚਲ ਅੱਖਰ*)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_ਪਹਿਲਾ_ਅਗਲਾ_FOR_TYPE(type_, ਮਾਪਾ_type, ਮਾਪਾ_ਖੇਤਰ) \
ਬਣਤਰ type_ * type_##_first(ਬਣਤਰ ਮਾਪਾ_type *ਮਾਪਾ) { \
    ਬਣਤਰ type_ *o = ਨਿਰਾ; \
    ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&ਮਾਪਾ->ਮਾਪਾ_ਖੇਤਰ)) \
        o = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&ਮਾਪਾ->ਮਾਪਾ_ਖੇਤਰ, o, ਅਧਾਰ.link); \
    ਵਾਪਸ o; \
} \
ਬਣਤਰ type_ * \
type_##_next(ਬਣਤਰ type_ *o) \
{ \
    ਬਣਤਰ ਮਾਪਾ_type *ਮਾਪਾ; \
    ਬਣਤਰ type_ *ਅਗਲਾ; \
    ਮਾਪਾ = container_ਦਾ(o->ਅਧਾਰ.ਮਾਪਾ, ਬਣਤਰ ਮਾਪਾ_type, ਅਧਾਰ); \
    ਅਗਲਾ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&o->ਅਧਾਰ.link, o, ਅਧਾਰ.link); \
    ਜੇ (list_is_ਆਖਰੀ(&ਮਾਪਾ->ਮਾਪਾ_ਖੇਤਰ, &o->ਅਧਾਰ.link)) \
        ਵਾਪਸ ਨਿਰਾ; \
    ਵਾਪਸ ਅਗਲਾ; \
}

ਸਥਿਰ ਖਾਲੀ
rxkb_object_init(ਬਣਤਰ rxkb_object *object, ਬਣਤਰ rxkb_object *ਮਾਪਾ)
{
    object->refcount = 1;
    object->ਮਾਪਾ = ਮਾਪਾ;
    list_init(&object->link);
}

ਸਥਿਰ ਖਾਲੀ *
rxkb_object_ref(ਬਣਤਰ rxkb_object *object)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(object->refcount >= 1);
    ++object->refcount;
    ਵਾਪਸ object;
}

ਸਥਿਰ ਖਾਲੀ
rxkb_iso639_ਕੋਡ_destroy(ਬਣਤਰ rxkb_iso639_ਕੋਡ *ਕੋਡ)
{
    ਮੁਕਤ(ਕੋਡ->ਕੋਡ);
}

ਬਣਤਰ rxkb_iso639_ਕੋਡ *
rxkb_layout_get_iso639_ਪਹਿਲਾ(ਬਣਤਰ rxkb_layout *layout)
{
    ਬਣਤਰ rxkb_iso639_ਕੋਡ *ਕੋਡ = ਨਿਰਾ;

    ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&layout->iso639s))
        ਕੋਡ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&layout->iso639s, ਕੋਡ, ਅਧਾਰ.link);

    ਵਾਪਸ ਕੋਡ;
}

ਬਣਤਰ rxkb_iso639_ਕੋਡ *
rxkb_iso639_ਕੋਡ_ਅਗਲਾ(ਬਣਤਰ rxkb_iso639_ਕੋਡ *ਕੋਡ)
{
    ਬਣਤਰ rxkb_iso639_ਕੋਡ *ਅਗਲਾ = ਨਿਰਾ;
    ਬਣਤਰ rxkb_layout *layout;

    layout = container_ਦਾ(ਕੋਡ->ਅਧਾਰ.ਮਾਪਾ, ਬਣਤਰ rxkb_layout, ਅਧਾਰ);

    ਜੇ (list_is_ਆਖਰੀ(&layout->iso639s, &ਕੋਡ->ਅਧਾਰ.link))
        ਵਾਪਸ ਨਿਰਾ;

    ਅਗਲਾ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&ਕੋਡ->ਅਧਾਰ.link, ਕੋਡ, ਅਧਾਰ.link);

    ਵਾਪਸ ਅਗਲਾ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_ਕੋਡ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_ਕੋਡ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_ਕੋਡ, ਕੋਡ);

ਸਥਿਰ ਖਾਲੀ
rxkb_iso3166_ਕੋਡ_destroy(ਬਣਤਰ rxkb_iso3166_ਕੋਡ *ਕੋਡ)
{
    ਮੁਕਤ(ਕੋਡ->ਕੋਡ);
}

ਬਣਤਰ rxkb_iso3166_ਕੋਡ *
rxkb_layout_get_iso3166_ਪਹਿਲਾ(ਬਣਤਰ rxkb_layout *layout)
{
    ਬਣਤਰ rxkb_iso3166_ਕੋਡ *ਕੋਡ = ਨਿਰਾ;

    ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&layout->iso3166s))
        ਕੋਡ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&layout->iso3166s, ਕੋਡ, ਅਧਾਰ.link);

    ਵਾਪਸ ਕੋਡ;
}

ਬਣਤਰ rxkb_iso3166_ਕੋਡ *
rxkb_iso3166_ਕੋਡ_ਅਗਲਾ(ਬਣਤਰ rxkb_iso3166_ਕੋਡ *ਕੋਡ)
{
    ਬਣਤਰ rxkb_iso3166_ਕੋਡ *ਅਗਲਾ = ਨਿਰਾ;
    ਬਣਤਰ rxkb_layout *layout;

    layout = container_ਦਾ(ਕੋਡ->ਅਧਾਰ.ਮਾਪਾ, ਬਣਤਰ rxkb_layout, ਅਧਾਰ);

    ਜੇ (list_is_ਆਖਰੀ(&layout->iso3166s, &ਕੋਡ->ਅਧਾਰ.link))
        ਵਾਪਸ ਨਿਰਾ;

    ਅਗਲਾ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&ਕੋਡ->ਅਧਾਰ.link, ਕੋਡ, ਅਧਾਰ.link);

    ਵਾਪਸ ਅਗਲਾ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_ਕੋਡ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_ਕੋਡ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_ਕੋਡ, ਕੋਡ);

ਸਥਿਰ ਖਾਲੀ
rxkb_option_destroy(ਬਣਤਰ rxkb_option *o)
{
    ਮੁਕਤ(o->name);
    ਮੁਕਤ(o->brief);
    ਮੁਕਤ(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ਗਿਣਤੀ rxkb_popularity);
bool rxkb_option_is_layout_specific(ਬਣਤਰ rxkb_option *object) {
    ਵਾਪਸ object->layout_specific;
}
DECLARE_ਪਹਿਲਾ_ਅਗਲਾ_FOR_TYPE(rxkb_option, rxkb_option_ਸਮੂਹ, options);

ਸਥਿਰ ਖਾਲੀ
rxkb_layout_destroy(ਬਣਤਰ rxkb_layout *l)
{
    ਬਣਤਰ rxkb_iso639_ਕੋਡ *iso639, *tmp_639;
    ਬਣਤਰ rxkb_iso3166_ਕੋਡ *iso3166, *tmp_3166;

    ਮੁਕਤ(l->name);
    ਮੁਕਤ(l->brief);
    ਮੁਕਤ(l->description);
    ਮੁਕਤ(l->variant);

    list_for_each_ਸੁਰੱਖਿਅਤ(iso639, tmp_639, &l->iso639s, ਅਧਾਰ.link) {
        rxkb_iso639_ਕੋਡ_unref(iso639);
    }
    list_for_each_ਸੁਰੱਖਿਅਤ(iso3166, tmp_3166, &l->iso3166s, ਅਧਾਰ.link) {
        rxkb_iso3166_ਕੋਡ_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ਗਿਣਤੀ rxkb_popularity);
DECLARE_ਪਹਿਲਾ_ਅਗਲਾ_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ਸਥਿਰ ਖਾਲੀ
rxkb_model_destroy(ਬਣਤਰ rxkb_model *m)
{
    ਮੁਕਤ(m->name);
    ਮੁਕਤ(m->vendor);
    ਮੁਕਤ(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ਗਿਣਤੀ rxkb_popularity);
DECLARE_ਪਹਿਲਾ_ਅਗਲਾ_FOR_TYPE(rxkb_model, rxkb_context, models);

ਸਥਿਰ ਖਾਲੀ
rxkb_option_ਸਮੂਹ_destroy(ਬਣਤਰ rxkb_option_ਸਮੂਹ *og)
{
    ਬਣਤਰ rxkb_option *o, *otmp;

    ਮੁਕਤ(og->name);
    ਮੁਕਤ(og->description);

    list_for_each_ਸੁਰੱਖਿਅਤ(o, otmp, &og->options, ਅਧਾਰ.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_ਸਮੂਹ_allows_ਗੁਣਜ(ਬਣਤਰ rxkb_option_ਸਮੂਹ *g)
{
    ਵਾਪਸ g->allow_ਗੁਣਜ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_ਸਮੂਹ);
DECLARE_CREATE_FOR_TYPE(rxkb_option_ਸਮੂਹ);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ਸਮੂਹ, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ਸਮੂਹ, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_ਸਮੂਹ, popularity, ਗਿਣਤੀ rxkb_popularity);
DECLARE_ਪਹਿਲਾ_ਅਗਲਾ_FOR_TYPE(rxkb_option_ਸਮੂਹ, rxkb_context, option_groups);

ਸਥਿਰ ਖਾਲੀ
rxkb_context_destroy(ਬਣਤਰ rxkb_context *ctx)
{
    ਬਣਤਰ rxkb_model *m, *mtmp;
    ਬਣਤਰ rxkb_layout *l, *ltmp;
    ਬਣਤਰ rxkb_option_ਸਮੂਹ *og, *ogtmp;
    ਅੱਖਰ **ਰਸਤਾ;

    list_for_each_ਸੁਰੱਖਿਅਤ(m, mtmp, &ctx->models, ਅਧਾਰ.link)
        rxkb_model_unref(m);
    ਪ੍ਰਤਿਜ੍ਞਾ(list_ਖਾਲੀ_ਥਾਵਾਂ(&ctx->models));

    list_for_each_ਸੁਰੱਖਿਅਤ(l, ltmp, &ctx->layouts, ਅਧਾਰ.link)
        rxkb_layout_unref(l);
    ਪ੍ਰਤਿਜ੍ਞਾ(list_ਖਾਲੀ_ਥਾਵਾਂ(&ctx->layouts));

    list_for_each_ਸੁਰੱਖਿਅਤ(og, ogtmp, &ctx->option_groups, ਅਧਾਰ.link)
        rxkb_option_ਸਮੂਹ_unref(og);
    ਪ੍ਰਤਿਜ੍ਞਾ(list_ਖਾਲੀ_ਥਾਵਾਂ(&ctx->option_groups));

    darray_foreach(ਰਸਤਾ, ctx->includes)
        ਮੁਕਤ(*ਰਸਤਾ);
    darray_free(ctx->includes);

    ਪ੍ਰਤਿਜ੍ਞਾ(darray_ਖਾਲੀ_ਥਾਵਾਂ(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ਗਿਣਤੀ rxkb_log_level);

ਸਥਿਰ ਅੱਖਰ *
rxkb_context_getenv(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *name)
{
    ਜੇ (ctx->use_secure_getenv) {
        ਵਾਪਸ secure_getenv(name);
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ getenv(name);
    }
}


ਖਾਲੀ
rxkb_context_ਸੈੱਟ_log_level(ਬਣਤਰ rxkb_context *ctx,
                           ਗਿਣਤੀ rxkb_log_level level)
{
    ctx->log_level = level;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
log_level_ਮੰਜ਼ਿਲ_ਅਗੇਤਰ(ਗਿਣਤੀ rxkb_log_level level)
{
    ਚੋਣ (level) {
    ਹਾਲਤ RXKB_LOG_LEVEL_DEBUG:
        ਵਾਪਸ "xkbregistry: DEBUG: ";
    ਹਾਲਤ RXKB_LOG_LEVEL_INFO:
        ਵਾਪਸ "xkbregistry: INFO: ";
    ਹਾਲਤ RXKB_LOG_LEVEL_WARNING:
        ਵਾਪਸ "xkbregistry: WARNING: ";
    ਹਾਲਤ RXKB_LOG_LEVEL_ERROR:
        ਵਾਪਸ "xkbregistry: ERROR: ";
    ਹਾਲਤ RXKB_LOG_LEVEL_CRITICAL:
        ਵਾਪਸ "xkbregistry: CRITICAL: ";
    ਮੂਲ:
        ਵਾਪਸ ਨਿਰਾ;
    }
}

ATTR_PRINTF(3, 0) ਸਥਿਰ ਖਾਲੀ
default_log_fn(ਬਣਤਰ rxkb_context *ctx, ਗਿਣਤੀ rxkb_log_level level,
               ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)
{
    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = log_level_ਮੰਜ਼ਿਲ_ਅਗੇਤਰ(level);

    ਜੇ (ਅਗੇਤਰ)
        fprintf(stderr, "%s", ਅਗੇਤਰ);
    vfprintf(stderr, fmt, ਦਲੀਲਾਂ);
}

ਸਥਿਰ ਗਿਣਤੀ rxkb_log_level
log_level(ਅਚਲ ਅੱਖਰ *level) {
    errno = 0;
    ਅੱਖਰ *endptr;
    ਅਚਲ ਲੰਮਾ lvl = strtol(level, &endptr, 10);
    ਜੇ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ਵਾਪਸ (ਗਿਣਤੀ rxkb_log_level)lvl;

    ਜੇ (istreq_ਅਗੇਤਰ("crit", level))
        ਵਾਪਸ RXKB_LOG_LEVEL_CRITICAL;
    ਜੇ (istreq_ਅਗੇਤਰ("err", level))
        ਵਾਪਸ RXKB_LOG_LEVEL_ERROR;
    ਜੇ (istreq_ਅਗੇਤਰ("warn", level))
        ਵਾਪਸ RXKB_LOG_LEVEL_WARNING;
    ਜੇ (istreq_ਅਗੇਤਰ("info", level))
        ਵਾਪਸ RXKB_LOG_LEVEL_INFO;
    ਜੇ (istreq_ਅਗੇਤਰ("debug", level) || istreq_ਅਗੇਤਰ("dbg", level))
        ਵਾਪਸ RXKB_LOG_LEVEL_DEBUG;

    ਵਾਪਸ RXKB_LOG_LEVEL_ERROR;
}

ਬਣਤਰ rxkb_context *
rxkb_context_new(ਗਿਣਤੀ rxkb_context_flags flags)
{
    ਬਣਤਰ rxkb_context *ctx = rxkb_context_create(ਨਿਰਾ);
    ਅਚਲ ਅੱਖਰ *env;

    ਜੇ (!ctx)
        ਵਾਪਸ ਨਿਰਾ;

    ctx->context_state = CONTEXT_NEW;
    ctx->ਲੱਦੋ_extra_ਨਿਯਮਾਂ_files = flags & RXKB_CONTEXT_ਲੱਦੋ_EXOTIC_ਨਿਯਮਾਂ;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ਜੇ (env)
        rxkb_context_ਸੈੱਟ_log_level(ctx, log_level(env));

    ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_ਲੱਦੋ_EXOTIC_ਨਿਯਮਾਂ
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ਜੇ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        ਮੁਕਤ(ctx);
        ਵਾਪਸ ਨਿਰਾ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ਜੇ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_ਰਸਤਾ_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_ਰਸਤਾ,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ਜੜ੍ਹ);
        rxkb_context_unref(ctx);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ ctx;
}

ਖਾਲੀ
rxkb_context_ਸੈੱਟ_log_fn(ਬਣਤਰ rxkb_context *ctx,
                        ਖਾਲੀ (*log_fn)(ਬਣਤਰ rxkb_context *ctx,
                                       ਗਿਣਤੀ rxkb_log_level level,
                                       ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_ਰਸਤਾ_append(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
    ਪੂਰਨ err = 0;

    ਜੇ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "include paths can only be appended to a new context\n");
        ਜਾਓ error;
    }

    ਬਣਤਰ stat stat_buf;
    err = stat(ਰਸਤਾ, &stat_buf);
    ਜੇ (err != 0) {
        err = errno;
        ਜਾਓ error;
    }
    ਜੇ (!S_ISDIR(stat_buf.st_ਬਹੁਲਕ)) {
        err = ENOTDIR;
        ਜਾਓ error;
    }

    ਜੇ (!check_eaccess(ਰਸਤਾ, R_OK | X_OK)) {
        err = EACCES;
        ਜਾਓ error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    ਅੱਖਰ ਨਿਯਮਾਂ[PATH_MAX];
    ਜੇ (!snprintf_ਸੁਰੱਖਿਅਤ(ਨਿਯਮਾਂ, ਆਕਾਰ(ਨਿਯਮਾਂ), "%s/rules/%s.xml",
                       ਰਸਤਾ, DEFAULT_XKB_ਨਿਯਮਾਂ)) {
        log_err(ctx, XKB_ERROR_INVALID_ਰਸਤਾ,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                ਆਕਾਰ(ਨਿਯਮਾਂ), ਰਸਤਾ, DEFAULT_XKB_ਨਿਯਮਾਂ);
        ਜਾਓ error;
    }

    ਅੱਖਰ * ਅਚਲ tmp = strdup(ਰਸਤਾ);
    ਜੇ (!tmp) {
        err = ENOMEM;
        ਜਾਓ error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID, "Include path added: %s\n", tmp);

    ਵਾਪਸ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
             "Include path failed: \"%s\" (%s)\n", ਰਸਤਾ, strerror(err));
    ਵਾਪਸ false;
}

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES

ਸਥਿਰ ਪੂਰਨ
compare_str(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਵਾਪਸ strcmp(*(ਅੱਖਰ **)a, *(ਅੱਖਰ **) b);
}

ਸਥਿਰ ਪੂਰਨ
ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                          darray_string *extensions,
                          darray_ਮਾਪ_t versioned_ਗਿਣਤੀ,
                          ਮਾਪ_t versioned_ਰਸਤਾ_ਲੰਮਾਈ)
{
    ਪੂਰਨ ret = 0;
    ਪੂਰਨ err = ENOMEM;
    DIR *dir = ਨਿਰਾ;

    /* Check extensions parent directory */
    ਬਣਤਰ stat stat_buf;
    err = stat(ਰਸਤਾ, &stat_buf);
    ਜੇ (err != 0) {
        err = errno;
        ਜਾਓ err;
    }
    ਜੇ (!S_ISDIR(stat_buf.st_ਬਹੁਲਕ)) {
        err = ENOTDIR;
        ਜਾਓ err;
    }
    ਜੇ (!check_eaccess(ਰਸਤਾ, R_OK | X_OK)) {
        err = EACCES;
        ਜਾਓ err;
    }

    dir = opendir(ਰਸਤਾ);
    ਜੇ (dir == ਨਿਰਾ) {
        err = EACCES;
        ਜਾਓ err;
    }

    ਬਣਤਰ dirent *ਇੰਦਰਾਜ;
    ਅੱਖਰ ਰਸਤਾ_buf[PATH_MAX] = "";
    versioned_ਰਸਤਾ_ਲੰਮਾਈ++; /* Additional final ‘/’ */
    ਜਦੋਂ ((ਇੰਦਰਾਜ = readdir(dir)) != ਨਿਰਾ) {
        ਅਚਲ ਅੱਖਰ *name = ਇੰਦਰਾਜ->d_name;

        /* Skip special entries */
        ਜੇ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ਜਾਰੀ;

        ਜੇ (!snprintf_ਸੁਰੱਖਿਅਤ(ਰਸਤਾ_buf, ਆਕਾਰ(ਰਸਤਾ_buf), "%s/%s", ਰਸਤਾ, name)) {
            err = ENOMEM;
            ਜਾਓ err;
        }
        ਜੇ (stat(ਰਸਤਾ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_ਬਹੁਲਕ))
            ਜਾਰੀ;

        /* Skip if the corresponding versioned directory is already included */
        ਲਈ (darray_ਮਾਪ_t i = 0; i < versioned_ਗਿਣਤੀ; i++) {
            ਅਚਲ ਅੱਖਰ * ਅਚਲ prev_name = darray_ਵਸਤੂ(*extensions, i)
                                         + versioned_ਰਸਤਾ_ਲੰਮਾਈ;
            ਜੇ (strcmp(name, prev_name) == 0)
                ਜਾਓ ਅਗਲਾ;
        }

        ਅੱਖਰ *ext_ਰਸਤਾ = strdup_ਸੁਰੱਖਿਅਤ(ਰਸਤਾ_buf);
        ਜੇ (!ext_ਰਸਤਾ) {
            err = ENOMEM;
            ਜਾਓ err;
        }

        darray_append(*extensions, ext_ਰਸਤਾ);
ਅਗਲਾ:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ਜੇ (darray_ਮਾਪ(*extensions) > versioned_ਗਿਣਤੀ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ਚੀਜ਼ਾਂ(*extensions) + versioned_ਗਿਣਤੀ,
              darray_ਮਾਪ(*extensions) - versioned_ਗਿਣਤੀ,
              ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(*extensions)), &compare_str);
        ਅੱਖਰ **ext_ਰਸਤਾ;
        darray_foreach_from(ext_ਰਸਤਾ, *extensions, versioned_ਗਿਣਤੀ) {
            ret |= rxkb_context_include_ਰਸਤਾ_append(ctx, *ext_ਰਸਤਾ);
        }
    }

    ਵਾਪਸ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", ਰਸਤਾ, strerror(err));
    ਜੇ (dir)
        closedir(dir);

    ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

bool
rxkb_context_include_ਰਸਤਾ_append_default(ਬਣਤਰ rxkb_context *ctx)
{
    ਅੱਖਰ user_ਰਸਤਾ[PATH_MAX];
    bool ret = false;

    ਜੇ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "include paths can only be appended to a new context\n");
        ਵਾਪਸ false;
    }

    ਅਚਲ ਅੱਖਰ * ਅਚਲ home = rxkb_context_getenv(ctx, "HOME");
    ਅਚਲ ਅੱਖਰ * ਅਚਲ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ਜੇ (xdg != ਨਿਰਾ) {
        ਜੇ (snprintf_ਸੁਰੱਖਿਅਤ(user_ਰਸਤਾ, ਆਕਾਰ(user_ਰਸਤਾ), "%s/xkb", xdg))
            ret |= rxkb_context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
    } ਨਹੀਂਤਾਂ ਜੇ (home != ਨਿਰਾ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ਜੇ (snprintf_ਸੁਰੱਖਿਅਤ(user_ਰਸਤਾ, ਆਕਾਰ(user_ਰਸਤਾ), "%s/.config/xkb", home))
            ret |= rxkb_context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
    }

    ਜੇ (home != ਨਿਰਾ) {
        ਜੇ (snprintf_ਸੁਰੱਖਿਅਤ(user_ਰਸਤਾ, ਆਕਾਰ(user_ਰਸਤਾ), "%s/.xkb", home))
            ret |= rxkb_context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
    }

    ਅਚਲ ਅੱਖਰ * ਅਚਲ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_ਰਸਤਾ_append(
        ctx, ((extra != ਨਿਰਾ) ? extra : DFLT_XKB_CONFIG_EXTRA_ਰਸਤਾ)
    );

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ਅਚਲ ਅੱਖਰ *extensions_ਰਸਤਾ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ਰਸਤਾ
    ਜੇ (extensions_ਰਸਤਾ == ਨਿਰਾ)
        extensions_ਰਸਤਾ = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ਰਸਤਾ;
#ਜੇ_ਅੰਤ
    ਮਾਪ_t versioned_ਰਸਤਾ_ਲੰਮਾਈ = 0;
    ਜੇ (extensions_ਰਸਤਾ != ਨਿਰਾ) {
        /* Add direct subdirectories */
        ret |= ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ctx, extensions_ਰਸਤਾ,
                                         &extensions, 0, 0);
        versioned_ਰਸਤਾ_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(extensions_ਰਸਤਾ);
    }

    /* Unversioned extensions directory */
    extensions_ਰਸਤਾ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ਰਸਤਾ
    ਜੇ (extensions_ਰਸਤਾ == ਨਿਰਾ)
        extensions_ਰਸਤਾ = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ਰਸਤਾ;
#ਜੇ_ਅੰਤ
    ਜੇ (extensions_ਰਸਤਾ != ਨਿਰਾ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ctx, extensions_ਰਸਤਾ, &extensions,
                                         darray_ਮਾਪ(extensions),
                                         versioned_ਰਸਤਾ_ਲੰਮਾਈ);
    }

    ਅੱਖਰ **ext_ਰਸਤਾ;
    darray_foreach(ext_ਰਸਤਾ, extensions) {
        ਮੁਕਤ(*ext_ਰਸਤਾ);
    }
    darray_free(extensions);
#ਜੇ_ਅੰਤ

    /* Canonical XKB root */
    ਅਚਲ ਅੱਖਰ * ਅਚਲ ਜੜ੍ਹ = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ਅਚਲ bool has_ਜੜ੍ਹ = rxkb_context_include_ਰਸਤਾ_append(
        ctx, ((ਜੜ੍ਹ != ਨਿਰਾ) ? ਜੜ੍ਹ: DFLT_XKB_CONFIG_ਜੜ੍ਹ)
    );
    ret |= has_ਜੜ੍ਹ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ਜੇ (!has_ਜੜ੍ਹ && (ਜੜ੍ਹ == ਨਿਰਾ || ਜੜ੍ਹ[0] != '\0')) {
        log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ਜੜ੍ਹ,
                 ((ਜੜ੍ਹ == ਨਿਰਾ) ? DFLT_XKB_CONFIG_ਜੜ੍ਹ : ਜੜ੍ਹ));
        ret |= rxkb_context_include_ਰਸਤਾ_append(ctx, DFLT_XKB_LEGACY_ਜੜ੍ਹ);
    }

    ਵਾਪਸ ret;
}

bool
rxkb_context_parse_default_ruleset(ਬਣਤਰ rxkb_context *ctx)
{
    ਵਾਪਸ rxkb_context_parse(ctx, DEFAULT_XKB_ਨਿਯਮਾਂ);
}

bool
rxkb_context_parse(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *ruleset)
{
    ਅੱਖਰ **ਰਸਤਾ;
    bool success = false;

    ਜੇ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "parse must only be called on a new context\n");
        ਵਾਪਸ false;
    }

    darray_foreach_ਉਲਟ(ਰਸਤਾ, ctx->includes) {
        ਅੱਖਰ ਨਿਯਮਾਂ[PATH_MAX];

        ਜੇ (snprintf_ਸੁਰੱਖਿਅਤ(ਨਿਯਮਾਂ, ਆਕਾਰ(ਨਿਯਮਾਂ), "%s/rules/%s.xml",
                           *ਰਸਤਾ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", ਨਿਯਮਾਂ);
            ਜੇ (parse(ctx, ਨਿਯਮਾਂ, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ਜੇ (ctx->ਲੱਦੋ_extra_ਨਿਯਮਾਂ_files &&
            snprintf_ਸੁਰੱਖਿਅਤ(ਨਿਯਮਾਂ, ਆਕਾਰ(ਨਿਯਮਾਂ), "%s/rules/%s.extras.xml",
                          *ਰਸਤਾ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", ਨਿਯਮਾਂ);
            ਜੇ (parse(ctx, ਨਿਯਮਾਂ, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ਵਾਪਸ success;
}


ਖਾਲੀ
rxkb_context_ਸੈੱਟ_user_ਅੰਕੜੇ(ਬਣਤਰ rxkb_context *ctx, ਖਾਲੀ *userdata)
{
    ctx->userdata = userdata;
}

ਖਾਲੀ *
rxkb_context_get_user_ਅੰਕੜੇ(ਬਣਤਰ rxkb_context *ctx)
{
    ਵਾਪਸ ctx->userdata;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ਗੰਢ(xml_ਗੰਢ *ਗੰਢ, ਅਚਲ ਅੱਖਰ *name)
{
    ਵਾਪਸ ਗੰਢ->type == XML_ELEMENT_ਗੰਢ &&
        xmlStrEqual(ਗੰਢ->name, (ਅਚਲ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ਸਥਿਰ ਅੱਖਰ *
ਕੱਢੋ_ਪਾਠ(xml_ਗੰਢ *ਗੰਢ)
{
    xml_ਗੰਢ *n;

    ਲਈ (n = ਗੰਢ->children; n; n = n->ਅਗਲਾ) {
        ਜੇ (n->type == XML_ਪਾਠ_ਗੰਢ)
            ਵਾਪਸ (ਅੱਖਰ *)xmlStrdup(n->content);
    }
    ਵਾਪਸ ਨਿਰਾ;
}

/* Data from “configItem” node */
ਬਣਤਰ config_ਵਸਤੂ {
    ਅੱਖਰ *name;
    ਅੱਖਰ *description;
    ਅੱਖਰ *brief;
    ਅੱਖਰ *vendor;
    ਗਿਣਤੀ rxkb_popularity popularity;
    bool layout_specific;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਵਸਤੂ_new(popularity_) { \
    .name = ਨਿਰਾ, \
    .description = ਨਿਰਾ, \
    .brief = ਨਿਰਾ, \
    .vendor = ਨਿਰਾ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ਸਥਿਰ ਖਾਲੀ
config_ਵਸਤੂ_free(ਬਣਤਰ config_ਵਸਤੂ *config) {
    ਮੁਕਤ(config->name);
    ਮੁਕਤ(config->description);
    ਮੁਕਤ(config->brief);
    ਮੁਕਤ(config->vendor);
}

ਸਥਿਰ bool
parse_config_ਵਸਤੂ(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *ਮਾਪਾ,
                  ਬਣਤਰ config_ਵਸਤੂ *config)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
    xml_ਗੰਢ *ci = ਨਿਰਾ;

    ਲਈ (ci = ਮਾਪਾ->children; ci; ci = ci->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ਅਚਲ xmlChar*)"popularity");
            ਜੇ (raw_popularity) {
                ਜੇ (xmlStrEqual(raw_popularity, (ਅਚਲ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ਨਹੀਂਤਾਂ ਜੇ (xmlStrEqual(raw_popularity, (ਅਚਲ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ਨਹੀਂਤਾਂ
                    log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->ਸਤਰ, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ਅਚਲ xmlChar*)"layout-specific");
            ਜੇ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ਅਚਲ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            ਲਈ (ਗੰਢ = ci->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
                ਜੇ (is_ਗੰਢ(ਗੰਢ, "name"))
                    config->name = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
                ਨਹੀਂਤਾਂ ਜੇ (is_ਗੰਢ(ਗੰਢ, "description"))
                    config->description = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
                ਨਹੀਂਤਾਂ ਜੇ (is_ਗੰਢ(ਗੰਢ, "shortDescription"))
                    config->brief = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
                ਨਹੀਂਤਾਂ ਜੇ (is_ਗੰਢ(ਗੰਢ, "vendor"))
                    config->vendor = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ਜੇ (!config->name || !ਲੜੀ_ਲੰਬਾਈ(config->name))  {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->ਸਤਰ);
                config_ਵਸਤੂ_free(config);
                ਵਾਪਸ false;
            }

            ਵਾਪਸ true; /* only one configItem allowed in the dtd */
        }
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
parse_model(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *model,
            ਗਿਣਤੀ rxkb_popularity popularity)
{
    ਬਣਤਰ config_ਵਸਤੂ config = config_ਵਸਤੂ_new(popularity);

    ਜੇ (parse_config_ਵਸਤੂ(ctx, model, &config)) {
        ਬਣਤਰ rxkb_model *m;

        list_for_each(m, &ctx->models, ਅਧਾਰ.link) {
            ਜੇ (streq(m->name, config.name)) {
                config_ਵਸਤੂ_free(&config);
                ਵਾਪਸ;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->ਅਧਾਰ);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->ਅਧਾਰ.link);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_model_list(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *model_list,
                 ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    ਲਈ (ਗੰਢ = model_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "model"))
            parse_model(ctx, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_language_list(xml_ਗੰਢ *language_list, ਬਣਤਰ rxkb_layout *layout)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
    ਬਣਤਰ rxkb_iso639_ਕੋਡ *ਕੋਡ;

    ਲਈ (ਗੰਢ = language_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "iso639Id")) {
            ਅੱਖਰ *str = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
            ਬਣਤਰ rxkb_object *ਮਾਪਾ;

            ਜੇ (!str || ਲੜੀ_ਲੰਬਾਈ(str) != 3) {
                ਮੁਕਤ(str);
                ਜਾਰੀ;
            }

            ਮਾਪਾ = &layout->ਅਧਾਰ;
            ਕੋਡ = rxkb_iso639_ਕੋਡ_create(ਮਾਪਾ);
            ਕੋਡ->ਕੋਡ = str;
            list_append(&layout->iso639s, &ਕੋਡ->ਅਧਾਰ.link);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_country_list(xml_ਗੰਢ *country_list, ਬਣਤਰ rxkb_layout *layout)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
    ਬਣਤਰ rxkb_iso3166_ਕੋਡ *ਕੋਡ;

    ਲਈ (ਗੰਢ = country_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "iso3166Id")) {
            ਅੱਖਰ *str = ਕੱਢੋ_ਪਾਠ(ਗੰਢ);
            ਬਣਤਰ rxkb_object *ਮਾਪਾ;

            ਜੇ (!str || ਲੜੀ_ਲੰਬਾਈ(str) != 2) {
                ਮੁਕਤ(str);
                ਜਾਰੀ;
            }

            ਮਾਪਾ = &layout->ਅਧਾਰ;
            ਕੋਡ = rxkb_iso3166_ਕੋਡ_create(ਮਾਪਾ);
            ਕੋਡ->ਕੋਡ = str;
            list_append(&layout->iso3166s, &ਕੋਡ->ਅਧਾਰ.link);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_variant(ਬਣਤਰ rxkb_context *ctx, ਬਣਤਰ rxkb_layout *l,
              xml_ਗੰਢ *variant, ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ci;
    ਬਣਤਰ config_ਵਸਤੂ config = config_ਵਸਤੂ_new(popularity);

    ਜੇ (parse_config_ਵਸਤੂ(ctx, variant, &config)) {
        ਬਣਤਰ rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, ਅਧਾਰ.link) {
            ਜੇ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ਰੋਕੋ;
            }
        }

        ਜੇ (!exists) {
            v = rxkb_layout_create(&ctx->ਅਧਾਰ);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ਨਿਰਾ ? strdup_ਸੁਰੱਖਿਅਤ(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->ਅਧਾਰ.link);

            ਲਈ (ci = variant->children; ci; ci = ci->ਅਗਲਾ) {
                xml_ਗੰਢ *ਗੰਢ;

                ਜੇ (!is_ਗੰਢ(ci, "configItem"))
                    ਜਾਰੀ;

                bool ਮਿਲਿਆ_language_list = false;
                bool ਮਿਲਿਆ_country_list = false;
                ਲਈ (ਗੰਢ = ci->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
                    ਜੇ (is_ਗੰਢ(ਗੰਢ, "languageList")) {
                        parse_language_list(ਗੰਢ, v);
                        ਮਿਲਿਆ_language_list = true;
                    }
                    ਜੇ (is_ਗੰਢ(ਗੰਢ, "countryList")) {
                        parse_country_list(ਗੰਢ, v);
                        ਮਿਲਿਆ_country_list = true;
                    }
                }
                ਜੇ (!ਮਿਲਿਆ_language_list) {
                    // inherit from parent layout
                    ਬਣਤਰ rxkb_iso639_ਕੋਡ* x;
                    list_for_each(x, &l->iso639s, ਅਧਾਰ.link) {
                        ਬਣਤਰ rxkb_iso639_ਕੋਡ* ਕੋਡ = rxkb_iso639_ਕੋਡ_create(&v->ਅਧਾਰ);
                        ਕੋਡ->ਕੋਡ = strdup(x->ਕੋਡ);
                        list_append(&v->iso639s, &ਕੋਡ->ਅਧਾਰ.link);
                    }
                }
                ਜੇ (!ਮਿਲਿਆ_country_list) {
                    // inherit from parent layout
                    ਬਣਤਰ rxkb_iso3166_ਕੋਡ* x;
                    list_for_each(x, &l->iso3166s, ਅਧਾਰ.link) {
                        ਬਣਤਰ rxkb_iso3166_ਕੋਡ* ਕੋਡ = rxkb_iso3166_ਕੋਡ_create(&v->ਅਧਾਰ);
                        ਕੋਡ->ਕੋਡ = strdup(x->ਕੋਡ);
                        list_append(&v->iso3166s, &ਕੋਡ->ਅਧਾਰ.link);
                    }
                }
            }
        } ਨਹੀਂਤਾਂ {
            config_ਵਸਤੂ_free(&config);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_variant_list(ਬਣਤਰ rxkb_context *ctx, ਬਣਤਰ rxkb_layout *l,
                   xml_ਗੰਢ *variant_list, ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    ਲਈ (ਗੰਢ = variant_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "variant"))
            parse_variant(ctx, l, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_layout(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *layout,
             ਗਿਣਤੀ rxkb_popularity popularity)
{
    ਬਣਤਰ config_ਵਸਤੂ config = config_ਵਸਤੂ_new(popularity);
    ਬਣਤਰ rxkb_layout *l;
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
    bool exists = false;

    ਜੇ (!parse_config_ਵਸਤੂ(ctx, layout, &config))
        ਵਾਪਸ;

    list_for_each(l, &ctx->layouts, ਅਧਾਰ.link) {
        ਜੇ (streq(l->name, config.name) && l->variant == ਨਿਰਾ) {
            exists = true;
            ਰੋਕੋ;
        }
    }

    ਜੇ (!exists) {
        l = rxkb_layout_create(&ctx->ਅਧਾਰ);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ਨਿਰਾ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->ਅਧਾਰ.link);
    } ਨਹੀਂਤਾਂ {
        config_ਵਸਤੂ_free(&config);
    }

    ਲਈ (ਗੰਢ = layout->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "variantList")) {
            parse_variant_list(ctx, l, ਗੰਢ, popularity);
        }
        ਜੇ (!exists && is_ਗੰਢ(ਗੰਢ, "configItem")) {
            xml_ਗੰਢ *ll;
            ਲਈ (ll = ਗੰਢ->children; ll; ll = ll->ਅਗਲਾ) {
                ਜੇ (is_ਗੰਢ(ll, "languageList"))
                    parse_language_list(ll, l);
                ਜੇ (is_ਗੰਢ(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_layout_list(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *layout_list,
                  ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    ਲਈ (ਗੰਢ = layout_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "layout"))
            parse_layout(ctx, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_option(ਬਣਤਰ rxkb_context *ctx, ਬਣਤਰ rxkb_option_ਸਮੂਹ *ਸਮੂਹ,
             xml_ਗੰਢ *option, ਗਿਣਤੀ rxkb_popularity popularity)
{
    ਬਣਤਰ config_ਵਸਤੂ config = config_ਵਸਤੂ_new(popularity);

    ਜੇ (parse_config_ਵਸਤੂ(ctx, option, &config)) {
        ਬਣਤਰ rxkb_option *o;

        list_for_each(o, &ਸਮੂਹ->options, ਅਧਾਰ.link) {
            ਜੇ (streq(o->name, config.name)) {
                config_ਵਸਤੂ_free(&config);
                ਵਾਪਸ;
            }
        }

        o = rxkb_option_create(&ਸਮੂਹ->ਅਧਾਰ);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&ਸਮੂਹ->options, &o->ਅਧਾਰ.link);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_ਸਮੂਹ(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *ਸਮੂਹ,
            ਗਿਣਤੀ rxkb_popularity popularity)
{
    ਬਣਤਰ config_ਵਸਤੂ config = config_ਵਸਤੂ_new(popularity);
    ਬਣਤਰ rxkb_option_ਸਮੂਹ *g;
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;
    xmlChar *ਗੁਣਜ;
    bool exists = false;

    ਜੇ (!parse_config_ਵਸਤੂ(ctx, ਸਮੂਹ, &config))
        ਵਾਪਸ;

    list_for_each(g, &ctx->option_groups, ਅਧਾਰ.link) {
        ਜੇ (streq(g->name, config.name)) {
            exists = true;
            ਰੋਕੋ;
        }
    }

    ਜੇ (!exists) {
        g = rxkb_option_ਸਮੂਹ_create(&ctx->ਅਧਾਰ);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        ਗੁਣਜ = xmlGetProp(ਸਮੂਹ, (ਅਚਲ xmlChar*)"allowMultipleSelection");
        ਜੇ (ਗੁਣਜ && xmlStrEqual(ਗੁਣਜ, (ਅਚਲ xmlChar*)"true"))
            g->allow_ਗੁਣਜ = true;
        xmlFree(ਗੁਣਜ);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->ਅਧਾਰ.link);
    } ਨਹੀਂਤਾਂ {
        config_ਵਸਤੂ_free(&config);
    }

    ਲਈ (ਗੰਢ = ਸਮੂਹ->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "option"))
            parse_option(ctx, g, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_option_list(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *option_list,
                  ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    ਲਈ (ਗੰਢ = option_list->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "group"))
            parse_ਸਮੂਹ(ctx, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
parse_ਨਿਯਮਾਂ_xml(ਬਣਤਰ rxkb_context *ctx, xml_ਗੰਢ *ਜੜ੍ਹ,
                ਗਿਣਤੀ rxkb_popularity popularity)
{
    xml_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    ਲਈ (ਗੰਢ = ਜੜ੍ਹ->children; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ) {
        ਜੇ (is_ਗੰਢ(ਗੰਢ, "modelList"))
            parse_model_list(ctx, ਗੰਢ, popularity);
        ਨਹੀਂਤਾਂ ਜੇ (is_ਗੰਢ(ਗੰਢ, "layoutList"))
            parse_layout_list(ctx, ਗੰਢ, popularity);
        ਨਹੀਂਤਾਂ ਜੇ (is_ਗੰਢ(ਗੰਢ, "optionList"))
            parse_option_list(ctx, ਗੰਢ, popularity);
    }
}

ਸਥਿਰ ਖਾਲੀ
ATTR_PRINTF(2, 0)
xml_error_func(ਖਾਲੀ *ctx, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
    ਸਥਿਰ ਅੱਖਰ buf[PATH_MAX];
    ਸਥਿਰ ਮਾਪ_t slen = 0;
    va_list ਦਲੀਲਾਂ;
    ਪੂਰਨ rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, ਸੁਨੇਹਾ);
    rc = vsnprintf(&buf[slen], ਆਕਾਰ(buf) - slen, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    /* This shouldn't really happen */
    ਜੇ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, ਆਕਾਰ(buf));
        ਵਾਪਸ;
    }

    slen += (ਮਾਪ_t)rc;
    ਜੇ (slen >= ਆਕਾਰ(buf)) {
        /* truncated, let's flush this */
        buf[ਆਕਾਰ(buf) - 1] = '\n';
        slen = ਆਕਾਰ(buf);
    }

    /* We're assuming here that the last character is \n. */
    ਜੇ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID, "%s", buf);
        memset(buf, 0, ਆਕਾਰ(buf));
        slen = 0;
    }
}

#ਜੇ HAVE_XML_CTXT_ਸੈੱਟ_ERRORHANDLER
ਸਥਿਰ ਖਾਲੀ
xml_structured_error_func(ਖਾਲੀ *user_ਅੰਕੜੇ_2, ਅਚਲ xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_ਅੰਕੜੇ_2);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XML_PARSE_NO_XXE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _XML_OPTIONS (XML_PARSE_NONET)
#ਜੇ_ਅੰਤ

ਸਥਿਰ bool
validate(ਬਣਤਰ rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ਅਚਲ ਅੱਖਰ dtdstr[] =
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

#ਜੇ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ਜੇ (!xmlCtxt)
        ਵਾਪਸ false;
    static_assert(HAVE_XML_CTXT_ਸੈੱਟ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_ਸੈੱਟ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_ਸੈੱਟ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ਨਿਵੇਸ਼_Ptr pinput =
        xml_New_ਨਿਵੇਸ਼_From_ਸਮ੍ਰਿਤੀ(ਨਿਰਾ, dtdstr, ARRAY_ਮਾਪ(dtdstr) - 1,
                              XML_ਨਿਵੇਸ਼_BUF_STATIC);
    ਜੇ (!pinput)
        ਜਾਓ dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ਨਿਰਾ, ਨਿਰਾ);
#ਨਹੀਂਤਾਂ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ਨਿਵੇਸ਼_ਬਫਰ_Ptr buf =
        xml_Parser_ਨਿਵੇਸ਼_ਬਫਰ_Create_Mem(dtdstr, ARRAY_ਮਾਪ(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ਜੇ (!buf)
        ਜਾਓ dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ਨਿਰਾ, buf, XML_CHAR_ENCODING_UTF8);
#ਜੇ_ਅੰਤ

    ਜੇ (!dtd) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID, "Failed to load DTD\n");
        ਜਾਓ dtd_error;
    }

#ਜੇ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ਨਹੀਂਤਾਂ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ਜੇ (!dtdvalid) {
        success = false;
        ਜਾਓ ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_ਅੰਕੜੇ_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#ਜੇ_ਅੰਤ
    xmlFreeDtd(dtd);

dtd_error:
#ਜੇ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ success;
}

ਸਥਿਰ bool
parse(ਬਣਤਰ rxkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
      ਗਿਣਤੀ rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ਨਿਰਾ;
    xml_ਗੰਢ *ਜੜ੍ਹ = ਨਿਰਾ;

    ਜੇ (!check_eaccess(ਰਸਤਾ, R_OK))
        ਵਾਪਸ false;

    LIBXML_TEST_ਸੰਸਕਰਣ

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ਜੇ (!xmlCtxt)
        ਵਾਪਸ false;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XML_PARSE_NO_XXE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _XML_OPTIONS (XML_PARSE_NONET)
#ਜੇ_ਅੰਤ
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ਜੇ HAVE_XML_CTXT_ਸੈੱਟ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_ਸੈੱਟ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#ਜੇ_ਅੰਤ
#ਜੇ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_ਸੈੱਟ_Generic_Error_Func(ctx, xml_error_func);
#ਜੇ_ਅੰਤ

    doc = xmlCtxtReadFile(xmlCtxt, ਰਸਤਾ, ਨਿਰਾ, 0);
    ਜੇ (!doc)
        ਜਾਓ parse_error;

    ਜੇ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "XML error: failed to validate document at %s\n", ਰਸਤਾ);
        ਜਾਓ validate_error;
    }

    ਜੜ੍ਹ = xml_Doc_Get_ਜੜ੍ਹ_Element(doc);
    parse_ਨਿਯਮਾਂ_xml(ctx, ਜੜ੍ਹ, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ਜੇ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xml_ਸੈੱਟ_Generic_Error_Func(ਨਿਰਾ, ਨਿਰਾ);
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_XML_CTXT_ਸੈੱਟ_ERRORHANDLER
    xml_Ctxt_ਸੈੱਟ_Error_Handler(xmlCtxt, ਨਿਰਾ, ਨਿਰਾ);
#ਜੇ_ਅੰਤ
    xmlFreeParserCtxt(xmlCtxt);

    ਵਾਪਸ success;
}
