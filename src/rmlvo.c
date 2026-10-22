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
                      ثابت حرف *قواعد, ثابت حرف *model,
                      تعداد xkb_rmlvo_builder_flags flags)
{
    ثابت تعداد xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(تعداد xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_مقادیر);
    إذا (invalid_flags) {
        log_err_func(context, XKB_LOG_پیام_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        أرجع عدم;
    }

    بنية xkb_rmlvo_builder * ثابت builder = حافظه_صفر_بده(1, حجم(*builder));
    إذا (!builder)
        اذهب error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->قواعد = strdup_امن(قواعد);
    إذا (!builder->قواعد && قواعد)
        اذهب error;

    builder->model = strdup_امن(model);
    إذا (!builder->model && model)
        اذهب error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    أرجع builder;

error:
    log_err(context, XKB_ERROR_تخصیص_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    أرجع عدم;
}

ساكن تعداد xkb_وضعیت
rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_ماسک_t layouts)
{
    إذا (isempty(option))
        أرجع XKB_ERROR_INVALID;

    /* Check for previous entry */
    بنية xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        إذا (streq(prev->option, option)) {
            /* Merge with previous entry */
            إذا (layouts == (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } وإلا إذا (prev->layouts != (xkb_layout_ماسک_t)
                       XKB_OPTION_LAYOUT_ماسک_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            أرجع XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت بنية xkb_rmlvo_builder_option new = {
        .option = strdup_امن(option),
        .layouts = layouts
    };
    إذا (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        أرجع XKB_ERROR_تخصیص_FAILURE;
    }
    darray_append(rmlvo->options, new);
    أرجع XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, اندازه_t options_len)
{
    ثابت xkb_layout_شاخص_t idx = (xkb_layout_شاخص_t)
                                   darray_اندازه(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    إذا (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        أرجع false;
    }

    /* Append layout entry */
    ثابت بنية xkb_rmlvo_builder_layout new = {
        .layout = strdup_امن(layout),
        .variant = strdup_امن(variant)
    };

    إذا (!new.layout || (!new.variant && variant)) {
        حرر(new.layout);
        حرر(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        أرجع false;
    }

    darray_append(rmlvo->layouts, new);

    إذا (!options)
        options_len = 0;

    ثابت xkb_layout_ماسک_t layout_ماسک = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    لكل (اندازه_t k = 0; k < options_len; k++) {
        ثابت تعداد xkb_وضعیت وضعیت =
            rmlvo_builder_append_option(rmlvo, options[k], layout_ماسک);

        إذا (وضعیت == XKB_ERROR_تخصیص_FAILURE)
            أرجع false;
    }

    أرجع true;
}

bool
xkb_rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت تعداد xkb_وضعیت وضعیت = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL
    );
    أرجع (وضعیت == XKB_SUCCESS);
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

    حرر(rmlvo->قواعد);
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
xkb_rmlvo_builder_مقصد_قواعد_نامها(ثابت بنية xkb_rmlvo_builder *builder,
                                 بنية xkb_قاعده_نامها *rmlvo,
                                 حرف *buf, اندازه_t buf_اندازه)
{
    rmlvo->قواعد = builder->قواعد;
    rmlvo->model = builder->model;

    حرف *شروع = buf;
    rmlvo->layout = شروع;
    darray_اندازه_t k;
    ثابت بنية xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح شمار = snprintf(شروع, buf_اندازه, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        إذا (شمار < 0 || (اندازه_t)شمار >= buf_اندازه)
            أرجع false;
        buf_اندازه -= (اندازه_t)شمار;
        شروع += شمار;
    }
    إذا (buf_اندازه <= 1)
        أرجع false;
    *شروع = '\0';
    شروع++;
    buf_اندازه--;

    rmlvo->variant = شروع;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح شمار = snprintf(شروع, buf_اندازه, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        إذا (شمار < 0 || (اندازه_t) شمار >= buf_اندازه)
            أرجع false;
        buf_اندازه -= (اندازه_t)شمار;
        شروع += شمار;
    }
    إذا (buf_اندازه <= 1)
        أرجع false;
    *شروع = '\0';
    شروع++;
    buf_اندازه--;

    rmlvo->options = شروع;
    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحيح شمار = snprintf(شروع, buf_اندازه, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        إذا (شمار < 0 || (اندازه_t) شمار >= buf_اندازه)
            أرجع false;
        buf_اندازه -= (اندازه_t)شمار;
        شروع += شمار;
        إذا (option->layouts != XKB_LAYOUT_INVALID) {
            شمار = snprintf(شروع, buf_اندازه, "%c%"PRIu32,
                             OPTIONS_گروه_SPECIFIER_پیشوند, option->layouts);
            إذا (شمار < 0 || (اندازه_t) شمار >= buf_اندازه)
                أرجع false;
            buf_اندازه -= (اندازه_t)شمار;
            شروع += شمار;
        }
    }
    إذا (buf_اندازه == 0)
        أرجع false;
    *شروع = '\0';
    أرجع true;
}
