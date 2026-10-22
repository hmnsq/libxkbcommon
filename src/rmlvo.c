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
    אם (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        החזר عدم;
    }

    بنية xkb_rmlvo_builder * ثابت builder = calloc(1, حجم(*builder));
    אם (!builder)
        اذهب error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    אם (!builder->rules && rules)
        اذهب error;

    builder->model = strdup_safe(model);
    אם (!builder->model && model)
        اذهب error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    החזר builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    החזר عدم;
}

ساكن تعداد xkb_status
rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_mask_t layouts)
{
    אם (isempty(option))
        החזר XKB_ERROR_INVALID;

    /* Check for previous entry */
    بنية xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        אם (streq(prev->option, option)) {
            /* Merge with previous entry */
            אם (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } אחרת אם (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            החזר XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت بنية xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    אם (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        החזר XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    החזר XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, size_t options_len)
{
    ثابت xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    אם (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        החזר false;
    }

    /* Append layout entry */
    ثابت بنية xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    אם (!new.layout || (!new.variant && variant)) {
        حرر(new.layout);
        حرر(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        החזר false;
    }

    darray_append(rmlvo->layouts, new);

    אם (!options)
        options_len = 0;

    ثابت xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    עבור (size_t k = 0; k < options_len; k++) {
        ثابت تعداد xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        אם (status == XKB_ERROR_ALLOCATION_FAILURE)
            החזר false;
    }

    החזר true;
}

bool
xkb_rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت تعداد xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    החזר (status == XKB_SUCCESS);
}

بنية xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    החזר rmlvo;
}

فراغ
xkb_rmlvo_builder_unref(بنية xkb_rmlvo_builder *rmlvo)
{
    أكد(!rmlvo || rmlvo->refcnt > 0);
    אם (!rmlvo || --rmlvo->refcnt > 0)
        החזר;

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
        אם (count < 0 || (size_t)count >= buf_size)
            החזר false;
        buf_size -= (size_t)count;
        start += count;
    }
    אם (buf_size <= 1)
        החזר false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        אם (count < 0 || (size_t) count >= buf_size)
            החזר false;
        buf_size -= (size_t)count;
        start += count;
    }
    אם (buf_size <= 1)
        החזר false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحيح count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        אם (count < 0 || (size_t) count >= buf_size)
            החזר false;
        buf_size -= (size_t)count;
        start += count;
        אם (option->layouts != XKB_LAYOUT_INVALID) {
            count = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            אם (count < 0 || (size_t) count >= buf_size)
                החזר false;
            buf_size -= (size_t)count;
            start += count;
        }
    }
    אם (buf_size == 0)
        החזר false;
    *start = '\0';
    החזר true;
}
