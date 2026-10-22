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

ساختار xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ساختار xkb_context *context,
                      ثابت نویسه *rules, ثابت نویسه *model,
                      شمارش xkb_rmlvo_builder_flags flags)
{
    ثابت شمارش xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(شمارش xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_مقادیر);
    اگر (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        بازگشت پوچ;
    }

    ساختار xkb_rmlvo_builder * ثابت builder = calloc(1, اندازه(*builder));
    اگر (!builder)
        برو error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    اگر (!builder->rules && rules)
        برو error;

    builder->model = strdup_safe(model);
    اگر (!builder->model && model)
        برو error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    بازگشت builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    بازگشت پوچ;
}

ایستا شمارش xkb_status
rmlvo_builder_append_option(ساختار xkb_rmlvo_builder *rmlvo,
                            ثابت نویسه *option, xkb_layout_mask_t layouts)
{
    اگر (isempty(option))
        بازگشت XKB_ERROR_INVALID;

    /* Check for previous entry */
    ساختار xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        اگر (streq(prev->option, option)) {
            /* Merge with previous entry */
            اگر (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } وگرنه اگر (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            بازگشت XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت ساختار xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    اگر (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        بازگشت XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    بازگشت XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ساختار xkb_rmlvo_builder *rmlvo,
                                ثابت نویسه *layout, ثابت نویسه *variant,
                                ثابت نویسه* ثابت* options, size_t options_len)
{
    ثابت xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    اگر (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        بازگشت false;
    }

    /* Append layout entry */
    ثابت ساختار xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    اگر (!new.layout || (!new.variant && variant)) {
        آزاد(new.layout);
        آزاد(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        بازگشت false;
    }

    darray_append(rmlvo->layouts, new);

    اگر (!options)
        options_len = 0;

    ثابت xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    برای (size_t k = 0; k < options_len; k++) {
        ثابت شمارش xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        اگر (status == XKB_ERROR_ALLOCATION_FAILURE)
            بازگشت false;
    }

    بازگشت true;
}

bool
xkb_rmlvo_builder_append_option(ساختار xkb_rmlvo_builder *rmlvo,
                                ثابت نویسه *option)
{
    ثابت شمارش xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    بازگشت (status == XKB_SUCCESS);
}

ساختار xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ساختار xkb_rmlvo_builder *rmlvo)
{
    أكد(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    بازگشت rmlvo;
}

تهی
xkb_rmlvo_builder_unref(ساختار xkb_rmlvo_builder *rmlvo)
{
    أكد(!rmlvo || rmlvo->refcnt > 0);
    اگر (!rmlvo || --rmlvo->refcnt > 0)
        بازگشت;

    آزاد(rmlvo->rules);
    آزاد(rmlvo->model);

    ثابت ساختار xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        آزاد(layout->layout);
        آزاد(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ثابت ساختار xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        آزاد(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    آزاد(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ثابت ساختار xkb_rmlvo_builder *builder,
                                 ساختار xkb_rule_names *rmlvo,
                                 نویسه *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    نویسه *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ثابت ساختار xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        صحیح شمار = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        اگر (شمار < 0 || (size_t)شمار >= buf_size)
            بازگشت false;
        buf_size -= (size_t)شمار;
        start += شمار;
    }
    اگر (buf_size <= 1)
        بازگشت false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        صحیح شمار = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        اگر (شمار < 0 || (size_t) شمار >= buf_size)
            بازگشت false;
        buf_size -= (size_t)شمار;
        start += شمار;
    }
    اگر (buf_size <= 1)
        بازگشت false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ثابت ساختار xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحیح شمار = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        اگر (شمار < 0 || (size_t) شمار >= buf_size)
            بازگشت false;
        buf_size -= (size_t)شمار;
        start += شمار;
        اگر (option->layouts != XKB_LAYOUT_INVALID) {
            شمار = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            اگر (شمار < 0 || (size_t) شمار >= buf_size)
                بازگشت false;
            buf_size -= (size_t)شمار;
            start += شمار;
        }
    }
    اگر (buf_size == 0)
        بازگشت false;
    *start = '\0';
    بازگشت true;
}
