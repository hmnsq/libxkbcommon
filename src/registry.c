/*
 * Copyright © 2020 Red Hat, Inc.
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <libxml/parser.h>

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #అన్తర్భావ <limits.h>
    #ఒకవేళ HAVE_UNISTD_H && HAVE_DIRENT_H
        #అన్తర్భావ <dirent.h>
        #అన్తర్భావ <unistd.h>
    #అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "xkbcommon/xkbregistry.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-list.h"
#అన్తర్భావ "util-mem.h"

నిర్మాణం rxkb_object;

/**
 * All our objects are refcounted and are linked to iterate through them.
 * Abstract those bits away into a shared parent class so we can generate
 * most of the functions through macros.
 */
నిర్మాణం rxkb_object {
    నిర్మాణం rxkb_object *మాతృ;
    uint32_t refcount;
    నిర్మాణం list link;
};

నిర్మాణం rxkb_iso639_సంకేతం {
    నిర్మాణం rxkb_object పునాది;
    అక్షరం *సంకేతం_2;
};

నిర్మాణం rxkb_iso3166_సంకేతం {
    నిర్మాణం rxkb_object పునాది;
    అక్షరం *సంకేతం_2;
};

గణన context_state {
    CONTEXT_NEW,
    CONTEXT_PARSED,
    CONTEXT_FAILED,
};

నిర్మాణం rxkb_context {
    నిర్మాణం rxkb_object పునాది;
    గణన context_state context_state;

    bool ఎక్కించు_extra_నియమాలు_files;
    bool use_secure_getenv;

    నిర్మాణం list models;         /* list of struct rxkb_models */
    నిర్మాణం list layouts;        /* list of struct rxkb_layouts */
    నిర్మాణం list option_groups;  /* list of struct rxkb_option_group */

    darray(అక్షరం *) includes;


    ATTR_PRINTF(3, 0) శూన్య (*log_fn)(నిర్మాణం rxkb_context *ctx,
                                     గణన rxkb_log_level level,
                                     స్థిరరాశి అక్షరం *fmt, va_list వాదనలు);
    గణన rxkb_log_level log_level;

    శూన్య *userdata;
};

నిర్మాణం rxkb_model {
    నిర్మాణం rxkb_object పునాది;

    అక్షరం *name;
    అక్షరం *vendor;
    అక్షరం *description;
    గణన rxkb_popularity popularity;
};

నిర్మాణం rxkb_layout {
    నిర్మాణం rxkb_object పునాది;

    అక్షరం *name;
    అక్షరం *brief;
    అక్షరం *description;
    అక్షరం *variant;
    గణన rxkb_popularity popularity;

    నిర్మాణం list iso639s;  /* list of struct rxkb_iso639_code */
    నిర్మాణం list iso3166s; /* list of struct rxkb_iso3166_code */
};

నిర్మాణం rxkb_option_సమూహం {
    నిర్మాణం rxkb_object పునాది;

    bool allow_గుణిజం;
    నిర్మాణం list options; /* list of struct rxkb_options */
    అక్షరం *name;
    అక్షరం *description;
    గణన rxkb_popularity popularity;
};

నిర్మాణం rxkb_option {
    నిర్మాణం rxkb_object పునాది;

    అక్షరం *name;
    అక్షరం *brief;
    అక్షరం *description;
    గణన rxkb_popularity popularity;
    bool layout_specific;
};

స్థిర bool
parse(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం,
      గణన rxkb_popularity popularity);

ATTR_PRINTF(3, 4)
స్థిర శూన్య
rxkb_log(నిర్మాణం rxkb_context *ctx, గణన rxkb_log_level level,
         స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list వాదనలు;

    ఒకవేళ (ctx->log_level < level)
        తిరిగి;

    va_ప్రారంభం(వాదనలు, fmt);
    ctx->log_fn(ctx, level, fmt, వాదనలు);
    va_చివర(వాదనలు);
}

/*
 * The format is not part of the argument list in order to avoid the
 * "ISO C99 requires rest arguments to be used" warning when only the
 * format is supplied without arguments. Not supplying it would still
 * result in an error, though.
 */
#నిర్వచించు rxkb_log_with_సంకేతం(ctx, level, సందేశం_id, fmt, ...) \
    rxkb_log(ctx, level, PREPEND_సందేశం_ID(సందేశం_id, fmt), ##__VA_ARGS__)
#నిర్వచించు log_dbg(ctx, ...) \
    rxkb_log((ctx), RXKB_LOG_LEVEL_DEBUG, __VA_ARGS__)
#నిర్వచించు log_info(ctx, id, ...) \
    rxkb_log_with_సంకేతం((ctx), RXKB_LOG_LEVEL_INFO, id, __VA_ARGS__)
#నిర్వచించు log_warn(ctx, id, ...) \
    rxkb_log_with_సంకేతం((ctx), RXKB_LOG_LEVEL_WARNING, id, __VA_ARGS__)
#నిర్వచించు log_err(ctx, id, ...) \
    rxkb_log_with_సంకేతం((ctx), RXKB_LOG_LEVEL_ERROR, id, __VA_ARGS__)
#నిర్వచించు log_wsgo(ctx, id, ...) \
    rxkb_log_with_సంకేతం((ctx), RXKB_LOG_LEVEL_CRITICAL, id, __VA_ARGS__)


#నిర్వచించు DECLARE_REF_UNREF_FOR_TYPE(type_) \
నిర్మాణం type_ * type_##_ref(నిర్మాణం type_ *object) { \
    rxkb_object_ref(&object->పునాది); \
    తిరిగి object; \
} \
నిర్మాణం type_ * type_##_unref(నిర్మాణం type_ *object) { \
    ఒకవేళ (!object) తిరిగి ఖాళీ; \
    ప్రతిజ్ఞా(object->పునాది.refcount >= 1); \
    ఒకవేళ (--object->పునాది.refcount == 0) {\
        type_##_destroy(object); \
        list_remove(&object->పునాది.link);\
        విడుదల(object); \
    } \
    తిరిగి ఖాళీ;\
}

#నిర్వచించు DECLARE_CREATE_FOR_TYPE(type_) \
స్థిర అంతఃవరుస నిర్మాణం type_ * type_##_create(నిర్మాణం rxkb_object *మాతృ) { \
    నిర్మాణం type_ *t = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం *t); \
    ఒకవేళ (t) \
        rxkb_object_init(&t->పునాది, మాతృ); \
    తిరిగి t; \
}

#నిర్వచించు DECLARE_TYPED_GETTER_FOR_TYPE(type_, క్షేత్రం, rtype_) \
rtype_ type_##_get_##field_(నిర్మాణం type_ *object) { \
    తిరిగి object->క్షేత్రం; \
}

#నిర్వచించు DECLARE_GETTER_FOR_TYPE(type_, క్షేత్రం) \
   DECLARE_TYPED_GETTER_FOR_TYPE(type_, క్షేత్రం, స్థిరరాశి అక్షరం*)

#నిర్వచించు DECLARE_మొదటి_తదుపరి_FOR_TYPE(type_, మాతృ_type, మాతృ_క్షేత్రం) \
నిర్మాణం type_ * type_##_first(నిర్మాణం మాతృ_type *మాతృ) { \
    నిర్మాణం type_ *o = ఖాళీ; \
    ఒకవేళ (!list_ఖాళీలు(&మాతృ->మాతృ_క్షేత్రం)) \
        o = list_మొదటి_నమోదు(&మాతృ->మాతృ_క్షేత్రం, o, పునాది.link); \
    తిరిగి o; \
} \
నిర్మాణం type_ * \
type_##_next(నిర్మాణం type_ *o) \
{ \
    నిర్మాణం మాతృ_type *మాతృ; \
    నిర్మాణం type_ *తదుపరి; \
    మాతృ = container_యొక్క(o->పునాది.మాతృ, నిర్మాణం మాతృ_type, పునాది); \
    తదుపరి = list_మొదటి_నమోదు(&o->పునాది.link, o, పునాది.link); \
    ఒకవేళ (list_is_చివరి(&మాతృ->మాతృ_క్షేత్రం, &o->పునాది.link)) \
        తిరిగి ఖాళీ; \
    తిరిగి తదుపరి; \
}

స్థిర శూన్య
rxkb_object_init(నిర్మాణం rxkb_object *object, నిర్మాణం rxkb_object *మాతృ)
{
    object->refcount = 1;
    object->మాతృ = మాతృ;
    list_init(&object->link);
}

స్థిర శూన్య *
rxkb_object_ref(నిర్మాణం rxkb_object *object)
{
    ప్రతిజ్ఞా(object->refcount >= 1);
    ++object->refcount;
    తిరిగి object;
}

స్థిర శూన్య
rxkb_iso639_సంకేతం_destroy(నిర్మాణం rxkb_iso639_సంకేతం *సంకేతం_2)
{
    విడుదల(సంకేతం_2->సంకేతం_2);
}

నిర్మాణం rxkb_iso639_సంకేతం *
rxkb_layout_get_iso639_మొదటి(నిర్మాణం rxkb_layout *layout)
{
    నిర్మాణం rxkb_iso639_సంకేతం *సంకేతం_2 = ఖాళీ;

    ఒకవేళ (!list_ఖాళీలు(&layout->iso639s))
        సంకేతం_2 = list_మొదటి_నమోదు(&layout->iso639s, సంకేతం_2, పునాది.link);

    తిరిగి సంకేతం_2;
}

నిర్మాణం rxkb_iso639_సంకేతం *
rxkb_iso639_సంకేతం_తదుపరి(నిర్మాణం rxkb_iso639_సంకేతం *సంకేతం_2)
{
    నిర్మాణం rxkb_iso639_సంకేతం *తదుపరి = ఖాళీ;
    నిర్మాణం rxkb_layout *layout;

    layout = container_యొక్క(సంకేతం_2->పునాది.మాతృ, నిర్మాణం rxkb_layout, పునాది);

    ఒకవేళ (list_is_చివరి(&layout->iso639s, &సంకేతం_2->పునాది.link))
        తిరిగి ఖాళీ;

    తదుపరి = list_మొదటి_నమోదు(&సంకేతం_2->పునాది.link, సంకేతం_2, పునాది.link);

    తిరిగి తదుపరి;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso639_సంకేతం);
DECLARE_CREATE_FOR_TYPE(rxkb_iso639_సంకేతం);
DECLARE_GETTER_FOR_TYPE(rxkb_iso639_సంకేతం, సంకేతం_2);

స్థిర శూన్య
rxkb_iso3166_సంకేతం_destroy(నిర్మాణం rxkb_iso3166_సంకేతం *సంకేతం_2)
{
    విడుదల(సంకేతం_2->సంకేతం_2);
}

నిర్మాణం rxkb_iso3166_సంకేతం *
rxkb_layout_get_iso3166_మొదటి(నిర్మాణం rxkb_layout *layout)
{
    నిర్మాణం rxkb_iso3166_సంకేతం *సంకేతం_2 = ఖాళీ;

    ఒకవేళ (!list_ఖాళీలు(&layout->iso3166s))
        సంకేతం_2 = list_మొదటి_నమోదు(&layout->iso3166s, సంకేతం_2, పునాది.link);

    తిరిగి సంకేతం_2;
}

నిర్మాణం rxkb_iso3166_సంకేతం *
rxkb_iso3166_సంకేతం_తదుపరి(నిర్మాణం rxkb_iso3166_సంకేతం *సంకేతం_2)
{
    నిర్మాణం rxkb_iso3166_సంకేతం *తదుపరి = ఖాళీ;
    నిర్మాణం rxkb_layout *layout;

    layout = container_యొక్క(సంకేతం_2->పునాది.మాతృ, నిర్మాణం rxkb_layout, పునాది);

    ఒకవేళ (list_is_చివరి(&layout->iso3166s, &సంకేతం_2->పునాది.link))
        తిరిగి ఖాళీ;

    తదుపరి = list_మొదటి_నమోదు(&సంకేతం_2->పునాది.link, సంకేతం_2, పునాది.link);

    తిరిగి తదుపరి;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_iso3166_సంకేతం);
DECLARE_CREATE_FOR_TYPE(rxkb_iso3166_సంకేతం);
DECLARE_GETTER_FOR_TYPE(rxkb_iso3166_సంకేతం, సంకేతం_2);

స్థిర శూన్య
rxkb_option_destroy(నిర్మాణం rxkb_option *o)
{
    విడుదల(o->name);
    విడుదల(o->brief);
    విడుదల(o->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option);
DECLARE_CREATE_FOR_TYPE(rxkb_option);
DECLARE_GETTER_FOR_TYPE(rxkb_option, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_option, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option, popularity, గణన rxkb_popularity);
bool rxkb_option_is_layout_specific(నిర్మాణం rxkb_option *object) {
    తిరిగి object->layout_specific;
}
DECLARE_మొదటి_తదుపరి_FOR_TYPE(rxkb_option, rxkb_option_సమూహం, options);

స్థిర శూన్య
rxkb_layout_destroy(నిర్మాణం rxkb_layout *l)
{
    నిర్మాణం rxkb_iso639_సంకేతం *iso639, *tmp_639;
    నిర్మాణం rxkb_iso3166_సంకేతం *iso3166, *tmp_3166;

    విడుదల(l->name);
    విడుదల(l->brief);
    విడుదల(l->description);
    విడుదల(l->variant);

    list_for_each_సురక్షిత(iso639, tmp_639, &l->iso639s, పునాది.link) {
        rxkb_iso639_సంకేతం_unref(iso639);
    }
    list_for_each_సురక్షిత(iso3166, tmp_3166, &l->iso3166s, పునాది.link) {
        rxkb_iso3166_సంకేతం_unref(iso3166);
    }
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_layout);
DECLARE_CREATE_FOR_TYPE(rxkb_layout);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, name);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, brief);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, description);
DECLARE_GETTER_FOR_TYPE(rxkb_layout, variant);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_layout, popularity, గణన rxkb_popularity);
DECLARE_మొదటి_తదుపరి_FOR_TYPE(rxkb_layout, rxkb_context, layouts);

స్థిర శూన్య
rxkb_model_destroy(నిర్మాణం rxkb_model *m)
{
    విడుదల(m->name);
    విడుదల(m->vendor);
    విడుదల(m->description);
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_model);
DECLARE_CREATE_FOR_TYPE(rxkb_model);
DECLARE_GETTER_FOR_TYPE(rxkb_model, name);
DECLARE_GETTER_FOR_TYPE(rxkb_model, vendor);
DECLARE_GETTER_FOR_TYPE(rxkb_model, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_model, popularity, గణన rxkb_popularity);
DECLARE_మొదటి_తదుపరి_FOR_TYPE(rxkb_model, rxkb_context, models);

స్థిర శూన్య
rxkb_option_సమూహం_destroy(నిర్మాణం rxkb_option_సమూహం *og)
{
    నిర్మాణం rxkb_option *o, *otmp;

    విడుదల(og->name);
    విడుదల(og->description);

    list_for_each_సురక్షిత(o, otmp, &og->options, పునాది.link) {
        rxkb_option_unref(o);
    }
}

bool
rxkb_option_సమూహం_allows_గుణిజం(నిర్మాణం rxkb_option_సమూహం *g)
{
    తిరిగి g->allow_గుణిజం;
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_option_సమూహం);
DECLARE_CREATE_FOR_TYPE(rxkb_option_సమూహం);
DECLARE_GETTER_FOR_TYPE(rxkb_option_సమూహం, name);
DECLARE_GETTER_FOR_TYPE(rxkb_option_సమూహం, description);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_option_సమూహం, popularity, గణన rxkb_popularity);
DECLARE_మొదటి_తదుపరి_FOR_TYPE(rxkb_option_సమూహం, rxkb_context, option_groups);

స్థిర శూన్య
rxkb_context_destroy(నిర్మాణం rxkb_context *ctx)
{
    నిర్మాణం rxkb_model *m, *mtmp;
    నిర్మాణం rxkb_layout *l, *ltmp;
    నిర్మాణం rxkb_option_సమూహం *og, *ogtmp;
    అక్షరం **మార్గం;

    list_for_each_సురక్షిత(m, mtmp, &ctx->models, పునాది.link)
        rxkb_model_unref(m);
    ప్రతిజ్ఞా(list_ఖాళీలు(&ctx->models));

    list_for_each_సురక్షిత(l, ltmp, &ctx->layouts, పునాది.link)
        rxkb_layout_unref(l);
    ప్రతిజ్ఞా(list_ఖాళీలు(&ctx->layouts));

    list_for_each_సురక్షిత(og, ogtmp, &ctx->option_groups, పునాది.link)
        rxkb_option_సమూహం_unref(og);
    ప్రతిజ్ఞా(list_ఖాళీలు(&ctx->option_groups));

    darray_foreach(మార్గం, ctx->includes)
        విడుదల(*మార్గం);
    darray_free(ctx->includes);

    ప్రతిజ్ఞా(darray_ఖాళీలు(ctx->includes));
}

DECLARE_REF_UNREF_FOR_TYPE(rxkb_context);
DECLARE_CREATE_FOR_TYPE(rxkb_context);
DECLARE_TYPED_GETTER_FOR_TYPE(rxkb_context, log_level, గణన rxkb_log_level);

స్థిర అక్షరం *
rxkb_context_getenv(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *name)
{
    ఒకవేళ (ctx->use_secure_getenv) {
        తిరిగి secure_getenv(name);
    } లేకపోతే {
        తిరిగి getenv(name);
    }
}


శూన్య
rxkb_context_సమితి_log_level(నిర్మాణం rxkb_context *ctx,
                           గణన rxkb_log_level level)
{
    ctx->log_level = level;
}

స్థిర స్థిరరాశి అక్షరం *
log_level_గమ్యం_ఉపసర్గ(గణన rxkb_log_level level)
{
    ఎంపిక (level) {
    స్థితి RXKB_LOG_LEVEL_DEBUG:
        తిరిగి "xkbregistry: DEBUG: ";
    స్థితి RXKB_LOG_LEVEL_INFO:
        తిరిగి "xkbregistry: INFO: ";
    స్థితి RXKB_LOG_LEVEL_WARNING:
        తిరిగి "xkbregistry: WARNING: ";
    స్థితి RXKB_LOG_LEVEL_ERROR:
        తిరిగి "xkbregistry: ERROR: ";
    స్థితి RXKB_LOG_LEVEL_CRITICAL:
        తిరిగి "xkbregistry: CRITICAL: ";
    అప్రమేయ:
        తిరిగి ఖాళీ;
    }
}

ATTR_PRINTF(3, 0) స్థిర శూన్య
default_log_fn(నిర్మాణం rxkb_context *ctx, గణన rxkb_log_level level,
               స్థిరరాశి అక్షరం *fmt, va_list వాదనలు)
{
    స్థిరరాశి అక్షరం *ఉపసర్గ = log_level_గమ్యం_ఉపసర్గ(level);

    ఒకవేళ (ఉపసర్గ)
        fprintf(stderr, "%s", ఉపసర్గ);
    vfprintf(stderr, fmt, వాదనలు);
}

స్థిర గణన rxkb_log_level
log_level(స్థిరరాశి అక్షరం *level) {
    errno = 0;
    అక్షరం *endptr;
    స్థిరరాశి దీర్ఘ lvl = strtol(level, &endptr, 10);
    ఒకవేళ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        తిరిగి (గణన rxkb_log_level)lvl;

    ఒకవేళ (istreq_ఉపసర్గ("crit", level))
        తిరిగి RXKB_LOG_LEVEL_CRITICAL;
    ఒకవేళ (istreq_ఉపసర్గ("err", level))
        తిరిగి RXKB_LOG_LEVEL_ERROR;
    ఒకవేళ (istreq_ఉపసర్గ("warn", level))
        తిరిగి RXKB_LOG_LEVEL_WARNING;
    ఒకవేళ (istreq_ఉపసర్గ("info", level))
        తిరిగి RXKB_LOG_LEVEL_INFO;
    ఒకవేళ (istreq_ఉపసర్గ("debug", level) || istreq_ఉపసర్గ("dbg", level))
        తిరిగి RXKB_LOG_LEVEL_DEBUG;

    తిరిగి RXKB_LOG_LEVEL_ERROR;
}

నిర్మాణం rxkb_context *
rxkb_context_new(గణన rxkb_context_flags flags)
{
    నిర్మాణం rxkb_context *ctx = rxkb_context_create(ఖాళీ);
    స్థిరరాశి అక్షరం *env;

    ఒకవేళ (!ctx)
        తిరిగి ఖాళీ;

    ctx->context_state = CONTEXT_NEW;
    ctx->ఎక్కించు_extra_నియమాలు_files = flags & RXKB_CONTEXT_ఎక్కించు_EXOTIC_నియమాలు;
    ctx->use_secure_getenv = !(flags & RXKB_CONTEXT_NO_SECURE_GETENV);
    ctx->log_fn = default_log_fn;
    ctx->log_level = RXKB_LOG_LEVEL_ERROR;

    /* Environment overwrites defaults. */
    env = rxkb_context_getenv(ctx, "RXKB_LOG_LEVEL");
    ఒకవేళ (env)
        rxkb_context_సమితి_log_level(ctx, log_level(env));

    స్థిర స్థిరరాశి గణన rxkb_context_flags  RXKB_CONTEXT_FLAGS
        = RXKB_CONTEXT_NO_DEFAULT_INCLUDES
        | RXKB_CONTEXT_ఎక్కించు_EXOTIC_నియమాలు
        | RXKB_CONTEXT_NO_SECURE_GETENV;

    ఒకవేళ (flags & ~RXKB_CONTEXT_FLAGS) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "%s: Invalid context flags: 0x%x\n", __func__,
                (flags & ~RXKB_CONTEXT_FLAGS));
        విడుదల(ctx);
        తిరిగి ఖాళీ;
    }

    list_init(&ctx->models);
    list_init(&ctx->layouts);
    list_init(&ctx->option_groups);

    ఒకవేళ (!(flags & RXKB_CONTEXT_NO_DEFAULT_INCLUDES) &&
        !rxkb_context_include_మార్గం_append_default(ctx)) {
        log_err(ctx, XKB_ERROR_NO_VALID_DEFAULT_INCLUDE_మార్గం,
                "Failed to add any default include path "
                "(default system path: %s)\n",
                DFLT_XKB_CONFIG_మూలం);
        rxkb_context_unref(ctx);
        తిరిగి ఖాళీ;
    }

    తిరిగి ctx;
}

శూన్య
rxkb_context_సమితి_log_fn(నిర్మాణం rxkb_context *ctx,
                        శూన్య (*log_fn)(నిర్మాణం rxkb_context *ctx,
                                       గణన rxkb_log_level level,
                                       స్థిరరాశి అక్షరం *fmt, va_list వాదనలు))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

bool
rxkb_context_include_మార్గం_append(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం)
{
    పూర్ణాంకం err = 0;

    ఒకవేళ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "include paths can only be appended to a new context\n");
        వెళ్ళు error;
    }

    నిర్మాణం stat stat_buf;
    err = stat(మార్గం, &stat_buf);
    ఒకవేళ (err != 0) {
        err = errno;
        వెళ్ళు error;
    }
    ఒకవేళ (!S_ISDIR(stat_buf.st_బహుళకం)) {
        err = ENOTDIR;
        వెళ్ళు error;
    }

    ఒకవేళ (!check_eaccess(మార్గం, R_OK | X_OK)) {
        err = EACCES;
        వెళ్ళు error;
    }

    /*
     * Pre-filter for the 99.9% case - if we can’t assemble the default ruleset
     * path, complain here instead of during parsing later. The niche cases
     * where this is the wrong behaviour aren’t worth worrying about.
     */
    అక్షరం నియమాలు[PATH_MAX];
    ఒకవేళ (!snprintf_సురక్షిత(నియమాలు, పరిమాణం(నియమాలు), "%s/rules/%s.xml",
                       మార్గం, DEFAULT_XKB_నియమాలు)) {
        log_err(ctx, XKB_ERROR_INVALID_మార్గం,
                "Path is too long: expected max length of %zu, "
                "got: %s/rules/%s.xml\n",
                పరిమాణం(నియమాలు), మార్గం, DEFAULT_XKB_నియమాలు);
        వెళ్ళు error;
    }

    అక్షరం * స్థిరరాశి tmp = strdup(మార్గం);
    ఒకవేళ (!tmp) {
        err = ENOMEM;
        వెళ్ళు error;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_సందేశం_NO_ID, "Include path added: %s\n", tmp);

    తిరిగి true;

error:
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_సందేశం_NO_ID,
             "Include path failed: \"%s\" (%s)\n", మార్గం, strerror(err));
    తిరిగి false;
}

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES

స్థిర పూర్ణాంకం
compare_str(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    తిరిగి strcmp(*(అక్షరం **)a, *(అక్షరం **) b);
}

స్థిర పూర్ణాంకం
కూడు_ప్రత్యక్ష_subdirectories(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం,
                          darray_string *extensions,
                          darray_కొలత_t versioned_లెక్క,
                          కొలత_t versioned_మార్గం_నిడివి)
{
    పూర్ణాంకం ret = 0;
    పూర్ణాంకం err = ENOMEM;
    DIR *dir = ఖాళీ;

    /* Check extensions parent directory */
    నిర్మాణం stat stat_buf;
    err = stat(మార్గం, &stat_buf);
    ఒకవేళ (err != 0) {
        err = errno;
        వెళ్ళు err;
    }
    ఒకవేళ (!S_ISDIR(stat_buf.st_బహుళకం)) {
        err = ENOTDIR;
        వెళ్ళు err;
    }
    ఒకవేళ (!check_eaccess(మార్గం, R_OK | X_OK)) {
        err = EACCES;
        వెళ్ళు err;
    }

    dir = opendir(మార్గం);
    ఒకవేళ (dir == ఖాళీ) {
        err = EACCES;
        వెళ్ళు err;
    }

    నిర్మాణం dirent *నమోదు_2;
    అక్షరం మార్గం_buf[PATH_MAX] = "";
    versioned_మార్గం_నిడివి++; /* Additional final ‘/’ */
    అయితే ((నమోదు_2 = readdir(dir)) != ఖాళీ) {
        స్థిరరాశి అక్షరం *name = నమోదు_2->d_name;

        /* Skip special entries */
        ఒకవేళ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            కొనసాగు;

        ఒకవేళ (!snprintf_సురక్షిత(మార్గం_buf, పరిమాణం(మార్గం_buf), "%s/%s", మార్గం, name)) {
            err = ENOMEM;
            వెళ్ళు err;
        }
        ఒకవేళ (stat(మార్గం_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_బహుళకం))
            కొనసాగు;

        /* Skip if the corresponding versioned directory is already included */
        కొరకు (darray_కొలత_t i = 0; i < versioned_లెక్క; i++) {
            స్థిరరాశి అక్షరం * స్థిరరాశి prev_name = darray_అంశం(*extensions, i)
                                         + versioned_మార్గం_నిడివి;
            ఒకవేళ (strcmp(name, prev_name) == 0)
                వెళ్ళు తదుపరి;
        }

        అక్షరం *ext_మార్గం = strdup_సురక్షిత(మార్గం_buf);
        ఒకవేళ (!ext_మార్గం) {
            err = ENOMEM;
            వెళ్ళు err;
        }

        darray_append(*extensions, ext_మార్గం);
తదుపరి:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ఒకవేళ (darray_కొలత(*extensions) > versioned_లెక్క) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_వస్తువులు(*extensions) + versioned_లెక్క,
              darray_కొలత(*extensions) - versioned_లెక్క,
              పరిమాణం(*darray_వస్తువులు(*extensions)), &compare_str);
        అక్షరం **ext_మార్గం;
        darray_foreach_from(ext_మార్గం, *extensions, versioned_లెక్క) {
            ret |= rxkb_context_include_మార్గం_append(ctx, *ext_మార్గం);
        }
    }

    తిరిగి ret;

err:
    log_dbg(ctx,
            "Include extensions path failed: %s (%s)\n", మార్గం, strerror(err));
    ఒకవేళ (dir)
        closedir(dir);

    తిరిగి ret;
}
#అయితే_ముగింపు

bool
rxkb_context_include_మార్గం_append_default(నిర్మాణం rxkb_context *ctx)
{
    అక్షరం user_మార్గం[PATH_MAX];
    bool ret = false;

    ఒకవేళ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "include paths can only be appended to a new context\n");
        తిరిగి false;
    }

    స్థిరరాశి అక్షరం * స్థిరరాశి home = rxkb_context_getenv(ctx, "HOME");
    స్థిరరాశి అక్షరం * స్థిరరాశి xdg = rxkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ఒకవేళ (xdg != ఖాళీ) {
        ఒకవేళ (snprintf_సురక్షిత(user_మార్గం, పరిమాణం(user_మార్గం), "%s/xkb", xdg))
            ret |= rxkb_context_include_మార్గం_append(ctx, user_మార్గం);
    } లేకపోతే ఒకవేళ (home != ఖాళీ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        ఒకవేళ (snprintf_సురక్షిత(user_మార్గం, పరిమాణం(user_మార్గం), "%s/.config/xkb", home))
            ret |= rxkb_context_include_మార్గం_append(ctx, user_మార్గం);
    }

    ఒకవేళ (home != ఖాళీ) {
        ఒకవేళ (snprintf_సురక్షిత(user_మార్గం, పరిమాణం(user_మార్గం), "%s/.xkb", home))
            ret |= rxkb_context_include_మార్గం_append(ctx, user_మార్గం);
    }

    స్థిరరాశి అక్షరం * స్థిరరాశి extra = rxkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ret |= rxkb_context_include_మార్గం_append(
        ctx, ((extra != ఖాళీ) ? extra : DFLT_XKB_CONFIG_EXTRA_మార్గం)
    );

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    స్థిరరాశి అక్షరం *extensions_మార్గం =
        rxkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
#నిర్వచితమైతే DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_మార్గం
    ఒకవేళ (extensions_మార్గం == ఖాళీ)
        extensions_మార్గం = DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_మార్గం;
#అయితే_ముగింపు
    కొలత_t versioned_మార్గం_నిడివి = 0;
    ఒకవేళ (extensions_మార్గం != ఖాళీ) {
        /* Add direct subdirectories */
        ret |= కూడు_ప్రత్యక్ష_subdirectories(ctx, extensions_మార్గం,
                                         &extensions, 0, 0);
        versioned_మార్గం_నిడివి = అక్షరశ్రేణి_పొడవు(extensions_మార్గం);
    }

    /* Unversioned extensions directory */
    extensions_మార్గం =
        rxkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
#నిర్వచితమైతే DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_మార్గం
    ఒకవేళ (extensions_మార్గం == ఖాళీ)
        extensions_మార్గం = DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_మార్గం;
#అయితే_ముగింపు
    ఒకవేళ (extensions_మార్గం != ఖాళీ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= కూడు_ప్రత్యక్ష_subdirectories(ctx, extensions_మార్గం, &extensions,
                                         darray_కొలత(extensions),
                                         versioned_మార్గం_నిడివి);
    }

    అక్షరం **ext_మార్గం;
    darray_foreach(ext_మార్గం, extensions) {
        విడుదల(*ext_మార్గం);
    }
    darray_free(extensions);
#అయితే_ముగింపు

    /* Canonical XKB root */
    స్థిరరాశి అక్షరం * స్థిరరాశి మూలం = rxkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    స్థిరరాశి bool has_మూలం = rxkb_context_include_మార్గం_append(
        ctx, ((మూలం != ఖాళీ) ? మూలం: DFLT_XKB_CONFIG_మూలం)
    );
    ret |= has_మూలం;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ఒకవేళ (!has_మూలం && (మూలం == ఖాళీ || మూలం[0] != '\0')) {
        log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_మూలం,
                 ((మూలం == ఖాళీ) ? DFLT_XKB_CONFIG_మూలం : మూలం));
        ret |= rxkb_context_include_మార్గం_append(ctx, DFLT_XKB_LEGACY_మూలం);
    }

    తిరిగి ret;
}

bool
rxkb_context_parse_default_ruleset(నిర్మాణం rxkb_context *ctx)
{
    తిరిగి rxkb_context_parse(ctx, DEFAULT_XKB_నియమాలు);
}

bool
rxkb_context_parse(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *ruleset)
{
    అక్షరం **మార్గం;
    bool success = false;

    ఒకవేళ (ctx->context_state != CONTEXT_NEW) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "parse must only be called on a new context\n");
        తిరిగి false;
    }

    darray_foreach_తిరగేయి(మార్గం, ctx->includes) {
        అక్షరం నియమాలు[PATH_MAX];

        ఒకవేళ (snprintf_సురక్షిత(నియమాలు, పరిమాణం(నియమాలు), "%s/rules/%s.xml",
                           *మార్గం, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", నియమాలు);
            ఒకవేళ (parse(ctx, నియమాలు, RXKB_POPULARITY_STANDARD))
                success = true;
        }

        ఒకవేళ (ctx->ఎక్కించు_extra_నియమాలు_files &&
            snprintf_సురక్షిత(నియమాలు, పరిమాణం(నియమాలు), "%s/rules/%s.extras.xml",
                          *మార్గం, ruleset)) {
            log_dbg(ctx, "Parsing %s\n", నియమాలు);
            ఒకవేళ (parse(ctx, నియమాలు, RXKB_POPULARITY_EXOTIC))
                success = true;
        }
    }

    ctx->context_state = success ? CONTEXT_PARSED : CONTEXT_FAILED;

    తిరిగి success;
}


శూన్య
rxkb_context_సమితి_user_దత్తాంశం(నిర్మాణం rxkb_context *ctx, శూన్య *userdata)
{
    ctx->userdata = userdata;
}

శూన్య *
rxkb_context_get_user_దత్తాంశం(నిర్మాణం rxkb_context *ctx)
{
    తిరిగి ctx->userdata;
}

స్థిర అంతఃవరుస bool
is_కణుపు(xml_కణుపు *కణుపు, స్థిరరాశి అక్షరం *name)
{
    తిరిగి కణుపు->type == XML_ELEMENT_కణుపు &&
        xmlStrEqual(కణుపు->name, (స్థిరరాశి xmlChar*)name);
}

/* return a copy of the text content from the first text node of this node */
స్థిర అక్షరం *
వెలికితీయి_పాఠ్యం(xml_కణుపు *కణుపు)
{
    xml_కణుపు *n;

    కొరకు (n = కణుపు->children; n; n = n->తదుపరి) {
        ఒకవేళ (n->type == XML_పాఠ్యం_కణుపు)
            తిరిగి (అక్షరం *)xmlStrdup(n->content);
    }
    తిరిగి ఖాళీ;
}

/* Data from “configItem” node */
నిర్మాణం config_అంశం {
    అక్షరం *name;
    అక్షరం *description;
    అక్షరం *brief;
    అక్షరం *vendor;
    గణన rxkb_popularity popularity;
    bool layout_specific;
};

#నిర్వచించు config_అంశం_new(popularity_) { \
    .name = ఖాళీ, \
    .description = ఖాళీ, \
    .brief = ఖాళీ, \
    .vendor = ఖాళీ, \
    .popularity = (popularity_), \
    .layout_specific = false \
}

స్థిర శూన్య
config_అంశం_free(నిర్మాణం config_అంశం *config) {
    విడుదల(config->name);
    విడుదల(config->description);
    విడుదల(config->brief);
    విడుదల(config->vendor);
}

స్థిర bool
parse_config_అంశం(నిర్మాణం rxkb_context *ctx, xml_కణుపు *మాతృ,
                  నిర్మాణం config_అంశం *config)
{
    xml_కణుపు *కణుపు = ఖాళీ;
    xml_కణుపు *ci = ఖాళీ;

    కొరకు (ci = మాతృ->children; ci; ci = ci->తదుపరి) {
        ఒకవేళ (is_కణుపు(ci, "configItem")) {
            /* Process attributes */
            xmlChar *raw_popularity = xmlGetProp(ci, (స్థిరరాశి xmlChar*)"popularity");
            ఒకవేళ (raw_popularity) {
                ఒకవేళ (xmlStrEqual(raw_popularity, (స్థిరరాశి xmlChar*)"standard"))
                    config->popularity = RXKB_POPULARITY_STANDARD;
                లేకపోతే ఒకవేళ (xmlStrEqual(raw_popularity, (స్థిరరాశి xmlChar*)"exotic"))
                    config->popularity = RXKB_POPULARITY_EXOTIC;
                లేకపోతే
                    log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                            "xml:%u: invalid popularity attribute: expected "
                            "'standard' or 'exotic', got: '%s'\n",
                            ci->పంక్తి, raw_popularity);
            }
            xmlFree(raw_popularity);

            /* Note: this is only useful for options */
            xmlChar *raw_layout_specific =
                xmlGetProp(ci, (స్థిరరాశి xmlChar*)"layout-specific");
            ఒకవేళ (raw_layout_specific &&
                xmlStrEqual(raw_layout_specific, (స్థిరరాశి xmlChar*)"true"))
                config->layout_specific = true;
            xmlFree(raw_layout_specific);

            /* Process children */
            కొరకు (కణుపు = ci->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
                ఒకవేళ (is_కణుపు(కణుపు, "name"))
                    config->name = వెలికితీయి_పాఠ్యం(కణుపు);
                లేకపోతే ఒకవేళ (is_కణుపు(కణుపు, "description"))
                    config->description = వెలికితీయి_పాఠ్యం(కణుపు);
                లేకపోతే ఒకవేళ (is_కణుపు(కణుపు, "shortDescription"))
                    config->brief = వెలికితీయి_పాఠ్యం(కణుపు);
                లేకపోతే ఒకవేళ (is_కణుపు(కణుపు, "vendor"))
                    config->vendor = వెలికితీయి_పాఠ్యం(కణుపు);
                /* Note: the DTD allows for vendor + brief but models only use
                 * vendor and everything else only uses shortDescription */
            }

            ఒకవేళ (!config->name || !అక్షరశ్రేణి_పొడవు(config->name))  {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "xml:%u: missing required element 'name'\n",
                        ci->పంక్తి);
                config_అంశం_free(config);
                తిరిగి false;
            }

            తిరిగి true; /* only one configItem allowed in the dtd */
        }
    }

    తిరిగి false;
}

స్థిర శూన్య
parse_model(నిర్మాణం rxkb_context *ctx, xml_కణుపు *model,
            గణన rxkb_popularity popularity)
{
    నిర్మాణం config_అంశం config = config_అంశం_new(popularity);

    ఒకవేళ (parse_config_అంశం(ctx, model, &config)) {
        నిర్మాణం rxkb_model *m;

        list_for_each(m, &ctx->models, పునాది.link) {
            ఒకవేళ (streq(m->name, config.name)) {
                config_అంశం_free(&config);
                తిరిగి;
            }
        }

        /* new model */
        m = rxkb_model_create(&ctx->పునాది);
        m->name = steal(&config.name);
        m->description = steal(&config.description);
        m->vendor = steal(&config.vendor);
        m->popularity = config.popularity;
        list_append(&ctx->models, &m->పునాది.link);
    }
}

స్థిర శూన్య
parse_model_list(నిర్మాణం rxkb_context *ctx, xml_కణుపు *model_list,
                 గణన rxkb_popularity popularity)
{
    xml_కణుపు *కణుపు = ఖాళీ;

    కొరకు (కణుపు = model_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "model"))
            parse_model(ctx, కణుపు, popularity);
    }
}

స్థిర శూన్య
parse_language_list(xml_కణుపు *language_list, నిర్మాణం rxkb_layout *layout)
{
    xml_కణుపు *కణుపు = ఖాళీ;
    నిర్మాణం rxkb_iso639_సంకేతం *సంకేతం_2;

    కొరకు (కణుపు = language_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "iso639Id")) {
            అక్షరం *str = వెలికితీయి_పాఠ్యం(కణుపు);
            నిర్మాణం rxkb_object *మాతృ;

            ఒకవేళ (!str || అక్షరశ్రేణి_పొడవు(str) != 3) {
                విడుదల(str);
                కొనసాగు;
            }

            మాతృ = &layout->పునాది;
            సంకేతం_2 = rxkb_iso639_సంకేతం_create(మాతృ);
            సంకేతం_2->సంకేతం_2 = str;
            list_append(&layout->iso639s, &సంకేతం_2->పునాది.link);
        }
    }
}

స్థిర శూన్య
parse_country_list(xml_కణుపు *country_list, నిర్మాణం rxkb_layout *layout)
{
    xml_కణుపు *కణుపు = ఖాళీ;
    నిర్మాణం rxkb_iso3166_సంకేతం *సంకేతం_2;

    కొరకు (కణుపు = country_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "iso3166Id")) {
            అక్షరం *str = వెలికితీయి_పాఠ్యం(కణుపు);
            నిర్మాణం rxkb_object *మాతృ;

            ఒకవేళ (!str || అక్షరశ్రేణి_పొడవు(str) != 2) {
                విడుదల(str);
                కొనసాగు;
            }

            మాతృ = &layout->పునాది;
            సంకేతం_2 = rxkb_iso3166_సంకేతం_create(మాతృ);
            సంకేతం_2->సంకేతం_2 = str;
            list_append(&layout->iso3166s, &సంకేతం_2->పునాది.link);
        }
    }
}

స్థిర శూన్య
parse_variant(నిర్మాణం rxkb_context *ctx, నిర్మాణం rxkb_layout *l,
              xml_కణుపు *variant, గణన rxkb_popularity popularity)
{
    xml_కణుపు *ci;
    నిర్మాణం config_అంశం config = config_అంశం_new(popularity);

    ఒకవేళ (parse_config_అంశం(ctx, variant, &config)) {
        నిర్మాణం rxkb_layout *v;
        bool exists = false;

        list_for_each(v, &ctx->layouts, పునాది.link) {
            ఒకవేళ (streq_null(v->variant, config.name) &&
                streq(v->name, l->name)) {
                exists = true;
                ఆపు;
            }
        }

        ఒకవేళ (!exists) {
            v = rxkb_layout_create(&ctx->పునాది);
            list_init(&v->iso639s);
            list_init(&v->iso3166s);
            v->name = strdup(l->name);
            v->variant = steal(&config.name);
            v->description = steal(&config.description);
            // if variant omits brief, inherit from parent layout.
            v->brief = config.brief == ఖాళీ ? strdup_సురక్షిత(l->brief) : steal(&config.brief);
            v->popularity = config.popularity;
            list_append(&ctx->layouts, &v->పునాది.link);

            కొరకు (ci = variant->children; ci; ci = ci->తదుపరి) {
                xml_కణుపు *కణుపు;

                ఒకవేళ (!is_కణుపు(ci, "configItem"))
                    కొనసాగు;

                bool దొరికింది_language_list = false;
                bool దొరికింది_country_list = false;
                కొరకు (కణుపు = ci->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
                    ఒకవేళ (is_కణుపు(కణుపు, "languageList")) {
                        parse_language_list(కణుపు, v);
                        దొరికింది_language_list = true;
                    }
                    ఒకవేళ (is_కణుపు(కణుపు, "countryList")) {
                        parse_country_list(కణుపు, v);
                        దొరికింది_country_list = true;
                    }
                }
                ఒకవేళ (!దొరికింది_language_list) {
                    // inherit from parent layout
                    నిర్మాణం rxkb_iso639_సంకేతం* x;
                    list_for_each(x, &l->iso639s, పునాది.link) {
                        నిర్మాణం rxkb_iso639_సంకేతం* సంకేతం_2 = rxkb_iso639_సంకేతం_create(&v->పునాది);
                        సంకేతం_2->సంకేతం_2 = strdup(x->సంకేతం_2);
                        list_append(&v->iso639s, &సంకేతం_2->పునాది.link);
                    }
                }
                ఒకవేళ (!దొరికింది_country_list) {
                    // inherit from parent layout
                    నిర్మాణం rxkb_iso3166_సంకేతం* x;
                    list_for_each(x, &l->iso3166s, పునాది.link) {
                        నిర్మాణం rxkb_iso3166_సంకేతం* సంకేతం_2 = rxkb_iso3166_సంకేతం_create(&v->పునాది);
                        సంకేతం_2->సంకేతం_2 = strdup(x->సంకేతం_2);
                        list_append(&v->iso3166s, &సంకేతం_2->పునాది.link);
                    }
                }
            }
        } లేకపోతే {
            config_అంశం_free(&config);
        }
    }
}

స్థిర శూన్య
parse_variant_list(నిర్మాణం rxkb_context *ctx, నిర్మాణం rxkb_layout *l,
                   xml_కణుపు *variant_list, గణన rxkb_popularity popularity)
{
    xml_కణుపు *కణుపు = ఖాళీ;

    కొరకు (కణుపు = variant_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "variant"))
            parse_variant(ctx, l, కణుపు, popularity);
    }
}

స్థిర శూన్య
parse_layout(నిర్మాణం rxkb_context *ctx, xml_కణుపు *layout,
             గణన rxkb_popularity popularity)
{
    నిర్మాణం config_అంశం config = config_అంశం_new(popularity);
    నిర్మాణం rxkb_layout *l;
    xml_కణుపు *కణుపు = ఖాళీ;
    bool exists = false;

    ఒకవేళ (!parse_config_అంశం(ctx, layout, &config))
        తిరిగి;

    list_for_each(l, &ctx->layouts, పునాది.link) {
        ఒకవేళ (streq(l->name, config.name) && l->variant == ఖాళీ) {
            exists = true;
            ఆపు;
        }
    }

    ఒకవేళ (!exists) {
        l = rxkb_layout_create(&ctx->పునాది);
        list_init(&l->iso639s);
        list_init(&l->iso3166s);
        l->name = steal(&config.name);
        l->variant = ఖాళీ;
        l->description = steal(&config.description);
        l->brief = steal(&config.brief);
        l->popularity = config.popularity;
        list_append(&ctx->layouts, &l->పునాది.link);
    } లేకపోతే {
        config_అంశం_free(&config);
    }

    కొరకు (కణుపు = layout->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "variantList")) {
            parse_variant_list(ctx, l, కణుపు, popularity);
        }
        ఒకవేళ (!exists && is_కణుపు(కణుపు, "configItem")) {
            xml_కణుపు *ll;
            కొరకు (ll = కణుపు->children; ll; ll = ll->తదుపరి) {
                ఒకవేళ (is_కణుపు(ll, "languageList"))
                    parse_language_list(ll, l);
                ఒకవేళ (is_కణుపు(ll, "countryList"))
                    parse_country_list(ll, l);
            }
        }
    }
}

స్థిర శూన్య
parse_layout_list(నిర్మాణం rxkb_context *ctx, xml_కణుపు *layout_list,
                  గణన rxkb_popularity popularity)
{
    xml_కణుపు *కణుపు = ఖాళీ;

    కొరకు (కణుపు = layout_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "layout"))
            parse_layout(ctx, కణుపు, popularity);
    }
}

స్థిర శూన్య
parse_option(నిర్మాణం rxkb_context *ctx, నిర్మాణం rxkb_option_సమూహం *సమూహం,
             xml_కణుపు *option, గణన rxkb_popularity popularity)
{
    నిర్మాణం config_అంశం config = config_అంశం_new(popularity);

    ఒకవేళ (parse_config_అంశం(ctx, option, &config)) {
        నిర్మాణం rxkb_option *o;

        list_for_each(o, &సమూహం->options, పునాది.link) {
            ఒకవేళ (streq(o->name, config.name)) {
                config_అంశం_free(&config);
                తిరిగి;
            }
        }

        o = rxkb_option_create(&సమూహం->పునాది);
        o->name = steal(&config.name);
        o->description = steal(&config.description);
        o->popularity = config.popularity;
        o->layout_specific = config.layout_specific;
        list_append(&సమూహం->options, &o->పునాది.link);
    }
}

స్థిర శూన్య
parse_సమూహం(నిర్మాణం rxkb_context *ctx, xml_కణుపు *సమూహం,
            గణన rxkb_popularity popularity)
{
    నిర్మాణం config_అంశం config = config_అంశం_new(popularity);
    నిర్మాణం rxkb_option_సమూహం *g;
    xml_కణుపు *కణుపు = ఖాళీ;
    xmlChar *గుణిజం;
    bool exists = false;

    ఒకవేళ (!parse_config_అంశం(ctx, సమూహం, &config))
        తిరిగి;

    list_for_each(g, &ctx->option_groups, పునాది.link) {
        ఒకవేళ (streq(g->name, config.name)) {
            exists = true;
            ఆపు;
        }
    }

    ఒకవేళ (!exists) {
        g = rxkb_option_సమూహం_create(&ctx->పునాది);
        g->name = steal(&config.name);
        g->description = steal(&config.description);
        g->popularity = config.popularity;

        గుణిజం = xmlGetProp(సమూహం, (స్థిరరాశి xmlChar*)"allowMultipleSelection");
        ఒకవేళ (గుణిజం && xmlStrEqual(గుణిజం, (స్థిరరాశి xmlChar*)"true"))
            g->allow_గుణిజం = true;
        xmlFree(గుణిజం);

        list_init(&g->options);
        list_append(&ctx->option_groups, &g->పునాది.link);
    } లేకపోతే {
        config_అంశం_free(&config);
    }

    కొరకు (కణుపు = సమూహం->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "option"))
            parse_option(ctx, g, కణుపు, popularity);
    }
}

స్థిర శూన్య
parse_option_list(నిర్మాణం rxkb_context *ctx, xml_కణుపు *option_list,
                  గణన rxkb_popularity popularity)
{
    xml_కణుపు *కణుపు = ఖాళీ;

    కొరకు (కణుపు = option_list->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "group"))
            parse_సమూహం(ctx, కణుపు, popularity);
    }
}

స్థిర శూన్య
parse_నియమాలు_xml(నిర్మాణం rxkb_context *ctx, xml_కణుపు *మూలం,
                గణన rxkb_popularity popularity)
{
    xml_కణుపు *కణుపు = ఖాళీ;

    కొరకు (కణుపు = మూలం->children; కణుపు; కణుపు = కణుపు->తదుపరి) {
        ఒకవేళ (is_కణుపు(కణుపు, "modelList"))
            parse_model_list(ctx, కణుపు, popularity);
        లేకపోతే ఒకవేళ (is_కణుపు(కణుపు, "layoutList"))
            parse_layout_list(ctx, కణుపు, popularity);
        లేకపోతే ఒకవేళ (is_కణుపు(కణుపు, "optionList"))
            parse_option_list(ctx, కణుపు, popularity);
    }
}

స్థిర శూన్య
ATTR_PRINTF(2, 0)
xml_error_func(శూన్య *ctx, స్థిరరాశి అక్షరం *సందేశం, ...)
{
    స్థిర అక్షరం buf[PATH_MAX];
    స్థిర కొలత_t slen = 0;
    va_list వాదనలు;
    పూర్ణాంకం rc;

    /* libxml2 prints IO errors from bad includes paths by
     * calling the error function once per word. So we get to
     * re-assemble the message here and print it when we get
     * the line break. My enthusiasm about this is indescribable.
     */
    va_ప్రారంభం(వాదనలు, సందేశం);
    rc = vsnprintf(&buf[slen], పరిమాణం(buf) - slen, సందేశం, వాదనలు);
    va_చివర(వాదనలు);

    /* This shouldn't really happen */
    ఒకవేళ (rc < 0) {
        log_err(ctx, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                "+++ out of cheese error. redo from start +++\n");
        slen = 0;
        memset(buf, 0, పరిమాణం(buf));
        తిరిగి;
    }

    slen += (కొలత_t)rc;
    ఒకవేళ (slen >= పరిమాణం(buf)) {
        /* truncated, let's flush this */
        buf[పరిమాణం(buf) - 1] = '\n';
        slen = పరిమాణం(buf);
    }

    /* We're assuming here that the last character is \n. */
    ఒకవేళ (buf[slen - 1] == '\n') {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID, "%s", buf);
        memset(buf, 0, పరిమాణం(buf));
        slen = 0;
    }
}

#ఒకవేళ HAVE_XML_CTXT_సమితి_ERRORHANDLER
స్థిర శూన్య
xml_structured_error_func(శూన్య *user_దత్తాంశం_2, స్థిరరాశి xmlError * error)
{
    xmlFormatError(error, xml_error_func, user_దత్తాంశం_2);
}
#అయితే_ముగింపు

#నిర్వచితమైతే XML_PARSE_NO_XXE
#నిర్వచించు _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#లేకపోతే
#నిర్వచించు _XML_OPTIONS (XML_PARSE_NONET)
#అయితే_ముగింపు

స్థిర bool
validate(నిర్మాణం rxkb_context *ctx, xmlDoc *doc)
{
    bool success = false;
    /* This is a modified version of the xkeyboard-config xkb.dtd:
     * • xkeyboard-config requires modelList, layoutList and optionList,
     *   but we allow for any of those to be missing.
     * • xkeyboard-config sets default value of “popularity” to “standard”,
     *   but we set this value depending if we are currently parsing an
     *   “extras” rule file.
     */
    స్థిరరాశి అక్షరం dtdstr[] =
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

#ఒకవేళ HAVE_XML_CTXT_PARSE_DTD
    /* Use safer function with context if available, and set
     * the contextual error handler. */
    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ఒకవేళ (!xmlCtxt)
        తిరిగి false;
    static_assert(HAVE_XML_CTXT_సమితి_ERRORHANDLER,
                  "xmlCtxtSetErrorHandler (2.13) introduced before "
                  "xmlCtxtParseDtd (2.14)");
    xml_Ctxt_సమితి_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
    xml_Ctxt_సమితి_Options(xmlCtxt, _XML_OPTIONS | XML_PARSE_DTDLOAD);

    xml_Parser_ఆదానం_Ptr pinput =
        xml_New_ఆదానం_From_జ్ఞాపకం(ఖాళీ, dtdstr, ARRAY_కొలత(dtdstr) - 1,
                              XML_ఆదానం_BUF_STATIC);
    ఒకవేళ (!pinput)
        వెళ్ళు dtd_error;

    xmlDtd *dtd = xmlCtxtParseDtd(xmlCtxt, pinput, ఖాళీ, ఖాళీ);
#లేకపోతే
    /* Note: do not use xmlParserInputBufferCreateStatic, it generates random
     * DTD validity errors for unknown reasons */
    xml_Parser_ఆదానం_బఫరు_Ptr buf =
        xml_Parser_ఆదానం_బఫరు_Create_Mem(dtdstr, ARRAY_కొలత(dtdstr) - 1,
                                      XML_CHAR_ENCODING_NONE);
    ఒకవేళ (!buf)
        వెళ్ళు dtd_error;
    xmlDtd *dtd = xmlIOParseDTD(ఖాళీ, buf, XML_CHAR_ENCODING_UTF8);
#అయితే_ముగింపు

    ఒకవేళ (!dtd) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID, "Failed to load DTD\n");
        వెళ్ళు dtd_error;
    }

#ఒకవేళ HAVE_XML_CTXT_PARSE_DTD
    success = xmlCtxtValidateDtd(xmlCtxt, doc, dtd);
#లేకపోతే
    xmlValidCtxt *dtdvalid = xmlNewValidCtxt();
    ఒకవేళ (!dtdvalid) {
        success = false;
        వెళ్ళు ctx_error;
    }
    dtdvalid->error = &xml_error_func;
    dtdvalid->user_దత్తాంశం_2 = ctx;
    success = xmlValidateDtd(dtdvalid, doc, dtd);
    xmlFreeValidCtxt(dtdvalid);

ctx_error:
#అయితే_ముగింపు
    xmlFreeDtd(dtd);

dtd_error:
#ఒకవేళ HAVE_XML_CTXT_PARSE_DTD
    xmlFreeParserCtxt(xmlCtxt);
#అయితే_ముగింపు
    తిరిగి success;
}

స్థిర bool
parse(నిర్మాణం rxkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం,
      గణన rxkb_popularity popularity)
{
    bool success = false;
    xmlDoc *doc = ఖాళీ;
    xml_కణుపు *మూలం = ఖాళీ;

    ఒకవేళ (!check_eaccess(మార్గం, R_OK))
        తిరిగి false;

    LIBXML_TEST_సంస్కరణ

    xmlParserCtxtPtr xmlCtxt = xmlNewParserCtxt();
    ఒకవేళ (!xmlCtxt)
        తిరిగి false;

#నిర్వచితమైతే XML_PARSE_NO_XXE
#నిర్వచించు _XML_OPTIONS (XML_PARSE_NONET | XML_PARSE_NOENT | XML_PARSE_NO_XXE)
#లేకపోతే
#నిర్వచించు _XML_OPTIONS (XML_PARSE_NONET)
#అయితే_ముగింపు
    xmlCtxtUseOptions(xmlCtxt, _XML_OPTIONS);

#ఒకవేళ HAVE_XML_CTXT_సమితి_ERRORHANDLER
    /* Prefer contextual handler whenever possible. It takes precedence over
     * the global generic handler. */
    xml_Ctxt_సమితి_Error_Handler(xmlCtxt, xml_structured_error_func, ctx);
#అయితే_ముగింపు
#ఒకవేళ !HAVE_XML_CTXT_PARSE_DTD
    /* This is needed for the DTD validation */
    xml_సమితి_Generic_Error_Func(ctx, xml_error_func);
#అయితే_ముగింపు

    doc = xmlCtxtReadFile(xmlCtxt, మార్గం, ఖాళీ, 0);
    ఒకవేళ (!doc)
        వెళ్ళు parse_error;

    ఒకవేళ (!validate(ctx, doc)) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "XML error: failed to validate document at %s\n", మార్గం);
        వెళ్ళు validate_error;
    }

    మూలం = xml_Doc_Get_మూలం_Element(doc);
    parse_నియమాలు_xml(ctx, మూలం, popularity);

    success = true;
validate_error:
    xmlFreeDoc(doc);
parse_error:

#ఒకవేళ !HAVE_XML_CTXT_PARSE_DTD
    /*
     * Reset the default libxml2 error handler to default, because this handler
     * is global and may be used on an invalid rxkb_context, e.g. *after* the
     * following sequence:
     *
     *     rxkb_context_new();
     *     rxkb_context_parse();
     *     rxkb_context_unref();
     */
    xml_సమితి_Generic_Error_Func(ఖాళీ, ఖాళీ);
#అయితే_ముగింపు
#ఒకవేళ HAVE_XML_CTXT_సమితి_ERRORHANDLER
    xml_Ctxt_సమితి_Error_Handler(xmlCtxt, ఖాళీ, ఖాళీ);
#అయితే_ముగింపు
    xmlFreeParserCtxt(xmlCtxt);

    తిరిగి success;
}
