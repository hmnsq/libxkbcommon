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

কাঠামো xkb_rmlvo_builder*
xkb_rmlvo_builder_new(কাঠামো xkb_context *context,
                      ধ্রুবক অক্ষর *rules, ধ্রুবক অক্ষর *model,
                      গণনা xkb_rmlvo_builder_flags flags)
{
    ধ্রুবক গণনা xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(গণনা xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_rmlvo_builder * ধ্রুবক builder = calloc(1, আকার(*builder));
    যদি (!builder)
        যাও error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    যদি (!builder->rules && rules)
        যাও error;

    builder->model = strdup_safe(model);
    যদি (!builder->model && model)
        যাও error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ফেরত builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ফেরত ফাঁকা;
}

স্থির গণনা xkb_status
rmlvo_builder_append_option(কাঠামো xkb_rmlvo_builder *rmlvo,
                            ধ্রুবক অক্ষর *option, xkb_layout_mask_t layouts)
{
    যদি (isempty(option))
        ফেরত XKB_ERROR_INVALID;

    /* Check for previous entry */
    কাঠামো xkb_rmlvo_builder_option *পূর্ব;
    darray_foreach(পূর্ব, rmlvo->options) {
        যদি (streq(পূর্ব->option, option)) {
            /* Merge with previous entry */
            যদি (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                পূর্ব->layouts = layouts;
            } অন্যথা যদি (পূর্ব->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                পূর্ব->layouts |= layouts;
            }
            ফেরত XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ধ্রুবক কাঠামো xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    যদি (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ফেরত XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ফেরত XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(কাঠামো xkb_rmlvo_builder *rmlvo,
                                ধ্রুবক অক্ষর *layout, ধ্রুবক অক্ষর *variant,
                                ধ্রুবক অক্ষর* ধ্রুবক* options, আকার_t options_len)
{
    ধ্রুবক xkb_layout_সূচক_t idx = (xkb_layout_সূচক_t)
                                   darray_আকার(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    যদি (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ফেরত false;
    }

    /* Append layout entry */
    ধ্রুবক কাঠামো xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    যদি (!new.layout || (!new.variant && variant)) {
        মুক্ত(new.layout);
        মুক্ত(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ফেরত false;
    }

    darray_append(rmlvo->layouts, new);

    যদি (!options)
        options_len = 0;

    ধ্রুবক xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    জন্য (আকার_t k = 0; k < options_len; k++) {
        ধ্রুবক গণনা xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        যদি (status == XKB_ERROR_ALLOCATION_FAILURE)
            ফেরত false;
    }

    ফেরত true;
}

bool
xkb_rmlvo_builder_append_option(কাঠামো xkb_rmlvo_builder *rmlvo,
                                ধ্রুবক অক্ষর *option)
{
    ধ্রুবক গণনা xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ফেরত (status == XKB_SUCCESS);
}

কাঠামো xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(কাঠামো xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ফেরত rmlvo;
}

শূন্য
xkb_rmlvo_builder_unref(কাঠামো xkb_rmlvo_builder *rmlvo)
{
    প্রতিজ্ঞা(!rmlvo || rmlvo->refcnt > 0);
    যদি (!rmlvo || --rmlvo->refcnt > 0)
        ফেরত;

    মুক্ত(rmlvo->rules);
    মুক্ত(rmlvo->model);

    ধ্রুবক কাঠামো xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        মুক্ত(layout->layout);
        মুক্ত(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ধ্রুবক কাঠামো xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        মুক্ত(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    মুক্ত(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ধ্রুবক কাঠামো xkb_rmlvo_builder *builder,
                                 কাঠামো xkb_rule_names *rmlvo,
                                 অক্ষর *buf, আকার_t buf_আকার)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    অক্ষর *start = buf;
    rmlvo->layout = start;
    darray_আকার_t k;
    ধ্রুবক কাঠামো xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণসংখ্যা গুনতি = snprintf(start, buf_আকার, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        যদি (গুনতি < 0 || (আকার_t)গুনতি >= buf_আকার)
            ফেরত false;
        buf_আকার -= (আকার_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_আকার <= 1)
        ফেরত false;
    *start = '\0';
    start++;
    buf_আকার--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        পূর্ণসংখ্যা গুনতি = snprintf(start, buf_আকার, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        যদি (গুনতি < 0 || (আকার_t) গুনতি >= buf_আকার)
            ফেরত false;
        buf_আকার -= (আকার_t)গুনতি;
        start += গুনতি;
    }
    যদি (buf_আকার <= 1)
        ফেরত false;
    *start = '\0';
    start++;
    buf_আকার--;

    rmlvo->options = start;
    ধ্রুবক কাঠামো xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        পূর্ণসংখ্যা গুনতি = snprintf(start, buf_আকার, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        যদি (গুনতি < 0 || (আকার_t) গুনতি >= buf_আকার)
            ফেরত false;
        buf_আকার -= (আকার_t)গুনতি;
        start += গুনতি;
        যদি (option->layouts != XKB_LAYOUT_INVALID) {
            গুনতি = snprintf(start, buf_আকার, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            যদি (গুনতি < 0 || (আকার_t) গুনতি >= buf_আকার)
                ফেরত false;
            buf_আকার -= (আকার_t)গুনতি;
            start += গুনতি;
        }
    }
    যদি (buf_আকার == 0)
        ফেরত false;
    *start = '\0';
    ফেরত true;
}
