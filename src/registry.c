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

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ಅನ್ತರ್ಭಾವ <limits.h>
    #ಆದರೆ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ಅನ್ತರ್ಭಾವ <dirent.h>
        #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbregistry.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-list.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"

ರಚನೆ rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
ರಚನೆ rxkb_object {
    ರಚನೆ rxkb_object *ಪೋಷಕ;
    uint32_t refcount;
    ರಚನೆ list link;
};

ರಚನೆ rxkb_iso639_ಸಂಕೇತ {
    ರಚನೆ rxkb_object ತಳ;
    ಅಕ್ಷರ *ಸಂಕೇತ_2;
};

ರಚನೆ rxkb_iso3166_ಸಂಕೇತ {
    ರಚನೆ rxkb_object ತಳ;
    ಅಕ್ಷರ *ಸಂಕೇತ_2;
};

ಎಣಿಕೆ context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

ರಚನೆ rxkb_context {
    ರಚನೆ rxkb_object ತಳ;
    ಎಣಿಕೆ context_state context_state;

    bool ತುಂಬು_extra_ನಿಯಮಗಳು_files;
    bool use_secure_getenv;

    ರಚನೆ list models;         /* list of struct rxkb_models */
    ರಚನೆ list layouts;        /* list of struct rxkb_layouts */
    ರಚನೆ list option_groups;  /* list of struct rxkb_option_group */

    darray(ಅಕ್ಷರ *) includes;


    ATTR_PRINTF(3, 0) ಶೂನ್ಯ (*log_fn)(ರಚನೆ rxkb_context *ctx,
                                     ಎಣಿಕೆ rxkb_log_level level,
                                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು);
    ಎಣಿಕೆ rxkb_log_level log_level;

    ಶೂನ್ಯ *userdata;
};

ರಚನೆ rxkb_model {
    ರಚನೆ rxkb_object ತಳ;

    ಅಕ್ಷರ *name;
    ಅಕ್ಷರ *vendor;
    ಅಕ್ಷರ *description;
    ಎಣಿಕೆ rxkb_popularity popularity;
};

ರಚನೆ rxkb_layout {
    ರಚನೆ rxkb_object ತಳ;

    ಅಕ್ಷರ *name;
    ಅಕ್ಷರ *brief;
    ಅಕ್ಷರ *description;
    ಅಕ್ಷರ *variant;
    ಎಣಿಕೆ rxkb_popularity popularity;

    ರಚನೆ list iso639s;  /* list of struct rxkb_iso639_code */
    ರಚನೆ list iso3166s; /* list of struct rxkb_iso3166_code */
};

ರಚನೆ rxkb_option_ಗುಂಪು {
    ರಚನೆ rxkb_object ತಳ;

    bool allow_ಅಪವರ್ತ್ಯ;
    ರಚನೆ list options; /* list of struct rxkb_options */
    ಅಕ್ಷರ *name;
    ಅಕ್ಷರ *description;
    ಎಣಿಕೆ rxkb_popularity popularity;
};

ರಚನೆ rxkb_option {
    ರಚನೆ rxkb_object ತಳ;

    ಅಕ್ಷರ *name;
    ಅಕ್ಷರ *brief;
    ಅಕ್ಷರ *description;
    ಎಣಿಕೆ rxkb_popularity popularity;
    bool layout_specific;
};

ಸ್ಥಿರ bool
parse(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
      ಎಣಿಕೆ rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_log(ರಚನೆ rxkb_context *ctx, ಎಣಿಕೆ rxkb_log_level level,
         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list ವಾದಗಳು;

    ಆದರೆ (ctx->log_level < level)
        ಹಿಂತಿರುಗಿ;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    ctx->log_fn(ctx, level, fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು rxkb_log_with_ಸಂಕೇತ(ctx, level, ಸಂದೇಶ_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_ಸಂದೇಶ_ID(ಸಂದೇಶ_id, fmt), ##__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_info(ctx, id, ...) \
    rxkb_log_with_ಸಂಕೇತ((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_warn(ctx, id, ...) \
    rxkb_log_with_ಸಂಕೇತ((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_err(ctx, id, ...) \
    rxkb_log_with_ಸಂಕೇತ((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು log_wsgo(ctx, id, ...) \
    rxkb_log_with_ಸಂಕೇತ((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_REF_UNREF_FOR_TYPE(type_) \
ರಚನೆ type_ * type_##_ref(ರಚನೆ type_ *object) { \
    rxkb_object_ref(&object->ತಳ); \
    ಹಿಂತಿರುಗಿ object; \
} \
ರಚನೆ type_ * type_##_unref(ರಚನೆ type_ *object) { \
    ಆದರೆ (!object) ಹಿಂತಿರುಗಿ ಖಾಲಿ; \
    ಪ್ರತಿಜ್ಞಾ(object->ತಳ.refcount >= 1); \
    ಆದರೆ (--object->ತಳ.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->ತಳ.link);\
        ಬಿಡುಗಡೆ(object); \
    } \
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;\
}

#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_CREATE_FOR_TYPE(type_) \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ type_ * type_##_create(ರಚನೆ rxkb_object *ಪೋಷಕ) { \
    ರಚನೆ type_ *t = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ *t); \
    ಆದರೆ (t) \
        rxkb_object_init(&t->ತಳ, ಪೋಷಕ); \
    ಹಿಂತಿರುಗಿ t; \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_TYPED_GETTER_FOR_TYPE(type_, ಕ್ಷೇತ್ರ, rtype_) \
rtype_ type_##_get_##field_(ರಚನೆ type_ *object) { \
    ಹಿಂತಿರುಗಿ object->ಕ್ಷೇತ್ರ; \
}

#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_GETTER_FOR_TYPE(type_, ಕ್ಷೇತ್ರ) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, ಕ್ಷೇತ್ರ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)

#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_ಮೊದಲ_ಮುಂದಿನ_FOR_TYPE(type_, ಪೋಷಕ_type, ಪೋಷಕ_ಕ್ಷೇತ್ರ) \
ರಚನೆ type_ * type_##_first(ರಚನೆ ಪೋಷಕ_type *ಪೋಷಕ) { \
    ರಚನೆ type_ *o = ಖಾಲಿ; \
    ಆದರೆ (!list_ಖಾಲಿಗಳು(&ಪೋಷಕ->ಪೋಷಕ_ಕ್ಷೇತ್ರ)) \
        o = list_ಮೊದಲ_ನಮೂದು(&ಪೋಷಕ->ಪೋಷಕ_ಕ್ಷೇತ್ರ, o, ತಳ.link); \
    ಹಿಂತಿರುಗಿ o; \
} \
ರಚನೆ type_ * \
type_##_next(ರಚನೆ type_ *o) \
{ \
    ರಚನೆ ಪೋಷಕ_type *ಪೋಷಕ; \
    ರಚನೆ type_ *ಮುಂದಿನ; \
    ಪೋಷಕ = container_ದ(o->ತಳ.ಪೋಷಕ, ರಚನೆ ಪೋಷಕ_type, ತಳ); \
    ಮುಂದಿನ = list_ಮೊದಲ_ನಮೂದು(&o->ತಳ.link, o, ತಳ.link); \
    ಆದರೆ (list_is_ಕೊನೆಯ(&ಪೋಷಕ->ಪೋಷಕ_ಕ್ಷೇತ್ರ, &o->ತಳ.link)) \
        ಹಿಂತಿರುಗಿ ಖಾಲಿ; \
    ಹಿಂತಿರುಗಿ ಮುಂದಿನ; \
}

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_object_init(ರಚನೆ rxkb_object *object, ರಚನೆ rxkb_object *ಪೋಷಕ)
{
    object->refcount = 1;
    object->ಪೋಷಕ = ಪೋಷಕ;
    list_init(&object->link);
}

ಸ್ಥಿರ ಶೂನ್ಯ *
rxkb_object_ref(ರಚನೆ rxkb_object *object)
{
    ಪ್ರತಿಜ್ಞಾ(object->refcount >= 1);
    ++object->refcount;
    ಹಿಂತಿರುಗಿ object;
}

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_iso639_ಸಂಕೇತ_destroy(ರಚನೆ rxkb_iso639_ಸಂಕೇತ *ಸಂಕೇತ_2)
{
    ಬಿಡುಗಡೆ(ಸಂಕೇತ_2->ಸಂಕೇತ_2);
}

ರಚನೆ rxkb_iso639_ಸಂಕೇತ *
rxkb_layout_get_iso639_ಮೊದಲ(ರಚನೆ rxkb_layout *layout)
{
    ರಚನೆ rxkb_iso639_ಸಂಕೇತ *ಸಂಕೇತ_2 = ಖಾಲಿ;

    ಆದರೆ (!list_ಖಾಲಿಗಳು(&layout->iso639s))
        ಸಂಕೇತ_2 = list_ಮೊದಲ_ನಮೂದು(&layout->iso639s, ಸಂಕೇತ_2, ತಳ.link);

    ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}

ರಚನೆ rxkb_iso639_ಸಂಕೇತ *
rxkb_iso639_ಸಂಕೇತ_ಮುಂದಿನ(ರಚನೆ rxkb_iso639_ಸಂಕೇತ *ಸಂಕೇತ_2)
{
    ರಚನೆ rxkb_iso639_ಸಂಕೇತ *ಮುಂದಿನ = ಖಾಲಿ;
    ರಚನೆ rxkb_layout *layout;

    layout = container_ದ(ಸಂಕೇತ_2->ತಳ.ಪೋಷಕ, ರಚನೆ rxkb_layout, ತಳ);

    ಆದರೆ (list_is_ಕೊನೆಯ(&layout->iso639s, &ಸಂಕೇತ_2->ತಳ.link))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಮುಂದಿನ = list_ಮೊದಲ_ನಮೂದು(&ಸಂಕೇತ_2->ತಳ.link, ಸಂಕೇತ_2, ತಳ.link);

    ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_ಸಂಕೇತ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_ಸಂಕೇತ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_ಸಂಕೇತ, ಸಂಕೇತ_2);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_iso3166_ಸಂಕೇತ_destroy(ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *ಸಂಕೇತ_2)
{
    ಬಿಡುಗಡೆ(ಸಂಕೇತ_2->ಸಂಕೇತ_2);
}

ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *
rxkb_layout_get_iso3166_ಮೊದಲ(ರಚನೆ rxkb_layout *layout)
{
    ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *ಸಂಕೇತ_2 = ಖಾಲಿ;

    ಆದರೆ (!list_ಖಾಲಿಗಳು(&layout->iso3166s))
        ಸಂಕೇತ_2 = list_ಮೊದಲ_ನಮೂದು(&layout->iso3166s, ಸಂಕೇತ_2, ತಳ.link);

    ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;
}

ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *
rxkb_iso3166_ಸಂಕೇತ_ಮುಂದಿನ(ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *ಸಂಕೇತ_2)
{
    ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *ಮುಂದಿನ = ಖಾಲಿ;
    ರಚನೆ rxkb_layout *layout;

    layout = container_ದ(ಸಂಕೇತ_2->ತಳ.ಪೋಷಕ, ರಚನೆ rxkb_layout, ತಳ);

    ಆದರೆ (list_is_ಕೊನೆಯ(&layout->iso3166s, &ಸಂಕೇತ_2->ತಳ.link))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಮುಂದಿನ = list_ಮೊದಲ_ನಮೂದು(&ಸಂಕೇತ_2->ತಳ.link, ಸಂಕೇತ_2, ತಳ.link);

    ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_ಸಂಕೇತ);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_ಸಂಕೇತ);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_ಸಂಕೇತ, ಸಂಕೇತ_2);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_option_destroy(ರಚನೆ rxkb_option *o)
{
    ಬಿಡುಗಡೆ(o->name);
    ಬಿಡುಗಡೆ(o->brief);
    ಬಿಡುಗಡೆ(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, ಎಣಿಕೆ rxkb_popularity);
bool rxkb_option_is_layout_specific(ರಚನೆ rxkb_option *object) {
    ಹಿಂತಿರುಗಿ object->layout_specific;
}
DECLARE_ಮೊದಲ_ಮುಂದಿನ_FOR_TYPE(rxkb_option, rxkb_option_ಗುಂಪು, options);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_layout_destroy(ರಚನೆ rxkb_layout *l)
{
    ರಚನೆ rxkb_iso639_ಸಂಕೇತ *iso639, *tmp_639;
    ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *iso3166, *tmp_3166;

    ಬಿಡುಗಡೆ(l->name);
    ಬಿಡುಗಡೆ(l->brief);
    ಬಿಡುಗಡೆ(l->description);
    ಬಿಡುಗಡೆ(l->variant);

    list_for_each_ಸುರಕ್ಷಿತ(iso639, tmp_639, &l->iso639s, ತಳ.link) {
        rxkb_iso639_ಸಂಕೇತ_unref(iso639);
    }
    list_for_each_ಸುರಕ್ಷಿತ(iso3166, tmp_3166, &l->iso3166s, ತಳ.link) {
        rxkb_iso3166_ಸಂಕೇತ_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, ಎಣಿಕೆ rxkb_popularity);
DECLARE_ಮೊದಲ_ಮುಂದಿನ_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_model_destroy(ರಚನೆ rxkb_model *m)
{
    ಬಿಡುಗಡೆ(m->name);
    ಬಿಡುಗಡೆ(m->vendor);
    ಬಿಡುಗಡೆ(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, ಎಣಿಕೆ rxkb_popularity);
DECLARE_ಮೊದಲ_ಮುಂದಿನ_FOR_TYPE(rxkb_model, rxkb_context, models);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_option_ಗುಂಪು_destroy(ರಚನೆ rxkb_option_ಗುಂಪು *og)
{
    ರಚನೆ rxkb_option *o, *otmp;

    ಬಿಡುಗಡೆ(og->name);
    ಬಿಡುಗಡೆ(og->description);

    list_for_each_ಸುರಕ್ಷಿತ(o, otmp, &og->options, ತಳ.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_ಗುಂಪು_allows_ಅಪವರ್ತ್ಯ(ರಚನೆ rxkb_option_ಗುಂಪು *g)
{
    ಹಿಂತಿರುಗಿ g->allow_ಅಪವರ್ತ್ಯ;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_ಗುಂಪು);
DECLARE_CREATE_FOR_TYPE(rxkb_option_ಗುಂಪು);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ಗುಂಪು, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_ಗುಂಪು, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_ಗುಂಪು, popularity, ಎಣಿಕೆ rxkb_popularity);
DECLARE_ಮೊದಲ_ಮುಂದಿನ_FOR_TYPE(rxkb_option_ಗುಂಪು, rxkb_context, option_groups);

ಸ್ಥಿರ ಶೂನ್ಯ
rxkb_context_destroy(ರಚನೆ rxkb_context *ctx)
{
    ರಚನೆ rxkb_model *m, *mtmp;
    ರಚನೆ rxkb_layout *l, *ltmp;
    ರಚನೆ rxkb_option_ಗುಂಪು *og, *ogtmp;
    ಅಕ್ಷರ **ಮಾರ್ಗ;

    list_for_each_ಸುರಕ್ಷಿತ(m, mtmp, &ctx->models, ತಳ.link)
        rxkb_model_unref(m);
    ಪ್ರತಿಜ್ಞಾ(list_ಖಾಲಿಗಳು(&ctx->models));

    list_for_each_ಸುರಕ್ಷಿತ(l, ltmp, &ctx->layouts, ತಳ.link)
        rxkb_layout_unref(l);
    ಪ್ರತಿಜ್ಞಾ(list_ಖಾಲಿಗಳು(&ctx->layouts));

    list_for_each_ಸುರಕ್ಷಿತ(og, ogtmp, &ctx->option_groups, ತಳ.link)
        rxkb_option_ಗುಂಪು_unref(og);
    ಪ್ರತಿಜ್ಞಾ(list_ಖಾಲಿಗಳು(&ctx->option_groups));

    darray_foreach(ಮಾರ್ಗ, ctx->includes)
        ಬಿಡುಗಡೆ(*ಮಾರ್ಗ);
    darray_free(ctx->includes);

    ಪ್ರತಿಜ್ಞಾ(darray_ಖಾಲಿಗಳು(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, ಎಣಿಕೆ rxkb_log_level);

ಸ್ಥಿರ ಅಕ್ಷರ *
rxkb_context_getenv(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಆದರೆ (ctx->use_secure_getenv) {
        ಹಿಂತಿರುಗಿ secure_getenv(name);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ getenv(name);
    }
}


ಶೂನ್ಯ
rxkb_context_ಗಣ_log_level(ರಚನೆ rxkb_context *ctx,
                           ಎಣಿಕೆ rxkb_log_level level)
{
    ctx->log_level = level;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
log_level_ಗಮ್ಯ_ಪೂರ್ವಪ್ರತ್ಯಯ(ಎಣಿಕೆ rxkb_log_level level)
{
    ಆಯ್ಕೆ (level) {
    ಸ್ಥಿತಿ RXKB_LOG_LEVEL_DEBUG:
        ಹಿಂತಿರುಗಿ "xkbregistry: DEBUG: ";
    ಸ್ಥಿತಿ RXKB_LOG_LEVEL_INFO:
        ಹಿಂತಿರುಗಿ "xkbregistry: INFO: ";
    ಸ್ಥಿತಿ RXKB_LOG_LEVEL_WARNING:
        ಹಿಂತಿರುಗಿ "xkbregistry: WARNING: ";
    ಸ್ಥಿತಿ RXKB_LOG_LEVEL_ERROR:
        ಹಿಂತಿರುಗಿ "xkbregistry: ERROR: ";
    ಸ್ಥಿತಿ RXKB_LOG_LEVEL_CRITICAL:
        ಹಿಂತಿರುಗಿ "xkbregistry: CRITICAL: ";
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
}

ATTR_PRINTF(3, 0) ಸ್ಥಿರ ಶೂನ್ಯ
default_log_fn(ರಚನೆ rxkb_context *ctx, ಎಣಿಕೆ rxkb_log_level level,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = log_level_ಗಮ್ಯ_ಪೂರ್ವಪ್ರತ್ಯಯ(level);

    ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
        fprintf(stderr, "%s", ಪೂರ್ವಪ್ರತ್ಯಯ);
    vfprintf(stderr, fmt, ವಾದಗಳು);
}

ಸ್ಥಿರ ಎಣಿಕೆ rxkb_log_level
log_level(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *level) {
    errno = 0;
    ಅಕ್ಷರ *endptr;
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ lvl = strtol(level, &endptr, 10);
    ಆದರೆ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ rxkb_log_level)lvl;

    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("crit", level))
        ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_CRITICAL;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("err", level))
        ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_ERROR;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("warn", level))
        ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_WARNING;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("info", level))
        ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_INFO;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("debug", level) || istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("dbg", level))
        ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_DEBUG;

    ಹಿಂತಿರುಗಿ RXKB_LOG_LEVEL_ERROR;
}

ರಚನೆ rxkb_context *
rxkb_context_new(ಎಣಿಕೆ rxkb_context_flags flags)
{
    ರಚನೆ rxkb_context *ctx = rxkb_context_create(ಖಾಲಿ);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env;

    ಆದರೆ (!ctx)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ctx->context_state = CONTEXT_NEW;
    ctx->ತುಂಬು_extra_ನಿಯಮಗಳು_files = flags & RXKB_CONTEXT_ತುಂಬು_EXOTIC_ನಿಯಮಗಳು;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ಆದರೆ (env)
        rxkb_context_ಗಣ_log_level(ctx, log_level(env));

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_ತುಂಬು_EXOTIC_ನಿಯಮಗಳು
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ಆದರೆ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        ಬಿಡುಗಡೆ(ctx);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ಆದರೆ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_ಮಾರ್ಗ_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_ಮಾರ್ಗ,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_ಬೇರು);
        rxkb_context_unref(ctx);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ
rxkb_context_ಗಣ_log_fn(ರಚನೆ rxkb_context *ctx,
                        ಶೂನ್ಯ (*log_fn)(ರಚನೆ rxkb_context *ctx,
                                       ಎಣಿಕೆ rxkb_log_level level,
                                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_ಮಾರ್ಗ_append(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
    ಪೂರ್ಣಾಂಕ err = 0;

    ಆದರೆ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "include paths can only be appended to a new context\n");
        ಹೋಗು error;
    }

    ರಚನೆ stat stat_buf;
    err = stat(ಮಾರ್ಗ, &stat_buf);
    ಆದರೆ (err != 0) {
        err = errno;
        ಹೋಗು error;
    }
    ಆದರೆ (!S_ISDIR(stat_buf.st_ಬಹುಲಕ)) {
        err = ENOTDIR;
        ಹೋಗು error;
    }

    ಆದರೆ (!check_eaccess(ಮಾರ್ಗ, R_OK | X_OK)) {
        err = EACCES;
        ಹೋಗು error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    ಅಕ್ಷರ ನಿಯಮಗಳು[PATH_MAX];
    ಆದರೆ (!snprintf_ಸುರಕ್ಷಿತ(ನಿಯಮಗಳು, ಗಾತ್ರ(ನಿಯಮಗಳು), "%s/rules/%s.xml",
                       ಮಾರ್ಗ, DEFAULT_XKB_ನಿಯಮಗಳು)) {
        log_err(ctx, XKB_ERROR_INVALID_ಮಾರ್ಗ,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                ಗಾತ್ರ(ನಿಯಮಗಳು), ಮಾರ್ಗ, DEFAULT_XKB_ನಿಯಮಗಳು);
        ಹೋಗು error;
    }

    ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ tmp = strdup(ಮಾರ್ಗ);
    ಆದರೆ (!tmp) {
        err = ENOMEM;
        ಹೋಗು error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID, "Include path added: %s\n", tmp);

    ಹಿಂತಿರುಗಿ true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
             "Include path failed: \"%s\" (%s)\n", ಮಾರ್ಗ, strerror(err));
    ಹಿಂತಿರುಗಿ false;
}

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_str(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಹಿಂತಿರುಗಿ strcmp(*(ಅಕ್ಷರ **)a, *(ಅಕ್ಷರ **) b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕೂಡಿಸು_ನೇರ_subdirectories(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                          darray_string *extensions,
                          darray_ಗಾತ್ರ_t versioned_ಎಣಿಕೆ,
                          ಗಾತ್ರ_t versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ)
{
    ಪೂರ್ಣಾಂಕ ret = 0;
    ಪೂರ್ಣಾಂಕ err = ENOMEM;
    DIR *dir = ಖಾಲಿ;

    /* Check extensions parent directory */
    ರಚನೆ stat stat_buf;
    err = stat(ಮಾರ್ಗ, &stat_buf);
    ಆದರೆ (err != 0) {
        err = errno;
        ಹೋಗು err;
    }
    ಆದರೆ (!S_ISDIR(stat_buf.st_ಬಹುಲಕ)) {
        err = ENOTDIR;
        ಹೋಗು err;
    }
    ಆದರೆ (!check_eaccess(ಮಾರ್ಗ, R_OK | X_OK)) {
        err = EACCES;
        ಹೋಗು err;
    }

    dir = opendir(ಮಾರ್ಗ);
    ಆದರೆ (dir == ಖಾಲಿ) {
        err = EACCES;
        ಹೋಗು err;
    }

    ರಚನೆ dirent *ನಮೂದು;
    ಅಕ್ಷರ ಮಾರ್ಗ_buf[PATH_MAX] = "";
    versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ++; /* Additional final ‘/’ */
    ಆಗ ((ನಮೂದು = readdir(dir)) != ಖಾಲಿ) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ನಮೂದು->d_name;

        /* Skip special entries */
        ಆದರೆ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ಮುಂದುವರಿ;

        ಆದರೆ (!snprintf_ಸುರಕ್ಷಿತ(ಮಾರ್ಗ_buf, ಗಾತ್ರ(ಮಾರ್ಗ_buf), "%s/%s", ಮಾರ್ಗ, name)) {
            err = ENOMEM;
            ಹೋಗು err;
        }
        ಆದರೆ (stat(ಮಾರ್ಗ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_ಬಹುಲಕ))
            ಮುಂದುವರಿ;

        /* Skip if the corresponding versioned directory is already included */
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < versioned_ಎಣಿಕೆ; i++) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ prev_name = darray_ಅಂಶ(*extensions, i)
                                         + versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ;
            ಆದರೆ (strcmp(name, prev_name) == 0)
                ಹೋಗು ಮುಂದಿನ;
        }

        ಅಕ್ಷರ *ext_ಮಾರ್ಗ = strdup_ಸುರಕ್ಷಿತ(ಮಾರ್ಗ_buf);
        ಆದರೆ (!ext_ಮಾರ್ಗ) {
            err = ENOMEM;
            ಹೋಗು err;
        }

        darray_append(*extensions, ext_ಮಾರ್ಗ);
ಮುಂದಿನ:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ಆದರೆ (darray_ಗಾತ್ರ(*extensions) > versioned_ಎಣಿಕೆ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ವಸ್ತುಗಳು(*extensions) + versioned_ಎಣಿಕೆ,
              darray_ಗಾತ್ರ(*extensions) - versioned_ಎಣಿಕೆ,
              ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(*extensions)), &compare_str);
        ಅಕ್ಷರ **ext_ಮಾರ್ಗ;
        darray_foreach_from(ext_ಮಾರ್ಗ, *extensions, versioned_ಎಣಿಕೆ) {
            ret |= rxkb_context_include_ಮಾರ್ಗ_append(ctx, *ext_ಮಾರ್ಗ);
        }
    }

    ಹಿಂತಿರುಗಿ ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", ಮಾರ್ಗ, strerror(err));
    ಆದರೆ (dir)
        closedir(dir);

    ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

bool
rxkb_context_include_ಮಾರ್ಗ_append_default(ರಚನೆ rxkb_context *ctx)
{
    ಅಕ್ಷರ user_ಮಾರ್ಗ[PATH_MAX];
    bool ret = false;

    ಆದರೆ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "include paths can only be appended to a new context\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ home = rxkb_context_getenv(ctx, "HOME");
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ಆದರೆ (xdg != ಖಾಲಿ) {
        ಆದರೆ (snprintf_ಸುರಕ್ಷಿತ(user_ಮಾರ್ಗ, ಗಾತ್ರ(user_ಮಾರ್ಗ), "%s/xkb", xdg))
            ret |= rxkb_context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (home != ಖಾಲಿ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ಆದರೆ (snprintf_ಸುರಕ್ಷಿತ(user_ಮಾರ್ಗ, ಗಾತ್ರ(user_ಮಾರ್ಗ), "%s/.config/xkb", home))
            ret |= rxkb_context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
    }

    ಆದರೆ (home != ಖಾಲಿ) {
        ಆದರೆ (snprintf_ಸುರಕ್ಷಿತ(user_ಮಾರ್ಗ, ಗಾತ್ರ(user_ಮಾರ್ಗ), "%s/.xkb", home))
            ret |= rxkb_context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_ಮಾರ್ಗ_append(
        ctx, ((extra != ಖಾಲಿ) ? extra : DFLT_XKB_CONFIG_EXTRA_ಮಾರ್ಗ)
    );

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *extensions_ಮಾರ್ಗ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ಮಾರ್ಗ
    ಆದರೆ (extensions_ಮಾರ್ಗ == ಖಾಲಿ)
        extensions_ಮಾರ್ಗ = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ಮಾರ್ಗ;
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾತ್ರ_t versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ = 0;
    ಆದರೆ (extensions_ಮಾರ್ಗ != ಖಾಲಿ) {
        /* Add direct subdirectories */
        ret |= ಕೂಡಿಸು_ನೇರ_subdirectories(ctx, extensions_ಮಾರ್ಗ,
                                         &extensions, 0, 0);
        versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(extensions_ಮಾರ್ಗ);
    }

    /* Unversioned extensions directory */
    extensions_ಮಾರ್ಗ =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ಮಾರ್ಗ
    ಆದರೆ (extensions_ಮಾರ್ಗ == ಖಾಲಿ)
        extensions_ಮಾರ್ಗ = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ಮಾರ್ಗ;
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (extensions_ಮಾರ್ಗ != ಖಾಲಿ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= ಕೂಡಿಸು_ನೇರ_subdirectories(ctx, extensions_ಮಾರ್ಗ, &extensions,
                                         darray_ಗಾತ್ರ(extensions),
                                         versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ);
    }

    ಅಕ್ಷರ **ext_ಮಾರ್ಗ;
    darray_foreach(ext_ಮಾರ್ಗ, extensions) {
        ಬಿಡುಗಡೆ(*ext_ಮಾರ್ಗ);
    }
    darray_free(extensions);
#ಆದರೆ_ಅಂತ್ಯ

    /* Canonical XKB root */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಬೇರು = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಸ್ಥಿರಾಂಕ bool has_ಬೇರು = rxkb_context_include_ಮಾರ್ಗ_append(
        ctx, ((ಬೇರು != ಖಾಲಿ) ? ಬೇರು: DFLT_XKB_CONFIG_ಬೇರು)
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
    ಆದರೆ (!has_ಬೇರು && (ಬೇರು == ಖಾಲಿ || ಬೇರು[0] != '\0')) {
        log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ಬೇರು,
                 ((ಬೇರು == ಖಾಲಿ) ? DFLT_XKB_CONFIG_ಬೇರು : ಬೇರು));
        ret |= rxkb_context_include_ಮಾರ್ಗ_append(ctx, DFLT_XKB_LEGACY_ಬೇರು);
    }

    ಹಿಂತಿರುಗಿ ret;
}

bool
rxkb_context_parse_default_ruleset(ರಚನೆ rxkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ rxkb_context_parse(ctx, DEFAULT_XKB_ನಿಯಮಗಳು);
}

bool
rxkb_context_parse(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ruleset)
{
    ಅಕ್ಷರ **ಮಾರ್ಗ;
    bool success = false;

    ಆದರೆ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "parse must only be called on a new context\n");
        ಹಿಂತಿರುಗಿ false;
    }

    darray_foreach_ಹಿಮ್ಮುಖ(ಮಾರ್ಗ, ctx->includes) {
        ಅಕ್ಷರ ನಿಯಮಗಳು[PATH_MAX];

        ಆದರೆ (snprintf_ಸುರಕ್ಷಿತ(ನಿಯಮಗಳು, ಗಾತ್ರ(ನಿಯಮಗಳು), "%s/rules/%s.xml",
                           *ಮಾರ್ಗ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", ನಿಯಮಗಳು);
            ಆದರೆ (parse(ctx, ನಿಯಮಗಳು, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ಆದರೆ (ctx->ತುಂಬು_extra_ನಿಯಮಗಳು_files &&
            snprintf_ಸುರಕ್ಷಿತ(ನಿಯಮಗಳು, ಗಾತ್ರ(ನಿಯಮಗಳು), "%s/rules/%s.extras.xml",
                          *ಮಾರ್ಗ, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", ನಿಯಮಗಳು);
            ಆದರೆ (parse(ctx, ನಿಯಮಗಳು, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    ಹಿಂತಿರುಗಿ success;
}


ಶೂನ್ಯ
rxkb_context_ಗಣ_user_ದತ್ತಾಂಶ(ರಚನೆ rxkb_context *ctx, ಶೂನ್ಯ *userdata)
{
    ctx->userdata = userdata;
}

ಶೂನ್ಯ *
rxkb_context_get_user_ದತ್ತಾಂಶ(ರಚನೆ rxkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ ctx->userdata;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ಗಂಟು(xml_ಗಂಟು *ಗಂಟು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಹಿಂತಿರುಗಿ ಗಂಟು->type == XML_ELEMENT_ಗಂಟು &&
        xmlStrEqual(ಗಂಟು->name, (ಸ್ಥಿರಾಂಕ xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
ಸ್ಥಿರ ಅಕ್ಷರ *
ಹೊರತೆಗೆ_ಪಠ್ಯ(xml_ಗಂಟು *ಗಂಟು)
{
    xml_ಗಂಟು *n;

    ಗಾಗಿ (n = ಗಂಟು->children; n; n = n->ಮುಂದಿನ) {
        ಆದರೆ (n->type == XML_ಪಠ್ಯ_ಗಂಟು)
            ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *)xmlStrdup(n->content);
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Data from “configItem” node */
ರಚನೆ config_ಅಂಶ {
    ಅಕ್ಷರ *name;
    ಅಕ್ಷರ *description;
    ಅಕ್ಷರ *brief;
    ಅಕ್ಷರ *vendor;
    ಎಣಿಕೆ rxkb_popularity popularity;
    bool layout_specific;
};

#ವ್ಯಾಖ್ಯಾನಿಸು config_ಅಂಶ_new(popularity_) { \
    .name = ಖಾಲಿ, \
    .description = ಖಾಲಿ, \
    .brief = ಖಾಲಿ, \
    .vendor = ಖಾಲಿ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

ಸ್ಥಿರ ಶೂನ್ಯ
config_ಅಂಶ_free(ರಚನೆ config_ಅಂಶ *config) {
    ಬಿಡುಗಡೆ(config->name);
    ಬಿಡುಗಡೆ(config->description);
    ಬಿಡುಗಡೆ(config->brief);
    ಬಿಡುಗಡೆ(config->vendor);
}

ಸ್ಥಿರ bool
parse_config_ಅಂಶ(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *ಪೋಷಕ,
                  ರಚನೆ config_ಅಂಶ *config)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
    xml_ಗಂಟು *ci = ಖಾಲಿ;

    ಗಾಗಿ (ci = ಪೋಷಕ->children; ci; ci = ci->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (ಸ್ಥಿರಾಂಕ xmlChar*)"popularity");
            ಆದರೆ (raw_popularity) {
                ಆದರೆ (xmlStrEqual(raw_popularity, (ಸ್ಥಿರಾಂಕ xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (xmlStrEqual(raw_popularity, (ಸ್ಥಿರಾಂಕ xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                ಇಲ್ಲದಿದ್ದರೆ
                    log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->ಸಾಲು, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (ಸ್ಥಿರಾಂಕ xmlChar*)"layout-specific");
            ಆದರೆ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (ಸ್ಥಿರಾಂಕ xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            ಗಾಗಿ (ಗಂಟು = ci->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
                ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "name"))
                    config->name = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "description"))
                    config->description = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "shortDescription"))
                    config->brief = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "vendor"))
                    config->vendor = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ಆದರೆ (!config->name || !ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(config->name))  {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->ಸಾಲು);
                config_ಅಂಶ_free(config);
                ಹಿಂತಿರುಗಿ false;
            }

            ಹಿಂತಿರುಗಿ true; /* only one configItem allowed in the dtd */
        }
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_model(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *model,
            ಎಣಿಕೆ rxkb_popularity popularity)
{
    ರಚನೆ config_ಅಂಶ config = config_ಅಂಶ_new(popularity);

    ಆದರೆ (parse_config_ಅಂಶ(ctx, model, &config)) {
        ರಚನೆ rxkb_model *m;

        list_for_each(m, &ctx->models, ತಳ.link) {
            ಆದರೆ (streq(m->name, config.name)) {
                config_ಅಂಶ_free(&config);
                ಹಿಂತಿರುಗಿ;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->ತಳ);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->ತಳ.link);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_model_list(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *model_list,
                 ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    ಗಾಗಿ (ಗಂಟು = model_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "model"))
            parse_model(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_language_list(xml_ಗಂಟು *language_list, ರಚನೆ rxkb_layout *layout)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
    ರಚನೆ rxkb_iso639_ಸಂಕೇತ *ಸಂಕೇತ_2;

    ಗಾಗಿ (ಗಂಟು = language_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "iso639Id")) {
            ಅಕ್ಷರ *str = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
            ರಚನೆ rxkb_object *ಪೋಷಕ;

            ಆದರೆ (!str || ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str) != 3) {
                ಬಿಡುಗಡೆ(str);
                ಮುಂದುವರಿ;
            }

            ಪೋಷಕ = &layout->ತಳ;
            ಸಂಕೇತ_2 = rxkb_iso639_ಸಂಕೇತ_create(ಪೋಷಕ);
            ಸಂಕೇತ_2->ಸಂಕೇತ_2 = str;
            list_append(&layout->iso639s, &ಸಂಕೇತ_2->ತಳ.link);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_country_list(xml_ಗಂಟು *country_list, ರಚನೆ rxkb_layout *layout)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
    ರಚನೆ rxkb_iso3166_ಸಂಕೇತ *ಸಂಕೇತ_2;

    ಗಾಗಿ (ಗಂಟು = country_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "iso3166Id")) {
            ಅಕ್ಷರ *str = ಹೊರತೆಗೆ_ಪಠ್ಯ(ಗಂಟು);
            ರಚನೆ rxkb_object *ಪೋಷಕ;

            ಆದರೆ (!str || ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str) != 2) {
                ಬಿಡುಗಡೆ(str);
                ಮುಂದುವರಿ;
            }

            ಪೋಷಕ = &layout->ತಳ;
            ಸಂಕೇತ_2 = rxkb_iso3166_ಸಂಕೇತ_create(ಪೋಷಕ);
            ಸಂಕೇತ_2->ಸಂಕೇತ_2 = str;
            list_append(&layout->iso3166s, &ಸಂಕೇತ_2->ತಳ.link);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_variant(ರಚನೆ rxkb_context *ctx, ರಚನೆ rxkb_layout *l,
              xml_ಗಂಟು *variant, ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ci;
    ರಚನೆ config_ಅಂಶ config = config_ಅಂಶ_new(popularity);

    ಆದರೆ (parse_config_ಅಂಶ(ctx, variant, &config)) {
        ರಚನೆ rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, ತಳ.link) {
            ಆದರೆ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ನಿಲ್ಲಿಸು;
            }
        }

        ಆದರೆ (!exists) {
            v = rxkb_layout_create(&ctx->ತಳ);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ಖಾಲಿ ? strdup_ಸುರಕ್ಷಿತ(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->ತಳ.link);

            ಗಾಗಿ (ci = variant->children; ci; ci = ci->ಮುಂದಿನ) {
                xml_ಗಂಟು *ಗಂಟು;

                ಆದರೆ (!is_ಗಂಟು(ci, "configItem"))
                    ಮುಂದುವರಿ;

                bool ಸಿಕ್ಕಿತು_language_list = false;
                bool ಸಿಕ್ಕಿತು_country_list = false;
                ಗಾಗಿ (ಗಂಟು = ci->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
                    ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "languageList")) {
                        parse_language_list(ಗಂಟು, v);
                        ಸಿಕ್ಕಿತು_language_list = true;
                    }
                    ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "countryList")) {
                        parse_country_list(ಗಂಟು, v);
                        ಸಿಕ್ಕಿತು_country_list = true;
                    }
                }
                ಆದರೆ (!ಸಿಕ್ಕಿತು_language_list) {
                    // inherit from parent layout
                    ರಚನೆ rxkb_iso639_ಸಂಕೇತ* x;
                    list_for_each(x, &l->iso639s, ತಳ.link) {
                        ರಚನೆ rxkb_iso639_ಸಂಕೇತ* ಸಂಕೇತ_2 = rxkb_iso639_ಸಂಕೇತ_create(&v->ತಳ);
                        ಸಂಕೇತ_2->ಸಂಕೇತ_2 = strdup(x->ಸಂಕೇತ_2);
                        list_append(&v->iso639s, &ಸಂಕೇತ_2->ತಳ.link);
                    }
                }
                ಆದರೆ (!ಸಿಕ್ಕಿತು_country_list) {
                    // inherit from parent layout
                    ರಚನೆ rxkb_iso3166_ಸಂಕೇತ* x;
                    list_for_each(x, &l->iso3166s, ತಳ.link) {
                        ರಚನೆ rxkb_iso3166_ಸಂಕೇತ* ಸಂಕೇತ_2 = rxkb_iso3166_ಸಂಕೇತ_create(&v->ತಳ);
                        ಸಂಕೇತ_2->ಸಂಕೇತ_2 = strdup(x->ಸಂಕೇತ_2);
                        list_append(&v->iso3166s, &ಸಂಕೇತ_2->ತಳ.link);
                    }
                }
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            config_ಅಂಶ_free(&config);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_variant_list(ರಚನೆ rxkb_context *ctx, ರಚನೆ rxkb_layout *l,
                   xml_ಗಂಟು *variant_list, ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    ಗಾಗಿ (ಗಂಟು = variant_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "variant"))
            parse_variant(ctx, l, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_layout(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *layout,
             ಎಣಿಕೆ rxkb_popularity popularity)
{
    ರಚನೆ config_ಅಂಶ config = config_ಅಂಶ_new(popularity);
    ರಚನೆ rxkb_layout *l;
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
    bool exists = false;

    ಆದರೆ (!parse_config_ಅಂಶ(ctx, layout, &config))
        ಹಿಂತಿರುಗಿ;

    list_for_each(l, &ctx->layouts, ತಳ.link) {
        ಆದರೆ (streq(l->name, config.name) && l->variant == ಖಾಲಿ) {
            exists = true;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (!exists) {
        l = rxkb_layout_create(&ctx->ತಳ);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ಖಾಲಿ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->ತಳ.link);
    } ಇಲ್ಲದಿದ್ದರೆ {
        config_ಅಂಶ_free(&config);
    }

    ಗಾಗಿ (ಗಂಟು = layout->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "variantList")) {
            parse_variant_list(ctx, l, ಗಂಟು, popularity);
        }
        ಆದರೆ (!exists && is_ಗಂಟು(ಗಂಟು, "configItem")) {
            xml_ಗಂಟು *ll;
            ಗಾಗಿ (ll = ಗಂಟು->children; ll; ll = ll->ಮುಂದಿನ) {
                ಆದರೆ (is_ಗಂಟು(ll, "languageList"))
                    parse_language_list(ll, l);
                ಆದರೆ (is_ಗಂಟು(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_layout_list(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *layout_list,
                  ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    ಗಾಗಿ (ಗಂಟು = layout_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "layout"))
            parse_layout(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_option(ರಚನೆ rxkb_context *ctx, ರಚನೆ rxkb_option_ಗುಂಪು *ಗುಂಪು,
             xml_ಗಂಟು *option, ಎಣಿಕೆ rxkb_popularity popularity)
{
    ರಚನೆ config_ಅಂಶ config = config_ಅಂಶ_new(popularity);

    ಆದರೆ (parse_config_ಅಂಶ(ctx, option, &config)) {
        ರಚನೆ rxkb_option *o;

        list_for_each(o, &ಗುಂಪು->options, ತಳ.link) {
            ಆದರೆ (streq(o->name, config.name)) {
                config_ಅಂಶ_free(&config);
                ಹಿಂತಿರುಗಿ;
            }
        }

        o = rxkb_option_create(&ಗುಂಪು->ತಳ);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&ಗುಂಪು->options, &o->ತಳ.link);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ಗುಂಪು(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *ಗುಂಪು,
            ಎಣಿಕೆ rxkb_popularity popularity)
{
    ರಚನೆ config_ಅಂಶ config = config_ಅಂಶ_new(popularity);
    ರಚನೆ rxkb_option_ಗುಂಪು *g;
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
    xmlChar *ಅಪವರ್ತ್ಯ;
    bool exists = false;

    ಆದರೆ (!parse_config_ಅಂಶ(ctx, ಗುಂಪು, &config))
        ಹಿಂತಿರುಗಿ;

    list_for_each(g, &ctx->option_groups, ತಳ.link) {
        ಆದರೆ (streq(g->name, config.name)) {
            exists = true;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (!exists) {
        g = rxkb_option_ಗುಂಪು_create(&ctx->ತಳ);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        ಅಪವರ್ತ್ಯ = xmlGetProp(ಗುಂಪು, (ಸ್ಥಿರಾಂಕ xmlChar*)"allowMultipleSelection");
        ಆದರೆ (ಅಪವರ್ತ್ಯ && xmlStrEqual(ಅಪವರ್ತ್ಯ, (ಸ್ಥಿರಾಂಕ xmlChar*)"true"))
            g->allow_ಅಪವರ್ತ್ಯ = true;
        xmlFree(ಅಪವರ್ತ್ಯ);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->ತಳ.link);
    } ಇಲ್ಲದಿದ್ದರೆ {
        config_ಅಂಶ_free(&config);
    }

    ಗಾಗಿ (ಗಂಟು = ಗುಂಪು->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "option"))
            parse_option(ctx, g, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_option_list(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *option_list,
                  ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    ಗಾಗಿ (ಗಂಟು = option_list->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "group"))
            parse_ಗುಂಪು(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ನಿಯಮಗಳು_xml(ರಚನೆ rxkb_context *ctx, xml_ಗಂಟು *ಬೇರು,
                ಎಣಿಕೆ rxkb_popularity popularity)
{
    xml_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    ಗಾಗಿ (ಗಂಟು = ಬೇರು->children; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ) {
        ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "modelList"))
            parse_model_list(ctx, ಗಂಟು, popularity);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "layoutList"))
            parse_layout_list(ctx, ಗಂಟು, popularity);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಗಂಟು(ಗಂಟು, "optionList"))
            parse_option_list(ctx, ಗಂಟು, popularity);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ATTR_PRINTF(2, 0)
xml_error_func(ಶೂನ್ಯ *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
    ಸ್ಥಿರ ಅಕ್ಷರ buf[PATH_MAX];
    ಸ್ಥಿರ ಗಾತ್ರ_t slen = 0;
    va_list ವಾದಗಳು;
    ಪೂರ್ಣಾಂಕ rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_ಆರಂಭ(ವಾದಗಳು, ಸಂದೇಶ);
    rc = vsnprintf(&buf[slen], ಗಾತ್ರ(buf) - slen, ಸಂದೇಶ, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    /* This shouldn't really happen */
    ಆದರೆ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, ಗಾತ್ರ(buf));
        ಹಿಂತಿರುಗಿ;
    }

    slen += (ಗಾತ್ರ_t)rc;
    ಆದರೆ (slen >= ಗಾತ್ರ(buf)) {
        /* truncated, let's flush this */
        buf[ಗಾತ್ರ(buf) - 1] = '\n';
        slen = ಗಾತ್ರ(buf);
    }

    /* We're assuming here that the last character is \n. */
    ಆದರೆ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID, "%s", buf);
        memset(buf, 0, ಗಾತ್ರ(buf));
        slen = 0;
    }
}

#ಆದರೆ HAVE_XML_CTXT_ಗಣ_ERRORHANDLER
ಸ್ಥಿರ ಶೂನ್ಯ
xml_structured_error_func(ಶೂನ್ಯ *user_ದತ್ತಾಂಶ_2, ಸ್ಥಿರಾಂಕ xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_ದತ್ತಾಂಶ_2);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XML_PARSE_NO_XXE
#ವ್ಯಾಖ್ಯಾನಿಸು _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು _XML_OPTIONS (XML_PARSE_NONET)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ bool
validate(ರಚನೆ rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ dtdstr[] =
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

#ಆದರೆ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ಆದರೆ (!xmlCtxt)
        ಹಿಂತಿರುಗಿ false;
    static_assert(HAVE_XML_CTXT_ಗಣ_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_ಗಣ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_ಗಣ_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ಒಳಹರಿವು_Ptr pinput =
        xml_New_ಒಳಹರಿವು_From_ಸ್ಮರಣೆ(ಖಾಲಿ, dtdstr, ARRAY_ಗಾತ್ರ(dtdstr) - 1,
                              XML_ಒಳಹರಿವು_BUF_STATIC);
    ಆದರೆ (!pinput)
        ಹೋಗು dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ಖಾಲಿ, ಖಾಲಿ);
#ಇಲ್ಲದಿದ್ದರೆ
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ಒಳಹರಿವು_ಬಫರ್_Ptr buf =
        xml_Parser_ಒಳಹರಿವು_ಬಫರ್_Create_Mem(dtdstr, ARRAY_ಗಾತ್ರ(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ಆದರೆ (!buf)
        ಹೋಗು dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ಖಾಲಿ, buf, XML_CHAR_ENCODING_UTF8);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (!dtd) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID, "Failed to load DTD\n");
        ಹೋಗು dtd_error;
    }

#ಆದರೆ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#ಇಲ್ಲದಿದ್ದರೆ
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ಆದರೆ (!dtdvalid) {
        success = false;
        ಹೋಗು ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_ದತ್ತಾಂಶ_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#ಆದರೆ_ಅಂತ್ಯ
    xmlFreeDtd(dtd);

dtd_error:
#ಆದರೆ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ success;
}

ಸ್ಥಿರ bool
parse(ರಚನೆ rxkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
      ಎಣಿಕೆ rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ಖಾಲಿ;
    xml_ಗಂಟು *ಬೇರು = ಖಾಲಿ;

    ಆದರೆ (!check_eaccess(ಮಾರ್ಗ, R_OK))
        ಹಿಂತಿರುಗಿ false;

    LIBXML_TEST_ಆವೃತ್ತಿ

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ಆದರೆ (!xmlCtxt)
        ಹಿಂತಿರುಗಿ false;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XML_PARSE_NO_XXE
#ವ್ಯಾಖ್ಯಾನಿಸು _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು _XML_OPTIONS (XML_PARSE_NONET)
#ಆದರೆ_ಅಂತ್ಯ
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ಆದರೆ HAVE_XML_CTXT_ಗಣ_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_ಗಣ_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_ಗಣ_Generic_Error_Func(ctx, xml_error_func);
#ಆದರೆ_ಅಂತ್ಯ

    doc = xmlCtxtReadFile(xmlCtxt, ಮಾರ್ಗ, ಖಾಲಿ, 0);
    ಆದರೆ (!doc)
        ಹೋಗು parse_error;

    ಆದರೆ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "XML error: failed to validate document at %s\n", ಮಾರ್ಗ);
        ಹೋಗು validate_error;
    }

    ಬೇರು = xml_Doc_Get_ಬೇರು_Element(doc);
    parse_ನಿಯಮಗಳು_xml(ctx, ಬೇರು, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ಆದರೆ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xml_ಗಣ_Generic_Error_Func(ಖಾಲಿ, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_XML_CTXT_ಗಣ_ERRORHANDLER
    xml_Ctxt_ಗಣ_Error_Handler(xmlCtxt, ಖಾಲಿ, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
    xmlFreeParserCtxt(xmlCtxt);

    ಹಿಂತಿರುಗಿ success;
}
