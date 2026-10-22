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
        (flags & ~(تعداد xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_اقدار);
    إذا (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        أرجع عدم;
    }

    بنية xkb_rmlvo_builder * ثابت builder = calloc(1, حجم(*builder));
    إذا (!builder)
        اذهب error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    إذا (!builder->rules && rules)
        اذهب error;

    builder->model = strdup_safe(model);
    إذا (!builder->model && model)
        اذهب error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    أرجع builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    أرجع عدم;
}

ساكن تعداد xkb_status
rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                            ثابت حرف *option, xkb_layout_mask_t layouts)
{
    إذا (isempty(option))
        أرجع XKB_ERROR_INVALID;

    /* Check for previous entry */
    بنية xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        إذا (streq(prev->option, option)) {
            /* Merge with previous entry */
            إذا (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } وإلا إذا (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            أرجع XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ثابت بنية xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    إذا (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        أرجع XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    أرجع XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *layout, ثابت حرف *variant,
                                ثابت حرف* ثابت* options, size_t options_len)
{
    ثابت xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    إذا (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        أرجع false;
    }

    /* Append layout entry */
    ثابت بنية xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    إذا (!new.layout || (!new.variant && variant)) {
        حرر(new.layout);
        حرر(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        أرجع false;
    }

    darray_append(rmlvo->layouts, new);

    إذا (!options)
        options_len = 0;

    ثابت xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    لكل (size_t k = 0; k < options_len; k++) {
        ثابت تعداد xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        إذا (status == XKB_ERROR_ALLOCATION_FAILURE)
            أرجع false;
    }

    أرجع true;
}

bool
xkb_rmlvo_builder_append_option(بنية xkb_rmlvo_builder *rmlvo,
                                ثابت حرف *option)
{
    ثابت تعداد xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    أرجع (status == XKB_SUCCESS);
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
        صحيح گنتی = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        إذا (گنتی < 0 || (size_t)گنتی >= buf_size)
            أرجع false;
        buf_size -= (size_t)گنتی;
        start += گنتی;
    }
    إذا (buf_size <= 1)
        أرجع false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        صحيح گنتی = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        إذا (گنتی < 0 || (size_t) گنتی >= buf_size)
            أرجع false;
        buf_size -= (size_t)گنتی;
        start += گنتی;
    }
    إذا (buf_size <= 1)
        أرجع false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ثابت بنية xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        صحيح گنتی = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        إذا (گنتی < 0 || (size_t) گنتی >= buf_size)
            أرجع false;
        buf_size -= (size_t)گنتی;
        start += گنتی;
        إذا (option->layouts != XKB_LAYOUT_INVALID) {
            گنتی = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            إذا (گنتی < 0 || (size_t) گنتی >= buf_size)
                أرجع false;
            buf_size -= (size_t)گنتی;
            start += گنتی;
        }
    }
    إذا (buf_size == 0)
        أرجع false;
    *start = '\0';
    أرجع true;
}
