/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "context.h"
#ضمن "features/enums.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"
#ضمن "rmlvo.h"
#ضمن "darray.h"
#ضمن "utils.h"
#ضمن "xkbcomp/rules.h"

بنية xkb_rmlvo_builder*
xkb_rmlvo_builder_new(بنية xkb_context *context,
                      ثابت حرف *قاعدا, ثابت حرف *model,
                      تعداد xkb_rmlvo_builder_flags flags)
{
    ثابت تعداد xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(تعداد xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_قدرون);
    إذا (invalid_flags) {
        log_err_func(context, XKB_LOG_پيغام_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        أرجع عدم;
    }

    بنية xkb_rmlvo_builder * ثابت builder = صفر_يادگيري_ڏيو(1, حجم(*builder));
    إذا (!builder)
        اذهب error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->قاعدا = strdup_محفوظ(قاعدا);
    إذا (!builder->قاعدا && قاعدا)
        اذهب error;

    builder->model = strdup_محفوظ(model);
    إذا (!builder->model && model)
        اذهب error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    أرجع builder;

error:
    log_err(context, XKB_ERROR_ورهاست_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    أرجع عدم;
}

ساكن تعداد xkb_حالت
rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_نقاب_t layouts)
{
    إذا (isempty(option))
        أرجع XKB_ERROR_INVALID;

    /* Check for previous entry */
    بنية xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        إذا (streq(prev->option, option)) {
            /* Merge with previous entry */
            إذا (layouts == (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } وإلا إذا (prev->layouts != (xkb_layout_نقاب_t)
                       XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            أرجع XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت بنية xkb_rmlvo_builder_option new = {
        .option = strdup_محفوظ(option),
        .layouts = layouts
    };
    إذا (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        أرجع XKB_ERROR_ورهاست_FAILURE;
    }
    darray_append(rmlvo->options, new);
    أرجع XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, سائيز_t options_len)
{
    ثابت xkb_layout_اشاريو_t idx = (xkb_layout_اشاريو_t)
                                   darray_سائيز(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    إذا (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        أرجع false;
    }

    /* Append layout entry */
    ثابت بنية xkb_rmlvo_builder_layout new = {
        .layout = strdup_محفوظ(layout),
        .variant = strdup_محفوظ(variant)
    };

    إذا (!new.layout || (!new.variant && variant)) {
        حرر(new.layout);
        حرر(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        أرجع false;
    }

    darray_append(rmlvo->layouts, new);

    إذا (!options)
        options_len = 0;

    ثابت xkb_layout_نقاب_t layout_نقاب = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    لكل (سائيز_t k = 0; k < options_len; k++) {
        ثابت تعداد xkb_حالت حالت =
            rmlvo_builder_append_option(rmlvo, options[k], layout_نقاب);

        إذا (حالت == XKB_ERROR_ورهاست_FAILURE)
            أرجع false;
    }

    أرجع true;
}

bool
xkb_rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت تعداد xkb_حالت حالت = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL
    );
    أرجع (حالت == XKB_SUCCESS);
}

بنية xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    أرجع rmlvo;
}

فراغ
xkb_rmlvo_builder_unref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(!rmlvo || rmlvo->refcnt > 0);
    إذا (!rmlvo || --rmlvo->refcnt > 0)
        أرجع;

    حرر(rmlvo->قاعدا);
    حرر(rmlvo->model);

    ثابت بنية xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        حرر(layout->layout);
        حرر(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        حرر(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    حرر(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_منزل_قاعدا_نالا(ثابت بنية xkb_rmlvo_builder *builder,
                                 بنية xkb_قاعدو_نالا *rmlvo,
                                 حرف *buf, سائيز_t buf_سائيز)
{
    rmlvo->قاعدا = builder->قاعدا;
    rmlvo->model = builder->model;

    حرف *شروعات = buf;
    rmlvo->layout = شروعات;
    darray_سائيز_t k;
    ثابت بنية xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح ڳڻپ = snprintf(شروعات, buf_سائيز, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        إذا (ڳڻپ < 0 || (سائيز_t)ڳڻپ >= buf_سائيز)
            أرجع false;
        buf_سائيز -= (سائيز_t)ڳڻپ;
        شروعات += ڳڻپ;
    }
    إذا (buf_سائيز <= 1)
        أرجع false;
    *شروعات = '\0';
    شروعات++;
    buf_سائيز--;

    rmlvo->variant = شروعات;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح ڳڻپ = snprintf(شروعات, buf_سائيز, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        إذا (ڳڻپ < 0 || (سائيز_t) ڳڻپ >= buf_سائيز)
            أرجع false;
        buf_سائيز -= (سائيز_t)ڳڻپ;
        شروعات += ڳڻپ;
    }
    إذا (buf_سائيز <= 1)
        أرجع false;
    *شروعات = '\0';
    شروعات++;
    buf_سائيز--;

    rmlvo->options = شروعات;
    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحيح ڳڻپ = snprintf(شروعات, buf_سائيز, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        إذا (ڳڻپ < 0 || (سائيز_t) ڳڻپ >= buf_سائيز)
            أرجع false;
        buf_سائيز -= (سائيز_t)ڳڻپ;
        شروعات += ڳڻپ;
        إذا (option->layouts != XKB_LAYOUT_INVALID) {
            ڳڻپ = snprintf(شروعات, buf_سائيز, "%c%"PRIu32,
                             OPTIONS_گروهه_SPECIFIER_اڳياڙي, option->layouts);
            إذا (ڳڻپ < 0 || (سائيز_t) ڳڻپ >= buf_سائيز)
                أرجع false;
            buf_سائيز -= (سائيز_t)ڳڻپ;
            شروعات += ڳڻپ;
        }
    }
    إذا (buf_سائيز == 0)
        أرجع false;
    *شروعات = '\0';
    أرجع true;
}
