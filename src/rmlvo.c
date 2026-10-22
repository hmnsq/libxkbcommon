/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "rmlvo.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/rules.h"

ସଂରଚନା xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ସଂରଚନା xkb_context *context,
                      ଅଚଳ ଅକ୍ଷର *rules, ଅଚଳ ଅକ୍ଷର *model,
                      ଗଣନା xkb_rmlvo_builder_flags flags)
{
    ଅଚଳ ଗଣନା xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ଗଣନା xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଯଦି (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_rmlvo_builder * ଅଚଳ builder = calloc(1, ଆକାର(*builder));
    ଯଦି (!builder)
        ଯାଅ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    ଯଦି (!builder->rules && rules)
        ଯାଅ error;

    builder->model = strdup_safe(model);
    ଯଦି (!builder->model && model)
        ଯାଅ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ଫେରନ୍ତୁ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଗଣନା xkb_status
rmlvo_builder_append_option(ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                            ଅଚଳ ଅକ୍ଷର *option, xkb_layout_mask_t layouts)
{
    ଯଦି (isempty(option))
        ଫେରନ୍ତୁ XKB_ERROR_INVALID;

    /* Check for previous entry */
    ସଂରଚନା xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ଯଦି (streq(prev->option, option)) {
            /* Merge with previous entry */
            ଯଦି (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ଅନ୍ୟଥା ଯଦି (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ଫେରନ୍ତୁ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    ଯଦି (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ଫେରନ୍ତୁ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ଫେରନ୍ତୁ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                                ଅଚଳ ଅକ୍ଷର *layout, ଅଚଳ ଅକ୍ଷର *variant,
                                ଅଚଳ ଅକ୍ଷର* ଅଚଳ* options, size_t options_len)
{
    ଅଚଳ xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ଯଦି (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ଫେରନ୍ତୁ false;
    }

    /* Append layout entry */
    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    ଯଦି (!new.layout || (!new.variant && variant)) {
        ମୁକ୍ତ(new.layout);
        ମୁକ୍ତ(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ଫେରନ୍ତୁ false;
    }

    darray_append(rmlvo->layouts, new);

    ଯଦି (!options)
        options_len = 0;

    ଅଚଳ xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    ପାଇଁ (size_t k = 0; k < options_len; k++) {
        ଅଚଳ ଗଣନା xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        ଯଦି (status == XKB_ERROR_ALLOCATION_FAILURE)
            ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}

bool
xkb_rmlvo_builder_append_option(ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                                ଅଚଳ ଅକ୍ଷର *option)
{
    ଅଚଳ ଗଣନା xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ଫେରନ୍ତୁ (status == XKB_SUCCESS);
}

ସଂରଚନା xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ସଂରଚନା xkb_rmlvo_builder *rmlvo)
{
    ପ୍ରତିଜ୍ଞା(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ଫେରନ୍ତୁ rmlvo;
}

ଶୂନ୍ୟ
xkb_rmlvo_builder_unref(ସଂରଚନା xkb_rmlvo_builder *rmlvo)
{
    ପ୍ରତିଜ୍ଞା(!rmlvo || rmlvo->refcnt > 0);
    ଯଦି (!rmlvo || --rmlvo->refcnt > 0)
        ଫେରନ୍ତୁ;

    ମୁକ୍ତ(rmlvo->rules);
    ମୁକ୍ତ(rmlvo->model);

    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        ମୁକ୍ତ(layout->layout);
        ମୁକ୍ତ(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        ମୁକ୍ତ(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    ମୁକ୍ତ(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *builder,
                                 ସଂରଚନା xkb_rule_names *rmlvo,
                                 ଅକ୍ଷର *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    ଅକ୍ଷର *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ଯଦି (ଗଣନା_2 < 0 || (size_t)ଗଣନା_2 >= buf_size)
            ଫେରନ୍ତୁ false;
        buf_size -= (size_t)ଗଣନା_2;
        start += ଗଣନା_2;
    }
    ଯଦି (buf_size <= 1)
        ଫେରନ୍ତୁ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ଯଦି (ଗଣନା_2 < 0 || (size_t) ଗଣନା_2 >= buf_size)
            ଫେରନ୍ତୁ false;
        buf_size -= (size_t)ଗଣନା_2;
        start += ଗଣନା_2;
    }
    ଯଦି (buf_size <= 1)
        ଫେରନ୍ତୁ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ଅଚଳ ସଂରଚନା xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ଯଦି (ଗଣନା_2 < 0 || (size_t) ଗଣନା_2 >= buf_size)
            ଫେରନ୍ତୁ false;
        buf_size -= (size_t)ଗଣନା_2;
        start += ଗଣନା_2;
        ଯଦି (option->layouts != XKB_LAYOUT_INVALID) {
            ଗଣନା_2 = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            ଯଦି (ଗଣନା_2 < 0 || (size_t) ଗଣନା_2 >= buf_size)
                ଫେରନ୍ତୁ false;
            buf_size -= (size_t)ଗଣନା_2;
            start += ଗଣନା_2;
        }
    }
    ଯଦି (buf_size == 0)
        ଫେରନ୍ତୁ false;
    *start = '\0';
    ଫେରନ୍ତୁ true;
}
