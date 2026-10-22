/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "rmlvo.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "xkbcomp/rules.h"

সংরচনা xkb_rmlvo_builder*
xkb_rmlvo_builder_new(সংরচনা xkb_context *context,
                      ধ্রুব বর্ণ *rules, ধ্রুব বর্ণ *model,
                      গণনা xkb_rmlvo_builder_flags flags)
{
    ধ্রুব গণনা xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(গণনা xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        প্রত্যাগম রিক্ত;
    }

    সংরচনা xkb_rmlvo_builder * ধ্রুব builder = calloc(1, পরিমাণ(*builder));
    যদি (!builder)
        গচ্ছ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    যদি (!builder->rules && rules)
        গচ্ছ error;

    builder->model = strdup_safe(model);
    যদি (!builder->model && model)
        গচ্ছ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    প্রত্যাগম builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    প্রত্যাগম রিক্ত;
}

স্থির গণনা xkb_status
rmlvo_builder_append_option(সংরচনা xkb_rmlvo_builder *rmlvo,
                            ধ্রুব বর্ণ *option, xkb_layout_mask_t layouts)
{
    যদি (isempty(option))
        প্রত্যাগম XKB_ERROR_INVALID;

    /* Check for previous entry */
    সংরচনা xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        যদি (streq(prev->option, option)) {
            /* Merge with previous entry */
            যদি (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } অন্যথা যদি (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            প্রত্যাগম XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ধ্রুব সংরচনা xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    যদি (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        প্রত্যাগম XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    প্রত্যাগম XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(সংরচনা xkb_rmlvo_builder *rmlvo,
                                ধ্রুব বর্ণ *layout, ধ্রুব বর্ণ *variant,
                                ধ্রুব বর্ণ* ধ্রুব* options, size_t options_len)
{
    ধ্রুব xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    যদি (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        প্রত্যাগম false;
    }

    /* Append layout entry */
    ধ্রুব সংরচনা xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    যদি (!new.layout || (!new.variant && variant)) {
        মুক্ত(new.layout);
        মুক্ত(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        প্রত্যাগম false;
    }

    darray_append(rmlvo->layouts, new);

    যদি (!options)
        options_len = 0;

    ধ্রুব xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    প্রতি (size_t k = 0; k < options_len; k++) {
        ধ্রুব গণনা xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        যদি (status == XKB_ERROR_ALLOCATION_FAILURE)
            প্রত্যাগম false;
    }

    প্রত্যাগম true;
}

bool
xkb_rmlvo_builder_append_option(সংরচনা xkb_rmlvo_builder *rmlvo,
                                ধ্রুব বর্ণ *option)
{
    ধ্রুব গণনা xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    প্রত্যাগম (status == XKB_SUCCESS);
}

সংরচনা xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(সংরচনা xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    প্রত্যাগম rmlvo;
}

শূন্য
xkb_rmlvo_builder_unref(সংরচনা xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(!rmlvo || rmlvo->refcnt > 0);
    যদি (!rmlvo || --rmlvo->refcnt > 0)
        প্রত্যাগম;

    মুক্ত(rmlvo->rules);
    মুক্ত(rmlvo->model);

    ধ্রুব সংরচনা xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        মুক্ত(layout->layout);
        মুক্ত(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ধ্রুব সংরচনা xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        মুক্ত(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    মুক্ত(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ধ্রুব সংরচনা xkb_rmlvo_builder *builder,
                                 সংরচনা xkb_rule_names *rmlvo,
                                 বর্ণ *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    বর্ণ *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ধ্রুব সংরচনা xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        যদি (গুনতি < 0 || (size_t)গুনতি >= buf_size)
            প্রত্যাগম false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_size <= 1)
        প্রত্যাগম false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
            প্রত্যাগম false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_size <= 1)
        প্রত্যাগম false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ধ্রুব সংরচনা xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
            প্রত্যাগম false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
        যদি (option->layouts != XKB_LAYOUT_INVALID) {
            গুনতি = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
                প্রত্যাগম false;
            buf_size -= (size_t)গুনতি;
            start += গুনতি;
        }
    }
    যদি (buf_size == 0)
        প্রত্যাগম false;
    *start = '\0';
    প্রত্যাগম true;
}
