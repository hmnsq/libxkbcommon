/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "context.h"
#شامل "features/enums.h"
#شامل "keymap.h"
#شامل "messages-codes.h"
#شامل "rmlvo.h"
#شامل "darray.h"
#شامل "utils.h"
#شامل "xkbcomp/rules.h"

ساخت xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ساخت xkb_context *context,
                      ثابت حرف *قواعد, ثابت حرف *model,
                      شمار xkb_rmlvo_builder_flags flags)
{
    ثابت شمار xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(شمار xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_اقدار);
    اگر (invalid_flags) {
        log_err_func(context, XKB_LOG_پیغام_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        واپس تہی;
    }

    ساخت xkb_rmlvo_builder * ثابت builder = صفر_حافظہ_دو(1, حجم(*builder));
    اگر (!builder)
        جاؤ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->قواعد = strdup_محفوظ(قواعد);
    اگر (!builder->قواعد && قواعد)
        جاؤ error;

    builder->model = strdup_محفوظ(model);
    اگر (!builder->model && model)
        جاؤ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    واپس builder;

error:
    log_err(context, XKB_ERROR_تخصیص_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    واپس تہی;
}

ساکن شمار xkb_حالت
rmlvo_builder_append_option(ساخت xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_نقاب_t layouts)
{
    اگر (isempty(option))
        واپس XKB_ERROR_INVALID;

    /* Check for previous entry */
    ساخت xkb_rmlvo_builder_option *پچھلا;
    darray_foreach(پچھلا, rmlvo->options) {
        اگر (streq(پچھلا->option, option)) {
            /* Merge with previous entry */
            اگر (layouts == (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                /* Global option overrides layout-specific ones */
                پچھلا->layouts = layouts;
            } ورنہ اگر (پچھلا->layouts != (xkb_layout_نقاب_t)
                       XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                /* Layout-specific option cannot override global */
                پچھلا->layouts |= layouts;
            }
            واپس XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت ساخت xkb_rmlvo_builder_option new = {
        .option = strdup_محفوظ(option),
        .layouts = layouts
    };
    اگر (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        واپس XKB_ERROR_تخصیص_FAILURE;
    }
    darray_append(rmlvo->options, new);
    واپس XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ساخت xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, سائز_t options_len)
{
    ثابت xkb_layout_اشاریہ_t idx = (xkb_layout_اشاریہ_t)
                                   darray_سائز(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    اگر (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        واپس false;
    }

    /* Append layout entry */
    ثابت ساخت xkb_rmlvo_builder_layout new = {
        .layout = strdup_محفوظ(layout),
        .variant = strdup_محفوظ(variant)
    };

    اگر (!new.layout || (!new.variant && variant)) {
        آزاد(new.layout);
        آزاد(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        واپس false;
    }

    darray_append(rmlvo->layouts, new);

    اگر (!options)
        options_len = 0;

    ثابت xkb_layout_نقاب_t layout_نقاب = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    برائے (سائز_t k = 0; k < options_len; k++) {
        ثابت شمار xkb_حالت حالت_2 =
            rmlvo_builder_append_option(rmlvo, options[k], layout_نقاب);

        اگر (حالت_2 == XKB_ERROR_تخصیص_FAILURE)
            واپس false;
    }

    واپس true;
}

bool
xkb_rmlvo_builder_append_option(ساخت xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت شمار xkb_حالت حالت_2 = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL
    );
    واپس (حالت_2 == XKB_SUCCESS);
}

ساخت xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ساخت xkb_rmlvo_builder *rmlvo)
{
    أكد(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    واپس rmlvo;
}

خالی
xkb_rmlvo_builder_unref(ساخت xkb_rmlvo_builder *rmlvo)
{
    أكد(!rmlvo || rmlvo->refcnt > 0);
    اگر (!rmlvo || --rmlvo->refcnt > 0)
        واپس;

    آزاد(rmlvo->قواعد);
    آزاد(rmlvo->model);

    ثابت ساخت xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        آزاد(layout->layout);
        آزاد(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ثابت ساخت xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        آزاد(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    آزاد(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_مقصد_قواعد_نام(ثابت ساخت xkb_rmlvo_builder *builder,
                                 ساخت xkb_قاعدہ_نام *rmlvo,
                                 حرف *buf, سائز_t buf_سائز)
{
    rmlvo->قواعد = builder->قواعد;
    rmlvo->model = builder->model;

    حرف *آغاز = buf;
    rmlvo->layout = آغاز;
    darray_سائز_t k;
    ثابت ساخت xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        صحیح گنتی = snprintf(آغاز, buf_سائز, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        اگر (گنتی < 0 || (سائز_t)گنتی >= buf_سائز)
            واپس false;
        buf_سائز -= (سائز_t)گنتی;
        آغاز += گنتی;
    }
    اگر (buf_سائز <= 1)
        واپس false;
    *آغاز = '\0';
    آغاز++;
    buf_سائز--;

    rmlvo->variant = آغاز;
    darray_enumerate(k, layout, builder->layouts) {
        صحیح گنتی = snprintf(آغاز, buf_سائز, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        اگر (گنتی < 0 || (سائز_t) گنتی >= buf_سائز)
            واپس false;
        buf_سائز -= (سائز_t)گنتی;
        آغاز += گنتی;
    }
    اگر (buf_سائز <= 1)
        واپس false;
    *آغاز = '\0';
    آغاز++;
    buf_سائز--;

    rmlvo->options = آغاز;
    ثابت ساخت xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحیح گنتی = snprintf(آغاز, buf_سائز, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        اگر (گنتی < 0 || (سائز_t) گنتی >= buf_سائز)
            واپس false;
        buf_سائز -= (سائز_t)گنتی;
        آغاز += گنتی;
        اگر (option->layouts != XKB_LAYOUT_INVALID) {
            گنتی = snprintf(آغاز, buf_سائز, "%c%"PRIu32,
                             OPTIONS_گروہ_SPECIFIER_سابقہ, option->layouts);
            اگر (گنتی < 0 || (سائز_t) گنتی >= buf_سائز)
                واپس false;
            buf_سائز -= (سائز_t)گنتی;
            آغاز += گنتی;
        }
    }
    اگر (buf_سائز == 0)
        واپس false;
    *آغاز = '\0';
    واپس true;
}
