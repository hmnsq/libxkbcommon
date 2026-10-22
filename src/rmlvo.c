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
                      ثابت حرف *rules, ثابت حرف *model,
                      تعداد xkb_rmlvo_builder_flags flags)
{
    ثابت تعداد xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(تعداد xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_VALUES);
    ܐܢ (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ܦܢܐ عدم;
    }

    بنية xkb_rmlvo_builder * ثابت builder = calloc(1, حجم(*builder));
    ܐܢ (!builder)
        اذهب error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    ܐܢ (!builder->rules && rules)
        اذهب error;

    builder->model = strdup_safe(model);
    ܐܢ (!builder->model && model)
        اذهب error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ܦܢܐ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ܦܢܐ عدم;
}

ساكن تعداد xkb_status
rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_mask_t layouts)
{
    ܐܢ (isempty(option))
        ܦܢܐ XKB_ERROR_INVALID;

    /* Check for previous entry */
    بنية xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ܐܢ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ܐܢ (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ܐܚܪܢܐ ܐܢ (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ܦܢܐ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت بنية xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    ܐܢ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ܦܢܐ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ܦܢܐ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, size_t options_len)
{
    ثابت xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ܐܢ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ܦܢܐ false;
    }

    /* Append layout entry */
    ثابت بنية xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    ܐܢ (!new.layout || (!new.variant && variant)) {
        حرر(new.layout);
        حرر(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ܦܢܐ false;
    }

    darray_append(rmlvo->layouts, new);

    ܐܢ (!options)
        options_len = 0;

    ثابت xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    لكل (size_t k = 0; k < options_len; k++) {
        ثابت تعداد xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        ܐܢ (status == XKB_ERROR_ALLOCATION_FAILURE)
            ܦܢܐ false;
    }

    ܦܢܐ true;
}

bool
xkb_rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت تعداد xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ܦܢܐ (status == XKB_SUCCESS);
}

بنية xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ܦܢܐ rmlvo;
}

فراغ
xkb_rmlvo_builder_unref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(!rmlvo || rmlvo->refcnt > 0);
    ܐܢ (!rmlvo || --rmlvo->refcnt > 0)
        ܦܢܐ;

    حرر(rmlvo->rules);
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
xkb_rmlvo_builder_to_rules_names(ثابت بنية xkb_rmlvo_builder *builder,
                                 بنية xkb_rule_names *rmlvo,
                                 حرف *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    حرف *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ثابت بنية xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ܐܢ (count < 0 || (size_t)count >= buf_size)
            ܦܢܐ false;
        buf_size -= (size_t)count;
        start += count;
    }
    ܐܢ (buf_size <= 1)
        ܦܢܐ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ܐܢ (count < 0 || (size_t) count >= buf_size)
            ܦܢܐ false;
        buf_size -= (size_t)count;
        start += count;
    }
    ܐܢ (buf_size <= 1)
        ܦܢܐ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحيح count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ܐܢ (count < 0 || (size_t) count >= buf_size)
            ܦܢܐ false;
        buf_size -= (size_t)count;
        start += count;
        ܐܢ (option->layouts != XKB_LAYOUT_INVALID) {
            count = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            ܐܢ (count < 0 || (size_t) count >= buf_size)
                ܦܢܐ false;
            buf_size -= (size_t)count;
            start += count;
        }
    }
    ܐܢ (buf_size == 0)
        ܦܢܐ false;
    *start = '\0';
    ܦܢܐ true;
}
