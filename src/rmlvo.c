/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাৱ "context.h"
#অন্তর্ভাৱ "features/enums.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "rmlvo.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "xkbcomp/rules.h"

সংরচনা xkb_rmlvo_builder*
xkb_rmlvo_builder_new(সংরচনা xkb_context *context,
                      ধ্রুৱ ৱর্ণ *rules, ধ্রুৱ ৱর্ণ *model,
                      গণনা xkb_rmlvo_builder_flags flags)
{
    ধ্রুৱ গণনা xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(গণনা xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ঘূৰাই রিক্ত;
    }

    সংরচনা xkb_rmlvo_builder * ধ্রুৱ builder = calloc(1, পরিমাণ(*builder));
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

    ঘূৰাই builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ঘূৰাই রিক্ত;
}

স্থির গণনা xkb_status
rmlvo_builder_append_option(সংরচনা xkb_rmlvo_builder *rmlvo,
                            ধ্রুৱ ৱর্ণ *option, xkb_layout_mask_t layouts)
{
    যদি (isempty(option))
        ঘূৰাই XKB_ERROR_INVALID;

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
            ঘূৰাই XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ধ্রুৱ সংরচনা xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    যদি (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ঘূৰাই XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ঘূৰাই XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(সংরচনা xkb_rmlvo_builder *rmlvo,
                                ধ্রুৱ ৱর্ণ *layout, ধ্রুৱ ৱর্ণ *variant,
                                ধ্রুৱ ৱর্ণ* ধ্রুৱ* options, size_t options_len)
{
    ধ্রুৱ xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    যদি (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ঘূৰাই false;
    }

    /* Append layout entry */
    ধ্রুৱ সংরচনা xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    যদি (!new.layout || (!new.variant && variant)) {
        মুক্ত(new.layout);
        মুক্ত(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ঘূৰাই false;
    }

    darray_append(rmlvo->layouts, new);

    যদি (!options)
        options_len = 0;

    ধ্রুৱ xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    বাবে (size_t k = 0; k < options_len; k++) {
        ধ্রুৱ গণনা xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        যদি (status == XKB_ERROR_ALLOCATION_FAILURE)
            ঘূৰাই false;
    }

    ঘূৰাই true;
}

bool
xkb_rmlvo_builder_append_option(সংরচনা xkb_rmlvo_builder *rmlvo,
                                ধ্রুৱ ৱর্ণ *option)
{
    ধ্রুৱ গণনা xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ঘূৰাই (status == XKB_SUCCESS);
}

সংরচনা xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(সংরচনা xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ঘূৰাই rmlvo;
}

শূন্য
xkb_rmlvo_builder_unref(সংরচনা xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(!rmlvo || rmlvo->refcnt > 0);
    যদি (!rmlvo || --rmlvo->refcnt > 0)
        ঘূৰাই;

    মুক্ত(rmlvo->rules);
    মুক্ত(rmlvo->model);

    ধ্রুৱ সংরচনা xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        মুক্ত(layout->layout);
        মুক্ত(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ধ্রুৱ সংরচনা xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        মুক্ত(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    মুক্ত(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ধ্রুৱ সংরচনা xkb_rmlvo_builder *builder,
                                 সংরচনা xkb_rule_names *rmlvo,
                                 ৱর্ণ *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    ৱর্ণ *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ধ্রুৱ সংরচনা xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        যদি (গুনতি < 0 || (size_t)গুনতি >= buf_size)
            ঘূৰাই false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_size <= 1)
        ঘূৰাই false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
            ঘূৰাই false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_size <= 1)
        ঘূৰাই false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ধ্রুৱ সংরচনা xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        পূর্ণাঙ্ক গুনতি = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
            ঘূৰাই false;
        buf_size -= (size_t)গুনতি;
        start += গুনতি;
        যদি (option->layouts != XKB_LAYOUT_INVALID) {
            গুনতি = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            যদি (গুনতি < 0 || (size_t) গুনতি >= buf_size)
                ঘূৰাই false;
            buf_size -= (size_t)গুনতি;
            start += গুনতি;
        }
    }
    যদি (buf_size == 0)
        ঘূৰাই false;
    *start = '\0';
    ঘূৰাই true;
}
